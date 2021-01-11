#pragma once
#include <vector>
#include <fstream>
#include <optional>
#include "Junction.hpp"

class InteractiveMode;


/**
 * Представя граф. Има методи за всички задачи, поставени в условието.
 * Има вектор от указатели към кръстовища. Няма нужда да пази множеството на улиците,
 * тъй като не би се използвало никъде, а реализираните алгоритми ги достъпват
 * чрез кръстовищата.
 * NOTE: Всички реализирани алгоритми приемат аргументи и връщат низове, които представят имена 
 * на кръстовища, вместо указатели към тях, например. Причината за това е че този клас
 * е на най-горното ниво на абстракция на проекта, т.е. очаква се че потребителят не знае
 * за вътрешното представяне на картата и факта, че съществува клас като Junction. Също така,
 * всички кръстовища имат уникално име (в контекста на истински град), което е хубав начин лесно
 * да се идентифицират по това.
 * */
class Map
{
public:
    friend std::ifstream &operator>>(std::ifstream &, Map &);
    Map() = default;
    Map &operator=(const Map &);
    Map(const Map &);
    ~Map();

    Map closeJunctions(const std::vector<std::string>&) const;

    bool hasPath(const std::string&, const std::string&) const;
    bool canReachEveryOtherJunction(const std::string&) const;
    bool hasCyclicWalkFromJunction(const std::string&) const;
    int getStreetsCount() const;
    std::vector<std::pair<std::vector<std::string>, int>> get3ShortestPaths(const std::string&,const  std::string&) const;
    std::vector<std::pair<std::vector<std::string>, int>> get3ShortestPaths(const std::string&,const  std::string&,const  std::vector<std::string>&) const;
    std::optional<std::vector<std::string>> getEulerWalk() const;
    std::vector<std::pair<std::string, std::string>> getDeadends() const;
private:
    void copy(const Map &);
    std::optional<std::pair<std::vector<std::string>, int>> getShortestPath(Junction *, Junction *) const;
    bool hasEulerianCycle() const;
    std::optional<std::string> readString(std::string &) const;
    std::optional<double> readDistance(std::string &) const;
    int reachableJunctionsCount(Junction *,std::vector<Junction *> &) const;
    Junction *getJunctionByName(const std::string &) const ;
    std::vector<Junction *> junctions;
    friend class InteractiveMode;
};