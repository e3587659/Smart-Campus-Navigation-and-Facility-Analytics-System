#include "CampusGraph.h"
#include "FileHelp.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <cctype>
#include <string>

static std::string trimString(std::string name) {
    while (!name.empty() && std::isspace(static_cast<unsigned char>(name[0]))) {
        name.erase(0, 1);
    }

    while (!name.empty() && std::isspace(static_cast<unsigned char>(name[name.size() - 1]))) {
        name.pop_back();
    }

    return name;
}

static bool areNamesEqual(std::string name1, std::string name2) {
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

int CampusGraph::findIndex(const std::string& name) const {
    for (size_t i = 0; i < buildings.size(); i++) {
        if (areNamesEqual(buildings[i], name)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

std::string CampusGraph::addBuilding(const std::string& rawName) {
    std::string name = trimString(rawName);

    if (name.empty()) {
        return "empty building name";
    }

    if (findIndex(name) != -1) {
        return "duplicate building '" + name + "'";
    }

    buildings.push_back(name);
    adjacencyList.emplace_back();
    return "";
}

std::string CampusGraph::addConnection(const std::string& building1, const std::string& building2) {
    if (trimString(building1).empty() || trimString(building2).empty()) {
        return "empty required field";
    }

    int index1 = findIndex(building1);
    int index2 = findIndex(building2);

    if (index1 == -1) {
        return "unknown building '" + trimString(building1) + "'";
    }
    if (index2 == -1) {
        return "unknown building '" + trimString(building2) + "'";
    }

    if (index1 == index2) {
        return "self-connection '" + buildings[index1] + "'";
    }

    for (int neighbor : adjacencyList[index1]) {
        if (neighbor == index2) {
            return "duplicate connection " + buildings[index1] + " - " + buildings[index2];
        }
    }

    adjacencyList[index1].push_back(index2);
    adjacencyList[index2].push_back(index1);
    edgeCount++;
    return "";
}

void CampusGraph::loadBuildings(const std::string& path, const std::string& fileLabel) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "Error: cannot open " << path << ". Treating it as empty.\n";
        std::cout << "Loaded " << fileLabel << ": 0 accepted, 0 rejected\n";
        return;
    }

    int accepted = 0, rejected = 0, lineNumber = 0;
    std::string line;
    while (std::getline(in, line)) {
        lineNumber++;
        std::string t = trim(line);
        if (isSkippableLine(t)) continue;

        std::string reason;
        if (splitPipe(t).size() != 1) {
            reason = "expected 1 field but found " + std::to_string(splitPipe(t).size());
        } else {
            reason = addBuilding(t);
        }

        if (reason.empty()) {
            accepted++;
        } else {
            printWarning(fileLabel, lineNumber, reason);
            rejected++;
        }
    }
    std::cout << "Loaded " << fileLabel << ": " << accepted << " accepted, "
              << rejected << " rejected\n";
}

void CampusGraph::loadConnections(const std::string& path, const std::string& fileLabel) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "Error: cannot open " << path << ". Treating it as empty.\n";
        std::cout << "Loaded " << fileLabel << ": 0 accepted, 0 rejected\n";
        return;
    }

    int accepted = 0, rejected = 0, lineNumber = 0;
    std::string line;
    while (std::getline(in, line)) {
        lineNumber++;
        std::string t = trim(line);
        if (isSkippableLine(t)) continue;

        std::vector<std::string> f = splitPipe(t);
        std::string reason;
        if (f.size() != 2) {
            reason = "expected 2 fields but found " + std::to_string(f.size());
        } else {
            reason = addConnection(f[0], f[1]);
        }

        if (reason.empty()) {
            accepted++;
        } else {
            printWarning(fileLabel, lineNumber, reason);
            rejected++;
        }
    }
    std::cout << "Loaded " << fileLabel << ": " << accepted << " accepted, "
              << rejected << " rejected\n";
}

void CampusGraph::displayAdjacencyList() const {
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

void CampusGraph::displayCountsAndDegrees() const {
    if (buildings.empty()) {
        std::cout << "No buildings are loaded.\nTotal buildings: 0\nTotal connections: 0\n";
        return;
    }
    std::cout << "Total buildings: " << buildings.size() << "\n";
    std::cout << "Total connections: " << edgeCount << "\n";
    std::cout << "Degree of each building:\n";
    for (size_t i = 0; i < buildings.size(); i++) {
        std::cout << "  " << buildings[i] << ": " << adjacencyList[i].size() << "\n";
    }
}

void CampusGraph::displayReachableBuildings(const std::string& startBuilding) const {
    int startIndex = findIndex(startBuilding);

    if (startIndex == -1) {
        std::cout << "Error: unknown building '" << trimString(startBuilding) << "'.\n";
        return;
    }

    std::vector<bool> visited(buildings.size(), false);
    std::vector<int> distance(buildings.size(), -1);
    int reachableCount = 0;
    std::queue<int> buildingQueue;

    buildingQueue.push(startIndex);
    visited[startIndex] = true;
    distance[startIndex] = 0;

    std::cout << "Reachable buildings from " << buildings[startIndex] << " (in BFS visit order):\n";

    while (!buildingQueue.empty()) {
        int currentBuilding = buildingQueue.front();
        buildingQueue.pop();
        reachableCount++;

        std::cout << buildings[currentBuilding] << " (Distance: " << distance[currentBuilding] << ")\n";

        for (int neighbor : adjacencyList[currentBuilding]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[currentBuilding] + 1;
                buildingQueue.push(neighbor);
            }
        }
    }
    std::cout << "Reachable: " << reachableCount
              << " of " << buildings.size() << " buildings\n";
}

void CampusGraph::displayShortestPath(const std::string& startBuilding, const std::string& endBuilding) const {
    int startIndex = findIndex(startBuilding);
    int endIndex = findIndex(endBuilding);

    if (startIndex == -1 || endIndex == -1) {
        std::cout << "Error: One or both buildings not found.\n";
        return;
    }

    std::vector<bool> visited(buildings.size(), false);
    std::vector<int> parent(buildings.size(), -1);
    std::queue<int> buildingQueue;

    buildingQueue.push(startIndex);
    visited[startIndex] = true;

    while (!buildingQueue.empty()) {
        int currentBuilding = buildingQueue.front();
        buildingQueue.pop();

        if (currentBuilding == endIndex) {
            break;
        }

        for (int neighbor : adjacencyList[currentBuilding]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = currentBuilding;
                buildingQueue.push(neighbor);
            }
        }
    }

    if (!visited[endIndex]) {
        std::cout << "No path found between " << startBuilding << " and " << endBuilding << ".\n";
        return;
    }

    std::vector<int> path;
    for (int at = endIndex; at != -1; at = parent[at]) {
        path.push_back(at);
    }

    std::cout << "Shortest path from " << startBuilding << " to " << endBuilding
              << " (Distance: " << (path.size() - 1) << " steps):\n";

    for (int i = static_cast<int>(path.size()) - 1; i >= 0; i--) {
        std::cout << buildings[path[i]] << "\n";
    }

    std::cout << "Number of connections: " << path.size() - 1 << "\n";
}

void CampusGraph::displayConnectivityReport() const {
    std::cout << "=== Campus Graph Connectivity Report ===\n";
    std::cout << "Total buildings: " << buildings.size() << "\n";
    std::cout << "Total connections: " << edgeCount << "\n\n";

    if (buildings.empty()) {
        std::cout << "The graph is empty.\n";
        return;
    }

    std::cout << "Building Degrees:\n";
    int maxDegree = 0;
    for (size_t i = 0; i < buildings.size(); i++) {
        int degree = static_cast<int>(adjacencyList[i].size());
        std::cout << "- " << buildings[i] << ": " << degree << " connection(s)\n";
        if (degree > maxDegree) {
            maxDegree = degree;
        }
    }
    std::cout << "\n";

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

    std::cout << "Building(s) with the highest degree (" << maxDegree << "):\n";
    for (size_t i = 0; i < buildings.size(); i++) {
        if (static_cast<int>(adjacencyList[i].size()) == maxDegree) {
            std::cout << "- " << buildings[i] << "\n";
        }
    }
    std::cout << "\n";

    std::vector<bool> visited(buildings.size(), false);
    int componentCount = 0;

    for (size_t i = 0; i < buildings.size(); i++) {
        if (!visited[i]) {
            componentCount++;

            std::queue<int> buildingQueue;
            buildingQueue.push(static_cast<int>(i));
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

    std::cout << "Connected Components: " << componentCount << "\n";
    if (componentCount == 1) {
        std::cout << "Status: The campus graph is fully connected.\n";
    } else {
        std::cout << "Status: The campus graph is disconnected.\n";
    }
    std::cout << "========================================\n";
}
