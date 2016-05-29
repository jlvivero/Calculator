#include "parser.h"
#include "tokenize.h"
#include "validateTokens.h"
#include "rNumber.h"
#include "functionSelect.h"

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
rNumber convertIf(std::vector<QString> & process, const QString & s, int& exception, rNumber & a, rNumber & b, rNumber & c)
{
    //DONE:20 implement the convertIf function more instructions below
    /*
        the Qstring s is gonna be tokenized into the following tokens:
            - Keyword
            - [Anything]
            - After that, the [Anything] will be divided into:
                - [boleanFunction]
                - [NormalFunction]
                - [IfFunction]
        Once everything is tokenized the program will redirect depending on
        what type of cuntion is inside
        boolean functions will have it's own parser,
        normal functions are already implemented and will call the convert func*
        if function will call this function recursively
    */
    QString token2;
    rNumber falseReturn;
    std::string str = s.toStdString();
    std::vector<std::string> v;
    //this tokenizes the function
    v = regexIf(str);
    /*
        keyword can have the following values:
            0: invalid token
            1: if
            2: else
            3: then
            4: AND
            5: or
    */
    int keyword = 0;
    bool condition;
    bool conditionAnswered = false;
    std::string token;

    for(int i = 0; i < v.size(); i++)
    {
        if(isKeyword(v[i]))
        {
            //this means the keyword was acceptable!
            if(keywordif(v[i]))
            {
                keyword = 1;
            }
            if(keywordelse(v[i]))
            {
                keyword = 2;
            }
            if(keywordthen(v[i]))
            {
                keyword = 3;
            }
            if(keywordAND(v[i]))
            {
                keyword = 4;
            }
            if(keywordOR(v[i]))
            {
                keyword = 5;
            }
        }
        else
        {
            //DONE:140 implement what happens when it's a [anything block]
            // use the keyword values to direct the flow
            switch (keyword)
            {
                case 0:
                    exception = 4;
                    return falseReturn;
                    //this means an error return an error
                    //DONE:160 return an error
                    break;
                case 1:
                    //DONE:40 implement parseCondition, must return a bool
                    token = v[i];
                    token.erase(token.begin());
                    token.erase(token.end()-1);
                    token2 = QString::fromStdString(token);
                    condition = parseCondition(process, token2, exception, a, b, c);
                    if(exception == 4)
                    {
                        return falseReturn;
                    }
                    conditionAnswered = true;
                    break;
                case 2:
                    if(conditionAnswered)
                    {
                        if(!condition)
                        {
                            //DONE:150 make it so that "[" and "]" get removed then parse the function again

                            //might have to include the iterator header
                            token = v[i];
                            token.erase(token.begin());
                            token.erase(token.end()-1);
                            token2 = QString::fromStdString(token);
                            //DONE:130 convert token into a Qstring and pass it to token2
                            return prepareForParse(process, token2, exception, a, b , c);
                        }
                    }
                    break;
                case 3:
                    if(conditionAnswered)
                    {
                        if(condition)
                        {
                            //DONE:120 remove '[' and ']' from token

                            //might have to include the iterator header
                            token = v[i];
                            token.erase(token.begin());
                            token.erase(token.end()-1);
                            token2 = QString::fromStdString(token);
                            return prepareForParse(process, token2, exception, a, b , c);
                        }
                    }
                    break;
                case 4:
                    if(conditionAnswered)
                    {
                        if(condition)
                        {
                            token = v[i];
                            token.erase(token.begin());
                            token.erase(token.end()-1);
                            token2 = QString::fromStdString(token);
                            //If condition is true, it will check that both are true
                            condition = condition && parseCondition(process, token2, exception, a, b, c);
                            if(exception == 4)
                            {
                                return falseReturn;
                            }
                        }
                        //if condition is false it won't do check it, everything is false
                    }
                    break;
                case 5:
                    if(conditionAnswered)
                    {
                        if(!condition)
                        {
                            token = v[i];
                            token.erase(token.begin());
                            token.erase(token.end()-1);
                            token2 = QString::fromStdString(token);
                            //if condition is false it will check if either of them are true
                            condition = condition || parseCondition(process, token2, exception, a, b, c);
                            if(exception == 4)
                            {
                                return falseReturn;
                            }
                        }
                        //if condition is true then it won't check the answer is true
                    }
                    break;
                default:
                    exception = 4;
                    return falseReturn;
                    //this means an error return an error
                    break;
            }
        }
    }
    return rNumber(dec::decimal_cast<8>(0),0);
}

bool parseCondition(std::vector<QString> & process, const QString & s, int& exception, rNumber & a, rNumber & b, rNumber & c)
{
    //DONE:100 tokenize parse condition more information below
    /*
        It will basically divide the tokens into two parts
            -Keywords:
                - = : meaning equals to
                - < : less than
                - > : more than
                - <>: not equals
                - <=: less or equals than
                - >=: more or equals than
            -Ecuation: Anything that i already have, might have to modify
            some stuff first which sucks cause i don't want to touch
            the original code because it sucks
        So in the end it will end up like this:
        Ecuation Keyword Ecuation I will solve, both ecuation and compare them
        There will always be three tokens, otherwise throw a syntax error
    */
    bool opUsed = false;
    std::string str = s.toStdString();
    rNumber left;
    rNumber right;
    std::vector<std::string> v;
    std::string token;
    QString token2;
    v = regexCondition(str);
    /*
        operation can have the following values:
            0: invalid token
            1: =
            2: <
            3: >
            4: <=
            5: >=
            6: <>
    */
    int operation = 0;

    //DONE:60 make sure the flow of the function works well and validate that only three tokens should exist at any given time
    int t = 0;
    for(int j = 0; j < v.size(); j++)
    {
        if(v[j] != "")
        {
            t++;
        }
    }
    if(t != 3)
    {
        //DONE:70 return an error and false here
        //make sure it sends the error up and breaks the whole opperation
        exception = 4;
        return false;
    }
    for(int i = 0; i < v.size(); i++)
    {
        if(v[i] == "")
        {
            continue;
        }
        //DONE:80 add validation methods for the different tokens
        if(opBool(v[i]))
        {
            opUsed = true;
            //this means the keyword was acceptable!
            if(opEquals(v[i]))
            {
                operation = 1;
            }
            if(opLessThan(v[i]))
            {
                operation = 2;
            }
            if(opMoreThan(v[i]))
            {
                operation = 3;
            }
            if(opLessOrEqual(v[i]))
            {
                operation = 4;
            }
            if(opMoreOrEqual(v[i]))
            {
                operation = 5;
            }
            if(opDifferent(v[i]))
            {
                operation = 6;
            }
        }
        else
        {
            //DONE:50 call method that will solve for a value
            //this part can only be a regular operation but without the = at the end
            //This is what i have to modify on the convert method ;_; i don't want to ._____.
            if(!opUsed)
            {
                //TODO:0 maybe validate if an exception happened
                //it might not be necessary because if it throws an exception it will go all the way up and throw it on calculator.cpp, but it might be better to make sure and validate every step of the way
                token = v[i];
                token2 = QString::fromStdString(token);
                left = convert(process,token2,exception,a,b,c);
            }
            else
            {
                //TODO:10 maybe validate the same thing as the if above
                token = v[i];
                token2 = QString::fromStdString(token);
                right = convert(process,token2,exception,a,b,c);
                switch (operation)
                {
                    case 1:
                        return left == right;
                    case 2:
                        return left < right;
                    case 3:
                        return left > right;
                    case 4:
                        return left <= right;
                    case 5:
                        return left >= right;
                    case 6:
                        return !(left == right);
                    default:
                        //DONE:30 call error here
                        exception = 4;
                        return false;
                        break;
                }
            }

        }

    }
}

rNumber format(std::vector<QString> & process, const QString & s, int & exception, rNumber & a, rNumber & b, rNumber & c)
{
  //TODO:30 implement the format change function, more instructions below
  /*

  */
}

//DONE:0 change convert so that it doesn't need the = at the end and put it on another method

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
    //TODO: implement removeRoot funtion
    //the removeRoot function will find out every substring that fits the format
    // root(rNumber) and transform it into rNumber^(1/2)
    // and root(rNumber,rNumber2) into rNumber2^(1/rNumber)
    // in the second case rNumber must be made into a a whole number no decimals
    str = removeRoot(str);
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

    //this splits the string into tokens
    v = regex(str);


    int cc = 0;
    //this counts that all the parenthesis are matched
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
    //DONE:200 validate that the operations are viable
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
            //DONE:180 VALIDATE THAT IF IT'S DIVIDED BY 0, THROW AN EXCEPTION
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
            //DONE:170 VALIDATE THAT IF IT'S DIVIDED BY 0, THROW AN EXCEPTION
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
