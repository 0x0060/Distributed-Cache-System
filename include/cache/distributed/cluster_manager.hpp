#pragma once

#include <string>
#include <vector>
#include <memory>
#include "cache/core/cache_entry.hpp"

namespace cache::distributed {

class ClusterNode {
public:
    virtual ~ClusterNode() = default;
    virtual bool isAlive() const = 0;
    virtual void ping() = 0;
    virtual std::string getAddress() const = 0;
};

class ClusterManager {
public:
    void addNode(std::unique_ptr<ClusterNode> node);
    void removeNode(const std::string& address);
    std::vector<ClusterNode*> getActiveNodes() const;
    
private:
    std::vector<std::unique_ptr<ClusterNode>> nodes_;
};

} // namespace cache::distributed