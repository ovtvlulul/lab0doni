#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <vector>

class Pipe
{
private:
    static int nextId;
    int id;
    std::string name;
    double length;
    int diameter;
    bool underRepair;
    bool isConnected;

public:
    Pipe();
    Pipe(int id, const std::string &name, double length, int diameter, bool underRepair = false, bool isConnected = false);

    // Статические методы для управления ID
    static int getNextId() { return nextId; }
    static void incrementId() { nextId++; }
    static void resetId() { nextId = 1; }
    static bool isValidDiameter(int diameter);

    // Геттеры
    int getId() const;
    std::string getName() const;
    double getLength() const;
    int getDiameter() const;
    bool isUnderRepair() const;
    bool getIsConnected() const;
    bool isAvailableForConnection() const;

    // Сеттеры
    void setName(const std::string &name);
    void setLength(double length);
    void setDiameter(int diameter);
    void setUnderRepair(bool status);
    void setIsConnected(bool connected);

    // Методы ввода/вывода
    void input();
    void display() const;
    void edit();

    // Методы работы с файлами
    void saveToFile(std::ofstream &file) const;
    void loadFromFile(std::ifstream &file);

    // Методы поиска
    bool matchesName(const std::string &searchName) const;
};

#endif