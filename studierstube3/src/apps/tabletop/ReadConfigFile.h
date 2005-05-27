 /* ======================================================================= */
/** header file for ReadConfigFile Class
  *
  * @author Christiane Ulbricht
  *
  * $Id: ReadConfigFile.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _READCONFIGFILE_H_
#define _READCONFIGFILE_H_

#include <string>
#include <fstream>  
#include <iostream>

using namespace std;

class ReadConfigFile 
{
    string fileContent;
public:
    ReadConfigFile(string filename);
    ~ReadConfigFile();
    string getValueOf(string name);
    string getValueOrDefault(string name, string defaultVal);
};
#endif //_READCONFIGFILE_H_