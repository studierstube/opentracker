// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       client
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Geometry Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================
#ifndef _GEOMMODULE_H_
#define _GEOMMODULE_H_

#ifndef DEBUG
#ifndef NODEBUG
#define NODEGUB
#endif
#endif

#include "avsdisplay.h"
#include "module.h"
#include "clientmodule.h"
#include "stbcomm.h"

#include "Bin2IV.h"

#include <stbapi/event3d/Base3D.h>
#include <stbapi/interaction/So3DSeparator.h>
#include <stbapi/context/SoContextSwitch.h>
#include <stbapi/util/zpthread.h>

struct decodeElement
{
  char *data;
  struct decodeElement *next;
};


class GeometryModule : public Module
{
public:
  //Constructor
  //needs the Pointer to its Client, its Id, and its Parameter string
  GeometryModule(ClientModule *theClient,int moduleId,char *para);

  //Destructor
  virtual ~GeometryModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects();

  //gets the Module Type, in this case it returns STB_MODULE_GEOM
  virtual int getType();

  //sets the properties to the values of the parameterstring.
  virtual void setUpdate(char *data);

  //is this Module a PipModule? in this case it returns always FALSE.
  virtual int isPipModule();

  //is this Module a WindowModule? in this case it returns always FALSE.
  virtual int isWindowModule();

  //got a Geometry update Packet from the Client...
  virtual void gotGeomUpdate(int len,char *data);

  void geomUpdateFinish();

  //saves the Geometry to the file....
  static void saveGeometry(SoNode *node,char *fileName);


private:
  So3DSeparator *geom;      //Separator to the Geometry
   
  CBuildTree myCBuildTree;  //CBuildTree Object used for decoding the packets

  ClientModule *client;     //pointer to the client Object
  SoSeparator *newGeom;     //Separator of the current decoding Geometry
  int geomReceiving;        //is the GeomModule currently receiving Geometry?
  int user;                 //for which user the Module is enabled.
  int clr;                  //wheter the old Geometry has to be removed or not

  SoContextSwitch *userSwitch;  //userContextSwitch to display Geoemtry only 
                                //for the spezified user

  static void geomDecodeThread(void *data);
  long geomDecodeThreadPid;
  Semaphore sem;
  struct decodeElement *currentDecodeElement,*lastDecodeElement;
  int accessAvailable;

}; //class GeometryModule



#endif//_GEOMMODULE_H_
