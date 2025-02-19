# Getting Started

## Quick Start

1. Include the necessary headers:
```cpp
#include "cache/core/cache_entry.hpp"
#include "cache/distributed/cluster_manager.hpp"
```

2. Initialize the cache system:
```cpp
auto metrics = std::make_shared<cache::metrics::CacheMetrics>();
cache::distributed::ClusterManager clusterManager;
```

3. Configure nodes:
```cpp
clusterManager.addNode(std::make_unique<TestNode>("node1:8080"));
```

## Basic Operations

### Storing Data
```cpp
cache.put("key", ComplexValue("value"));
```

### Retrieving Data
```cpp
auto value = cache.get("key");
```