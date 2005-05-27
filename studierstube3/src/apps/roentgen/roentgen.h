// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       roentgen
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Demo application for workspace - roentgen
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw		Hermann Wurnig
// ===========================================================================
//  HISTORY:
//
//  09-apr-99 08:00:00  ds      created
// ===========================================================================

#ifndef __ROENTGEN_H__
#define __ROENTGEN_H__

#ifdef WIN32
#ifdef ROENTGEN_EXPORTS
#define ROENTGEN_API __declspec(dllexport)
#else
#define ROENTGEN_API __declspec(dllimport)
#endif
#else
#define ROENTGEN_API
#endif

#define MSG_ROENTGEN_GEOMETRY_LOADED	1001

typedef struct 
{
	char				messageID[30];
	SoSeparator * 		maleSeparator;
	SoSeparator * 		penSeparator;
	SoWindowKit *		roentgenWindow;
	StbCommunicator *	communicationManager;
} 
geometryLoadedMsg;


typedef struct
{
	SoWindowKit *		roentgenWindow;
	StbCommunicator *	communicationManager;
}
threadParameterStruct;


typedef struct
{
	SoTransform * 		borderTransform;
	SoTransform *		penTransform;
	SoWindowKit *		roentgenWindow;
}
penTransformStruct;


typedef struct
{
	SoFieldSensor * 		usedSensor[MAX_USERS];
    SoSeparator *           seamSep[MAX_USERS];
}
nodeSensorStruct;


typedef struct
{
	SoSwitch * 			lensSwitch[MAX_USERS];
}
lensSwitchStruct;

#endif
