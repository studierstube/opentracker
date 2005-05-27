/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** globals.h - some useful macros and defines.
 * globals.h contains some macros and defines needed throughout the project.
 *
 * @author  EMC
 *
 * $Id: globals.h,v 1.3 2005/01/22 20:10:28 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

//-------------------------------------------------------------------------------------
// Math Macros and Definitions
//-------------------------------------------------------------------------------------
#define PI         3.14159265358979323846f				// Pi
#define PIUNDER180 57.2957795130823208767981548141052f	// 180°/PI
#define EPSILON    1e-12

#define DEG2RAD(x)	( ((x) * PI) / 180 )
#define DEGTORAD(x)	( ((x) * PI) / 180 )

#define RAD2DEG(x)	( ((x) * 180) / PI )
#define RADTODEG(x)	( ((x) * 180) / PI )

//-------------------------------------------------------------------------------------
// Macros for OpenInventor
//-------------------------------------------------------------------------------------
#ifndef DELPTR
#define DELPTR(p) { if (p != NULL) { delete p; p = NULL; } }
#endif

#ifndef DELAPTR
#define DELAPTR(p) { if (p != NULL) { delete[] p; p = NULL; } }
#endif

#ifndef UNREF
#define UNREF(p) if ((p) != NULL) { p->unref(); p = NULL; }
#endif

#ifndef SET_SENSOR
#define SET_SENSOR(sensor, field, func) { sensor.setData(this); sensor.setFunction(func); sensor.attach(&field); }
#endif

#endif // _GLOBALS_H_