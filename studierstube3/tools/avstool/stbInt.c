/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbInt
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    AVS Module which provides an integer output    
//              which can be controlled via the Studierstube display-server
//              or direct within the AVS.
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
#include <avs/geom.h>

#define NODEBUG
#include "stb.h"
#include "stbFifo.h"

/*------------------------------------------------------------------------------
//static globals....                                                            */
static int moduleId=-1;       /* the ID of the Module */
static int registered=FALSE;  /* is it currently registered to the stbWin?*/
static int fifo_fd;           /* used to send data to the fifo */
static int parentPipGroup=0;  /* parent pipGroup (if is connected to a pipgroup)*/


/*------------------------------------------------------------------------------
//some prototypes....                                                           */
int stbInt_init();            /* initialization routine */
int stbInt_destruction();     /* destruction routine */
int stbInt_compute();         /* computation routine */
int stbInt_desc();            /* description function */


/*------------------------------------------------------------------------------
// AVSinit_modules
//
// called by the AVS 
// tells the AVS the pointer to the description function                        */
void AVSinit_modules()
{
  AVSmodule_from_desc(stbInt_desc);
}


/*------------------------------------------------------------------------------
// stbIntRegister
//
// sends a register message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// pipGroup: the pipGroup or the moduleId of the stbWin if it has no pipGroup
// min:      the min-integer value at the lowest slider(dial) position
// max:      the max-integer value at the highest slider(dial) position
// val:      the integer Value 
// order:    the ordering value (where it should be placed on the PIP)
// curVis:   'D' for Dial or 'S' for Slider 
// usr:      for which users the module should be visible
// name:     help-text                                                          */   
void stbIntRegister(int pipGroup,int min,int max,int val,int order,char curVis,int usr,char *name)
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;
  char *tmp;

  sprintf(buf, "%d %d %d %d %d %c %d %s",pipGroup,min,max,val,order,curVis,usr,name);
  msg.moduleId=moduleId;
  msg.type=STBFIFO_REGISTER_MODULE;
  msg.length= sizeof(int)+strlen(buf)+1; /*moduleType +buf+\0*/
  msg.data=malloc(msg.length);
  tmp=msg.data;
  stbNCPutInt(&tmp,STB_MODULE_INT);
  memcpy(tmp, buf, strlen(buf)+1);
  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
  Debug(printf("stbInt: registration sent %d %s\n",msg.moduleId,buf));
  registered=TRUE;
}


/*------------------------------------------------------------------------------
// stbIntUnregister
//
// sends an unregister message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId                                         */
void stbIntUnregister()
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;

  /* send unregister message  */
  msg.moduleId=moduleId;
  msg.type=STBFIFO_UNREGISTER_MODULE;
  msg.length= 0;
  msg.data=NULL;
  stbFifoSend(fifo_fd, &msg);
  registered=FALSE;
}


/*------------------------------------------------------------------------------
// stbIntUpdate
//
// sends an update message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// pipGroup: the pipGroup or the moduleId of the stbWin if it has no pipGroup
// min:      the min-integer value at the lowest slider(dial) position
// max:      the max-integer value at the highest slider(dial) position
// val:      the integer Value 
// order:    the ordering value (where it should be placed on the PIP)
// curVis:   'D' for Dial or 'S' for Slider 
// usr:      for which users the module should be visible
// name:     help-text                                                          */   
void stbIntUpdate(int pipGroup,int min,int max,int val,int order,char curVis,int usr,char *name)
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;

  sprintf(buf, "%d %d %d %d %d %c %d %s",pipGroup,min,max,val,order,curVis,usr,name);
  msg.moduleId=moduleId;
  msg.type=STBFIFO_UPDATE_MODULE;
  msg.length= strlen(buf)+1; /*+buf+\0*/
  msg.data=malloc(msg.length);
  memcpy(msg.data, buf, strlen(buf)+1);
  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}


/*------------------------------------------------------------------------------
// stbIntEventHandled
//
// sends an event-handled message via fifo to the stbWin Module.
// to tell the stbWin Module to remove the event on its output
// uses the global fifo_fd and moduleId                                         
// 
//Parameter:
// eventId:    the id of the event which has to be removed.                     */
void stbIntEventHandled(int eventId)
{
  char *tmp;
  stbFifoMsg msg;
 
  Debug(printf("stbInt sending acknowledgement"));
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
// stbInt_desc
//
// description Function
// called from the AVS on initialization
// describes the module: Widgets, output, modul-type and name...                */
int stbInt_desc()		/* description function */
{
  int com_in_port, data_port;
  int min_id, max_id, name_id, order_id, value_id, vis_id, slider_id, dial_id;

  AVSset_module_name("stbInt", MODULE_DATA);
  com_in_port=AVScreate_input_port("Communication Channel in",COMPORT_SPECS, OPTIONAL);
  AVScreate_input_port("user input","integer", OPTIONAL);

  data_port=AVScreate_output_port("integer output", "integer");
  min_id=AVSadd_parameter("Min.:", "integer", 0, INT_UNBOUND, INT_UNBOUND);
  max_id=AVSadd_parameter("Max.:", "integer", 99, INT_UNBOUND, INT_UNBOUND);
  name_id=AVSadd_parameter("Name:", "string", "stbInt", NULL, NULL);
  order_id=AVSadd_parameter("Order:", "integer", 0, INT_UNBOUND, INT_UNBOUND);
  value_id=AVSadd_parameter("Wert:", "integer", 0, 0, 99);
  vis_id = AVSadd_parameter("select", "choice", "slider", "slider:dial", ":");
  slider_id=AVSadd_parameter("slider", "integer", 0, 0, 99);
  dial_id=AVSadd_parameter("dial", "integer", 0, 0, 99);  


  AVSconnect_widget(min_id, "typein_integer");
  AVSconnect_widget(max_id, "typein_integer");
  AVSconnect_widget(name_id, "typein");
  AVSconnect_widget(order_id, "typein_integer");
  AVSconnect_widget(value_id, "typein_integer");
  AVSconnect_widget(vis_id, "radio_buttons");
  AVSconnect_widget(slider_id, "islider");
  AVSconnect_widget(dial_id, "idial");

  AVSadd_parameter_prop(vis_id,"layout","string_block","manipulator $Module:select -xy 69,128 -wh 118,44");
  AVSadd_parameter_prop(slider_id,"layout","string_block","manipulator $Module:slider -xy 10,180 -wh 236,32");
  AVSadd_parameter_prop(dial_id,"layout","string_block","manipulator $Module:dial -hide -xy 81,180 -wh 90,130");


  AVSset_compute_proc(stbInt_compute);
  AVSset_init_proc(stbInt_init);
  AVSset_destroy_proc(stbInt_destruction);

  return 0;
} 


/*------------------------------------------------------------------------------
// stbInt_init
//
// init function
// called from the AVS on initialization
// nothing to be done                                                           */
int stbInt_init() 
{
  return TRUE;
}


/*------------------------------------------------------------------------------
// stbInt_destruction
//
// destruction function
// called from the AVS on removing the module from the network
// sends an unregister to the stbWin and closes the fifo                        */
int stbInt_destruction() 
{
  stbFifoMsg msg;

  stbIntUnregister();
  stbFifoClose(fifo_fd);
  return 1;
}


/*------------------------------------------------------------------------------
// stbInt_compute
//
// computation function
// called from the AVS when an input or widget has changed...
// the main function of the stbInt AVS Module                            
//
//Parameters:
// widget and input values from the AVS                                         */
int stbInt_compute(char *com_in, int user, int *int_out, int min, int max, char *name, int order, int val, char *vis, int sliderVal, int dialVal) 
{
  char *com_in_ptr;	
  int stbId;
  static int oldStbId=-1;
  static int oldStbPipGroup=-1;
  char *p;
  int event_id, dest_id,remove_event_id;
  static int last_event_id=0;
  int markunchanged=TRUE;
  char buf[STB_MAXPARAMETERLEN];
  char conModuleName[256];
  char curVis;
  int conModuleType,conModuleParam;
  int users;
  int dummy;
	
  /*First Run? */
  if (moduleId==-1)
  {
    moduleId=AVSget_unique_id();
    Debug(printf("stbInt: moduleId %d\n", moduleId));
    /*AVSparameter_visible("dial",0); */
    markunchanged=FALSE;
  }

  Debug(printf("stbInt_compute() invoked %d  \n",parentPipGroup));

  users=~user;    

  if (strcmp(vis,"slider")==0) 
    curVis='S';
  else 
    curVis='D';

  if (AVSparameter_changed("select"))
  {
    if (curVis=='D')
    {
      AVSparameter_visible("dial",1);
      AVSparameter_visible("slider",0);
    }
    if (curVis=='S')
    {
      AVSparameter_visible("dial",0);
      AVSparameter_visible("slider",1);
    }
  }

  if (AVSparameter_changed("Wert:"))
  {
    AVSmodify_parameter("dial",AVS_VALUE,val,0,0);
    AVSmodify_parameter("slider",AVS_VALUE,val,0,0);
    markunchanged=FALSE;
  }
  if (AVSparameter_changed("dial"))
  {
    AVSmodify_parameter("Wert:",AVS_VALUE,dialVal,0,0);
    AVSmodify_parameter("slider",AVS_VALUE,dialVal,0,0);
    val=dialVal;
    markunchanged=FALSE;
  }
  if (AVSparameter_changed("slider"))
  {
    AVSmodify_parameter("Wert:",AVS_VALUE,sliderVal,0,0);
    AVSmodify_parameter("dial",AVS_VALUE,sliderVal,0,0);
    val=sliderVal;
    markunchanged=FALSE;
  }

  if (com_in==NULL)
  {
    if (registered) 
    {
      stbIntUnregister();
    }      
  }
  else
  {
    if (AVSinput_changed("Communication Channel in", 0) || (!registered)) 
    {
      /* check for stb Kennung  */
      sscanf(com_in, "%s %d %d %s %d %d",buf, &stbId, &conModuleType, conModuleName, &dummy, &conModuleParam);
      if (strcmp(buf,STB_ID)!=0)
      {
        PortDisconnect(NULL,":0");
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
      Debug(printf("stbInt: stbWin id is %d\n", stbId));
      if ((oldStbId != stbId) || (!registered) )
      {
        oldStbId=stbId;
        if (registered) 
        {
          stbIntUnregister();
        }
        if (stbId!=0)
        { 
          /* Create write end of fifo */
          Debug(printf("stbInt: about to create write end of fifo\n"));
          if (!stbFifoCreate(stbId)) 
          {
            fprintf(stderr, "stbInt: can't create fifo\n");
            exit(1);
          }
          if ((fifo_fd=stbFifoOpen(stbId, MODE_WRITE)) == -1) 
          {
            fprintf(stderr, "stbInt: can't open fifo for writing\n");
            exit(1);
          }
          stbIntRegister(parentPipGroup,min,max,val,order,curVis,users,name);
        }
      }
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
            last_event_id=event_id;
            remove_event_id=event_id;
            markunchanged=FALSE;
            sscanf(p, "%d %d %d %d %d %d", &dummy, &dummy, &dummy, &dummy, &dummy, &newVal);
            AVSmodify_parameter("Wert:",AVS_VALUE | AVS_MINVAL | AVS_MAXVAL | AVS_RECORD_VALUE, newVal, min,max);
            AVSmodify_parameter("slider",AVS_VALUE | AVS_MINVAL | AVS_MAXVAL, newVal, min,max);
            AVSmodify_parameter("dial",AVS_VALUE | AVS_MINVAL | AVS_MAXVAL, newVal, min,max);
            val=newVal;
          }
          /* next token */
          p=strtok(NULL, "!");
        }
        if (remove_event_id!=-1)
        {
          /* send acknowledgment to stbWin */
          /* from the last event for speedup */
          stbIntEventHandled(remove_event_id);
        }
      }
    }
  }
  
  if (AVSparameter_changed("Min.:") || AVSparameter_changed("Max.:") ||
      AVSparameter_changed("Wert:") || AVSparameter_changed("Name:") ||
      AVSparameter_changed("slider") || AVSparameter_changed("dial") ||
      AVSparameter_changed("Order:") || AVSparameter_changed("select") ||
     (oldStbPipGroup!=parentPipGroup) || AVSinput_changed("user input", 0))
  {
    AVSmodify_parameter("Wert:",AVS_MINVAL|AVS_MAXVAL, val, min,max);
    AVSmodify_parameter("slider",AVS_MINVAL|AVS_MAXVAL, val, min,max);
    AVSmodify_parameter("dial",AVS_MINVAL|AVS_MAXVAL, val, min,max);
    if (registered) 
    {
      stbIntUpdate(parentPipGroup,min,max,val,order,curVis,users,name);
    }
    markunchanged=FALSE;
  }

  if (markunchanged) 
  {
    AVSmark_output_unchanged("integer output");
  }
  else
  {
    *int_out=val;
  }

  return(1);
}
    

