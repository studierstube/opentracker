// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       GlobalDef
//  TYPE:       h header file
//  PROJECT:    Studierstube
//  CONTENT:    Constants and structs relevant for the trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:18:12  rr      last modification
// ===========================================================================

#ifndef _GLOBALDEF_H_
#define _GLOBALDEF_H_

#define ABOUT "Trackerserver Version 2.02\n"

#include "CommonDef.h"
#include "NewKFilt.h"

const float inchesToMeters=0.0254;

const int maxBirds=14;

const int filterMax=256;// Maximum amount of position data used for filtering
const int maxBuffer=100;// Size of buffer used for reading from the serial dev

#define BUTTON_HACK      // buttonvalues from tracker nonconform with manual
                         // as used in Studierstube98 VR-Lab @ TU Vienna
//#define DEBUG_DATA       // Outputs debugging information on stderr


typedef struct
{
    char serDevName[256];  // Name of the serial device to the station
    int fd;           // Filedescriptor of the serial interface to the station
    int mode;         // Output mode of station (Position_Angles/Matrix/Quat)
    int erc;          // Bird with or without Extended Ranger Controller
    int sent;

    char name[256];     // Name of the station from configfile
    char servName[256]; // Name of the trackerserver

    int numOfBytes;    // number of bytes of the tracker's output record
    int buttonMode;    // Button_Mode of the station
    int buttons[8];    // Stations where buttons come from
    char rawButton;    // Rawbuttonbyte from the station
    int error;         // Indicates an error. Used for removing msgs.
    int received;      // Used for calculating the frequency of the station.
    int pos;           // Number of bytes read from the station of one record
    char buffer[maxBuffer];
                       // Buffer holding the bytes being read from the station

    float scaleX, scaleY, scaleZ;
    float position[3];
    float orientation[9];

    int positionFilter;
    int positionFilterParam;
    int orientationFilter;
    int orientationFilterParam;

    float fposition[3][filterMax];
    float forientation[9][filterMax];

    KalmanFilter kfilter;
    
} StationRecord;


typedef struct
{
    int fdParallel;             // Filedescriptor of the parallel interface
    short int basePort;         // Baseport of the parallel interface
    char parButtons;            // Buttonbyte from the parallel interface
} ParRecord;

typedef struct
{
    char toaddr[256];
    char toport[256];
    int sock;
} NetPropRecord;

// Internal defines

const int fobMulti=1;
const int fobSingle=2;
const int polhemus=3;
const int parButton=4;

const int buttonNone=0;
const int buttonTracker=1;

const int fobBird=0;
const int fobErc=1;

const int noFilter=0;
const int boxFilter=1;
const int anisoFilter=2;
const int kalmanFilter=3;

#endif
