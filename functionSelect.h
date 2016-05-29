#ifndef FUNCTION_SELECT_H
#define FUNCTION_SELECT_H
#include <QString>
#include <vector>
#include "rNumber.h"

rNumber prepareForParse(std::vector<QString> &process, const QString & s, int & exception, rNumber &a, rNumber &b, rNumber &c);
std::string removeRoot(std::string s);

#endif
