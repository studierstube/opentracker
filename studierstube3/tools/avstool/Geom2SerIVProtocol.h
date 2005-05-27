/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       Geom2SerIVProtocol.h
//  TYPE:       c code
//  PROJECT:    Studierstube
//  CONTENT:    defines for the protocol the Geom2SerIV is using
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  21-apr-99 08:00:00  peci      created
// ===========================================================================  */

#ifndef _GEOMPRIM_H
#define _GEOMPRIM_H

#define MAX_VEC_PER_PACKET 64

#define COORDINATE3   1
#define INFO          2
#define NORMAL        3
#define NORMALBINDING 4
#define BASECOLOR     5
#define MATERIALBINDING 6
#define DRAWSTYLE     7
#define TRANSLATION   8
#define SPHERE        9
/*#define SEPARATOR     10 */
#define FONT          11
#define MATERIAL      12
#define TEXT2         13
#define INDEXEDLINESET 14
#define TRIANGLESTRIPSET 15
#define LINESET 16
#define QUADMESH 17
#define RESET    18
#define INDEXEDFACESET 19
#define ENDOFDATA 20

#define SEPARATOR_START 21
#define SEPARATOR_END 22
#define GEOMETRY_START	101
#define GEOMETRY_END	102

#define SI	sizeof(int)
#define SF 	sizeof(float)
#define SB	sizeof(char)

/* Justification */
#define _LEFT 0
#define _RIGHT 1
#define _CENTER 2

/* Value for Binding */
#define _PER_VERTEX 1

/* Style */
#define _FILLED 2

#endif
