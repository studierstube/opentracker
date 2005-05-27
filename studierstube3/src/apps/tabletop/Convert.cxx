 /* ========================================================================
  * (C) 2002 Vienna University of Technology
  * ========================================================================
  * PROJECT: Tabletop Game
  * ======================================================================== */ 
/** contains the implementation of the Convert class
  *
  * @author  Christiane Ulbricht
  *
  * $Id: Convert.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

/** source file for Convert class.*/

#include "Convert.h"

Convert::Convert() 
{

}
Convert::~Convert() 
{
}

int 
Convert::stringToInt(string str)
{    
    char* cStr = stringToChar(str);
    int ret = atoi(cStr);
    delete []cStr;
    return ret;
}

float
Convert::stringToFloat(string str)
{    
    char* cStr = stringToChar(str);
    float ret = atof(cStr);
    delete []cStr;
    return ret;
}

string 
Convert::intToString(int val)
{    
    int count = 1, temp = val;
    //count the number of digits of val
    while (temp >= 10)  
    {
        temp /= 10;
        count++;
    }
    //+1 for the "end of string sign"
    char* cStr = (char*)malloc(count+1); 
    itoa(val,cStr,10);
    cStr[count] = 0;
    string s(cStr);
    delete []cStr;
    return s;
}

char* 
Convert::intToChar(int val)
{    
    int count = 1, temp = val;
    //count the number of digits of val
    while (temp >= 10)  
    {
        temp /= 10;
        count++;
    }
    //+1 for the "end of string sign"
    char* cStr = (char*)malloc(count+1);
    cStr = itoa(val,cStr,10);
    cStr[count] = 0;
    return cStr;
}

string 
Convert::floatToString(float val)
{    
    //+1 for the "end of string sign"
    char* cStr = (char*)malloc(32+1); 
    sprintf(cStr,"%f",val);
    cStr[32] = 0;
    string s(cStr);
    delete []cStr;
    return s;
}

char* 
Convert::stringToChar(string str)
{
    //+1 for the "end of string sign"
    char* cArray=(char*)malloc(str.length()+1);
    str.copy(cArray, str.length());
    cArray[str.length()]=0;
    return cArray;
}

int 
Convert::sbNameToInt(SbName name)
{
    const char* tempName = name.getString();
    char* temp = new char[(int)floor(log(sizeof(tempName)))];
    sprintf(temp,"%c",tempName[1]);
    temp[sizeof(tempName)]=0;
    int colorNr = atoi(temp);
    return colorNr;
}

