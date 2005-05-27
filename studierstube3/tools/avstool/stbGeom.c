/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbGeom
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    AVS Module which sends the AVS Geometry to the display-server
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

#include <avs/avs.h>
#include <avs/field.h>
#include <avs/geom.h>
#include "stb.h"
#include "stbFifo.h"
#include "stbComm.h"
#include "Geom2SerIV.h"


/*------------------------------------------------------------------------------
//static globals....                                                            */
static int moduleId=-1;       /* the ID of the Module */
static int registered=FALSE;  /* is it currently registered to the stbWin?*/
static int fifo_fd;           /* used to send data to the fifo */
static int stbWinConnected=FALSE; /* is stbWin connected to the displayserver?*/


/*------------------------------------------------------------------------------
//some prototypes....                                                           */
int stbGeom_destruction();    /* destruction routine */
int stbGeom_compute();        /* computation routine */
int stbGeom_desc();           /* description function */
int stbGeom_init();           /* initialization routine */


/*------------------------------------------------------------------------------
// AVSinit_modules
//
// called by the AVS 
// tells the AVS the pointer to the description function                        */
void AVSinit_modules()
{
  AVSmodule_from_desc(stbGeom_desc);
}


/*------------------------------------------------------------------------------
// stbGeomRegister
//
// sends a register message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// clr:      should the old geometry be deleted or the new one added?
// usr:      for which users the module should be visible                       */
void stbGeomRegister(char clr,int usr)
{
  char buf[STB_MAXPARAMETERLEN];
  char *tmp;
  stbFifoMsg msg;

  sprintf(buf,"%c %d",clr,usr);

  msg.moduleId=moduleId;
  msg.type=STBFIFO_REGISTER_MODULE;
  msg.length= sizeof(int)+strlen(buf)+1; /*moduleType +buf+\0*/
  msg.data=malloc(msg.length);
  tmp=msg.data;
  stbNCPutInt(&tmp,STB_MODULE_GEOM);
  memcpy(tmp, buf, strlen(buf)+1);
  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
  registered=TRUE;
  Debug(printf("stbGeom: registration sent\n"));
}


/*------------------------------------------------------------------------------
// stbGeomUnregister
//
// sends an unregister message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId                                         */
void stbGeomUnregister()
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
// stbGeomUpdate
//
// sends an update message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// clr:      should the old geometry be deleted or the new one added?
// usr:      for which users the module should be visible                       */
void stbGeomUpdate(char clr,int usr)
{
  char buf[STB_MAXPARAMETERLEN];
  stbFifoMsg msg;

  sprintf(buf, "%c %d",clr,usr);
  msg.moduleId=moduleId;
  msg.type=STBFIFO_UPDATE_MODULE;
  msg.length= strlen(buf)+1; /*+buf+\0*/
  msg.data=malloc(msg.length);
  memcpy(msg.data, buf, strlen(buf)+1);
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
// stbGeom_desc
//
// description Function
// called from the AVS on initialization
// describes the module: Widgets, output, modul-type and name...                */
int stbGeom_desc()    /* description function */
{
  int clr_id;

  AVSset_module_name("stbGeom",MODULE_RENDER);
  AVScreate_input_port("Communication Channel in",COMPORT_SPECS, OPTIONAL);
  AVScreate_input_port("user input","integer", OPTIONAL);
  AVScreate_input_port("geom input","geom", OPTIONAL);

  clr_id = AVSadd_parameter("clear Geometry", "choice", "yes", "yes:no", ":");
  AVSconnect_widget(clr_id, "radio_buttons");

  AVSset_compute_proc((AVS_FNCP) stbGeom_compute);
  AVSset_init_proc(stbGeom_init);
  AVSset_destroy_proc(stbGeom_destruction);

  return 0;
}


/*------------------------------------------------------------------------------
// stbGeom_init
//
// init function
// called from the AVS on initialization
// nothing to be done                                                           */
int stbGeom_init() 
{ 
  return 1;
}


/*------------------------------------------------------------------------------
// stbGeom_destruction
//
// destruction function
// called from the AVS on removing the module from the network
// sends an unregister to the stbWin and closes the fifo                        */
int stbGeom_destruction() 
{
  stbFifoMsg msg;

  /* send unregister message via fifo to stbWin */
  msg.moduleId=moduleId;
  msg.type=STBFIFO_UNREGISTER_MODULE;
  msg.length=0;
  msg.data=NULL;

  stbFifoSend(fifo_fd, &msg);

  stbFifoClose(fifo_fd);
  return 1;
}


/*------------------------------------------------------------------------------
// stbGeom_compute
//
// computation function
// called from the AVS when an input or widget has changed...
// the main function of the stbFloat AVS Module                            
//
//Parameters:
// widget and input values from the AVS                                         */
int stbGeom_compute(char *com_in, int user,GEOMedit_list geom_in,char *clr)
{
  int stbId;
  static int oldStbId=-1;
  char servername[256];
  int redo_geometry=FALSE;
  int users;
  int dummy,newStbWinConnected;
  char stbWinName[256];
  char buf[STB_MAXPARAMETERLEN];

  if (moduleId==-1)
  {
    moduleId=AVSget_unique_id();
    Debug(printf("stbGeom: moduleId %d, moduleType %d\n", moduleId, STB_MODULE_GEOM));
  }

  Debug(printf("stbGeom_compute() invoked\n"));

  users=~user;

  if (com_in==NULL)
  {
    if (registered) 
    {
      stbGeomUnregister();
    }      
    return 1;
  }
  if (AVSinput_changed("Communication Channel in", 0) || (!registered)) 
  {
    /* check for STB Kennung  */
    sscanf(com_in, "%s %d %d %s %d",buf, &stbId,&dummy,stbWinName, &newStbWinConnected);
    if (strcmp(buf,STB_ID)!=0)
    {
      PortDisconnect(NULL,":0");
      AVSerror("Module is not a STB Connection Module"); 
      return 0;
    }
    Debug(printf("stbGeom: stbWin id is %d\n", stbId));
    if ((oldStbId != stbId) || (!registered) )
    {
      oldStbId=stbId;
      if (registered) 
      {
        stbGeomUnregister();
      }
      if (stbId!=0)
      { 
        /* Create write end of fifo */
        Debug(printf("stbGeom: about to create write end of fifo\n"));
        if (!stbFifoCreate(stbId)) 
        {
          fprintf(stderr, "stbGeom: can't create fifo\n");
          exit(1);
        }
        if ((fifo_fd=stbFifoOpen(stbId, MODE_WRITE)) == -1) 
        {
          fprintf(stderr, "stbGeom: can't open fifo for writing\n");
          exit(1);
        }
        Debug(printf("stbGeom: about to register to stbWin\n"));
        /* register stbGeom to stbWin */
        stbGeomRegister(clr[0],users);
      }
      redo_geometry=TRUE;
    }
    if ((newStbWinConnected!=stbWinConnected) && (newStbWinConnected!=FALSE))
    {
      redo_geometry=TRUE;
    }
    stbWinConnected=newStbWinConnected;
  }

  if ((AVSparameter_changed("clear Geometry")|| AVSinput_changed("user input", 0)) 
      && (registered) )
  {
    stbGeomUpdate(clr[0],users);
  }

  if (AVSinput_changed("geom input", 0) || (geom_in && redo_geometry)) 
/*  if ( ((AVSinput_changed("geom input", 0)) || (redo_geometry)) && (geom_in!=NULL) ) */
  {
    if (registered) 
    {
        /* convert and send geometry */
        Debug(printf("Sending geometry\n"));
        do_output(geom_in,fifo_fd,moduleId);
        Debug(printf("Sending complete\n"));
    }
    else
    {
      Debug(printf("wanted to send geometry but not registered to stbWin\n"));
    }
  }
  return(1);
} /* geom_compute */
    
