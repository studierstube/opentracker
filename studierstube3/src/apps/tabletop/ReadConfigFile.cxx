 /* ========================================================================
  * (C) 2002 Vienna University of Technology
  * ========================================================================
  * PROJECT: Tabletop Game
  * ======================================================================== */ 
/** contains the implementation of the ReadConfigFile class
  *
  * @author  Christiane Ulbricht
  *
  * $Id: ReadConfigFile.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

/** source file for ReadConfigFile class.*/

#include "ReadConfigFile.h"
#include "Convert.h"

ReadConfigFile::ReadConfigFile(string filename) 
{
    char* cName = Convert::stringToChar(filename);
  
    ifstream iFile(cName);
    delete []cName;

    char* cLine = new char[100];
    fileContent = string("");
    while(iFile.getline(cLine, 100)) 
    {
        fileContent += cLine;
        fileContent += "\n";
    }
}
ReadConfigFile::~ReadConfigFile() 
{
}

string ReadConfigFile::getValueOf(string name)
{
    int endOfLine;
    int j = fileContent.find(name);

    if (j>-1) 
    {
        j = fileContent.find("=",j);
        endOfLine = fileContent.find("\n",j);
    }
    return fileContent.substr(j+1,endOfLine-j-1);
}

string ReadConfigFile::getValueOrDefault(string name, string defaultVal)
{
    int endOfLine;
    int j = fileContent.find(name);
    
    while (j>-1) 
    {
        if (fileContent[j-1]!='#') 
        {
            j = fileContent.find("=",j);
            endOfLine = fileContent.find("\n",j);
            return fileContent.substr(j+1,endOfLine-j-1);
        }
        else 
            j = fileContent.find(name,j+1);
    }
    return defaultVal;
}
