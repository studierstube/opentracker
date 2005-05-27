// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       pipgroupmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    PIP-Group Input Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  15-jun-99 08:00:00  peci      created
// ===========================================================================
#ifndef _PIPGROUPMODULE_H_
#define _PIPGROUPMODULE_H_

#include "pipmodule.h"

#include <stbapi/window3d/SoSizeShapeKit.h> 
#include <Inventor/nodes/SoSeparator.h>
#include <stbapi/widgets/So3DRadioButton.h>

#define PIPGROUPMODULE_NUMBEROFBUTTONS 5

class PipGroupModule : public PipModule
{
public:
  //Constructor
  //needs the Pointer to its Client, its Id, and its Parameter string
  PipGroupModule(ClientModule *client,int moduleId,char *para);

  //Destructor
  virtual ~PipGroupModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {}

  //gets the Module Type, in this case it returns STB_MODULE_PIPGROUP
  virtual int getType();

  //sets the properties to the values of the parameterstring.
  virtual void setUpdate(char *data);

  //is this Module a PipModule? in this case it returns always TRUE.
  virtual int isPipModule();

  //virtual method used for generating the Pip-widgets....
  virtual void updatePip(int user,SoSeparator *sep,float &left);

  //send an update parameterstring direct to the stbWin Module...
  void update(int button);

  //switches the geometry to the spezified group for the spezified user
  void setGroupGeom(int fromUser,int button);

private:
  //SoSizeShapeKit for all users to fit all widgets onto the pip group
  SoSizeShapeKit *sk[MAX_USERS];  

  //geometry of the pipGroup, with the buttons, and the frame.......
  SoSeparator *group[MAX_USERS];  
                                  
  //pip geometry for every user and every group....
  SoSeparator *geom[MAX_USERS][PIPGROUPMODULE_NUMBEROFBUTTONS];

  //the Buttons for every user for every group
  So3DRadioButton *button[MAX_USERS][PIPGROUPMODULE_NUMBEROFBUTTONS];

  //the PipGroup IDs for every group
  //this id is spezified with the parendId in every PipModule
  int buttonId[PIPGROUPMODULE_NUMBEROFBUTTONS];

  //static callback function of all Buttons.
  //the buttons are distinguished by their names...
  static void myButtonCB(void *userData, So3DRadioButton *button);

}; //class PipGroupModule


#endif//_PIPGROUPMODULE_H_
