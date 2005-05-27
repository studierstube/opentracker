// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       context
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Function prototypes for context sensitive pip display module
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:17:57  gh      last modification
//  17-sep-98 21:47:52  ds      created
// ===========================================================================


#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <Inventor/nodes/SoCoordinate3.h>

// context demonstrates how the pip can be used as a context sensitive display
// when this mode is active, the object closest to the center of the pip
// is picked (if there is any), it is selected and its name is displayed.
// furthermore, context sensitive controls are displayed depending on the
// type of the object: if it is a tree, then a set of green coloring buttons
// is displayed; if it is a house, a collection of yellow buttons is displayed.


extern SoCoordinate3* pipCoord; // external pointer to pip coordinate set

void setupContext(              // setup context stuff
    );
void doContext(                 // called every frame to display context
    );

#endif//_CONTEXT_H_
