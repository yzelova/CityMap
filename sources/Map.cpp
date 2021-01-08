#include "../lib/Map.hpp"
#include "../lib/utils.hpp"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>

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

Map::Map(const Map &other)
{
    copy(other);
}

Map::~Map()
{
    for (auto junction : junctions)
    {
        delete junction;
    }
}

/// Чете и парсва карта от файл по зададения формат в условието
std::ifstream &operator>>(std::ifstream &in, Map &map)
{
    std::string line;
    while (std::getline(in, line))
    {
        auto junctionName = map.readString(line);
        if (!junctionName)
        {
            break;
        }
        auto junction = map.getJunctionByName(junctionName.value());
        if (junction == nullptr)
        {
            junction = new Junction(junctionName.value());
            map.junctions.push_back(junction);
        }
        while (!line.empty())
        {
            auto name = map.readString(line);
            if (!name)
            {
                break;
            }
            auto distance = map.readDistance(line);
            if (!distance)
            {
                std::cout << "Parsing error!\n";
            }
            auto endJunction = map.getJunctionByName(name.value());
            if (endJunction == nullptr)
            {
                endJunction = new Junction(name.value());
                map.junctions.push_back(endJunction);
            }
            Street street;
            street.distance = distance.value();
            street.begin = junction;
            street.end = endJunction;
            junction->addStreet(street);
        }
    }
    return in;
}

/// Помощен метод който чете име на кръстовище
std::optional<std::string> Map::readString(std::string &line)
{
    while (!line.empty() && line[0] == ' ')
    {
        line.erase(0, 1);
    }
    std::string name;
    if (line.empty())
    {
        return {};
    }
    while (!line.empty() && line[0] != ' ')
    {
        name += line[0];
        line.erase(0, 1);
    }
    return name;
}

/// Помощен метод който чете разстояние
std::optional<double> Map::readDistance(std::string &line)
{
    try
    {
        auto raw = readString(line);
        if(!raw)
        {
            return {};
        }
        return std::stod(raw.value());
    }
    catch (...)
    {
        return {};
    }
}

/// Проверява дали има път между две крастовища, използва BFS
bool Map::hasPath(const std::string &startJunctionName, const std::string &endJunctionName)
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

/// Проверява дали всички други кръстовища са достижими от подаденото
bool Map::canReachEveryOtherJunction(const std::string &junctionName)
{
    auto currentJunction = getJunctionByName(junctionName);
    std::vector<Junction *> reachedJunctions{currentJunction};
    int reachedJunctionsCount = reachableJunctionsCount(currentJunction, reachedJunctions);
    return reachedJunctionsCount == junctions.size();
}

/// Брои достижимите кръстовища чрез DFS
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

/// Проверява дали ако тръгнем от едно кръстовище, можем да се върнем обратно в него - търси цикъл в графа, ползва BFS
bool Map::hasCyclicWalkFromJunction(const std::string &junctionName)
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

/**
 *  Търси всички задънени улици.
 *  Единственият метод, който изисква достъп до множеството от улици.
 *  Причината да не се пази списъкът от улици директно е, че е напълно излишно
 *  (дали ще имаме списък от улици, който да итерираме, или ще обходим всички кръстовища
 *  с всички улици, излизащи от тях, не променя броят на итерациите), 
 *  а при възможно добавяне/махане на кръстовища (което се поддържа заради интерактивния режим)
 *  ще трябва да се актуализира още един списък - този с улиците. 
 * */
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

/// Помощен метод който връща указател към кръстовище по неговото име. Ако не го намери - nullptr
Junction *Map::getJunctionByName(const std::string &junctiontName)
{
    auto junction = std::find_if(junctions.begin(),
                                 junctions.end(),
                                 [junctiontName](Junction *junction) { return (junctiontName.compare(junction->getName())) == 0; });
    if (junction == junctions.end())
        return nullptr;
    return *junction;
}

/// Намира туристическа обиколка на града, която минава по всички улици - търси цикъл на Ойлер
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

/**
 * Брои улиците - неоптимално, вместо да държи броя на улиците, но се ползва само   
 * при търсенето на Ойлеров цикъл.
 * */
int Map::getStreetsCount()
{
    int streetsCount{0};
    for (auto junction : junctions)
    {
        streetsCount += junction->getStreets().size();
    }
    return streetsCount;
}

/**
 * Затваря кръстовища. Това включва и махане на всички улици, които излизат от тях.
 * Тази функция връща нов Map, а не модифицира сегашния, тъй като затварянето на
 * кръстовища се реализира като триенето им от картата. Не искаме да губим 
 * оригиналната карта обаче, тъй като отварянето на кръстовища ще изисква възстановяването
 * им - най-вече на улиците, излизащи от тях. Тоест, ако трием от оригиналната карта,
 * ще загубим оригиналните кръстовища и техните улици.
 * */
Map Map::closeJunctions(const std::vector<std::string> &closedJunctions)
{
    Map cpy{*this};
    for (auto name : closedJunctions)
    {
        auto junction = getJunctionByName(name);
        cpy.junctions.erase(std::find_if(cpy.junctions.begin(), cpy.junctions.end(), [&junction](auto vecJunction) {
            return junction->getName() == vecJunction->getName();
        }));
        for (auto junc : cpy.junctions)
        {
            Street street;
            street.begin = junc;
            street.end = junction;
            if (vectorContainsStreet(junc->getStreets(), street))
            {
                junc->removeStreet(street);
            }
        }
    }
    return cpy;
}

/// Взима 3те най-къси пътища между 2 точки, с подаден списък от затворени кръстовища
std::vector<std::pair<std::vector<std::string>, int>> Map::get3ShortestPaths(const std::string &begin, const std::string &end, const std::vector<std::string> &closedJunctions)
{
    auto cpy = closeJunctions(closedJunctions);
    return cpy.get3ShortestPaths(begin, end);
}

/**
 * Взима 3те най-къси пътища между 2 точки - използва модификация на алгоритъмът на Дийкстра:
 * За най-късият път използва стандартна реализация на алгоритъмът
 * За вторият най-къс път - премахва се всяко ребро 1 по 1 и се намира най-късият път в новополученият граф. 
 * Този път с най-малка обща дължина е 2рият най-къс път.
 * За третият най-къс път - Реброто, което е било премахнато за получаването на този път, се премахва и се прилага същата процедура
 * като за 2рият най-къс път.
 * */
std::vector<std::pair<std::vector<std::string>, int>> Map::get3ShortestPaths(const std::string &begin, const std::string &end)
{
    std::vector<std::pair<std::vector<std::string>, int>> result{};
    auto first = getShortestPath(getJunctionByName(begin), getJunctionByName(end));
    if (!first)
    {
        return result;
    }
    result.push_back(first.value());
    auto prev = first.value().first;
    Map alteredMap;
    for (int i = 0; i < 2; i++)
    {
        int minDistance = INT_MAX;
        std::optional<std::pair<std::vector<std::string>, int>> path{};
        for (auto junction : junctions)
        {
            for (auto street : junction->getStreets())
            {
                junction->removeStreet(street);
                auto minDistanceWithoutEdge = getShortestPath(getJunctionByName(begin), getJunctionByName(end));
                if (minDistanceWithoutEdge &&
                    minDistanceWithoutEdge.value().second < minDistance &&
                    minDistanceWithoutEdge.value().first != prev)
                {
                    path = minDistanceWithoutEdge;
                    minDistance = minDistanceWithoutEdge.value().second;
                    alteredMap = *this;
                }
                junction->addStreet(street);
            }
        }
        if (!path)
        {
            return result;
        }
        prev = path.value().first;
        result.push_back(path.value());
    }

    return result;
}

/**
 * Реализира алгоритъм на Дийкстра
 * Ако не намери път - връща optional без value
 * Ако намери - връща пътя и дължината му
 * */
std::optional<std::pair<std::vector<std::string>, int>> Map::getShortestPath(Junction *begin, Junction *end)
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

/// Проверява дали съществува Ойлеров цикъл
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
