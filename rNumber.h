#ifndef RNUMBER_H
#define RNUMBER_H

#include "decimal.h"
#include <iostream>
#include <string>

//using namespace dec;
class rNumber
{
private:
    //values
    dec::decimal<8> number1;
    int number2;
    bool error;

public:
    rNumber();
    rNumber(dec::decimal<8> number1, int number2);
    rNumber(const rNumber & n);
    rNumber(dec::decimal<8> number1, int number2, bool error);
    void reduce();
    void parseExponent(std::string s);
    void match(int n);
    bool extend();
    bool gerror();
    void parse(std::string s);
    std::string printNumber();
    rNumber add(rNumber second);
    rNumber substract(rNumber second);
    rNumber multiply(rNumber second);
    rNumber divide(rNumber second);
    rNumber exponent(rNumber n);
    void negate();
    dec::decimal<8> getN1();
    int getN2();
    void setN1(dec::decimal<8> number1);
    void setN1(int n);
    void setN2(int number2);
    void copy(rNumber second);

};


#endif
