#include "CampusGraph.h"
#include <iostream>
#include <queue>
#include <vector>
#include <cctype>
#include <string>

//Removes spaces from the beginning and end of a name//
std::string trimString(std::string name) {
    while (!name.empty() && std::isspace(static_cast<unsigned char>(name[0]))) {
        name.erase(0, 1);
    }

    while (!name.empty() && std::isspace(static_cast<unsigned char>(name[name.size() - 1]))) {
        name.pop_back();
    }

    return name;
}

//Compares building names without capitalization differences//
bool areNamesEqual(std::string name1, std::string name2) {
    name1 = trimString(name1);
    name2 = trimString(name2);

    if (name1.size() != name2.size()) {
        return false;
    }

    for (size_t i = 0; i < name1.size(); i++) {
        if (std::tolower(static_cast<unsigned char>(name1[i])) !=
            std::tolower(static_cast<unsigned char>(name2[i]))) {
            return false;
        }
    }

    return true;
}

//Adds a building to the graph//
void CampusGraph::addBuilding(std::string name) {
    name = trimString(name);

    //Rejects empty building names//
    if (name.empty()) {
        std::cout << "Error: Building name cannot be empty.\n";
        return;
    }

    //Prevents duplicate building names//
    for (const std::string& building : buildings) {
        if (areNamesEqual(building, name)) {
            std::cout << "Error: Building already exists.\n";
            return;
        }
    }

    buildings.push_back(name);
    adjacencyList.emplace_back();
}

//Connects two buildings//
void CampusGraph::addConnection(std::string building1, std::string building2) {
    int index1 = -1;
    int index2 = -1;

    //Finds indices for both buildings//
//Finds indices for both buildings//
for (size_t i = 0; i < buildings.size(); i++) {
    if (areNamesEqual(buildings[i], building1)) {
        index1 = i;
    }

    if (areNamesEqual(buildings[i], building2)) {
        index2 = i;
    }
}

    //Stops if either building does not exist//
    if (index1 == -1 || index2 == -1) {
        return;
    }

    if(index1 == index2) {
        return;
    }

    //Prevents duplicate connections//
    for(int neighbor : adjacencyList[index1]){
        if (neighbor == index2){
            return;
        }
    }

    //Adds the connection in both directions//
    adjacencyList[index1].push_back(index2);
    adjacencyList[index2].push_back(index1);
    edgeCount++;
}

//Displays buildings and their connections//
void CampusGraph::displayAdjacencyList() {
    if (buildings.empty()) {
        std::cout << "No buildings loaded in the campus graph.\n";
        return;
    }

    for (size_t i = 0; i < buildings.size(); i++) {
        std::cout << buildings[i] << ": ";
        
        if (adjacencyList[i].empty()) {
            std::cout << "(no connections)";
        } else {
            for (size_t j = 0; j < adjacencyList[i].size(); j++) {
                int neighborIndex = adjacencyList[i][j];
                std::cout << buildings[neighborIndex];
                
                if (j + 1 < adjacencyList[i].size()) {
                    std::cout << ", ";
                }
            }
        }
        std::cout << "\n";
    }
}

//Finds all buildings reachable from the starting building//
void CampusGraph::displayReachableBuildings(std::string startBuilding) {
    int startIndex = -1;
    for (size_t i = 0; i < buildings.size(); i++) {
        if (areNamesEqual(buildings[i], startBuilding)) {
            startIndex = i;
            break;
        }
    }

    if (startIndex == -1) {
        std::cout << "Error: Building not found.\n";
        return;
    }

    std::vector<bool> visited(buildings.size(), false);
    //Tracks shortest distance//
    std::vector<int> distance(buildings.size(), -1);
    //Counts reachable buildings//
    int reachableCount = 0;
    std::queue<int> buildingQueue;

    buildingQueue.push(startIndex);
    visited[startIndex] = true;
    distance[startIndex] = 0;

    std::cout << "Reachable buildings from " << startBuilding << ":\n";

    //BFS traversal loop//
    while (!buildingQueue.empty()) {
        int currentBuilding = buildingQueue.front();
        buildingQueue.pop();
        reachableCount++;

        std::cout << buildings[currentBuilding] << " (Distance: " << distance[currentBuilding] << ")\n";

        //Visits connected buildings that have not been visited//
        for (int neighbor : adjacencyList[currentBuilding]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
              
                //Calculates distance to neighboring building//
                distance[neighbor] = distance[currentBuilding] + 1;
                buildingQueue.push(neighbor);
            }
        }
    }
    std::cout << "Reachable: " << reachableCount
          << " of " << buildings.size() << " buildings\n";
}
//Finds the shortest path between two buildings//
void CampusGraph::displayShortestPath(std::string startBuilding, std::string endBuilding) {
    int startIndex = -1;
    int endIndex = -1;

    //Finds indices for both start and end buildings//
    for (size_t i = 0; i < buildings.size(); i++) {
        if (areNamesEqual(buildings[i], startBuilding)) {
            startIndex = i;
        }
        if (areNamesEqual(buildings[i], endBuilding)) {
            endIndex = i;
        }
    }

    //Stops if either building does not exist//
    if (startIndex == -1 || endIndex == -1) {
        std::cout << "Error: One or both buildings not found.\n";
        return;
    }

    //Prepares BFS//
    std::vector<bool> visited(buildings.size(), false);
    std::vector<int> parent(buildings.size(), -1);
    std::queue<int> buildingQueue;
    
    buildingQueue.push(startIndex);
    visited[startIndex] = true;

     //BFS traversal loop//
    while (!buildingQueue.empty()) {
        int currentBuilding = buildingQueue.front();
        buildingQueue.pop();

        //Stops when destination is reached//
        if (currentBuilding == endIndex) {
            break;
        }

        for (int neighbor : adjacencyList[currentBuilding]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                //Tracks previous building//
                parent[neighbor] = currentBuilding;
                buildingQueue.push(neighbor);
            }
        }
    }

    //Checks if destination was reached//
    if (!visited[endIndex]) {
        std::cout << "No path found between " << startBuilding << " and " << endBuilding << ".\n";
        return;
    }

    //Tracks path backwards//
    std::vector<int> path;
    for (int at = endIndex; at != -1; at = parent[at]) {
        path.push_back(at);
    }

    //Displays shortest path//
    std::cout << "Shortest path from " << startBuilding << " to " << endBuilding
              << " (Distance: " << (path.size() - 1) << " steps):\n";

//Displays each building on a separate line//
for (int i = path.size() - 1; i >= 0; i--) {
    std::cout << buildings[path[i]] << "\n";
}

//Displays the number of connections//
std::cout << "Number of connections: " << path.size() - 1 << "\n";
}

//Displays campus graph connectivity report//
void CampusGraph::displayConnectivityReport() {
    //Displays graph totals//
    std::cout << "=== Campus Graph Connectivity Report ===\n";
    std::cout << "Total buildings: " << buildings.size() << "\n";
    std::cout << "Total connections: " << edgeCount << "\n\n";

    if (buildings.empty()) {
        std::cout << "The graph is empty.\n";
        return;
    }

    //Displays building degrees//
    std::cout << "Building Degrees:\n";
    int maxDegree = 0;
    for (size_t i = 0; i < buildings.size(); i++) {
        int degree = adjacencyList[i].size();
        std::cout << "- " << buildings[i] << ": " << degree << " connection(s)\n";
        if (degree > maxDegree) {
            maxDegree = degree;
        }
    }
    std::cout << "\n";

    //Finds isolated buildings//
    std::cout << "Isolated Buildings:\n";
    bool hasIsolated = false;
    for (size_t i = 0; i < buildings.size(); i++) {
        if (adjacencyList[i].empty()) {
            std::cout << "- " << buildings[i] << "\n";
            hasIsolated = true;
        }
    }
    if (!hasIsolated) {
        std::cout << "None (all buildings have at least one connection).\n";
    }
    std::cout << "\n";

    //Finds buildings with highest degree//
    std::cout << "Building(s) with the highest degree (" << maxDegree << "):\n";
    for (size_t i = 0; i < buildings.size(); i++) {
        if (adjacencyList[i].size() == maxDegree) {
            std::cout << "- " << buildings[i] << "\n";
        }
    }
    std::cout << "\n";

    //Counts connected components using BFS//
    std::vector<bool> visited(buildings.size(), false);
    int componentCount = 0;

    for (size_t i = 0; i < buildings.size(); i++) {
        if (!visited[i]) {
            componentCount++;

            //Visits buildings in current component//
            std::queue<int> buildingQueue;
            buildingQueue.push(i);
            visited[i] = true;

            while (!buildingQueue.empty()) {
                int currentBuilding = buildingQueue.front();
                buildingQueue.pop();

                for (int neighbor : adjacencyList[currentBuilding]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        buildingQueue.push(neighbor);
                    }
                }
            }
        }
    }

    //Displays connectivity status//
    std::cout << "Connected Components: " << componentCount << "\n";
    if (componentCount == 1) {
        std::cout << "Status: The campus graph is fully connected.\n";
    } else {
        std::cout << "Status: The campus graph is disconnected.\n";
    }
    std::cout << "========================================\n";
}