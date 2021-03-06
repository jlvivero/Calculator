#include "tokenize.h"
#include <regex>
#include <iterator>
#include <iostream>
#include <string>

std::vector<std::string> regex(const std::string & infix_op)
{
    std::regex tokenize("\\d+\\.?(E-|[^*+/\\^ ()=-])*|\\+|-|\\*|/|\\(|\\)|=|\\^|[^\\s=]");

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

std::vector<std::string> regexIf(const std::string & infix_op)
{
    //i hope it's correct
    std::regex tokenize("[a-zA-Z]+|(\\[[^\\[\\]]*\\])");

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

std::vector<std::string> regexCondition(const std::string & infix_op)
{
    std::regex tokenize("[^=<>]+|=|<(=|>)*|>=*|<>");
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

std::vector<std::string> regexRoot2(const std::string & infix_op)
{
    std::regex tokenize("[^,\\s]+|,");
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

std::vector<std::string> regexRoot(const std::string & infix_op)
{
    //std::regex tokenize("((root|ROOT)\\[(.+)\\])|\\d+\\.?(E-|[^*+/\\^ ()=-])*|\\+|-|\\*|/|\\(|\\)|=|\\^|[^\\s=]");
    std::regex tokenize("((root|ROOT)\\((.+)\\))|\\d+\\.?(E-|[^*+/\\^ ()=-])*|\\+|-|\\*|/|\\(|\\)|=|\\^|[^\\s=]");

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
