#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>

class CampusGraph {
private:
    std::vector<std::string> buildings;
    std::vector<std::vector<int>> adjacencyList;
    int edgeCount = 0;

    int findIndex(const std::string& name) const;

public:
    void loadBuildings(const std::string& path, const std::string& fileLabel);
    void loadConnections(const std::string& path, const std::string& fileLabel);

    std::string addBuilding(const std::string& name);
    std::string addConnection(const std::string& building1, const std::string& building2);

    const std::vector<std::string>& getBuildings() const { return buildings; }

    void displayAdjacencyList() const;
    void displayCountsAndDegrees() const;
    void displayReachableBuildings(const std::string& start) const;

    void displayShortestPath(const std::string& startBuilding,
                             const std::string& endBuilding) const;
    void displayConnectivityReport() const;
};

#endif
