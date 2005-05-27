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
#define NODEBUG

#include "pipgroupmodule.h"

#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSwitch.h>

//---------------------------------------------------------------------------
// Constructor
//
// initlializes the PipGroupModule Object.
// generates all PipGroup Geoemtries for all users....
// initilaizes the Help-Text Geometry,
// and sets the propertys to the value of the parameter string.
// Parameter:
//   the Pointer to the Client, 
//   the Module-Id, 
//   its Parameter string
//
PipGroupModule::PipGroupModule(ClientModule *theClient,int moduleId,char *para)
{
  SoTranslation *trans;
  SoSeparator *buttons,*sheet;
  SoCube *cCube;
  int i;
  int sel;
  char str[30];
  char groupName[256];
  SoSwitch *turnOnOffSwitch;

  Debug(printf("new PipGroupModule %s\n",para));
  id=moduleId;
  client=theClient;
  memset(name,0,255);
  sscanf(para,"%d %d %d %d %d %d %d %d %d %255c",&parentId,&(buttonId[0]),&(buttonId[1]),&(buttonId[2]),&(buttonId[3]),&(buttonId[4]),&sel,&order,&user,name);

  setHelpText(name,3,-2);

  for(int t=0;t<MAX_USERS;t++)
  {
    sprintf(groupName,"RadioButtons%d_%d",id,t);
    group[t]=new SoSeparator;
    group[t]->ref();

    buttons=new SoSeparator;
    buttons->setName(groupName);
    trans=new SoTranslation;
    trans->translation.setValue(-0.16+0.03, 0.001 ,0.025-0.11);

    buttons->addChild(trans);
    trans=new SoTranslation;
    trans->translation.setValue(0.32/PIPGROUPMODULE_NUMBEROFBUTTONS, 0 ,0);

    //sets the Buttons, with helptext geometry
    for (i=0;i<PIPGROUPMODULE_NUMBEROFBUTTONS;i++)
    {
      if (i>0) buttons->addChild(trans);
      button[t][i]= new So3DRadioButton(groupName,0.32/PIPGROUPMODULE_NUMBEROFBUTTONS, 0.025, 0.025);
      if (i==sel) button[t][i]->setPressed(TRUE);
      sprintf(str,"%d:%d",t,i);
      button[t][i]->nodeId.setValue(str);
      button[t][i]->addReleaseCallback(PipGroupModule::myButtonCB, (void *) this);

      turnOnOffSwitch = new SoSwitch;
      turnOnOffSwitch->whichChild.connectFrom(&button[t][i]->highlighted);
      turnOnOffSwitch->addChild(new SoGroup);
      turnOnOffSwitch->addChild(helpGeom);
      turnOnOffSwitch->addChild(helpGeom);

      buttons->addChild(turnOnOffSwitch);
      buttons->addChild(button[t][i]);

      geom[t][i]=new SoSeparator;
      geom[t][i]->ref();
    }
    sheet=new SoSeparator;
    trans=new SoTranslation;
    trans->translation.setValue(0, 0 ,0.025);
    sheet->addChild(trans); 
    cCube = new SoCube;
    cCube->height = 0.001;
    cCube->width = 0.32;
    cCube->depth = 0.22-0.025;
    sheet->addChild(cCube);  
    trans=new SoTranslation;
    trans->translation.setValue(0.0, 0.0075 ,0.0);
    sheet->addChild(trans);
    sk[t]=new SoSizeShapeKit;
    sk[t]->size.setValue(0.30,0.03,0.20-0.025); //0.30,0.015,0.20-0.025//0.32,0.015,0.22-0.025 //0.32,0.12,0.22
    sk[t]->setPart("geomSeparator",geom[t][sel]);
    sheet->addChild(sk[t]);
    group[t]->addChild(buttons);
    group[t]->addChild(sheet);
  }

}


//---------------------------------------------------------------------------
// Destructor
//
// deletes all groups and all geometries
//
PipGroupModule::~PipGroupModule()
{
  int i,t;
  Debug(printf("delete IntModule \n"));
  

  for (t=0;t<MAX_USERS;t++)
  {
    group[t]->removeAllChildren();
    group[t]->unref();

    for (i=0;i<PIPGROUPMODULE_NUMBEROFBUTTONS;i++)
    {
      geom[t][i]->removeAllChildren();
      geom[t][i]->unref();
    }
  }

}


//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, in this case STB_MODULE_PIPGROUP
//
int PipGroupModule::getType()
{ 
  return STB_MODULE_PIPGROUP;
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// in this case it is a PIPModule - returns TRUE
//
int PipGroupModule::isPipModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// setUpdate(char *para)
//
// sets the properties to the values of the parameterstring.
// this Method is called from the Client, when an update parameterstring 
// comes from the stbWin....
// calls the client->updatePIP 
//
void PipGroupModule::setUpdate(char *para)
{
  int sel;

  memset(name,0,255);
  sscanf(para,"%d %d %d %d %d %d %d %d %d %255c",&parentId,&(buttonId[0]),&(buttonId[1]),&(buttonId[2]),&(buttonId[3]),&(buttonId[4]),&sel,&order,&user,name);
  setHelpText(name,3,-2);
    
  for (int i=0;i<MAX_USERS;i++)
  {
    setGroupGeom(i,sel);
    button[i][sel]->setPressed(TRUE);
  }
  client->updatePip();  
}


//---------------------------------------------------------------------------
// setGroupGeom(int fromUser,int selecedButton)
//
// switches the geometry to the spezified group for the spezified user
// Parameter:
//   fromUser: the Id of the user whose geometry has to be switched
//   selectedButton: the group to which has to be switched
//
void PipGroupModule::setGroupGeom(int fromUser,int selecedButton)
{
  Debug(printf("PIP Group set %d \n",selecedButton));
  sk[fromUser]->setPart("geomSeparator",geom[fromUser][selecedButton]);  
}


//---------------------------------------------------------------------------
// myButtonCB
//
// static Callback function called when a Button was Pressed
// this function calls the update Method to send the changed value to 
// the stbWin.
//
void PipGroupModule::myButtonCB(void *userData, So3DRadioButton *theButton)
{
  int button,fromUser;
  char *buttonNoStr;
  char buttonUsrStr[30];

  if (theButton->isPressed())
  {
    PipGroupModule *thePipGroupModule=(PipGroupModule *)userData;
    
    //Format of the nodeId String-->    user:button
    strcpy(buttonUsrStr,theButton->nodeId.getValue().getString());
    buttonNoStr=strchr(buttonUsrStr,':');
    (*buttonNoStr)=0;
    buttonNoStr++;
    button=atoi(buttonNoStr);
    fromUser=atoi(buttonUsrStr);

    thePipGroupModule->setGroupGeom(fromUser,button);
    thePipGroupModule->update(button);
  }
}


//---------------------------------------------------------------------------
// update(int selecedButton)
//
// sends an update parameterstring with the current selected group 
// direct to the stbWin Module...
// this Method is called from the static Button-Callback functions...
// Parameter:
//    selectButton: the Id of the Button which was currently pressed.
// 
void PipGroupModule::update(int selecedButton)
{ 
  char buf[STB_MAXPARAMETERLEN];
  sprintf(buf,"%d %d %d %d %d %d %d %d %d %s",parentId,buttonId[0],buttonId[1],buttonId[2],buttonId[3],buttonId[4],selecedButton,order,user,name);
  Debug(printf("PipGroup update send %s\n",buf));
  stbNCSend(client->nc,id,STB_UPDATE_MODULE,strlen(buf)+1,(unsigned char *)buf);  
} 


//---------------------------------------------------------------------------
// updatePip(int user,SoSeparator *sep,float &left)
//
// virtual method used for generating the Pip-widgets....
// in this case the method has to generate the pipgeometry for all groups
// like the updatePip Methode of the ClientModule
// Parameter:
//   user:        the user for whom the geometry has to be build..
//   SoSeparator: separator where to append the widgets...
//   left:        value where to place the widget.
//
void PipGroupModule::updatePip(int user,SoSeparator *sep,float &left)
{
  SoSeparator   *newSep;
  SoTranslation *trans;
  float newLeft;

  if (isModuleForUser(user))
  {
    for (int i=0;i<PIPGROUPMODULE_NUMBEROFBUTTONS;i++)
    {
      newLeft=0.0;
      PipModule *module;


      geom[user][i]->removeAllChildren();
      trans = new SoTranslation;
      geom[user][i]->addChild(trans);
    
      module=client->getFirstPipModule();

      while (module!=NULL)
      {
        if (module->parentId==buttonId[i])
        {
          module->updatePip(user,geom[user][i],newLeft);
        }
        module=module->nextPipModule;
      }  

      trans->translation.setValue(-(newLeft/2.0f),0.0f,0.0f);
    }
    newSep= new SoSeparator;
    trans = new SoTranslation;
    trans->translation.setValue(left+0.16+0.01,0.0f,0.0f);
    newSep->addChild(trans);
    newSep->addChild(group[user]);
    sep->addChild(newSep);
    left+=0.32+0.02;
    sk[user]->size.setValue(0.30,0.03,0.20-0.025);
  }
}
