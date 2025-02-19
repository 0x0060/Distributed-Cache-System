# Distributed System API

## ClusterManager Class
Manages the distributed cache cluster.

### Methods

#### addNode()
```cpp
void addNode(std::unique_ptr<ClusterNode> node)
```
Adds a new node to the cluster.

#### removeNode()
```cpp
void removeNode(const std::string& address)
```
Removes a node from the cluster.

## ClusterNode Interface
Base interface for implementing cache nodes.

### Required Methods
- `isAlive()`
- `ping()`
- `getAddress()`