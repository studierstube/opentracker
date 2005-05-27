// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       pointmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    point in the Window....
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  19-jul-99 17:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "pointmodule.h"


//---------------------------------------------------------------------------
// Constructor
//
// initlializes the PointModule Object.
// initilaizes the Widgets, the Help-Text Geometry with the initWindowModule
// methode, initializes the actionPoint and actionDragPoint Geometry,
// and sets the propertys to the value of the parameter string.
// Parameter:
//   the Pointer to the Client, 
//   the Module-Id, 
//   its Parameter string
//
PointModule::PointModule(ClientModule *theClient,int moduleId,char *para)
{
  Debug(printf("new PointModule %s\n",para));
  id=moduleId;
  client=theClient;
  float posx,posy,posz;
  char withTexture;
  char buffer[1024];
  char textureFileName[256];

  memset(buffer,0,1024);
  sscanf(para,"%d %f %f %f %d %d %c %1023c",&parentId,&posx,&posy,&posz,&order,&user,&withTexture,buffer);
  //has it a texture parameter?
  if (withTexture=='Y')
  {
    memset(name,0,256);
    sscanf(buffer,"%s %255c",textureFileName,name);
    strChrReplace(textureFileName,(char)255,(char)' ');
  }
  else
  {
    strcpy(name,buffer);
  }

//  position.setValue(-posy,-posz,-posx);
  position.setValue(-posz,-posx,-posy);
 
  WindowModule::initWindowModule(name);
  Debug(printf("PointModule ende\n"));
 
  SbColor col(1.0f,0.3f,0.0f);
  actionPoint=(SoSeparator *)CBuildTree::NewPoint(&position,&col);
  actionPoint->ref();
  actionDragPoint=WindowModule::generateDragPoint(&position);
  actionDragPoint->ref();

  client->correctPointsAndLines(actionPoint);
  client->correctPointsAndLines(actionDragPoint);

  isPressedFocus=FALSE;
  isPressed=-1;  
}


//---------------------------------------------------------------------------
// Destructor
//
// deletes the actionPoint and actionDragPoint Geometry
//
PointModule::~PointModule()
{
  actionDragPoint->unref();
  actionPoint->unref();

  Debug(printf("delete PointModule \n"));
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// in this case it is a PIPModule because every WindowModule is a
// pipModule - returns TRUE
//
int PointModule::isPipModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// isWindowModule()
//
// returns wheter the Module is a WindowModule or not.
// in this case it is a WindowModule - returns TRUE
//
int PointModule::isWindowModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, in this case STB_MODULE_POINT
//
int PointModule::getType()
{ 
  return STB_MODULE_POINT;
}


//---------------------------------------------------------------------------
// setUpdate(char *para)
//
// sets the properties to the values of the parameterstring.
// this Method is called from the Client, when an update parameterstring 
// comes from the stbWin....
// calls the client->updatePIP if the update will modify the PIP Geometry...
// updates the point position of the actionPoint and actionDragPoint Geometry
//
void PointModule::setUpdate(char *para)
{
  int newParentId,newOrder,newUser;
  float posx,posy,posz;
  char withTexture;
  char buffer[1024];
  char textureFileName[256];

  Debug(printf("PointModule gotnewUpdate %s\n",para));  
  memset(buffer,0,1024);
  sscanf(para,"%d %f %f %f %d %d %c %1023c",&newParentId,&posx,&posy,&posz,&newOrder,&newUser,&withTexture,buffer);

  //has it a texture parameter?
  if (withTexture=='Y')
  {
    memset(name,0,256);
    sscanf(buffer,"%s %255c",textureFileName,name);
    buttonTexture->filename.setValue(textureFileName);
  }
  else
  {
    strcpy(name,buffer);
    buttonTexture->filename.setValue("");
  }

//  position.setValue(-posy,-posz,-posx);
  position.setValue(-posz,-posx,-posy);
  CBuildTree::setPointPos(actionPoint,&(position));
  WindowModule::setDragPointPosition(actionDragPoint,&(position));    

  setHelpText(name,4,-4.5);
  if ((newParentId!=parentId) || (newOrder!=order) || (newUser!=user))
  {
    order=newOrder;
    parentId=newParentId;
    user=newUser;
    client->updatePip();
  }
}


//---------------------------------------------------------------------------
// update()
//
// sends an update parameterstring direct to the stbWin Module...
// this Method is called from the static eventInWindow
// when the interaction is finished
//
void PointModule::update()
{ 
  char buf[STB_MAXPARAMETERLEN];

  sprintf(buf,"%d %f %f %f %d %d %s",parentId,-position[1],-position[2],-position[0],order,user,name);
  Debug(printf("Point update send %s\n",buf));
  stbNCSend(client->nc,id,STB_UPDATE_MODULE,strlen(buf)+1,(unsigned char *)buf);  
} 


//---------------------------------------------------------------------------
// showInteractionElements()
//
// shows interaction Elements like the actionPoint and the actionDragPoint
//
void PointModule::showInteractionElements()
{
  if (client->clientVolume->findChild(actionPoint)==-1)
    client->clientVolume->addChild(actionPoint);
  if (client->clientVolume->findChild(actionDragPoint)==-1)
    client->clientVolume->addChild(actionDragPoint);
}


//---------------------------------------------------------------------------
// hideInteractionElements()
//
// hides interaction Elements like the actionPoint and the actionDragPoint
//
void PointModule::hideInteractionElements()
{
  if (client->clientVolume->findChild(actionPoint)!=-1)
    client->clientVolume->removeChild(actionPoint);
  if (client->clientVolume->findChild(actionDragPoint)!=-1)
    client->clientVolume->removeChild(actionDragPoint);
}


//---------------------------------------------------------------------------
// eventInWindow(So3DEvent *event,int userID, SbVec3f *Pos)
//
// called from the client when an event in the window occured
// performs interaction with the pen
// Parameter:
//  event:  the So3DEvent
//  userId: the user which made the event
//  Pos:    the Position of the Pen in the Window where the event occured
//
void PointModule::eventInWindow(So3DEvent *event,int userID, SbVec3f *Pos)
{
  int pressedButtons= WindowModule::getButtonState(event);
 
  if (isModuleForUser(userID))
  {
    if (myButton[userID]->isPressed())
    {
	  if ((pressedButtons & actionButton)==actionButton)
      {
        if ((client->userFocus[userID]==FALSE) || (isPressedFocus))
        {
          isPressedFocus=TRUE;
        }
        else
        {
          if (client->clientVolume->findChild(actionDragPoint)!=-1)
            client->clientVolume->removeChild(actionDragPoint);
          if (isPressed==-1)	//Button pressed ->start Point
          {
            isPressed=userID;
          }
          if (isPressed==userID)    //user is dragging point
          {
            position=*Pos;
            CBuildTree::setPointPos(actionPoint,Pos);
            WindowModule::setDragPointPosition(actionDragPoint,Pos);    
          }
        }
      }
      else
      {
        if (isPressedFocus)
        {
          isPressedFocus=FALSE;
        }
        else
        {
          if (isPressed==userID)   //Button release
          {
            isPressed=-1;
            update();
          }

          if (client->clientVolume->findChild(actionDragPoint)==-1)
          {
            client->clientVolume->addChild(actionDragPoint);
            client->correctPointsAndLines(actionPoint); 
            client->correctPointsAndLines(actionDragPoint); 
          }
		  
          SbViewportRegion vp;
          SoGetBoundingBoxAction bba(vp);
          bba.apply(actionPoint);
          SbBox3f box=bba.getBoundingBox();
          if(box.intersect(*Pos)==TRUE)
          {
            WindowModule::setDragPointColor(actionDragPoint,SbColor(0.4,0.1,0.1));
          }
          else
          {
            WindowModule::setDragPointColor(actionDragPoint,SbColor(0.3,0.3,0.3));
          } 
        } 
      }
    }
  }
}
