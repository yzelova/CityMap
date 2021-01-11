#pragma once
#include <string>
#include <vector>
#include "Map.hpp"
#include "Junction.hpp"

/// Грижи се за интерактивният режим
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
    Map originalMap; //оригиналната карта, заредена от подадения файл
    Junction *currentJunction; //позицията, на която потребителят се намира в момента
    std::vector<std::string> closedJunctions; //затворените кръстовища
};