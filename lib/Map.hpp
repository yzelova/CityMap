#pragma once
#include <vector>
#include <fstream>
#include <optional>
#include "Junction.hpp"

class Map
{
public:
    friend std::ifstream &operator>>(std::ifstream &, Map &);
    Map& operator=(const Map&);

    bool hasPath(std::string, std::string);
    bool canReachEveryOtherJunction(std::string);
    bool hasCyclicWalkFromJunction(std::string);
    int getStreetsCount();
    std::vector<std::pair<std::vector<std::string>, int>> getKShortestPaths(int, std::string, std::string);
    std::optional<std::vector<std::string>> getEulerWalk();
    std::vector<std::pair<std::string, std::string>> getDeadends();

    std::vector<Junction *> getJunctions() const;

private:
    void copy(const Map&);
    std::optional<std::pair<std::vector<std::string>, int>> getKShortestPaths(int, int, Junction*, Junction*);
    bool hasEulerianCycle();
    std::string readJunctionName(std::string &);
    double readDistance(std::string &);
    Junction *getJunctionByName(const std::string &);
    int reachableJunctionsCount(Junction *, std::vector<Junction *> &);
    std::vector<Junction *> junctions;
};