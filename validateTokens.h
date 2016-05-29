#ifndef VALIDATETOKENS_H
#define VALIDATETOKENS_H

#include <regex>
#include <iostream>
#include <string>
#include <QString>

bool isrNumber(std::string s);
bool isNumber(std::string s);
bool isDecimal(std::string s);
bool isOp(std::string s);
bool isExp(std::string s);
bool isVariable(std::string s);
bool noMatch(std::string s);
bool isEnd(std::string s);
bool isif(std::string s);
bool isKeyword(std::string s);
bool keywordif(std::string s);
bool keywordAND(std::string s);
bool keywordOR(std::string s);
bool keywordelse(std::string s);
bool keywordthen(std::string s);
bool opBool(std::string s);
bool opEquals(std::string s);
bool opLessThan(std::string s);
bool opMoreThan(std::string s);
bool opLessOrEqual(std::string s);
bool opMoreOrEqual(std::string s);
bool opDifferent(std::string s);
bool isRoot(std::string s);
bool isFormat(const QString & s);

#endif
