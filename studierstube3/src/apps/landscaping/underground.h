// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       underground
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Function prototypes for underground cable wiring draw tool
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


#ifndef _UNDERGROUND_H_
#define _UNDERGROUND_H_

// underground demonstrates seams and the fact that the pip has two-sides,
// which makes for a nice spring-loaded mode. when the pip is flipped,
// the user sees the scene (island) in wireframe and can consequently 
// manipulate stuff _under_ the ground by simply reaching into the ground with
// the pen. cable TV wires can be drawn in the ground to wire up houses on 
// the surface.

void goUnderground(  //setup underground stuff
    );

#endif//_UNDERGROUND_H_
