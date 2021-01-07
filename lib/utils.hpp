#include <vector>
#include <algorithm>
#include "Junction.hpp"
#include "Street.hpp"

//Помощни функции за проверки, които се извършват често - дали вектор съдържа улица или кръстовище

bool vectorContainsStreet(const std::vector<Street> &v, const Street &street)
{
    return std::find_if(v.begin(),
                        v.end(),
                        [street](auto passedStreet) { return (passedStreet.begin->getName().compare(street.begin->getName()) == 0 && passedStreet.end->getName().compare(street.end->getName()) == 0); }) != v.end();
}

bool vectorContainsJunction(const std::vector<Junction *> &v, Junction *junction)
{
    return std::find_if(v.begin(),
                        v.end(),
                        [junction](auto passedJunction) { return passedJunction->getName().compare(junction->getName()) == 0; }) != v.end();
}