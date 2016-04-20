#include "rNumber.h"
#include "decimal.h"
#include "validateTokens.h"
#include <iostream>
#include <string>


rNumber::rNumber()
{
    //number1 = 0;
    //number2 = 0;
    //error = false;
    //nothing so far, idk if it's worth adding anything here
}


rNumber::rNumber(dec::decimal<8> number1, int number2)
{
    this->number1 = number1;
    this->number2 = number2;
    error = false;
}

rNumber::rNumber(const rNumber & n)
{
    number1 = n.number1;
    number2 = n.number2;
    error = n.error;
}

void rNumber::reduce()
{
    dec::decimal<8> temp(10);
    dec::decimal<8> temp2(1);
    dec::decimal<8> useless(0);
    if(number1 == useless)
    {
        number2 = 0;
        return;
    }
    while(number1 > temp)
    {
        number1 = number1 / dec::decimal_cast<8>(10);
        number2++;
    }
    while(number1 < temp2)
    {
        number1 = number1 * dec::decimal_cast<8>(10);
        number2--;
    }
    if(number2 > 99 || number2 < -99)
    {
        error = true;
    }
}

void rNumber::parseExponent(std::string s)
{
    std::string exponentPart = "";
    std::string decimalPart = "";
    bool ePassed = false;
    //parse the number, everything before the E goes to decimal, everything after goes to integer
    for(int i = 0; i < s.size(); i++)
    {
        // maybe it's 'E'
        if(s[i] == 'E')
        {
            ePassed = true;
        }
        else
        {
            if(ePassed)
            {
                exponentPart = exponentPart + s[i];
            }
            else
            {
                decimalPart = decimalPart + s[i];
            }
        }
    }
    //dec::decimal<8> dummystuff(decimalPart);
    //idk which option is correct, test this
    //number1 = number1->fromString(decimalPart);
    //number1->fromString(decimalPart);
    //number1(decimalPart);
    //number1 = number1.fromString(decimalPart);
    //number1 = dummystuff;
    dec::fromString(decimalPart,number1);
    number2 = std::stoi(exponentPart,nullptr);
    if(number2 > 99)
    {
        error = true;
    }
    else
    {
        reduce();
    }

}

void rNumber::match(int n)
{
    while(number2 != n)
    {
        number1 = number1 / dec::decimal_cast<8>(10);
        number2++;
    }
}

bool rNumber::extend()
{
    dec::decimal<8> temp;
    std::string hold;
    int significantFigures;
    while(number2 <=7 && number2 > 0)
    {
        number1 = number1 * dec::decimal_cast<8>(10);
        number2--;
    }
    if(number2 == 0)
    {
        return true;
    }
    if(number2 < 0)
    {
        temp = number1;
        if(temp < dec::decimal_cast<8>(0))
        {
            //temp.toString(temp,hold);
            dec::toString(temp,hold);
            if(isDecimal(hold))
            {
                significantFigures = hold.size() - 3;
            }
            else
            {
                significantFigures = 0;
            }
        }
        else
        {
            //temp.toString(temp,hold);
            dec::toString(temp,hold);
            if(isDecimal(hold))
            {
                significantFigures = hold.size() - 2;
            }
            else
            {
                significantFigures = 0;
            }
        }
        if(significantFigures + number2 <= 8)
        {
            while(number2 < 0)
            {
                number1 = number1 / dec::decimal_cast<8>(10);
                number2++;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}




bool rNumber::gerror()
{
    return error;
}

void rNumber::parse(std::string s)
{
    //parses the string to a real number
    if(isExp(s))
    {
        parseExponent(s);
    }
    else
    {
        //dec::decimal<8> temp(s);
        //number1 = temp;
        dec::fromString(s,number1);
        number2 = 0;
        reduce();
    }
}

std::string rNumber::printNumber()
{
    if(extend())
    {
        //not sure if this is how it works
        //the number1.toString(number1,value) THING
        std::string value;
        //number1.toString(number1,value);
        dec::toString(number1,value);
        return value;
    }
    else
    {
        std::string value;
        //number1.toString(number1,value);
        dec::toString(number1,value);
        value = value + "E";
        value = value + std::to_string(number2);
        return value;
    }
}

rNumber rNumber::add(rNumber second)
{
    dec::decimal<8> hold;
    if(number2 > second.getN2())
    {
        if(number2 - second.getN2() > 8)
        {
            rNumber wtv(number1,number2);
            return wtv;
        }
        else
        {
            second.match(number2);
            hold = number1 + second.getN1();
            rNumber temp(hold, number2);
            temp.reduce();
            return temp;
        }
    }
    else
    {
        if(second.getN2() - number2 > 8)
        {
            return second;
        }
        else
        {
            match(second.getN2());
            hold = number1 + second.getN1();
            rNumber temp(hold,second.getN2());
            temp.reduce();
            return temp;
        }
    }
}

rNumber rNumber::substract(rNumber second)
{
    dec::decimal<8> hold;
    if(number2 > second.getN2())
    {
        if(number2 - second.getN2() > 8)
        {
            rNumber wtv(number1,number2);
            return wtv;
        }
        else
        {
            second.match(number2);
            hold = number1 - second.getN1();
            rNumber temp(hold, number2);
            temp.reduce();
            return temp;
        }
    }
    else
    {
        if(second.getN2() - number2 > 8)
        {
            return second;
        }
        else
        {
            match(second.getN2());
            hold = number1 - second.getN1();
            rNumber temp(hold,second.getN2());
            temp.reduce();
            return temp;
        }
    }
}

rNumber rNumber::multiply(rNumber second)
{
    dec::decimal<8> hold;
    hold = number1 * second.getN1();
    rNumber temp(hold,(number2 + second.getN2()));
    temp.reduce();
    return temp;
}


rNumber rNumber::divide(rNumber second)
{
    dec::decimal<8> hold;
    hold = number1 / second.getN1();
    rNumber temp(hold,(number2 - second.getN2()));
    temp.reduce();
    return temp;
}

rNumber rNumber::exponent(rNumber second)
{
    //TODO: IMPLEMENT EXPONENT METHOD
    return second;
}

void rNumber::negate()
{
    number1 = number1 * dec::decimal_cast<8>(-1);
}

dec::decimal<8> rNumber::getN1()
{
    return number1;
}

int rNumber::getN2()
{
    return number2;
}

//might want to overload it so that it accepts doubles/floats/int etc.
void rNumber::setN1(dec::decimal<8> number1)
{
    this->number1 = number1;
}
//might be this->number1 = number1 anyways, it's weird idk how this class really works yet
void rNumber::setN1(int number1)
{
    this->number1 = dec::decimal_cast<8>(number1);
}

void rNumber::setN2(int number2)
{
    this->number2 = number2;
}

void rNumber::copy(rNumber second)
{
    this->number1 = second.getN1();
    this->number2 = second.getN2();
}
