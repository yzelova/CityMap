#include "../lib/Map.hpp"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>

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
            street.begin = junction;
            street.end = endJunction;
            junction->addStreet(street);
        }
    }
    return in;
}

bool vectorContainsStreet(const std::vector<Street> &v, Street street)
{
    return std::find_if(v.begin(),
                        v.end(),
                        [street](auto passedStreet) { return (passedStreet.begin->getName().compare(street.begin->getName()) == 0 && passedStreet.end->getName().compare(street.end->getName()) == 0); }) != v.end();
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
    std::vector<Junction *> reachedJunctions{currentJunction};
    int reachedJunctionsCount = reachableJunctionsCount(currentJunction, reachedJunctions);
    return reachedJunctionsCount == junctions.size();
}

int Map::reachableJunctionsCount(Junction *currentJunction, std::vector<Junction *> &reachedJunctions)
{
    if (currentJunction->getStreets().size() == 0)
    {
        return 1;
    }
    int count{0};
    for (auto street : currentJunction->getStreets())
    {
        if (vectorContainsJunction(reachedJunctions, street.end))
        {
            continue;
        }
        reachedJunctions.push_back(street.end);
        count += reachableJunctionsCount(street.end, reachedJunctions);
        reachedJunctions.pop_back();
    }
    return count + 1;
}

bool Map::hasCyclicWalkFromJunction(std::string junctionName)
{
    auto junction = getJunctionByName(junctionName);
    if (junction->getStreets().size() == 0)
    {
        return false;
    }
    std::vector<Street> passedStreets;
    std::queue<Junction *> junctions;
    junctions.push(junction);
    while (!junctions.empty())
    {
        auto currentJunction = junctions.front();
        for (auto street : currentJunction->getStreets())
        {
            if (street.end->getName().compare(junctionName) == 0)
            {
                return true;
            }
            if (vectorContainsStreet(passedStreets, street))
            {
                continue;
            }
            passedStreets.push_back(street);
            junctions.push(street.end);
        }
        junctions.pop();
    }
    return false;
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
std::optional<std::vector<std::string>> Map::getEulerWalk()
{
    bool hasCycle = hasEulerianCycle();
    if (!hasCycle)
    {
        return {};
    }
    if (junctions.size() == 0)
    {
        return {std::vector<std::string>{}};
    }
    Junction *begin = junctions[0];
    std::stack<Junction *> traversalStack;
    traversalStack.push(begin);
    int streetsCount = getStreetsCount();
    std::vector<Street> usedStreets;
    while (usedStreets.size() != streetsCount)
    {
        auto currentJunction = traversalStack.top();
        bool flag = false;
        for (auto street : currentJunction->getStreets())
        {
            if (!vectorContainsStreet(usedStreets, street))
            {
                usedStreets.push_back(street);
                traversalStack.push(street.end);
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            traversalStack.pop();
        }
    }
    std::vector<std::string> walk{};
    for (int cnt = 0; !traversalStack.empty(); cnt++)
    {
        walk.push_back(traversalStack.top()->getName());
        traversalStack.pop();
    }
    std::reverse(walk.begin(), walk.end());
    return walk;
}

int Map::getStreetsCount()
{
    int streetsCount{0};
    for (auto junction : junctions)
    {
        streetsCount += junction->getStreets().size();
    }
    return streetsCount;
}

std::vector<std::pair<std::vector<std::string>, int>> Map::getKShortestPaths(int k, std::string begin, std::string end)
{
    std::vector<std::pair<std::vector<std::string>, int>> result{};
    auto first = getKShortestPaths(0, k, getJunctionByName(begin), getJunctionByName(end));
    if (!first)
    {
        return result;
    }
    result.push_back(first.value());
    int minDistance = INT_MAX;
    std::optional<std::pair<std::vector<std::string>, int>> second{};
    Map alteredMap;
    for (auto junction : junctions)
    {
        for (auto street : junction->getStreets())
        {
            junction->removeStreet(street);
            auto minDistanceWithoutEdge = getKShortestPaths(0, k, getJunctionByName(begin), getJunctionByName(end));
            if (minDistanceWithoutEdge &&
                minDistanceWithoutEdge.value().second < minDistance &&
                minDistanceWithoutEdge.value().first != first.value().first)
            {
                second = minDistanceWithoutEdge;
                minDistance = minDistanceWithoutEdge.value().second;
                alteredMap = *this; //VERY REFACTOR!!!
            }
            junction->addStreet(street);
        }
    }
    if (!second)
    {
        return result;
    }
    result.push_back(second.value());
    minDistance = INT_MAX;
    std::optional<std::pair<std::vector<std::string>, int>> third{};
    for (auto junction : alteredMap.junctions)
    {
        for (auto street : junction->getStreets())
        {
            junction->removeStreet(street);
            auto minDistanceWithoutEdge = alteredMap.getKShortestPaths(0, k,
                                                                       alteredMap.getJunctionByName(begin),
                                                                       alteredMap.getJunctionByName(end));

            if (minDistanceWithoutEdge &&
                minDistanceWithoutEdge.value().second < minDistance &&
                minDistanceWithoutEdge.value().first != second.value().first)
            {
                third = minDistanceWithoutEdge;
                minDistance = minDistanceWithoutEdge.value().second;
            }
            junction->addStreet(street);
        }
    }
    if (!third)
    {
        return result;
    }
    result.push_back(third.value());

    return result;
}

std::optional<std::pair<std::vector<std::string>, int>> Map::getKShortestPaths(int current, int k, Junction *begin, Junction *end)
{
    int junctionsCount = junctions.size();
    std::map<Junction *, int> distances;
    std::map<Junction *, Junction *> parents;
    for (auto junction : junctions)
    {
        distances[junction] = INT_MAX;
        parents[junction] = nullptr;
    }
    distances[begin] = 0;
    parents[begin] = nullptr;
    std::vector<Junction *> passedJunctions;
    while (passedJunctions.size() != junctionsCount)
    {
        int minDistance = INT_MAX;
        Junction *nextJunction{nullptr};

        for (auto junction : junctions)
        {
            if (!vectorContainsJunction(passedJunctions, junction))
            {
                if (distances[junction] < minDistance)
                {
                    minDistance = distances[junction];
                    nextJunction = junction;
                }
            }
        }
        if (nextJunction == nullptr)
        {
            return {};
        }
        passedJunctions.push_back(nextJunction);
        for (auto street : nextJunction->getStreets())
        {
            auto adj = street.end;
            if (street.distance + distances[nextJunction] < distances[adj])
            {
                distances[adj] = street.distance + distances[nextJunction];
                parents[adj] = nextJunction;
            }
        }
    }
    if (end != begin && parents[end] == nullptr)
    {
        return {};
    }
    std::vector<std::string> currentPath;
    for (auto current = end; current != nullptr;)
    {
        currentPath.push_back(current->getName());
        current = parents[current];
    }
    std::reverse(currentPath.begin(), currentPath.end());

    return {{currentPath, distances[end]}};
}

bool Map::hasEulerianCycle()
{
    for (auto junction : junctions)
    {
        if (junction->getIndegree() != junction->getOutdegree())
        {
            return false;
        }
    }
    return true;
}

Map &Map::operator=(const Map &other)
{
    if (this != &other)
    {
        copy(other);
    }
    return *this;
}

void Map::copy(const Map &other)
{
    for (auto junction : other.junctions)
    {
        auto newJunction = new Junction(junction->getName());
        junctions.push_back(newJunction);
    }

    for (auto junction : other.junctions)
    {
        for (auto street : junction->getStreets())
        {
            Street newStreet;
            newStreet.begin = getJunctionByName(street.begin->getName());
            newStreet.end = getJunctionByName(street.end->getName());
            newStreet.distance = street.distance;
            getJunctionByName(street.begin->getName())->addStreet(newStreet);
        }
    }
}