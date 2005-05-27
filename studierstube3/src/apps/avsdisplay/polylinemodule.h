// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       polylinemodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    polyline in the Window... 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  4-okt-99 15:00:00  peci      created
// ===========================================================================
#ifndef _POLYLINEMODULE_H_
#define _POLYLINEMODULE_H_

#include "windowmodule.h"
#include <Inventor/fields/SoMFVec3f.h> 

class PolylineModule : public WindowModule
{
public:
  //Constructor
  //needs the Pointer to its Client, its Id, and its Parameter string
  PolylineModule(ClientModule *client,int moduleId,char *para);

  //Destructor
  virtual ~PolylineModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects() {}

  //gets the Module Type, in this case it returns STB_MODULE_POLYLINE
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

  //got geometry packet from client.
  // used to get an update of the line positions, because the number
  // of points is not limited and therefore it is not usefull to get the
  // positions in the parameterstring
  virtual void gotGeomUpdate(int len,char *data);

  //send an update parameterstring direct to the stbWin Module...
  void update(int lineNo);

private:
  SoMFVec3f positions;          //position array of points of the polyline
  float minDrawLineLength;      //length for generating a new point
  SoSeparator *actionLine;      //Line-Geometry Separator 
  SoSeparator *actionDragLine;  //DragPoints Geometry Separator
  int curDragPoint;             //which Point is currently be dragged 
                                //-1 if none...
  int isPressed;                //was the focus set when Pen Button was pressed
  SbColor LineCol;              //line Color

}; //class PolylineModule



#endif//_POLYLINEMODULE_H_
