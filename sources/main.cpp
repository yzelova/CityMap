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
    std::cout << map.hasPath("Popa", "NDK") << std::endl;
    return 0;
}