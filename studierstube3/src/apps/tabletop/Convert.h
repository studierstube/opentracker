 /* ======================================================================= */
/** header file for Convert Class
  *
  * @author Christiane Ulbricht
  *
  * $Id: Convert.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <string>
#include <fstream>  
#include <iostream>
#include <Inventor/SbString.h>

using namespace std;

class Convert 
{
public:
    Convert();
    ~Convert();
    static int stringToInt(string str);
    static float stringToFloat(string str);
    static string intToString(int val);
    static char* intToChar(int val);
    static string floatToString(float val);
    static char* stringToChar(string str);
    static int sbNameToInt(SbName name);
};
#endif //_CONVERT_H_