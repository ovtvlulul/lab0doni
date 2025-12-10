#include "NetworkCalculator.h"
#include <cmath>
#include <map>

using namespace std;

// Инициализация статической константы
const map<int, double> NetworkCalculator::DIAMETER_CAPACITY = {
    {500, 2.5},   // 500 мм - 2.5 млн м³/сутки
    {700, 7.0},   // 700 мм - 7.0 млн м³/сутки
    {1000, 20.0}, // 1000 мм - 20.0 млн м³/сутки
    {1400, 55.0}  // 1400 мм - 55.0 млн м³/сутки
};

double NetworkCalculator::calculatePipeCapacity(double length_km, int diameter_mm, bool isUnderRepair)
{
    // Если труба в ремонте, производительность = 0
    if (isUnderRepair)
    {
        return 0.0;
    }

    // Используем табличные значения производительности в зависимости от диаметра
    auto it = DIAMETER_CAPACITY.find(diameter_mm);
    if (it != DIAMETER_CAPACITY.end())
    {
        // Конвертируем из млн м³/сутки в м³/час
        double dailyCapacity = it->second * 1e6; // млн -> абсолютное значение
        double hourlyCapacity = dailyCapacity / 24.0;

        // Учитываем длину трубы (чем длиннее труба, тем меньше пропускная способность)
        // Упрощенная формула: Q = Q0 * (1 / sqrt(1 + 0.01 * length))
        double lengthFactor = 1.0 / sqrt(1.0 + 0.01 * length_km);
        return hourlyCapacity * lengthFactor;
    }

    // Если диаметра нет в таблице, используем формулу Q = k * sqrt(d^5 / l)
    double length_m = length_km * 1000.0;     // переводим в метры
    double diameter_m = diameter_mm / 1000.0; // переводим в метры

    // Формула: Q = k * sqrt(d^5 / l)
    double capacity = CORRECTION_COEFFICIENT * sqrt(pow(diameter_m, 5) / length_m);

    return capacity;
}

double NetworkCalculator::calculateEdgeWeight(double length_km, bool isUnderRepair)
{
    // Если труба в ремонте, вес = бесконечность
    if (isUnderRepair)
    {
        return INF;
    }

    // В простейшем случае вес = длина
    return length_km;
}

vector<int> NetworkCalculator::findShortestPath(
    const Graph &graph,
    const PipelineNetwork &network,
    int sourceStation,
    int targetStation,
    double &totalDistance)
{
    vector<int> path;
    totalDistance = 0.0;

    // Проверка существования станций
    if (!network.stationExists(sourceStation) || !network.stationExists(targetStation))
    {
        cout << "❌ Одна из указанных станций не существует!" << endl;
        return path;
    }

    // Получаем список всех вершин
    set<int> vertices;

    // Для простоты используем список смежности из графа
    // В реальности нужно получить все вершины из adjacencyList и vertexIds

    // Получаем все соединения
    auto connections = graph.getConnectionsWithPipe();

    if (connections.empty())
    {
        cout << "❌ В сети нет соединений!" << endl;
        return path;
    }

    // Создаем граф для алгоритма Дейкстры
    map<int, vector<pair<int, double>>> adjList;
    map<int, double> dist;
    map<int, int> prev;

    // Инициализация
    for (const auto &conn : connections)
    {
        int u = conn.first;
        int v = conn.second.first;
        int pipeId = conn.second.second;

        const Pipe *pipe = network.getPipeById(pipeId);
        if (pipe)
        {
            double weight = calculateEdgeWeight(pipe->getLength(), pipe->isUnderRepair());
            adjList[u].push_back({v, weight});
        }

        // Добавляем вершины в карту расстояний
        dist[u] = INF;
        dist[v] = INF;
        prev[u] = -1;
        prev[v] = -1;
    }

    // Инициализация источника
    dist[sourceStation] = 0;

    // Очередь с приоритетом для алгоритма Дейкстры
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0, sourceStation});

    // Алгоритм Дейкстры
    while (!pq.empty())
    {
        auto [currentDist, u] = pq.top();
        pq.pop();

        if (currentDist > dist[u])
            continue;

        if (u == targetStation)
            break;

        for (const auto &[v, weight] : adjList[u])
        {
            double newDist = dist[u] + weight;
            if (newDist < dist[v])
            {
                dist[v] = newDist;
                prev[v] = u;
                pq.push({newDist, v});
            }
        }
    }

    // Восстанавливаем путь
    if (dist[targetStation] < INF)
    {
        totalDistance = dist[targetStation];

        // Восстановление пути от цели к источнику
        int current = targetStation;
        while (current != -1)
        {
            path.push_back(current);
            current = prev[current];
        }

        // Переворачиваем путь
        reverse(path.begin(), path.end());
    }
    else
    {
        cout << "❌ Путь между станциями " << sourceStation
             << " и " << targetStation << " не найден!" << endl;
    }

    return path;
}

double NetworkCalculator::calculateMaxFlow(
    const Graph &graph,
    const PipelineNetwork &network,
    int sourceStation,
    int targetStation)
{
    // Проверка существования станций
    if (!network.stationExists(sourceStation) || !network.stationExists(targetStation))
    {
        cout << "❌ Одна из указанных станций не существует!" << endl;
        return 0.0;
    }

    // Получаем все соединения
    auto connections = graph.getConnectionsWithPipe();

    if (connections.empty())
    {
        cout << "❌ В сети нет соединений!" << endl;
        return 0.0;
    }

    // Создаем граф для алгоритма Форда-Фалкерсона
    map<int, map<int, double>> capacity;
    map<int, map<int, double>> flow;

    // Инициализация графа пропускных способностей
    for (const auto &conn : connections)
    {
        int u = conn.first;
        int v = conn.second.first;
        int pipeId = conn.second.second;

        const Pipe *pipe = network.getPipeById(pipeId);
        if (pipe)
        {
            double cap = calculatePipeCapacity(
                pipe->getLength(),
                pipe->getDiameter(),
                pipe->isUnderRepair());
            capacity[u][v] = cap;
            flow[u][v] = 0.0;

            // Добавляем обратное ребро с нулевой пропускной способностью
            capacity[v][u] = 0.0;
            flow[v][u] = 0.0;
        }
    }

    // Алгоритм Форда-Фалкерсона
    double maxFlow = 0.0;

    while (true)
    {
        // Поиск увеличивающего пути с помощью BFS
        map<int, int> parent;
        queue<int> q;
        q.push(sourceStation);
        parent[sourceStation] = -1;

        bool foundPath = false;

        while (!q.empty() && !foundPath)
        {
            int u = q.front();
            q.pop();

            for (const auto &[v, cap] : capacity[u])
            {
                if (parent.find(v) == parent.end() && cap > flow[u][v])
                {
                    parent[v] = u;
                    if (v == targetStation)
                    {
                        foundPath = true;
                        break;
                    }
                    q.push(v);
                }
            }
        }

        // Если путь не найден, завершаем
        if (!foundPath)
            break;

        // Находим минимальную остаточную пропускную способность
        double pathFlow = INF;
        for (int v = targetStation; v != sourceStation; v = parent[v])
        {
            int u = parent[v];
            pathFlow = min(pathFlow, capacity[u][v] - flow[u][v]);
        }

        // Обновляем поток
        for (int v = targetStation; v != sourceStation; v = parent[v])
        {
            int u = parent[v];
            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

void NetworkCalculator::displayPath(
    const vector<int> &path,
    const PipelineNetwork &network,
    const Graph &graph)
{
    if (path.empty())
    {
        cout << "Путь не найден!" << endl;
        return;
    }

    cout << "\n════════════════════════════════════════" << endl;
    cout << "           НАЙДЕННЫЙ ПУТЬ" << endl;
    cout << "════════════════════════════════════════" << endl;

    double totalLength = 0.0;
    double totalCapacity = 0.0;

    for (size_t i = 0; i < path.size(); i++)
    {
        cout << "Станция " << path[i];

        const CompressorStation *station = network.getStationById(path[i]);
        if (station)
        {
            cout << " (" << station->getName() << ")";
        }

        if (i < path.size() - 1)
        {
            cout << " → ";

            // Получаем информацию о трубе между станциями
            int pipeId = graph.getPipeId(path[i], path[i + 1]);
            if (pipeId != -1)
            {
                const Pipe *pipe = network.getPipeById(pipeId);
                if (pipe)
                {
                    double capacity = calculatePipeCapacity(
                        pipe->getLength(),
                        pipe->getDiameter(),
                        pipe->isUnderRepair());

                    cout << "\n   ├── Труба ID: " << pipeId
                         << ", Длина: " << pipe->getLength() << " км"
                         << ", Диаметр: " << pipe->getDiameter() << " мм"
                         << ", Пропускная способность: " << capacity << " м³/час";

                    totalLength += pipe->getLength();
                    totalCapacity = min(totalCapacity == 0.0 ? INF : totalCapacity, capacity);
                }
            }
            cout << endl;
        }
        else
        {
            cout << endl;
        }
    }

    cout << "════════════════════════════════════════" << endl;
    cout << "Общая длина пути: " << totalLength << " км" << endl;
    cout << "Максимальная пропускная способность пути: "
         << (totalCapacity == INF ? "∞" : to_string(totalCapacity) + " м³/час") << endl;
    cout << "════════════════════════════════════════" << endl;
}