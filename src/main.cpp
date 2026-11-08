#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include "FacilityManager.h"

void displayMenu() {
    std::cout << "\n=== Smart Campus Navigation & Facility Analytics (Member A) ===\n";
    std::cout << "1. View all facilities\n";
    std::cout << "2. Search facility by ID\n";
    std::cout << "3. Search facility by Name\n";
    std::cout << "4. View facility category report\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice (1-5): ";
}

int main() {
    FacilityHashTable facilityTable;
    std::unordered_set<std::string> acceptedBuildings;

    // Load accepted buildings from data/buildings.txt
    std::ifstream bFile("data/buildings.txt");
    if (bFile.is_open()) {
        std::string line;
        while (std::getline(bFile, line)) {
            // Trim or ignore comment lines
            if (!line.empty() && line[0] != '#') {
                acceptedBuildings.insert(line);
            }
        }
        bFile.close();
    } else {
        std::cout << "Warning: Could not open data/buildings.txt. Using default building fallback list.\n";
        acceptedBuildings.insert("Library");
        acceptedBuildings.insert("Student Union");
        acceptedBuildings.insert("Science Building");
        acceptedBuildings.insert("Engineering Building");
        acceptedBuildings.insert("Business Building");
        acceptedBuildings.insert("Gym");
        acceptedBuildings.insert("Observatory");
    }

    // Load facilities from data/facilities.txt
    facilityTable.loadFacilities("data/facilities.txt", acceptedBuildings);

    int choice = 0;
    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number between 1 and 5.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                facilityTable.printAllFacilities();
                break;
            }
            case 2: {
                std::string searchID;
                std::cout << "Enter Facility ID (e.g., F001): ";
                std::cin >> searchID;
                Facility* found = facilityTable.searchByID(searchID);
                if (found) {
                    std::cout << "\nFound Facility:\n"
                              << "ID: " << found->id << "\n"
                              << "Name: " << found->name << "\n"
                              << "Building: " << found->building << "\n"
                              << "Category: " << found->category << "\n";
                } else {
                    std::cout << "\nFacility ID not found.\n";
                }
                break;
            }
            case 3: {
                std::string searchName;
                std::cout << "Enter Facility Name (e.g., Main Reading Room): ";
                std::cin.ignore();
                std::getline(std::cin, searchName);
                facilityTable.searchByName(searchName);
                break;
            }
            case 4: {
                facilityTable.printCategoryReport();
                break;
            }
            case 5:
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default:
                std::cout << "Out of range. Please choose a number between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}
