#include "parser.h"
#include "tokenize.h"
#include "validateTokens.h"
#include "rNumber.h"

#include <iostream>
#include <stack>
#include <QString>
#include <string>
#include <vector>

/*
Exceptions:
    0 = no problem everything went alright
    1 = divided by 0, big nono
    2 = square root of a negative number big nono
    3 = integer overflow
    4 = syntax error
*/

rNumber convert(std::vector<QString> & process, const QString & s, int& exception)
{
    //these variables will be changed to numbers probably the real numbers class ill make
    bool addToken = false;
    bool savetoA = false;
    bool savetoB = false;
    rNumber falseReturn;
    rNumber value;
    int error = 0;
    char thisOp;
    char pastOp;
    rNumber a;
    rNumber b;
    rNumber val1;
    rNumber val2;
    //rnumber a*;
    //rnumber b*;
    std::stack<rNumber> valueStack;
    std::stack<std::string> opStack;
    std::string token;
    std::string pastToken;
    std::string str = s.toStdString();
    std::vector<std::string> v;
    /*
        gonna use past to tell what token came before the other one
        im gonna use this to follow the basic rules of arithmetic
        rnumbers = 1
        variables = 2
        operators = 3
        parenthesis = 4
        0 means there's nothing so far
    */
    int past = 0;

    v = regex(str);

    for(int i = 0; i < v.size(); i++)
    {
        if(i == 0)
        {
            //if it's the first token and it's a -, I have to check if a number is next and make taht number negative
            if(isOp(v[i]) && v[i] == "-")
            {
                if(isrNumber(v[i+1]))
                {
                    token = v[i+1];
                    value.parse(token);
                    value.negate();
                    if(!value.gerror())
                    {
                        valueStack.push(value);
                        past = 1;
                        i++;
                    }
                    else
                    {
                        exception = 3;
                        return falseReturn;
                    }

                    //insert in stack
                }
                else
                {
                    //if it's not a number it could be a variable, which i have to take the value and push it
                    if(isVariable(v[i+1]))
                    {
                        rNumber temp;
                        if(v[i+1] == "a" || v[i+1] == "A")
                        {
                            temp.copy(a);
                        }
                        else
                        {
                            temp.copy(b);
                        }

                        temp.negate();
                        //method to change the number to negative
                        valueStack.push(temp);
                        past = 2;
                        i++;
                    }
                    else
                    {
                        //if it's not a number or a variable it means that there's an error, might need to add an exception with parenthesis
                        exception = 4;
                        return falseReturn;
                    }
                }
            }
            else
            {
                //when it doesnn't start with a '-' you proceed normally
                if(isrNumber(v[i]))
                {
                    token = v[i];
                    value.parse(token);
                    if(!value.gerror())
                    {
                        valueStack.push(value);
                        past = 1;
                    }
                    else
                    {
                        exception = 3;
                        return falseReturn;
                    }
                }
                if(isOp(v[i]))
                {
                    //you can't start with an operator, might need to add an exception to parenthesis
                    exception = 4;
                    return falseReturn;
                }
                if(isVariable(v[i]))
                {   //if it's variable just push the value of said variable
                    if(v[i] == "a" || v[i] == "A")
                    {
                        valueStack.push(a);
                    }
                    else
                    {
                        valueStack.push(b);
                    }
                    past = 2;
                }
                if(noMatch(v[i]))
                {
                    //didn't match anything, that means it's a syntax error
                    exception = 4;
                    return falseReturn;
                }
            }
        }
        else
        {
            if(isrNumber(v[i]))
            {
                token = v[i];
                value.parse(token);
                if(!value.gerror())
                {
                    valueStack.push(value);
                    past = 1;
                }
                else
                {
                    exception = 3;
                    return falseReturn;
                }
            }
            if(isVariable(v[i]))
            {
                if(v[i] == "a" || v[i] == "A")
                {
                    valueStack.push(a);
                }
                else
                {
                    valueStack.push(b);
                }
                past = 2;
            }
            if(isOp(v[i]))
            {
                //if there's 2 tokens on a row
                if(past == 3)
                {
                    //if the token is - and the last token was - or +
                    if(v[i] == "-")
                    {
                        if(isrNumber(v[i+1]))
                        {
                            token = v[i+1];
                            value.parse(token);
                            value.negate();
                            if(value.gerror())
                            {
                                valueStack.push(value);
                                i++;
                                past = 1;
                            }
                            else
                            {
                                exception = 3;
                                return falseReturn;
                            }
                        }
                        else
                        {
                            //if there's 3 operators throw an exception
                            //need to make exception if it's a parenthesis
                            if(isOp(v[i+1]))
                            {
                                exception = 4;
                                return falseReturn;
                            }
                            if(isVariable(v[i+1]))
                            {
                                if(v[i] == "a" || v[i] == "A")
                                {
                                    valueStack.push(a);
                                }
                                else
                                {
                                    valueStack.push(b);
                                }
                                past = 2;
                            }
                            //can't end in an operator so that means it's an exception
                            if(noMatch(v[i+1]))
                            {
                                exception = 4;
                                return falseReturn;
                            }
                        }
                    }
                    else
                    {
                        // I think this is wrong
                        addToken = true;
                        token = v[i];
                        thisOp = token[0];
                    }
                }
                else
                {
                    //if the last token was not an operator
                    addToken = true;
                    token = v[i];
                    thisOp = token[0];
                }

            }
            if(isEnd(v[i]))
            {
                if(i < (v.size() - 1) && isVariable(v[i+1]))
                {
                    if(v[i+1] == "a" || v[i+1] == "A")
                    {
                        savetoA = true;
                    }
                    else
                    {
                        savetoB = true;
                    }
                }
                break;
            }
        }

        if(addToken)
        {
            switch(thisOp)
            {
                case '+':
                //if(past == 3 && pastToken != "-")
                while(opStack.size() != 0)
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOp = pastToken[0];
                    val1.copy(valueStack.top());
                    valueStack.pop();
                    val2.copy(valueStack.top());
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOp,process,error));
                }
                past = 3;
                opStack.push(token);
                break;
                case '-':
                while(opStack.size() != 0)
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOp = pastToken[0];
                    val1.copy(valueStack.top());
                    valueStack.pop();
                    val2.copy(valueStack.top());
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOp,process,error));
                }
                past = 3;
                opStack.push(token);
                break;
                case '*':
                while(opStack.size() != 0 && opStack.top() != "+" && opStack.top() != "-")
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOp = pastToken[0];
                    val1.copy(valueStack.top());
                    valueStack.pop();
                    val2.copy(valueStack.top());
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOp,process,error));
                }
                past = 3;
                opStack.push(token);
                break;
                case '/':
                while(opStack.size() != 0 && opStack.top() != "+" && opStack.top() != "-")
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOp = pastToken[0];
                    val1.copy(valueStack.top());
                    valueStack.pop();
                    val2.copy(valueStack.top());
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOp,process,error));
                }
                past = 3;
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
                    pastOp = pastToken[0];
                    val1.copy(valueStack.top());
                    valueStack.pop();
                    val2.copy(valueStack.top());
                    valueStack.pop();
                    valueStack.push(evaluate(val1,val2,pastOp,process,error));
                }
                past = 4;
                opStack.pop();
                break;
                default:
                /*size_t equals = str.find_first_of("=");
                str.erase(equals);*/
                break;//special case
            }
            addToken = false;
        }


    }

    while(opStack.size() != 0)
    {
        char thisOP;
        token = opStack.top();
        opStack.pop();
        thisOP = token[0];
        val1.copy(valueStack.top());
        valueStack.pop();
        val2.copy(valueStack.top());
        valueStack.pop();
        valueStack.push(evaluate(val1,val2,thisOP,process,error));
    }
    value.copy(valueStack.top());
    valueStack.pop();
    exception = error;
    //a and b are probably gonna be reference variables, so i can keep them alive
    if(savetoA)
        a.copy(value);
    if(savetoB)
        b.copy(value);
    return value;
}


rNumber evaluate(rNumber value1, rNumber value2, const char & op, std::vector<QString> & process, int & error)
{
    //TODO: validate that the operations are viable
    QString str;
    std::string tmp;
    tmp = value1.printNumber();
    QString number1 = QString::fromStdString(tmp);
    tmp = value2.printNumber();
    QString number2 = QString::fromStdString(tmp);

    //fix this part, idk how to put the limits yet ):
    rNumber maxNumber;

    switch(op)
    {
        case '+':
            str = number2 + "+" + number1;
            process.push_back(str);
            maxNumber.copy(value2.add(value1));
            if(maxNumber.getN2() > 99 || maxNumber.getN2() < -99)
            {
                error = 3;
                maxNumber.setN1(0);
                maxNumber.setN2(0);
                return maxNumber;
            }
            return maxNumber;
        case '-':
            str = number2 + "-" + number1;
            process.push_back(str);
            maxNumber.copy(value2.substract(value1));
            if(maxNumber.getN2() > 99 || maxNumber.getN2() < -99)
            {
                error = 3;
                maxNumber.setN1(0);
                maxNumber.setN2(0);
                return maxNumber;
            }
            return maxNumber;
        case '*':
            str = number2 + "*" + number1;
            process.push_back(str);
            maxNumber.copy(value2.multiply(value1));
            if(maxNumber.getN2() > 99 || maxNumber.getN2() < -99)
            {
                error = 3;
                maxNumber.setN1(0);
                maxNumber.setN2(0);
                return maxNumber;
            }
            return maxNumber;
        case '/':
            dec::decimal<8> n(0);
            //TODO: VALIDATE THAT IF IT'S DIVIDED BY 0, THROW AN EXCEPTION
            //HOWTO? PUT A TOKEN ON EVALUATE, AND MAKE IT BE CHECKED AFTER EACH EVALUATION, IF THE TOKEN IS TRUE RETURN ERROR
            if(value1.getN1() == n)
            {
                maxNumber.setN1(0);
                maxNumber.setN2(0);
                error = 1;
                return maxNumber;
            }
            str = number2 + "/" + number1;
            process.push_back(str);
            return value2.divide(value1);
    }
    return maxNumber;
}
