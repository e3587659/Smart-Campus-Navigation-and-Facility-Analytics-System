#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include "FacilityManager.h"

void displayMenu() {
    std::cout << "\n=== Smart Campus Navigation & Facility Analytics (Member A Module) ===\n";
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

std::ifstream bFile("data/buildings.txt");
    if (bFile.is_open()) {
        std::string line;
        while (std::getline(bFile, line)) {
            if (!line.empty() && line[0] != '#') {
                acceptedBuildings.insert(line);
            }
        }
        bFile.close();
    } else {
        acceptedBuildings.insert("Library");
        acceptedBuildings.insert("Student Union");
        acceptedBuildings.insert("Science Building");
        acceptedBuildings.insert("Engineering Building");
        acceptedBuildings.insert("Business Building");
        acceptedBuildings.insert("Gym");
        acceptedBuildings.insert("Observatory");
    }

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

