#ifndef TOKENIZE_H
#define TOKENIZE_H
#include<vector>
#include<string>
std::vector<std::string> regex(const std::string & infix_op);
std::vector<std::string> regexIf(const std::string & infix_op);
std::vector<std::string> regexCondition(const std::string & infix_op);
#endif
