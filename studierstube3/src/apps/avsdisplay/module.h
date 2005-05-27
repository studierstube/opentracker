// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Module
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Module Object 
//              Superclass of all Module Objects...
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================

#ifndef _MODULE_H_
#define _MODULE_H_

class Module
{
public:
  //Constructor..
  Module() {};

  //Destructor..
  virtual ~Module() {};

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {};

  //gets the ModuleType..
  virtual int getType() = 0; 

  //sends an update Parameterstirng to the Module
  virtual void setUpdate(char *data) = 0; 

  //is the Module a PIPModule?
  virtual int isPipModule() {return 0;}

  //is the Module a WindowModule?
  virtual int isWindowModule() {return 0;}

  //send the Geometry update Packet to the Module...
  virtual void gotGeomUpdate(int len,char *data) {}

  //--public member data
  int id;  //the unique Module Id.

}; //class Module



#endif//_MODULE_H_
