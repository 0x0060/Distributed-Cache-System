# Core API Documentation

## CacheEntry Class
The fundamental building block of the caching system.

### Template Parameters
- `K`: Key type
- `V`: Value type

### Methods

#### Constructor
```cpp
CacheEntry(V value, TimePoint expiry)
```
Creates a new cache entry with the specified value and expiration time.

#### getValue()
Returns the stored value.

#### getExpiry()
Returns the expiration timestamp.

## CachePolicy Class
Base class for implementing cache eviction policies.

### Available Policies
- LRUPolicy
- LFUPolicy

### Interface Methods
- `shouldEvict()`
- `onInsert()`
- `onAccess()`