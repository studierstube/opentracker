// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       AVSDISPLAY Server
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Studierstube Application 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  23-nov-99 15:33:10  peci      NT Port
//  21-apr-99 08:00:00  peci      created
// ===========================================================================
#ifdef WIN32

#pragma warning( disable : 4305 )



#ifdef AVSDISPLAY_EXPORTS
#define AVSDISPLAY_API __declspec(dllexport)
#else
#define AVSDISPLAY_API __declspec(dllimport)
#endif


#endif

#ifndef _AVSDISPLAY_H_
#define _AVSDISPLAY_H_

//if WITH_GEOM_DECODE_THREAD is defined the geometry parsing is done in
//a seperate thread, which cannot use the SoStreamLine Object!!
//#define WITH_GEOM_DECODE_THREAD


#include <stdio.h>
#include <stdlib.h>

#include <stbapi/workspace/StbCommunicator.h>

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/SbBasic.h>
#include <Inventor/nodes/SoMaterial.h>

#include "stbcomm.h"
#include <stbapi/util/zpthread.h>

#include <sys/types.h>

#include "module.h"
#include "clientmodule.h"
#include "geommodule.h"

#define MSG_STBSERVER_WSAPP_MSG 1001
#define MSG_STBSERVER_ISRUNNING_MSG 1111

//---------------------------------------------------------------------------
//structure used for Parameter handling to the NetworkConnection thread
typedef struct
{
  stbNetCom 			*nc;
  StbCommunicator 		*communicationManager;
  ModuleManager			*ClientMan;
}
threadParameterProcessNetworkConnectionStruct;


//---------------------------------------------------------------------------
//structure used for Parameter handling to the Wait4Connection thread
typedef struct
{
  StbCommunicator 		*communicationManager;
  ModuleManager			*ClientMan;
}
threadParameterWait4ConnectionStruct;


//---------------------------------------------------------------------------
//structure used for Parameter handling to the StbMessage callback function
typedef struct
{
  char				MsgIdString[30];
  ModuleManager			*ClientMan;
  int			     	clientId;
  stbMessage			msg;
}
MesageParameterStruct;

//---------------------------------------------------------------------------
//structure used for Parameter handling to the StbMessage to check wheter the
// AVSDISPLAY Server Application is still running
typedef struct
{
  char				MsgIdString[30];
}
MesageParameterIsRunningStruct;


//---------------------------------------------------------------------------
//structure used for Parameter handling to the StbMessage callback function
//if it was a LOGIN packet. its the same as the MesageParameterStruct
//with the pointer to the stbNetCom added to the stucture..
typedef struct
{
  char				MsgIdString[30];
  ModuleManager			*ClientMan;
  int			     	clientId;
  stbMessage			msg;
  stbNetCom			*nc;
}
MesageParameterClientStruct;



#endif//_AVSDISPLAY_H_
