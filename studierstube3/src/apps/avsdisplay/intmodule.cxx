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
//  1-jun-99 08:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "intmodule.h"

#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSwitch.h>

//---------------------------------------------------------------------------
// Constructor
//
// initlializes the IntModule Object.
// initilaizes the Widgets, the Help-Text Geometry,
// and sets the propertys to the value of the parameter string.
// Parameter:
//   the Pointer to the Client, 
//   the Module-Id, 
//   its Parameter string
//
IntModule::IntModule(ClientModule *theClient,int moduleId,char *para)
{
  Debug(printf("new IntModule %s\n",para));
  id=moduleId;
  client=theClient;
  SoSwitch *turnOnOffSwitch;

  memset(name,0,255);
  sscanf(para,"%d %d %d %d %d %c %d %255c",&parentId,&min,&max,&val,&order,&visType,&user,name);
 
  mySlider= new So3DIntSlider;
  mySlider->ref();
  mySlider->setMinMax(min,max);
  mySlider->setCurrentValue(val);
  mySlider->addPositionChangeCallback(IntModule::mySliderCB, (void*)this);

  myDial= new So3DIntDial;
  myDial->ref();
  myDial->setMinMax(min,max);
  myDial->setCurrentValue(val);
  myDial->addDialChangeCallback(IntModule::myDialCB, (void*)this);

  setHelpText(name,8,-16);
  helpShowSlider=new SoSeparator;
  helpShowSlider->ref();
  helpShowDial=new SoSeparator;
  helpShowDial->ref();

  turnOnOffSwitch = new SoSwitch;
  turnOnOffSwitch->whichChild.connectFrom(&mySlider->sliderHL);
  turnOnOffSwitch->addChild(new SoGroup);
  turnOnOffSwitch->addChild(helpGeom);
  turnOnOffSwitch->addChild(new SoGroup);
  helpShowSlider->addChild(turnOnOffSwitch);
  turnOnOffSwitch = new SoSwitch;
  turnOnOffSwitch->whichChild.connectFrom(&mySlider->bodyHL);
  turnOnOffSwitch->addChild(new SoGroup);
  turnOnOffSwitch->addChild(helpGeom);
  turnOnOffSwitch->addChild(new SoGroup);
  helpShowSlider->addChild(turnOnOffSwitch);

  turnOnOffSwitch = new SoSwitch;
  turnOnOffSwitch->whichChild.connectFrom(&myDial->highlighted);
  turnOnOffSwitch->addChild(new SoGroup);
  turnOnOffSwitch->addChild(helpGeom);
  turnOnOffSwitch->addChild(new SoGroup);
  SoTranslation *trans = new SoTranslation; 
  helpShowDial->addChild(turnOnOffSwitch);
  trans->translation.setValue(0.0f,0.0f,+0.02f); 
}


//---------------------------------------------------------------------------
// Destructor
//
// deletes the Widgets and their Help-Text geometry
//
IntModule::~IntModule()
{
  Debug(printf("delete IntModule \n"));
  mySlider->unref();
  myDial->unref();
  helpShowSlider->unref();
  helpShowDial->unref();
}


//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, in this case STB_MODULE_BOOL
//
int IntModule::getType()
{ 
  return STB_MODULE_INT;
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// in this case it is a PIPModule - returns TRUE
//
int IntModule::isPipModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// setUpdate(char *para)
//
// sets the properties to the values of the parameterstring.
// this Method is called from the Client, when an update parameterstring 
// comes from the stbWin....
// calls the client->updatePIP if the update will modify the PIP Geometry...
//
void IntModule::setUpdate(char *para)
{
  int newParentId,newOrder,newUser;
  char newVisType;


  Debug(printf("IntModule gotnewUpdate %s\n",para));  
  memset(name,0,255);

  sscanf(para,"%d %d %d %d %d %c %d %255c",&newParentId,&min,&max,&val,&newOrder,&newVisType,&newUser,name);

  mySlider->setCurrentValue(val);
  mySlider->setMinMax(min,max);
  myDial->setCurrentValue(val);
  myDial->setMinMax(min,max);

  setHelpText(name,8,-16);

  if ((newParentId!=parentId) || (newOrder!=order) || 
      (newVisType!=visType)  || (newUser!=user))
  {
    order=newOrder;
    visType=newVisType;
    parentId=newParentId;
    user=newUser;
    client->updatePip();
  }
}



//---------------------------------------------------------------------------
// update()
//
// sets the current Value of the Module depending of the Visualization Type
// sends an update parameterstring direct to the stbWin Module...
// this Method is called from the static Widget-Callback functions...\
// 
void IntModule::update()
{ 
  char buf[STB_MAXPARAMETERLEN];
  int newVal;
  
  if (visType=='S')
  {
    newVal=mySlider->getCurrentValue();
  }
  if (visType=='D')
  {
    newVal=myDial->getCurrentValue();
  }


  if (val!=newVal)
  {
    val=newVal;
    sprintf(buf,"%d %d %d %d %d %c %d %s",parentId,min,max,val,order,visType,user,name);
    Debug(printf("Int Slider update send %s\n",buf));
    stbNCSend(client->nc,id,STB_UPDATE_MODULE,strlen(buf)+1,(unsigned char *)buf);  
  }
} 


//---------------------------------------------------------------------------
// updatePip(int user,SoSeparator *sep,float &left)
//
// virtual method used for generating the Pip-widgets....
// Parameter:
//   user:        the user for whom the geometry has to be build..
//   SoSeparator: separator where to append the widgets...
//   left:        value where to place the widget.
//
void IntModule::updatePip(int user,SoSeparator *sep,float &left)
{
  SoSeparator   *newSep= new SoSeparator;
  SoTranslation *trans = new SoTranslation;
  SoScale *scale = new SoScale;

  if (isModuleForUser(user))
  {
    if (visType=='S')
    {
      SoSeparator   *SliderSep= new SoSeparator;
      SoRotation    *rot   = new SoRotation;
      SliderSep->addChild(rot);
      SliderSep->addChild(mySlider);
      rot->rotation.setValue(0.0f, 1.0f, 0.0f, 1.0f);
 
      newSep->addChild(trans);
      newSep->addChild(helpShowSlider);
      newSep->addChild(scale);
      newSep->addChild(SliderSep);
      sep->addChild(newSep);

      PipModule::setTransScal(SliderSep,trans,scale,0.07,0.03,0.2,left+0.045,0,0);
      left+=0.09;  
    }

    if (visType=='D')
    {
      newSep->addChild(trans);
      newSep->addChild(helpShowDial);
      newSep->addChild(scale);
      newSep->addChild(myDial);
      sep->addChild(newSep);

      PipModule::setTransScal(myDial,trans,scale,0.15,0.03,0.15,left+0.085,0,0);
    
      left+=0.17;
    }
  }
  
}


//---------------------------------------------------------------------------
// mySliderCB
//
// static Callback function called when the Slider changes its Value.
// this function calls the update Method to send the changed value to 
// the stbWin.
//
void IntModule::mySliderCB(void *userData, So3DIntSlider *slider)
{
  IntModule *theIntModule=(IntModule *)userData;
  
  theIntModule->update();
}


//---------------------------------------------------------------------------
// myDialCB
//
// static Callback function called when the Dial changes its Value.
// this function calls the update Method to send the changed value to 
// the stbWin.
//
void IntModule::myDialCB(void *userData, So3DIntDial *dial)
{
  IntModule *theIntModule=(IntModule *)userData;
  
  theIntModule->update();
}

