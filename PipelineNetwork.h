#ifndef PIPELINE_NETWORK_H
#define PIPELINE_NETWORK_H

#include "Pipe.h"
#include "CompressorStation.h"
#include <vector>
#include <unordered_map>
#include <fstream>
#include <map>

class PipelineNetwork
{
private:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CompressorStation> stations;

    void logAction(const std::string &action) const;

public:
    // Основные методы
    void addPipe();
    void addStation();

    // Методы отображения
    void displayAllObjects() const;
    void displayPipes() const;
    void displayStations() const;

    // Методы поиска
    std::vector<int> findPipesByName(const std::string &name) const;
    std::vector<int> findPipesByRepairStatus(bool status) const;
    std::vector<int> findPipesByDiameter(int diameter) const;
    std::vector<int> findPipesByAvailability(bool available) const;
    std::vector<int> findStationsByName(const std::string &name) const;
    std::vector<int> findStationsByUnusedPercentage(double percentage) const;

    // Методы редактирования
    void editPipe(int id);
    void editStation(int id);
    void batchEditPipes(const std::vector<int> &pipeIds);

    // Методы удаления
    void deletePipe(int id);
    void deleteStation(int id);

    // Методы работы с файлами
    void saveToFile(const std::string &filename) const;
    void loadFromFile(const std::string &filename);

    // Методы проверки существования
    bool pipeExists(int id) const;
    bool stationExists(int id) const;

    // Методы отображения ID
    void displayPipeIds() const;
    void displayStationIds() const;

    // Методы для работы с GasNetwork
    Pipe *getPipeById(int id);
    const Pipe *getPipeById(int id) const;
    CompressorStation *getStationById(int id);
    const CompressorStation *getStationById(int id) const;
    std::vector<Pipe> getAvailablePipesByDiameter(int diameter) const;
    void markPipeAsConnected(int pipeId, bool connected);
};

#endif