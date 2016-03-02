#include "parser.h"

#include <iostream>
#include <stack>
#include <QString>
#include <string>

/*
Exceptions:
    0 = no problem everything went alright
    1 = divided by 0, big nono
    2 = square root of a negative number big nono
    3 = integer overflow
    4 = syntax error
*/

long long convert(std::vector<QString> & process, const QString & s, int& exception)
{
    long long value;
    long long val1;
    long long val2;
    int error = 0;
    int j = 0;
    std::stack<long long> valueStack;
    std::stack<std::string> opStack;
    std::string str = s.toStdString();
    std::string token;
    std::string pastToken;

    //str.pop_back(); //gets rid of the = sign
    size_t equals = str.find_first_of("=");
    if(equals != std::string::npos)
    {
        str.erase(equals+1);
    }
    //str.erase(equals);
    while(!str.empty())
    {
        char thisOP;
        char pastOP;
        //this assumes that the equation is already validated as having only 12 digits per number, and that the sequence of +/*- etc, are not together
        //TODO: make a cpp that has all the possible tokens with regex instead of this solution, do this after initial testings
        size_t found = str.find_first_of("+-/*()=");
        if(found == std::string::npos)
        {
            j == -1;
        }
        else
        {
            j = int (found);
        }
        if(j == 0) // if it starts with a negative number
        {
            str.erase(0,1);
            j = int (str.find_first_of("+-/*()="));
            if(j == -1)
            {
                token = str;
            }
            else
            {
                token = str.substr(0,j);
            }
            value = std::stoll(token,nullptr);
            value = value * -1;
        }
        else
        {
            if(j == -1)
            {
                token = str;
            }
            else
            {
                token = str.substr(0,j);
            }
            value = std::stoll(token,nullptr);
        }
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
        if(str.empty())
        {
            continue;
        }
        token = str[0];
        //this is just to make sure the switch works fine with a character
        thisOP = token[0];
        str.erase(0,1);

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
                    valueStack.push(evaluate(val1,val2,pastOP,process,error));
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
                    valueStack.push(evaluate(val1,val2,pastOP,process,error));
                }
                opStack.push(token);
                break;
            case '*':
                while(opStack.size() != 0 && opStack.top() != "+" && opStack.top() != "-")
                //opStack.top().compare("+") && opStack.top().compare("-"))
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOP = pastToken[0];
                    val1 = valueStack.top();
                    valueStack.pop();
                    val2 = valueStack.top();
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOP,process,error));
                }
                opStack.push(token);
                break;
            case '/':
                while(opStack.size() != 0 && opStack.top() != "+" && opStack.top() != "-")
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOP = pastToken[0];
                    val1 = valueStack.top();
                    valueStack.pop();
                    val2 = valueStack.top();
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOP,process,error));
                }
                opStack.push(token);
                break;
            case '(':
                opStack.push(token);
                break;
            case ')':
                while(opStack.top() != "(")
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOP = pastToken[0];
                    val1 = valueStack.top();
                    valueStack.pop();
                    val2 = valueStack.top();
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOP,process,error));
                }
                opStack.pop();
                break;
            default:
                /*size_t equals = str.find_first_of("=");
                str.erase(equals);*/
                break;//special case
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
        valueStack.push(evaluate(val1,val2,thisOP,process,error));
    }
    value = valueStack.top();
    valueStack.pop();
    exception = error;
    return value;
}


long long evaluate(long long value1, long long value2, const char & op, std::vector<QString> & process, int & error)
{
    //TODO: validate that the operations are viable
    QString str;
    QString number1 = QString::number(value1);
    QString number2 = QString::number(value2);
    long long maxNumber = 999999999999;
    long long minNumber = -999999999999;

    switch(op)
    {
        case '+':

            str = number2 + "+" + number1;
            process.push_back(str);
            if(value2+value1 > maxNumber)
            {
                error = 3;
                return 0;
            }
            return value2 + value1;
        case '-':
            str = number2 + "-" + number1;
            process.push_back(str);
            if(value2 - value1 < minNumber)
            {
                error = 3;
                return 0;
            }
            return value2 - value1;
        case '*':
            str = number2 + "*" + number1;
            process.push_back(str);
            if(value2 * value1 > maxNumber)
            {
                error = 3;
                return 0;
            }
            return value2 * value1;
        case '/':
            //TODO: VALIDATE THAT IF IT'S DIVIDED BY 0, THROW AN EXCEPTION
            //HOWTO? PUT A TOKEN ON EVALUATE, AND MAKE IT BE CHECKED AFTER EACH EVALUATION, IF THE TOKEN IS TRUE RETURN ERROR
            if(value1 == 0)
            {
                error = 1;
                return 0;
            }
            str = number2 + "/" + number1;
            process.push_back(str);
            return value2 / value1;
    }
    return 0;
}
