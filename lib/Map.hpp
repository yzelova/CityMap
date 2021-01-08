#pragma once
#include <vector>
#include <fstream>
#include <optional>
#include "Junction.hpp"

class InteractiveMode;


/*
Представя граф. Има методи за всички задачи, поставени в условието.
Има вектор от указатели към кръстовища. Няма нужда да пази множеството на улиците,
тъй като не би се използвало никъде, а реализираните алгоритми ги достъпват
чрез кръстовищата.
NOTE: Всички реализирани алгоритми приемат аргументи и връщат низове, които представят имена 
на кръстовища, вместо указатели към тях, например. Причината за това е че този клас
е на най-горното ниво на абстракция на проекта, т.е. очаква се че потребителят не знае
за вътрешното представяне на картата и факта, че съществува клас като Junction. Също така,
всички кръстовища имат уникално име (в контекста на истински град), което е хубав начин лесно
да се идентифицират по това.
*/
class Map
{
public:
    friend std::ifstream &operator>>(std::ifstream &, Map &);
    Map() = default;
    Map &operator=(const Map &);
    Map(const Map &);
    ~Map();

    Map closeJunctions(const std::vector<std::string>&);

    bool hasPath(const std::string&, const std::string&);
    bool canReachEveryOtherJunction(const std::string&);
    bool hasCyclicWalkFromJunction(const std::string&);
    int getStreetsCount();
    std::vector<std::pair<std::vector<std::string>, int>> get3ShortestPaths(const std::string&,const  std::string&);
    std::vector<std::pair<std::vector<std::string>, int>> get3ShortestPaths(const std::string&,const  std::string&,const  std::vector<std::string>&);
    std::optional<std::vector<std::string>> getEulerWalk();
    std::vector<std::pair<std::string, std::string>> getDeadends();
private:
    void copy(const Map &);
    std::optional<std::pair<std::vector<std::string>, int>> getShortestPath(Junction *, Junction *);
    bool hasEulerianCycle();
    std::optional<std::string> readString(std::string &);
    std::optional<double> readDistance(std::string &);
    int reachableJunctionsCount(Junction *,std::vector<Junction *> &);
    std::vector<Junction *> junctions;
    Junction *getJunctionByName(const std::string &);
    friend class InteractiveMode;
};