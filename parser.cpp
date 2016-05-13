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
    5 = unmatched parenthesis
    6 = variable not defined
*/

rNumber convert(std::vector<QString> & process, const QString & s, int& exception, rNumber & a, rNumber & b, rNumber & c)
{
    //these variables will be changed to numbers probably the real numbers class ill make
    bool addToken = false;
    bool savetoA = false;
    bool savetoB = false;
    bool savetoC = false;
    rNumber falseReturn;
    rNumber value;
    int error = 0;
    char thisOp;
    char pastOp;
    rNumber val1;
    rNumber val2;
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
    int cc = 0;
    for(int i = 0; i < v.size(); i++)
    {
        if(v[i] == "(")
            cc++;
        if(v[i] == ")")
            cc--;

    }
    if(cc!= 0)
    {
        exception = 5;
        return falseReturn;
    }

    for(int i = 0; i < v.size(); i++)
    {
        if(i == 0)
        {
            //if it's the first token and it's a -, I have to check if a number is next and make taht number negative
            if(isOp(v[i]) && v[i] == "-")
            {
                if(isrNumber(v[i+1]))
                {
                    addToken = true;
                    token = v[i];
                    thisOp = token[0];
                    past = 3;
                    /*
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
                        exception = 4;
                        return falseReturn;
                    }*/

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
                            if(v[i+1] == "b" || v[i+1] == "B")
                            {

                                temp.copy(b);
                            }
                            else
                            {
                                temp.copy(c);
                            }
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
                        if(v[i+1] == "(")
                        {
                            addToken = true;
                            token = v[i];
                            thisOp = token[0];
                            past = 3;
                        }
                        else
                        {
                            exception = 4;
                            return falseReturn;
                        }
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
                    if(v[i] == "(" )
                    {
                        addToken = true;
                        token = v[i];
                        thisOp = token[0];
                        past = 3;
                    }
                    else
                    {
                        //you can't start with an operator, might need to add an exception to parenthesis
                        exception = 4;
                        return falseReturn;
                    }

                }
                if(isVariable(v[i]))
                {   //if it's variable just push the value of said variable
                    if(v[i] == "a" || v[i] == "A")
                    {
                        valueStack.push(a);
                    }
                    else
                    {
                        if(v[i] == "b" || v[i] == "B")
                        {
                            valueStack.push(b);
                        }
                        else
                        {
                            valueStack.push(c);
                        }
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
                    if(a.gerror())
                    {
                        exception = 6;
                        return falseReturn;
                    }
                    else
                    {
                        valueStack.push(a);
                    }
                }
                else
                {
                    if(v[i] == "b" || v[i] == "B")
                    {
                        if(b.gerror())
                        {
                            exception = 6;
                            return falseReturn;
                        }
                        valueStack.push(b);
                    }
                    else
                    {
                        if(c.gerror())
                        {
                            exception = 6;
                            return falseReturn;
                        }
                        valueStack.push(c);
                    }
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
                            if((i+2) < v.size())
                            {
                                if(v[i+2] == "^")
                                {
                                    addToken = true;
                                    token = v[i];
                                    thisOp = token[0];
                                    past = 3;
                                }
                                else
                                {
                                    token = v[i+1];
                                    value.parse(token);
                                    value.negate();
                                    if(!value.gerror())
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
                            }
                            else
                            {
                                token = v[i+1];
                                value.parse(token);
                                value.negate();
                                if(!value.gerror())
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
                        }
                        else
                        {
                            //if there's 3 operators throw an exception
                            //need to make exception if it's a parenthesis
                            if(isOp(v[i+1]))
                            {
                                if(v[i+1] == "(")
                                {
                                    addToken = true;
                                    token = v[i];
                                    thisOp = token[0];
                                    past = 3;
                                }
                                else
                                {
                                    exception = 4;
                                    return falseReturn;

                                }
                            }
                            if(isVariable(v[i+1]))
                            {
                                if(v[i] == "a" || v[i] == "A")
                                {
                                    if(a.gerror())
                                    {
                                        exception = 6;
                                        return falseReturn;
                                    }
                                    else
                                    {
                                        valueStack.push(a);
                                    }
                                }
                                else
                                {
                                    if(v[i] == "b" || v[i] == "B")
                                    {
                                        if(b.gerror())
                                        {
                                            exception = 6;
                                            return falseReturn;
                                        }
                                        else
                                        {
                                            valueStack.push(b);
                                        }
                                    }
                                    else
                                    {
                                        if(c.gerror())
                                        {
                                            exception = 6;
                                            return falseReturn;
                                        }
                                        else
                                        {
                                            valueStack.push(c);
                                        }
                                    }
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
                        if(v[i] == "(")
                        {
                            addToken = true;
                            token = v[i];
                            thisOp = token[0];
                            past = 3;
                        }
                        else
                        {
                            exception = 4;
                            return falseReturn;
                        }
                    }
                }
                else
                {
                    //if the last token was not an operator
                    addToken = true;
                    token = v[i];
                    thisOp = token[0];
                    past = 3;
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
                        if(v[i+1] == "b" || v[i+1] == "B")
                        {
                            savetoB = true;
                        }
                        else
                        {
                            savetoC = true;
                        }
                    }
                }
                break;
            }
            if(noMatch(v[i]))
            {
                exception = 4;
                return falseReturn;
            }
        }

        if(addToken)
        {
            switch(thisOp)
            {
                case '+':
                //if(past == 3 && pastToken != "-")
                while(opStack.size() != 0 && opStack.top() != "(" && opStack.top() != ")")
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
                while(opStack.size() != 0 && opStack.top() != "(" && opStack.top() != ")")
                {
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOp = pastToken[0];
                    val1.copy(valueStack.top());
                    valueStack.pop();
                    if(valueStack.size() != 0)
                    {
                        val2.copy(valueStack.top());
                        valueStack.pop();
                        valueStack.push(evaluate(val1,val2,pastOp,process,error));
                    }
                    else
                    {
                        val2.parse("0");
                        valueStack.push(evaluate(val1,val2,pastOp,process,error));
                    }

                }
                past = 3;
                opStack.push(token);
                break;
                case '*':
                while(opStack.size() != 0 && opStack.top() != "+" && opStack.top() != "-" && opStack.top() != "(" && opStack.top() != ")")
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
                while(opStack.size() != 0 && opStack.top() != "+" && opStack.top() != "-" && opStack.top() != "(" && opStack.top() != ")")
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
                    if(opStack.size() == 0)
                    {
                        exception = 3;
                        return falseReturn;
                    }
                    pastToken = opStack.top();
                    opStack.pop();
                    pastOp = pastToken[0];
                    if(valueStack.size() != 0)
                    {
                        val1.copy(valueStack.top());
                        valueStack.pop();
                    }
                    else
                    {
                        exception = 3;
                        return falseReturn;
                    }
                    if(valueStack.size() != 0)
                    {
                        val2.copy(valueStack.top());
                        valueStack.pop();
                    }
                    else
                    {
                        exception = 3;
                        return falseReturn;
                    }
                    valueStack.push(evaluate(val1,val2,pastOp,process,error));
                }
                past = 4;
                opStack.pop();
                break;
                case '^':
                //if(past == 3 && pastToken != "-")
                while(opStack.size() != 0 && opStack.top() != "(" && opStack.top() != ")" && opStack.top() != "+" && opStack.top() != "-" && opStack.top() != "*" && opStack.top() != "/")
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
        if(valueStack.size() == 0)
        {
            val2.parse("0");
        }
        else
        {
            val2.copy(valueStack.top());
            valueStack.pop();
        }
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
    if(savetoC)
        c.copy(value);
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
        {
            str = number2 + "+" + number1;
            process.push_back(str);
            maxNumber = value2.add(value1);
            if(maxNumber.getN2() > 99 || maxNumber.getN2() < -99)
            {
                error = 3;
                return rNumber(dec::decimal_cast<8>(0),0);
            }
            return maxNumber;
        }
        case '-':
        {
            str = number2 + "-" + number1;
            process.push_back(str);
            maxNumber = value2.substract(value1);
            if(maxNumber.getN2() > 99 || maxNumber.getN2() < -99)
            {
                error = 3;
                return rNumber(dec::decimal_cast<8>(0),0);
            }
            return maxNumber;
        }
        case '*':
            str = number2 + "*" + number1;
            process.push_back(str);
            maxNumber = value2.multiply(value1);
            if(maxNumber.getN2() > 99 || maxNumber.getN2() < -99)
            {
                error = 3;
                return rNumber(dec::decimal_cast<8>(0),0);
            }
            return maxNumber;
        case '/':
        {
            dec::decimal<8> n(0);
            //TODO: VALIDATE THAT IF IT'S DIVIDED BY 0, THROW AN EXCEPTION
            //HOWTO? PUT A TOKEN ON EVALUATE, AND MAKE IT BE CHECKED AFTER EACH EVALUATION, IF THE TOKEN IS TRUE RETURN ERROR
            if(value1.getN1() == n)
            {
                error = 1;
                return rNumber(dec::decimal_cast<8>(0),0);
            }
            str = number2 + "/" + number1;
            process.push_back(str);
            return value2.divide(value1);
        }
        case '(':
        {
            error = 3;
            dec::decimal<8> trash(0);
            rNumber trashy(trash,0);
            return trashy;
            break;
        }
        case '^':
        {
            //dec::decimal<8> n(0);
            //TODO: VALIDATE THAT IF IT'S DIVIDED BY 0, THROW AN EXCEPTION
            //HOWTO? PUT A TOKEN ON EVALUATE, AND MAKE IT BE CHECKED AFTER EACH EVALUATION, IF THE TOKEN IS TRUE RETURN ERROR
            str = number2 + "^" + number1;
            process.push_back(str);
            maxNumber = value2.exponent(value1);
            if(maxNumber.gerror())
            {
                if(maxNumber.getN1() == dec::decimal_cast<8>(1))
                {
                    error = 3;
                    dec::decimal<8> trash(0);
                    rNumber trashy(trash,0);
                    return trashy;
                }
                error = 2;
                dec::decimal<8> trash(0);
                rNumber trashy(trash,0);
                return trashy;
            }
            return maxNumber;
            break;
        }

    }
    return maxNumber;
}
