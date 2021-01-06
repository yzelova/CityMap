#include "../lib/InteractiveMode.hpp"
#include <iostream>
#include <cassert>

InteractiveMode::InteractiveMode(std::string filePath, std::string startJunctionName)
{
    std::ifstream fin{"test.txt"};
    assert(fin.good());
    fin >> originalMap;
    currentJunction = originalMap.getJunctionByName(startJunctionName);
}

bool InteractiveMode::readAndProcessCommand()
{
    auto currentMap = getMap();
    std::string command;
    std::cin >> command;
    if (command.compare("location") == 0)
    {
        std::cout << currentJunction->getName() << std::endl;
    }
    else if (command.compare("change") == 0)
    {
        std::string newJunctionName;
        std::cin >> newJunctionName;
        auto newJunction = currentMap.getJunctionByName(newJunctionName);
        if (newJunction == nullptr)
        {
            std::cout << "Junction with that name does not exist or is closed!\n";
            return true;
        }
        currentJunction = newJunction;
    }
    else if (command.compare("neighbours") == 0)
    {
        std::cout << "Neighbours count: " << currentJunction->getStreets().size() << std::endl;
        for (auto street : currentJunction->getStreets())
        {
            std::cout << street.end->getName() << " ";
        }
        std::cout << std::endl;
    }
    else if (command.compare("move") == 0)
    {
        std::string moveToName;
        std::cin >> moveToName;
        auto path = currentMap.getKShortestPaths(1, currentJunction->getName(), moveToName);
        if (path.size() == 0)
        {
            std::cout << "No path from " << currentJunction->getName() << " to " << moveToName << " or it is closed!\n";
            return true;
        }
        auto fastestPath = path[0];
        std::cout << "Fastest route found: " << fastestPath.second << std::endl;
        for (auto junctionName : fastestPath.first)
        {
            std::cout << junctionName << " ";
        }
        std::cout << std::endl;
        currentJunction = currentMap.getJunctionByName(moveToName);
    }
    else if (command.compare("close") == 0)
    {
        std::string junctionName;
        std::cin >> junctionName;
        auto found = originalMap.getJunctionByName(junctionName) != nullptr;
        if (!found)
        {
            std::cout << "No such junction exists!\n";
            return true;
        }
        closedJunctions.push_back(junctionName);
    }
    else if (command.compare("open") == 0)
    {
        std::string junctionName;
        std::cin >> junctionName;
        auto found = originalMap.getJunctionByName(junctionName) != nullptr;
        if (!found)
        {
            std::cout << "No such junction exists!\n";
            return true;
        }
        closedJunctions.erase(std::find_if(closedJunctions.begin(), closedJunctions.end(), [junctionName](auto vecJunction) {
            return vecJunction.compare(junctionName) == 0;
        }));
    }
    else if (command.compare("closed") == 0)
    {
        for (auto closed : closedJunctions)
        {
            std::cout << closed << " ";
        }
        std::cout << std::endl;
    }
    else if (command.compare("tour") == 0)
    {
        auto tour = currentMap.getEulerWalk();
        if (!tour)
        {
            std::cout << "No tour is available!\n";
            return true;
        }
        for (auto junction : tour.value())
        {
            std::cout << junction << " ";
        }
        std::cout << std::endl;
    }
    else if (command.compare("quit") == 0)
    {
        return false;
    }
    else
    {
        std::cout << "Invalid command" << std::endl;
    }
    return true;
}

Map InteractiveMode::getMap()
{
    return originalMap.closeJunctions(closedJunctions);
}

InteractiveMode::InteractiveMode(const InteractiveMode &other)
{
    originalMap = other.originalMap;
    currentJunction = originalMap.getJunctionByName(other.currentJunction->getName());
    closedJunctions = other.closedJunctions;
}

InteractiveMode &InteractiveMode::operator=(const InteractiveMode &other)
{
    if (this != &other)
    {
        originalMap = other.originalMap;
        currentJunction = originalMap.getJunctionByName(other.currentJunction->getName());
        closedJunctions = other.closedJunctions;
    }
    return *this;
}