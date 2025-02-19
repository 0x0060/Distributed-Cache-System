#include "cache/core/cache_entry.hpp"
#include "cache/core/cache_policy.hpp"
#include "cache/metrics/cache_metrics.hpp"
#include "cache/distributed/cluster_manager.hpp"
#include "cache/serialization/serializer.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

class ComplexValue {
public:
    ComplexValue(std::string data) : data_(std::move(data)) {}
    const std::string& getData() const { return data_; }

private:
    std::string data_;
};

class TestNode : public cache::distributed::ClusterNode {
public:
    TestNode(std::string address) : address_(std::move(address)) {}
    bool isAlive() const override { return true; }
    void ping() override {}
    std::string getAddress() const override { return address_; }

private:
    std::string address_;
};

int main() {
    try {
        // Initialize metrics
        auto metrics = std::make_shared<cache::metrics::CacheMetrics>();
        std::cout << "Initialized cache metrics system\n";


        // Example Usage
        // Initialize cluster manager and add nodes
        cache::distributed::ClusterManager clusterManager;
        clusterManager.addNode(std::make_unique<TestNode>("node1:8080"));
        clusterManager.addNode(std::make_unique<TestNode>("node2:8080"));
        clusterManager.addNode(std::make_unique<TestNode>("node3:8080"));
        
        std::cout << "Cluster initialized with " 
                  << clusterManager.getActiveNodes().size() 
                  << " nodes\n";

        // Initialize serializer
        auto serializer = std::make_shared<cache::serialization::JsonSerializer<ComplexValue>>();

        // Create cache policies
        auto lruPolicy = std::make_shared<cache::core::LRUPolicy<std::string, ComplexValue>>();
        
        // Simulate cache operations
        std::vector<std::thread> workers;
        for (int i = 0; i < 5; ++i) {
            workers.emplace_back([i, &metrics]() {
                for (int j = 0; j < 100; ++j) {
                    auto start = std::chrono::high_resolution_clock::now();
                    
                    // Simulate cache operation
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    
                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
                    
                    metrics->recordLatency(duration);
                    if (j % 3 == 0) metrics->recordHit();
                    else metrics->recordMiss();
                }
                std::cout << "Worker " << i << " completed operations\n";
            });
        }

        // Wait for all workers to complete
        for (auto& worker : workers) {
            worker.join();
        }

        // Print final metrics
        std::cout << "\nCache Performance Metrics:\n";
        std::cout << "Hit Ratio: " << metrics->getHitRatio() * 100 << "%\n";
        std::cout << "Average Latency: " 
                  << std::chrono::duration_cast<std::chrono::microseconds>(metrics->getAverageLatency()).count() 
                  << " microseconds\n";
        std::cout << "Total Operations: " << metrics->getTotalOperations() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}