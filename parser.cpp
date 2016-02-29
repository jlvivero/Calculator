#include "parser.h"
#include <iostream>
#include <stack>
#include <QString>
#include <string>

//TODO: implement the shunting yard algorithm

long long convert(std::vector<QString> & process, const QString & s)
{
    long long value;
    bool negative = false;
    std::stack<long long> valueStack;
    std::stack<std::string> opStack;
    std::string str = s.toStdString();
    std::string token;
    std::string pastToken;
    int j = 0;

    while(!str.empty())
    {
        char thisOP;
        char pastOP;
        //this assumes that the equation is already validated as having only 12 digits per number, and that the sequence of +/*- etc, are not together
        //TODO: make a cpp that has all the possible tokens with regex instead of this solution, do this after initial testings
        j = int (str.find_first_of("+-/*()"));
        if(j == -1) //if the last value is an int
        {
            token = str;
            value = std::stoll(token);
            valueStack.push(value);
            str.erase(0,str.end());
            continue;
        }
        if(j == 0) // if it starts with a negative number
        {
            negative = true;
            str.erase(0,1);
            continue;
        }

        token = str.substr(0,j);
        value = std::stoll(token,nullptr);
        if(negative)
        {
            value = value * -1;
            negative = false;
        }
        valueStack.push(value);
        str.erase(0,j);
        //up to here you get the number and add it to the value

        token = str[0];
        //this is just to make sure the switch works fine with a character
        thisOP = token[0];

        //TODO: this might not be needed if i can detect what kind of token it is  with the complimentary cpp i need to do
        switch(thisOP)
        {
            case '+':
            case '-':
                while(opStack.size != 0)
                {
                    pastToken = opStack.pop();
                    pastOP = pastToken[0];
                    valueStack.push(evaluate(valueStack.pop(),valueStack.pop(),pastOP,process));
                }
                opStack.push(token);
                break;
            case '*':
                while(opStack.size != 0 && opStack.top != '+' && opStack.top != '-')
                {
                    pastToken = opStack.pop();
                    pastOP = pastToken[0]
                    valueStack.push(evaluate(valueStack.pop(),valueStack.pop(),pastOP,process));
                }
                opStack.push(token);
                break;
            case '/':
                while(opStack.size != 0 && opStack.top != '+' && opStack.top != '-')
                {
                    pastToken = opStack.pop();
                    pastOP = pastToken;
                    valueStack.push(evaluate(valueStack.pop(),valueStack.pop(),pastOP,process));
                }
                opStack.push(token);
                break;
            case '(':
                opStack.push(token);
                break;
            case ')':
                std::string trash;
                while(opStack.top != '(')
                {
                    pastToken = opStack.pop();
                    pastOP = pastToken;
                    valueStack.push(evaluate(valueStack.pop(),valueStack.pop(),pastOP,process));
                }
                trash = opStack.pop();
            default: //special case
        }

    }//TODO: use regex properly to check if the string is a number
    while(opStack.size != 0)
    {
        char thisOP
        token = opStack.pop();
        thisOP = token[0];
        valueStack.push(evaluate(valueStack.pop(),valueStack.pop(),thisOP,process));
    }
    value = valueStack.pop();
    return value;
}


long long evaluate(long long value1, long long value2, const char & op, std::vector<Qstring> & process )
{
    //TODO: validate that the operations are viable
    QString str;
    switch(op)
    {
        case '+':
            str = value1 + "+" + value2;
            process.push_back(str);
            return value1 + value2;
        case '-':
            str = value1 + "-" + value2;
            process.push_back(str);
            return value1 - value2;
        case '*':
            str = value1 + "*" + value2;
            process.push_back(str);
            return value1 * value2;
        case '/':
            str = value1 + "/" + value2;
            process.push_back(str);
            return value1 / value2;
    }
}
