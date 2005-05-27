/* =========================================================================
 *  (c) 2000,2001 VRVis GmbH, Vienna, Austria
 *  (c) 1999,2000 Vienna University of Technology
 * =========================================================================
 *  PROJECT: Studierstube VR Slide Show system
 * ========================================================================= */
/** 
 * Callbacks used in VRSS.
 * The callbacks provided by this module are responsible mainly for moving 
 * between slides in the presentation. Some additional call backs responsible
 * for scene graph saving and proper transparent texturing are part of this
 * module.
 *
 * @author Jan Prikryl
 * 
 * $Id: callbacks.h 3426 2004-07-14 12:11:27Z tamer $
 * @ingroup VRSS
 * @file
 */
/* ========================================================================= */
/*@{*/

#ifndef _STBAPP_VRSS_CALLBACKS_H_
#define _STBAPP_VRSS_CALLBACKS_H_

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <stbapi/widgets/So3DButton.h>

/*
 *   Function prototypes. Documented within callbacks.cxx
 */

void myKeyPressCB      (void *, SoEventCallback *);
void saveSceneCB       (void *, SoEventCallback *);
void myEndTransitionCB (void *, SoSensor *);
void buttonNextCB      (void *, So3DButton *);
void buttonPrevCB      (void *, So3DButton *);
void buttonFirstCB     (void *, So3DButton *);
void transparentTexturingCB (void *, SoAction *);
void myReadCB (const SbString &url, void *userData, SoWWWInline *node);

#endif

/*@}*/
/* ===========================================================================
   End of callbacks.h
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
