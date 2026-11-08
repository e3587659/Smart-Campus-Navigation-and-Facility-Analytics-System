#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include "FacilityManager.h"
#include "CampusGraph.h"

void displayMenu() {
    std::cout << "\n=== Smart Campus Navigation & Facility Analytics (Member A) ===\n";
    std::cout << "1. View all facilities\n";
    std::cout << "2. Search facility by ID\n";
    std::cout << "3. Search facility by Name\n";
    std::cout << "4. View facility category report\n";
    std::cout << "5. Display campus adjacency list\n";
    std::cout << "6. Find reachable buildings (BFS)\n";
    std::cout << "7. Find shortest path between buildings\n";
    std::cout << "8. Display graph connectivity report\n";
    std::cout << "9. Exit\n";
    std::cout << "Enter your choice (1-9): ";
}

int main() {
    FacilityHashTable facilityTable;
    CampusGraph campusGraph;
    std::unordered_set<std::string> acceptedBuildings;

    // Load accepted buildings from data/buildings.txt
    std::ifstream bFile("data/buildings.txt");
    if (bFile.is_open()) {
        std::string line;
        while (std::getline(bFile, line)) {
            // Trim or ignore comment lines
            if (!line.empty() && line[0] != '#') {
                acceptedBuildings.insert(line);
                campusGraph.addBuilding(line);
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
        //Adds fallback buildings to the graph//
        for (const std::string& building : acceptedBuildings) {
            campusGraph.addBuilding(building);
        }
    }

    // Load facilities from data/facilities.txt
    facilityTable.loadFacilities("data/facilities.txt", acceptedBuildings);

    //Loads connections between campus buildings//
    std::ifstream cFile("data/connections.txt");

    if (cFile.is_open()) {
        std::string line;

        while (std::getline(cFile, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            size_t separator = line.find('|');

            if (separator != std::string::npos) {
                std::string building1 = line.substr(0, separator);
                std::string building2 = line.substr(separator + 1);

                campusGraph.addConnection(building1, building2);
            }
        }

        cFile.close();
    } else {
        std::cout << "Warning: Could not open data/connections.txt.\n";
    }

    int choice = 0;
    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number between 1 and 9.\n";
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
            case 5: {
                //Displays the campus adjacency list//
                campusGraph.displayAdjacencyList();
                break;
            }
            case 6: {
                //Finds buildings reachable from a starting building//
                std::string startBuilding;

                std::cout << "Enter starting building: ";
                std::getline(std::cin >> std::ws, startBuilding);

                campusGraph.displayReachableBuildings(startBuilding);
                break;
            }
            case 7: {
                //Finds the shortest path between two buildings//
                std::string startBuilding;
                std::string endBuilding;

                std::cout << "Enter starting building: ";
                std::getline(std::cin >> std::ws, startBuilding);

                std::cout << "Enter destination building: ";
                std::getline(std::cin, endBuilding);

                campusGraph.displayShortestPath(startBuilding, endBuilding);
                break;
            }
            case 8: {
                //Displays the campus graph connectivity report//
                campusGraph.displayConnectivityReport();
                break;
            }
            case 9:
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default:
                std::cout << "Out of range. Please choose a number between 1 and 9.\n";
        }
    } while (choice != 9);

    return 0;
}
