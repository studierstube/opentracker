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
/** SoCameraControlMode.cxx contains the implementation of class SoCameraControlMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoCameraControlMode.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <cassert>
#include "SoCameraControlMode.h"

//----------------------------------------------------------------------------

SO_NODE_SOURCE(SoCameraControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization 

void 
SoCameraControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoCameraControlMode, SoNode, "Node");
}

//----------------------------------------------------------------------------
// Abstract constructor. 

SoCameraControlMode::SoCameraControlMode()
:
    camKit(NULL)
{
  SO_NODE_CONSTRUCTOR(SoCameraControlMode);
}

//----------------------------------------------------------------------------
// Abstract destructor. 

SoCameraControlMode::~SoCameraControlMode()
{
    // nil
}

//----------------------------------------------------------------------------
// Sets camKit to aCamKit.

void
SoCameraControlMode::setStereoCameraKit(SoStereoCameraKit* aCamKit)
{
    camKit = aCamKit;
    assert(camKit);
}

//----------------------------------------------------------------------------
// Returns the SoSteroCameraKit by this class.

SoStereoCameraKit*
SoCameraControlMode::getStereoCameraKit()
{
    return camKit;
}

//----------------------------------------------------------------------------
// Switch to the left camera view.

void 
SoCameraControlMode::selectCameraLeft()
{
    assert(camKit);
    camKit->selectCameraLeft();
}

//----------------------------------------------------------------------------
// Switch to the right camera view.

void
SoCameraControlMode::selectCameraRight()
{
    assert(camKit);
    camKit->selectCameraRight();
}

//----------------------------------------------------------------------------
// Returns FALSE

SbBool
SoCameraControlMode::isViewing()
{
    return FALSE; // default is false
}
//----------------------------------------------------------------------------
// Returns the position of the left eye in world space.

SbVec3f
SoCameraControlMode::getEyePositionLeft()
{
	return camKit->getCameraLeft()->eyepointPosition.getValue();
} 

//----------------------------------------------------------------------------
// Returns the display position of the left camera in world space. 

SbVec3f
SoCameraControlMode::getDisplayPositionLeft()
{
	return camKit->getCameraLeft()->position.getValue();
} 

//----------------------------------------------------------------------------
// Returns the display orientation of the left camera in world space. 

SbRotation
SoCameraControlMode::getDisplayOrientationLeft()
{
	return camKit->getCameraLeft()->orientation.getValue();
} 

//----------------------------------------------------------------------------
// Returns the position of the right eye in world space.

SbVec3f
SoCameraControlMode::getEyePositionRight()
{
	// the eyepointPosition is in world space
	return camKit->getCameraRight()->eyepointPosition.getValue();
} 

//----------------------------------------------------------------------------
// Returns the display position of the right camera in world space. 

SbVec3f
SoCameraControlMode::getDisplayPositionRight()
{
	return camKit->getCameraRight()->position.getValue();
} 

//----------------------------------------------------------------------------
// Returns the display orientation of the right camera in world space. 

SbRotation
SoCameraControlMode::getDisplayOrientationRight()
{
	return camKit->getCameraRight()->orientation.getValue();
} 

//----------------------------------------------------------------------------
//eof

/* ===========================================================================
    End of SoCameraControlMode.cxx
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
