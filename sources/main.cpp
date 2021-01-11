#include "../lib/Junction.hpp"
#include <fstream>
#include <iostream>
#include <exception>
#include "../lib/Map.hpp"
#include "../lib/InteractiveMode.hpp"

int main(int argc, char *argv[])
{
    if (argc == 4 && std::strcmp(argv[1], "-i") == 0)
    {
        std::string filePath{argv[2]};
        std::string startJunction{argv[3]};
        try
        {
            InteractiveMode interactive{filePath, startJunction};
            while (interactive.readAndProcessCommand())
            {
            }
        }
        catch (std::runtime_error &e)
        {
            if (std::strcmp(e.what(), ("can not open file")) == 0 || std::strcmp(e.what(), "does not exist") == 0)
            {
                return 0;
            }
            else
                throw e;
        }
    }
    return 0;
}