#pragma once

#include <memory>
#include <string>

namespace cache::core {

template<typename K, typename V>
class CachePolicy {
public:
    virtual ~CachePolicy() = default;
    virtual bool shouldEvict(const K& key, const V& value) = 0;
    virtual void onInsert(const K& key, const V& value) = 0;
    virtual void onAccess(const K& key) = 0;
    virtual std::string getName() const = 0;
};

template<typename K, typename V>
class LRUPolicy : public CachePolicy<K, V> {
public:
    bool shouldEvict(const K& key, const V& value) override {
        std::lock_guard<std::mutex> lock(mutex_);
        return access_list_.size() >= max_size_;
    }

    void onInsert(const K& key, const V& value) override {
        std::lock_guard<std::mutex> lock(mutex_);
        access_list_.push_front(key);
        key_map_[key] = access_list_.begin();
        
        if (access_list_.size() > max_size_) {
            auto last = access_list_.back();
            access_list_.pop_back();
            key_map_.erase(last);
        }
    }

    void onAccess(const K& key) override {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = key_map_.find(key);
        if (it != key_map_.end()) {
            access_list_.erase(it->second);
            access_list_.push_front(key);
            it->second = access_list_.begin();
        }
    }

    std::string getName() const override {
        return "LRU";
    }

private:
    std::list<K> access_list_;
    std::unordered_map<K, typename std::list<K>::iterator> key_map_;
    std::mutex mutex_;
    const size_t max_size_ = 1000;
};

template<typename K, typename V>
class LFUPolicy : public CachePolicy<K, V> {
public:
    bool shouldEvict(const K& key, const V& value) override {
        std::lock_guard<std::mutex> lock(mutex_);
        return frequency_map_.size() >= max_size_;
    }

    void onInsert(const K& key, const V& value) override {
        std::lock_guard<std::mutex> lock(mutex_);
        frequency_map_[key] = 1;
        min_frequency_ = 1;
        
        if (!freq_lists_[1].contains(key)) {
            freq_lists_[1].insert(key);
        }
        
        if (frequency_map_.size() > max_size_) {
            auto it = freq_lists_[min_frequency_].begin();
            K evict_key = *it;
            freq_lists_[min_frequency_].erase(it);
            frequency_map_.erase(evict_key);
        }
    }

    void onAccess(const K& key) override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (frequency_map_.contains(key)) {
            size_t freq = frequency_map_[key];
            frequency_map_[key]++;
            
            freq_lists_[freq].erase(key);
            if (freq_lists_[freq].empty() && freq == min_frequency_) {
                min_frequency_++;
            }
            
            freq_lists_[freq + 1].insert(key);
        }
    }

    std::string getName() const override {
        return "LFU";
    }

private:
    std::unordered_map<K, size_t> frequency_map_;
    std::unordered_map<size_t, std::unordered_set<K>> freq_lists_;
    std::mutex mutex_;
    size_t min_frequency_ = 0;
    const size_t max_size_ = 1000;
};

} // namespace cache::core