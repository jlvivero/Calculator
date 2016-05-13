#include "validateTokens.h"
#include <regex>
#include <iostream>
#include <string>
#include <QString>

bool isrNumber(std::string s)
{
    return isNumber(s)||isDecimal(s)||isExp(s);
}

bool isNumber(std::string s)
{
    return std::regex_match (s, std::regex("\\d+"));
}

bool isDecimal(std::string s)
{
    return std::regex_match(s,std::regex("\\d+\\.\\d+"));
}

bool isOp(std::string s)
{
    return std::regex_match(s,std::regex("\\+|-|\\^|\\*|\\/|\\(|\\)"));
}

bool isExp(std::string s)
{
    return std::regex_match(s,std::regex("(\\d+(E-|E)\\d+)|(\\d+\\.?\\d+(E-|E)\\d+)"));
}

bool isVariable(std::string s)
{
    return std::regex_match(s,std::regex("A|B|C|a|b|c"));
}

bool noMatch(std::string s)
{
    return !(isrNumber(s)|| isOp(s)|| isVariable(s));
}

bool isEnd(std::string s)
{
    return std::regex_match(s,std::regex("="));
}

bool isif(const QString & s)
{
    std::string str = s.toStdString();
    return std::regex_match(str,std::regex("(i|I)(f|F)(\\d)*(\\D)*"));
}
