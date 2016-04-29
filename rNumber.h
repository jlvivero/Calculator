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
    //constructors
    rNumber();
    rNumber(dec::decimal<8> number1, int number2);
    rNumber(const rNumber & n);
    rNumber(dec::decimal<8> number1, int number2, bool error);
    rNumber(std::string s);

    //operator Overloads
    rNumber operator-();
    rNumber operator+(const rNumber &b);
    rNumber operator-(const rNumber &b);
    rNumber operator*(const rNumber &b);
    rNumber operator/(const rNumber &b);
    rNumber operator^(const rNumber &b);
    bool operator<(const rNumber &b) const;
    bool operator==(const rNumber &b) const;

    inline bool operator>(const rNumber &b) {
        return b < *this;
    }

    inline bool operator<=(const rNumber &b) {
        return *this < b || *this == b;
    }

    inline bool operator>=(const rNumber &b) {
        return *this > b || *this == b;
    }

    //exterior methods that are used commonly
    void parse(std::string s);
    std::string printNumber();
    bool gerror();
    void negate();
    void copy(rNumber second);
    dec::decimal<8> getN1();
    int getN2();

    //helper methods that should mostly be used internally but i suck and use them externally as well
    rNumber add(rNumber second);
    rNumber substract(rNumber second);
    rNumber multiply(rNumber second);
    rNumber divide(rNumber second);
    rNumber exponent(rNumber n);

    //interal methods that should only be called inside the class
    void reduce();
    void parseExponent(std::string s);
    void match(int n);
    bool extend();
    std::string intToString(long int n);
    void simplifyFraction(long int &top, long int &bottom);


    //probably unused
    rNumber internalPow(long long int beforeDecimal, rNumber rleftOperand);

};


#endif
