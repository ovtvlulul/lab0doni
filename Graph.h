#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>

class Graph
{
private:
    struct Edge
    {
        int pipeId;
        int b;
        int diameter;
        bool isAvailable;
    };

    std::map<int, std::map<int, Edge>> adjacencyList; // from -> (to -> Edge)
    std::set<int> vertexIds;                          // все вершины (станции)

    bool dfsCycleCheck(int vertex, std::map<int, int> &visited) const;

public:
    Graph();

    bool addConnection(int fromStation, int toStation, int pipeId, int diameter);
    bool removeConnection(int fromStation, int toStation);
    std::vector<int> topologicalSort() const;
    void display() const;
    bool hasCycle() const;
    bool vertexExists(int stationId) const;
    void addVertex(int stationId);
    void removeVertex(int stationId);
    int getPipeId(int fromStation, int toStation) const;
    std::vector<std::pair<int, int>> getConnections() const;
    std::vector<std::pair<int, std::pair<int, int>>> getConnectionsWithPipe() const;
    void clear();
    bool isEmpty() const;
    size_t getVertexCount() const;
    size_t getEdgeCount() const;
};

#endif