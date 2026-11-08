#include "CampusGraph.h"

int main() {
    CampusGraph graph;

    graph.addBuilding("Library");
    graph.addBuilding("Union");
    graph.addBuilding("Science");
    graph.addBuilding("Gym");
    graph.addBuilding("Parking");
    graph.displayShortestPath("Library", "Hospital");

    graph.addConnection("Library", "Union");
    graph.addConnection(" library ", "UNION");
    graph.addConnection("Union", "Science");
    graph.addConnection("Science", "Gym");
    graph.addConnection("Union", "Gym");
    graph.addConnection("Library", "Science");

    graph.displayAdjacencyList();
    graph.displayReachableBuildings(" library ");
    graph.addBuilding("  library  ");
    graph.displayReachableBuildings("Hospital");
    graph.displayReachableBuildings("Parking");
    graph.displayShortestPath("Library", "GYM");
    graph.displayShortestPath("Library", "Library");
    graph.displayShortestPath("Library", "Parking");
    graph.displayConnectivityReport();
    graph.addBuilding("   ");

    CampusGraph emptyGraph;
    emptyGraph.displayAdjacencyList();
    emptyGraph.displayConnectivityReport();

    return 0;
}