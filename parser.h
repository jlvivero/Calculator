#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <QString>

long long convert(std::vector<QString> & process, const QString & s);
long long evaluate(long long value1, long long value2, const char & op, std::vector<QString> & process);

#endif
