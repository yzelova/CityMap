#include "../lib/Junction.hpp"

Junction::Junction(const std::string &name)
{
    this->name = name;
}

std::string Junction::getName() const
{
    return name;
}

void Junction::addStreet(Street street)
{
    streets.push_back(street);
}