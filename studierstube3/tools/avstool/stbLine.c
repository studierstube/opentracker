/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbLine
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    AVS Module which provides a line in an Geometry-Output    
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
#include "3dvector.h"


/*------------------------------------------------------------------------------
//static globals....                                                            */
static int moduleId=-1;       /* the ID of the Module */
static int registered=FALSE;  /* is it currently registered to the stbWin?*/
static int fifo_fd;           /* used to send data to the fifo */
static int parentPipGroup=0;  /* parent pipGroup (if is connected to a pipgroup)*/ 


/*------------------------------------------------------------------------------
//some prototypes....                                                           */
int stbLine_init();           /* initialization routine */
int stbLine_destruction();    /* destruction routine */
int stbLine_compute();        /* computation routine */
int stbLine_desc();           /* description function */


/*------------------------------------------------------------------------------
// AVSinit_modules
//
// called by the AVS 
// tells the AVS the pointer to the description function                        */
void AVSinit_modules()
{
  AVSmodule_from_desc(stbLine_desc);
}


/*------------------------------------------------------------------------------
// stbLineRegister
//
// sends a register message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// pipGroup:       the pipGroup or the stbWin's moduleId if it has no pipGroup
// fromX:        \
// fromY:         |-   the starting 3D coordinate of the line
// fromZ:        /
// toX:          \
// toY:           |-   the end 3D coordinate of the line
// toZ:          /
// order:          the ordering value (where it should be placed on the PIP)
// usr:            for which users the module should be visible
// withTexture:    does the activatebutton have a texture?
// textureFileName:filename of the texture if the activatebutton has one.
// name:           help-text                                                          */   
void stbLineRegister(int pipGroup,float fromX,float fromY,float fromZ,float toX,float toY,float toZ,int order,int usr,int withTexture,char *textureFileName,char *name)
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;
  char *tmp;

  if ((withTexture) && (textureFileName==NULL)) withTexture=FALSE;
  if ((withTexture) && (strlen(textureFileName)<1)) withTexture=FALSE;
 
  if (withTexture)
  {
    char tmp[256];
    strcpy(tmp,textureFileName);
    strChrReplace(tmp,' ',255);
    sprintf(buf, "%d %f %f %f %f %f %f %d %d Y %s %s",pipGroup,fromX,fromY,fromZ,toX,toY,toZ,order,usr,tmp,name);
  }
  else 
  {
    sprintf(buf, "%d %f %f %f %f %f %f %d %d N %s",pipGroup,fromX,fromY,fromZ,toX,toY,toZ,order,usr,name);
  }

  msg.moduleId=moduleId;
  msg.type=STBFIFO_REGISTER_MODULE;
  msg.length= sizeof(int)+strlen(buf)+1; /*moduleType +buf+\0*/
  msg.data=malloc(msg.length);
  tmp=msg.data;
  stbNCPutInt(&tmp,STB_MODULE_LINE);
  memcpy(tmp, buf, strlen(buf)+1);
  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
  Debug(printf("stbLine: registration sent %d %s\n",msg.moduleId,buf));
  registered=TRUE;
}


/*------------------------------------------------------------------------------
// stbLineUnregister
//
// sends an unregister message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId                                         */
void stbLineUnregister()
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
// stbLineUpdate
//
// sends an update message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// pipGroup:       the pipGroup or the stbWin's moduleId if it has no pipGroup
// fromX:        \
// fromY:         |-   the starting 3D coordinate of the line
// fromZ:        /
// toX:          \
// toY:           |-   the end 3D coordinate of the line
// toZ:          /
// order:          the ordering value (where it should be placed on the PIP)
// usr:            for which users the module should be visible
// withTexture:    does the activatebutton have a texture?
// textureFileName:filename of the texture if the activatebutton has one.
// name:           help-text                                                          */   
void stbLineUpdate(int pipGroup,float fromX,float fromY,float fromZ,float toX,float toY,float toZ,int order,int usr,int withTexture,char *textureFileName,char *name)
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;

  if ((withTexture) && (textureFileName==NULL)) withTexture=FALSE;
  if ((withTexture) && (strlen(textureFileName)<1)) withTexture=FALSE;

  if (withTexture)
  {
    char tmp[256];
    strcpy(tmp,textureFileName);
    strChrReplace(tmp,' ',255);
    sprintf(buf, "%d %f %f %f %f %f %f %d %d Y %s %s",pipGroup,fromX,fromY,fromZ,toX,toY,toZ,order,usr,tmp,name);
  }
  else 
  {
    sprintf(buf, "%d %f %f %f %f %f %f %d %d N %s",pipGroup,fromX,fromY,fromZ,toX,toY,toZ,order,usr,name);
  }

  msg.moduleId=moduleId;
  msg.type=STBFIFO_UPDATE_MODULE;
  msg.length= strlen(buf)+1; /*+buf+\0*/
  msg.data=malloc(msg.length);
  memcpy(msg.data, buf, strlen(buf)+1);
  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
}


/*------------------------------------------------------------------------------
// stbLineEventHandled
//
// sends an event-handled message via fifo to the stbWin Module.
// to tell the stbWin Module to remove the event on its output
// uses the global fifo_fd and moduleId                                         
// 
//Parameter:
// eventId:    the id of the event which has to be removed.                     */
void stbLineEventHandled(int eventId)
{
  char *tmp;
  stbFifoMsg msg;
 
  Debug(printf("stbLine: sending acknowledgement"));
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
// stbLine_desc
//
// description Function
// called from the AVS on initialization
// describes the module: Widgets, output, modul-type and name...                */
int stbLine_desc()    /* description function */
{
  int stbLine_compute();
  int com_in_port, data_port;
  int fromx_id,fromy_id,fromz_id,tox_id,toy_id,toz_id,name_id,order_id,steps_id;
  int textEnabled_id,textFile_id;

  AVSset_module_name("stbLine",MODULE_DATA);
  com_in_port=AVScreate_input_port("Communication Channel in",COMPORT_SPECS, OPTIONAL);
  AVScreate_input_port("user input","integer", OPTIONAL);
  data_port=AVScreate_output_port("line output","geom");

  name_id=AVSadd_parameter("Name:", "string", "stbLine", NULL, NULL);
  order_id=AVSadd_parameter("Order:", "integer", 0, INT_UNBOUND, INT_UNBOUND);
  textEnabled_id=AVSadd_parameter("with Texture", "boolean", 0, NULL, NULL);
  steps_id=AVSadd_parameter("Steps:", "integer", 1, 1, INT_UNBOUND);

  fromx_id=AVSadd_float_parameter("from X:", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);  
  fromy_id=AVSadd_float_parameter("from Y:", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);  
  fromz_id=AVSadd_float_parameter("from Z:", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);  

  tox_id=AVSadd_float_parameter("to X:", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);  
  toy_id=AVSadd_float_parameter("to Y:", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);  
  toz_id=AVSadd_float_parameter("to Z:", 0.0, FLOAT_UNBOUND, FLOAT_UNBOUND);  

  textFile_id = AVSadd_parameter("Texture:", "string", "", NULL, NULL);

  AVSconnect_widget(name_id, "typein");
  AVSconnect_widget(order_id, "typein_integer");
  AVSconnect_widget(textEnabled_id, "toggle");
  AVSconnect_widget(steps_id, "typein_integer");
  AVSconnect_widget(fromx_id, "typein_real");
  AVSconnect_widget(fromy_id, "typein_real");
  AVSconnect_widget(fromz_id, "typein_real");
  AVSconnect_widget(tox_id, "typein_real");
  AVSconnect_widget(toy_id, "typein_real");
  AVSconnect_widget(toz_id, "typein_real");
  AVSconnect_widget(textFile_id, "browser");

  AVSset_compute_proc(stbLine_compute);
  AVSset_init_proc(stbLine_init);
  AVSset_destroy_proc(stbLine_destruction);
  return 0;
}


/*------------------------------------------------------------------------------
// stbLine_init
//
// init function
// called from the AVS on initialization
// nothing to be done                                                           */
int stbLine_init() 
{
  return TRUE;
}


/*------------------------------------------------------------------------------
// stbLine_destruction
//
// destruction function
// called from the AVS on removing the module from the network
// sends an unregister to the stbWin and closes the fifo                        */
int stbLine_destruction() 
{
  stbFifoMsg msg;

  stbLineUnregister();
  stbFifoClose(fifo_fd);
  return 1;
}


/*------------------------------------------------------------------------------
// stbLine_compute
//
// computation function
// called from the AVS when an input or widget has changed...
// the main function of the stbLine AVS Module                            
//
//Parameters:
// widget and input values from the AVS                                         */
int stbLine_compute(char *com_in, int user, GEOMedit_list *geom_out,char *name, int order,int withTexture,int steps, float *fromX,float *fromY,float *fromZ,float *toX,float *toY,float *toZ,char *textureFileName)
{
  GEOMobj *obj;
  float vec[3], col[3];
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
    Debug(printf("stbLine: moduleId %d\n", moduleId));
    markunchanged=FALSE;
  }

  Debug(printf("stbLine_compute() invoked %d  \n",parentPipGroup));

  users=~user;

  if (withTexture)
  {
    AVSparameter_visible("Texture:",1); 
  }
  else
  {
    AVSparameter_visible("Texture:",0); 
  }


  if (com_in==NULL)
  {
    if (registered) 
    {
      stbLineUnregister();
    }      
  }
  else
  {
    if (AVSinput_changed("Communication Channel in", 0) || (!registered)) 
    {
      /* check for STB ID  */
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
      Debug(printf("stbLine: stbWin id is %d\n", stbId));
      if ((oldStbId != stbId) || (!registered) )
      {
        oldStbId=stbId;
        if (registered) 
        {
          stbLineUnregister();
        }
        if (stbId!=0)
        { 
          /* Create write end of fifo */
          Debug(printf("stbLine: about to create write end of fifo\n"));
          if (!stbFifoCreate(stbId)) 
          {
            fprintf(stderr, "stbLine: can't create fifo\n");
            exit(1);
          }
          if ((fifo_fd=stbFifoOpen(stbId, MODE_WRITE)) == -1) 
          {
            fprintf(stderr, "stbLine: can't open fifo for writing\n");
            exit(1);
          }
          stbLineRegister(parentPipGroup,*fromX,*fromY,*fromZ,*toX,*toY,*toZ,order,users,withTexture,textureFileName,name);
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
          if ((moduleId == dest_id) && (event_id>last_event_id) )
          {       /* it's for me ! */
            int dummy;
            float newFromPosX,newFromPosY,newFromPosZ,newToPosX,newToPosY,newToPosZ;
            last_event_id=event_id;
            markunchanged=FALSE;
            remove_event_id=event_id;
            sscanf(p, "%d %d %d %f %f %f %f %f %f", &dummy, &dummy, &dummy, &newFromPosX, &newFromPosY, &newFromPosZ, &newToPosX, &newToPosY, &newToPosZ);
            AVSmodify_parameter("from X:",AVS_VALUE | AVS_RECORD_VALUE, &newFromPosX, 0,0);
            AVSmodify_parameter("from Y:",AVS_VALUE | AVS_RECORD_VALUE, &newFromPosY, 0,0);
            AVSmodify_parameter("from Z:",AVS_VALUE | AVS_RECORD_VALUE, &newFromPosZ, 0,0);
            AVSmodify_parameter("to X:",AVS_VALUE | AVS_RECORD_VALUE, &newToPosX, 0,0);
            AVSmodify_parameter("to Y:",AVS_VALUE | AVS_RECORD_VALUE, &newToPosY, 0,0);
            AVSmodify_parameter("to Z:",AVS_VALUE | AVS_RECORD_VALUE, &newToPosZ, 0,0);
          
            *fromX=newFromPosX;
            *fromY=newFromPosY;
            *fromZ=newFromPosZ;
            *toX=newToPosX;
            *toY=newToPosY;
            *toZ=newToPosZ;
          }
          /* next token */
          p=strtok(NULL, "!");
        }
        if (remove_event_id!=-1)
        {
          /* send acknowledgment to stbWin */
          /* from the last event for speedup */
          stbLineEventHandled(remove_event_id);
          markunchanged=FALSE;
        }
  
      }
    }
  }
  if (AVSparameter_changed("from X:") || AVSparameter_changed("from Y:") ||
      AVSparameter_changed("from Z:") || AVSparameter_changed("Name:") ||
      AVSparameter_changed("to X:") || AVSparameter_changed("to Y:") ||
      AVSparameter_changed("to Z:") || 
      AVSparameter_changed("with Texture") || AVSparameter_changed("Texture:", 0) ||
      AVSparameter_changed("Order:") || AVSinput_changed("user input", 0) ||
     (oldStbPipGroup!=parentPipGroup) )
  {
    if (registered) 
    {
      stbLineUpdate(parentPipGroup,*fromX,*fromY,*fromZ,*toX,*toY,*toZ,order,users,withTexture,textureFileName,name);
    }
    markunchanged=FALSE;
  }

  if (markunchanged) 
  {
    AVSmark_output_unchanged("line output");
  }
  else
  {
    float *vertices, *colors;
    struct vector vp, vq, dummyvec;
    int i;

    if ((vertices=malloc(sizeof(float)*3*(steps+1)))==NULL) 
    {
      AVSerror("Out of memory while allocating vertex buffer\n ");
      return(0);
    }
    if ((colors=malloc(sizeof(float)*3*(steps+1)))==NULL) 
    {	
      AVSerror("Out of memory while allocating color buffer\n ");
      return(0);
    }
    vp.x=*fromX;
    vp.y=*fromY;
    vp.z=*fromZ;
    vq.x=*toX;
    vq.y=*toY;
    vq.z=*toZ;

    VecSub(vp, vq, &dummyvec);
    Skalar(&dummyvec, ((float)1/steps));
    VecMove(dummyvec, &vp);
    for(i=0;i<=steps;i++)
    {
      vertices[i*3]=vq.x;
      vertices[i*3+1]=vq.y;
      vertices[i*3+2]=vq.z;
      colors[i*3]=1.0;
      colors[i*3+1]=1.0;
      colors[i*3+2]=0.0;
      VecAdd(vq, vp, &vq);
    }

    
    /* init geometry output */
    obj = GEOMcreate_obj(GEOM_POLYTRI, GEOM_NULL);
    GEOMadd_polyline(obj, vertices, colors, steps+1, GEOM_COPY_DATA);
    *geom_out = GEOMinit_edit_list(*geom_out);
    GEOMedit_geometry(*geom_out,"initialTimeLine",obj);
    GEOMdestroy_obj(obj);

    free(vertices);
    free(colors);
  }

  return(1);
}
