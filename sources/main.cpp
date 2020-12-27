#include "../lib/Junction.hpp"
#include <fstream>
#include <iostream>
#include "../lib/Map.hpp"

int main()
{
    std::ifstream fin{"../test.txt"};
    Map map;
    fin >> map;
    for (auto junct : map.getJunctions())
    {
        std::cout << junct->getName() << std::endl;
    }
    auto deadEnds = map.getDeadends();
    for (auto deadend : deadEnds)
    {
        std::cout << deadend.first << " " << deadend.second << std::endl;
    }
    return 0;
}