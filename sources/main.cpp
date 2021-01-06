#include "../lib/Junction.hpp"
#include <fstream>
#include <iostream>
#include "../lib/Map.hpp"
#include "../lib/InteractiveMode.hpp"

int main(int argc, char *argv[])
{
    if (argc == 4 && std::strcmp(argv[1], "-i") == 0)
    {
        std::string filePath{argv[2]};
        std::string startJunction{argv[3]};
        InteractiveMode interactive{filePath, startJunction};
        while(interactive.readAndProcessCommand())
        {

        }
    }
    std::ifstream fin{"../test.txt"};
    Map map;
    fin >> map;
    for (auto junct : map.getJunctions())
    {
        std::cout << junct->getName() <<" "<< junct->getStreets().size() << std::endl;
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
    auto paths = map.getKShortestPaths(2, "NDK", "BSFS");
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
    auto pathsWithCLosedJunctions = map.getKShortestPaths(2, "NDK", "BSFS", std::vector<std::string>{"neshtosi"});
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