#pragma once
#include <string>
#include <vector>
#include "Map.hpp"
#include "Junction.hpp"

//This class takes care of when the program is run in interactive mode
class InteractiveMode
{
public:
    InteractiveMode(const std::string &, const std::string &);
    InteractiveMode(const InteractiveMode &);
    InteractiveMode &operator=(const InteractiveMode &);
    ~InteractiveMode() = default;
    bool readAndProcessCommand();

private:
    Map getMap();
    Map originalMap; //stores the original junctions and streets
    Junction *currentJunction; //stores where the user is currently placed
    std::vector<std::string> closedJunctions; //stores all currently closed junctions
};