// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       client
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Client Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================
#ifndef _CLIENTMODULE_H_
#define _CLIENTMODULE_H_

#include "module.h"
#include "moduleman.h"

#include <stbapi/workspace/StbCommunicator.h>
#include <stbapi/window3d/SoSizeShapeKit.h> 


class ClientModule;  //Prototype, to make the include files happy...
#include "pipmodule.h"
#include "windowmodule.h"

#include "avsdisplay.h"
#include "stbcomm.h"


#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>

class ClientModule : public Module
{
public:
  //Constructor
  //needs the Pointer to the StbCommunicator, the Pointer to the ClientManager
  //its Id, the Id of the corresponding stbWin Module,
  // the stbNetCom struct for the network-communication,
  // and its Parameter string
  ClientModule(StbCommunicator *theComMan,ModuleManager *theClientMan,int ModuleId,int newDestModId,stbNetCom *newNetCom, char *para);

  //Destructor
  virtual ~ClientModule();

  //does the initialization of the OIV Objects within the rendering loop.
  virtual void initOIVObjects();

  //calls the destructor. 
  //if sendQuit is TRUE then it sends an STB_SERVERQUIT packet to the stbWin
  // before calling the destructor...
  void remove(int sendQuit);

  //gets the Module Type, in this case it returns STB_MODULE_WIN
  virtual int getType();

  //sets the properties to the values of the parameterstring.
  virtual void setUpdate(char *data);

  // calls the setUpdate method from the corresponding Module with data
  void setUpdate(int moduleId,char *data);

  //send an update parameterstring direct to the stbWin Module...
  void update();

  //sets current displayed PIPGeometry for the user....
  void setClientWindowPIP(int userID);

  //genearates a new Object of Type moduleType with the parameterstring para
  //and insert it into the ModuleManager. 
  void addNewModule(int moduleId,int moduleType,char *para);

  //returns the PipModule with the lowest order.
  PipModule* getFirstPipModule();

  //returns the first WindowModule.
  WindowModule* getFirstWinModule();

  //sets the prev and next Pointers of all Window and PipModules.
  //calls the order methode to order the PipModules..
  void initPipModuleList();

  //generates the PipGeometry for all users. 
  // calls the updatePip Method  of all PipModules in the current order..
  void updatePip();

  //sets the cube sizes of the Lines and Points to the values depending on the
  //current window size.
  void correctPointsAndLines(SoSeparator *sep);

  //returns TRUE if the Module is enabled for the user theUser.
  int isModuleForUser(int theUser);
 


  stbNetCom *nc;                        //Pointer to NetCom structure
                                        //used for the network communication
  ModuleManager *Modules;		//Module Manager to store Modules 
					//connected to the stbWin Module....
  SoSeparator *clientVolume;            //Separator to the window Geometry 
  SoSeparator *PIPGeometry[MAX_USERS];  //Pip Gepmetry separator for each user.

  int userFocus[MAX_USERS];             //stores for all users if they have 
                                        //a focus to the window or not.

private:
  float geomSizeX,geomSizeY,geomSizeZ;  //geometry size displayed in the Window
  SoWindowKit *window;                  //the stb Window 
  SoScale *winScale;                    //scales the Geometry to the geomSize 
  SoTranslation *winTrans;              //moves the Geometry before scaling

  SoSizeShapeKit *sk[MAX_USERS];        //SoSizeShapeKit for all users 
                                        //to fit all widgets onto the pip

  ModuleManager *clientMan;             //Pointer to global ModuleManager 
                                        //where all clients are stored to
  StbCommunicator *cMan;                //Pointer to global StbCommunicator
                                        //necessary to set the PIP Geometry
  int destModId;                        //Id of the corresponding stbWin Module
  int user;                             //for which user the Module is enabled.

  float posX,posY,posZ,sizeX,sizeY,sizeZ,rotX,rotY,rotZ,rotW;
                                        //parameter from the constructor to the
                                        //initOIVObjects() function.
 
  //Static Window Callback function
  static SbBool ClientModule::MyWindowCB(void *userData,int messageID,SoWindowKit *window, 
			    int userID,So3DEvent *event, SoPath *clientVolumePath);

}; //class ClientModule



#endif//_CLIENTMODULE_H_
