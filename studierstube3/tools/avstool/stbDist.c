/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbDist
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    AVS Module which distributes stbWin Modules...
//              it does react like a stbWin Modul, and distributes all events
//              to two stbWin Modules.
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

#define numOfStbInputs 2


/*------------------------------------------------------------------------------
//static globals....                                                            */
static int moduleId;               /* the ID of the Module */
static int fifo_fd,dummy_fifo_fd;  /* used to receive data from the fifo */


/*------------------------------------------------------------------------------
//prototypes (needed for forward declaration)                                   */
void PortDisconnect(char *myMod,char *myPort);


/*------------------------------------------------------------------------------
// structure ang globals for the Input ports                                    */
struct stbInput
{
  int connected;
  char *data;
  int cn;
  int stbId;
  int parentPipGroup;
  int fifo_fd;
  int changed;
  int last_event_id;
} stbInputs[2];


/*------------------------------------------------------------------------------
// structure for the module list                                                */
struct stbModule
{
  int moduleId;
  int moduleType;
  char parameter[STB_MAXPARAMETERLEN];
  struct stbModule *next;
} *stbModules;


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
  
  Debug(printf("%d RemoveModule %d\n",moduleId,moduleId));
  p=stbModules;
  pb=NULL;
  while(p!=NULL)
  {
    if (p->moduleId==moduleId)
    {
      if (pb==NULL) 
      {
        stbModules=p->next;
      }
      else
      {
        pb->next=p->next;
      }
      free(p);
      return TRUE;
    }
    pb=p;
    p=p->next;
  }
  return FALSE;
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
// RemoveEventfromEventStr
//
// removes an event from the event string
// the diffrent to the stbWin's RemoveEvent function is that this function 
// works with events at the begining of the string....
//
//Parameter:
// s:        the event (output) string of the stbWin
// moduleId: the id of the module the event has to be removed
// eventnumber: the id of the event wich has to be removed                      */
void RemoveEventfromEventStr(char *s, int eventnumber, int moduleId) 
{
  int eno,mno;
  char *p,*buf;

  buf=(char *) malloc(strlen(s)+1);
  strcpy(buf, "!");

  p=strtok(s, "!"); /* read over first token */
  while (p != NULL) 
  {
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
    p=strtok(NULL, "!");
  }
  strcpy(s, buf);
  free (buf);
}


/*------------------------------------------------------------------------------
// Disconnect
//
// sends an STBFIFO_UNREGISTER_MODULE packewt of all Modules to the stbWin  
//
//Parameter:

// in: pointer which spezifies the AVS input port                               */
void Disconnect(struct stbInput *in)
{
  struct stbModule *mod;
  char buf[1024];
  stbFifoMsg msg;

  if(in->connected==FALSE) return;  /*nothing to do*/

  mod=stbModules;

  while(mod!=NULL)
  {
    msg.moduleId=mod->moduleId;
    msg.type=STBFIFO_UNREGISTER_MODULE;
    msg.length= 0;
    msg.data=NULL;
    stbFifoSend(in->fifo_fd, &msg);
    free(msg.data);
    mod=mod->next;
  }
  stbFifoClose(in->fifo_fd);
  in->connected=FALSE;
}


/*------------------------------------------------------------------------------
// isConnectionForStbIDValid
//
// checks if the input is allready connected...
//
//Parameter:
// in:   the first input Port element, the elements are stepped through with []
// cur:  the input port which has to be checked...
//
//returns:
// TRUE if the connected input is not connected to any other input port         */
int isConnectionForStbIDValid(struct stbInput *in,struct stbInput *cur)
{
  int t=0;

  while (t<numOfStbInputs)
  {
    if ((&(in[t])) !=cur)
    {
      if ((in[t].connected) && (in[t].stbId==cur->stbId))
      {
        return FALSE;
      }
    }
    t++;
  }
  return TRUE;
}


/*------------------------------------------------------------------------------
// Connect
//
// connects to the stbWin
// checks the stbWin's output and the connection....
// opens the FIFO to the stbWin 
// sends an STBFIFO_REGISTER_MODULE of all modules....
//
//Parameter:
// in:          the input port of the stbWin to be connected
// allInputs:   the first input Port element                                    */
void Connect(struct stbInput *in,struct stbInput *allInputs)
{
  struct stbModule *mod;
  char buf[1024];
  stbFifoMsg msg;
  char conModuleName[256];
  int conModuleType,conModuleParam;
  char *tmp;

  if(in->connected==TRUE) return;  /*nothing to do*/
  in->last_event_id=0;
  Debug(printf("stbDIST Connect to stbWin \n"));

  sscanf(in->data, "%s %d %d %s %d %d",buf, &(in->stbId), &conModuleType, conModuleName, &(in->cn), &conModuleParam);

  if ((strcmp(buf,STB_ID)!=0) || (in->stbId <=0 ))
  {
    PortDisconnect(NULL,":0");
    AVSerror("Module is not a STB Connection Module"); 
    return;
  }
  if (!isConnectionForStbIDValid(allInputs,in))
  {
    PortDisconnect(NULL,":0");
    AVSerror("stbWin Module is allready Connected"); 
    return;
  }
  if (conModuleType==STB_MODULE_PIPGROUP)
  {
    in->parentPipGroup=conModuleParam;
  }
  else
  {
    in->parentPipGroup=in->stbId;
  }

  if (in->stbId==0) return;

  /* Create write end of fifo */
  Debug(printf("stbDist: about to create write end of fifo\n"));
  if (!stbFifoCreate(in->stbId)) 
  {
    fprintf(stderr, "stbDist: can't create fifo\n");
    exit(1);
  }
  if ((in->fifo_fd=stbFifoOpen(in->stbId, MODE_WRITE)) == -1) 
  {
    fprintf(stderr, "stbDist: can't open fifo for writing\n");
    exit(1);
  }

  mod=stbModules;

  while(mod!=NULL)
  {
    if (mod->moduleType==STB_MODULE_GEOM)
    {
      strcpy(buf,mod->parameter);
    }
    else
    {
      char *hlp;
      hlp=strchr(mod->parameter,' ');
      sprintf(buf,"%d%s",in->parentPipGroup,hlp);
    }

    msg.moduleId=mod->moduleId;
    msg.type=STBFIFO_REGISTER_MODULE;
    msg.length=strlen(buf)+1+sizeof(int);
    msg.data=(char *)malloc(msg.length);
    tmp=htonl(mod->moduleType);
    memcpy(msg.data, &tmp, sizeof(int));
    memcpy(msg.data+sizeof(int), buf, strlen(buf)+1);
    stbFifoSend(in->fifo_fd, &msg);

    free(msg.data);
    mod=mod->next;
  }
  in->connected=TRUE;
}


/*------------------------------------------------------------------------------
// PortDisconnect
//
// disconnects two Modules within the AVS 
// used when the connected string is not an stb-Outputstring from the stbWin
// 
//Parameter:
// myMod:    the name of the module, if it is NULL it gets the Modulename...
// myPort:   name of the Input Port                                             */
void PortDisconnect(char *myMod,char *myPort)
{
  char moduleName[1024];
  char myModCon[1024];
  char destModCon[1024];
  char compareStr[1024];
  char *findStr,*helpStr,*helpStr2;
  char command[2048];
  char *commandOut,*commandError;


  if (myMod==NULL)
  {
    /*Get the AVS Module Name */
    AVScommand("kernel","var_get Module",&commandOut,&commandError);
    strcpy(moduleName,commandOut);
    moduleName[strlen(moduleName)-1]=0;
    myMod=moduleName;
  }
  
  strcpy(myModCon,myMod);
  strcat(myModCon,myPort);
  

  /*get information about modules and connections */
  AVScommand("kernel","net_show",&commandOut,&commandError); 
  findStr=commandOut;

  /*find out if there are Connections to me and where they are.... */
  while (findStr!=NULL)
  {
    /*first find the connection*/
    findStr=strstr(findStr,"port_connect ");
    if (findStr!=NULL)
    {
      /*ok, found, then search the begining of the module which is connected to me*/
      findStr+=strlen("port_connect ");
      helpStr=strchr(findStr,' ');
      if ((helpStr2=strchr(findStr,'"'))!=NULL)
      {
        if (helpStr2<helpStr)
        {
          if ((helpStr2=strchr(helpStr2+1,'"'))!=NULL)
          {
            if (helpStr2>helpStr)
            {
              helpStr=strchr(helpStr2,' ');
            }
          }
        }
      }

      if (helpStr!=0)
      {
        (*helpStr)=0;
        strcpy(destModCon,findStr);
      }
      findStr=helpStr;
      if (findStr!=NULL) findStr++;

      /*and copy it to the compareStr */       
      if ((*findStr)!=0) 
      {
        helpStr=compareStr;
        while ( ((*findStr)!=10) && ((*findStr)!=0) )
        {
          (*(helpStr++))=(*(findStr++));
        }
        *helpStr=0;
        if ((*findStr)!=0) findStr++;
        /*and now do the check if it is a connection to me... */
        if (strncmp(compareStr,myModCon,strlen(myModCon))==0)
        {
          /* and now delete the wrong connection....*/

          strcpy(command,"port_disconnect ");
          strcat(command,destModCon);
          strcat(command," ");
          strcat(command,myModCon);
          AVScommand("kernel",command,&commandOut,&commandError); 
          Debug(printf("Disconnecting Modules %s %s\n",destModCon,myModCon));
        }
      } 
    }
  }

}


/*------------------------------------------------------------------------------
// CleanUp
//
// should be called when the stbDist Modul is removed from the network
// or the avs is quited
// but it is not called always, only the devil knows why.....                   */
int CleanUp()
{
  int i;

  Debug(printf("stbDist has quit! (ID: %d)...\n",moduleId));

  stbFifoClose(fifo_fd);
  stbFifoClose(dummy_fifo_fd);
  stbFifoRemove(moduleId);

  /*send the unregister to all stbWins... */
  for (i=0;i<numOfStbInputs;i++)
  {
    Disconnect(&(stbInputs[i]));
  }
  removeAllModules();  

  return 1;
}


/*------------------------------------------------------------------------------
// main
//
// the main AVS coroutine called by the AVS                                      
// this function is running forever!!!                                          */
int main(int argc, char **argv) {
  int BP_Coroutine_desc();
  char com_out[1024],outBuf[1024];
  char buffer[1024];
  int dummy, run=-1;
  /*fd_set myset; */
  int mask=COROUT_WAIT;
  int doOutPut;
  stbFifoMsg *fifoMsg;
  int shutdown_flag;
  int event_number=1;
  stbMessage msg;
  char *in1,*in2;
  int cn,cnold;
  int i;

  fd_set myset;
  STBFIFO_IPC_INFO ipc_info;
  char moduleName[256];
  char *commandOut,*commandError;

  stbModules=NULL;
  AVScorout_init(argc,argv,BP_Coroutine_desc);

  AVScorout_set_sync(1);

  for (i=0;i<numOfStbInputs;i++)
  {
    stbInputs[i].connected=FALSE;
    stbInputs[i].cn=FALSE;
    stbInputs[i].last_event_id=0;
  }


  moduleId=AVSget_unique_id();
  Debug(printf("stbDist: moduleId %d\n", moduleId));

  /* Create read end of fifo */
  if (!stbFifoCreate(moduleId)) {
    fprintf(stderr, "stbDist: cannot create fifo\n");
    return -1;
  }
  if ((fifo_fd=stbFifoOpen(moduleId, O_RDONLY|O_NDELAY)) == -1) {
    fprintf(stderr, "stbDist: cannot open fifo for reading\n");
    return -1;
  }
  /* create dummy write end */
  if ((dummy_fifo_fd=stbFifoOpen(moduleId, O_WRONLY)) == -1) {
    fprintf(stderr, "stbDist: cannot open fifo for writing\n");
    return -1;
  }

  /*Get the AVS Module Name */
  AVScommand("kernel","var_get Module",&commandOut,&commandError);
  strcpy(moduleName,commandOut);
  moduleName[strlen(moduleName)-1]=0;

  AVScorout_input(&(stbInputs[0].data),&(stbInputs[1].data)); 
  stbInputs[0].changed=AVSinput_changed("Communication Channel in1", 0);
  stbInputs[1].changed=AVSinput_changed("Communication Channel in2", 0);


  /*Debug(printf("read end of fifo created and opened"));*/
  /* put STB-identification on output port (i.e. STBID,window-Id) */
  com_out[0]='\0';
  strcpy(outBuf,"!");
  sprintf(com_out, "%s %d %d %s %d%s", STB_ID, moduleId, STB_MODULE_WIN, moduleName,FALSE,outBuf);
  AVScorout_output(com_out);


  /* main loop */
  FOREVER     /* main loop (running 'til connection broken or server shutdown */
  {
    doOutPut=FALSE;

    FD_ZERO(&myset);
    FD_SET(fifo_fd, &myset);
    mask=COROUT_WAIT;
    AVScorout_event_wait(getdtablehi(),&myset,NULL,NULL,NULL,&mask); 

    AVScorout_input(&(stbInputs[0].data),&(stbInputs[1].data)); 

    stbInputs[0].changed=AVSinput_changed("Communication Channel in1", 0);
    stbInputs[1].changed=AVSinput_changed("Communication Channel in2", 0);


    cnold=cn;
    cn=FALSE;
    /*Handle the Input Connection changings*/
    for (i=0;i<numOfStbInputs;i++)
    {
      if (stbInputs[i].data==NULL) 
      {
        if (stbInputs[i].connected==TRUE)
        {
          /*Send disconnects from all Modules to Input */
          Disconnect(&(stbInputs[i]));
          doOutPut=TRUE;
        }
      }
      else
      {
        if (stbInputs[i].connected==FALSE)
        {
          Connect(&(stbInputs[i]),stbInputs);
          doOutPut=TRUE;
        }
        else
        {
          int newStbId;
          char conModuleName[256];
          int conModuleType,conModuleParam;

          /*Parse the stbWin Input String and check for updates....*/
          sscanf(stbInputs[i].data, "%s %d %d %s %d %d",buffer, &newStbId, &conModuleType, conModuleName, &(stbInputs[i].cn), &conModuleParam);
          if (newStbId!=stbInputs[i].stbId)
          {
            Disconnect(stbInputs[i]);
            Connect(stbInputs[i],stbInputs);
            doOutPut=TRUE;
          }
          if (conModuleType==STB_MODULE_PIPGROUP)
          {
            if (stbInputs[i].parentPipGroup!=conModuleParam)
            {
              stbInputs[i].parentPipGroup=conModuleParam;
              doOutPut=TRUE;
            }
          }
          else
          {
            if (stbInputs[i].parentPipGroup!=stbInputs[i].stbId)
            {
              stbInputs[i].parentPipGroup=stbInputs[i].stbId;
              doOutPut=TRUE;
            }
          }

          if (stbInputs[i].changed)
          {  
            /* handle events */
            struct stbModule *module;
            char *p;
            int event_id,dest_id;

            strcpy(buffer,stbInputs[i].data);
            p=strtok(buffer, "!");
            if (p!=NULL) 
            {
              p=strtok(NULL, "!");
              while (p != NULL) 
              {
                sscanf(p, "%d %d", &event_id, &dest_id);
                if ( ((module=findModule(dest_id))!=NULL) && event_id>stbInputs[i].last_event_id) 
                {       
                  int dummy, newVal;
                  char *tmp;
                  stbFifoMsg msg;
                  int t;
                  char buf[STB_MAXPARAMETERLEN];

                  /*first handle the update for me*/
                  stbInputs[i].last_event_id=event_id;
                  memset(module->parameter,0,STB_MAXPARAMETERLEN);
                  sscanf(p, "%d %d %255c", &dummy, &dummy, module->parameter);
                  msg.moduleId=module->moduleId;
                  msg.type=STBFIFO_EVENT_HANDLED;
                  msg.length=sizeof(int);
                  msg.data=malloc(sizeof(int));
                  tmp=msg.data;
                  stbNCPutInt(&tmp,event_id);
                  stbFifoSend(stbInputs[i].fifo_fd, &msg);
                  free(msg.data);

                  /*second handle the update for all other stbWins above */
                  for (t=0;t<numOfStbInputs;t++)
                  {
                    if (t!=i)
                    {
                      if (module->moduleType==STB_MODULE_GEOM)
                      {
                        strcpy(buf,module->parameter);
                      }
                      else
                      {
                        char *hlp;
                        hlp=strchr(module->parameter,' ');
                        sprintf(buf,"%d%s",stbInputs[t].parentPipGroup,hlp);
                      }

                      msg.moduleId=module->moduleId;
                      msg.type=STBFIFO_UPDATE_MODULE;
                      msg.length= strlen(buf)+1; /*+buf+\0*/
                      msg.data=malloc(msg.length);
                      memcpy(msg.data, buf, strlen(buf)+1);
                      stbFifoSend(stbInputs[t].fifo_fd, &msg);
                      free(msg.data);
                    }
                  }                 
                  /*third generate a new event for modules below */
                  sprintf(buf, "%d %d ", event_number++,module->moduleId);
                  if (strlen(outBuf)==0) strcpy(outBuf,"!");
                  strcat(outBuf, buf);
                  strcat(outBuf, module->parameter); /* append event */
                  strcat(outBuf, "!");
                  doOutPut=TRUE;
                }
                /* next token */
                p=strtok(NULL, "!");
              }
            }           
          }         
        } 
        cn+=stbInputs[i].cn; 
      }
    }

    /*Has the number of stbWin connections changed? */
    if (cnold!=cn)
    {
      doOutPut=TRUE;
    }

    sprintf(com_out, "%s %d %d %s %d%s", STB_ID, moduleId, STB_MODULE_WIN, moduleName,cn,outBuf);

    while ((fifoMsg=stbFifoReceive(fifo_fd)) != NULL)   /* message arrived on fifo */
    {
      Debug(printf("stbDist(%d): received message via FIFO from stbModule %d\n", moduleId,fifoMsg->moduleId));
      switch (fifoMsg->type) 
      {
      case STBFIFO_REGISTER_MODULE:
        {
          struct stbModule *newModule;
          int newModId,newModType;
          char *tmp;

           /* first store it in the local database....*/
          newModId=fifoMsg->moduleId;

          tmp=fifoMsg->data;
          if (tmp==NULL) printf("Internal Error\n");
          newModType=stbNCGetInt(&tmp);
          Debug(printf("stbDist(%d): STBFIFO_REGISTER_MODULE %d  %d %s\n", moduleId, newModId,newModType,tmp));
          if (findModule(newModId))
          {
            printf("Internal Error\n");
          }
          newModule=(struct stbModule *)malloc(sizeof(struct stbModule));
          newModule->moduleId=newModId;
          newModule->moduleType=newModType;  	
          strcpy(newModule->parameter,tmp);  	
	  insertModule(newModule);
          /*update the stbWins above.....*/
          for (i=0;i<numOfStbInputs;i++)
          {
            if (stbInputs[i].connected)
            {
              tmp=buffer;
              stbNCPutInt(&tmp,newModule->moduleType);
              if (newModule->moduleType==STB_MODULE_GEOM)
              {
                strcpy(buffer+sizeof(int),newModule->parameter);
              }
              else
              {
                char *hlp;
                hlp=strchr(newModule->parameter,' ');
                sprintf((char *)tmp,"%d%s",stbInputs[i].parentPipGroup,hlp);
              }
              fifoMsg->moduleId=newModule->moduleId;
              fifoMsg->data=buffer;
              fifoMsg->length=strlen(tmp)+sizeof(int)+1;
              stbFifoSend(stbInputs[i].fifo_fd, fifoMsg);
            }          
          }
        }		
        break;
      case STBFIFO_UNREGISTER_MODULE:        /* !! what if sending unregister while event already there ? */
        {
          /* remove it from the database...*/
          int remModId;
          Debug(printf("stbDist(%d): STBFIFO_UNREGISTER_MODULE\n", moduleId));
         
	  if (!removeModule(fifoMsg->moduleId))
          {
            printf("Internal Error\n");
          }          

          /*update the stbWins above.....*/
          for (i=0;i<numOfStbInputs;i++)
          {
            if (stbInputs[i].connected)
            {
              stbFifoSend(stbInputs[i].fifo_fd, fifoMsg);
            }
          }
        }
        break;
      case STBFIFO_UPDATE_MODULE:
        {
	  struct stbModule *module;
          Debug(printf("stbDist(%d): STBFIFO_UPDATE_MODULE\n", moduleId));
	  if ((module=findModule(fifoMsg->moduleId))==NULL)
          {
            Debug(printf("Fatal Error \n"));
	    exit(1);
          }
          if (strcmp(module->parameter,fifoMsg->data)!=0)
          {
            strcpy(module->parameter,fifoMsg->data);
            /*update the stbWins above.....*/
            for (i=0;i<numOfStbInputs;i++)
            {
              if (stbInputs[i].connected)
              {
                if (module->moduleType==STB_MODULE_GEOM)
                {
                  strcpy(buffer+sizeof(int),module->parameter);
                }
                else
                {
                  char *hlp;
                  hlp=strchr(module->parameter,' ');
                  sprintf(buffer+sizeof(int),"%d%s",stbInputs[i].parentPipGroup,hlp);
                }
                fifoMsg->data=buffer;
                fifoMsg->length=strlen(buffer)+1;
                stbFifoSend(stbInputs[i].fifo_fd, fifoMsg);
              }
            }          
          }
        }
	break;
      case STBFIFO_GEOM_MODULE:
        {
          Debug(printf("stbDist(%d): STBFIFO_GEOM_MODULE\n")); 
          /*update the stbWins above.....*/
          for (i=0;i<numOfStbInputs;i++)
          {
            if (stbInputs[i].connected)
            {
              stbFifoSend(stbInputs[i].fifo_fd, fifoMsg);
            }
          }
        }
        break;
      case STBFIFO_EVENT_HANDLED:
        {
          int eventNo;
          char *tmp;

          Debug(printf("stbDist(%d): STBFIFO_EVENT_HANDLED\n"));

          /* remove event from outport */
          doOutPut=TRUE;
          tmp=fifoMsg->data;
          RemoveEventfromEventStr(outBuf, stbNCGetInt(&tmp)); 
          sprintf(com_out, "%s %d %d %s %d%s", STB_ID, moduleId, STB_MODULE_WIN, moduleName,cn,outBuf);
        }
        break;
      }
    }
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

  AVSset_module_name("StbDistributor", MODULE_DATA);

  AVSset_destroy_proc(CleanUp); 

  AVScreate_input_port("Communication Channel in1",COMPORT_SPECS, OPTIONAL);
  AVScreate_input_port("Communication Channel in2",COMPORT_SPECS, OPTIONAL);

  AVScreate_output_port("Communication Channel out",COMPORT_SPECS);

  return(1);
}


