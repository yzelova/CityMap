#pragma once
#include <string>
#include <vector>
#include "Map.hpp"
#include "Junction.hpp"

class InteractiveMode
{
public:
    InteractiveMode(std::string, std::string);
    InteractiveMode(const InteractiveMode&);
    InteractiveMode& operator=(const InteractiveMode&);
    ~InteractiveMode() = default;
    bool readAndProcessCommand();

private:
    Map getMap();
    Map originalMap;
    Junction *currentJunction;
    std::vector<std::string> closedJunctions;
};