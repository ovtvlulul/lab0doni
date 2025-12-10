#include "GasNetwork.h"
#include "utils.h"
#include <iostream>
#include <vector>

using namespace std;

void displayMainMenu()
{
    cout << "\n=== GAS TRANSMISSION NETWORK MANAGEMENT SYSTEM ===" << endl;
    cout << "1. Add Pipe" << endl;
    cout << "2. Add Compressor Station" << endl;
    cout << "3. View All Objects" << endl;
    cout << "4. Edit Pipe" << endl;
    cout << "5. Edit Station" << endl;
    cout << "6. Delete Pipe" << endl;
    cout << "7. Delete Station" << endl;
    cout << "8. Search Pipes" << endl;
    cout << "9. Search Stations" << endl;
    cout << "10. Batch Edit Pipes" << endl;
    cout << "11. Connect Stations (Create Network)" << endl;
    cout << "12. Disconnect Stations" << endl;
    cout << "13. View Network" << endl;
    cout << "14. Topological Sort" << endl;
    cout << "15. Calculate Shortest Path" << endl;
    cout << "16. Calculate Maximum Flow" << endl;
    cout << "17. Save Data" << endl;
    cout << "18. Load Data" << endl;
    cout << "19. Network Status" << endl;
    cout << "0. Exit" << endl;
    cout << "--------------------------------" << endl;
    cout << "Choice: ";
}

void searchPipesMenu(GasNetwork &network)
{
    cout << "\n=== SEARCH PIPES ===" << endl;
    cout << "1. Search by name" << endl;
    cout << "2. Search by repair status" << endl;
    cout << "3. Search by diameter" << endl;
    cout << "4. Search by availability" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choice: ";

    int choice = getIntegerInput("");
    switch (choice)
    {
    case 1:
    {
        string name = getStringInput("Enter pipe name to search: ");
        vector<int> results = network.getPipelineNetwork().findPipesByName(name);
        if (results.empty())
        {
            cout << "No pipes found with name: " << name << endl;
        }
        else
        {
            cout << "Found " << results.size() << " pipes:" << endl;
            for (int id : results)
            {
                const Pipe *pipe = network.getPipelineNetwork().getPipeById(id);
                if (pipe)
                {
                    cout << "Pipe ID: " << id << ", Name: " << pipe->getName()
                         << ", Diameter: " << pipe->getDiameter() << " mm" << endl;
                }
            }
        }
        break;
    }
    case 2:
    {
        cout << "Search pipes: 1 - In repair, 0 - Not in repair: ";
        bool status = getIntegerInput("") == 1;
        vector<int> results = network.getPipelineNetwork().findPipesByRepairStatus(status);
        if (results.empty())
        {
            cout << "No pipes found with specified repair status." << endl;
        }
        else
        {
            cout << "Found " << results.size() << " pipes:" << endl;
            for (int id : results)
            {
                const Pipe *pipe = network.getPipelineNetwork().getPipeById(id);
                if (pipe)
                {
                    cout << "Pipe ID: " << id << ", Status: "
                         << (pipe->isUnderRepair() ? "In repair" : "Operational") << endl;
                }
            }
        }
        break;
    }
    case 3:
    {
        cout << "Enter diameter to search (500, 700, 1000, 1400): ";
        int diameter = getIntegerInput("");
        vector<int> results = network.getPipelineNetwork().findPipesByDiameter(diameter);
        if (results.empty())
        {
            cout << "No pipes found with diameter: " << diameter << " mm" << endl;
        }
        else
        {
            cout << "Found " << results.size() << " pipes:" << endl;
            for (int id : results)
            {
                const Pipe *pipe = network.getPipelineNetwork().getPipeById(id);
                if (pipe)
                {
                    cout << "Pipe ID: " << id << ", Name: " << pipe->getName()
                         << ", Length: " << pipe->getLength() << " km" << endl;
                }
            }
        }
        break;
    }
    case 4:
    {
        cout << "Search pipes: 1 - Available, 0 - Not available: ";
        bool available = getIntegerInput("") == 1;
        vector<int> results = network.getPipelineNetwork().findPipesByAvailability(available);
        if (results.empty())
        {
            cout << "No pipes found with specified availability." << endl;
        }
        else
        {
            cout << "Found " << results.size() << " pipes:" << endl;
            for (int id : results)
            {
                const Pipe *pipe = network.getPipelineNetwork().getPipeById(id);
                if (pipe)
                {
                    cout << "Pipe ID: " << id << ", Available: "
                         << (pipe->isAvailableForConnection() ? "Yes" : "No") << endl;
                }
            }
        }
        break;
    }
    case 0:
        return;
    default:
        cout << "Invalid choice!" << endl;
    }
}

void searchStationsMenu(GasNetwork &network)
{
    cout << "\n=== SEARCH STATIONS ===" << endl;
    cout << "1. Search by name" << endl;
    cout << "2. Search by unused percentage" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choice: ";

    int choice = getIntegerInput("");
    switch (choice)
    {
    case 1:
    {
        string name = getStringInput("Enter station name to search: ");
        vector<int> results = network.getPipelineNetwork().findStationsByName(name);
        if (results.empty())
        {
            cout << "No stations found with name: " << name << endl;
        }
        else
        {
            cout << "Found " << results.size() << " stations:" << endl;
            for (int id : results)
            {
                const CompressorStation *station = network.getPipelineNetwork().getStationById(id);
                if (station)
                {
                    cout << "Station ID: " << id << ", Name: " << station->getName()
                         << ", Unused: " << station->getUnusedPercentage() << "%" << endl;
                }
            }
        }
        break;
    }
    case 2:
    {
        double percentage = getDoubleInput("Enter minimum unused percentage: ");
        vector<int> results = network.getPipelineNetwork().findStationsByUnusedPercentage(percentage);
        if (results.empty())
        {
            cout << "No stations found with unused percentage >= " << percentage << "%" << endl;
        }
        else
        {
            cout << "Found " << results.size() << " stations:" << endl;
            for (int id : results)
            {
                const CompressorStation *station = network.getPipelineNetwork().getStationById(id);
                if (station)
                {
                    cout << "Station ID: " << id << ", Name: " << station->getName()
                         << ", Unused: " << station->getUnusedPercentage() << "%" << endl;
                }
            }
        }
        break;
    }
    case 0:
        return;
    default:
        cout << "Invalid choice!" << endl;
    }
}

void connectStationsMenu(GasNetwork &network)
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "         CONNECT STATIONS" << endl;
    cout << "════════════════════════════════════════" << endl;

    // Показываем все станции
    cout << "\n--- ALL STATIONS ---" << endl;
    network.getPipelineNetwork().displayStations();

    // Запрашиваем ID станций
    int fromStation = getIntegerInput("\nEnter SOURCE station ID: ");
    int toStation = getIntegerInput("Enter DESTINATION station ID: ");

    if (fromStation == toStation)
    {
        cout << "❌ Error: Cannot connect station to itself!" << endl;
        return;
    }

    // Проверяем существование станций
    if (!network.stationExists(fromStation))
    {
        cout << "❌ Error: Station with ID " << fromStation << " does not exist!" << endl;
        return;
    }

    if (!network.stationExists(toStation))
    {
        cout << "❌ Error: Station with ID " << toStation << " does not exist!" << endl;
        return;
    }

    // Проверяем, нет ли уже соединения
    if (network.getGraph().getPipeId(fromStation, toStation) != -1)
    {
        cout << "❌ Error: Connection already exists between these stations!" << endl;
        return;
    }

    // Выбор диаметра
    cout << "\n--- SELECT PIPE DIAMETER ---" << endl;
    cout << "1. 500 mm" << endl;
    cout << "2. 700 mm" << endl;
    cout << "3. 1000 mm" << endl;
    cout << "4. 1400 mm" << endl;
    cout << "Choice: ";

    int diameterChoice = getIntegerInput("");
    int diameter;

    switch (diameterChoice)
    {
    case 1:
        diameter = 500;
        break;
    case 2:
        diameter = 700;
        break;
    case 3:
        diameter = 1000;
        break;
    case 4:
        diameter = 1400;
        break;
    default:
        cout << "Invalid choice! Using 500 mm." << endl;
        diameter = 500;
    }

    // Показываем доступные трубы этого диаметра
    cout << "\n--- AVAILABLE PIPES (" << diameter << " mm) ---" << endl;
    auto availablePipes = network.getPipelineNetwork().getAvailablePipesByDiameter(diameter);

    if (availablePipes.empty())
    {
        cout << "No available pipes with diameter " << diameter << " mm." << endl;
        cout << "\nOptions:" << endl;
        cout << "1. Create new pipe" << endl;
        cout << "2. Use existing pipe (enter ID manually)" << endl;
        cout << "0. Cancel" << endl;
        cout << "Choice: ";

        int option = getIntegerInput("");

        if (option == 1)
        {
            cout << "\nCreating new pipe..." << endl;
            network.addPipe();
            cout << "Please connect stations again after creating pipe." << endl;
            return;
        }
        else if (option == 2)
        {
            cout << "Enter pipe ID: ";
            int pipeId = getIntegerInput("");

            // Пытаемся подключиться с указанной трубой
            if (network.connectStations(fromStation, toStation, diameter, pipeId))
            {
                cout << "✅ Connection successful!" << endl;
            }
            else
            {
                cout << "❌ Failed to connect stations." << endl;
            }
            return;
        }
        else
        {
            cout << "Operation cancelled." << endl;
            return;
        }
    }

    // Показываем список доступных труб
    cout << "Found " << availablePipes.size() << " available pipe(s):" << endl;
    cout << "--------------------------------" << endl;

    for (size_t i = 0; i < availablePipes.size(); i++)
    {
        const Pipe &pipe = availablePipes[i];
        cout << i + 1 << ". ID: " << pipe.getId()
             << " | Name: " << pipe.getName()
             << " | Length: " << pipe.getLength() << " km"
             << " | Status: " << (pipe.isUnderRepair() ? "In repair" : "OK") << endl;
    }

    cout << availablePipes.size() + 1 << ". Enter pipe ID manually" << endl;
    cout << availablePipes.size() + 2 << ". Create NEW pipe" << endl;
    cout << "0. Cancel" << endl;
    cout << "--------------------------------" << endl;
    cout << "Choice: ";

    int pipeChoice = getIntegerInput("");

    if (pipeChoice == 0)
    {
        cout << "Operation cancelled." << endl;
        return;
    }
    else if (pipeChoice == (int)availablePipes.size() + 1)
    {
        // Ручной ввод ID
        cout << "Enter pipe ID: ";
        int pipeId = getIntegerInput("");

        if (network.connectStations(fromStation, toStation, diameter, pipeId))
        {
            cout << "✅ Connection successful!" << endl;
        }
        else
        {
            cout << "❌ Failed to connect stations." << endl;
        }
    }
    else if (pipeChoice == (int)availablePipes.size() + 2)
    {
        // Создание новой трубы
        cout << "\nCreating new pipe..." << endl;
        network.addPipe();
        cout << "Please connect stations again after creating pipe." << endl;
    }
    else if (pipeChoice >= 1 && pipeChoice <= (int)availablePipes.size())
    {
        // Выбор трубы из списка
        int selectedPipeId = availablePipes[pipeChoice - 1].getId();

        if (network.connectStations(fromStation, toStation, diameter, selectedPipeId))
        {
            cout << "✅ Connection successful!" << endl;
        }
        else
        {
            cout << "❌ Failed to connect stations." << endl;
        }
    }
    else
    {
        cout << "Invalid choice!" << endl;
    }
}

void disconnectStationsMenu(GasNetwork &network)
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "         DISCONNECT STATIONS" << endl;
    cout << "════════════════════════════════════════" << endl;

    // Показываем текущие соединения
    network.getGraph().display();

    if (network.getGraph().isEmpty())
    {
        cout << "No connections to disconnect." << endl;
        return;
    }

    int fromStation = getIntegerInput("\nEnter SOURCE station ID: ");
    int toStation = getIntegerInput("Enter DESTINATION station ID: ");

    network.disconnectStations(fromStation, toStation);
}

// НОВЫЕ ФУНКЦИИ МЕНЮ

void shortestPathMenu(GasNetwork &network)
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "    РАСЧЕТ КРАТЧАЙШЕГО ПУТИ" << endl;
    cout << "════════════════════════════════════════" << endl;

    // Показываем доступные станции
    network.getPipelineNetwork().displayStationIds();

    int sourceStation = getIntegerInput("\nВведите ID станции-источника: ");
    int targetStation = getIntegerInput("Введите ID станции-цели: ");

    network.calculateShortestPath(sourceStation, targetStation);
}

void maxFlowMenu(GasNetwork &network)
{
    cout << "\n════════════════════════════════════════" << endl;
    cout << "    РАСЧЕТ МАКСИМАЛЬНОГО ПОТОКА" << endl;
    cout << "════════════════════════════════════════" << endl;

    // Показываем доступные станции
    network.getPipelineNetwork().displayStationIds();

    int sourceStation = getIntegerInput("\nВведите ID станции-источника: ");
    int targetStation = getIntegerInput("Введите ID станции-цели: ");

    network.calculateMaxFlow(sourceStation, targetStation);
}

int main()
{
    GasNetwork network;
    int choice;

    cout << "════════════════════════════════════════════════════════════════" << endl;
    cout << "          GAS TRANSMISSION NETWORK MANAGEMENT SYSTEM" << endl;
    cout << "════════════════════════════════════════════════════════════════" << endl;
    cout << "System initialized. Ready to manage pipeline network." << endl;

    do
    {
        displayMainMenu();
        choice = getIntegerInput("");

        switch (choice)
        {
        case 1:
            network.addPipe();
            break;
        case 2:
            network.addStation();
            break;
        case 3:
            network.displayAllObjects();
            break;
        case 4:
        {
            network.getPipelineNetwork().displayPipeIds();
            int id = getIntegerInput("Enter pipe ID to edit: ");
            network.editPipe(id);
            break;
        }
        case 5:
        {
            network.getPipelineNetwork().displayStationIds();
            int id = getIntegerInput("Enter station ID to edit: ");
            network.editStation(id);
            break;
        }
        case 6:
        {
            network.getPipelineNetwork().displayPipeIds();
            int id = getIntegerInput("Enter pipe ID to delete: ");
            network.deletePipe(id);
            break;
        }
        case 7:
        {
            network.getPipelineNetwork().displayStationIds();
            int id = getIntegerInput("Enter station ID to delete: ");
            network.deleteStation(id);
            break;
        }
        case 8:
            searchPipesMenu(network);
            break;
        case 9:
            searchStationsMenu(network);
            break;
        case 10:
        {
            string name = getStringInput("Enter pipe name to search for batch edit: ");
            vector<int> results = network.getPipelineNetwork().findPipesByName(name);
            if (!results.empty())
            {
                network.getPipelineNetwork().batchEditPipes(results);
            }
            else
            {
                cout << "No pipes found for batch editing." << endl;
            }
            break;
        }
        case 11:
            connectStationsMenu(network);
            break;
        case 12:
            disconnectStationsMenu(network);
            break;
        case 13:
            network.displayNetwork();
            break;
        case 14:
            network.performTopologicalSort();
            break;
        case 15: // Кратчайший путь
            shortestPathMenu(network);
            break;
        case 16: // Максимальный поток
            maxFlowMenu(network);
            break;
        case 17:
        {
            string filename = getStringInput("Enter filename to save (without extension): ");
            network.saveToFile(filename);
            network.saveNetworkToFile(filename);
            break;
        }
        case 18:
        {
            string filename = getStringInput("Enter filename to load (without extension): ");
            network.loadFromFile(filename);
            network.loadNetworkFromFile(filename);
            break;
        }
        case 19:
            network.displayNetworkStatus();
            break;
        case 0:
            cout << "\n════════════════════════════════════════" << endl;
            cout << "   Thank you for using the system!" << endl;
            cout << "   Goodbye!" << endl;
            cout << "════════════════════════════════════════" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }

    } while (choice != 0);

    return 0;
}