#include "../lib/Map.hpp"
#include <iostream>
#include <queue>

std::ifstream &operator>>(std::ifstream &in, Map &map)
{
    std::string line;
    while (std::getline(in, line))
    {
        auto junctionName = map.readJunctionName(line);
        auto junction = map.getJunctionByName(junctionName);
        if (junction == nullptr)
        {
            junction = new Junction(junctionName);
            map.junctions.push_back(junction);
        }
        while (!line.empty())
        {
            auto name = map.readJunctionName(line);
            auto distance = map.readDistance(line);
            auto endJunction = map.getJunctionByName(name);
            if (endJunction == nullptr)
            {
                endJunction = new Junction(name);
                map.junctions.push_back(endJunction);
            }
            Street street;
            street.distance = distance;
            street.end = endJunction;
            junction->addStreet(street);
        }
    }
    return in;
}

bool vectorContainsJunction(const std::vector<Junction *> &v, Junction *junction)
{
    return std::find_if(v.begin(),
                        v.end(),
                        [junction](auto passedJunction) { return passedJunction->getName().compare(junction->getName()) == 0; }) != v.end();
}

bool Map::hasPath(std::string startJunctionName, std::string endJunctionName)
{
    auto startJunction = getJunctionByName(startJunctionName);
    auto endJunction = getJunctionByName(endJunctionName);
    std::vector<Junction *> passed;
    std::queue<Junction *> bfsQueue;
    bfsQueue.push(startJunction);
    passed.push_back(startJunction);
    while (!bfsQueue.empty())
    {
        auto currentJunction = bfsQueue.front();
        bfsQueue.pop();
        for (auto street : currentJunction->getStreets())
        {
            if (street.end == endJunction)
                return true;
            if (vectorContainsJunction(passed, street.end))
            {
                bfsQueue.push(street.end);
            }
        }
    }
    return false;
}

bool Map::canReachEveryOtherJunction(std::string junctionName)
{
    auto currentJunction = getJunctionByName(junctionName);
    std::vector<Junction*> reachedJunctions{currentJunction};
    int reachedJunctionsCount = reachableJunctionsCount(currentJunction, reachedJunctions);
    std::cout<<reachedJunctionsCount<<std::endl;
    return reachedJunctionsCount == junctions.size();
}

int Map::reachableJunctionsCount(Junction* currentJunction, std::vector<Junction*>& reachedJunctions)
{
    if(currentJunction->getStreets().size() == 0)
    {
        return 1;
    }
    int count{0};
    for(auto street : currentJunction->getStreets())
    {
        std::cout<<street.end->getName()<<std::endl;
        if(vectorContainsJunction(reachedJunctions, street.end))
        {
            continue;
        }
        reachedJunctions.push_back(street.end);
        count += reachableJunctionsCount(street.end, reachedJunctions);
        reachedJunctions.pop_back();
    }
    return count + 1;
}

std::vector<std::pair<std::string, std::string>> Map::getDeadends()
{
    std::vector<std::pair<std::string, std::string>> deadends;
    for (auto junction : junctions)
    {
        for (auto street : junction->getStreets())
        {
            auto nextJunction = street.end;
            if (nextJunction->getStreets().size() == 0)
            {
                deadends.push_back({junction->getName(), nextJunction->getName()});
            }
        }
    }
    return deadends;
}

Junction *Map::getJunctionByName(const std::string &junctiontName)
{
    auto junction = std::find_if(junctions.begin(),
                                 junctions.end(),
                                 [junctiontName](Junction *junction) { return (junctiontName.compare(junction->getName())) == 0; });
    if (junction == junctions.end())
        return nullptr;
    return *junction;
}

std::string Map::readJunctionName(std::string &line)
{
    while (!line.empty() && line[0] == ' ')
    {
        line.erase(0, 1);
    }
    std::string name;
    while (!line.empty() && line[0] != ' ')
    {
        name += line[0];
        line.erase(0, 1);
    }
    return name;
}

double Map::readDistance(std::string &line)
{
    while (!line.empty() && line[0] == ' ')
    {
        line.erase(0, 1);
    }
    std::string strDistance;
    while (!line.empty() && line[0] != ' ')
    {
        strDistance += line[0];
        line.erase(0, 1);
    }
    return std::stod(strDistance);
}

std::vector<Junction *> Map::getJunctions() const
{
    return junctions;
}