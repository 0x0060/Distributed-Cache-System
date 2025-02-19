#pragma once

#include <chrono>
#include <memory>
#include <future>
#include <variant>

namespace cache::core {

template<typename V>
class CacheEntry {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using RefreshPromise = std::shared_ptr<std::promise<void>>;
    using Statistics = std::tuple<size_t, size_t, double>; // hits, misses, hit_ratio

    CacheEntry(V value, TimePoint expiry)
        : value_(std::move(value))
        , expiry_(expiry)
        , creation_time_(Clock::now())
        , access_count_(0) {}

    const V& getValue() const { return value_; }
    void setValue(V value) { value_ = std::move(value); }
    
    TimePoint getExpiry() const { return expiry_; }
    void setExpiry(TimePoint expiry) { expiry_ = expiry; }
    
    void incrementAccessCount() { ++access_count_; }
    size_t getAccessCount() const { return access_count_; }

private:
    V value_;
    TimePoint expiry_;
    TimePoint creation_time_;
    size_t access_count_;
    RefreshPromise refresh_promise_;
};

} // namespace cache::core