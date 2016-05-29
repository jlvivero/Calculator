#ifndef PARSER_H
#define PARSER_H
#include "tokenize.h"
#include "rNumber.h"

#include <vector>
#include <QString>
#include <iostream>
#include <stack>
#include <string>

rNumber convertIf(std::vector<QString> & process, const QString & s, int & exception, rNumber & a, rNumber & b, rNumber & c);
rNumber convert(std::vector<QString> & process, const QString & s, int & exception, rNumber & a, rNumber & b, rNumber & c);
rNumber format(std::vector<QString> & process, const QString & s, int & exception, rNumber &a, rNumber & b, rNumber & c);
rNumber evaluate(rNumber value1, rNumber value2, const char & op, std::vector<QString> & process, int & error);

#endif
