#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <QString>

long long convert(std::vector<QString> & process, const Qstring & s);
void evaluate(long long value1, long long value2, const char & op);

#endif
