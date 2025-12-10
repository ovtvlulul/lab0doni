#ifndef COMPRESSOR_STATION_H
#define COMPRESSOR_STATION_H

#include <string>

class CompressorStation
{
private:
    static int nextId;
    int id;
    std::string name;
    int totalShops;
    int workingShops;
    int stationClass;

public:
    CompressorStation();

    // Статические методы для управления ID
    static int getNextId() { return nextId; }
    static void incrementId() { nextId++; }
    static void resetId() { nextId = 1; }

    int getId() const;
    std::string getName() const;
    int getTotalShops() const;
    int getWorkingShops() const;
    int getStationClass() const;
    double getUnusedPercentage() const;

    void setName(const std::string &name);
    void setTotalShops(int total);
    void setWorkingShops(int working);
    void setStationClass(int stationClass);

    void input();
    void display() const;
    void edit();

    void saveToFile(std::ofstream &file) const;
    void loadFromFile(std::ifstream &file);

    bool matchesName(const std::string &searchName) const;
};

#endif