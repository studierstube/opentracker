/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       stb.h
//  TYPE:       c code
//  PROJECT:    Studierstube
//  CONTENT:    common defines 
//              like TRUE,FALSE,FOREVER,DEBUG, and all module Type defines...
//              used in the AVS and in the Stb-Code....
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//    first version by Andreas H. Koenig but the name was x3.h
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================  */

#ifndef _H_STB
#define _H_STB

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define COMPORT_SPECS "string"

#ifndef TRUE
#define TRUE (1==1)
#endif

#ifndef FALSE
#define FALSE (1==0)
#endif

#define FOREVER while(TRUE)

#define STB_ID "STBID_V1a"
/*AVS-Module */
#define STB_MODULE_WIN          1
#define STB_MODULE_GEOM        10
#define STB_MODULE_PIPGROUP    20
#define STB_MODULE_INT         21
#define STB_MODULE_BOOL        22
#define STB_MODULE_FLOAT       25
#define STB_MODULE_POINT       30
#define STB_MODULE_LINE        35
#define STB_MODULE_POLYLINE    36

#define STB_MAXPARAMETERLEN    512

#ifndef NODEBUG
#ifdef DEBUG
#define Debug(_x) _x
#endif
#ifndef DEBUG
#define Debug(_x) 
#endif
#endif
#ifdef NODEBUG
#define Debug(_x) 
#endif

#endif
