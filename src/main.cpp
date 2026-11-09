#include <iostream>
#include <string>
#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "FacilityManager.h"
#include "FileHelp.h"

static const int MENU_MIN = 0;
static const int MENU_MAX = 9;

static void displayMenu() {
    std::cout << "\n===== Smart Campus Navigation (Milestone 1) =====\n"
              << "1. View all facilities\n"
              << "2. Search facility by ID\n"
              << "3. View campus graph\n"
              << "4. Display graph counts and degrees\n"
              << "5. Find reachable buildings (BFS)\n"
              << "6. Add department\n"
              << "7. Search department\n"
              << "8. Display department hierarchy\n"
              << "9. Run preorder traversal\n"
              << "0. Exit\n"
              << "Enter choice: ";
}

static int readMenuChoice() {
    std::string line;
    if (!std::getline(std::cin, line)) return -2;
    line = trim(line);
    if (line.empty()) {
        std::cout << "Error: please enter a number from " << MENU_MIN << " to " << MENU_MAX << ".\n";
        return -1;
    }
    for (char c : line) {
        if (c < '0' || c > '9') {
            std::cout << "Error: '" << line << "' is not a number. Choose "
                      << MENU_MIN << " to " << MENU_MAX << ".\n";
            return -1;
        }
    }
    if (line.size() > 2 || std::stoi(line) > MENU_MAX) {
        std::cout << "Error: " << line << " is out of range. Choose "
                  << MENU_MIN << " to " << MENU_MAX << ".\n";
        return -1;
    }
    return std::stoi(line);
}

static bool readName(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    if (!std::getline(std::cin, out)) out = "";
    out = trim(out);
    if (out.empty()) {
        std::cout << "Error: input cannot be blank. Returning to the menu.\n";
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    std::string dataDir = (argc > 1) ? argv[1] : "data";
    if (!dataDir.empty() && dataDir.back() == '/') dataDir.pop_back();

    CampusGraph campusGraph;
    FacilityManager facilities;
    DepartmentTree departments;

    campusGraph.loadBuildings(dataDir + "/buildings.txt", "buildings.txt");
    campusGraph.loadConnections(dataDir + "/connections.txt", "connections.txt");
    facilities.load(dataDir + "/facilities.txt", "facilities.txt", campusGraph.getBuildings());
    departments.loadFromFile(dataDir + "/departments.txt", "departments.txt");

    while (true) {
        displayMenu();
        int choice = readMenuChoice();
        if (choice == -2) {
            std::cout << "\nInput ended. Goodbye!\n";
            break;
        }
        if (choice == -1) continue;

        std::string name, parent;
        switch (choice) {
            case 1:
                facilities.displayAll();
                break;
            case 2:
                if (readName("Enter facility ID: ", name)) facilities.searchById(name);
                break;
            case 3:
                campusGraph.displayAdjacencyList();
                break;
            case 4:
                campusGraph.displayCountsAndDegrees();
                break;
            case 5:
                if (readName("Enter starting building: ", name))
                    campusGraph.displayReachableBuildings(name);
                break;
            case 6: {
                if (!readName("Enter new department name: ", name)) break;
                bool wasEmpty = departments.isEmpty();
                if (!wasEmpty && !readName("Enter parent department: ", parent)) break;
                std::string reason = departments.addDepartment(name, parent);
                if (reason.empty()) {
                    std::cout << "Added " << name
                              << (wasEmpty ? " as the root department." : " under " + parent + ".")
                              << " (memory only; departments.txt is not changed)\n";
                } else {
                    std::cout << "Error: could not add department: " << reason << ".\n";
                }
                break;
            }
            case 7:
                if (readName("Enter department name: ", name)) departments.displaySearchResult(name);
                break;
            case 8:
                departments.displayHierarchy();
                break;
            case 9:
                departments.preorderTraversal();
                break;
            case 0:
                std::cout << "Goodbye!\n";
                return 0;
        }
    }
    return 0;
}
