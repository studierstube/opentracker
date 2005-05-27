// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       windowmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    WindowModule abstract Class and common methods 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  7-jun-99 08:00:00  peci      created
// ===========================================================================


#ifndef _WINDOWMODULE_H_
#define _WINDOWMODULE_H_

class WindowModule;
#include "pipmodule.h"

#include <stbapi/widgets/So3DCheckBox.h>
#include <Inventor/nodes/SoTexture2.h>


class WindowModule : public PipModule
{
public:
  //Constructor - initializes activateButton,nextWinModule and prevWinModule
  WindowModule();

  //Destructor
  virtual ~WindowModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {}

  //sets the helpText and the activateButton
  void initWindowModule(char *name);

  //gets the Module Type,in this case it does nothing ->its an abstract class
  virtual int getType()=0;

  //sets the properties to the values of the parameterstring.
  //not implemented for the abstract class
  virtual void setUpdate(char *data);

  //is this Module a PipModule? for all WindowModule it should TRUE.
  virtual int isPipModule();

  //is this Module a WindowModule? for all WindowModule it should TRUE.
  virtual int isWindowModule();

  //event in the Window occured, called by the client to every windowModule
  virtual void eventInWindow(So3DEvent *event,int userID, SbVec3f *Pos);

  //shows interaction Elements for the specific windowModule
  virtual void showInteractionElements() {}

  //hides interaction Elements for the specific windowModule
  virtual void hideInteractionElements() {}

  //returns an integer coded state of the Button pressed in the event
  static int getButtonState(So3DEvent *event);

  //Callbacks for the activateButton to enable/disable the windowInteraction
  static void myButtonCB(void *userData, So3DCheckBox *button);

  //generates a tansparent DragPoint Cube
  static SoSeparator* generateDragPoint(const SbVec3f *pos);

  //sets the DragPoint position
  static void setDragPointPosition(SoSeparator *DragPoint,const SbVec3f *pos);

  //sets the DragPoint color
  static void setDragPointColor(SoSeparator *DragPoint,const SbColor col);

  //virtual method used for generating the activate ButtonGeometry on the PIP
  virtual void updatePip(int user,SoSeparator *sep,float &left);

  //gets the First windowModule in the list
  static WindowModule* getFirstWinModule(WindowModule *winModule);


  WindowModule *prevWinModule;       //pointer to the previous windowModule
  WindowModule *nextWinModule;       //pointer to the next windowModule

  So3DCheckBox *myButton[MAX_USERS]; //activate Button for every user
  SoSwitch  *helpShow[MAX_USERS];    //helpGeometry for the activateButton

  int actionButton;                  //on which buttons should the window react

  SoTexture2 *buttonTexture;         //Texture for the activate Buttons
private:
}; //class WindowModule



#endif//_WINDOWMODULE_H_
