// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Polhemus
//  TYPE:       h header file
//  PROJECT:    Studierstube
//  CONTENT:    Definition of Polhemus Fastrak specific functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:18:12  rr      last modification
// ===========================================================================


#ifndef _POLHEMUS_H_
#define _POLHEMUS_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <curses.h>
#include <ctype.h>
#include "GlobalDef.h"
#include "SerComm.h"
#include "NetComm.h"
#include "CfgOpts.h"
#include <util/os.h>

int runFastrak(char* initFilename, int numOfStations,
               NetworkTrackerDataRecord netRecord, StationRecord *station,
               NetPropRecord npr);

#endif
