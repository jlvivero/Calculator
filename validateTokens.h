#ifndef VALIDATETOKENS_H
#define VALIDATETOKENS_H

#include <regex>
#include <iostream>
#include <string>

bool isrNumber(std::string s);
bool isNumber(std::string s);
bool isDecimal(std::string s);
bool isOp(std::string s);
bool isExp(std::string s);
bool isVariable(std::string s);
bool noMatch(std::string s);
bool isEnd(std::string s);

#endif
