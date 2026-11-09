#ifndef FACILITY_HASH_TABLE_H
#define FACILITY_HASH_TABLE_H

#include <list>
#include <string>
#include <vector>

class FacilityHashTable {
public:
    static const int DEFAULT_CAPACITY = 101;

explicit FacilityHashTable(int capacity = DEFAULT_CAPACITY);

    bool insert(const std::string& id, int index);
    int find(const std::string& id) const;

    int getCapacity() const   { return capacity; }
    int getSize() const       { return size; }
    int getCollisions() const { return collisions; }

    int hash(const std::string& normalizedKey) const;

private:
    struct Entry {
        std::string key;
        int index;
    };
