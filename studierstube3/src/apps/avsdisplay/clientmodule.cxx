// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       client
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Client Object
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "clientmodule.h"

#include "geommodule.h"
#include "intmodule.h"
#include "boolmodule.h"
#include "floatmodule.h"
#include "pipgroupmodule.h"
#include "pointmodule.h"
#include "linemodule.h"
#include "polylinemodule.h"

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoTranslation.h>

//---------------------------------------------------------------------------
// Constructor
//
// initlializes the ClientModule Object.
// initializes and generates the 3D Window
// initializes the Modulemanager which stores the Modules from the Client
// initlializes the PIP Geometry separator for each user
// initlializes and adds the clientVolumne and the Winscale to the 3D Window
// initlializes the focus for each user...
// Parameter:
//   Pointer to the StbCommunicator,
//   Pointer to the ClientManager
//   its Id,
//   the Id of the corresponding stbWin Module
//   the stbNetCom struct for the network-communication,
//   its Parameter string
//
ClientModule::ClientModule(StbCommunicator *theComMan,ModuleManager *theClientMan,int ModuleId,int newDestModId,stbNetCom *newNetCom, char *para)
{
  float geomRatioX,geomRatioY,geomRatioZ;
  float geomMoveX,geomMoveY,geomMoveZ;
  int fitGeometry;  //fit Geometry has no effect in the constructor....
  int i;

  sscanf(para,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d",&posX,&posY,&posZ,&sizeX,&sizeY,&sizeZ,&geomSizeX,&geomSizeY,&geomSizeZ,&geomMoveX,&geomMoveY,&geomMoveZ,&rotX,&rotY,&rotZ,&rotW,&fitGeometry,&user);

  id=ModuleId;
  nc=newNetCom;
  destModId=newDestModId;
  clientMan=theClientMan;
  cMan=theComMan;
  Modules = new ModuleManager;

  Debug(printf("Client construktor1 \n%s\n%f  %f %f %f %f %f %f %f %f %f %f %f %f\n",para,posX,posY,posZ,sizeX,sizeY,sizeZ,geomSizeX,geomSizeY,geomSizeZ,rotX,rotY,rotZ,rotW));

  for (i=0;i<MAX_USERS;i++)
  {
    PIPGeometry[i]=new SoSeparator;
    PIPGeometry[i]->ref();
    sk[i]=new SoSizeShapeKit;
    sk[i]->ref();
    sk[i]->size.setValue(0.32,0.015,0.20);
    sk[i]->setPart("geomSeparator",PIPGeometry[i]);
  }

  clientVolume = new SoSeparator;
  clientVolume->ref();
  winScale=new SoScale;
  winScale->ref();

  winTrans=new SoTranslation;
  winTrans->ref();
  winTrans->translation.setValue(geomMoveX,geomMoveY,geomMoveZ);

  if (geomSizeX==0.0) geomRatioX=sizeX; else geomRatioX=sizeX/geomSizeX;
  if (geomSizeY==0.0) geomRatioY=sizeY; else geomRatioY=sizeY/geomSizeY;
  if (geomSizeZ==0.0) geomRatioZ=sizeZ; else geomRatioZ=sizeZ/geomSizeZ;

  winScale->scaleFactor.setValue(geomRatioX,geomRatioY,geomRatioZ);

}

//---------------------------------------------------------------------------
// initOIVObjects
//
// does the initialization of the OIV Objects within the rendering loop.
// necessary for some OIV Objects and StudierstubeOIV Objects like the WindowKit
//
void ClientModule::initOIVObjects()
{
  WindowCreate wc;
  SoSeparator *windowVolume;
  int i;

  wc.position.setValue(posX, posY, posZ);
  wc.orientation.setValue(rotX, rotY, rotZ, rotW);
  wc.size.setValue(sizeX,sizeY,sizeZ);
  wc.style = 6+24+32;
  wc.state = 0;
  wc.title = SbString("AVS Interface");
  wc.eventMask = Base3D::BM_ALL_EVENTS;
  wc.clientEventMask = Base3D::BM_ALL_EVENTS;

  window = cMan->createWindow(NULL, &wc, ClientModule::MyWindowCB, (void*)this);
  window->setBorderThickness(0.02);
  window->setIconSize(SbVec3f(0.08, 0.08, 0.08));

  windowVolume =window->getClientVolumeSeparator();
  windowVolume->addChild(winScale);
  windowVolume->addChild(winTrans);
  windowVolume->addChild(clientVolume);

  for (i=0;i<MAX_USERS;i++)  //set Window - User visibility
  {
    window->setVisible(i, (((user>>i) & 1)==1) );
    userFocus[i]=FALSE;
  }

}


//---------------------------------------------------------------------------
// Destructor
//
// deletes the PIP Geopmetry for each user,
// removes the 3D Window
// deletes all Modules and the ModuleManager...
//
ClientModule::~ClientModule()
{
  Debug(printf("Removing Window\n"));
  window->closeWindow();

  clientVolume->unref();
  winScale->unref();
  winTrans->unref();

  for (int i=0;i<MAX_USERS;i++)
  {
    PIPGeometry[i]->removeAllChildren();
    PIPGeometry[i]->unref();
    sk[i]->unref();
  }
  delete Modules;
}


//---------------------------------------------------------------------------
// remove(int sendQuit)
//
// calls the destructor.
//Parameter:
//  if sendQuit is TRUE then it sends an STB_SERVERQUIT packet to the stbWin
//  before calling the destructor...
//
void ClientModule::remove(int sendQuit)
{
  if (sendQuit)
  {
    stbNCSend(nc,destModId,STB_SERVERQUIT,0,NULL);
  }
  delete ((ClientModule *)clientMan->removeModule(id));
}


//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, in this case STB_MODULE_WIN
//
int ClientModule::getType()
{
  return STB_MODULE_WIN;
}



//---------------------------------------------------------------------------
// update()
//
// sets the winscale to the values of the GeomSize and
// sends an update parameterstring direct to the stbWin Module...
//
void ClientModule::update()
{
  char buf[STB_MAXPARAMETERLEN];
  float posX,posY,posZ,sizeX,sizeY,sizeZ,rotX,rotY,rotZ,rotW;
  float geomRatioX,geomRatioY,geomRatioZ;
  SbVec3f geomMove;

  window->getPosition().getValue(posX,posY,posZ);
  window->getRotation().getValue(rotX,rotY,rotZ,rotW);
  window->getSize().getValue(sizeX,sizeY,sizeZ);

  if (geomSizeX==0.0) geomRatioX=sizeX; else geomRatioX=sizeX/geomSizeX;
  if (geomSizeY==0.0) geomRatioY=sizeY; else geomRatioY=sizeY/geomSizeY;
  if (geomSizeZ==0.0) geomRatioZ=sizeZ; else geomRatioZ=sizeZ/geomSizeZ;
  winScale->scaleFactor.setValue(geomRatioX,geomRatioY,geomRatioZ);

  geomMove=winTrans->translation.getValue();

  sprintf(buf,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f 0 %d",posX,posY,posZ,sizeX,sizeY,sizeZ,geomSizeX,geomSizeY,geomSizeZ,geomMove[0],geomMove[1],geomMove[2],rotX,rotY,rotZ,rotW,user);
  Debug(printf("Client Update send %s\n",buf));
  stbNCSend(nc,destModId,STB_UPDATE_MODULE,strlen(buf)+1,(unsigned char *)buf);

  for (int i=0;i<MAX_USERS;i++)     //set Window - User visibility
    window->setVisible(i, (((user>>i) & 1)==1) );

}


//---------------------------------------------------------------------------
// setClientWindowPIP(int userID))
//
// sets current displayed PIPGeometry for the user....
//
void ClientModule::setClientWindowPIP(int userID)
{
  cMan->setPipGeometry(userID, sk[userID], TRUE);
}


//---------------------------------------------------------------------------
// setUpdate(char *para)
//
// sets the properties to the values of the parameterstring.
// and corrects the Point and line cube-sizes...
// Parameter:
//   the parameterstring data.
//
void ClientModule::setUpdate(char *data)
{
  float posX,posY,posZ,sizeX,sizeY,sizeZ,rotX,rotY,rotZ,rotW;
  float geomRatioX,geomRatioY,geomRatioZ;
  float geomMoveX,geomMoveY,geomMoveZ;
  int fitGeometry;

  sscanf(data,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d",&posX,&posY,&posZ,&sizeX,&sizeY,&sizeZ,&geomSizeX,&geomSizeY,&geomSizeZ,&geomMoveX,&geomMoveY,&geomMoveZ,&rotX,&rotY,&rotZ,&rotW,&fitGeometry,&user);

  //should i fit the geomSize and geomMove to fit the Geometry into the Window?
  if (fitGeometry)
  {
    //if so, then forget the values of the update and calculate the new values
    float minx,miny,minz,maxx,maxy,maxz;

    PipModule::getBBox(clientVolume,minx,miny,minz,maxx,maxy,maxz);
    geomMoveX=-(minx+((maxx-minx)/2.0f)); //move the geometry
    geomMoveY=-(miny+((maxy-miny)/2.0f)); //that the middle of the geometry
    geomMoveZ=-(minz+((maxz-minz)/2.0f)); //is in the origin of the window

    geomSizeX= (maxx-minx)*1.1; //show 10% more....
    geomSizeY= (maxy-miny)*1.1;
    geomSizeZ= (maxz-minz)*1.1;
  }
  SbVec3f pos(posX,posY,posZ);
  SbRotation rot(rotX,rotY,rotZ,rotW);
  SbVec3f size(sizeX,sizeY,sizeZ);

  window->setPosition(pos);
  window->setRotation(rot);
  window->setSize(size);

  winTrans->translation.setValue(geomMoveX,geomMoveY,geomMoveZ);

  if (geomSizeX==0.0) geomRatioX=sizeX; else geomRatioX=sizeX/geomSizeX;
  if (geomSizeY==0.0) geomRatioY=sizeY; else geomRatioY=sizeY/geomSizeY;
  if (geomSizeZ==0.0) geomRatioZ=sizeZ; else geomRatioZ=sizeZ/geomSizeZ;
  winScale->scaleFactor.setValue(geomRatioX,geomRatioY,geomRatioZ);

  correctPointsAndLines(clientVolume);
  if (fitGeometry)
  {
    update();  //send the update of the new parameter to the client
  }
}


//---------------------------------------------------------------------------
// setUpdate(char *para)
//
// calls the setUpdate method from the Module moduleId with data
// Parameter:
//   moduleId the Id of the Module which parameters have to be updated...
//   data the parameterstring with the updateing values
//
void ClientModule::setUpdate(int moduleId,char *data)
{
  Module *updateModule;
  Debug(printf("Client Update %d %s\n",moduleId,data));
  if (moduleId==destModId)
  {
    updateModule=this;
  }
  else
  {
    updateModule=Modules->findModule(moduleId);
  }

  if (updateModule!=NULL)
  {
    updateModule->setUpdate(data);
  }
}


//---------------------------------------------------------------------------
// addNewModule(int moduleId,int moduleType,char *para)
//
// genearates a new Object of Type moduleType with the parameterstring para
// and insert it into the ModuleManager.
// Parameter:
//   moduleId, id of the Module to be created
//   moduleType, type of the new Module
//   para, parameterstring of the new Module
//
void ClientModule::addNewModule(int moduleId,int moduleType,char *para)
{
  Module *newModule=NULL;


  if ((newModule=Modules->findModule(moduleId))!=NULL)
  {
    if (moduleType==newModule->getType())  //ok. Module already here - update
      setUpdate(moduleId,para);
  }
  else
  {
    Debug(printf("New Module type:%d  Id:%d\n",moduleType,moduleId));
    switch (moduleType)
    {
      case STB_MODULE_PIPGROUP:
      {
        newModule= new PipGroupModule(this,moduleId,para);
      }
      break;
      case STB_MODULE_INT:
      {
        newModule= new IntModule(this,moduleId,para);
      }
      break;
      case STB_MODULE_BOOL:
      {
        newModule= new BoolModule(this,moduleId,para);
      }
      break;
      case STB_MODULE_FLOAT:
      {
        newModule= new FloatModule(this,moduleId,para);
      }
      break;
      case STB_MODULE_GEOM:
      {
        newModule= new GeometryModule(this,moduleId,para);
      }
      break;
      case STB_MODULE_POINT:
      {
        newModule= new PointModule(this,moduleId,para);
      }
      break;
      case STB_MODULE_LINE:
      {
        newModule= new LineModule(this,moduleId,para);
      }
      break;
      case STB_MODULE_POLYLINE:
      {
        newModule= new PolylineModule(this,moduleId,para);
      }
      break;

    }

    //add only known Modules to the Modulemanager
    if (newModule!=NULL) Modules->addModule(newModule);
  }
}


//---------------------------------------------------------------------------
// getFirstPipModule()
//
// returns the PipModule with the lowest order.
// if no PipModule is found NULL is returned.
//
PipModule* ClientModule::getFirstPipModule()
{
  Module *module;

  Modules->reset();
  while ((module=Modules->getNextModule())!=NULL)
  {
    if (module->isPipModule())
    {
      return PipModule::getFirstPipModule((PipModule *)module);
    }
  }
  return NULL;
}


//---------------------------------------------------------------------------
// getFirstWinModule()
//
// returns the first WindowModule.
// if no WindowModule is found NULL is returned.
//
WindowModule* ClientModule::getFirstWinModule()
{
  Module *module;

  Modules->reset();
  while ((module=Modules->getNextModule())!=NULL)
  {
    if (module->isWindowModule())
    {
      return WindowModule::getFirstWinModule((WindowModule *)module);
    }
  }

  return NULL;
}


//---------------------------------------------------------------------------
// initPipModuleList()
//
// sets the prev and next Pointers of all Window and PipModules.
// calls the order methode to order the PipModules..
//
void ClientModule::initPipModuleList()
{
  Module *module;
  PipModule *prevPipModule,*curPipModule;
  WindowModule *prevWinModule,*curWinModule;

  prevPipModule=NULL;
  curPipModule=NULL;
  prevWinModule=NULL;
  curWinModule=NULL;


  Modules->reset();
  while ((module=Modules->getNextModule())!=NULL)
  {
    if (module->isPipModule())
    {
      curPipModule=(PipModule *)module;
      curPipModule->prevPipModule=prevPipModule;
      if (prevPipModule!=NULL)
        prevPipModule->nextPipModule=curPipModule;
      prevPipModule=curPipModule;
    }
    if (module->isWindowModule())
    {
      curWinModule=(WindowModule *)module;
      curWinModule->prevWinModule=prevWinModule;
      if (prevWinModule!=NULL)
        prevWinModule->nextWinModule=curWinModule;
      prevWinModule=curWinModule;
    }
  }
  if (curPipModule!=NULL)
    curPipModule->nextPipModule=NULL;
  if (curWinModule!=NULL)
    curWinModule->nextWinModule=NULL;
  PipModule::orderPipModules(getFirstPipModule());
}


//---------------------------------------------------------------------------
// correctPointsAndLines(SoSeparator *sep)
//
// sets the cube sizes of the Lines and Points to the values depending on the
// current window size.
// Parameter:
//   the SoSeparator where the Lines and Points can be found as childs
//   or as childs from childs ......
//
void ClientModule::correctPointsAndLines(SoSeparator *sep)
{
  SoNodeList listPoints,listLines;
  int numElements;
  int i;
  float size;
  SoCube *cube;
  SbVec3f geomScale=winScale->scaleFactor.getValue();


  //calculate the scalefactor from the current window geometry display size...
  size=geomScale[0];
  if ((size>geomScale[1]) || (size==0.0)) size=geomScale[2];
  if ((size>geomScale[2]) || (size==0.0)) size=geomScale[3];
  if (size==0.0) size=1;
  size=0.001/size;


  //seach for the points
  numElements=sep->getByName("Cube_as_Point",listPoints);
  for (i=0;i<numElements;i++)
  {
    cube=(SoCube *)listPoints[i];
    cube->width.setValue(size);
    cube->height.setValue(size);
    cube->depth.setValue(size);
  }

  //I dont know why, but lines should be bigger to look the same as points
  size*=2;

  //search for lines
  numElements=sep->getByName("Cube_as_Line",listLines);
  for (i=0;i<numElements;i++)
  {
    cube=(SoCube *)listLines[i];
    cube->width.setValue(size);
    cube->depth.setValue(size);
  }

}


//---------------------------------------------------------------------------
// isModuleForUser(int theUser)
//
// returns TRUE if the Module is enabled for the user theUser.
// the users are encoded in the corresponding bits of the user property...
//
int ClientModule::isModuleForUser(int theUser)
{
  return ( ((user>>theUser)&1)==1);
}


//---------------------------------------------------------------------------
// updatePip()
//
// generates the PipGeometry for all users.
// calls the updatePip Method  of all PipModules in the current order..
//
void ClientModule::updatePip()
{
  SoTranslation *trans[MAX_USERS];
  PipModule *module;
  float left[MAX_USERS];
  int i;

  initPipModuleList();

  for (i=0;i<MAX_USERS;i++)
  {
    left[i]=0.0f;
    trans[i]=new SoTranslation;
    PIPGeometry[i]->removeAllChildren();
    PIPGeometry[i]->addChild(trans[i]);
  }

  module=getFirstPipModule();
  while (module!=NULL)
  {
    if (module->parentId==destModId)
    {
      for (i=0;i<MAX_USERS;i++)
      {
        if (isModuleForUser(user))  //only if the user is enabled for the client.
          module->updatePip(i,PIPGeometry[i],left[i]);
      }
    }
    module=module->nextPipModule;
  }

  //shift the Geoemtry half to the left, to garantee the middle at 0
  for (i=0;i<MAX_USERS;i++)
  {
    trans[i]->translation.setValue(-(left[i]/2.0f),0.0f,0.0f);
    sk[i]->size.setValue(0.32,0.015,0.20);
  }
}


//---------------------------------------------------------------------------
// updatePip()
//
// static Window Callback function
// is called from the Window of the client....
// if the window is moved or resized then it calls the update methode.
// if it is closed, then it calls the remove methode.
// if an event occurs in the window the eventInWindow methods are called
//   for each WindowModule stored in the ModuleManager....
// at least the focus is set to the current focus values....
//
SbBool ClientModule::MyWindowCB(void *userData,int messageID,SoWindowKit *window,
                int userID,So3DEvent *event, SoPath *clientVolumePath)
{
  ClientModule *client=(ClientModule*)userData;

  if (client==NULL) return TRUE;

  switch (messageID)
  {
    case WM_RESIZE:
    case WM_WINDOWMOVE:
    {
      client->update();  //sends an Update to the Visualization system
    }
    break;
    case WM_CLOSEW:
    case WM_KILL:
    {
      client->remove(TRUE);
      client=NULL;
    }
    break;
    case CB_WINDOW_WILL_GET_FOCUS:
    {
      client->setClientWindowPIP(userID);
    }
    break;
    case WM_EVENT:
    {
      if (client->isModuleForUser(userID))
      {
        WindowModule* curModule;
        SbVec3f windowTranslation;
        SbVec3f geomScale=client->winScale->scaleFactor.getValue();

        window->getRotation().inverse().multVec(event->getTranslation()-window->getPosition(),windowTranslation);

        windowTranslation[0]*=1.0f/geomScale[0];
        windowTranslation[1]*=1.0f/geomScale[1];
        windowTranslation[2]*=1.0f/geomScale[2];

        curModule=client->getFirstWinModule();
        while (curModule !=NULL)
        {
          curModule->eventInWindow(event,userID,&windowTranslation);
          curModule=curModule->nextWinModule;
        }
      }
    }
    break;
  }


  if ((userID>=0) && (window!=NULL))
    client->userFocus[userID]=window->hasFocus(userID);

  return TRUE;
}
