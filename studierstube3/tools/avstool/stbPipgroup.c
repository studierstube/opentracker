/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbPipgroup
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    AVS Module which provides to group stb-Input Modules    
//              
//  VERSION:    1.1
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================   */

#include <avs/avs.h>
#include <avs/field.h>
#define NODEBUG

#include "stb.h"
#include "stbFifo.h"


/*------------------------------------------------------------------------------
//static globals....                                                            */
static int moduleId=-1;       /* the ID of the Module */
static int groupId[5];        /* the IDs of the 5 Groups */
static int registered=FALSE;  /* is it currently registered to the stbWin?*/
static int fifo_fd;           /* used to send data to the fifo */
static int parentPipGroup=0;  /* parent pipGroup (if is connected to a pipgroup)*/ 
static int cn;                /* is the stbWin connected to the displayserver?*/
static char moduleName[STB_MAXPARAMETERLEN]; /*my AVS Modulename */


/*------------------------------------------------------------------------------
//some prototypes....                                                           */
int stbPipGroup_init();           /* initialization routine */
int stbPipGroup_destruction();    /* destruction routine */
int stbPipGroup_compute();        /* computation routine */
int stbPipGroup_desc();           /* description function */


/*------------------------------------------------------------------------------
// AVSinit_modules
//
// called by the AVS 
// tells the AVS the pointer to the description function                        */
void AVSinit_modules()
{
  AVSmodule_from_desc(stbPipGroup_desc);
}


/*------------------------------------------------------------------------------
// stbPipGroupRegister
//
// sends a register message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// pipGroup:       the pipGroup or the stbWin's moduleId if it has no pipGroup
// current:        the default group 
// order:          the ordering value (where it should be placed on the PIP)
// usr:            for which users the module should be visible
// name:           help-text                                                          */   
void stbPipGroupRegister(int parent,int current,int order,int usr,char *name)
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;
  char *tmp;

  sprintf(buf, "%d %d %d %d %d %d %d %d %d %s",parent,groupId[0],groupId[1],groupId[2],groupId[3],groupId[4],current,order,usr,name);
  msg.moduleId=moduleId;
  msg.type=STBFIFO_REGISTER_MODULE;
  msg.length= sizeof(int)+strlen(buf)+1; /*moduleType +buf+\0*/
  msg.data=malloc(msg.length);
  tmp=msg.data;
  stbNCPutInt(&tmp,STB_MODULE_PIPGROUP);
  memcpy(tmp, buf, strlen(buf)+1);
  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
  Debug(printf("stbPipGroup: registration sent %d %s\n",msg.moduleId,buf));
  registered=TRUE;
}


/*------------------------------------------------------------------------------
// stbPipGroupUnregister
//
// sends an unregister message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId                                         */
void stbPipGroupUnregister()
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;

  /* send unregister message */
  msg.moduleId=moduleId;
  msg.type=STBFIFO_UNREGISTER_MODULE;
  msg.length= 0;
  msg.data=NULL;
  stbFifoSend(fifo_fd, &msg);
  registered=FALSE;
}


/*------------------------------------------------------------------------------
// stbPipGroupUpdate
//
// sends an update message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// pipGroup:       the pipGroup or the stbWin's moduleId if it has no pipGroup
// current:        the default group 
// order:          the ordering value (where it should be placed on the PIP)
// usr:            for which users the module should be visible
// name:           help-text                                                          */   
void stbPipGroupUpdate(int parent,int current,int order,int usr,char *name)
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;

  sprintf(buf, "%d %d %d %d %d %d %d %d %d %s",parent,groupId[0],groupId[1],groupId[2],groupId[3],groupId[4],current,order,usr,name);
  msg.moduleId=moduleId;
  msg.type=STBFIFO_UPDATE_MODULE;
  msg.length= strlen(buf)+1; /*+buf+\0*/
  msg.data=malloc(msg.length);
  memcpy(msg.data, buf, strlen(buf)+1);
  stbFifoSend(fifo_fd, &msg);
  Debug(printf("stbPipGroup: update sent %d %s\n",msg.moduleId,buf));
  free(msg.data);
}


/*------------------------------------------------------------------------------
// stbPipGroupEventHandled
//
// sends an event-handled message via fifo to the stbWin Module.
// to tell the stbWin Module to remove the event on its output
// uses the global fifo_fd and moduleId                                         
// 
//Parameter:
// eventId:    the id of the event which has to be removed.                     */
void stbPipGroupEventHandled(int eventId)
{
  char *tmp;
  stbFifoMsg msg;
 
  Debug(printf("stbPipGroup: sending acknowledgement"));
  msg.moduleId=moduleId;
  msg.type=STBFIFO_EVENT_HANDLED;
  msg.length=sizeof(int);
  msg.data=malloc(sizeof(int));
  tmp=msg.data;
  stbNCPutInt(&tmp,eventId);

  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
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
// stbPipGroup_desc
//
// description Function
// called from the AVS on initialization
// describes the module: Widgets, output, modul-type and name...                */
int stbPipGroup_desc()		/* description function */
{
  int com_in_port, data_port;
  int min_id, max_id, name_id, order_id, cur_id;

  AVSset_module_name("stbPipGroup", MODULE_DATA);
  com_in_port=AVScreate_input_port("Communication Channel in",COMPORT_SPECS, OPTIONAL);
  AVScreate_input_port("user input","integer", OPTIONAL);

  data_port=AVScreate_output_port("Communication Channel out1", COMPORT_SPECS);
  data_port=AVScreate_output_port("Communication Channel out2", COMPORT_SPECS);
  data_port=AVScreate_output_port("Communication Channel out3", COMPORT_SPECS);
  data_port=AVScreate_output_port("Communication Channel out4", COMPORT_SPECS);
  data_port=AVScreate_output_port("Communication Channel out5", COMPORT_SPECS);

  name_id=AVSadd_parameter("Name:", "string", "stbPipGroup", NULL, NULL);
  order_id=AVSadd_parameter("Order:", "integer", 0, INT_UNBOUND, INT_UNBOUND);
  cur_id = AVSadd_parameter("select", "choice", "group1", "group1:group2:group3:group4:group5", ":");
  AVSconnect_widget(name_id, "typein");
  AVSconnect_widget(order_id, "typein_integer");

  AVSset_compute_proc(stbPipGroup_compute);
  AVSset_init_proc(stbPipGroup_init);
  AVSset_destroy_proc(stbPipGroup_destruction);
  AVSconnect_widget(cur_id, "radio_buttons");

  return 0;
} 


/*------------------------------------------------------------------------------
// stbPipGroup_init
//
// init function
// called from the AVS on initialization
// nothing to be done                                                           */
int stbPipGroup_init() 
{
  return TRUE;
}


/*------------------------------------------------------------------------------
// stbPipGroup_destruction
//
// destruction function
// called from the AVS on removing the module from the network
// sends an unregister to the stbWin and closes the fifo                        */
int stbPipGroup_destruction() 
{
  stbFifoMsg msg;

  stbPipGroupUnregister();
  stbFifoClose(fifo_fd);
  return 1;
}


/*------------------------------------------------------------------------------
// stbPipGroup_compute
//
// computation function
// called from the AVS when an input or widget has changed...
// the main function of the stbLine AVS Module                            
//
//Parameters:
// widget and input values from the AVS                                         */
int stbPipGroup_compute(char *com_in, int user,char **com_out0,char **com_out1,char **com_out2,char **com_out3,char **com_out4, char *name,int order, char *selectedGroup) 
{
  char *com_in_ptr;	
  int stbId;
  static int oldStbId=-1;
  static int oldStbPipGroup=-1;
  char *p;
  int event_id, dest_id,remove_event_id;
  static int last_event_id=0;
  int markunchanged=TRUE;
  int registrationSended=FALSE;
  char buf[STB_MAXPARAMETERLEN];
  char inParam[1024];
  char conModuleName[256];
  int conModuleType,conModuleParam;
  char *commandOut,*commandError;
  char outBuffer[5][1024];
  int i;
  char** com_outs[5];
  int sel;
  int users;
	
  com_outs[4]=com_out0;
  com_outs[3]=com_out1;
  com_outs[2]=com_out2;
  com_outs[1]=com_out3;
  com_outs[0]=com_out4;

  sel=selectedGroup[5]-'1';
  /*First Run? */
  if (moduleId==-1)
  {
    moduleId=AVSget_unique_id();
    groupId[0]=moduleId;
    for(i=1;i<5;i++) 
    {
      groupId[i]=AVSget_unique_id();
    }
    Debug(printf("stbPipGroup: moduleId %d\n", moduleId));

    cn=FALSE;

    /*Get the AVS Module Name */
    AVScommand("kernel","var_get Module",&commandOut,&commandError);
    strcpy(moduleName,commandOut); 
    moduleName[strlen(moduleName)-1]=0;

    markunchanged=FALSE;
  }

  Debug(printf("stbPipGroup_compute() invoked\n"));
  

  users=~user;

  if (com_in==NULL)
  {
    inParam[0]=0;
    if (registered) 
    {
      stbPipGroupUnregister();
      stbId=0;oldStbId=0;
      for (i=0;i<5;i++)
      {
        sprintf(outBuffer[i], "%s %d %d %s %d %d",buf, stbId, STB_MODULE_PIPGROUP, moduleName, cn ,0);    
        *(com_outs[i])=outBuffer[i];
      }

    }      
    return 1;
  }
  else
  {
    p=strchr(com_in,'!');
    if (p==NULL) 
      inParam[0]=0;
    else
      strcpy(inParam,p);
  }

  if (AVSinput_changed("Communication Channel in", 0) || (!registered)) 
  {
    /* check for STB Kennung  */
    sscanf(com_in, "%s %d %d %s %d %d",buf, &stbId, &conModuleType, conModuleName, &cn, &conModuleParam);
    if (strcmp(buf,STB_ID)!=0)
    {
      PortDisconnect(moduleName,":0");
      AVSerror("Module is not a STB Connection Module"); 
      return 0;
    }
    oldStbPipGroup=parentPipGroup;
    if (conModuleType==STB_MODULE_PIPGROUP)
    {
      parentPipGroup=conModuleParam;
    }
    else
    {
      parentPipGroup=stbId;
    }
    Debug(printf("stbPipGroup: stbWin id is %d\n", stbId));
    if ( (oldStbId != stbId) || (!registered) )
    {
      oldStbId=stbId;
      if (registered) 
      {
        stbPipGroupUnregister();
      }
      if (stbId!=0)
      { 
        /* Create write end of fifo */
        Debug(printf("stbPipGroup: about to create write end of fifo\n"));
        if (!stbFifoCreate(stbId)) 
        {
          fprintf(stderr, "stbPipGroup: can't create fifo\n");
          exit(1);
        }
        if ((fifo_fd=stbFifoOpen(stbId, MODE_WRITE)) == -1) 
        {
          fprintf(stderr, "stbPipGroup: can't open fifo for writing\n");
          exit(1);
        }
        stbPipGroupRegister(parentPipGroup,sel,order,users,name);
        registrationSended=TRUE;
      }
    }
    markunchanged=FALSE;
  }


  if (AVSinput_changed("Communication Channel in", 0) && (registered))
  {
    p=strtok(com_in, "!");
    if (p!=NULL) 
    {
      p=strtok(NULL, "!");
      remove_event_id=-1;
      while (p != NULL) 
      {
        sscanf(p, "%d %d", &event_id, &dest_id);
        if (moduleId == dest_id && event_id>last_event_id) 
        {       /* it's for me ! */
          int dummy, newVal;
          char groupText[]="group1";
	  last_event_id=event_id;
	  remove_event_id=event_id;
          markunchanged=FALSE;
          sscanf(p, "%d %d %d %d %d %d %d %d %d %d",&dummy,&dummy,&dummy, &dummy, &dummy, &dummy,&dummy, &sel, &dummy, &dummy);
          groupText[5]='1'+sel;
	  AVSmodify_parameter("select",AVS_VALUE, groupText);
        }
        /* next token */
        p=strtok(NULL, "!");
      }
      if (remove_event_id!=-1)
      {
        /* send acknowledgment to stbWin */
        /* from the last event for speedup */
        stbPipGroupEventHandled(remove_event_id);
      }
    }
  }



  if (AVSparameter_changed("Name:") || AVSparameter_changed("Order:") ||
      AVSparameter_changed("select")||(oldStbPipGroup!=parentPipGroup) ||
      AVSinput_changed("user input", 0)) 
  {
    if ((registered) && (registrationSended==FALSE) )
    {
      stbPipGroupUpdate(parentPipGroup,sel,order,users,name);
    }
    markunchanged=FALSE;
  }

  if (markunchanged) 
  {
    AVSmark_output_unchanged("Communication Channel out1");
    AVSmark_output_unchanged("Communication Channel out2");
    AVSmark_output_unchanged("Communication Channel out3");
    AVSmark_output_unchanged("Communication Channel out4");
    AVSmark_output_unchanged("Communication Channel out5");
  }
  else
  {
    for (i=0;i<5;i++)
    {
      sprintf(outBuffer[i], "%s %d %d %s %d %d%s",buf, stbId, STB_MODULE_PIPGROUP, moduleName, cn, groupId[i],inParam);
      *(com_outs[i])=outBuffer[i];
    }
  }

  return(1);
}
    

