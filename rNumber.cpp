#include "rNumber.h"
#include "decimal.h"
#include "validateTokens.h"
#include <iostream>
#include <string>
#include <cmath>
#include "log.h"
#include <sstream>

/*******************************************************************************
* CONSTRUCTORS
*******************************************************************************/

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

rNumber::rNumber(dec::decimal<8> number1, int number2, bool error)
{
    this->number1 = number1;
    this->number2 = number2;
    this->error = error;
}

rNumber::rNumber(const rNumber & n)
{
    number1 = n.number1;
    number2 = n.number2;
    error = n.error;
}

rNumber::rNumber(std::string s)
{
    parse(s);
}

/*******************************************************************************
*                           OPERATOR OVERLOADS
*******************************************************************************/
rNumber rNumber::operator-()
{
    this->negate();
    return *this;
}

rNumber rNumber::operator+(const rNumber &b)
{
    rNumber copy;
    copy = this->add(b);
    return copy;
}

rNumber rNumber::operator-(const rNumber &b) {
    rNumber copy;
    copy = this->substract(b);
    return copy;
}

rNumber rNumber::operator*(const rNumber &b) {
    rNumber copy;
    copy = this->multiply(b);
    return copy;
}

rNumber rNumber::operator/(const rNumber &b) {
    rNumber copy;
    copy = this->divide(b);
    return copy;
}

rNumber rNumber::operator^(const rNumber &b) {
    rNumber copy;
    copy = this->exponent(b);
    return copy;
}

bool rNumber::operator<(const rNumber &b) const{
    if(this->number1 == dec::decimal_cast<8>(0)){
        return b.number1 > dec::decimal_cast<8>(8);
    }

    if(b.number1 == dec::decimal_cast<8>(0)){
        return this->number1 < dec::decimal_cast<8>(0);
    }

    if(this->number2 < b.number2) return true;
    if(this->number2 > b.number2) return false;
    return this->number1 < b.number1;
}

bool rNumber::operator==(const rNumber &b) const {
    return this->number2 == b.number2 && this->number1 == b.number1;
}


/*******************************************************************************
*                  EXTERIOR METHODS THAT ARE COMMONLY USED
*******************************************************************************/

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
        int c = 0;
        int d = 0;
        int b = 0;
        bool found = false;
        bool found2 = false;
        std::string value;
        dec::toString(number1,value);
        for(int i = 0; i < value.size(); i ++)
        {
            if(value[i] == '.')
            {
                found = true;
            }
            else
            {
                b++;
                if(found)
                {
                    c++;
                }
            }
        }

        if(b > 8)
        {
            d = 0;
            while(c > 0)
            {
                if(b <= 8)
                    break;
                b--;
                c--;
                d++;
            }
        }

        std::string fixedValue = "";
        for(int i = 0; i < (value.size() - d); i++)
        {
            fixedValue = fixedValue + value[i];
        }

        value = "";
        d = 0;
        for(int i = (fixedValue.size() - 1); i >= 0; i--)
        {
            if(fixedValue[i] == '0' && found)
            {
                d++;
            }
            else
            {
                if(fixedValue[i] == '.')
                {
                    found2 = true;
                }
                else
                {
                    found2 = false;
                }
                break;
            }

        }
        if(found2){d++;}
        for(int i = 0; i < (fixedValue.size() - d); i++)
        {
            value = value + fixedValue[i];
        }
        return value;
    }
    else
    {
        int c = 0;
        int d = 0;
        int b = 0;
        bool found = false;
        bool found2 = false;
        std::string value;
        dec::toString(number1,value);
        for(int i = 0; i < value.size(); i ++)
        {
            if(value[i] == '.')
            {
                found = true;
            }
            else
            {
                b++;
                if(found)
                {
                    c++;
                }
            }
        }

        if(b > 8)
        {
            d = 0;
            while(c > 0)
            {
                if(b <= 8)
                    break;
                b--;
                c--;
                d++;
            }
        }

        std::string fixedValue = "";
        for(int i = 0; i < (value.size() - d); i++)
        {
            fixedValue = fixedValue + value[i];
        }

        value = "";
        d = 0;
        for(int i = (fixedValue.size() - 1); i >= 0; i--)
        {
            if(fixedValue[i] == '0' && found)
            {
                d++;
            }
            else
            {
                if(fixedValue[i] == '.')
                {
                    found2 = true;
                }
                else
                {
                    found2 = false;
                }
                break;
            }

        }
        if(found2){d++;}
        for(int i = 0; i < (fixedValue.size() - d); i++)
        {
            value = value + fixedValue[i];
        }

        value = value + "E";
        value = value + std::to_string(number2);
        return value;
    }
}

bool rNumber::gerror()
{
    return error;
}

void rNumber::negate()
{
    number1 = number1 * dec::decimal_cast<8>(-1);
}

void rNumber::copy(rNumber second)
{
    this->number1 = second.getN1();
    this->number2 = second.getN2();
}

dec::decimal<8> rNumber::getN1()
{
    return number1;
}

int rNumber::getN2()
{
    return number2;
}

/*******************************************************************************
* HELPER METHODS THAT SHOULD BE USED INTERNALLY BUT I SUCK AND USE THEM
* EXTERNALLY AS WELL
*******************************************************************************/

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
            if(!(number2 == second.getN2()))
                match(second.getN2());

            hold = number1 - second.number1;
            rNumber temp(hold,second.number2);
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


//IM A FUCKING CHITERO
rNumber rNumber::exponent(rNumber second)
{
    //declare most variables
    rNumber legit;
    bool notEqual = false;
    long int bdec, adec, absolute;
    long int unpacktemp, checkDecimal;
    long int bottom = 9;
    dec::decimal<8> temp, rightOperand;

    // function does a^b legit will be a, rightOperand and absolute represent b
    legit.copy(*this);
    rightOperand = second.number1; // this is b
    absolute = second.number2;// this is the number after the E of b
    if(absolute < 0){absolute = absolute * -1;}//this is to make sure it loops well
    //this converts both the decimal version and the double version of the number into the orignal number with E0
    for(int i = 0; i < absolute; i++)
    {
        if(second.number2 < 0){
            //fuckMyLife = fuckMyLife / 10;
            rightOperand = rightOperand / dec::decimal_cast<8>(10);
        }
        else{
            //fuckMyLife = fuckMyLife * 10;
            rightOperand = rightOperand * dec::decimal_cast<8>(10);
        }
    }
    /***************************************************************************
    *   in the format a^b, if b is not a fraction, then I have no problem and
    *   can do the regular form of the exponents, weather a is negative or not
    *   if b is a fraction, then i have to use logarithms and it brings problems
    *   when a is negative
    ***************************************************************************/
    rightOperand.unpack(unpacktemp, checkDecimal);
    if(checkDecimal == 0)
    {
        //call the regular methods
        legit = internalPow(unpacktemp,legit);
        legit.reduce();
        return legit;
    }

    //if number1 is positive, then you can just cast it without any problem
    if(legit.number1 > dec::decimal_cast<8>(0))
    {
        legit = pow2(legit,second);
        legit.reduce();
        return legit;
    }

    /***************************************************************************
    *   this is the case where a is negative and b is a decimal number
    ***************************************************************************/
    long int beforeDecimal,afterDecimal;
    //unpack b
    rightOperand.unpack(beforeDecimal,afterDecimal);

    //convert the decimal part into a string
    std::string Result;
    Result = intToString(afterDecimal);

    std::string compare;
    std::string resultletter;
    for(int i = 0; i < Result.size(); i++)
    {
        if(i == 0)
        {
            compare = Result[i];
        }
        else
        {
            resultletter = Result[i];
            if(compare != resultletter)
            {
                notEqual = true;
            }
        }
    }

    /***************************************************************************
    *   there are three cases of decimals that I know so far:
    *   case one is where they have a repeating number for example .333333333
    *   case two  is when they don't but it's a finite number for example
    *   .5 or .23 anything of sorts.
    *   I know of another case(3) that i have not encountered, for example
    *   .2323232323, repeats infinitely but with more than one number.
    ***************************************************************************/

    //if notEqual is true it means it's the second case
    if(notEqual)
    {
        dec::decimal<8> temp2 = rightOperand;
        //temp2 is b
        temp2.unpack(bdec,adec);
        int counter = 0;
        //this counts how many decimals are in place
        while(adec > 0)
        {
            adec = adec / 10;
            counter++;
        }
        //counter = 1
        temp2.unpack(bdec,adec);
        bottom = 1;
        for(int j = 0; j < counter; j++)
        {
            bottom = bottom * 10;
        }
        //from here i have a fraction adec/bottom with bdec as the whole number
        //i add the whole number here and get adec / bottom
        adec = adec + (bdec * bottom);
        simplifyFraction(adec,bottom);

        //number1 is always negative in this scenario
        if(adec != 1)
        {
            temp.pack(adec,0);
            legit.negate(); //turns into positive
            legit = pow2(legit,rNumber(temp,0));
            if(adec % 2 != 0)
            {
                //if the b is not even then you turn it back to negative
                legit.negate();
            }
        }
        //in here you do not know if it's negative all the time
        if(legit.number1 < dec::decimal_cast<8>(0))
        {
            //if it's negative
            if(bottom % 2 == 0)
            {
                //if a is a negative number, and b is even on and a square
                //then it's a failure
                rNumber failure(dec::decimal_cast<8>(0),0,true);
                return failure;
            }
            dec::decimal<8> divisor(bottom);
            temp = dec::decimal_cast<8>(1) / divisor;
            legit.negate(); // turns into positive
            legit = pow2(legit,rNumber(temp,0));
            legit.negate();
        }
        else
        {
            dec::decimal<8> divisor(bottom);
            temp = dec::decimal_cast<8>(1) / divisor;
            legit = pow2(legit, rNumber(temp,0));
        }
    }
    else
    {
        dec::decimal<8> tempx10 = rightOperand * dec::decimal_cast<8>(10);
        dec::decimal<8> tempx1 = rightOperand;
        dec::decimal<8> tempx9 = tempx10 - tempx1;

        long int decimal, integ;
        tempx9.unpack(integ,decimal);

        //now checks if there's a bunch of 9999
        std::string Result2;          // string which will contain the result
        Result2 = intToString(decimal);

        bottom = 9;
        if(Result2[1] == '9' && Result2[2] == '9')
        {
            tempx9 = tempx9 + dec::decimal_cast<8>(.01);
        }

        tempx9.unpack(bdec,adec);
        //i get fraction bdec / bottom
        simplifyFraction(bdec,bottom);

        //we already know it's a negative number
        if(bdec != 1)
        {
            temp.pack(bdec,0);
            legit.negate(); // turns it into positive number
            legit = pow2(legit,rNumber(temp,0));
            if(bdec % 2 != 0)
            {
                //if the b is not even then you turn it back to negative
                legit.negate();
            }
        }

        //in here you do not know if it's negative all the time
        if(legit.number1 < dec::decimal_cast<8>(0))
        {
            if(bottom % 2 == 0)
            {
                rNumber failure(dec::decimal_cast<8>(0),0,true);
                return failure;
            }
            dec::decimal<8> divisor(bottom);
            temp = dec::decimal_cast<8>(1) / divisor;
            legit.negate(); // turns it into a positive number
            legit = pow2(legit,rNumber(temp,0));
            legit.negate();
        }
        else
        {
            dec::decimal<8> divisor(bottom);
            temp = dec::decimal_cast<8>(1) / divisor;
            legit = pow2(legit, rNumber(temp,0));
        }
    }

    legit.reduce();
    return legit;
}

/*******************************************************************************
*     INTERNAL METHODS THAT SHOULD ONLY BE CALLED FROM INSIDE THE CLASS
*******************************************************************************/

void rNumber::reduce()
{
    bool negative = false;
    dec::decimal<8> temp(10);
    dec::decimal<8> temp2(1);
    dec::decimal<8> useless(0);
    dec::decimal<8> temp3;
    if(number1 == useless)
    {
        number2 = 0;
        return;
    }
    if(number1 < useless)
    {
        temp3 = number1 * dec::decimal_cast<8>(-1);
    }
    else
    {
        temp3 = number1;
    }
    while(temp3 > temp)
    {
        number1 = number1 / dec::decimal_cast<8>(10);
        temp3 = temp3/ dec::decimal_cast<8>(10);
        number2++;
    }
    while(temp3 < temp2)
    {
        number1 = number1 * dec::decimal_cast<8>(10);
        temp3 = temp3 * dec::decimal_cast<8>(10);
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
    if(exponentPart.size() > 0)
    {
        dec::fromString(decimalPart,number1);
        number2 = std::stoi(exponentPart,nullptr);
    }
    else
    {
        error = true;
    }
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
        if(significantFigures + (number2*-1) <= 8)
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

std::string rNumber::intToString(long int n)
{
    ostringstream convert;   // stream used for the conversion
    convert << n;      // insert the textual representation of
    return convert.str();
}

void rNumber::simplifyFraction(long int & top, long int & bottom)
{
    if(top > bottom)
    {
        for(int j = 2; j <= bottom; j++)
        {
            while(bottom % j == 0 && top % j == 0)
            {
                top = top / j;
                bottom = bottom / j;
            }
        }
    }
    else
    {
        for(int j = 2; j <= top; j++)
        {
            while(bottom % j == 0 && top % j == 0)
            {
                top = top / j;
                bottom = bottom / j;
            }
        }
    }
}

/*******************************************************************************
*                 PROBABLY UNUSED METHODS, DELETE IF POSSIBLE
*******************************************************************************/

rNumber rNumber::internalPow(long long int beforeDecimal, rNumber rleftOperand)
{
    if(beforeDecimal == 0){
        return rNumber(dec::decimal_cast<8>(1),0);
    }
    else{
        if(beforeDecimal % 2 == 0 || beforeDecimal % 3 == 0){
            if(beforeDecimal % 3 == 0){
                rNumber thirdPow = internalPow(beforeDecimal / 3, rleftOperand);
                return thirdPow * thirdPow * thirdPow;
            }
            else{
                rNumber halfPow = internalPow(beforeDecimal / 2,rleftOperand);
                return halfPow * halfPow;
            }
        }
        else{
            if(beforeDecimal < 0){
                return rNumber(dec::decimal_cast<8>(1),0)/internalPow(beforeDecimal * -1, rleftOperand);
            }
            else{
                return rleftOperand * internalPow(beforeDecimal - 1, rleftOperand);
            }
        }
    }
}
