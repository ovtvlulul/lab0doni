#include "PipelineNetwork.h"
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>

using namespace std;

void PipelineNetwork::logAction(const string &action) const
{
    ofstream logfile("logs.txt", ios::app);
    if (logfile.is_open())
    {
        logfile << getCurrentDateTime() << " - " << action << endl;
        logfile.close();
    }
}

void PipelineNetwork::addPipe()
{
    Pipe pipe;
    pipe.input();
    pipes[pipe.getId()] = pipe;
    logAction("Added pipe with ID: " + to_string(pipe.getId()));
}

void PipelineNetwork::addStation()
{
    CompressorStation station;
    station.input();
    stations[station.getId()] = station;
    logAction("Added station with ID: " + to_string(station.getId()));
}

void PipelineNetwork::displayAllObjects() const
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "           ALL PIPES" << endl;
    cout << "════════════════════════════════════════" << endl;

    if (pipes.empty())
    {
        cout << "No pipes available." << endl;
    }
    else
    {
        for (const auto &pair : pipes)
        {
            pair.second.display();
        }
        cout << "Total pipes: " << pipes.size() << endl;
    }

    cout << "\n════════════════════════════════════════" << endl;
    cout << "         ALL STATIONS" << endl;
    cout << "════════════════════════════════════════" << endl;

    if (stations.empty())
    {
        cout << "No stations available." << endl;
    }
    else
    {
        for (const auto &pair : stations)
        {
            pair.second.display();
        }
        cout << "Total stations: " << stations.size() << endl;
    }
}

void PipelineNetwork::displayPipes() const
{
    if (pipes.empty())
    {
        cout << "No pipes available." << endl;
        return;
    }

    cout << "\n════════════════════════════════════════" << endl;
    cout << "           ALL PIPES" << endl;
    cout << "════════════════════════════════════════" << endl;

    for (const auto &pair : pipes)
    {
        pair.second.display();
    }
}

void PipelineNetwork::displayStations() const
{
    if (stations.empty())
    {
        cout << "No stations available." << endl;
        return;
    }

    cout << "\n════════════════════════════════════════" << endl;
    cout << "         ALL STATIONS" << endl;
    cout << "════════════════════════════════════════" << endl;

    for (const auto &pair : stations)
    {
        pair.second.display();
    }
}

vector<int> PipelineNetwork::findPipesByName(const string &name) const
{
    vector<int> result;
    for (const auto &pair : pipes)
    {
        if (pair.second.matchesName(name))
        {
            result.push_back(pair.first);
        }
    }
    logAction("Search pipes by name: '" + name + "' - found " + to_string(result.size()) + " results");
    return result;
}

vector<int> PipelineNetwork::findPipesByRepairStatus(bool status) const
{
    vector<int> result;
    for (const auto &pair : pipes)
    {
        if (pair.second.isUnderRepair() == status)
        {
            result.push_back(pair.first);
        }
    }
    logAction("Search pipes by repair status: " + to_string(status) + " - found " + to_string(result.size()) + " results");
    return result;
}

vector<int> PipelineNetwork::findPipesByDiameter(int diameter) const
{
    vector<int> result;
    for (const auto &pair : pipes)
    {
        if (pair.second.getDiameter() == diameter)
        {
            result.push_back(pair.first);
        }
    }
    logAction("Search pipes by diameter: " + to_string(diameter) + " - found " + to_string(result.size()) + " results");
    return result;
}

vector<int> PipelineNetwork::findPipesByAvailability(bool available) const
{
    vector<int> result;
    for (const auto &pair : pipes)
    {
        if (pair.second.isAvailableForConnection() == available)
        {
            result.push_back(pair.first);
        }
    }
    logAction("Search pipes by availability: " + to_string(available) + " - found " + to_string(result.size()) + " results");
    return result;
}

vector<int> PipelineNetwork::findStationsByName(const string &name) const
{
    vector<int> result;
    for (const auto &pair : stations)
    {
        if (pair.second.matchesName(name))
        {
            result.push_back(pair.first);
        }
    }
    logAction("Search stations by name: '" + name + "' - found " + to_string(result.size()) + " results");
    return result;
}

vector<int> PipelineNetwork::findStationsByUnusedPercentage(double percentage) const
{
    vector<int> result;
    for (const auto &pair : stations)
    {
        if (pair.second.getUnusedPercentage() >= percentage)
        {
            result.push_back(pair.first);
        }
    }
    logAction("Search stations by unused percentage: " + to_string(percentage) + "% - found " + to_string(result.size()) + " results");
    return result;
}

void PipelineNetwork::editPipe(int id)
{
    auto it = pipes.find(id);
    if (it != pipes.end())
    {
        it->second.edit();
        logAction("Edited pipe with ID: " + to_string(id));
    }
    else
    {
        cout << "❌ Pipe with ID " << id << " not found!" << endl;
    }
}

void PipelineNetwork::editStation(int id)
{
    auto it = stations.find(id);
    if (it != stations.end())
    {
        it->second.edit();
        logAction("Edited station with ID: " + to_string(id));
    }
    else
    {
        cout << "❌ Station with ID " << id << " not found!" << endl;
    }
}

void PipelineNetwork::batchEditPipes(const vector<int> &pipeIds)
{
    if (pipeIds.empty())
    {
        cout << "No pipes to edit!" << endl;
        return;
    }

    cout << "Batch editing " << pipeIds.size() << " pipes:" << endl;
    for (int id : pipeIds)
    {
        auto it = pipes.find(id);
        if (it != pipes.end())
        {
            cout << "\nEditing pipe ID: " << id << endl;
            it->second.edit();
        }
    }
    logAction("Batch edited " + to_string(pipeIds.size()) + " pipes");
}

void PipelineNetwork::deletePipe(int id)
{
    if (pipes.erase(id))
    {
        cout << "✅ Pipe with ID " << id << " deleted successfully!" << endl;
        logAction("Deleted pipe with ID: " + to_string(id));
    }
    else
    {
        cout << "❌ Pipe with ID " << id << " not found!" << endl;
    }
}

void PipelineNetwork::deleteStation(int id)
{
    if (stations.erase(id))
    {
        cout << "✅ Station with ID " << id << " deleted successfully!" << endl;
        logAction("Deleted station with ID: " + to_string(id));
    }
    else
    {
        cout << "❌ Station with ID " << id << " not found!" << endl;
    }
}

void PipelineNetwork::saveToFile(const string &filename) const
{
    ofstream file(filename);
    if (file.is_open())
    {
        // Сохраняем трубы
        for (const auto &pair : pipes)
        {
            pair.second.saveToFile(file);
        }

        // Сохраняем станции
        for (const auto &pair : stations)
        {
            pair.second.saveToFile(file);
        }

        file.close();
        cout << "✅ Data saved to " << filename << " successfully!" << endl;
        logAction("Saved data to file: " + filename);
    }
    else
    {
        cout << "❌ Error opening file " << filename << " for writing!" << endl;
    }
}

void PipelineNetwork::loadFromFile(const string &filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        pipes.clear();
        stations.clear();

        string line;
        while (getline(file, line))
        {
            if (line == "Pipe")
            {
                Pipe pipe;
                pipe.loadFromFile(file);
                pipes[pipe.getId()] = pipe;
            }
            else if (line == "Station")
            {
                CompressorStation station;
                station.loadFromFile(file);
                stations[station.getId()] = station;
            }
        }
        file.close();
        cout << "✅ Data loaded from " << filename << " successfully!" << endl;
        logAction("Loaded data from file: " + filename);
    }
    else
    {
        cout << "❌ Error opening file " << filename << " for reading!" << endl;
    }
}

bool PipelineNetwork::pipeExists(int id) const
{
    return pipes.find(id) != pipes.end();
}

bool PipelineNetwork::stationExists(int id) const
{
    return stations.find(id) != stations.end();
}

void PipelineNetwork::displayPipeIds() const
{
    if (pipes.empty())
    {
        cout << "No pipes available." << endl;
        return;
    }

    cout << "\nAvailable pipe IDs: ";
    for (const auto &pair : pipes)
    {
        cout << pair.first << " ";
    }
    cout << endl;

    cout << "Total pipes: " << pipes.size() << endl;
}

void PipelineNetwork::displayStationIds() const
{
    if (stations.empty())
    {
        cout << "No stations available." << endl;
        return;
    }

    cout << "\nAvailable station IDs: ";
    for (const auto &pair : stations)
    {
        cout << pair.first << " ";
    }
    cout << endl;

    cout << "Total stations: " << stations.size() << endl;
}

Pipe *PipelineNetwork::getPipeById(int id)
{
    auto it = pipes.find(id);
    if (it != pipes.end())
    {
        return &it->second;
    }
    return nullptr;
}

const Pipe *PipelineNetwork::getPipeById(int id) const
{
    auto it = pipes.find(id);
    if (it != pipes.end())
    {
        return &it->second;
    }
    return nullptr;
}

CompressorStation *PipelineNetwork::getStationById(int id)
{
    auto it = stations.find(id);
    if (it != stations.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CompressorStation *PipelineNetwork::getStationById(int id) const
{
    auto it = stations.find(id);
    if (it != stations.end())
    {
        return &it->second;
    }
    return nullptr;
}

vector<Pipe> PipelineNetwork::getAvailablePipesByDiameter(int diameter) const
{
    vector<Pipe> result;
    for (const auto &pair : pipes)
    {
        if (pair.second.getDiameter() == diameter && pair.second.isAvailableForConnection())
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

void PipelineNetwork::markPipeAsConnected(int pipeId, bool connected)
{
    auto it = pipes.find(pipeId);
    if (it != pipes.end())
    {
        it->second.setIsConnected(connected);
        logAction("Marked pipe ID " + to_string(pipeId) + " as " + (connected ? "connected" : "disconnected"));
    }
}