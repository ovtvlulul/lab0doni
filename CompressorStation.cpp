#include "CompressorStation.h"
#include <iostream>
#include <fstream>
#include <cctype>

int CompressorStation::nextId = 1;

CompressorStation::CompressorStation()
    : id(0), name(""), totalShops(0), workingShops(0), stationClass(0) {}

int CompressorStation::getId() const { return id; }
std::string CompressorStation::getName() const { return name; }
int CompressorStation::getTotalShops() const { return totalShops; }
int CompressorStation::getWorkingShops() const { return workingShops; }
int CompressorStation::getStationClass() const { return stationClass; }

double CompressorStation::getUnusedPercentage() const
{
    if (totalShops == 0)
        return 0.0;
    return ((totalShops - workingShops) * 100.0) / totalShops;
}

void CompressorStation::setName(const std::string &name) { this->name = name; }
void CompressorStation::setTotalShops(int total) { this->totalShops = total; }
void CompressorStation::setWorkingShops(int working) { this->workingShops = working; }
void CompressorStation::setStationClass(int stationClass) { this->stationClass = stationClass; }

void CompressorStation::input()
{
    id = getNextId(); // Устанавливаем ID перед вводом данных
    incrementId();    // Увеличиваем счетчик

    std::cout << "Enter station name: ";
    std::getline(std::cin, name);

    // Защита ввода общего количества цехов
    while (true)
    {
        std::cout << "Enter total number of shops: ";
        std::string input;
        std::getline(std::cin, input);

        // Проверка на ведущие нули
        if (!input.empty() && input[0] == '0' && input.length() > 1)
        {
            std::cout << "Error! Numbers with leading zeros are not allowed. Try again." << std::endl;
            continue;
        }

        // Проверка на буквы и символы
        bool valid = true;
        for (char c : input)
        {
            if (!std::isdigit(c))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            std::cout << "Error! Enter a valid integer. Try again." << std::endl;
            continue;
        }

        try
        {
            totalShops = std::stoi(input);
            if (totalShops > 0)
            {
                break;
            }
            else
            {
                std::cout << "Error! Number must be positive. Try again." << std::endl;
            }
        }
        catch (...)
        {
            std::cout << "Error! Enter a valid integer. Try again." << std::endl;
        }
    }

    // Защита ввода рабочих цехов
    while (true)
    {
        std::cout << "Enter number of working shops: ";
        std::string input;
        std::getline(std::cin, input);

        // Проверка на ведущие нули
        if (!input.empty() && input[0] == '0' && input.length() > 1)
        {
            std::cout << "Error! Numbers with leading zeros are not allowed. Try again." << std::endl;
            continue;
        }

        // Проверка на буквы и символы
        bool valid = true;
        for (char c : input)
        {
            if (!std::isdigit(c))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            std::cout << "Error! Enter a valid integer. Try again." << std::endl;
            continue;
        }

        try
        {
            workingShops = std::stoi(input);
            if (workingShops >= 0 && workingShops <= totalShops)
            {
                break;
            }
            else
            {
                std::cout << "Error! Working shops must be between 0 and " << totalShops << ". Try again." << std::endl;
            }
        }
        catch (...)
        {
            std::cout << "Error! Enter a valid integer. Try again." << std::endl;
        }
    }

    // Защита ввода класса станции
    while (true)
    {
        std::cout << "Enter station class: ";
        std::string input;
        std::getline(std::cin, input);

        // Проверка на ведущие нули
        if (!input.empty() && input[0] == '0' && input.length() > 1)
        {
            std::cout << "Error! Numbers with leading zeros are not allowed. Try again." << std::endl;
            continue;
        }

        // Проверка на буквы и символы
        bool valid = true;
        for (char c : input)
        {
            if (!std::isdigit(c))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            std::cout << "Error! Enter a valid integer. Try again." << std::endl;
            continue;
        }

        try
        {
            stationClass = std::stoi(input);
            if (stationClass > 0)
            {
                break;
            }
            else
            {
                std::cout << "Error! Class must be positive. Try again." << std::endl;
            }
        }
        catch (...)
        {
            std::cout << "Error! Enter a valid integer. Try again." << std::endl;
        }
    }

    std::cout << "Station added successfully! ID: " << id << std::endl;
}

void CompressorStation::display() const
{
    std::cout << "Station ID: " << id << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Total shops: " << totalShops << std::endl;
    std::cout << "Working shops: " << workingShops << std::endl;
    std::cout << "Station class: " << stationClass << std::endl;
    std::cout << "Unused percentage: " << getUnusedPercentage() << "%" << std::endl;
    std::cout << "------------------------" << std::endl;
}

void CompressorStation::edit()
{
    std::cout << "Current working shops: " << workingShops << "/" << totalShops << std::endl;

    // Защита ввода выбора
    while (true)
    {
        std::cout << "1. Start shop" << std::endl;
        std::cout << "2. Stop shop" << std::endl;
        std::cout << "0. Cancel" << std::endl;
        std::cout << "Choice: ";

        std::string input;
        std::getline(std::cin, input);

        // Проверка на один символ
        if (input.length() != 1 || (input[0] != '0' && input[0] != '1' && input[0] != '2'))
        {
            std::cout << "Error! Enter 0, 1 or 2. Try again." << std::endl;
            continue;
        }

        int choice = input[0] - '0';

        switch (choice)
        {
        case 1:
            if (workingShops < totalShops)
            {
                workingShops++;
                std::cout << "Shop started. Now working: " << workingShops << std::endl;
            }
            else
            {
                std::cout << "All shops are already working!" << std::endl;
            }
            return;
        case 2:
            if (workingShops > 0)
            {
                workingShops--;
                std::cout << "Shop stopped. Now working: " << workingShops << std::endl;
            }
            else
            {
                std::cout << "No shops are working!" << std::endl;
            }
            return;
        case 0:
            std::cout << "Operation cancelled." << std::endl;
            return;
        }
    }
}

void CompressorStation::saveToFile(std::ofstream &file) const
{
    file << "Station" << std::endl;
    file << id << std::endl;
    file << name << std::endl;
    file << totalShops << std::endl;
    file << workingShops << std::endl;
    file << stationClass << std::endl;
}

void CompressorStation::loadFromFile(std::ifstream &file)
{
    file >> id;
    file.ignore();
    std::getline(file, name);
    file >> totalShops;
    file >> workingShops;
    file >> stationClass;
    file.ignore();

    // Обновляем nextId чтобы избежать конфликтов
    if (id >= nextId)
    {
        nextId = id + 1;
    }
}

bool CompressorStation::matchesName(const std::string &searchName) const
{
    return name.find(searchName) != std::string::npos;
}