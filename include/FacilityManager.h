#ifndef FACILITY_MANAGER_H
#define FACILITY_MANAGER_H

#include "FacilityHashTable.h"
#include <string>
#include <vector>

struct Facility {
    std::string id;
    std::string name;
    std::string building;
    std::string category;
};

class FacilityManager {
public:
    void load(const std::string& path, const std::string& fileLabel,
              const std::vector<std::string>& buildings);

    void displayAll() const;
    void searchById(const std::string& input) const;

    static bool isValidIdFormat(const std::string& id);

    int count() const { return static_cast<int>(facilities.size()); }

private:
    std::vector<Facility> facilities;
    FacilityHashTable idTable;

    void printFacility(const Facility& f) const;
    void printSummary(const std::string& fileLabel, int accepted, int rejected) const;
};

#endif
