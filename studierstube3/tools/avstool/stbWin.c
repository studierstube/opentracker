/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbWin
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    AVS Module which provides the communication to the displayserver
//              stores information about all module connected to it
//              sends updates from the display server to the modules via AVS
//              all connected stbModules communicate to it via FIFO
//              
//  VERSION:    1.1
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================   */

#define NODEBUG

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <avs/avs.h>
#include <unistd.h>
#include <avs/port.h>
#include <avs/field.h>
#include <avs/geom.h>
#include "stb.h"
#include "stbFifo.h"
#include "stbComm.h"


/*------------------------------------------------------------------------------
//static globals....                                                            */
static stbNetCom nc;               /* used for the network communication */
static int moduleId;               /* the ID of the Module */
static int fifo_fd,dummy_fifo_fd;  /* used to receive data from the fifo */


/*------------------------------------------------------------------------------
// structure for the module list                                                */
struct stbModule
{
  int moduleId;
  int moduleType;
  char parameter[STB_MAXPARAMETERLEN];
  struct stbModule *next;
} *stbModules;  /* and a pointer to its first entry */


/*------------------------------------------------------------------------------
// findModule
//
// searches a module in the module-List
//
//Parameter:
// moduleId: id of the module to search for
//
//returns:
// a pointer to the module if it was found, NULL otherwhise                     */
struct stbModule* findModule(int moduleId)
{
  struct stbModule *p;
  
  p=stbModules;
  while(p!=NULL)
  {
    if (p->moduleId==moduleId) return p;
    p=p->next;
  }
  return p;
}


/*------------------------------------------------------------------------------
// printModule
//
// prints all Modules and their infos from the module list
// used for debug only                                                          */
/*
void printModule()
{
  struct stbModule *p;
  int i;

  i=0;
  p=stbModules;
  while(p!=NULL)
  {
    printf("%d: %d %d %s\n",i,p->moduleId,p->moduleType,p->parameter);
    i++;
    p=p->next;
  }
}
*/


/*------------------------------------------------------------------------------
// removeModule
//
// removes a module in the module-List
//
//Parameter:
// moduleId: the id of the module which has to be removed
//
//returns:
// TRUE if the module was found and removed, FALSE otherwhise                   */
int removeModule(int moduleId)
{
  struct stbModule *p,*pb;
  int isPipGroup=FALSE;  
  int parent;
  int groupId[5];
  int wasDeleted=FALSE;

  Debug(printf("%d RemoveModule %d\n",moduleId,moduleId));
  p=stbModules;
  pb=NULL;
  while ((p!=NULL) && (!wasDeleted) )
  {
    if (p->moduleId==moduleId)
    {
      if (isPipGroup=(p->moduleType == STB_MODULE_PIPGROUP))
      {
        int dummy;
        sscanf(p->parameter,"%d %d %d %d %d %d",&dummy,&groupId[0],&groupId[1],&groupId[2],&groupId[3],&groupId[4]);
      }      
      if (pb==NULL) 
      {
        stbModules=p->next;
      }
      else
      {
        pb->next=p->next;
      }
      free(p);
      wasDeleted=TRUE;
      return TRUE;
    }
    pb=p;
    p=p->next;
  }

  return FALSE;

  /*if it is a PipGroupModule then remove also the childs of the Module*/
  if (isPipGroup)
  {
    p=stbModules;
    while(p!=NULL)
    {
      p=stbModules;
      while(p!=NULL)
      {
        if (p->moduleType>=STB_MODULE_PIPGROUP)
        {
          sscanf(p->parameter,"%d ",&parent);
          if ( (parent==groupId[0]) || (parent==groupId[1]) ||
               (parent==groupId[1]) || (parent==groupId[2]) ||
               (parent==groupId[3]) )
          {
            stbFifoMsg msg;

            /* send unregister message to client*/
            msg.moduleId=p->moduleId;
            msg.type=STBFIFO_UNREGISTER_MODULE;
            msg.length= 0;
            msg.data=NULL;
            stbNCSendMsg(&nc,&msg);
            /* must be recursive to ensure that group-goup kombinations are also removed... */
            if (removeModule(p->moduleId))
            {
              p=NULL;
            }
          } 
        }
        if (p!=NULL)
        {
          p=p->next;
        }
      }
    }
  }

  return wasDeleted;
}


/*------------------------------------------------------------------------------
// removeAllModules
//
// removes all modules from the module-List (at the end...)
//
//returns:
// TRUE if all modules were removed (in this case everytime!)                   */
int removeAllModules()
{
  struct stbModule *p,*p_hlp;
  
  Debug(printf("%d RemoveAllModules \n"));
  p=stbModules;
  while(p!=NULL)
  {
    p_hlp=p;
    p=p->next;
    free(p_hlp);
  }
  return TRUE;
}


/*------------------------------------------------------------------------------
// insertModule
//
// inserts a module to the module-List
//
//Parameter:
// insMod: a valid pointer to the Modulstructure which has to be inserted       */
void insertModule(struct stbModule *insMod)
{
  struct stbModule *p,*pb;
  Debug(printf("%d insertModule %d %s\n",moduleId,insMod->moduleId,insMod->parameter));

  insMod->next=NULL;  
  p=stbModules;
  if (p==NULL)
  {
    stbModules=insMod;
    return;
  }

  while((p->next) !=NULL)
  {
    p=p->next;
  }
  p->next=insMod;
}


/*------------------------------------------------------------------------------
// RemoveEvent
//
// removes an event from the event (output) string
//
//Parameter:
// s:        the event (output) string of the stbWin
// moduleId: the id of the module the event has to be removed
// eventnumber: the id of the event wich has to be removed                      */
void RemoveEvent(char *s, int eventnumber, int moduleId) 
{
  int eno,mno;
  char *p,*buf;

  buf=(char *) malloc(strlen(s)+1);
  (*buf)=0;

  p=strtok(s, "!"); /* read over first token */
  strcpy(buf, p);
  strcat(buf, "!");
  while (p != NULL) 
  {
    p=strtok(NULL, "!");
    if (p != NULL) 
    {
      sscanf(p, "%d %d", &eno, &mno);
      /*Optimization, remove also events that happened bevore */
      if ( (moduleId != mno) || ((moduleId == mno) && (eventnumber<eno)) )
      {
        strcat(buf, p);
        strcat(buf, "!");
      }
    }
  }
  strcpy(s, buf);
  free (buf);
}


/*------------------------------------------------------------------------------
// Connect
//
// connects to the displayserver
// disables the Port and Host Widgets if the connection was successfull
//
//Parameter:
// cn:      pointer to boolean(int),TRUE if connection was successfull....
// hn:      hostname
// pn:      portname
// param:   parameter string of the stbWin
// silence: should a warning be displayed or not?                               */
void Connect(int *cn,char *hn,char *pn,char *param,int silence)
{
  char s[255];

  strcpy(s,"Host: ");strcat(s,hn);
  AVSmodify_parameter("Host",AVS_VALUE,s);
  strcpy(s,"Port: ");strcat(s,pn);
  AVSmodify_parameter("Port",AVS_VALUE,s);

  if (stbNCConnect(hn,pn,moduleId,param,&nc))
  {
    struct stbModule *mod;
    stbMessage msg;
    int tmp;
    

    AVSparameter_visible("Host",1); 
    AVSparameter_visible("Port",1);
    AVSparameter_visible("Host:",0); 
    AVSparameter_visible("Port:",0);

    mod=stbModules;
    if (mod==NULL) Debug(printf("No Modules connected!\n"));
    while(mod!=NULL)
    {
      msg.moduleId=mod->moduleId;
      msg.type=STB_REGISTER_MODULE;
      msg.length=strlen(mod->parameter)+1+sizeof(int);
      msg.data=(char *)malloc(msg.length);
      tmp=htonl(mod->moduleType);
      memcpy(msg.data, &tmp, sizeof(int));
      memcpy(msg.data+sizeof(int), mod->parameter, strlen(mod->parameter)+1);
      Debug(printf("Sending %d %s \n",mod->moduleId,mod->parameter));
      stbNCSendMsg(&nc,&msg);

      free(msg.data);
      mod=mod->next;
    }
    if (!(*cn))
    {
      AVSmodify_parameter("connect",AVS_VALUE,1); 
      (*cn)=1;
    }
  }
  else
  {
    if (!silence)
    {
      AVSerror("Could not connect to Server at Host:%s with Port:%s",hn,pn); 
    }
    if (*cn)
    {
      AVSmodify_parameter("connect",AVS_VALUE,0);
      (*cn)=0;
    }
    AVSparameter_visible("Host",0); 
    AVSparameter_visible("Port",0);
    AVSparameter_visible("Host:",1); 
    AVSparameter_visible("Port:",1);
  }
}


/*------------------------------------------------------------------------------
// Disconnect
//
// disconnect to the displayserver
// enables the Port and Host Widgets 
//
//Parameter:
// cn:      pointer to boolean(int),FALSE at the return of the function
// sendDisconnectToServer: should a disconnect packet be sent to the server?    */
void Disconnect(int *cn,int sendDisconnectToServer)
{
  Debug(printf("stbWin Disconnect %d %d\n",*cn,sendDisconnectToServer));

  AVSparameter_visible("Host",0); 
  AVSparameter_visible("Port",0);
  AVSparameter_visible("Host:",1); 
  AVSparameter_visible("Port:",1);
  if (sendDisconnectToServer)
    stbNCDisconnect(moduleId,&nc);  
  else
    stbNCClose(&nc);
  if (*cn)
  {
    AVSmodify_parameter("connect",AVS_VALUE,0);
    (*cn)=0;
  }
} 

/*------------------------------------------------------------------------------
// CleanUp
//
// should be called when the stbWin Modul is removed from the network
// or the avs is quited
// but it is not called always, only the devil knows why.....                   */
int CleanUp()
{
  Debug(printf("stbWin has quit! (ID: %d)...\n",moduleId));
  stbNCDisconnect(moduleId,&nc);

  stbFifoClose(fifo_fd);
  stbFifoClose(dummy_fifo_fd);
  stbFifoRemove(moduleId);

  removeAllModules();  

  return 1;
}


/*------------------------------------------------------------------------------
// main
//
// the main AVS coroutine called by the AVS                                      
// this function is running forever!!!                                          */
int main(int argc, char **argv) 
{
  int BP_Coroutine_desc();
  char com_out[1024];
  char buffer[1024];
  int dummy, run=-1;
  /*fd_set myset; */
  int mask=COROUT_WAIT;
  int doOutPut;
  stbFifoMsg *fifo_msg;
  int shutdown_flag;
  int event_number=1;
  stbMessage msg;
  char *hn,*pn,*hnd,*pnd;
  int cn,db;
  fd_set myset;
  STBFIFO_IPC_INFO ipc_info;
  float *posX,*posY,*posZ;
  float *sizeX,*sizeY,*sizeZ;
  float *geomSizeX,*geomSizeY,*geomSizeZ;
  float *geomMoveX,*geomMoveY,*geomMoveZ;
  float *rotX,*rotY,*rotZ,*rotW;
  char moduleName[256];
  char *commandOut,*commandError;
  int NumNoMsgs=0;
  int user;
  int users;
  int fitGeometry;

  stbModules=NULL;
  AVScorout_init(argc,argv,BP_Coroutine_desc);

  AVScorout_set_sync(1);

  moduleId=AVSget_unique_id();
  Debug(printf("stbWin: moduleId %d\n", moduleId));

  /* Create read end of fifo */
  if (!stbFifoCreate(moduleId)) {
    fprintf(stderr, "stbWin: cannot create fifo\n");
    return -1;
  }
  if ((fifo_fd=stbFifoOpen(moduleId, O_RDONLY|O_NDELAY)) == -1) {
    fprintf(stderr, "stbWin: cannot open fifo for reading\n");
    return -1;
  }
  /* create dummy write end */
  if ((dummy_fifo_fd=stbFifoOpen(moduleId, O_WRONLY)) == -1) {
    fprintf(stderr, "stbWin: cannot open fifo for writing\n");
    return -1;
  }

  /*Get the AVS Module Name */
  AVScommand("kernel","var_get Module",&commandOut,&commandError);
  strcpy(moduleName,commandOut);
  moduleName[strlen(moduleName)-1]=0;

  AVScorout_input(&user,&hn,&hnd,&pn,&pnd,&cn,&posX,&posY,&posZ,&rotX,&rotY,&rotZ,&rotW,&sizeX,&sizeY,&sizeZ,&geomSizeX,&geomSizeY,&geomSizeZ,&geomMoveX,&geomMoveY,&geomMoveZ,&fitGeometry); 

  users=~user;  


  sprintf(buffer,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d",*posX,*posY,*posZ,*sizeX,*sizeY,*sizeZ,*geomSizeX,*geomSizeY,*geomSizeZ,*geomMoveX,*geomMoveY,*geomMoveZ,*rotX,*rotY,*rotZ,*rotW,fitGeometry,users);
  if (cn)
  {
    Debug(printf("Trying Autoconnect %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d\n",*posX,*posY,*posZ,*sizeX,*sizeY,*sizeZ,*geomSizeX,*geomSizeY,*geomSizeZ,*geomMoveX,*geomMoveY,*geomMoveZ,*rotX,*rotY,*rotZ,*rotW,users));      
    Connect(&cn,hn,pn,buffer,TRUE);
  }
  else
  {
    AVSparameter_visible("Host",0); 
    AVSparameter_visible("Port",0);
    AVSparameter_visible("Host:",1); 
    AVSparameter_visible("Port:",1);
  }

  /* put STB-identification on output port (i.e. STBID,window-Id) */
  com_out[0]='\0';
  sprintf(com_out, "%s %d %d %s %d", STB_ID, moduleId, STB_MODULE_WIN, moduleName,cn?1:0);
  strcat(com_out,"!");
  AVScorout_output(com_out);

  /* main loop */
  FOREVER     /* main loop (running 'til connection broken or server shutdown */
  {
    doOutPut=FALSE;

    if (cn)
    {
      /*with Network*/
      FD_ZERO(&myset);
      FD_SET(nc.socket,&myset); 
      FD_SET(fifo_fd, &myset);
      mask=COROUT_WAIT;
      AVScorout_event_wait(getdtablehi(),&myset,NULL,NULL,NULL,&mask); 
    }
    else
    {
      /*without Network*/
      FD_ZERO(&myset);
      FD_SET(fifo_fd, &myset);
      mask=COROUT_WAIT;
      AVScorout_event_wait(getdtablehi(),&myset,NULL,NULL,NULL,&mask); 
    }

    if (cn)
    {
      Debug(printf("stbWin(%d): is TCP/IP packet available?\n", moduleId));
      if (stbNCReceiveMsgNoWait(&nc,&msg))
      {
        NumNoMsgs=0;
        Debug(printf("stbWin(%d): received message via TCP/IP\n", moduleId));
        switch (msg.type) 
        {
          case STB_STILLRUNNING:
          {
            stbNCSendCheckConnectionAck(&nc);
          }
          break;
          case STB_SERVERQUIT:
          {
	    Disconnect(&cn,FALSE);
            AVSmodify_parameter("connect",AVS_VALUE,0,0,0);

            /*tell all stbModules that i'm no longer conected*/
            com_out[0]='\0';
            sprintf(com_out, "%s %d %d %s %d", STB_ID, moduleId, STB_MODULE_WIN, moduleName,cn?1:0);
            strcat(com_out,"!");
            doOutPut=TRUE;
          }
          break;
          case STBFIFO_UPDATE_MODULE:
          {
            if (msg.moduleId==moduleId)
            {
              /* Its my Update!!! */
              Debug(printf("Update %s\n",msg.data));
              sscanf(msg.data,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",posX,posY,posZ,sizeX,sizeY,sizeZ,geomSizeX,geomSizeY,geomSizeZ,geomMoveX,geomMoveY,geomMoveZ,rotX,rotY,rotZ,rotW);    
              AVSmodify_parameter("Pos x",AVS_VALUE,posX,0,0);
              AVSmodify_parameter("Pos y",AVS_VALUE,posY,0,0);
              AVSmodify_parameter("Pos z",AVS_VALUE,posZ,0,0);

              AVSmodify_parameter("Rot x",AVS_VALUE,rotX,0,0);
              AVSmodify_parameter("Rot y",AVS_VALUE,rotY,0,0);
              AVSmodify_parameter("Rot z",AVS_VALUE,rotZ,0,0);
              AVSmodify_parameter("Rot w",AVS_VALUE,rotW,0,0);

              AVSmodify_parameter("Size x",AVS_VALUE,sizeX,0,0);
              AVSmodify_parameter("Size y",AVS_VALUE,sizeY,0,0);
              AVSmodify_parameter("Size z",AVS_VALUE,sizeZ,0,0);

              AVSmodify_parameter("GeomSize x",AVS_VALUE,geomSizeX,0,0);
              AVSmodify_parameter("GeomSize y",AVS_VALUE,geomSizeY,0,0);
              AVSmodify_parameter("GeomSize z",AVS_VALUE,geomSizeZ,0,0);

              AVSmodify_parameter("GeomMove x",AVS_VALUE,geomMoveX,0,0);
              AVSmodify_parameter("GeomMove y",AVS_VALUE,geomMoveY,0,0);
              AVSmodify_parameter("GeomMove z",AVS_VALUE,geomMoveZ,0,0);
            }
            else
            {
              struct stbModule *module;
              Debug(printf("stbWin(%d): Got Update %s\n", moduleId,msg.data));
              if ((module=findModule(msg.moduleId))==NULL)
              {
                Debug(printf("Fatal Error \n"));
	        exit(1);
              }
              else
              {
                strcpy(module->parameter,msg.data);
	        sprintf(buffer, "%d %d ", event_number++,msg.moduleId);
                strcat(com_out, buffer);
                strcat(com_out, module->parameter); /* append event */
                strcat(com_out, "!");
                doOutPut=TRUE;
              }
            }
          }
          break;
        }
	if (msg.data!=NULL)
          free(msg.data);
      }
      else
      {
        if ((NumNoMsgs++)>100)
        {
          if (!stbNCCheckConnection(&nc))
          {
            Disconnect(&cn,TRUE);

            /*tell all stbModules that i'm no longer conected*/
            com_out[0]='\0';
            sprintf(com_out, "%s %d %d %s %d", STB_ID, moduleId, STB_MODULE_WIN, moduleName,cn?1:0);
            strcat(com_out,"!");
            doOutPut=TRUE;
          }
          NumNoMsgs=0;
        } 
      }
    }

    while ((fifo_msg=stbFifoReceive(fifo_fd)) != NULL)   /* message arrived on fifo */
    {
      Debug(printf("stbWin(%d): received message via FIFO from stbModule %d\n", moduleId,fifo_msg->moduleId));      switch (fifo_msg->type) 
      {
      case STBFIFO_REGISTER_MODULE:
        {
          struct stbModule *newModule;
          int newModId,newModType;
          char *tmp;

           /* first store it in the local database....*/
          newModId=fifo_msg->moduleId;

          tmp=fifo_msg->data;
          if (tmp==NULL) printf("Internal Error\n");
          newModType=stbNCGetInt(&tmp);
          Debug(printf("stbWin(%d): STBFIFO_REGISTER_MODULE %d  %d %s\n", moduleId, newModId,newModType,tmp));
          if (findModule(newModId))
          {
            Debug(printf("Internal Error\n"));
          }
          newModule=(struct stbModule *)malloc(sizeof(struct stbModule));
          newModule->moduleId=newModId;
          newModule->moduleType=newModType;  	
          strcpy(newModule->parameter,tmp);  	
	  insertModule(newModule);

          /* if connection is established then update it...*/
          if (cn)
          {
            if (stbNCSendMsg(&nc,fifo_msg)) NumNoMsgs=0;
          }          
        }		
        break;
      case STBFIFO_UNREGISTER_MODULE:        /* !! what if sending unregister while event already there ? */
        {
          /* remove it from the database...*/
          int remModId;
          Debug(printf("stbWin(%d): STBFIFO_UNREGISTER_MODULE\n", moduleId));
         
	  if (!removeModule(fifo_msg->moduleId))
          {
            Debug(printf("Internal Error\n"));
          }          

          /* if connection is established then send the removeMsg...*/
          if (cn)
          {
            if (stbNCSendMsg(&nc,fifo_msg))  NumNoMsgs=0;
          }          

        }
        break;
      case STBFIFO_UPDATE_MODULE:
        {
	  struct stbModule *module;
          Debug(printf("stbWin(%d): STBFIFO_UPDATE_MODULE %d %s\n", moduleId,fifo_msg->moduleId,fifo_msg->data));
	  if ((module=findModule(fifo_msg->moduleId))==NULL)
          {
            Debug(printf("Fatal Error \n"));
	    exit(1);
          }
          strcpy(module->parameter,fifo_msg->data);
          if (cn)
          {
            if (stbNCSendMsg(&nc,fifo_msg))  NumNoMsgs=0;
          }  
        }
	break;
      case STBFIFO_GEOM_MODULE:
        {
          Debug(printf("stbWin(%d): STBFIFO_GEOM_MODULE\n",moduleId)); 
          if (cn)
          {
            if (stbNCSendMsg(&nc,fifo_msg))  NumNoMsgs=0;
          }          
          Debug(printf("stbWin(%d): sending Geom from %d\n",moduleId,fifo_msg->moduleId));	
        }
        break;
      case STBFIFO_EVENT_HANDLED:
        {
          int eventNo;
          char *tmp;

          Debug(printf("stbWin(%d): STBFIFO_EVENT_HANDLED\n",moduleId));
          tmp=fifo_msg->data;

          /* remove event from outport */
          doOutPut=TRUE;
          RemoveEvent(com_out, stbNCGetInt(&tmp),fifo_msg->moduleId);
        }
        break;
      }
      if (fifo_msg->data!=NULL)
        free(fifo_msg->data);
      free(fifo_msg);
    }


    AVScorout_input(&user,&hn,&hnd,&pn,&pnd,&cn,&posX,&posY,&posZ,&rotX,&rotY,&rotZ,&rotW,&sizeX,&sizeY,&sizeZ,&geomSizeX,&geomSizeY,&geomSizeZ,&geomMoveX,&geomMoveY,&geomMoveZ,&fitGeometry); 

    users=~user; 

    if (AVSparameter_changed("connect"))
    {
      if (cn)
      {
        sprintf(buffer,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d",*posX,*posY,*posZ,*sizeX,*sizeY,*sizeZ,*geomSizeX,*geomSizeY,*geomSizeZ,*geomMoveX,*geomMoveY,*geomMoveZ,*rotX,*rotY,*rotZ,*rotW,fitGeometry,users);
        Debug(printf("connect %s\n",buffer));      

        Connect(&cn,hn,pn,buffer,FALSE);
        /*tell all stbModules that i'm now conected*/
        com_out[0]='\0';
        sprintf(com_out, "%s %d %d %s %d", STB_ID, moduleId, STB_MODULE_WIN, moduleName,cn?1:0);
        strcat(com_out,"!");

	doOutPut=TRUE;	/*do an output, that all modules will run the compute routine*/
      }
      else
      {
        Disconnect(&cn,TRUE);

        /*tell all stbModules that i'm no longer conected*/
        com_out[0]='\0';
        sprintf(com_out, "%s %d %d %s %d", STB_ID, moduleId, STB_MODULE_WIN, moduleName,cn?1:0);
        strcat(com_out,"!");
        doOutPut=TRUE;
      }
    }

    if ((cn) && (!AVSparameter_changed("connect")))
    {
      if ((AVSparameter_changed("Pos x") || AVSparameter_changed("Pos y") || AVSparameter_changed("Pos z")) ||
          (AVSparameter_changed("Rot x") || AVSparameter_changed("Rot y") || AVSparameter_changed("Rot z") || AVSparameter_changed("Rot w")) ||
          (AVSparameter_changed("Size x") || AVSparameter_changed("Size y") || AVSparameter_changed("Size z")) ||
          (AVSparameter_changed("GeomSize x") || AVSparameter_changed("GeomSize y") || AVSparameter_changed("GeomSize z")) ||
          (AVSparameter_changed("GeomMove x") || AVSparameter_changed("GeomMove y") || AVSparameter_changed("GeomMove z")) ||
          (AVSparameter_changed("fit Geometry")) ||
          (AVSinput_changed("user input", 0)) )
      {
        sprintf(buffer,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d",*posX,*posY,*posZ,*sizeX,*sizeY,*sizeZ,*geomSizeX,*geomSizeY,*geomSizeZ,*geomMoveX,*geomMoveY,*geomMoveZ,*rotX,*rotY,*rotZ,*rotW,fitGeometry,users);
        Debug(printf("update %s\n",buffer));      
        if (stbNCSend(&nc,moduleId,STB_UPDATE_MODULE,strlen(buffer)+1,(unsigned char*) buffer))
        {
          NumNoMsgs=0;
        }
      }
    }

    /*if (AVSparameter_changed("debug")) printModule(); */

    if (doOutPut)
    {
      AVScorout_output(com_out);
    }
  }  /* end forever */

}


/*------------------------------------------------------------------------------
// BP_Coroutine_desc
//
// description Function
// called from the AVS on initialization
// describes the module: Widgets, output, modul-type and name...                */
int BP_Coroutine_desc()
{
  int host_id,port_id,button_id,i;
  int posIdX,posIdY,posIdZ;
  int sizeIdX,sizeIdY,sizeIdZ;
  int rotIdX,rotIdY,rotIdZ,rotIdW;
  int geomSizeIdX,geomSizeIdY,geomSizeIdZ;
  int geomMoveIdX,geomMoveIdY,geomMoveIdZ;
  int fitGeomId;
  char* env;
  char portName[80]=SERVER_PORT, serverName[80]=SERVER_HOST;
  char s[255];


  env=getenv(SERVER_ENV);

  /* change ":" into blank*/
  if (env) {
    i=0;
    while (env[i]) {
      if (env[i]==':') env[i]=' ';
      i++;
    }
    sscanf(env, "%s %s", serverName, portName);
  }

  AVSset_destroy_proc(CleanUp); 

  AVSset_module_name("stbWin", MODULE_DATA);
  AVScreate_input_port("user input","integer", OPTIONAL);

  AVScreate_output_port("Communication Channel out",COMPORT_SPECS);

  host_id=AVSadd_parameter("Host:", "string", serverName, NULL, NULL);
  AVSconnect_widget(host_id, "typein");
  strcpy(s,"Host: ");strcat(s,serverName);
  host_id=AVSadd_parameter("Host", "string", s, NULL, NULL);
  AVSadd_parameter_prop(host_id,"editable","boolean",0);
  AVSadd_parameter_prop(host_id,"layout","string_block","manipulator $Module:Host -xy 10,10");
  AVSconnect_widget(host_id, "text");


  port_id=AVSadd_parameter("Port:", "string", portName, NULL, NULL);
  AVSconnect_widget(port_id, "typein");
  strcpy(s,"Port: ");strcat(s,portName);
  port_id=AVSadd_parameter("Port", "string", s, NULL, NULL);
  AVSadd_parameter_prop(port_id,"editable","boolean",0);
  AVSadd_parameter_prop(port_id,"layout","string_block","manipulator $Module:Port -xy 10,30");
  AVSconnect_widget(port_id, "text"); 

  button_id=AVSadd_parameter("connect", "boolean", 1, NULL, NULL);
  AVSadd_parameter_prop(button_id,"layout","string_block","manipulator $Module:connect -xy 10,50");
  AVSconnect_widget(button_id, "toggle");


  posIdX=AVSadd_float_parameter("Pos x", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  posIdY=AVSadd_float_parameter("Pos y", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  posIdZ=AVSadd_float_parameter("Pos z", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  AVSconnect_widget(posIdX, "typein_real");
  AVSconnect_widget(posIdY, "typein_real");
  AVSconnect_widget(posIdZ, "typein_real");

  rotIdX=AVSadd_float_parameter("Rot x", 1.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  rotIdY=AVSadd_float_parameter("Rot y", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  rotIdZ=AVSadd_float_parameter("Rot z", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  rotIdW=AVSadd_float_parameter("Rot w", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  AVSconnect_widget(rotIdX, "typein_real");
  AVSconnect_widget(rotIdY, "typein_real");
  AVSconnect_widget(rotIdZ, "typein_real");
  AVSconnect_widget(rotIdW, "typein_real");

  sizeIdX=AVSadd_float_parameter("Size x", 0.5, FLOAT_UNBOUND, FLOAT_UNBOUND);
  sizeIdY=AVSadd_float_parameter("Size y", 0.5, FLOAT_UNBOUND, FLOAT_UNBOUND);
  sizeIdZ=AVSadd_float_parameter("Size z", 0.5, FLOAT_UNBOUND, FLOAT_UNBOUND);
  AVSconnect_widget(sizeIdX, "typein_real");
  AVSconnect_widget(sizeIdY, "typein_real");
  AVSconnect_widget(sizeIdZ, "typein_real");

  geomSizeIdX=AVSadd_float_parameter("GeomSize x", 1.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  geomSizeIdY=AVSadd_float_parameter("GeomSize y", 1.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  geomSizeIdZ=AVSadd_float_parameter("GeomSize z", 1.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  AVSconnect_widget(geomSizeIdX, "typein_real");
  AVSconnect_widget(geomSizeIdY, "typein_real");
  AVSconnect_widget(geomSizeIdZ, "typein_real");

  geomMoveIdX=AVSadd_float_parameter("GeomMove x", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  geomMoveIdY=AVSadd_float_parameter("GeomMove y", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  geomMoveIdZ=AVSadd_float_parameter("GeomMove z", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);
  AVSconnect_widget(geomMoveIdX, "typein_real");
  AVSconnect_widget(geomMoveIdY, "typein_real");
  AVSconnect_widget(geomMoveIdZ, "typein_real");

  fitGeomId=AVSadd_parameter("fit Geometry", "oneshot", 0, NULL, NULL);
  AVSconnect_widget(fitGeomId,"oneshot");

/*  Debug(AVSconnect_widget(AVSadd_parameter("debug", "boolean", 0, NULL, NULL), "toggle")); */

  return(1);
}


