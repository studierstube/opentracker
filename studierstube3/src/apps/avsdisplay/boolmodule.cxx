// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       boolmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Logical True-False Input Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  11-jul-99 17:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "boolmodule.h"

#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSwitch.h>


//---------------------------------------------------------------------------
// Constructor
//
// initlializes the BoolModule Object.
// initilaizes the Widgets, the Help-Text Geometry,
// and sets the propertys to the value of the parameter string.
// Parameter:
//   the Pointer to the Client, 
//   the Module-Id, 
//   its Parameter string
//
BoolModule::BoolModule(ClientModule *theClient,int moduleId,char *para)
{
  Debug(printf("new BoolModule %s\n",para));
  id=moduleId;
  client=theClient;
  SoSwitch *turnOnOffSwitch;


  memset(name,0,255);
  sscanf(para,"%d %d %d %c %d %255c",&parentId,&val,&order,&visType,&user,name);
 
  mySlider= new So3DIntSlider;
  mySlider->ref();
  mySlider->setMinMax(0,1);
  mySlider->setCurrentValue(val);
  mySlider->addPositionChangeCallback(BoolModule::mySliderCB, (void*)this);


  myDial= new So3DIntDial;
  myDial->ref();
  myDial->setMinMax(0,1);
  myDial->setCurrentValue(val);
  myDial->addDialChangeCallback(BoolModule::myDialCB, (void*)this);

  myCheckBox= new So3DCheckBox(0.07,0.06,0.07);
  myCheckBox->ref();
  myCheckBox->setPressed(val);
  myCheckBox->addPressCallback(BoolModule::myCheckBoxCB, (void*)this);

  setHelpText(name,8,-16);
  helpShowSlider=new SoSeparator;
  helpShowSlider->ref();
  helpShowDial=new SoSeparator;
  helpShowDial->ref();
  helpShowCheckBox=new SoSeparator;
  helpShowCheckBox->ref();

  SoTranslation *trans;
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
  trans = new SoTranslation; 
  helpShowDial->addChild(trans);
  helpShowDial->addChild(turnOnOffSwitch);
  trans->translation.setValue(0.0f,0.0f,+0.02f); 


  turnOnOffSwitch = new SoSwitch;
  turnOnOffSwitch->whichChild.connectFrom(&myCheckBox->highlighted);
  turnOnOffSwitch->addChild(new SoGroup);
  turnOnOffSwitch->addChild(helpGeom);
  turnOnOffSwitch->addChild(new SoGroup);
  trans = new SoTranslation; 
  helpShowCheckBox->addChild(trans);
  helpShowCheckBox->addChild(turnOnOffSwitch);
  trans->translation.setValue(0.0f,0.0f,+0.07f); 
}

//---------------------------------------------------------------------------
// Destructor
//
// deletes the Widgets and their Help-Text geometry
//
BoolModule::~BoolModule()
{
  Debug(printf("delete BoolModule \n"));
  mySlider->unref();
  myDial->unref();
  myCheckBox->unref();
  helpShowSlider->unref();
  helpShowDial->unref();
  helpShowCheckBox->unref();
}

//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, in this case STB_MODULE_BOOL
//
int BoolModule::getType()
{ 
  return STB_MODULE_BOOL;
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// in this case it is a PIPModule - returns TRUE
//
int BoolModule::isPipModule() 
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
void BoolModule::setUpdate(char *para)
{
  int newParentId,newOrder;
  char newVisType;


  Debug(printf("BoolModule gotnewUpdate %s\n",para));  
  memset(name,0,255);
  sscanf(para,"%d %d %d %c %d %255c",&newParentId,&val,&newOrder,&newVisType,&user,name);

  mySlider->setCurrentValue(val);
  myDial->setCurrentValue(val);
  myCheckBox->setPressed(val);

  setHelpText(name,8,-16);
  if ((newParentId!=parentId) || (newOrder!=order) || (newVisType!=visType))
  {
    order=newOrder;
    visType=newVisType;
    parentId=newParentId;
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
void BoolModule::update()
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
  if (visType=='B')
  {
    newVal=myCheckBox->isPressed();
  }


  if (val!=newVal)
  {
    val=newVal;
    sprintf(buf,"%d %d %d %c %d %s",parentId,val,order,visType,user,name);
    Debug(printf("bool update send %s\n",buf));
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
void BoolModule::updatePip(int user,SoSeparator *sep,float &left)
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

    if (visType=='B')
    {
      newSep->addChild(trans);
      newSep->addChild(helpShowCheckBox);
      newSep->addChild(scale);
      newSep->addChild(myCheckBox);
      sep->addChild(newSep);
 
      PipModule::setTransScal(myCheckBox,trans,scale,0.07,0.03,0.06,left+0.045,0,0);
    
      left+=0.09;
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
void BoolModule::mySliderCB(void *userData, So3DIntSlider *slider)
{
  BoolModule *theBoolModule=(BoolModule *)userData;
  
  theBoolModule->update();
}

//---------------------------------------------------------------------------
// myDialCB
//
// static Callback function called when the Dial changes its Value.
// this function calls the update Method to send the changed value to 
// the stbWin.
//
void BoolModule::myDialCB(void *userData, So3DIntDial *dial)
{
  BoolModule *theBoolModule=(BoolModule *)userData;
  
  theBoolModule->update();
}

//---------------------------------------------------------------------------
// myCheckBoxCB
//
// static Callback function called when the Button changes its Value.
// this function calls the update Method to send the changed value to 
// the stbWin.
//
void BoolModule::myCheckBoxCB(void *userData, So3DCheckBox *checkBox)
{
  BoolModule *theBoolModule=(BoolModule *)userData;
  
  theBoolModule->update();
}


