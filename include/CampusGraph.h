#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <vector>
#include <string>

class CampusGraph {
    private:
    std::vector<std::string> buildings;
    std::vector<std::vector<int>> adjacencyList;
    int edgeCount = 0;

    public:
    void addBuilding(std::string name);
    void addConnection(std::string building1, std::string building2);
    void displayAdjacencyList();
    void displayReachableBuildings(std::string startBuilding);
    void displayShortestPath(std::string startBuilding, std::string endBuilding);
    void displayConnectivityReport();
};

#endif