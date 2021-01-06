#pragma once
#include <vector>
#include <fstream>
#include <optional>
#include "Junction.hpp"

class InteractiveMode;

class Map
{
public:
    friend std::ifstream &operator>>(std::ifstream &, Map &);
    Map() = default;
    Map &operator=(const Map &);
    Map(const Map &);
    ~Map();

    Map closeJunctions(std::vector<std::string>);

    bool hasPath(std::string, std::string);
    bool canReachEveryOtherJunction(std::string);
    bool hasCyclicWalkFromJunction(std::string);
    int getStreetsCount();
    std::vector<std::pair<std::vector<std::string>, int>> get3ShortestPaths(std::string, std::string);
    std::vector<std::pair<std::vector<std::string>, int>> get3ShortestPaths(std::string, std::string, std::vector<std::string>);
    std::optional<std::vector<std::string>> getEulerWalk();
    std::vector<std::pair<std::string, std::string>> getDeadends();
    std::vector<Junction *> getJunctions() const;

private:
    void copy(const Map &);
    std::optional<std::pair<std::vector<std::string>, int>> getShortestPath(Junction *, Junction *);
    bool hasEulerianCycle();
    std::string readJunctionName(std::string &);
    double readDistance(std::string &);
    int reachableJunctionsCount(Junction *, std::vector<Junction *> &);
    std::vector<Junction *> junctions;
    Junction *getJunctionByName(const std::string &);
    friend class InteractiveMode;
};