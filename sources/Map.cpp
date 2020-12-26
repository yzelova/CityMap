#include "../lib/Map.hpp"
#include <iostream>

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