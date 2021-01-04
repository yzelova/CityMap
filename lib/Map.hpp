#pragma once
#include <vector>
#include <fstream>
#include <optional>
#include "Junction.hpp"

class Map
{
public:
    friend std::ifstream &operator>>(std::ifstream &, Map &);

    bool hasPath(std::string, std::string);
    bool canReachEveryOtherJunction(std::string);
    bool hasCyclicWalkFromJunction(std::string);
    int getStreetsCount();
    std::optional<std::vector<std::string>> getEulerWalk();
    std::vector<std::pair<std::string, std::string>> getDeadends();

    std::vector<Junction *> getJunctions() const;

private:
    bool hasEulerianCycle();
    std::string readJunctionName(std::string &);
    double readDistance(std::string &);
    Junction *getJunctionByName(const std::string &);
    int reachableJunctionsCount(Junction *, std::vector<Junction *> &);
    std::vector<Junction *> junctions;
};