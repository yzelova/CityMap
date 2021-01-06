#include <vector>
#include <algorithm>
#include "Junction.hpp"

bool vectorContainsStreet(const std::vector<Street> &v, Street street)
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