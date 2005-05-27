// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       linemodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    line in the Window....
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  4-okt-99 15:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "linemodule.h"


//---------------------------------------------------------------------------
// Constructor
//
// initlializes the LineModule Object.
// initilaizes the Widgets, the Help-Text Geometry with the initWindowModule
// methode, initializes the actionLine and actionDragLine Geometry,
// and sets the propertys to the value of the parameter string.
// Parameter:
//   the Pointer to the Client, 
//   the Module-Id, 
//   its Parameter string
//
LineModule::LineModule(ClientModule *theClient,int moduleId,char *para)
{
  Debug(printf("new LineModule %s\n",para));
  float startPosX,startPosY,startPosZ,endPosX,endPosY,endPosZ;
  int i;
  char withTexture;
  char buffer[1024];
  char textureFileName[256];

  id=moduleId;
  client=theClient;


  memset(buffer,0,1024);
  sscanf(para,"%d %f %f %f %f %f %f %d %d %c %1023c",&parentId,&startPosX,&startPosY,&startPosZ,&endPosX,&endPosY,&endPosZ,&order,&user,&withTexture,buffer);

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

//  position[0].setValue(-startPosY,-startPosZ,-startPosX);
//  position[1].setValue(-endPosY,-endPosZ,-endPosX);

  position[0].setValue(-startPosZ,-startPosX,-startPosY);
  position[1].setValue(-endPosZ,-endPosX,-endPosY);

  WindowModule::initWindowModule(name);

  if (withTexture=='Y')
    buttonTexture->filename.setValue(textureFileName);
  else
    buttonTexture->filename.setValue("");

  SbColor col(1.0f,0.3f,0.0f);
  actionLine=(SoSeparator *)CBuildTree::NewLine(&position[0],&position[1],&col);
  actionLine->ref();
  actionDragLine=new SoSeparator;
  actionDragLine->ref();

  for (i=0;i<2;i++)
    actionDragLine->addChild(WindowModule::generateDragPoint(&(position[i])));

  client->correctPointsAndLines(actionLine);
  client->correctPointsAndLines(actionDragLine);

  curDragPoint=-1;
  isPressedFocus=FALSE;
  isPressed=-1;
}


//---------------------------------------------------------------------------
// Destructor
//
// deletes the actionLine and actionDragLine Geometry
//
LineModule::~LineModule()
{
  actionDragLine->unref();
  actionLine->unref();
  Debug(printf("delete LineModule \n"));
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// in this case it is a PIPModule because every WindowModule is a
// pipModule - returns TRUE
//
int LineModule::isPipModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// isWindowModule()
//
// returns wheter the Module is a WindowModule or not.
// in this case it is a WindowModule - returns TRUE
//
int LineModule::isWindowModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, in this case STB_MODULE_LINE
//
int LineModule::getType()
{ 
  return STB_MODULE_LINE;
}



//---------------------------------------------------------------------------
// setUpdate(char *para)
//
// sets the properties to the values of the parameterstring.
// this Method is called from the Client, when an update parameterstring 
// comes from the stbWin....
// calls the client->updatePIP if the update will modify the PIP Geometry...
// updates the line position of the actionLine and actionDragLine Geometry
//
void LineModule::setUpdate(char *para)
{
  int newParentId,newOrder,newUser;
  float startPosX,startPosY,startPosZ,endPosX,endPosY,endPosZ;
  char buffer[1024];
  char textureFileName[256];
  char withTexture;
  int i;

  Debug(printf("LineModule gotnewUpdate %s\n",para));  
  memset(buffer,0,1024);
  sscanf(para,"%d %f %f %f %f %f %f %d %d %c %1023c",&newParentId,&startPosX,&startPosY,&startPosZ,&endPosX,&endPosY,&endPosZ,&newOrder,&newUser,&withTexture,buffer);
  //has it a texture parameter?
  if (withTexture=='Y')
  {
    memset(name,0,256);
    sscanf(buffer,"%s %255c",textureFileName,name);
    strChrReplace(textureFileName,(char)255,(char)' ');
    buttonTexture->filename.setValue(textureFileName);
  }
  else
  {
    strcpy(name,buffer);
    buttonTexture->filename.setValue("");
  }

//  position[0].setValue(-startPosY,-startPosZ,-startPosX);
//  position[1].setValue(-endPosY,-endPosZ,-endPosX);

  position[0].setValue(-startPosZ,-startPosX,-startPosY);
  position[1].setValue(-endPosZ,-endPosX,-endPosY);

  setHelpText(name,4,-4.5);
  if ((newParentId!=parentId) || (newOrder!=order) || (newUser!=user))
  {
    order=newOrder;
    parentId=newParentId;
    user=newUser;
    client->updatePip();
  }
  CBuildTree::setLinePos(actionLine,&(position[0]),&(position[1]));
  for (i=0;i<2;i++)
  {
    WindowModule::setDragPointPosition((SoSeparator *)(actionDragLine->getChild(i)),(&position[i]));    
  }
}


//---------------------------------------------------------------------------
// update()
//
// updates the line position of the actionLine and actionDragLine Geometry
// sends an update parameterstring direct to the stbWin Module...
// this Method is called from the eventInWindow(0 method
// when the interaction is finished
//
void LineModule::update()
{ 
  char buf[STB_MAXPARAMETERLEN];
  int i;

  sprintf(buf,"%d %f %f %f %f %f %f %d %d %s",parentId,-position[0][1],-position[0][2],-position[0][0],-position[1][1],-position[1][2],-position[1][0],order,user,name);
  Debug(printf("Line update send %s\n",buf));
  stbNCSend(client->nc,id,STB_UPDATE_MODULE,strlen(buf)+1,(unsigned char *)buf);  
  for (i=0;i<2;i++)
  {
    WindowModule::setDragPointPosition((SoSeparator *)(actionDragLine->getChild(i)),(&position[i]));    
  }

} 


//---------------------------------------------------------------------------
// showInteractionElements()
//
// shows interaction Elements like the actionLine and the actionDragLine
//
void LineModule::showInteractionElements()
{
  if (client->clientVolume->findChild(actionLine)==-1)
    client->clientVolume->addChild(actionLine);
  if (client->clientVolume->findChild(actionDragLine)==-1)
    client->clientVolume->addChild(actionDragLine);
}


//---------------------------------------------------------------------------
// hideInteractionElements()
//
// hides interaction Elements like the actionLine and the actionDragLine
//
void LineModule::hideInteractionElements()
{
  if (client->clientVolume->findChild(actionLine)!=-1)
    client->clientVolume->removeChild(actionLine);
  if (client->clientVolume->findChild(actionDragLine)!=-1)
    client->clientVolume->removeChild(actionDragLine);
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
void LineModule::eventInWindow(So3DEvent *event,int userID, SbVec3f *Pos)
{
  int pressedButtons= WindowModule::getButtonState(event);
  int i;

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
          isPressedFocus=FALSE;
          
          if (client->clientVolume->findChild(actionDragLine)!=-1)
            client->clientVolume->removeChild(actionDragLine);
          if (isPressed==-1)	//Button pressed ->start Line
          {
            if (curDragPoint==-1)
            {
              position[0]=*Pos;
            }
            else
            {
              position[curDragPoint]=*Pos;
            }
            isPressed=userID;
          }
          if (isPressed==userID)    //user is drawing Line  ->send Update...
          {
            if (curDragPoint==-1)
            {
              position[1]=*Pos;
            }
            else
            {
              position[curDragPoint]=*Pos;
            }
            CBuildTree::setLinePos(actionLine,&(position[0]),&(position[1]));
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
            client->clientVolume->addChild(actionDragLine);
          }
          else
          {
            client->correctPointsAndLines(actionDragLine); 
            curDragPoint=-1;

            for (i=0;i<2;i++)
            {
              SoSeparator *actionDragPoint=(SoSeparator *)(actionDragLine->getChild(i));

              SbViewportRegion vp;
              SoGetBoundingBoxAction bba(vp);
              bba.apply(actionDragPoint);
              SbXfBox3f xfbox=bba.getXfBoundingBox();
              if ((xfbox.intersect(*Pos)==TRUE) && (curDragPoint==-1))
              {
                WindowModule::setDragPointColor(actionDragPoint,SbColor(0.4,0.1,0.1));
                curDragPoint=i;
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
  }
}
