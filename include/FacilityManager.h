#ifndef FACILITY_MANAGER_H
#define FACILITY_MANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include <iomanip>

struct Facility {
    std::string id;
    std::string name;
    std::string building;
    std::string category;
};

class FacilityHashTable {
private:
    static const int TABLE_SIZE = 101;
    std::list<Facility> table[TABLE_SIZE];
    std::unordered_set<std::string> uniqueIDs;
    
    int totalEntries = 0;
    int collisionCount = 0;

    unsigned long hashFunction(const std::string& key) const {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash % TABLE_SIZE;
    }

    std::string toUpper(std::string str) const {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    std::string trim(const std::string& str) const {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    bool isValidIDFormat(const std::string& id) const {
        if (id.length() != 4) return false;
        if (std::toupper(id[0]) != 'F') return false;
        for (int i = 1; i < 4; ++i) {
            if (!std::isdigit(id[i])) return false;
        }
        return true;
    }

public:
    bool insert(const Facility& facility, int lineNumber, const std::string& filename, const std::unordered_set<std::string>& acceptedBuildings) {
        std::string upperID = toUpper(facility.id);

        if (!isValidIDFormat(upperID)) {
            std::cout << "Warning [" << filename << ":" << lineNumber << "]: Invalid ID format (" << facility.id << "). Record rejected.\n";
            return false;
        }

        if (uniqueIDs.find(upperID) != uniqueIDs.end()) {
            std::cout << "Warning [" << filename << ":" << lineNumber << "]: Duplicate facility ID (" << facility.id << "). Record rejected.\n";
            return false;
        }

        if (facility.name.empty() || facility.category.empty()) {
            std::cout << "Warning [" << filename << ":" << lineNumber << "]: Empty facility name or category. Record rejected.\n";
            return false;
        }

        std::string upperBuilding = toUpper(facility.building);
        bool buildingExists = false;
        for (const auto& b : acceptedBuildings) {
            if (toUpper(b) == upperBuilding) {
                buildingExists = true;
                break;
            }
        }

        if (!buildingExists) {
            std::cout << "Warning [" << filename << ":" << lineNumber << "]: Unknown building reference (" << facility.building << "). Record rejected.\n";
            return false;
        }

        int index = hashFunction(upperID);
        if (!table[index].empty()) {
            collisionCount++;
        }

        table[index].push_back(facility);
        uniqueIDs.insert(upperID);
        totalEntries++;
        return true;
    }

    bool loadFacilities(const std::string& filename, const std::unordered_set<std::string>& acceptedBuildings) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file " << filename << "\n";
            return false;
        }

        std::string line;
        int lineNumber = 0;
        int acceptedCount = 0;

        while (std::getline(file, line)) {
            lineNumber++;
            std::string trimmedLine = trim(line);
            if (trimmedLine.empty() || trimmedLine[0] == '#') {
                continue;
            }

            std::stringstream ss(trimmedLine);
            std::string id, name, building, category;

            if (std::getline(ss, id, '|') &&
                std::getline(ss, name, '|') &&
                std::getline(ss, building, '|') &&
                std::getline(ss, category, '|')) {
                
                std::string extra;
                if (std::getline(ss, extra)) {
                    std::cout << "Warning [" << filename << ":" << lineNumber << "]: Too many fields. Record rejected.\n";
                    continue;
                }

                Facility fac;
                fac.id = trim(id);
                fac.name = trim(name);
                fac.building = trim(building);
                fac.category = trim(category);

                if (insert(fac, lineNumber, filename, acceptedBuildings)) {
                    acceptedCount++;
                }
            } else {
                std::cout << "Warning [" << filename << ":" << lineNumber << "]: Incorrect field count / delimiter error. Record rejected.\n";
            }
        }

        file.close();
        std::cout << "Loaded " << acceptedCount << " facilities successfully from " << filename << ".\n";
        return true;
    }

    void printAllFacilities() const {
        std::cout << "\n=== All Facilities ===\n";
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (const auto& fac : table[i]) {
                std::cout << fac.id << " | " << fac.name << " | " << fac.building << " | " << fac.category << "\n";
            }
        }
    }

    Facility* searchByID(std::string id) {
        std::string upperID = toUpper(id);
        int index = hashFunction(upperID);
        for (auto& fac : table[index]) {
            if (toUpper(fac.id) == upperID) {
                return &fac;
            }
        }
        return nullptr;
    }

    void searchByName(const std::string& name) const {
        std::vector<Facility> matches;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (const auto& fac : table[i]) {
                if (fac.name == name) {
                    matches.push_back(fac);
                }
            }
        }

        std::cout << "\nSearching \"" << name << "\" returns:\n";
        for (const auto& fac : matches) {
            std::cout << fac.id << " | " << fac.name << " | " << fac.building << " | " << fac.category << "\n";
        }
        std::cout << "Matches: " << matches.size() << "\n";
    }

    void printCategoryReport() const {
        std::vector<std::string> categories;
        std::vector<int> counts;

        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (const auto& fac : table[i]) {
                bool found = false;
                for (size_t j = 0; j < categories.size(); ++j) {
                    if (categories[j] == fac.category) {
                        counts[j]++;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    categories.push_back(fac.category);
                    counts.push_back(1);
                }
            }
        }

        std::cout << "\n=== Facility Category Report ===\n";
        std::cout << "--------------------------------------------------------\n";
        std::cout << std::left << std::setw(25) << "Category" 
                  << std::left << std::setw(12) << "Count" 
                  << "Percentage\n";
        std::cout << "--------------------------------------------------------\n";

        for (size_t i = 0; i < categories.size(); ++i) {
            double percentage = (totalEntries > 0) ? (static_cast<double>(counts[i]) / totalEntries) * 100.0 : 0.0;
            std::cout << std::left << std::setw(25) << categories[i]
                      << std::left << std::setw(12) << counts[i]
                      << std::fixed << std::setprecision(2) << percentage << "%\n";
        }
        std::cout << "--------------------------------------------------------\n";
        std::cout << "Total Valid Facilities: " << totalEntries << "\n";
        std::cout << "Hash Table Collisions: " << collisionCount << "\n";
    }
};

#endif
