/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoTrackedDisplayControlMode.cxx contains the implementation of class 
  * SoTrackedDisplayControlMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoTrackedDisplayControlMode.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <cassert>

#include <stbapi/tracker/SoTrakEngine.h>

#include "SoTrackedDisplayControlMode.h"


SO_NODE_SOURCE(SoTrackedDisplayControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoTrackedDisplayControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoTrackedDisplayControlMode, SoCameraControlMode, "SoCameraControlMode");
}
//----------------------------------------------------------------------------
// The constructor creates and stores a reference to a new SoTrakEngine.

SoTrackedDisplayControlMode::SoTrackedDisplayControlMode()
{
    SO_NODE_CONSTRUCTOR(SoTrackedDisplayControlMode);

    SO_KIT_ADD_FIELD(station, (-1)); 

    trakEngine = new SoTrakEngine;
    trakEngine->station.connectFrom(&station);
    trakEngine->ref();

}

//----------------------------------------------------------------------------
// The destructor unrefs the internal SoTrakEngine. 

SoTrackedDisplayControlMode::~SoTrackedDisplayControlMode()
{
    trakEngine->unref();
}

//----------------------------------------------------------------------------
// Returns the station used by the internal SoTrakEngine.

int
SoTrackedDisplayControlMode::getTrackerStation()
{
    return trakEngine->station.getValue();
}

//----------------------------------------------------------------------------
// Hooks the camKit up to the tracker.

void
SoTrackedDisplayControlMode::setStereoCameraKit(SoStereoCameraKit* aCamKit)
{
//--first remember the camera
    SoCameraControlMode::setStereoCameraKit(aCamKit);

//--now just hook it up to the tracker
    assert(camKit != NULL);
    camKit->getTransform()->translation.connectFrom(&trakEngine->translation);
    camKit->getTransform()->rotation.connectFrom(&trakEngine->rotation);

}

//----------------------------------------------------------------------------
// Calculates and returns the position of the left eye in world space.

SbVec3f
SoTrackedDisplayControlMode::getEyePositionLeft()
{
	return transformPosition(camKit->getCameraLeft()->eyepointPosition.getValue());
} 

//----------------------------------------------------------------------------
// Returns the display position of the left camera in world space. 

SbVec3f
SoTrackedDisplayControlMode::getDisplayPositionLeft()
{
	return transformPosition(camKit->getCameraLeft()->position.getValue());
} 

//----------------------------------------------------------------------------
// Returns the display orientation of the left camera in world space. 

SbRotation
SoTrackedDisplayControlMode::getDisplayOrientationLeft()
{
	return camKit->getCameraLeft()->orientation.getValue() *
   	       camKit->getTransform()->rotation.getValue();
} 

//----------------------------------------------------------------------------
// Calculates and returns the position of the right eye in world space.

SbVec3f
SoTrackedDisplayControlMode::getEyePositionRight()
{
	return transformPosition(camKit->getCameraRight()->eyepointPosition.getValue());
} 

//----------------------------------------------------------------------------
// Returns the display position of the right camera in world space. 

SbVec3f
SoTrackedDisplayControlMode::getDisplayPositionRight()
{
	return transformPosition(camKit->getCameraRight()->position.getValue());
} 

//----------------------------------------------------------------------------
// Returns the display orientation of the right camera in world space. 

SbRotation
SoTrackedDisplayControlMode::getDisplayOrientationRight()
{
	return camKit->getCameraRight()->orientation.getValue() *
           camKit->getTransform()->rotation.getValue();
} 

//----------------------------------------------------------------------------
// Transforms given src-vertex from camera space to world space.

SbVec3f
SoTrackedDisplayControlMode::transformPosition(SbVec3f src)
{
	SbVec3f transformedPosition;
   	SbMatrix t2w, dummy;
    // get transformation matrix (tracker to world) from camera kit
    camKit->getTransform()->getTranslationSpaceMatrix(t2w, dummy);
    // transform position from tracker space to world space
    t2w.multVecMatrix(src, transformedPosition);

	return transformedPosition;
}

//----------------------------------------------------------------------------
//eof

/* ===========================================================================
    End of SoTrackedDisplayControlMode.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
