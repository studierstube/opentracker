// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       linemodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    line in the Window... 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  4-okt-99 15:00:00  peci      created
// ===========================================================================
#ifndef _LINEMODULE_H_
#define _LINEMODULE_H_

#include "windowmodule.h"

class LineModule : public WindowModule
{
public:
  //Constructor
  //needs the Pointer to its Client, its Id, and its Parameter string
  LineModule(ClientModule *client,int moduleId,char *para);

  //Destructor
  virtual ~LineModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {}

  //gets the Module Type, in this case it returns STB_MODULE_LINE
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
  SbVec3f position[2];          //position array (0..startpopint,1..endpoint)
  SoSeparator *actionLine;      //Line-Geometry Separator 
  SoSeparator *actionDragLine;  //DragPoints Geometry Separator
  int curDragPoint;             //which Point is currently be dragged 
                                //-1 if none...
  int isPressedFocus;           //was the focus set when Pen Button was pressed
  int isPressed;                //is the Pen Button currently pressed?

}; //class LineModule



#endif//_LINEMODULE_H_
