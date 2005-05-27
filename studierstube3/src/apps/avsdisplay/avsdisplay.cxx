/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       AVSDISPLAY Server
    TYPE:       c++ code
    PROJECT:    Studierstube
    CONTENT:    Studierstube Application
    VERSION:    2.10
   ===========================================================================
    AUTHORS:    peci    Clemens Pecinovsky
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    15-Dec-00 16:17:50  jp      last modification
    27-Nov-00 11:21:28  jp      Linux port
    21-apr-99 08:00:00  peci    created
=========================================================================== */
#define NODEBUG

#ifdef  WIN32
#include <windows.h>
#ifdef  AVSDISPLAY_EXPORTS
#define AVSDISPLAY_API __declspec(dllexport)
#else
#define AVSDISPLAY_API __declspec(dllimport)
#endif

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
             )
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
#else
#define AVSDISPLAY_API
#endif

#include "avsdisplay.h"
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DRadioButton.h>
#include <stbapi/widgets/So3DIntSlider.h>
#include <stbapi/widgets/So3DExpSlider.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <stbapi/widgets/So3DDial.h>


#include <stbapi/util/multicast.h>

//-----------------------------------------------------------------------
//--- ProcessNetworkConnection Thread   receives TCP connection packets--
//-----------------------------------------------------------------------
THREADFUNC_RETURN_TYPE ProcessNetworkConnection(void *data)
{
  threadParameterProcessNetworkConnectionStruct *inParameter=(threadParameterProcessNetworkConnectionStruct*) data;

  stbNetCom *nc;
  stbMessage msg;
  int shouldquit=FALSE,mid;
  StbCommunicator *cMan;
  ModuleManager  *ClientMan;
  MesageParameterStruct *wsMsg;
  MesageParameterClientStruct *wsCMsg;
  char *para;
  int numNoMsgs=0;
  int sendStbMsg;

  ClientMan=inParameter->ClientMan;
  cMan=inParameter->communicationManager;
  nc=inParameter->nc;

  stbNCReceiveMsg(nc,&msg);
  if ((msg.type==STB_LOGIN) && (strcmp((char *)msg.data,STB_ID)==0) )
  {
    ClientModule *client;
    //mid=msg.moduleId;
    mid=nc->socket;

    para=((char*)(msg.data))+strlen(STB_ID)+1;
    para=stbNCGetString(&para);

    client=new ClientModule(cMan,ClientMan,mid,msg.moduleId,nc,para);
    if (client!=NULL)
      ClientMan->addModule(client);
    stbNCSend(nc,msg.moduleId,STB_LOGIN_ACK,0,NULL);
  }
  else
  {
    stbNCClose(nc);  //connection Error
    free(nc);
    return THREADFUNC_NULL;
  }

  para=((char*)(msg.data))+strlen(STB_ID)+1;
  para=stbNCGetString(&para);

  wsCMsg=(MesageParameterClientStruct *) malloc(sizeof(MesageParameterClientStruct));
  strcpy(wsCMsg->MsgIdString,STB_ID);
  wsCMsg->ClientMan=ClientMan;
  wsCMsg->clientId=mid;
  wsCMsg->msg.type=msg.type;
  wsCMsg->msg.length=msg.length;
  wsCMsg->msg.moduleId=msg.moduleId;
  wsCMsg->msg.data=(unsigned char*) malloc(strlen(para)+1);
  strcpy((char *)wsCMsg->msg.data,para);
  wsCMsg->nc=nc;

  cMan->sendMessage(MSG_STBSERVER_WSAPP_MSG,cMan->appID, cMan->appID, (void *) wsCMsg);
  while(!shouldquit)
  {
    memset((void*)&msg,0,sizeof(stbMessage));
    if (stbNCReceiveMsg(nc,&msg))
    {
      numNoMsgs=0;
      Debug(printf("Receive %d  %d \n",msg.moduleId,msg.type));
      if ((msg.moduleId==0) && (msg.type==STB_STILLRUNNING))
      {
        Debug(printf("stbNCSendCheckConnectionAck\n"));
        stbNCSendCheckConnectionAck(nc);
      }
      else
      {
        sendStbMsg=TRUE;
        if (msg.type==STB_REGISTER_MODULE)
        {
          int newModuleType;
          char *data;
          ClientModule *client;

          data=(char *)(msg.data);
          newModuleType=stbNCGetInt(&data);
          Debug(printf("STB_REGISTER_MODULE %d\n",newModuleType));
          client=(ClientModule *)ClientMan->findModule(mid);
          if (client!=NULL)
            client->addNewModule(msg.moduleId,newModuleType,data);
        }

#ifdef WITH_GEOM_DECODE_THREAD
        if (msg.type==STB_GEOMETRY)
        {
          //do the Geometryparsing in this Thread instead in rendering Loop!!
          ClientModule *client;

          Debug(printf("STB_GEOMETRY %d\n",msg.moduleId));
          client=(ClientModule *)ClientMan->findModule(mid);
          if (client!=NULL)
          {
            Module *geom=(GeometryModule *)client->Modules->findModule(msg.moduleId);
            if (geom!=NULL)
            {
              if (geom->getType()==STB_MODULE_GEOM)
              {
                char *tmp=(char *)msg.data;
                // do the parsing only if it is a GeometryModule and the Geometry
                // is not finished.
                geom->gotGeomUpdate(msg.length,(char *)msg.data);
                if (stbNCGetChar(&tmp)!=GEOMETRY_END)
                {
                  sendStbMsg=FALSE;
                  msg.data=NULL; //the data is freed inside the parser!!!
                }
                else
                {
                  //the data is freed inside the parser,
                  //but in this case we need a copy, which is freed here.
                  unsigned char *dataCopy=(unsigned char*) malloc(msg.length);
                  memcpy(dataCopy,msg.data,msg.length);
                  msg.data=dataCopy;
                }
              }
            }
          }
        }
#endif

        if (sendStbMsg)
        {
          wsMsg=(MesageParameterStruct *) malloc(sizeof(MesageParameterStruct));
          strcpy(wsMsg->MsgIdString,STB_ID);
          wsMsg->ClientMan=ClientMan;
          wsMsg->clientId=mid;
          wsMsg->msg.type=msg.type;
          wsMsg->msg.length=msg.length;
          wsMsg->msg.moduleId=msg.moduleId;
          if (msg.data==NULL)
          {
            wsMsg->msg.data=NULL;
          }
          else
          {
            wsMsg->msg.data=(unsigned char*) malloc(wsMsg->msg.length);
            memcpy(wsMsg->msg.data,msg.data,wsMsg->msg.length);
          }

          cMan->sendMessage(MSG_STBSERVER_WSAPP_MSG,cMan->appID, cMan->appID, (void *) wsMsg);
          if (msg.data!=NULL)
            free(msg.data);
          shouldquit=(msg.type==STB_LOGOUT);
        }
      }
    }
    else
    {
      if ((numNoMsgs++)>1000)
      {
        if (!stbNCCheckConnection(nc))
        {
          wsMsg=(MesageParameterStruct *) malloc(sizeof(MesageParameterStruct));
          strcpy(wsMsg->MsgIdString,STB_ID);
          wsMsg->ClientMan=ClientMan;
          wsMsg->clientId=mid;
          wsMsg->msg.type=STB_LOGOUT;
          wsMsg->msg.data=NULL;
          wsMsg->msg.length=0;
          wsMsg->msg.moduleId=mid;
          cMan->sendMessage(MSG_STBSERVER_WSAPP_MSG,cMan->appID, cMan->appID, (void *) wsMsg);

          shouldquit=TRUE;
        }
      }
    }
  }
  Debug(printf("Connection QUIT \n"));
  stbNCClose(nc);
  free(nc);
}

//-----------------------------------------------------------------------
//--- Network Thread    waits for TCP connection              ---
//-----------------------------------------------------------------------
THREADFUNC_RETURN_TYPE Wait4Connection(void *data)
{
  stbNetCom *nc;
  threadParameterWait4ConnectionStruct
    *inParameter=(threadParameterWait4ConnectionStruct*) data;
  threadParameterProcessNetworkConnectionStruct *outParameter=
    (threadParameterProcessNetworkConnectionStruct *)
    malloc(sizeof(threadParameterProcessNetworkConnectionStruct));;

  //get port from the environment
  char * env;
  char   portName[80]=SERVER_PORT, serverName[80]=SERVER_HOST;
  int    i;

  env = getenv(SERVER_ENV);

  /* change ":" into blank*/
  if (env)
  {
      i=0;
      while (env[i])
      {
          if (env[i]==':') env[i]=' ';
          i++;
      }
      sscanf(env, "%s %s", serverName, portName);
  }

  outParameter->nc                   = NULL;
  outParameter->communicationManager = inParameter->communicationManager;
  outParameter->ClientMan            = inParameter->ClientMan;
  free(inParameter);
  nc=(stbNetCom*) malloc(sizeof(stbNetCom));

  while (nc!=NULL)
  {
      stbNCServerInit(portName,nc);
      Debug(printf("new connection\n"));
      outParameter->nc=nc;
      createThread(ProcessNetworkConnection,(void*)outParameter);
      nc = (stbNetCom*) malloc(sizeof(stbNetCom));
  }

  return THREADFUNC_NULL;
}


//-----------------------------------------------------------------------
//--- InfoWindowCB  closes Information Window             ---
//-----------------------------------------------------------------------
SbBool InfoWindowCB(void *userData,int messageID,SoWindowKit *window,
        int userID,So3DEvent *event, SoPath *clientVolumePath)
{
  if (messageID==WM_EVENT)
  {
    window->closeWindow();
  }

  return TRUE;
}


//-----------------------------------------------------------------------
//--- Initaization routine of workspace application           ---
//-----------------------------------------------------------------------
void AVSDISPLAY_API Stb_init(int userID, StbCommunicator *cMan)
{
  ModuleManager *ClientMan;
  threadParameterWait4ConnectionStruct *threadParameter=
    (threadParameterWait4ConnectionStruct *) malloc(sizeof(threadParameterWait4ConnectionStruct));

  //generate the Information Window
  WindowCreate wc;
  SoWindowKit *windowKit = NULL;

  wc.position.setValue(-0.5, -0.5, 0.5);
  wc.orientation.setValue(SbVec3f(1.0, 0.0, 0.0), 0);
  wc.size.setValue(0.5, 0.3, 0.5);
  wc.style = 6+24+32;
  wc.state = 0;
  wc.title = SbString("");
  wc.eventMask = Base3D::BM_ALL_EVENTS;
  wc.clientEventMask = Base3D::BM_ALL_EVENTS;
  windowKit = cMan->createWindow(NULL, &wc, InfoWindowCB, NULL);
  windowKit->setBorderThickness(0.02);
  windowKit->setIconSize(SbVec3f(0.08, 0.08, 0.08));
  SoSeparator * clientVolume = windowKit->getClientVolumeSeparator();

  SoElapsedTime * textEngine = new SoElapsedTime;
  SoComposeRotation *xfRot = new SoComposeRotation;
  xfRot->axis.setValue(0,1,0);
  xfRot->angle.connectFrom(&textEngine->timeOut);
  SoTransform *infoXf = new SoTransform;
  infoXf->scaleFactor.setValue(0.07, 0.07, 0.03);
  infoXf->rotation.connectFrom(&xfRot->rotation);

  SoFont *infoFont = new SoFont;
  infoFont->size = 1;
  SoMaterial *infoMaterial = new SoMaterial;
  infoMaterial->diffuseColor.setValue(0.8, 0.2, 0.2);
  SoText3 *infoText = new SoText3;
  infoText->string.setValue("Stb - Server");
  infoText->justification = SoText3::CENTER;
  infoText->parts = SoText3::ALL;
  clientVolume->addChild(infoXf);
  clientVolume->addChild(infoFont);
  clientVolume->addChild(infoMaterial);
  clientVolume->addChild(infoText);
  infoMaterial = new SoMaterial;
  infoMaterial->diffuseColor.setValue(0.2, 0.2, 0.8);
  clientVolume->addChild(infoMaterial);
  SoTranslation *infoTrans = new SoTranslation;
  infoTrans->translation.setValue(0.0f,-1.0f,0.0f);
  clientVolume->addChild(infoTrans);
  infoText = new SoText3;
  infoText->string.setValue("is running");
  infoText->justification = SoText3::CENTER;
  infoText->parts = SoText3::ALL;
  clientVolume->addChild(infoText);

  //Create the ClientManager and start the Wait4Connection Thread
  ClientMan= new ModuleManager;
  cMan->setExitData((void *)ClientMan);
  cMan->setMessageData((void *)cMan);
  cMan->sendMessage(MSG_STBSERVER_ISRUNNING_MSG,cMan->appID, BROADCAST_ADDR, (void *) STB_ID);

  threadParameter->ClientMan=ClientMan;
  threadParameter->communicationManager=cMan;
  createThread(Wait4Connection,(void *)threadParameter);
}


//-----------------------------------------------------------------------
//--- Cleanup routine of workspace application                ---
//-----------------------------------------------------------------------
SbBool AVSDISPLAY_API Stb_exit(void *data)
{
  ModuleManager *ClientMan=(ModuleManager *) data;
  ClientModule *client;

  while (ClientMan->getNumModules()>0)
  {
    ClientMan->reset();
    if ((client=(ClientModule*)ClientMan->getNextModule())!=NULL)
    {
      stbNCServerClose(client->nc);
      ClientMan->removeModule(client->id);
      delete client;
    }
  }
  delete ClientMan;
  return TRUE;
}


//-----------------------------------------------------------------------
//--- Message routine of workspace application                        ---
//-----------------------------------------------------------------------
//gets the Messages from the NetworkThreads                           ---
//sends them to the objects....                                       ---
//-----------------------------------------------------------------------
void AVSDISPLAY_API Stb_getMessage(void *userData, int message, int sender, void *messageData)
{
  MesageParameterStruct *wsMsg=(MesageParameterStruct *)messageData;
  StbCommunicator *cMan=(StbCommunicator *)userData;

  if (wsMsg==NULL) return;

  //is it a MSG_STBSERVER_WSAPP_MSG ????
  if ((message==MSG_STBSERVER_WSAPP_MSG) && (strcmp(wsMsg->MsgIdString,STB_ID)==0))
  {
    ModuleManager *ClientMan=wsMsg->ClientMan;
    ClientModule *client;

    switch (wsMsg->msg.type)
    {
      case STB_LOGIN:
      {
        //MesageParameterClientStruct *wsCMsg=(MesageParameterClientStruct *)messageData;
        client=(ClientModule *)ClientMan->findModule(wsMsg->clientId);
        client->initOIVObjects();
      }
      break;
      case STB_LOGOUT:
      {
        client=(ClientModule *)ClientMan->findModule(wsMsg->clientId);
        client->remove(FALSE);
      }
      break;
      case STB_REGISTER_MODULE:
      {
        client=(ClientModule *)ClientMan->findModule(wsMsg->clientId);
        if (client!=NULL)
        {
          Module *modul=(Module *)client->Modules->findModule(wsMsg->msg.moduleId);
          if (modul!=NULL)
          {
            modul->initOIVObjects();
            client->updatePip();
          }
        }
      }
      break;
      case STB_UNREGISTER_MODULE:
      {
        Debug(printf("STB_UNREGISTER_MODULE %d\n",wsMsg->msg.moduleId));
        client=(ClientModule *)ClientMan->findModule(wsMsg->clientId);
        Module *theModule=(client->Modules->removeModule(wsMsg->msg.moduleId));
        if (theModule!=NULL) delete (theModule);
        client->updatePip();
      }
      break;
      case STB_UPDATE_MODULE:
      {
        client=(ClientModule *)ClientMan->findModule(wsMsg->clientId);
        if (client!=NULL)
        {
          client->setUpdate(wsMsg->msg.moduleId,(char *) wsMsg->msg.data);
        }
      }
      break;
      case STB_GEOMETRY:
      {
        Debug(printf("STB_GEOMETRY %d\n",wsMsg->msg.moduleId));
        client=(ClientModule *)ClientMan->findModule(wsMsg->clientId);
        if (client!=NULL)
        {
          Module *geom=client->Modules->findModule(wsMsg->msg.moduleId);
          if (geom!=NULL)
          {
#ifdef WITH_GEOM_DECODE_THREAD
            if (geom->getType()==STB_MODULE_GEOM)
            {
              ((GeometryModule *)geom)->geomUpdateFinish();
            }
            else
            {
              geom->gotGeomUpdate(wsMsg->msg.length,(char *)wsMsg->msg.data);
            }
#endif

#ifndef WITH_GEOM_DECODE_THREAD
            geom->gotGeomUpdate(wsMsg->msg.length,(char *)wsMsg->msg.data);
#endif
          }
        }
      }
      break;
    } //end switch
    if (wsMsg->msg.data!=NULL) free(wsMsg->msg.data);
    free(wsMsg);
  }
  else
  {
    if ((message==MSG_STBSERVER_ISRUNNING_MSG) && (strcmp(wsMsg->MsgIdString,STB_ID)==0))
    {
      if (sender!=cMan->appID)
      {
        //the Avsdisplay-Server is already running...
        //so quit myself
        cMan->stopApplication(cMan->appID,TRUE);
      }
    }
    else
    {
      Debug(printf("No STBMSG %d\n",message));
    }
  }
}

/* ===========================================================================
    End of acsdisplay.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
