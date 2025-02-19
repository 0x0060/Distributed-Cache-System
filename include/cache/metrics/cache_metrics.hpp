#pragma once

#include <atomic>
#include <chrono>
#include <string>
#include <unordered_map>

namespace cache::metrics {

class CacheMetrics {
public:
    void recordHit();
    void recordMiss();
    void recordEviction();
    void recordLatency(std::chrono::nanoseconds latency);
    
    double getHitRatio() const;
    std::chrono::nanoseconds getAverageLatency() const;
    size_t getTotalOperations() const;
    
private:
    std::atomic<size_t> hits_{0};
    std::atomic<size_t> misses_{0};
    std::atomic<size_t> evictions_{0};
    std::atomic<size_t> total_latency_{0};
};

} // namespace cache::metrics