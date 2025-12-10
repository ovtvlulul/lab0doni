#include "Pipe.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>

using namespace std;

int Pipe::nextId = 1;

Pipe::Pipe() : id(0), name(""), length(0.0), diameter(0), underRepair(false), isConnected(false) {}

Pipe::Pipe(int id, const std::string &name, double length, int diameter, bool underRepair, bool isConnected)
    : id(id), name(name), length(length), diameter(diameter), underRepair(underRepair), isConnected(isConnected)
{
    if (id >= nextId)
    {
        nextId = id + 1;
    }
}

bool Pipe::isValidDiameter(int diameter)
{
    vector<int> validDiameters = {500, 700, 1000, 1400};
    return find(validDiameters.begin(), validDiameters.end(), diameter) != validDiameters.end();
}

int Pipe::getId() const { return id; }
string Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::isUnderRepair() const { return underRepair; }
bool Pipe::getIsConnected() const { return isConnected; }

bool Pipe::isAvailableForConnection() const
{
    return !underRepair && !isConnected;
}

void Pipe::setName(const string &name) { this->name = name; }
void Pipe::setLength(double length) { this->length = length; }
void Pipe::setDiameter(int diameter) { this->diameter = diameter; }
void Pipe::setUnderRepair(bool status) { this->underRepair = status; }
void Pipe::setIsConnected(bool connected) { this->isConnected = connected; }

void Pipe::input()
{
    id = getNextId();
    incrementId();

    cout << "\n════════════════════════════════════════" << endl;
    cout << "          CREATE NEW PIPE" << endl;
    cout << "════════════════════════════════════════" << endl;

    cout << "Enter pipe name: ";
    getline(cin, name);

    // Защита ввода длины
    while (true)
    {
        cout << "Enter pipe length (km): ";
        string input;
        getline(cin, input);

        // Проверка на ведущие нули
        if (!input.empty() && input[0] == '0' && input.length() > 1 && input[1] != '.')
        {
            cout << "❌ Error! Numbers with leading zeros are not allowed. Try again." << endl;
            continue;
        }

        // Проверка на буквы и символы
        bool valid = true;
        bool hasDecimal = false;
        for (char c : input)
        {
            if (!isdigit(c) && c != '.' && c != '-')
            {
                valid = false;
                break;
            }
            if (c == '.')
            {
                if (hasDecimal)
                {
                    valid = false;
                    break;
                }
                hasDecimal = true;
            }
        }

        if (!valid)
        {
            cout << "❌ Error! Enter a valid number. Try again." << endl;
            continue;
        }

        try
        {
            length = stod(input);
            if (length > 0)
            {
                break;
            }
            else
            {
                cout << "❌ Error! Length must be positive. Try again." << endl;
            }
        }
        catch (...)
        {
            cout << "❌ Error! Enter a valid number. Try again." << endl;
        }
    }

    // Защита ввода диаметра
    while (true)
    {
        cout << "Enter pipe diameter (mm). Allowed values: 500, 700, 1000, 1400: ";
        string input;
        getline(cin, input);

        // Проверка на ведущие нули
        if (!input.empty() && input[0] == '0' && input.length() > 1)
        {
            cout << "❌ Error! Numbers with leading zeros are not allowed. Try again." << endl;
            continue;
        }

        // Проверка на буквы и символы
        bool valid = true;
        for (char c : input)
        {
            if (!isdigit(c))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            cout << "❌ Error! Enter a valid integer. Try again." << endl;
            continue;
        }

        try
        {
            diameter = stoi(input);
            if (diameter > 0 && isValidDiameter(diameter))
            {
                break;
            }
            else
            {
                cout << "❌ Error! Diameter must be one of: 500, 700, 1000, 1400 mm. Try again." << endl;
            }
        }
        catch (...)
        {
            cout << "❌ Error! Enter a valid integer. Try again." << endl;
        }
    }

    underRepair = false;
    isConnected = false;

    cout << "\n════════════════════════════════════════" << endl;
    cout << "✅ Pipe added successfully!" << endl;
    cout << "Pipe ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Length: " << length << " km" << endl;
    cout << "Diameter: " << diameter << " mm" << endl;
    cout << "════════════════════════════════════════" << endl;
}

void Pipe::display() const
{
    cout << "════════════════════════════════════════" << endl;
    cout << "Pipe ID:          " << id << endl;
    cout << "Name:             " << name << endl;
    cout << "Length:           " << length << " km" << endl;
    cout << "Diameter:         " << diameter << " mm" << endl;
    cout << "Under repair:     " << (underRepair ? "Yes ⚠️" : "No ✅") << endl;
    cout << "Connected:        " << (isConnected ? "Yes 🔗" : "No 🔓") << endl;
    cout << "Available:        " << (isAvailableForConnection() ? "Yes ✅" : "No ❌") << endl;
    cout << "════════════════════════════════════════" << endl;
}

void Pipe::edit()
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "         EDIT PIPE ID: " << id << endl;
    cout << "════════════════════════════════════════" << endl;

    cout << "Current status:" << endl;
    cout << "1. Name: " << name << endl;
    cout << "2. Length: " << length << " km" << endl;
    cout << "3. Diameter: " << diameter << " mm" << endl;
    cout << "4. Under repair: " << (underRepair ? "Yes" : "No") << endl;
    cout << "5. Connected to network: " << (isConnected ? "Yes" : "No") << endl;
    cout << "════════════════════════════════════════" << endl;

    cout << "What do you want to edit?" << endl;
    cout << "1. Change repair status" << endl;
    cout << "2. Change connection status" << endl;
    cout << "0. Cancel" << endl;
    cout << "Choice: ";

    string choiceStr;
    getline(cin, choiceStr);

    if (choiceStr == "1")
    {
        cout << "Current repair status: " << (underRepair ? "Yes" : "No") << endl;
        cout << "Change repair status? (1 - Yes, 0 - No): ";
        string input;
        getline(cin, input);

        if (input == "1")
        {
            underRepair = !underRepair;
            cout << "✅ Repair status changed to: " << (underRepair ? "Yes" : "No") << endl;
        }
        else
        {
            cout << "No changes made." << endl;
        }
    }
    else if (choiceStr == "2")
    {
        cout << "Current connection status: " << (isConnected ? "Yes" : "No") << endl;
        cout << "Change connection status? (1 - Yes, 0 - No): ";
        string input;
        getline(cin, input);

        if (input == "1")
        {
            isConnected = !isConnected;
            cout << "✅ Connection status changed to: " << (isConnected ? "Yes" : "No") << endl;
        }
        else
        {
            cout << "No changes made." << endl;
        }
    }
    else
    {
        cout << "Edit cancelled." << endl;
    }
}

void Pipe::saveToFile(ofstream &file) const
{
    file << "Pipe" << endl;
    file << id << endl;
    file << name << endl;
    file << length << endl;
    file << diameter << endl;
    file << underRepair << endl;
    file << isConnected << endl;
}

void Pipe::loadFromFile(ifstream &file)
{
    file >> id;
    file.ignore();
    getline(file, name);
    file >> length;
    file >> diameter;
    file >> underRepair;
    file >> isConnected;
    file.ignore();

    if (id >= nextId)
    {
        nextId = id + 1;
    }
}

bool Pipe::matchesName(const string &searchName) const
{
    if (searchName.empty())
        return true;

    string pipeNameLower = name;
    string searchNameLower = searchName;

    transform(pipeNameLower.begin(), pipeNameLower.end(), pipeNameLower.begin(), ::tolower);
    transform(searchNameLower.begin(), searchNameLower.end(), searchNameLower.begin(), ::tolower);

    return pipeNameLower.find(searchNameLower) != string::npos;
}