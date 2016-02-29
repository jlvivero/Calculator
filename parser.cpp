#include "parser.h"
#include <iostream>
#include <stack>
#include <QString>
#include <string>

//TODO: implement the shunting yard algorithm

long long convert(std::vector<QString> & process, const QString & s)
{
    long long value;
    long long val1;
    long long val2;
    bool negative = false;
    bool first = true;
    std::stack<long long> valueStack;
    std::stack<std::string> opStack;
    std::string str = s.toStdString();
    std::string token;
    std::string pastToken;
    int j = 0;
    str.pop_back();

    while(!str.empty())
    {
        char thisOP;
        char pastOP;
        //this assumes that the equation is already validated as having only 12 digits per number, and that the sequence of +/*- etc, are not together
        //TODO: make a cpp that has all the possible tokens with regex instead of this solution, do this after initial testings
        j = int (str.find_first_of("+-/*()"));
        if(j == 0) // if it starts with a negative number
        {
            negative = true;
            str.erase(0,1);
            j = int (str.find_first_of("+-/*()"));
        }

        if(j == -1)
        {
            token = str;
        }
        else
        {
        token = str.substr(0,j);
        }

        value = std::stoll(token,nullptr);

        if(negative && first)
        {
            value = value * -1;
            negative = false;
            first = false;
        }
        first = false;
        valueStack.push(value);
        if(j == -1)
        {
            str.erase(0);
        }
        else
        {
        str.erase(0,j);
        }
        //up to here you get the number and add it to the value

        token = str[0];
        //this is just to make sure the switch works fine with a character
        thisOP = token[0];
        //TODO: this might not be needed if i can detect what kind of token it is  with the complimentary cpp i need to do
        switch(thisOP)
        {
            case '+':
                while(opStack.size() != 0)
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOP = pastToken[0];
                    val1 = valueStack.top();
                    valueStack.pop();
                    val2 = valueStack.top();
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOP,process));
                }
                opStack.push(token);
                break;
            case '-':
                while(opStack.size() != 0)
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOP = pastToken[0];
                    val1 = valueStack.top();
                    valueStack.pop();
                    val2 = valueStack.top();
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOP,process));
                }
                opStack.push(token);
                break;
            case '*':
                while(opStack.size() != 0 && !opStack.top().compare("+") && !opStack.top().compare("-"))
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOP = pastToken[0];
                    val1 = valueStack.top();
                    valueStack.pop();
                    val2 = valueStack.top();
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOP,process));
                }
                opStack.push(token);
                break;
            case '/':
                while(opStack.size() != 0 && !opStack.top().compare("+") && !opStack.top().compare("-"))
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOP = pastToken[0];
                    val1 = valueStack.top();
                    valueStack.pop();
                    val2 = valueStack.top();
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOP,process));
                }
                opStack.push(token);
                break;
            case '(':
                opStack.push(token);
                break;
            case ')':
                while(!opStack.top().compare("("))
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOP = pastToken[0];
                    val1 = valueStack.top();
                    valueStack.pop();
                    val2 = valueStack.top();
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOP,process));
                }
                opStack.pop();
                break;
            default: break;//special case
        }
    }//TODO: use regex properly to check if the string is a number

    while(opStack.size() != 0)
    {
        char thisOP;
        token = opStack.top();
        opStack.pop();
        thisOP = token[0];
        val1 = valueStack.top();
        valueStack.pop();
        val2 = valueStack.top();
        valueStack.pop();
        valueStack.push(evaluate(val1,val2,thisOP,process));
    }
    value = valueStack.top();
    valueStack.pop();
    return value;
}


long long evaluate(long long value1, long long value2, const char & op, std::vector<QString> & process )
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
            return value2 - value1;
        case '*':
            str = value1 + "*" + value2;
            process.push_back(str);
            return value1 * value2;
        case '/':
            str = value1 + "/" + value2;
            process.push_back(str);
            return value2 / value1;
    }
    return 0;
}
