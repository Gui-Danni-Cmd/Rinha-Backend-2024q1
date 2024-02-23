#include <iostream>
#include <map>
#include <string>

template <typename KeyType, typename ValueType>
class Cache {
private:
    std::map<KeyType, ValueType> cacheMap;
public:
    void insert(const KeyType& key, const ValueType& value) {
        cacheMap[key] = value;
    }

    bool contains(const KeyType& key) const {
        return cacheMap.find(key) != cacheMap.end();
    }

    const ValueType& get(const KeyType& key) const {
        return cacheMap.at(key);
    }

    void remove(const KeyType& key) {
        cacheMap.erase(key);
    }

    void clear() {
        cacheMap.clear();
    }
};