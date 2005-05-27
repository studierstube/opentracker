// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       intmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Integer-Slider Input Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  17-jul-99 08:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "windowmodule.h"

#include <Inventor/nodes/SoSwitch.h>


//---------------------------------------------------------------------------
// Constructor
//
// initializes the activateButtons for every user and
// the nextWinModule and prevWinModule pointers with NULL
//
WindowModule::WindowModule()
{
  for (int i=0;i<MAX_USERS;i++)
  {
    myButton[i]=NULL;
    helpShow[i]=NULL;
  }
  nextWinModule=NULL;
  prevWinModule=NULL;
  buttonTexture=NULL;
  actionButton=1;
}


//---------------------------------------------------------------------------
// Destructor
//
// deletes the activate Buttons and the help Geometry 
//
WindowModule::~WindowModule()
{
  if (prevWinModule!=NULL)
  {
    prevWinModule->nextWinModule=this->nextWinModule;
  }
  if (nextWinModule!=NULL)
  {
    nextWinModule->prevWinModule=this->prevWinModule;
  }

  nextWinModule=NULL;
  prevWinModule=NULL;

  for (int i=0;i<MAX_USERS;i++)
  {
    if (myButton[i]!=NULL)  myButton[i]->unref();
    if (helpShow[i]!=NULL)  helpShow[i]->unref();
  }
  buttonTexture->unref();
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// every WindowModule has an activateButton, so every WindowModule is also
// a PipModule, therefore this method should always return TRUE
//
int WindowModule::isPipModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// isWindowModule()
//
// returns wheter the Module is a WindowModule or not.
// for all WindowModules it should return TRUE
//
int WindowModule::isWindowModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// setUpdate(char *data)
//
// in this case it does nothing because its an abstract class
//
void WindowModule::setUpdate(char *data)
{
}


//---------------------------------------------------------------------------
// eventInWindow(So3DEvent *event,int userID, SbVec3f *Pos)
//
// in this case it does nothing because its an abstract class
//
void WindowModule::eventInWindow(So3DEvent *event,int userID, SbVec3f *Pos)
{
}


//---------------------------------------------------------------------------
// static myButtonCB
//
// Callbacks for the activateButton to enable/disable the windowInteraction
//
void WindowModule::myButtonCB(void *userData, So3DCheckBox *button)
{
  WindowModule *theWindowModule= (WindowModule*) userData;
  if (button->isPressed())
  {
    button->buttonColor.setValue(1.0, 0.3, 0.0);
    theWindowModule->showInteractionElements();
  }
  else
  {
    button->buttonColor.setValue(0.7, 0.65, 0.7);    
    theWindowModule->hideInteractionElements();
  }
}


//---------------------------------------------------------------------------
// static getButtonState(So3DEvent *event)
//
// returns an integer coded state of the Button pressed in the event
//
int WindowModule::getButtonState(So3DEvent *event)
{
  int ret=0;
  if (event->getButton(So3DEvent::BUTTON0)) ret|=1;
  if (event->getButton(So3DEvent::BUTTON1)) ret|=2;
  if (event->getButton(So3DEvent::BUTTON2)) ret|=4;
  if (event->getButton(So3DEvent::BUTTON3)) ret|=8;
  if (event->getButton(So3DEvent::BUTTON4)) ret|=16;
  if (event->getButton(So3DEvent::BUTTON5)) ret|=32;
  if (event->getButton(So3DEvent::BUTTON6)) ret|=64;
  if (event->getButton(So3DEvent::BUTTON7)) ret|=128;
  return ret;
}


//---------------------------------------------------------------------------
// static getFirstWinModule(WindowModule* winModule)
//
// gets the first WindowModule in the winModule List.
//
WindowModule* WindowModule::getFirstWinModule(WindowModule* winModule)
{
  if (winModule==NULL) return NULL;
  
  while (winModule->prevWinModule!=NULL) 
    winModule=winModule->prevWinModule;

  return winModule;
}


//---------------------------------------------------------------------------
// initWindowModule(char *name)
//
// sets the helpText and the activateButton
// called by all constructors of all windowModules
//
void WindowModule::initWindowModule(char *name)
{
  setHelpText(name,4,-4.5);

  buttonTexture=new SoTexture2;
  buttonTexture->ref();

  for (int i=0;i<MAX_USERS;i++)
  {
    myButton[i]= new So3DCheckBox(0.04,0.005,0.02,buttonTexture);
    myButton[i]->ref();
    myButton[i]->addPressCallback(WindowModule::myButtonCB, (void*)this);
//    myButton[i]->setTexture(buttonTexture);

    helpShow[i]= new SoSwitch;
    helpShow[i]->ref();

    helpShow[i]->whichChild.connectFrom(&myButton[i]->highlighted);
    helpShow[i]->addChild(new SoGroup);
    helpShow[i]->addChild(helpGeom);
    helpShow[i]->addChild(new SoGroup);
  }
  actionButton=1;
}


//---------------------------------------------------------------------------
// updatePip(int user,SoSeparator *sep,float &left)
//
// virtual method used for generating the activateButton on the Pip
// Parameter:
//   user:        the user for whom the geometry has to be build..
//   SoSeparator: separator where to append the widgets...
//   left:        value where to place the widget.
//
void WindowModule::updatePip(int theUser,SoSeparator *sep,float &left)
{
  SoSeparator   *newSep= new SoSeparator;
  SoTranslation *trans = new SoTranslation;
  SoScale *scale = new SoScale;

  if (isModuleForUser(theUser))
  {
    newSep->addChild(trans);
    newSep->addChild(helpShow[theUser]);
    newSep->addChild(scale);
    newSep->addChild(myButton[theUser]);
    sep->addChild(newSep);

    PipModule::setTransScal(myButton[theUser],trans,scale,0.07,0.03,0.06,left+0.045,0,0);
    
    left+=0.09;
  }
}


//---------------------------------------------------------------------------
// static generateDragPoint(const SbVec3f *pos)
//
// generates a tansparent DragPoint Cube at the given Position
//
SoSeparator* WindowModule::generateDragPoint(const SbVec3f *pos)
{
  SoScale *scale=new SoScale();
  SoMaterial *mat=new SoMaterial();
  SoTranslation *trans = new SoTranslation;
  SoCube  *cube=new SoCube();
  SoSeparator *actionDragPoint=new SoSeparator;

  actionDragPoint->addChild(mat);
  actionDragPoint->addChild(trans);
  actionDragPoint->addChild(scale);
  actionDragPoint->addChild(cube);
  cube->width.setValue(0.0025);
  cube->height.setValue(0.0025);
  cube->depth.setValue(0.0025);
  cube->setName("Cube_as_Point");
  scale->scaleFactor.setValue(16,16,16);
  mat->transparency.setValue(0.8);
  mat->ambientColor.setValue(0.3,0.3,0.3);
  mat->diffuseColor.setValue(0.3,0.3,0.3);
  trans->translation=*pos;

  return actionDragPoint;
}


//---------------------------------------------------------------------------
// static setDragPointPosition(SoSeparator *DragPoint,const SbVec3f *pos)
//
// sets the DragPoint position
// Parameter:
//   DragPoint: Separator of the DragPoint, created with generateDragPoint
//   pos:       new Position of the DragPoint
//
void WindowModule::setDragPointPosition(SoSeparator *DragPoint,const SbVec3f *pos)
{
  SoTranslation *trans = (SoTranslation *)(DragPoint->getChild(1));
  trans->translation=*pos;
}


//---------------------------------------------------------------------------
// static setDragPointColor(SoSeparator *DragPoint,const SbColor col)
//
// sets the DragPoint color
// Parameter:
//   DragPoint: Separator of the DragPoint, created with generateDragPoint
//   col:       new color of the DragPoint
//
void WindowModule::setDragPointColor(SoSeparator *DragPoint,const SbColor col)
{
  SoMaterial *mat=(SoMaterial *)(DragPoint->getChild(0));
  mat->ambientColor=col;
  mat->diffuseColor=col;
}


