#include "../lib/Junction.hpp"

Junction::Junction(const std::string &name)
{
    this->name = name;
}

std::vector<Street> Junction::getStreets() const
{
    return streets;
}

std::string Junction::getName() const
{
    return name;
}

void Junction::addStreet(Street street)
{
    street.begin->outdegree++;
    street.end->indegree++;
    streets.push_back(street);
}

int Junction::getIndegree() const
{
    return indegree;
}

int Junction::getOutdegree() const
{
    return outdegree;
}

void Junction::removeStreet(Street street)
{
    streets.erase(std::remove_if(streets.begin(), streets.end(), [street](auto vecStreet) {
        return (
                   street.begin->getName().compare(vecStreet.begin->getName()) == 0 &&
                   street.end->getName().compare(vecStreet.end->getName())) == 0;
    }));
    return;
}