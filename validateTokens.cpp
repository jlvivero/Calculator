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

bool isif(std::string s)
{
    //std::string str = s.toStdString();
    //this returns true if the format of the string is if[whatever] THEN[WTV] ELSE[wtv] or if[wtv]AND[WTV]THEN[wtv]ELSE[wtv]
    //what will go inside of those wtv, is ecuations which i already covered, and conditions which i will cover soon
    return std::regex_match(s,std::regex("(if|IF)\\[.*\\]((AND|OR)\\[.*\\])*(THEN|then)\\[.*\\]((ELSE|else)\\[.*\\])*"));
    // std::regex_match(s,std::regex("(if|IF)\\[.*\\]((AND|OR)\\[.*\\])*THEN\\[.*\\]ELSE\\[.*\\]"));
}

bool isKeyword(std::string s)
{
    return keywordif(s) || keywordAND(s) || keywordOR(s) || keywordelse(s) || keywordthen(s);
}

bool keywordif(std::string s)
{
    //returns true if the keyword is IF or if
    return std::regex_match(s,std::regex("if|IF"));
}

bool keywordAND(std::string s)
{
    //returns true if the keyword is AND
    return std::regex_match(s,std::regex("AND"));
}

bool keywordOR(std::string s)
{
    //returns true if the keyword is OR
    return std::regex_match(s,std::regex("OR"));
}

bool keywordelse(std::string s)
{
    //returns true if they keyword is else or ELSE
    return std::regex_match(s,std::regex("ELSE|else"));
}

bool keywordthen(std::string s)
{
    //returns true if the keyword is then or THEN
    return std::regex_match(s,std::regex("then|THEN"));
}

bool opBool(std::string s)
{
    //returns true if it's a bool
    return opEquals(s) || opLessThan(s) || opMoreThan(s) || opLessOrEqual(s) || opMoreOrEqual(s) || opDifferent(s);
}

bool opEquals(std::string s)
{
    return std::regex_match(s,std::regex("="));
}

bool opLessThan(std::string s)
{
    return std::regex_match(s,std::regex("<"));
}

bool opMoreThan(std::string s)
{
    return std::regex_match(s,std::regex(">"));
}

bool opLessOrEqual(std::string s)
{
    return std::regex_match(s,std::regex("<="));
}

bool opMoreOrEqual(std::string s)
{
    return std::regex_match(s,std::regex(">="));
}

bool opDifferent(std::string s)
{
    return std::regex_match(s,std::regex("<>"));
}

bool isRoot(std::string s)
{
    return std::regex_match(s,std::regex("(root|ROOT)\\((.+)\\)"));
}

//TODO:20 do the regex match for the format thing
bool isFormat(const QString & s)
{
  return false;
}
