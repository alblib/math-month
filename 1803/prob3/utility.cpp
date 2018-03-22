#include "utility.h"

#include <sstream>
#include <string>

bool is_palindrome(const boost::multiprecision::int256_t& value){
    std::stringstream ss;
    ss << value;
    std::string str=ss.str();
    auto it = str.begin();
    auto it2 = str.rbegin();
    while (1){
        if (it == it2.base()) break;
        if (*it != *it2) return false;
        if (++it == it2++.base()) break;
    }
    return true;
}