/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbPolyline
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    AVS Module which provides a polyline in an Geometry-Output    
//              which can be controlled via the Studierstube display-server
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
//the structure for the point-list                                               */
struct point
{
  float pos[3];
  struct point *next;
};


/*------------------------------------------------------------------------------
//static globals....                                                            */
static struct point *pointList;/* the pointer to the first point in the list */
static int curNumLines=0;      /* number of lines the polyline consists of */
static int moduleId=-1;        /* the ID of the Module */
static int registered=FALSE;   /* is it currently registered to the stbWin?*/
static int fifo_fd;            /* used to send data to the fifo */
static int parentPipGroup=0;   /* parent pipGroup(if is connected to a pipgroup)*/ 


/*------------------------------------------------------------------------------
//some prototypes....                                                           */
int stbPolylineinit();           /* initialization routine */
int stbPolylinedestruction();    /* destruction routine */
int stbPolylinecompute();        /* computation routine */
int stbPolylinedesc();           /* description function */


/*------------------------------------------------------------------------------
// PrintPoints                             
//
// Prints out information and all coordinates of the polyline's points 
// used for debug only                                                          */
void PrintPoints()
{
  struct point *thePoint,*hlp;
  int i=0,j;
  int numNewPoints;
  float dif[3];

  thePoint=pointList;
  while (thePoint!=NULL)
  {
    printf("Print Points %d %d    %f %f %f\n",curNumLines,i,thePoint->pos[0],thePoint->pos[1],thePoint->pos[2]);
    i++;
    thePoint=thePoint->next;
  } 
}


/*------------------------------------------------------------------------------
// genNewPoints                             
//
// Appands new Point at the end of the list 
// at the end of this function the list should have newNumPoints entries                                                         
// note: the newNumPoints should be bigger than the curNumLines!!!!
//
//Parameter:
// newNumPoints: number of points should be in the list.                        */
void genNewPoints(int newNumPoints)
{
  struct point *thePoint;
  int i=0;

  if (pointList==NULL)
  {
    thePoint=(struct point*)malloc(sizeof(struct point));
    thePoint->pos[0]=0.0f;
    thePoint->pos[1]=0.0f;
    thePoint->pos[2]=0.0f;
    thePoint->next=NULL;
    pointList=thePoint;
  }
  else
  {
    thePoint=pointList;
  }
  while (thePoint->next!=NULL)
  {
    i++;
    thePoint=thePoint->next;
  }
  while (i<newNumPoints)
  {
    thePoint->next=(struct point*)malloc(sizeof(struct point));
    thePoint=thePoint->next;
    thePoint->next=NULL;
    thePoint->pos[0]=0.0f;
    thePoint->pos[1]=0.0f;
    thePoint->pos[2]=0.0f;
    i++;
  } 
  curNumLines=i;
}


/*------------------------------------------------------------------------------
// deleteAllPoints                             
//
// deletes all entries in the point-List                                        */
void deleteAllPoints()
{
  struct point *thePoint,*hlp;

  thePoint=pointList;
  while (thePoint!=NULL)
  {
    hlp=thePoint;
    thePoint=thePoint->next;
    free(hlp);
  }
  pointList=NULL;
  curNumLines=0;
}


/*------------------------------------------------------------------------------
// reducePoints                             
//
// reduces the point list of the polyline to the spezified number of entries 
// it deletes interpolated entries so that the polyline should have the same
// begin and end....
// note: the newNumPoints should be less than the curNumLines!!!!
//
//Parameter:
// newNumPoints: number of points should be in the list.                        */
void reducePoints(newNumPoints)
{
  struct point *thePoint,*hlp;
  int i=1;

  thePoint=pointList;
  if (newNumPoints==1)
  {
    thePoint=thePoint->next;
    /*remove all instead the first and the last...*/
    while (thePoint->next!=NULL)
    {
      hlp=thePoint;
      thePoint=thePoint->next;
      free(hlp);
    }
    pointList->next=thePoint;   
  }
  else
  {
    while ((thePoint->next!=NULL) && (thePoint!=NULL))
    {
      if ((i*newNumPoints/curNumLines)==((i-1)*newNumPoints/curNumLines))
      {
        hlp=thePoint->next;
        thePoint->next=hlp->next;
        free(hlp);
      }
      else 
      {
        thePoint=thePoint->next;
      }
      i++;
    }
    if (thePoint==NULL)
    {
      /*remove the Last Point*/
      thePoint=pointList;  
      hlp=NULL;
      while (thePoint->next!=NULL)
      {
        hlp=thePoint; 
        thePoint=thePoint->next;
      }
      hlp->next=NULL;
      free(thePoint);
    }
  }
  curNumLines=newNumPoints;
}


/*------------------------------------------------------------------------------
// insertPoints                             
//
// expands the point list to newNumPoints entries 
// the coordinates of the points are interpolated and the begin and end of the
// polyline should be the same.
// note: the newNumPoints should be bigger than the curNumLines!!!!
//
//Parameter:
// newNumPoints: number of points should be in the list.                        */
void insertPoints(int newNumPoints)
{
  struct point *thePoint,*hlp;
  int i=1,j;
  int numNewPoints;
  float dif[3];

  thePoint=pointList;
  while (thePoint->next!=NULL)
  {
    numNewPoints=((i*newNumPoints)/curNumLines)-(((i-1)*newNumPoints)/curNumLines)-1;
    if (numNewPoints>0)
    {
      dif[0]=(thePoint->next->pos[0]-thePoint->pos[0])/(numNewPoints+1);
      dif[0]=(thePoint->next->pos[1]-thePoint->pos[1])/(numNewPoints+1);
      dif[0]=(thePoint->next->pos[2]-thePoint->pos[2])/(numNewPoints+1);
      for (j=0;j<numNewPoints;j++)
      {
        hlp=(struct point*)malloc(sizeof(struct point));
        hlp->pos[0]=thePoint->pos[0]+dif[0];
        hlp->pos[1]=thePoint->pos[1]+dif[1];
        hlp->pos[2]=thePoint->pos[2]+dif[2];
        hlp->next=thePoint->next;
        thePoint->next=hlp;    
        thePoint=thePoint->next;
      }
    }
    i++;
    thePoint=thePoint->next;
  }
  curNumLines=newNumPoints;
}


/*------------------------------------------------------------------------------
// updatePoint                             
//
// modifies the entrie (position) of a point in the list
// note: the pointToModify should be less or equal the curNumLines!!!!
//
//Parameter:
// pointToModify: the pointentrie which has to be modified.
// posX,float posY,float posZ: The new coordinate of the point entrie.          */
void updatePoint(int pointToModify,float posX,float posY,float posZ)
{
  struct point *thePoint,*hlp;
  int i=1;

  genNewPoints(pointToModify);
  thePoint=pointList;
  if (pointToModify==0)
  {
    thePoint->pos[0]=posX;
    thePoint->pos[1]=posY;
    thePoint->pos[2]=posZ;   
  }
  else
  {
    while ((thePoint->next!=NULL) && (i<pointToModify))
    {
      i++;
      thePoint=thePoint->next;
    }
    if (thePoint->next==NULL)
    {
      thePoint->next=(struct point*)malloc(sizeof(struct point));
      thePoint=thePoint->next;
      thePoint->pos[0]=posX;
      thePoint->pos[1]=posY;
      thePoint->pos[2]=posZ;   
      thePoint->next=NULL;
      curNumLines++;
    }
    else
    {
      thePoint=thePoint->next;
      thePoint->pos[0]=posX;
      thePoint->pos[1]=posY;
      thePoint->pos[2]=posZ;   
    }
  }
}

/*------------------------------------------------------------------------------
// updateLine                             
//
// modifies the pointentries (positions) of a line-part of the polyline
// calls the updatePoint function
//
//Parameter:
// startPoint: the pointentrie which has to be modified.
// fromX, fromY, fromZ: The new start coordinate of the line.          
// toX, toY, toZ:       The new  end  coordinate of the line.                   */
void updateLine(int startPoint,float fromX,float fromY,float fromZ,float toX,float toY,float toZ)
{
  struct point *thePoint;
  int i=0;

  if (curNumLines==0)
  {
    thePoint=(struct point*)malloc(sizeof(struct point));
    thePoint->pos[0]=fromX;
    thePoint->pos[1]=fromY;
    thePoint->pos[2]=fromZ;
    pointList=thePoint;

    thePoint->next=(struct point*)malloc(sizeof(struct point));
    thePoint=thePoint->next;
    thePoint->pos[0]=toX;
    thePoint->pos[1]=toY;
    thePoint->pos[2]=toZ;   
    thePoint->next=NULL;
    curNumLines=1;
  }  
  else
  {
    updatePoint(startPoint,fromX,fromY,fromZ);
    updatePoint(startPoint+1,toX,toY,toZ);
  }
  Debug(PrintPoints());
}


/*------------------------------------------------------------------------------
// processPointList                             
//
// generates,reduces,deletes & inserts point to the spezified number of entries
//
//Parameter:
// newNumPoints: The number of entries in the pointlist.                        */
void processPointList(int newNumPoints)
{
  if (curNumLines==newNumPoints) 
    return;
  if (curNumLines==0)
  {
    if (newNumPoints>0)
    {
      genNewPoints(newNumPoints);
    }
  }
  else
  {
    if (curNumLines>newNumPoints)
    {
      if (newNumPoints==0)
      {
        deleteAllPoints();
      }
      else
      {
        reducePoints(newNumPoints);
      }        
    }
    else
    {
      insertPoints(newNumPoints);
    }
  }
} 


/*------------------------------------------------------------------------------
// sendPoints                             
//
// sends all points of the pointlist, the whole polyline coords
// via a STBFIFO_GEOM_MODULE packet via FIFO to the stbWin.                     */
void sendPoints()
{
  struct point *thePoint,*hlp;
  int i=0,len;
  char *b;
  stbFifoMsg msg;


  len=3*sizeof(float)+2*sizeof(int);
  msg.type=STBFIFO_GEOM_MODULE;
  msg.moduleId=moduleId;
  msg.length=len;
  msg.data=malloc(len);

  thePoint=pointList;
  while (thePoint!=NULL)
  {
    b=msg.data;
    stbNCPutInt(&b,curNumLines);
    stbNCPutInt(&b,i);
    stbNCPutFloat(&b,thePoint->pos[0]);
    stbNCPutFloat(&b,thePoint->pos[1]);
    stbNCPutFloat(&b,thePoint->pos[2]);
    stbFifoSend(fifo_fd, &msg);
    Debug(printf("Sending Point %d %d  %f %f %f\n",curNumLines,i,thePoint->pos[0],thePoint->pos[1],thePoint->pos[2]));
    i++;
    thePoint=thePoint->next;

  }
  free(msg.data);  

}


/*------------------------------------------------------------------------------
// readPointList                             
//
// reads the coords from a polyline-AVS-Geometry ASCII file
//
//Paramter:
// fileName: filename of the ASCII AVS Geometry file.
// numPoints:returns the new number of points through this pointer              */
void readPointList(char *fileName,int *numPoints)
{
  FILE *in;
  GEOMobj *obj;
  int i;
  float *vtx,x,y,z;

  if ((in=fopen(fileName,"r"))==NULL) return;
  obj=GEOMread_text(in,0);
  Debug(printf("LOADING PolyLine GEOM....\n"));

  if (obj!=NULL)
  {
    if (obj->type==GEOM_POLYTRI)
    {
      if ( obj->d.pt.npls > 0)
      {
        if (obj->d.pt.npls > 0) /*take the first polyLine... */
        {
          deleteAllPoints();
          vtx=obj->d.pt.plverts[0].l;
          for (i=0;i<obj->d.pt.plverts[0].n;i++)
          {
            x=*(vtx++);
            y=*(vtx++);
            z=*(vtx++);
            updatePoint(i,x,y,z);
            Debug(PrintPoints());
          }
        }
      }
    }
    GEOMdestroy_obj(obj);
  }
  AVSmodify_parameter("Lines:",AVS_VALUE, curNumLines, 0,0);
  (*numPoints)=curNumLines;
  fclose(in);
}


/*------------------------------------------------------------------------------
// AVSinit_modules
//
// called by the AVS 
// tells the AVS the pointer to the description function                        */
void AVSinit_modules()
{
  AVSmodule_from_desc(stbPolylinedesc);
}


/*------------------------------------------------------------------------------
// stbLineRegister
//
// sends a register message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// pipGroup:       the pipGroup or the stbWin's moduleId if it has no pipGroup
// num:            number of points of the polyline
// cur:            line entrie for which the coords are  
// len:            length distance, where to generate a new point
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
void stbPolylineRegister(int pipGroup,int num,int cur,float len,float fromX,float fromY,float fromZ,float toX,float toY,float toZ,int order,int usr,int withTexture,char *textureFileName,char *name)
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
    sprintf(buf, "%d %d %d %f %f %f %f %f %f %f %d %d Y %s %s",pipGroup,num,cur,len,fromX,fromY,fromZ,toX,toY,toZ,order,usr,tmp,name);
  }
  else 
  {
    sprintf(buf, "%d %d %d %f %f %f %f %f %f %f %d %d N %s",pipGroup,num,cur,len,fromX,fromY,fromZ,toX,toY,toZ,order,usr,name);
  }

  msg.moduleId=moduleId;
  msg.type=STBFIFO_REGISTER_MODULE;
  msg.length= sizeof(int)+strlen(buf)+1; /*moduleType +buf+\0*/
  msg.data=malloc(msg.length);
  tmp=msg.data;
  stbNCPutInt(&tmp,STB_MODULE_POLYLINE);
  memcpy(tmp, buf, strlen(buf)+1);
  stbFifoSend(fifo_fd, &msg);
  free(msg.data);
  Debug(printf("stbPolyline: registration sent %d %s\n",msg.moduleId,buf));
  registered=TRUE;
}


/*------------------------------------------------------------------------------
// stbPolylineUnregister
//
// sends an unregister message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId                                         */
void stbPolylineUnregister()
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
// stbPolylineUpdate
//
// sends an update message via fifo to the stbWin Module.
// uses the global fifo_fd and moduleId
//
//Parameter:
// pipGroup:       the pipGroup or the stbWin's moduleId if it has no pipGroup
// num:            number of points of the polyline
// cur:            line entrie for which the coords are  
// len:            length distance, where to generate a new point
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
void stbPolylineUpdate(int pipGroup,int num,int cur,float len,float fromX,float fromY,float fromZ,float toX,float toY,float toZ,int order,int usr,int withTexture,char *textureFileName,char *name)
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
    sprintf(buf, "%d %d %d %f %f %f %f %f %f %f %d %d Y %s %s",pipGroup,num,cur,len,fromX,fromY,fromZ,toX,toY,toZ,order,usr,tmp,name);
  }
  else 
  {
    sprintf(buf, "%d %d %d %f %f %f %f %f %f %f %d %d N %s",pipGroup,num,cur,len,fromX,fromY,fromZ,toX,toY,toZ,order,usr,name);
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
void stbPolylineEventHandled(int eventId)
{
  char *tmp;
  stbFifoMsg msg;
 
  Debug(printf("stbPolyline: sending acknowledgement"));
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
// stbPolylinedesc
//
// description Function
// called from the AVS on initialization
// describes the module: Widgets, output, modul-type and name...                */
int stbPolylinedesc()    /* description function */
{
  int stbPolylinecompute();
  int com_in_port, data_port;
  int points_id,num_id,len_id,name_id,order_id,file_id;
  char str[100000],s[20];
  int textEnabled_id,textFile_id;
  int i;

  strcpy(str,"");
  for (i=0;i<2048;i++)
  {
    if (i!=0) strcat(str,":");
    sprintf(s,"%d",i);
    strcat(str,s);
  }

  AVSset_module_name("stbPolyline",MODULE_DATA);
  com_in_port=AVScreate_input_port("Communication Channel in",COMPORT_SPECS, OPTIONAL);
  AVScreate_input_port("user input","integer", OPTIONAL);
  data_port=AVScreate_output_port("point output","geom");

  name_id=AVSadd_parameter("Name:", "string", "stbPolyline", NULL, NULL);
  order_id=AVSadd_parameter("Order:", "integer", 0, INT_UNBOUND, INT_UNBOUND);
  textEnabled_id=AVSadd_parameter("with Texture", "boolean", 0, NULL, NULL);
  len_id=AVSadd_float_parameter("length:", 0.1f, 0.0, FLOAT_UNBOUND);  
  num_id=AVSadd_parameter("Lines:", "integer", 0, 0, 10000);
  file_id = AVSadd_parameter("File:", "string", "", NULL, NULL);
  textFile_id = AVSadd_parameter("Texture:", "string", "", NULL, NULL);

  AVSconnect_widget(name_id, "typein");
  AVSconnect_widget(order_id, "typein_integer");
  AVSconnect_widget(textEnabled_id, "toggle");
  AVSconnect_widget(len_id, "typein_real");
  AVSconnect_widget(num_id, "typein_integer");
  AVSconnect_widget(file_id, "browser");
  AVSconnect_widget(textFile_id, "browser");

  AVSset_compute_proc(stbPolylinecompute);
  AVSset_init_proc(stbPolylineinit);
  AVSset_destroy_proc(stbPolylinedestruction);
  return 0;
}


/*------------------------------------------------------------------------------
// stbPolylineinit
//
// init function
// called from the AVS on initialization
// nothing to be done                                                           */
int stbPolylineinit() 
{
  return TRUE;
}


/*------------------------------------------------------------------------------
// stbPolylinedestruction
//
// destruction function
// called from the AVS on removing the module from the network
// sends an unregister to the stbWin and closes the fifo                        */
int stbPolylinedestruction() 
{
  stbFifoMsg msg;

  deleteAllPoints();
  stbPolylineUnregister();
  stbFifoClose(fifo_fd);
  return 1;
}


/*------------------------------------------------------------------------------
// stbPolylinecompute
//
// computation function
// called from the AVS when an input or widget has changed...
// the main function of the stbLine AVS Module                            
//
//Parameters:
// widget and input values from the AVS                                         */
int stbPolylinecompute(char *com_in, int user, GEOMedit_list *geom_out,char *name, int order,int withTexture,float *len,int numLines,char *fileName,char *textureFileName)
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
  int i;	
  int dummy;


  if (withTexture)
  {
    AVSparameter_visible("Texture:",1); 
  }
  else
  {
    AVSparameter_visible("Texture:",0); 
  }


  /*First Run? */
  if (moduleId==-1)
  {
    pointList=NULL;
    curNumLines=0;

    moduleId=AVSget_unique_id();
    Debug(printf("stbPolyline: moduleId %d\n", moduleId));
    if (strlen(fileName)==0)
    {
      char moduleName[256],cmpStr[256],strPath[1024];
      char *findStr,*helpStr,*lastDelim;
      char *commandOut,*commandError;

      /*Get the AVS Module Name */
      AVScommand("kernel","var_get Module",&commandOut,&commandError);
      strcpy(moduleName,commandOut);
      moduleName[strlen(moduleName)-1]=0;
      sprintf(cmpStr,"module %s ",moduleName);
    
      /*get information about modules and connections */
      AVScommand("kernel","net_show",&commandOut,&commandError); 
      findStr=commandOut;
    
      /*find out if there are Connections to me and where they are.... */
      while (findStr!=NULL)
      {
        /*first find the connection*/
        findStr=strstr(findStr,cmpStr);
        if (findStr!=NULL)
        {
          findStr+=strlen(cmpStr);
          findStr=strstr(findStr,"-ex ");
          if (findStr!=NULL) 
          {
            helpStr=strPath;
            findStr+=strlen("-ex ");
            while ( ((*findStr)!=10) && ((*findStr)!=0) )
            {
              if ((*findStr)=='/') lastDelim=helpStr;
              (*(helpStr++))=(*(findStr++));
            }
            (*(lastDelim+1))=0;
            AVSmodify_parameter("File:",AVS_VALUE, strPath, NULL,NULL);
          }
        }
      }
    }
    readPointList(fileName,&numLines);
    markunchanged=FALSE;
  }

  Debug(printf("stbPolylinecompute() invoked %d  \n",parentPipGroup));
  users=~user;


  if (com_in==NULL)
  {
    if (registered) 
    {
      stbPolylineUnregister();
    }      
  }
  else
  {
    if (AVSinput_changed("Communication Channel in", 0) || (!registered)) 
    {
      /* check for STB Kennung  */
      sscanf(com_in, "%s %d %d %s %d %d",buf, &stbId, &conModuleType, conModuleName, & dummy, &conModuleParam);
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
      Debug(printf("stbPolyline: stbWin id is %d\n", stbId));
      if ((oldStbId != stbId) || (!registered) )
      {
        oldStbId=stbId;
        if (registered) 
        {
          stbPolylineUnregister();
        }
        if (stbId!=0)
        { 
          /* Create write end of fifo */
          Debug(printf("stbPolyline: about to create write end of fifo\n"));
          if (!stbFifoCreate(stbId)) 
          {
            fprintf(stderr, "stbPolyline: can't create fifo\n");
            exit(1);
          }
          if ((fifo_fd=stbFifoOpen(stbId, MODE_WRITE)) == -1) 
          {
            fprintf(stderr, "stbPolyline: can't open fifo for writing\n");
            exit(1);
          }
          stbPolylineRegister(parentPipGroup,numLines,0,*len,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,order,users,withTexture,textureFileName,name);
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
            int dummy,newNum,newCur;
            float newFromX,newFromY,newFromZ,newToX,newToY,newToZ,newLen;
            last_event_id=event_id;
            remove_event_id=event_id;
            sscanf(p, "%d %d %d %d %d %f %f %f %f %f %f %f", &dummy, &dummy, &dummy, &newNum, &newCur, &newLen, &newFromX, &newFromY, &newFromZ, &newToX,&newToY,&newToZ);
            AVSmodify_parameter("length:",AVS_VALUE | AVS_RECORD_VALUE, &newLen, 0,0);
            if (newNum!=curNumLines)
            { 
              AVSmodify_parameter("Lines:",AVS_VALUE, newNum, 0,0);
              numLines=newNum;
              processPointList(newNum);
            }
            if (newCur==-1)
            {
              markunchanged=FALSE;
            }
            else
            {
              if (newCur<newNum)
              {
                updateLine(newCur,newFromX,newFromY,newFromZ,newToX,newToY,newToZ);
              }
            }
          }
          /* next token */
          p=strtok(NULL, "!");
        }
        if (remove_event_id!=-1)
        {
          /* send acknowledgment to stbWin */
          /* from the last event for speedup */
          stbPolylineEventHandled(remove_event_id);
        }
        else
        {
          markunchanged=FALSE;
        }
      }
    }
  }

  if (AVSparameter_changed("length:") || AVSparameter_changed("Lines:") ||
      AVSparameter_changed("File:") || AVSparameter_changed("Name:") ||
      AVSparameter_changed("Order:") || AVSinput_changed("user input", 0) ||
     (oldStbPipGroup!=parentPipGroup) )
  {
    oldStbPipGroup=parentPipGroup;
    if (AVSparameter_changed("File:"))
    {
      readPointList(fileName,&numLines); 
    }
    if (AVSparameter_changed("Lines:"))
    {
      processPointList(numLines);
    }
    if (registered)      
    {
      stbPolylineUpdate(parentPipGroup,numLines,-1,*len,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,order,users,withTexture,textureFileName,name);
    }
    markunchanged=FALSE;
  }

  if (markunchanged) 
  {
    AVSmark_output_unchanged("point output");
  }
  else
  {
    if (curNumLines>0)
    {
      struct point *thePoint;
      int i=0;
      float *vertices, *colors;

      if ((vertices=(float *)malloc(sizeof(float)*3*(curNumLines+1)))==NULL) 
      {
        AVSerror("Out of memory while allocating vertex buffer\n ");
        return;
      }
      if ((colors=(float *)malloc(sizeof(float)*3*(curNumLines+1)))==NULL) 
      {	
        AVSerror("Out of memory while allocating color buffer\n ");
        return;
      }
      thePoint=pointList;
      while (thePoint!=NULL)
      {
        vertices[i*3]=thePoint->pos[0];
        vertices[i*3+1]=thePoint->pos[1];
        vertices[i*3+2]=thePoint->pos[2];
        colors[i*3]=1.0;
        colors[i*3+1]=1.0;
        colors[i*3+2]=0.0;
        i++;
        thePoint=thePoint->next;
      }
      /* init geometry output */
      obj = GEOMcreate_obj(GEOM_POLYTRI, GEOM_NULL);
      GEOMadd_polyline(obj, vertices, colors, curNumLines+1, GEOM_COPY_DATA);
      *geom_out = GEOMinit_edit_list(*geom_out);
      GEOMedit_geometry(*geom_out,"initialTimeLine",obj);
      if (fileName!=NULL)
      {
        if (fileName[strlen(fileName)-1]!='/') 
        {
          FILE *out;
          if ((out=fopen(fileName,"w+"))!=NULL)
          {
            GEOMwrite_text(obj,out,0);
            fclose(out);
          }
        }
      }
      GEOMdestroy_obj(obj);
      free(vertices);
      free(colors);
      if (registered) 
      {
        sendPoints();
      }
    }
  }
  return(1);
}
