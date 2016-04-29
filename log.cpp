#include "rNumber.h"
#include <iostream>
#include <cmath>
#include "decimal.h"
using namespace std;

rNumber factorial (int n) {
  if(n == 0) return rNumber (dec::decimal_cast<8>(1), 0);
  if(n == 1) return rNumber (dec::decimal_cast<8>(1),0);
  return rNumber(dec::decimal_cast<8>(n), 0) * factorial(n-1);
}
rNumber exp(rNumber a, rNumber b) {
  if(b < rNumber(dec::decimal_cast<8>(0),0)) return exp(rNumber(dec::decimal_cast<8>(1),0) / a, -b);
  if(b == rNumber(dec::decimal_cast<8>(0),0)) return rNumber(dec::decimal_cast<8>(1),0);
  if(b == rNumber(dec::decimal_cast<8>(1),0)) return a;
    return a * exp(a , b - rNumber(dec::decimal_cast<8>(1),0));
  }

rNumber decimalExp(rNumber power) {
  rNumber result(dec::decimal_cast<8>(1),0);
  for(int iter = 1; iter < 150; iter++ ) {
    rNumber factoria = factorial(iter);
    result = result + exp(power, rNumber(dec::decimal_cast<8>(iter), 0)) / factoria;
  }
  return result;
}
rNumber LogN(rNumber number) {
  rNumber x = number - rNumber(dec::decimal_cast<8>(1), 0);

  rNumber result = x;

  for(int i = 2; i < 40; i++) {
    if(i%2 == 0) {
      result = result - (exp(x, rNumber(dec::decimal_cast<8>(i),0)) / rNumber(dec::decimal_cast<8>(i),0));
    }
    else {
      result = result + (exp(x, rNumber(dec::decimal_cast<8>(i),0)) / rNumber(dec::decimal_cast<8>(i),0));
    }
  }
  return result;

}
rNumber LogN2(rNumber n) {
  rNumber x = (n - rNumber(dec::decimal_cast<8>(1),0)) / (n + rNumber(dec::decimal_cast<8>(1),0));

  rNumber ans = rNumber(dec::decimal_cast<8>(0),0);
  for(int i = 1; i < 500 ; i += 2) {
    ans = ans +  rNumber(dec::decimal_cast<8>(2),0) * exp(x, rNumber(dec::decimal_cast<8>(i),0))/ rNumber(dec::decimal_cast<8>(i),0) ;
  }
  return ans;
}
rNumber preN(rNumber number) {
  int i = 0;
  while(number >= rNumber(dec::decimal_cast<8>(2),0)) {
    number = number /rNumber(dec::decimal_cast<8>(2),0);
    i++;
  }

  // ln 11
  //return Number("2.3978952","0") * Number(i,0) + LogN2(number);
   //ln 3
  //return Number("1.0986123","0") * Number(i,0) + LogN2(number);
   //ln 2
  return rNumber("0.69314718") * rNumber(dec::decimal_cast<8>(i),0) + LogN(number);
}

rNumber preN2(rNumber number) {
  int i = 0;
  while(number >= rNumber(dec::decimal_cast<8>(83),0)) {
    number = number /rNumber(dec::decimal_cast<8>(83),0);
    i++;
  }
  //cout << "number beforeLogN: " << number << '\n';

   //ln 47
  //return Number("3.8501476","0") * Number(i,0) + LogN2(number);
   //ln 71
  //return Number("4.2626798","0") * Number(i,0) + LogN2(number);
  //ln 83
  return rNumber("4.4188406") * rNumber(dec::decimal_cast<8>(i),0) + LogN2(number);
}

rNumber pow2(rNumber a, rNumber b) {
  //cout << " valueof pren  :  "<< preN(a) << '\n';
  rNumber x = b * preN(a);
  //cout << "to be E :  "<< x<< '\n';
  return decimalExp(x);
}
rNumber pow3(rNumber a, rNumber b) {
  //cout << " valueof pren  :  "<< preN(a) << '\n';
  rNumber x = b * preN2(a);
  //cout << "to be E :  "<< x<< '\n';
  return decimalExp(x);
}
