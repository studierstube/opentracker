// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       snapshot
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Function prototypes for snapshot/seam file system module
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


#ifndef _SNAPSHOT_H_
#define _SNAPSHOT_H_

// the snapshot tool allows to store 3D scenes in seam (aka magic lenses)
// containers that may be associated with the pip or floating above the 
// scene. it is a first step towards a concept for a file-system equivalent
// when dealing with virtual worlds. seams are like files, only the content
// is visible through the seam surface (which makes for a kind of preview
// seams are live views, so at the same time they are like windows in a 
// desktop system.
// the snapshot tool gives the user a seam on the pip. the view on the pip/seam
// may be frozen, and the seam may be disconnected from the pip. after that,
// the secondary world of the seam is disconnected from the primary world
// by pressing the store button, and changes made to the primary world are no
// longer reflected in the secondary world - it is a scene container, however,
// it may also be manipulated by reaching through the seam (this is only
// partially implemented). seams can be picked up again by pressing the 
// pickup button and the sweeping the pip through the seam to be picked up.
// finally, by releasing the freeze button that locks the view on the pip 
// the view can also be relased again.


void setupSnapshot(            //setup snapshot stuff
    );
void setCurrentSeamVisibility( //switch visbility of seam currently on pip
    SbBool onOrOff             //flag for visibility
    );

#endif//_SNAPSHOT_H_
