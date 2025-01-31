#include "../lib/InteractiveMode.hpp"
#include "../lib/Street.hpp"
#include <iostream>
#include <exception>
#include <cassert>

//отваря файл, прочита карта и намира началното кръстовище
InteractiveMode::InteractiveMode(const std::string &filePath, const std::string &startJunctionName)
{
    std::ifstream fin{filePath};
    if(!fin.good())
    {
        std::cout<<"That file does not exist or can not be opened!\n";
        throw(std::runtime_error("can not open file"));
    }
    fin >> originalMap;
    currentJunction = originalMap.getJunctionByName(startJunctionName);
    if(!currentJunction)
     {
        std::cout<<"Start junction does not exist in the map!\n";
        throw(std::runtime_error("does not exist"));
    }
}

//Парсва и изпълнява команда
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
        auto newJunction = currentMap.getJunctionByName(newJunctionName); //проверяваме дали в картата със затворени кръстовища има такова кръстовище
        if (newJunction == nullptr)
        {
            std::cout << "Junction with that name does not exist or is closed!\n";
            return true;
        }
        newJunction = originalMap.getJunctionByName(newJunctionName); //взимаме кръстовището от оригиналната карта
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
        auto path = currentMap.get3ShortestPaths(currentJunction->getName(), moveToName);
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
        auto newJunction = originalMap.getJunctionByName(moveToName);
        if(newJunction == nullptr)
        {
            std::cout<<"This junction does not exist\n";
            return true;
        }
        currentJunction = newJunction;
    }
    else if (command.compare("close") == 0)
    {
        std::string junctionName;
        std::cin >> junctionName;
        auto junction = originalMap.getJunctionByName(junctionName);
        if (!junction)
        {
            std::cout<<"This junction does not exist\n";
            return true;
        }
        if(junction == currentJunction)
        {
            std::cout << "Cannot close current junction!\n";
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
            std::cout<<"This junction does not exist\n";
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

/// Взима карта без затворените кръстовища
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