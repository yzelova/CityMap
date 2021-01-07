#include "../lib/Junction.hpp"
#include "../lib/Street.hpp"

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

void Junction::addStreet(const Street &street)
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

void Junction::removeStreet(const Street &street)
{
    street.begin->outdegree--;
    street.end->indegree--;
    streets.erase(std::find_if(streets.begin(), streets.end(), [&street](auto vecStreet) {
        return (
                   street.begin->getName().compare(vecStreet.begin->getName()) == 0 &&
                   street.end->getName().compare(vecStreet.end->getName())) == 0;
    }));
    return;
}