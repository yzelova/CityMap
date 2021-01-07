#pragma once
#include <string>
#include <vector>
#include <algorithm>

struct Street;

/*
Представя връх на граф. Има вектор от ребра/улици, име, както и степен на входа/изхода, която се променя всеки път, когато 
се добавя/маха улица, чийто край/начало е този връх. 
*/
class Junction
{
public:
    Junction() = default;
    Junction(const std::string &);
    void addStreet(const Street &street);
    std::vector<Street> getStreets() const;
    std::string getName() const;
    int getIndegree() const;
    int getOutdegree() const;
    void removeStreet(const Street &street);

private:
    int indegree{0};
    int outdegree{0};
    std::string name;
    std::vector<Street> streets;
};
