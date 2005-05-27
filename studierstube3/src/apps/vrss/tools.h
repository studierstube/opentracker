/* =========================================================================
 *  (c) 2000,2001 VRVis GmbH, Vienna, Austria
 *  (c) 1999,2000 Vienna University of Technology
 * =========================================================================
 *  PROJECT: Studierstube VR Slide Show system
 * ========================================================================= */
/** 
 * This file contains helper functions used by the VRSS application.
 * These functions deal with file node nuking, initialising caches of 
 * OpenInventor nodes, saving the scene graph in proper file format
 * et cetera.
 *
 * @author Jan Prikryl
 * @author Rainer Splechtna
 * @author Anton Furhmann
 * @author Dieter Schmalstieg
 *
 * 
 * $Id: tools.h 3426 2004-07-14 12:11:27Z tamer $
 * @ingroup VRSS
 * @file
 */
/* ========================================================================= */
/*@{*/

#ifndef _STBAPP_VRSS_TOOLS_H_
#define _STBAPP_VRSS_TOOLS_H_

#include <Inventor/nodes/SoNode.h>

#ifdef USE_TGS
#include <Inventor/VRMLnodes/SoVRMLSwitch.h>
#include <Inventor/VRMLnodes/SoVRMLGroup.h>
// #include <Inventor/VRMLnodes/SoVRMLColorInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLCoordinateInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLNormalInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLOrientationInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLPositionInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLScalarInterpolator.h>
// #include <Inventor/VRMLnodes/SoVRMLTimeSensor.h>
// #include <Inventor/VRMLnodes/SoVRMLPROTODef.h>
#endif

/*
 *   Function prototypes. Documented within tools.cxx
 */

void nukeFileNodes(SoNode *&root);
void writeScene(SoNode *rootToWrite, const char* name);

#ifdef USE_TGS
void cacheSlides(SoVRMLSwitch * slideSwitch, SoStudierstubeViewer *viewer);
#endif

#endif

/*@}*/
/* ===========================================================================
   End of tools.h
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   End:
=========================================================================== */
