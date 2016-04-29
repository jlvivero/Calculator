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
    /***************************************************************************
    *  I SUCK
    **************************************************************************/
    rNumber legit;
    legit.copy(*this);
    if(legit.number1 > dec::decimal_cast<8>(0))
    {
        legit = pow2(legit,second);
        legit.reduce();
        return legit;
    }
    bool notEqual = false;
    long int bdec, adec;
    long int bottom = 9;
    dec::decimal<8> temp;

    dec::decimal<8> rightOperand = second.number1; // this is b
    int absolute = second.number2;// this is the number after the E of b
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
    long int beforeDecimal,afterDecimal;
    rightOperand.unpack(beforeDecimal,afterDecimal);
       // number to be converted to a string
    string Result;          // string which will contain the result
    ostringstream convert;   // stream used for the conversion
    convert << afterDecimal;      // insert the textual representation of 'Number' in the characters in the stream
    Result = convert.str();
    string compare;
    string resultletter;
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
    if(notEqual)
    {
        dec::decimal<8> temp2 = rightOperand;
        //do code that does the .5 thing
        temp2.unpack(bdec,adec);
        int counter = 0;
        while(adec > 0)
        {
            adec = adec / 10;
            counter++;
        }
        temp2.unpack(bdec,adec);
        bottom = 1;
        for(int j = 0; j < counter; j++)
        {
            bottom = bottom * 10;
        }

        //this is simplified fraction
        if(bdec > bottom)
        {
            for(int j = 2; j < bottom; j++)
            {
                while(bottom % j == 0 && bdec % j == 0)
                {
                    bdec = bdec / j;
                    bottom = bottom / j;
                }
            }
        }
        else
        {
            for(int j = 2; j < bdec; j++)
            {
                while(bottom % j == 0 && bdec % j == 0)
                {
                    bdec = bdec / j;
                    bottom = bottom / j;
                }
            }
        }
        bdec = bdec + (adec * bottom);
        if(legit.number1 < dec::decimal_cast<8>(0))
        {
            if(bdec != 1)
            {
                temp.pack(bdec,0);
                legit.negate();
                legit = pow2(legit,rNumber(temp,0));
                if(bdec % 2 == 0)
                {
                    legit.negate();
                }
            }
        }
        else
        {
            if(bdec != 1)
            {
                temp.pack(bdec,0);
                legit.negate();
                legit = pow2(legit,rNumber(temp,0));
            }
        }
        if(legit.number1 < dec::decimal_cast<8>(0))
        {
            temp.pack(0,bottom);
            legit.negate();
            legit = pow2(legit,rNumber(temp,0));
            if(bottom % 2 == 0)
            {
                rNumber failure(dec::decimal_cast<8>(0),0,true);
                return failure;
            }
            legit.negate();
        }
        else
        {
            temp.pack(0,bottom);
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
        string Result2;          // string which will contain the result
        ostringstream convert;   // stream used for the conversion
        convert << decimal;      // insert the textual representation of 'Number' in the characters in the stream
        Result2 = convert.str();
        bottom = 9;
        if(Result2[1] == '9' && Result2[2] == '9')
        {
            tempx9 = tempx9 + dec::decimal_cast<8>(.01);
        }
        tempx9.unpack(bdec,adec);
        //this is the code that simplifies the fraction
        if(bdec > bottom)
        {
            for(int j = 2; j <= bottom; j++)
            {
                while(bottom % j == 0 && bdec % j == 0)
                {
                    bdec = bdec / j;
                    bottom = bottom / j;
                }
            }
        }
        else
        {
            for(int j = 2; j <= bdec; j++)
            {
                while(bottom % j == 0 && bdec % j == 0)
                {
                    bdec = bdec / j;
                    bottom = bottom / j;
                }
            }
        }
        if(legit.number1 < dec::decimal_cast<8>(0))
        {
            if(bdec != 1)
            {
                temp.pack(bdec,0);
                legit.negate();
                legit = pow2(legit,rNumber(temp,0));
                if(bdec % 2 == 0)
                {
                    legit.negate();
                }
            }
        }
        else
        {
            if(bdec != 1)
            {
                temp.pack(bdec,0);
                legit.negate();
                legit = pow2(legit,rNumber(temp,0));
            }
        }
        if(legit.number1 < dec::decimal_cast<8>(0))
        {
            temp.pack(0,bottom);
            legit.negate();
            legit = pow2(legit,rNumber(temp,0));
            if(bottom % 2 == 0)
            {
                rNumber failure(dec::decimal_cast<8>(0),0,true);
                return failure;
            }
            legit.negate();
        }
        else
        {
            temp.pack(0,bottom);
            legit = pow3(legit,rNumber(temp,0));
            //legit.negate();
        }
    }
    //legit = pow2(*this,copy);
    /*int zero = 0;
    //this is the number of the right side of the exponenciation a^b
    //double fuckMyLife = rightOperand.getAsDouble();// this is b for pow, not yet



    //this is the number of the left side of the exponenciation a^b
    //double leftOperand = number1.getAsDouble();//this is a for pow not yet

    //this is are both a, one in rNumbers and the other in dec
    rNumber rleftOperand(number1,number2);
    dec::decimal<8> dleftOperand = number1;

    //reusing decimal for the same purpose
    absolute = number2;

    if(absolute < 0){ absolute = absolute * -1;}//this makes sure it loops well
    //this converts both the decimal version and the double version of the number into the orignal number with E0
    for(int i = 0; i < absolute; i++)
    {
        if(number2 < 0){
            //leftOperand = leftOperand / 10;
            dleftOperand = dleftOperand / dec::decimal_cast<8>(10);
        }
        else{
            //leftOperand = leftOperand * 10;
            dleftOperand = dleftOperand * dec::decimal_cast<8>(10);
        }
    }

    //this is the part where i get the pow, this is where i put my conditions


    //this is for the pow
    double imCheating = 0;
    //this is the number ill check on the right operand
    //this divides the right operand into a before decimal and afterDecimal

    //this packs into a new decimal number, only the number after the decimal. so it ends up being 0.wtv is left
    dec::decimal<8> tooPac;*/
    //simplifiedPow makes a shortcut for the decimal part of the operation
    /*
        example if i have 2^20.5
        i divide this into two parts 2^20
        and then result^0.5
    */
    //trying using rnumbers all the time for now
    //if(beforeDecimal != 0)
        //simplifiedPow(beforeDecimal,rleftOperand);
    //simplifiedPow(beforeDecimal,dleftOperand);

    /*

    if(beforeDecimal != 0){
        legit = internalPow(beforeDecimal, rleftOperand);
        legit.reduce();
    }
    else{
        zero++;
        legit.copy(rleftOperand);
    }
    if(legit.number2 > 99 || legit.number2 < -99){
        rNumber failure(dec::decimal_cast<8>(1),0,true);
        return failure;
    }

    //if(!(beforeDecimal < 0)){
    //    imCheating = internalPow(beforeDecimal,dleftOperand);
    //}

    // after this comes the second part of the hard part per say
    //now i have my number, and i only have to exponentiate by the afterDecimal
    if(afterDecimal != 0){
        tooPac.pack(1,afterDecimal);
        rNumber rrightOperand(tooPac,0);
        legit = pow2(legit,rrightOperand);
    }
    else{
        zero++;
    }
    if(zero == 2)
    {
        legit.parse("0");
    }*/
    legit.reduce();



    //imCheating = pow(leftOperand,fuckMyLife);
    /*if(std::isnan(imCheating) || std::isinf(imCheating))
    {
        if(std::isinf(imCheating))
        {
            rNumber failure(dec::decimal_cast<8>(1),0,true);
            return failure;
        }
        rNumber failure(dec::decimal_cast<8>(0),0,true);
        return failure;
    }*/
    //dec::decimal<8> mylife(imCheating);
    //rNumber temp(mylife,0);
    //temp.reduce();
    //return temp;
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

/*******************************************************************************
*                 PROBABLY UNUSED METHODS, DELETE IF POSSIBLE
*******************************************************************************/

/*
    Example of how internal pow works
    if i have 2^20
    i don't have to multiply 2*2 20 times
    I can do 2*2 = 4 and divide 20 by 2
    4^10 is the same as 2^20
    then 16^5 = 2^20 = 4^10
    after you finished "simplifying you can do the regular pow"
*/
void rNumber::simplifiedPow(long int &beforeDecimal, dec::decimal<8> &dleftOperand)
{
    while(beforeDecimal % 3 == 0)
    {
        beforeDecimal = beforeDecimal/3;
        dleftOperand = dleftOperand * dleftOperand * dleftOperand;
    }
    while(beforeDecimal % 2 == 0)
    {
        beforeDecimal = beforeDecimal / 2;
        dleftOperand = dleftOperand * dleftOperand;
    }
}

void rNumber::simplifiedPow(long int &beforeDecimal, rNumber &rleftOperand)
{
    if(beforeDecimal == 0){return;}
    while(beforeDecimal % 3 == 0)
    {
        beforeDecimal = beforeDecimal / 3;
        rleftOperand = rleftOperand * rleftOperand * rleftOperand;
    }
    while(beforeDecimal % 2 == 0)
    {
        beforeDecimal = beforeDecimal / 2;
        rleftOperand = rleftOperand * rleftOperand;
    }
}

double rNumber::internalPow(long long int beforeDecimal, dec::decimal<8> dleftOperand)
{
    if(beforeDecimal == 0){
        return 1;
    }
    else{
        if(beforeDecimal < 0){
            return 1/internalPow(beforeDecimal * -1,dleftOperand);
        }
        else{
            dec::decimal<8> convert(dleftOperand * dec::decimal_cast<8>(internalPow(beforeDecimal -1,dleftOperand)));
            return convert.getAsDouble();
        }
    }
}

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
