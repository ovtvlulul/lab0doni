#ifndef GASNETWORK_H
#define GASNETWORK_H

#include "PipelineNetwork.h"
#include "Graph.h"
#include "NetworkCalculator.h"
#include <vector>
#include <string>

class GasNetwork
{
private:
    PipelineNetwork pipelineNetwork;
    Graph networkGraph;

public:
    GasNetwork() = default;

    PipelineNetwork &getPipelineNetwork() { return pipelineNetwork; }
    Graph &getGraph() { return networkGraph; }

    bool connectStations(int fromStation, int toStation, int diameter, int pipeId = -1);
    void disconnectStations(int fromStation, int toStation);
    void displayNetwork() const;
    void performTopologicalSort() const;
    void saveNetworkToFile(const std::string &filename) const;
    void loadNetworkFromFile(const std::string &filename);
    void displayNetworkStatus() const;

    // НОВЫЕ МЕТОДЫ ДЛЯ РАСЧЕТОВ
    void calculateShortestPath(int sourceStation, int targetStation);
    void calculateMaxFlow(int sourceStation, int targetStation);

    // Методы-обертки для PipelineNetwork
    void addPipe() { pipelineNetwork.addPipe(); }
    void addStation() { pipelineNetwork.addStation(); }
    void displayAllObjects() const { pipelineNetwork.displayAllObjects(); }
    void editPipe(int id) { pipelineNetwork.editPipe(id); }
    void editStation(int id) { pipelineNetwork.editStation(id); }
    void deletePipe(int id) { pipelineNetwork.deletePipe(id); }
    void deleteStation(int id);
    void saveToFile(const std::string &filename) const { pipelineNetwork.saveToFile(filename); }
    void loadFromFile(const std::string &filename) { pipelineNetwork.loadFromFile(filename); }

    // Новые методы для работы с сетью
    bool stationExists(int id) const { return pipelineNetwork.stationExists(id); }
    void displayStations() const { pipelineNetwork.displayStations(); }
};

#endif