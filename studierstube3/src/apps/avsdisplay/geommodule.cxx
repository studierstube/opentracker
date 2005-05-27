// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       client
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Geometry Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "geommodule.h"
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSphere.h>


#include <stbapi/util/ivio.h>
#include <stbapi/util/os.h>

#include "Bin2IV.h"


//---------------------------------------------------------------------------
// Constructor
//
// initlializes the GeometryModule Object.
// initilaizes the userSwitch  and the geom separator
// and sets the propertys to the value of the parameter string.
// Parameter:
//   the Pointer to the Client, 
//   the Module-Id, 
//   its Parameter string
//
GeometryModule::GeometryModule(ClientModule *theClient,int moduleId,char *para)
{
  char geomClear;

  Debug(printf("new GeomModule %d %s\n",moduleId,para));
  id=moduleId;
  client=theClient;

  userSwitch=new SoContextSwitch;
  userSwitch->ref();
  userSwitch->context.setValue(SoContextSwitch::USER);

  geom= new So3DSeparator;
  geom->ref();
  geom->eventMask=Base3D::BM_NO_EVENT;


  sscanf(para,"%c %d",&geomClear,&user);
  if ( (geomClear=='y') || (geomClear=='Y') )
    clr=TRUE;
  else
    clr=FALSE;

  geomReceiving=FALSE;

  //inserts the userSwitch and sets its children to the user specified users..
  for (int i=0;i<MAX_USERS;i++)
  {
    if (((user>>i) & 1)==1)
      userSwitch->addChild(geom);
    else
      userSwitch->addChild(new SoGroup);  
  }
  
  currentDecodeElement=NULL;
  lastDecodeElement=NULL;
  accessAvailable=TRUE;

#ifdef WITH_GEOM_DECODE_THREAD
  geomDecodeThreadPid=createThread(GeometryModule::geomDecodeThread,(void *)this); 
#endif
}

//---------------------------------------------------------------------------
// initOIVObjects
//
// does the initialization of the OIV Objects within the rendering loop.
// necessary for some OIV Objects and StudierstubeOIV Objects like the WindowKit
//
void GeometryModule::initOIVObjects()
{
  client->clientVolume->addChild(userSwitch);
}

//---------------------------------------------------------------------------
// Destructor
//
// deletes the geom separator and the userswitch
//
GeometryModule::~GeometryModule()
{
#ifdef WITH_GEOM_DECODE_THREAD
  killThread(geomDecodeThreadPid);
#endif

  Debug(printf("Geom destructor...\n"));
  client->clientVolume->removeChild(geom);

  userSwitch->removeAllChildren();
  userSwitch->ref();

  geom->removeAllChildren();
  geom->unref();
}


//---------------------------------------------------------------------------
// saveGeometry(SoNode *node,char *fileName)
//
// static method, 
// saves the geometry specified in the node to the file fileName...
// Parameters:
//    node, node of the geometry to be saved
//    fileName, name of the file where the geometry should be saved to..
//
void GeometryModule::saveGeometry(SoNode *node,char *fileName)
{
  SoWriteAction myAction;
  Debug(printf("About to save %s .....\n",fileName));
  myAction.getOutput()-> openFile(fileName);
  myAction.getOutput()-> setBinary(FALSE);
  myAction.apply(node);
  myAction.getOutput()-> closeFile();
}


//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, in this case STB_MODULE_GEOM
//
int GeometryModule::getType()
{ 
  return STB_MODULE_GEOM;
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// in this case it is a PIPModule - returns TRUE
//
int GeometryModule::isPipModule()
{ 
  return FALSE;
}


//---------------------------------------------------------------------------
// isWindowModule()
//
// returns wheter the Module is a WindowModule or not.
// in this case it is no WindowModule because it handles no windowEvents...
// therefore it returns always FALSE.
//
int GeometryModule::isWindowModule()
{ 
  return FALSE;
}


//---------------------------------------------------------------------------
// setUpdate(char *para)
//
// sets the properties to the values of the parameterstring.
// this Method is called from the Client, when an update parameterstring 
// comes from the stbWin....
//
void GeometryModule::setUpdate(char *para)
{
  int newUser;
  char geomClear;

  sscanf(para,"%c %d",&geomClear,&newUser);
  if ( (geomClear=='y') || (geomClear=='Y') )
    clr=TRUE;
  else
    clr=FALSE;

  if (newUser!=user)
  {
    user=newUser;

    userSwitch->removeAllChildren();  
    for (int i=0;i<MAX_USERS;i++)
    {
      if (((user>>i) & 1)==1)
        userSwitch->addChild(geom);
      else
        userSwitch->addChild(new SoGroup);  
    }    
  }
}


//---------------------------------------------------------------------------
// gotGeomUpdate(char *para)
//
// this function is called from the client when the client gets a
// geometry packet. 
// it uses the CBuildTree object to decode the packet.
//
void GeometryModule::gotGeomUpdate(int len,char *data)
{
  char *tmp;
  tmp=data;

  if (!geomReceiving)
  {
    if (stbNCGetChar(&tmp)==GEOMETRY_START)
    {
      Debug(printf("Geometry start...\n"));
      newGeom=new SoSeparator;
      newGeom->ref();
      geomReceiving=TRUE;
      myCBuildTree.setSeparator(newGeom);
    }
  }
  else
  {
    int type=stbNCGetChar(&tmp);

    switch (type)
    {
      case GEOMETRY_START:
      {
        //start new Geometry, so delete old.....
        Debug(printf("new Geometry start...\n"));
        newGeom->removeAllChildren();
        geomReceiving=TRUE;
        myCBuildTree.setSeparator(newGeom);
      }
      break;
      case GEOMETRY_END:
      {
        geomReceiving=FALSE;
#ifndef WITH_GEOM_DECODE_THREAD
        geomUpdateFinish();
#endif
        Debug(printf("Geometry complete...\n"));
      }
      break;
      default:
      {
        //parsing the geometry....
#ifdef WITH_GEOM_DECODE_THREAD
        struct decodeElement *newDataElement;
        newDataElement=(struct decodeElement*)malloc(sizeof(struct decodeElement));
        newDataElement->next=NULL;
        newDataElement->data=data;
        while (!accessAvailable);
        accessAvailable=FALSE;
        if (lastDecodeElement!=NULL)
        {
          lastDecodeElement->next=newDataElement;
        }
        else
        {
          currentDecodeElement=newDataElement;
        }
        lastDecodeElement=newDataElement;
        accessAvailable=TRUE;
        sem.signal();
#endif

#ifndef WITH_GEOM_DECODE_THREAD
        myCBuildTree.Parse(data);
#endif
      }
    }
  }
}

//---------------------------------------------------------------------------
// geomUpdateFinish
//
// this function is called from the client  
// if the geometry is complete, to insert it into the geom separator.
//
void GeometryModule::geomUpdateFinish()
{

  if (geomReceiving==FALSE)
  {
    if (clr)
      geom->removeAllChildren();
    geom->renderCaching.setValue(SoSeparator::OFF);
    newGeom->renderCaching.setValue(SoSeparator::OFF);

    geom->addChild(newGeom);
    newGeom->unref();
    newGeom=NULL;
  }
}


#ifdef WITH_GEOM_DECODE_THREAD
//---------------------------------------------------------------------------
// geomDecodeThread
//
// does the decoding in a separate thread...
//
void GeometryModule::geomDecodeThread(void *data)
{
  GeometryModule *geom=(GeometryModule *)data;
  struct decodeElement *decoding;

  FOREVER
  {
    geom->sem.wait();
    if (geom->currentDecodeElement!=NULL)
    {
      while (!geom->accessAvailable);
      geom->accessAvailable=FALSE;
      if (geom->lastDecodeElement==geom->currentDecodeElement)
        geom->lastDecodeElement=NULL;
      decoding=geom->currentDecodeElement;
      geom->currentDecodeElement=geom->currentDecodeElement->next;
      geom->accessAvailable=TRUE;      
      geom->myCBuildTree.Parse(decoding->data);
      free(decoding->data);
      free(decoding);
    }
  }
}  
#endif

