#include "GasNetwork.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

bool GasNetwork::connectStations(int fromStation, int toStation, int diameter, int pipeId)
{
    // Проверяем валидность диаметра
    if (diameter != 500 && diameter != 700 && diameter != 1000 && diameter != 1400)
    {
        cout << "❌ Error: Invalid diameter! Allowed diameters: 500, 700, 1000, 1400 mm." << endl;
        return false;
    }

    // Проверяем существование станций
    if (!pipelineNetwork.stationExists(fromStation))
    {
        cout << "❌ Error: Station with ID " << fromStation << " does not exist!" << endl;
        return false;
    }

    if (!pipelineNetwork.stationExists(toStation))
    {
        cout << "❌ Error: Station with ID " << toStation << " does not exist!" << endl;
        return false;
    }

    if (fromStation == toStation)
    {
        cout << "❌ Error: Cannot connect station to itself!" << endl;
        return false;
    }

    // Проверяем, нет ли уже такого соединения
    if (networkGraph.getPipeId(fromStation, toStation) != -1)
    {
        cout << "❌ Error: Connection already exists between Station "
             << fromStation << " and Station " << toStation << "!" << endl;
        return false;
    }

    // Если pipeId не указан, ищем доступную трубу
    if (pipeId == -1)
    {
        cout << "❌ Error: Pipe ID must be specified!" << endl;
        return false;
    }

    // Проверяем выбранную трубу
    const Pipe *selectedPipe = pipelineNetwork.getPipeById(pipeId);

    if (!selectedPipe)
    {
        cout << "❌ Error: Pipe with ID " << pipeId << " does not exist!" << endl;
        return false;
    }

    if (selectedPipe->getDiameter() != diameter)
    {
        cout << "❌ Error: Pipe diameter is " << selectedPipe->getDiameter()
             << " mm, but required " << diameter << " mm!" << endl;
        return false;
    }

    if (!selectedPipe->isAvailableForConnection())
    {
        cout << "❌ Error: Pipe is not available for connection!" << endl;
        cout << "Status: " << (selectedPipe->isUnderRepair() ? "In repair" : "OK")
             << ", Connected: " << (selectedPipe->getIsConnected() ? "Yes" : "No") << endl;
        return false;
    }

    // Всё проверено, создаем соединение
    pipelineNetwork.markPipeAsConnected(pipeId, true);

    if (networkGraph.addConnection(fromStation, toStation, pipeId, diameter))
    {
        cout << "\n════════════════════════════════════════" << endl;
        cout << "✅ CONNECTION SUCCESSFUL!" << endl;
        cout << "════════════════════════════════════════" << endl;
        cout << "Source:      Station " << fromStation << endl;
        cout << "Destination: Station " << toStation << endl;
        cout << "Pipe:        ID " << pipeId << " (" << selectedPipe->getName() << ")" << endl;
        cout << "Diameter:    " << diameter << " mm" << endl;
        cout << "Length:      " << selectedPipe->getLength() << " km" << endl;

        // Показываем расчетную производительность
        double capacity = NetworkCalculator::calculatePipeCapacity(
            selectedPipe->getLength(),
            selectedPipe->getDiameter(),
            selectedPipe->isUnderRepair());
        cout << "Capacity:    " << capacity << " м³/час" << endl;

        cout << "════════════════════════════════════════" << endl;
        return true;
    }

    cout << "❌ Failed to create connection in graph." << endl;
    return false;
}

void GasNetwork::disconnectStations(int fromStation, int toStation)
{
    int pipeId = networkGraph.getPipeId(fromStation, toStation);
    if (pipeId != -1)
    {
        if (networkGraph.removeConnection(fromStation, toStation))
        {
            // Помечаем трубу как свободную
            pipelineNetwork.markPipeAsConnected(pipeId, false);

            cout << "\n════════════════════════════════════════" << endl;
            cout << "✅ DISCONNECTION SUCCESSFUL!" << endl;
            cout << "════════════════════════════════════════" << endl;
            cout << "Disconnected: Station " << fromStation << " → Station " << toStation << endl;
            cout << "Pipe ID " << pipeId << " is now available for use." << endl;
            cout << "════════════════════════════════════════" << endl;
        }
        else
        {
            cout << "❌ Failed to disconnect stations in graph." << endl;
        }
    }
    else
    {
        cout << "❌ No connection found between Station " << fromStation
             << " and Station " << toStation << endl;
    }
}

void GasNetwork::displayNetwork() const
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "      GAS TRANSMISSION NETWORK" << endl;
    cout << "════════════════════════════════════════" << endl;

    // Показываем все объекты
    pipelineNetwork.displayAllObjects();

    // Показываем связи
    networkGraph.display();

    // Показываем статус сети
    displayNetworkStatus();
}

void GasNetwork::performTopologicalSort() const
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "        TOPOLOGICAL SORT" << endl;
    cout << "════════════════════════════════════════" << endl;

    if (networkGraph.isEmpty())
    {
        cout << "Network is empty. No stations connected." << endl;
        return;
    }

    if (networkGraph.hasCycle())
    {
        cout << "⚠️  Warning: Network contains cycles!" << endl;
        cout << "Topological sort may not be possible for cyclic graphs." << endl;
        cout << "--------------------------------" << endl;
    }

    vector<int> sorted = networkGraph.topologicalSort();

    cout << "Topological order of stations:" << endl;
    cout << "--------------------------------" << endl;

    if (sorted.empty())
    {
        cout << "No stations in the network." << endl;
    }
    else
    {
        for (size_t i = 0; i < sorted.size(); i++)
        {
            cout << i + 1 << ". Station " << sorted[i];

            // Показываем информацию о станции
            const CompressorStation *station = pipelineNetwork.getStationById(sorted[i]);
            if (station)
            {
                cout << " (" << station->getName() << ")";
            }
            cout << endl;
        }
    }

    cout << "--------------------------------" << endl;
    cout << "Total stations in sorted order: " << sorted.size() << endl;
}

void GasNetwork::saveNetworkToFile(const std::string &filename) const
{
    string networkFilename = filename + "_network.txt";
    ofstream file(networkFilename);

    if (file.is_open())
    {
        // Сохраняем информацию о сети
        file << "GasNetworkData" << endl;
        file << "Version:1.0" << endl;
        file << "Connections:" << endl;

        // Сохраняем все соединения
        auto connections = networkGraph.getConnectionsWithPipe();
        for (const auto &conn : connections)
        {
            int fromStation = conn.first;
            int toStation = conn.second.first;
            int pipeId = conn.second.second;

            file << fromStation << " " << toStation << " " << pipeId << endl;
        }

        file << "EndConnections" << endl;
        file.close();

        cout << "✅ Network structure saved to: " << networkFilename << endl;

        // Сохраняем данные объектов в отдельный файл
        string dataFilename = filename + "_data.txt";
        pipelineNetwork.saveToFile(dataFilename);
    }
    else
    {
        cout << "❌ Error: Could not open file " << networkFilename << " for writing!" << endl;
    }
}

void GasNetwork::loadNetworkFromFile(const std::string &filename)
{
    string networkFilename = filename + "_network.txt";
    ifstream file(networkFilename);

    if (!file.is_open())
    {
        cout << "❌ Error: Could not open network file " << networkFilename << endl;
        return;
    }

    // Очищаем текущую сеть
    networkGraph.clear();

    string line;
    bool readingConnections = false;

    while (getline(file, line))
    {
        if (line == "GasNetworkData")
        {
            continue;
        }
        else if (line == "Connections:")
        {
            readingConnections = true;
            continue;
        }
        else if (line == "EndConnections")
        {
            readingConnections = false;
            break;
        }
        else if (readingConnections)
        {
            int fromStation, toStation, pipeId;
            if (sscanf(line.c_str(), "%d %d %d", &fromStation, &toStation, &pipeId) == 3)
            {
                // Получаем диаметр трубы
                const Pipe *pipe = pipelineNetwork.getPipeById(pipeId);
                int diameter = pipe ? pipe->getDiameter() : 500;

                networkGraph.addConnection(fromStation, toStation, pipeId, diameter);

                // Помечаем трубу как подключенную
                if (pipe)
                {
                    pipelineNetwork.markPipeAsConnected(pipeId, true);
                }
            }
        }
    }

    file.close();

    cout << "✅ Network structure loaded from: " << networkFilename << endl;

    // Загружаем данные объектов
    string dataFilename = filename + "_data.txt";
    pipelineNetwork.loadFromFile(dataFilename);
}

void GasNetwork::displayNetworkStatus() const
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "          NETWORK STATUS" << endl;
    cout << "════════════════════════════════════════" << endl;
    cout << "Total stations:        " << networkGraph.getVertexCount() << endl;
    cout << "Total connections:     " << networkGraph.getEdgeCount() << endl;
    cout << "Contains cycles:       " << (networkGraph.hasCycle() ? "Yes ⚠️" : "No ✅") << endl;

    if (!networkGraph.isEmpty())
    {
        vector<int> sorted = networkGraph.topologicalSort();
        cout << "Topological order:      ";
        for (size_t i = 0; i < sorted.size(); i++)
        {
            cout << sorted[i];
            if (i < sorted.size() - 1)
                cout << " → ";
        }
        cout << endl;
    }
    cout << "════════════════════════════════════════" << endl;
}

void GasNetwork::deleteStation(int id)
{
    // Удаляем все соединения с этой станцией
    auto connections = networkGraph.getConnections();
    for (const auto &conn : connections)
    {
        if (conn.first == id || conn.second == id)
        {
            int pipeId = networkGraph.getPipeId(conn.first, conn.second);
            networkGraph.removeConnection(conn.first, conn.second);

            // Освобождаем трубу
            if (pipeId != -1)
            {
                pipelineNetwork.markPipeAsConnected(pipeId, false);
            }

            cout << "Removed connection: Station " << conn.first
                 << " → Station " << conn.second << endl;
        }
    }

    // Удаляем вершину из графа
    networkGraph.removeVertex(id);

    // Удаляем саму станцию
    pipelineNetwork.deleteStation(id);
}

// НОВЫЕ МЕТОДЫ ДЛЯ РАСЧЕТОВ

void GasNetwork::calculateShortestPath(int sourceStation, int targetStation)
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "    РАСЧЕТ КРАТЧАЙШЕГО ПУТИ" << endl;
    cout << "════════════════════════════════════════" << endl;

    if (!stationExists(sourceStation))
    {
        cout << "❌ Станция-источник " << sourceStation << " не найдена!" << endl;
        return;
    }

    if (!stationExists(targetStation))
    {
        cout << "❌ Станция-цель " << targetStation << " не найдена!" << endl;
        return;
    }

    if (sourceStation == targetStation)
    {
        cout << "❌ Станция-источник и станция-цель совпадают!" << endl;
        return;
    }

    double totalDistance = 0.0;
    vector<int> path = NetworkCalculator::findShortestPath(
        networkGraph,
        pipelineNetwork,
        sourceStation,
        targetStation,
        totalDistance);

    if (!path.empty())
    {
        cout << "✅ Кратчайший путь найден!" << endl;
        cout << "Станция-источник: " << sourceStation << endl;
        cout << "Станция-цель: " << targetStation << endl;
        cout << "Общее расстояние: " << totalDistance << " км" << endl;

        NetworkCalculator::displayPath(path, pipelineNetwork, networkGraph);
    }
    else
    {
        cout << "❌ Путь между станциями не найден!" << endl;
    }

    cout << "════════════════════════════════════════" << endl;
}

void GasNetwork::calculateMaxFlow(int sourceStation, int targetStation)
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "    РАСЧЕТ МАКСИМАЛЬНОГО ПОТОКА" << endl;
    cout << "════════════════════════════════════════" << endl;

    if (!stationExists(sourceStation))
    {
        cout << "❌ Станция-источник " << sourceStation << " не найдена!" << endl;
        return;
    }

    if (!stationExists(targetStation))
    {
        cout << "❌ Станция-цель " << targetStation << " не найдена!" << endl;
        return;
    }

    if (sourceStation == targetStation)
    {
        cout << "❌ Станция-источник и станция-цель совпадают!" << endl;
        return;
    }

    if (networkGraph.isEmpty())
    {
        cout << "❌ В сети нет соединений!" << endl;
        return;
    }

    double maxFlow = NetworkCalculator::calculateMaxFlow(
        networkGraph,
        pipelineNetwork,
        sourceStation,
        targetStation);

    cout << "Максимальный поток от станции " << sourceStation
         << " к станции " << targetStation << ":" << endl;
    cout << "════════════════════════════════════════" << endl;
    cout << "  " << maxFlow << " м³/час" << endl;

    // Переводим в более удобные единицы
    cout << "  или " << maxFlow * 24 << " м³/сутки" << endl;
    cout << "  или " << maxFlow * 24 / 1e6 << " млн м³/сутки" << endl;

    cout << "════════════════════════════════════════" << endl;

    // Дополнительная информация
    cout << "\n💡 ИНФОРМАЦИЯ О СЕТИ:" << endl;
    cout << "Количество станций: " << networkGraph.getVertexCount() << endl;
    cout << "Количество соединений: " << networkGraph.getEdgeCount() << endl;

    // Проверяем наличие труб в ремонте на пути
    auto connections = networkGraph.getConnectionsWithPipe();
    int pipesUnderRepair = 0;
    for (const auto &conn : connections)
    {
        int pipeId = conn.second.second;
        const Pipe *pipe = pipelineNetwork.getPipeById(pipeId);
        if (pipe && pipe->isUnderRepair())
        {
            pipesUnderRepair++;
        }
    }

    if (pipesUnderRepair > 0)
    {
        cout << "⚠️  ВНИМАНИЕ: " << pipesUnderRepair
             << " труб находятся в ремонте!" << endl;
        cout << "Это ограничивает пропускную способность сети." << endl;
    }

    cout << "════════════════════════════════════════" << endl;
}