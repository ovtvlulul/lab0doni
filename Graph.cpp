#include "Graph.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

Graph::Graph()
{
    adjacencyList.clear();
    vertexIds.clear();
}

bool Graph::addConnection(int fromStation, int toStation, int pipeId, int diameter)
{
    if (fromStation == toStation)
    {
        cout << "Error: Cannot connect station to itself!" << endl;
        return false;
    }

    // Проверяем, нет ли уже соединения
    if (adjacencyList[fromStation].find(toStation) != adjacencyList[fromStation].end())
    {
        cout << "Error: Connection already exists between stations "
             << fromStation << " and " << toStation << "!" << endl;
        return false;
    }

    Edge edge;
    edge.pipeId = pipeId;
    edge.diameter = diameter;
    edge.isAvailable = true;

    adjacencyList[fromStation][toStation] = edge;
    vertexIds.insert(fromStation);
    vertexIds.insert(toStation);

    return true;
}

bool Graph::removeConnection(int fromStation, int toStation)
{
    auto fromIt = adjacencyList.find(fromStation);
    if (fromIt != adjacencyList.end())
    {
        auto toIt = fromIt->second.find(toStation);
        if (toIt != fromIt->second.end())
        {
            fromIt->second.erase(toIt);
            if (fromIt->second.empty())
            {
                adjacencyList.erase(fromIt);
            }
            return true;
        }
    }
    return false;
}

vector<int> Graph::topologicalSort() const
{
    vector<int> result;

    if (vertexIds.empty())
    {
        return result;
    }

    // Вычисляем входящие степени
    map<int, int> inDegree;
    for (int vertex : vertexIds)
    {
        inDegree[vertex] = 0;
    }

    for (const auto &fromPair : adjacencyList)
    {
        for (const auto &toPair : fromPair.second)
        {
            inDegree[toPair.first]++;
        }
    }

    // Очередь вершин с нулевой входящей степенью
    queue<int> zeroInDegreeQueue;
    for (const auto &pair : inDegree)
    {
        if (pair.second == 0)
        {
            zeroInDegreeQueue.push(pair.first);
        }
    }

    // Процесс топологической сортировки
    while (!zeroInDegreeQueue.empty())
    {
        int vertex = zeroInDegreeQueue.front();
        zeroInDegreeQueue.pop();
        result.push_back(vertex);

        auto it = adjacencyList.find(vertex);
        if (it != adjacencyList.end())
        {
            for (const auto &neighbor : it->second)
            {
                inDegree[neighbor.first]--;
                if (inDegree[neighbor.first] == 0)
                {
                    zeroInDegreeQueue.push(neighbor.first);
                }
            }
        }
    }

    // Проверка на цикл
    if (result.size() != vertexIds.size())
    {
        // Добавляем оставшиеся вершины
        for (int vertex : vertexIds)
        {
            if (find(result.begin(), result.end(), vertex) == result.end())
            {
                result.push_back(vertex);
            }
        }
    }

    return result;
}

void Graph::display() const
{
    if (adjacencyList.empty())
    {
        cout << "📭 Network is empty. No connections between stations." << endl;
        return;
    }

    cout << "\n🔗 NETWORK CONNECTIONS" << endl;
    cout << "════════════════════════════════════════" << endl;
    cout << "From Station → To Station [Pipe Details]" << endl;
    cout << "────────────────────────────────────────" << endl;

    int connectionCount = 0;
    for (const auto &fromPair : adjacencyList)
    {
        for (const auto &toPair : fromPair.second)
        {
            connectionCount++;
            cout << "Station " << fromPair.first
                 << " → Station " << toPair.first
                 << " [Pipe ID: " << toPair.second.pipeId
                 << ", Diameter: " << toPair.second.diameter << " mm]"
                 << endl;
        }
    }

    cout << "════════════════════════════════════════" << endl;
    cout << "Total connections: " << connectionCount << endl;
    cout << "════════════════════════════════════════" << endl;
}

bool Graph::dfsCycleCheck(int vertex, map<int, int> &visited) const
{
    visited[vertex] = 1; // Посещаем вершину

    auto it = adjacencyList.find(vertex);
    if (it != adjacencyList.end())
    {
        for (const auto &neighbor : it->second)
        {
            if (visited[neighbor.first] == 0)
            {
                if (dfsCycleCheck(neighbor.first, visited))
                    return true;
            }
            else if (visited[neighbor.first] == 1)
            {
                return true; // Найден цикл
            }
        }
    }

    visited[vertex] = 2; // Завершаем обработку
    return false;
}

bool Graph::hasCycle() const
{
    map<int, int> visited;
    for (int vertex : vertexIds)
    {
        visited[vertex] = 0;
    }

    for (int vertex : vertexIds)
    {
        if (visited[vertex] == 0)
        {
            if (dfsCycleCheck(vertex, visited))
                return true;
        }
    }

    return false;
}

bool Graph::vertexExists(int stationId) const
{
    return vertexIds.find(stationId) != vertexIds.end();
}

void Graph::addVertex(int stationId)
{
    vertexIds.insert(stationId);
}

void Graph::removeVertex(int stationId)
{
    // Удаляем все исходящие связи
    adjacencyList.erase(stationId);

    // Удаляем все входящие связи
    for (auto &fromPair : adjacencyList)
    {
        fromPair.second.erase(stationId);
    }

    // Удаляем вершину
    vertexIds.erase(stationId);
}

int Graph::getPipeId(int fromStation, int toStation) const
{
    auto fromIt = adjacencyList.find(fromStation);
    if (fromIt != adjacencyList.end())
    {
        auto toIt = fromIt->second.find(toStation);
        if (toIt != fromIt->second.end())
        {
            return toIt->second.pipeId;
        }
    }
    return -1;
}

vector<pair<int, int>> Graph::getConnections() const
{
    vector<pair<int, int>> connections;
    for (const auto &fromPair : adjacencyList)
    {
        for (const auto &toPair : fromPair.second)
        {
            connections.push_back({fromPair.first, toPair.first});
        }
    }
    return connections;
}

vector<pair<int, pair<int, int>>> Graph::getConnectionsWithPipe() const
{
    vector<pair<int, pair<int, int>>> connections;
    for (const auto &fromPair : adjacencyList)
    {
        for (const auto &toPair : fromPair.second)
        {
            connections.push_back({fromPair.first, {toPair.first, toPair.second.pipeId}});
        }
    }
    return connections;
}

void Graph::clear()
{
    adjacencyList.clear();
    vertexIds.clear();
}

bool Graph::isEmpty() const
{
    return adjacencyList.empty();
}

size_t Graph::getVertexCount() const
{
    return vertexIds.size();
}

size_t Graph::getEdgeCount() const
{
    size_t count = 0;
    for (const auto &fromPair : adjacencyList)
    {
        count += fromPair.second.size();
    }
    return count;
}