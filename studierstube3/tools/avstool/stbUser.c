/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stbUser
//  TYPE:       c code
//  PROJECT:    Integrating Scientific Visualizations into Studierstube Workspace
//  CONTENT:    AVS Module which provides a userlimitation for all stbModules    
//              the modul sends an integer value to its output
//              every user has a bit in this integer -value
//              and the output is bitwise negated
//              so all user, a value of -1 is 0 at the output
//              currently the modul provides max 4 user limitations.
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


/*------------------------------------------------------------------------------
//some prototypes....                                                           */
int stbUserCompute();        /* computation routine */
int stbUserDesc();           /* description function */


/*------------------------------------------------------------------------------
// AVSinit_modules
//
// called by the AVS 
// tells the AVS the pointer to the description function                        */
void AVSinit_modules()
{
  AVSmodule_from_desc(stbUserDesc);
}


/*------------------------------------------------------------------------------
// stbUserDesc
//
// description Function
// called from the AVS on initialization
// describes the module: Widgets, output, modul-type and name...                */
int stbUserDesc()		/* description function */
{
  int com_in_port, data_port;
  int usr1Id,usr2Id,usr3Id,usr4Id;

  AVSset_module_name("stbUser", MODULE_DATA);

  AVScreate_input_port("User1 input","integer", OPTIONAL);

  AVScreate_input_port("User2 input","integer", OPTIONAL);
  AVScreate_input_port("User3 input","integer", OPTIONAL);
  AVScreate_input_port("User4 input","integer", OPTIONAL);

  data_port=AVScreate_output_port("stbUser output", "integer");

  usr1Id = AVSadd_parameter("User 1", "boolean", 1, NULL, NULL);
  AVSconnect_widget(usr1Id, "toggle");

  usr2Id = AVSadd_parameter("User 2", "boolean", 1, NULL, NULL);
  AVSconnect_widget(usr2Id, "toggle");

  usr3Id = AVSadd_parameter("User 3", "boolean", 1, NULL, NULL);
  AVSconnect_widget(usr2Id, "toggle");

  usr4Id = AVSadd_parameter("User 4", "boolean", 1, NULL, NULL);
  AVSconnect_widget(usr2Id, "toggle");

  AVSset_compute_proc(stbUserCompute);

  return 0;
} 


/*------------------------------------------------------------------------------
// stbUserCompute
//
// computation function
// called from the AVS when an input or widget has changed...
// the main function of the stbFloat AVS Module                            
//
//Parameters:
// widget and input values from the AVS                                         */
int stbUserCompute(int usr4Inp,int usr3Inp,int usr2Inp,int usr1Inp,int *int_out,int usr1,int usr2,int usr3,int usr4) 
{
  char moduleName[256];
  char *commandOut,*commandError;
  char *findStr,*helpStr;
  char compareStr[256];
  int usrInputConnected;

  /*Get the AVS Module Name */
  AVScommand("kernel","var_get Module",&commandOut,&commandError);
  strcpy(moduleName,commandOut);
  moduleName[strlen(moduleName)-1]=0;

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
      /*ok, found, then search the begining of the module which is connected th stbUser*/
      findStr+=strlen("port_connect ");
      findStr=strchr(findStr,' ');
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
        /*and now do the compare with the connection found */
        if (strncmp(compareStr,moduleName,strlen(moduleName))==0)
        {
          helpStr=compareStr+strlen(moduleName)+1;
          usrInputConnected=atoi(helpStr);
          switch (usrInputConnected)
          {
            case 3: 
            {
              if (usr1!=usr1Inp) 
              {
                AVSmodify_parameter("User 1",AVS_VALUE,usr1Inp,0,0);
                usr1=usr1Inp;
              }
            }
            break;
            case 2: 
            {
              if (usr2!=usr2Inp) 
              {
                AVSmodify_parameter("User 2",AVS_VALUE,usr2Inp,0,0);
                usr2=usr2Inp;
              }
            }
            break;
            case 1: 
            {
              if (usr3!=usr3Inp) 
              {
                AVSmodify_parameter("User 3",AVS_VALUE,usr3Inp,0,0);
                usr3=usr3Inp;
              }
            }
            break;
            case 0: 
            {
              if (usr4!=usr4Inp) 
              {
                AVSmodify_parameter("User 4",AVS_VALUE,usr4Inp,0,0);
                usr4=usr4Inp;
              }
            }
            break;
          }
        }
      } 
    }
  }

  /*set the Bits 4 the selected users */
  *int_out=~((usr1*1)|(usr2*2)|(usr3*4)|(usr4*8));  

  return(1);
}
    


