#pragma once
#include <string>
#include <vector>
#include <algorithm>

struct Street;

class Junction
{
public:
    Junction(const std::string &);
    void addStreet(const Street& street);
    std::vector<Street> getStreets() const;
    std::string getName() const;
    int getIndegree() const;
    int getOutdegree() const;
    void removeStreet(const Street& street);

private:
    int indegree{0};
    int outdegree{0};
    std::string name;
    std::vector<Street> streets;
};

struct Street
{
    Junction *begin;
    Junction *end;
    double distance;
};