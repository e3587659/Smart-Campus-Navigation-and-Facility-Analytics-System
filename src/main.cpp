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
