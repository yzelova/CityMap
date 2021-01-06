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
    std::cout << map.canReachEveryOtherJunction("5Kiosheta") << std::endl;
    std::cout << map.hasCyclicWalkFromJunction("Popa") << std::endl;
    //std::cout<<map.hasEulerianCycle()<<std::endl;
    auto walk = map.getEulerWalk();
    if (walk.has_value())
    {
        for (auto name : walk.value())
        {
            std::cout << name << " " << std::endl;
        }
    }
    else
    {
        std::cout << "No path" << std::endl;
    }
    auto paths = map.getKShortestPaths(2, "NDK", "Popa");
    for (auto path : paths)
    {
        for (auto junction : path.first)
        {
            std::cout << junction << " ";
        }
        std::cout << path.second << " ";
        std::cout << std::endl;
    }
    std::cout<<"---------------------------------------->\n";
    auto pathsWithCLosedJunctions = map.getKShortestPaths(2, "NDK", "Popa", std::vector<std::string>{"neshtosi"});
    for (auto path : pathsWithCLosedJunctions)
    {
        for (auto junction : path.first)
        {
            std::cout << junction << " ";
        }
        std::cout << path.second << " ";
        std::cout << std::endl;
    }
    return 0;
}