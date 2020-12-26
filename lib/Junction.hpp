#pragma once
#include <string>
#include <vector>

class Street;

class Junction
{
public:
    Junction(const std::string &);
    void addStreet(Street street);
    std::vector<Street> getStreets() const;
    std::string getName() const;
private:
    std::string name;
    std::vector<Street> streets;
};

struct Street
{
    Junction *end;
    double distance;
};