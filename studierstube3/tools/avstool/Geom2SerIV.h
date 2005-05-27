/* ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       Geom2SerIV.h
//  TYPE:       c code
//  PROJECT:    Studierstube
//  CONTENT:    defines and prototypes for the Geom2SerIV 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    krutz     Markus Krutz
//              peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  12-sep-97 08:00:00  krutz      created
//  21-apr-99 08:00:00  peci       modified
// ===========================================================================  */

#ifndef _GEOM2SERIV_H
#define _GEOM2SERIV_H

#include <stdio.h>
#include <avs/avs.h>
#include <avs/port.h>
#include <avs/geom.h>

#define	POLYLINE_WIDTH		5.0
#define	DISJOINTLINE_WIDTH	2.0

#undef	BIDIRECTIONAL_LIGHT

#define	LABEL_SCALE	300.0

int do_output(GEOMedit_list in,int fifo_fd_in,int modulId);

int output_mesh(char *name, GEOMobj *input);
int output_sphere(char *name, GEOMobj *input);
int output_polytri(char *name, GEOMobj *input);
int output_polyline(char *name, GEOMobj *input);
int output_djline(char *name, GEOMobj *input);
int output_label(char *name, GEOMobj *input);

 
#endif
