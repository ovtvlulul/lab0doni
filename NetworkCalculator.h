#ifndef NETWORKCALCULATOR_H
#define NETWORKCALCULATOR_H

#include "Graph.h"
#include "PipelineNetwork.h"
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>

class NetworkCalculator
{
private:
    // Пропускные способности труб в зависимости от диаметра (м³/час)
    // Данные из таблицы 1.1 https://www.turbinist.ru/5641-proektirovanie-i-ekspluataciya-mg.html
    static const std::map<int, double> DIAMETER_CAPACITY;

    // Поправочный коэффициент для перевода в целые числа
    static constexpr double CORRECTION_COEFFICIENT = 1000.0;

    // Бесконечность для весов
    static constexpr double INF = std::numeric_limits<double>::max();

public:
    // Рассчитать производительность трубы по формуле Q = k * sqrt(d^5 / l)
    static double calculatePipeCapacity(double length_km, int diameter_mm, bool isUnderRepair);

    // Рассчитать вес ребра (в простейшем случае - длина)
    static double calculateEdgeWeight(double length_km, bool isUnderRepair);

    // Алгоритм Дейкстры для поиска кратчайшего пути
    static std::vector<int> findShortestPath(
        const Graph &graph,
        const PipelineNetwork &network,
        int sourceStation,
        int targetStation,
        double &totalDistance);

    // Алгоритм Форда-Фалкерсона для расчета максимального потока
    static double calculateMaxFlow(
        const Graph &graph,
        const PipelineNetwork &network,
        int sourceStation,
        int targetStation);

    // Отобразить путь между станциями
    static void displayPath(
        const std::vector<int> &path,
        const PipelineNetwork &network,
        const Graph &graph);
};

#endif