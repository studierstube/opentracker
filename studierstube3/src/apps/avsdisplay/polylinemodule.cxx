// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       polyPolylinemodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    polyPolyline in the Window....
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  4-okt-99 15:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "polylinemodule.h"


//---------------------------------------------------------------------------
// Constructor
//
// initlializes the PolylineModule Object.
// initilaizes the Widgets, the Help-Text Geometry with the initWindowModule
// methode, initializes the actionLine and actionDragLine Geometry,
// and sets the propertys to the value of the parameter string.
// Parameter:
//   the Pointer to the Client, 
//   the Module-Id, 
//   its Parameter string
//
PolylineModule::PolylineModule(ClientModule *theClient,int moduleId,char *para)
{
  int dummy;
  float floatDummy;
  char withTexture;
  char buffer[1024];
  char textureFileName[256];

  Debug(printf("new PolylineModule %s\n",para));
  id=moduleId;
  client=theClient;

  memset(buffer,0,1024);
  sscanf(para,"%d %d %d %f %f %f %f %f %f %f %d %d %c %1023c",&parentId,&dummy,&dummy,&minDrawLineLength,&floatDummy,&floatDummy,&floatDummy,&floatDummy,&floatDummy,&floatDummy,&order,&user,&withTexture,buffer);

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

  WindowModule::initWindowModule(name);

  actionLine=new SoSeparator;
  actionLine->ref();
  actionDragLine=new SoSeparator;
  actionDragLine->ref();

  curDragPoint=-1;

  LineCol.setValue(1.0f,0.3f,0.0f);

  isPressed=-1;
}


//---------------------------------------------------------------------------
// Destructor
//
// deletes the actionLine and actionDragLine Geometry
//
PolylineModule::~PolylineModule()
{
  actionDragLine->unref();
  actionLine->unref();
  Debug(printf("delete PolylineModule \n"));
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// in this case it is a PIPModule because every WindowModule is a
// pipModule - returns TRUE
//
int PolylineModule::isPipModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// isWindowModule()
//
// returns wheter the Module is a WindowModule or not.
// in this case it is a WindowModule - returns TRUE
//
int PolylineModule::isWindowModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, in this case STB_MODULE_POLYLINE
//
int PolylineModule::getType()
{ 
  return STB_MODULE_POLYLINE;
}


//---------------------------------------------------------------------------
// setUpdate(char *para)
//
// sets the properties to the values of the parameterstring.
// this Method is called from the Client, when an update parameterstring 
// comes from the stbWin....
// calls the client->updatePIP if the update will modify the PIP Geometry...
//
void PolylineModule::setUpdate(char *para)
{
  int newParentId,newOrder,newUser;
  int dummy;
  float floatDummy;
  char buffer[1024];
  char textureFileName[256];
  char withTexture;

  Debug(printf("PolylineModule gotnewUpdate %s\n",para));  
  memset(buffer,0,1024);
  sscanf(para,"%d %d %d %f %f %f %f %f %f %f %d %d %c %1023c",&newParentId,&dummy,&dummy,&minDrawLineLength,&floatDummy,&floatDummy,&floatDummy,&floatDummy,&floatDummy,&floatDummy,&newOrder,&newUser,&withTexture,buffer);

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
// update(int lineNo)
//
// updates the line position of the actionDragLine Geometry
// sends an update parameterstring of the lineNo line direct 
// to the stbWin Module...
// this Method is called from the eventInWindow() method
// when the interaction is finished
// Parameter:
//   lineNo: there are 2 Points in the Parameterstring,
//           the positions of the 2 points of the line lineNo are stored
//           in the parameterstring
//
void PolylineModule::update(int lineNo)
{ 
  char buf[STB_MAXPARAMETERLEN];
  int i,num;
  
  num=positions.getNum()-1;
  if (lineNo==num) lineNo--;
  if ((num<0) || (lineNo<0))
  {
    if (num<0) num=0;
    sprintf(buf,"%d %d %d %f %f %f %f %f %f %f %d %d %s",parentId,num,-1,minDrawLineLength,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,order,user,name);
  }
  else
  {
    sprintf(buf,"%d %d %d %f %f %f %f %f %f %f %d %d %s",parentId,num,lineNo,minDrawLineLength,-positions[lineNo][1],-positions[lineNo][2],-positions[lineNo][0],-positions[lineNo+1][1],-positions[lineNo+1][2],-positions[lineNo+1][0],order,user,name);
  }
  Debug(printf("Polyline update send %s\n",buf));
  stbNCSend(client->nc,id,STB_UPDATE_MODULE,strlen(buf)+1,(unsigned char *)buf);  
  for (i=0;i<positions.getNum();i++)
  {
    WindowModule::setDragPointPosition((SoSeparator *)(actionDragLine->getChild(i)),&(positions[i]));    
  }
} 


//---------------------------------------------------------------------------
// update(int lineNo)
//
// used to get an update of the line positions, because the number
// of points is not limited and therefore it is not usefull to get the
// positions in the parameterstring
// Parameters:
//   len:  packet len of data array
//   data: data of the packet...
//
void PolylineModule::gotGeomUpdate(int len,char *data)
{
  int curNumPoints,curPoint;
  float posX,posY,posZ;
  int i;

  curNumPoints=stbNCGetInt(&data);
  curPoint=stbNCGetInt(&data);

  posX=stbNCGetFloat(&data);
  posY=stbNCGetFloat(&data);
  posZ=stbNCGetFloat(&data);

  Debug(printf("PolyLine gotGeomUpdate %d %d %d %f %f %f\n",positions.getNum(),curNumPoints,curPoint,posX,posY,posZ));
  if (curNumPoints<(positions.getNum()-1))
  {
    positions.deleteValues(curNumPoints+1,-1);
    for (i=curNumPoints;i<actionLine->getNumChildren();i++)
    {
      actionLine->removeChild(i);
    }
    for (i=curNumPoints+1;i<actionDragLine->getNumChildren();i++)
    {
      actionDragLine->removeChild(i);
    }
  }

  positions.set1Value(curPoint,-posZ,-posX,-posY);

  if (actionLine->getNumChildren()<(curPoint+1))
  {
    for (i=actionLine->getNumChildren();i<curNumPoints;i++)
    {
      actionLine->addChild(CBuildTree::NewLine(&(positions[i]),&(positions[i+1]),&LineCol));
    }
    for (i=actionDragLine->getNumChildren();i<=curNumPoints;i++)
    {
      actionDragLine->addChild(WindowModule::generateDragPoint(&(positions[i])));
    }
  }
  if (curPoint>0)
  {
    CBuildTree::setLinePos((SoSeparator*)actionLine->getChild(curPoint-1),&(positions[curPoint-1]),&(positions[curPoint]));
  }
  if (curPoint<(positions.getNum()-1))
  {
    CBuildTree::setLinePos((SoSeparator*)actionLine->getChild(curPoint),&(positions[curPoint]),&(positions[curPoint+1]));
  }
  WindowModule::setDragPointPosition((SoSeparator *)(actionDragLine->getChild(curPoint)),&(positions[curPoint]));    
}


//---------------------------------------------------------------------------
// showInteractionElements()
//
// shows interaction Elements like the actionLine and the actionDragLine
//
void PolylineModule::showInteractionElements()
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
void PolylineModule::hideInteractionElements()
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
void PolylineModule::eventInWindow(So3DEvent *event,int userID, SbVec3f *Pos)
{
  int pressedButtons= WindowModule::getButtonState(event);
  int i;

  if (isModuleForUser(userID))
  {
    if (myButton[userID]->isPressed())
    {
      if ((pressedButtons & actionButton)==actionButton)
      {
        if (client->clientVolume->findChild(actionDragLine)!=-1)
          client->clientVolume->removeChild(actionDragLine);
        if (isPressed==-1)	//Button pressed ->start Polyline
        {
	  if (curDragPoint==-1)
          {
            positions.deleteValues(0,-1);
            actionLine->removeAllChildren();
            actionDragLine->removeAllChildren();
            positions.set1Value(0,(*Pos)[0],(*Pos)[1],(*Pos)[2]);            
            positions.set1Value(1,(*Pos)[0],(*Pos)[1],(*Pos)[2]);            

            actionLine->addChild(CBuildTree::NewLine(Pos,Pos,&LineCol));
            actionDragLine->addChild(WindowModule::generateDragPoint(Pos));
            actionDragLine->addChild(WindowModule::generateDragPoint(Pos));
          }
          else
          {
            positions.set1Value(curDragPoint,(*Pos)[0],(*Pos)[1],(*Pos)[2]);   
          }
          isPressed=userID;
        }
        if (isPressed==userID)    //user is drawing Polyline  ->send Update...
        {
	  if (curDragPoint==-1)
          {
            SbVec3f size;
            int curPoint=positions.getNum()-1;

            size=positions[curPoint]-positions[curPoint-1];
            if (size.length()<minDrawLineLength)
            {
              positions.set1Value(curPoint,(*Pos)[0],(*Pos)[1],(*Pos)[2]);   
              CBuildTree::setLinePos((SoSeparator*)actionLine->getChild(curPoint-1),&(positions[curPoint-1]),&(positions[curPoint]));
            }
            else
            {
              positions.set1Value(curPoint+1,(*Pos)[0],(*Pos)[1],(*Pos)[2]);            
              actionLine->addChild(CBuildTree::NewLine(&(positions[curPoint]),&(positions[curPoint+1]),&LineCol));
              actionDragLine->addChild(WindowModule::generateDragPoint(Pos));
            }
          }
          else
          {
            positions.set1Value(curDragPoint,(*Pos)[0],(*Pos)[1],(*Pos)[2]);   
            if (curDragPoint>0)
            {
              CBuildTree::setLinePos((SoSeparator*)actionLine->getChild(curDragPoint-1),&(positions[curDragPoint-1]),&(positions[curDragPoint]));
            }
            if (curDragPoint<(positions.getNum()-1))
            {
              CBuildTree::setLinePos((SoSeparator*)actionLine->getChild(curDragPoint),&(positions[curDragPoint]),&(positions[curDragPoint+1]));
            }
          }

        }
      }
      else
      {
        if (isPressed==userID)   //Button release
        {
          if (curDragPoint==-1)
          {
            for (i=0;i<positions.getNum();i+=2)
            {
              update(i); 
            }
            update(-1); 
          }
          else
          {
            update(curDragPoint);
            update(-1);
          }
          if (client->clientVolume->findChild(actionDragLine)==-1)
            client->clientVolume->addChild(actionDragLine);
          curDragPoint=-1;
          isPressed=-1;
        }
        else
        {
          client->correctPointsAndLines(actionDragLine); 
          curDragPoint=-1;
          for (i=0;i<positions.getNum();i++)
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
