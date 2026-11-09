#include "FacilityManager.h"
#include "FileHelp.h"
#include <cctype>
#include <fstream>
#include <iostream>

static const size_t FACILITY_FIELDS = 4;

bool FacilityManager::isValidIdFormat(const std::string& id) {
    if (id.size() != 4 || id[0] != 'F') return false;
    for (size_t i = 1; i < id.size(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(id[i]))) return false;
    }
    return true;
}

void FacilityManager::printSummary(const std::string& fileLabel,
                                   int accepted, int rejected) const {
    std::cout << "Loaded " << fileLabel << ": " << accepted << " accepted, "
              << rejected << " rejected | Hash table capacity: "
              << idTable.getCapacity() << ", entries: " << idTable.getSize()
              << ", collisions: " << idTable.getCollisions() << "\n";
}

void FacilityManager::load(const std::string& path, const std::string& fileLabel,
                           const std::vector<std::string>& buildings) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "Error: cannot open " << path << ". Treating it as empty.\n";
        printSummary(fileLabel, 0, 0);
        return;
    }


