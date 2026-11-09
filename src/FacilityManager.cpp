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

  int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;
    std::string line;

    while (std::getline(in, line)) {
        lineNumber++;
        std::string t = trim(line);
        if (isSkippableLine(t)) continue;

        std::vector<std::string> f = splitPipe(t);
        std::string reason;

        if (f.size() != FACILITY_FIELDS) {
            reason = "expected 4 fields but found " + std::to_string(f.size());
        } else if (f[0].empty() || f[1].empty() || f[2].empty() || f[3].empty()) {
            reason = "empty required field";
        } else if (!isValidIdFormat(toUpperStr(f[0]))) {
            reason = "invalid facility ID format '" + f[0] + "'";
        } else if (idTable.find(f[0]) != -1) {
            reason = "duplicate facility ID '" + toUpperStr(f[0]) + "'";
        }

        std::string officialBuilding;
        if (reason.empty()) {
            for (const std::string& b : buildings) {
                if (equalsIgnoreCase(b, f[2])) { officialBuilding = b; break; }
            }
            if (officialBuilding.empty()) {
                reason = "unknown building '" + f[2] + "'";
            }
        }
