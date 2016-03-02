#include "basec.h"

#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

std::string base2(long long answer)
{
    std::string s;
    std::stringstream str;
    std::bitset<sizeof(long long) * 8> bits(answer);
    str << bits;
    s = str.str();
    return s;
}

std::string base16(long long answer)
{
    std::string s;
    std::stringstream str;
    str << std::hex << answer;
    s = str.str();
    return s;
}
