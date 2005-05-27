// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       fishtool
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Function prototypes for fishnet selection tool
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


#ifndef _FISHTOOL_H_
#define _FISHTOOL_H_

#include <Inventor/SbLinear.h>
#include <stbapi/misc/SoFishnet.h>

// the fishing tool allows to select many artifacts with a single sweep of
// the pip through the scene. all artifacts penetrating the seam are selected
// ("caught") and also displayed as 3D icons on the pip (3d icons look just
// like the artifacts only smaller). With the "arrange" button we can then 
// arrange the 3D icons on the pip in a nice grid, and by clicking on any of
// the 3D icons we can unselect the corresponding artifact in case it was 
// erraneously selected.

extern SoFishnet* fishnetNode; //node for fishing

void setupFishing(     //setup fishing stuff
    SoSeparator* sceneRoot,
    SoSeparator* pipRoot
    );
void resetFishing(     //reset fishing - no object in fishnet
    );
void setFishingMode(   //switch fishing on/off
    SbBool onOrOff     //flag to set fishing on or off
    );

#endif//_FISHTOOL_H_
