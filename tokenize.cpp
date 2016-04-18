#include "tokenize.h"
#include <regex>
#include <iterator>
#include <iostream>

std::vector<std::string> regex(const std::string & infix_op)
{
  std::regex tokenize("(\\d+\\.?(E-|[^*+/\\^ ()=-])*|\\+|-|\\*|/|\\(|\\)|=|\\^|[^\\s=]+)|([a-zA-Z](E-|[^*+/\\^ ()=-])*|\\+|-|\\*|/|\\(|\\)|=|\\^|[^\\s=]+)");

  auto words_begin = std::sregex_iterator(infix_op.begin(), infix_op.end(), tokenize);
  auto words_end = std::sregex_iterator();

  std::vector<std::string> v;
  for(auto i = words_begin; i != words_end; ++i)
  {
    std::smatch match = *i;
    std::string match_str = match.str();
    v.push_back(match_str);
  }
  return v;
}
