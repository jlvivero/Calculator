#include "functionSelect.h"
#include "rNumber.h"

#include <vector>
#include <QString>
#include <string>


/*
    this function is in charge of checking what type of ecuation this is, for example if it's an if it'll detect is an if and call the correct parser for each thing.

    This will allow me to nest if functions for example if i have
    if[2=2]THEN[if[3=4]THEN[0=]ELSE[3+3=]]ELSE[2=]
    it will follow this steps:
        -call convertIf that will tokenize the string to have all the keywords[] pairs
        -call the method condition which will solve 2=2 for true or false
        -if it's true it will grab the block [] that's next to the keyword THEN
        -if it's false it will grab the block [] that's next to the keyword ELSE
        -it will take that keyboard and send it back to perpareForParse
        -if it's not an if function it will call the original convert method
        -otherwise it will call other ones, it will eventually find an answer and return the value to thestack

    This will allow me to have nested if's (a big bonus for the teacher) without too much effort
*/
rNumber prepareForParse(std::vector<QString> &process, const QString &s, int &exception, rNumber &a, rNumber &b, rNumber &c)
{
    std::string str = s.toStdString();
    if(isif(str))
    {
        return convertIf(process,s,exception,a,b,c);
    }
    else
    {
      if(isFormat(str))
      {
        return format(process, s, exception, a, b, c);
      }
      else
      {
        //this means it's neither an if or a format so i do the normal parsing
        return convert(process, s, exception, a, b, c);
      }
    }
}
