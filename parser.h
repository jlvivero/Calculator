#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <QString>

long long convert(std::vector<QString> & process, const Qstring & s);
long long evaluate(long long value1, long long value2, const char & op, std::vector<Qstring> & process);

#endif
