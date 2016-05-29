#include "functionSelect.h"
#include "rNumber.h"
#include "validateTokens.h"
#include "parser.h"

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
      if(isFormat(s))
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

//IM REALLY NOT SURE THIS SHOULD BE HERE BUT IMA DO IT FOR NOW HERE SO I DON'T
//HAVE TO MAKE A NEW CPP FILE
std::string removeRoot(std::string s)
{
    std::vector<std::string> v;
    std::vector<std::string> u;
    //v = regexRoot(s);
    u = regexRoot(s);
    //this will tokenize everything and have root[] as a token
    std::string token;
    std::string wordNew = "";
    std::string newString = "";
    //this will give you every instance of root[anything] in a vector
    for(int j = 0; j < u.size(); j++)
    {
        if(isRoot(u[j]))
        {
            token = u[j];
            token.erase(0,5);
            token.erase(token.end()-1);
            v = regexRoot2(token);
            //this will tokenize whatever is inside the []
            if(v.size() == 1)
            {
                wordNew = v[0] + "^(1/2)";
            }
            else
            {
                wordNew = "(" + v[2] + ")^(1/" + v[0] + ")";
            }
            u[j] = wordNew;
        }
    }
    for(int j = 0; j < u.size(); j++)
    {
        newString = newString + u[j];
    }
    return newString;
}
//root[2,10*10]=
//root[2,10*10]
//2;,;10*10
//(10*10)^(1/(2))
