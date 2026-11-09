#include "FacilityHashTable.h"
#include "FileHelp.h"

FacilityHashTable::FacilityHashTable(int cap)
    : capacity(cap > 0 ? cap : DEFAULT_CAPACITY), size(0), collisions(0) {
    buckets.resize(capacity);
}

int FacilityHashTable::hash(const std::string& normalizedKey) const {
    unsigned long h = 5381;
    for (char c : normalizedKey) {
        h = h * 33 + static_cast<unsigned char>(c);
    }
    return static_cast<int>(h % static_cast<unsigned long>(capacity));
}

bool FacilityHashTable::insert(const std::string& id, int index) {
    std::string key = toUpperStr(trim(id));
    std::list<Entry>& bucket = buckets[hash(key)];

    for (const Entry& e : bucket) {
        if (e.key == key) {
            return false;
        }
    }
