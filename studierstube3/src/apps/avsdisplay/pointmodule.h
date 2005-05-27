// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       pointmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Logical True-False Input Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  19-jul-99 17:00:00  peci      created
// ===========================================================================
#ifndef _POINTMODULE_H_
#define _POINTMODULE_H_

#include "windowmodule.h"

class PointModule : public WindowModule
{
public:
  //Constructor
  //needs the Pointer to its Client, its Id, and its Parameter string
  PointModule(ClientModule *client,int moduleId,char *para);

  //Destructor
  virtual ~PointModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {}

  //gets the Module Type, in this case it returns STB_MODULE_POINT
  virtual int getType();

  //sets the properties to the values of the parameterstring.
  virtual void setUpdate(char *data);

  //is this Module a PipModule? in this case it returns always TRUE.
  virtual int isPipModule();

  //is this Module a WindowModule? in this case it returns always TRUE.
  virtual int isWindowModule();

  //event in the Window occured, called by the client...
  virtual void eventInWindow(So3DEvent *event,int userID, SbVec3f *Pos);

  //shows interaction Elements like the actionLine and the actionDragLine
  virtual void showInteractionElements();

  //hides interaction Elements like the actionLine and the actionDragLine
  virtual void hideInteractionElements();

  //send an update parameterstring direct to the stbWin Module...
  void update();

private:
  SbVec3f position;             //position of the Point
  SoSeparator *actionPoint;     //point geometry
  SoSeparator *actionDragPoint; //dragpoint geometry
  int isPressedFocus;           //was the focus set when Pen Button was pressed
  int isPressed;                //is the Pen Button currently pressed?

}; //class PointModule



#endif//_PointModule_H_
