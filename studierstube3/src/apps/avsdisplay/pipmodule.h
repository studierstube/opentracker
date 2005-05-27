// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       pipmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    pipModule abstract Class and common methods
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  7-jun-99 08:00:00  peci      created
// ===========================================================================
#ifndef _PIPMODULE_H_
#define _PIPMODULE_H_


#include "module.h"
#include "stbcomm.h"
class PipModule;
class ClientModule;

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>


class PipModule : public Module
{
public:
  //Constructor - initializes  nextPipModule,prevPipModule,helpGeom
  PipModule();

  //Destructor
  virtual ~PipModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {}

  //gets the Module Type,in this case it -1 because its an abstract class
  virtual int getType();

  //sets the properties to the values of the parameterstring.
  //not implemented for the abstract class
  virtual void setUpdate(char *data);

  //virtual method used for generating the Pip-widgets....
  //not implemented for the abstract class
  virtual void updatePip(int user,SoSeparator *sep,float &left);

  //is this Module a PipModule? for all PipModule it should TRUE.
  virtual int isPipModule();

  //is this Module a WindowModule? for all Pip only Modules it should FALSE.
  virtual int isWindowModule() {return FALSE;}

  //method to set/update the help geometry with the given text on the position
  void PipModule::setHelpText(char *text,float posy,float posz);

  //TRUE if the order of pipModule1 is less than the order of pipModule2
  static int isLess(PipModule *pipModule1,PipModule *pipModule2);

  //gets the first PipModule in the list
  static PipModule* getFirstPipModule(PipModule *pipModule);

  //orders all PipModule, but the list has to be initialized before calling...
  static void orderPipModules(PipModule *pipModule);

  //sets the translation and scaling of the node to the spezified size....
  static void setTransScal(SoNode *node,SoTranslation *trans,SoScale *scal,float sx,float sy,float sz,float px,float py,float pz);

  //method to compute the boundingbox of a node, used by setTransScal
  static void getBBox(SoNode *geom,float &minx,float &miny,float &minz,float &maxx,float &maxy,float &maxz);

  //returns TRUE if the Module is enabled for the user theUser.  
  int isModuleForUser(int theUser);

  ClientModule *client;      //pointer to the client
  int parentId;              //Id of the pipgroup, or from the client if root.
  int order;                 //order-value, used to order all pipModules
  int user;                  //for which user the Module is enabled.
  char name[256];            //the help Text

  PipModule *nextPipModule;  //pointer to the next PipModule
  PipModule *prevPipModule;  //pointer to the previous PipModule

  SoSeparator *helpGeom;     //helptext geoemtry

}; //class PipModule

#include "clientmodule.h"

#endif//_PIPMODULE_H_
