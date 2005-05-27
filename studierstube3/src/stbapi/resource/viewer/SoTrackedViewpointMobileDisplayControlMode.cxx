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
/** SoTrackedViewpointMobileDisplayControlMode.cxx contains the implementation of class 
  * SoTrackedViewpointMobileDisplayControlMode
  *
  * @author Axel Gross
  *
  * $Id: SoTrackedViewpointMobileDisplayControlMode.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoTransform.h>
#include <stbapi/util/ivio.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/tracker/SoTrakEngine.h>

#include "SoTrackedViewpointMobileDisplayControlMode.h"

SO_NODE_SOURCE(SoTrackedViewpointMobileDisplayControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoTrackedViewpointMobileDisplayControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoTrackedViewpointMobileDisplayControlMode, SoCameraControlMode,
                       "SoCameraControlMode");
}
//----------------------------------------------------------------------------
// The constructor creates and stores a reference to a new SoTrakEngine.

SoTrackedViewpointMobileDisplayControlMode::SoTrackedViewpointMobileDisplayControlMode()
{
    SO_NODE_CONSTRUCTOR(SoTrackedViewpointMobileDisplayControlMode);

    SO_KIT_ADD_FIELD(viewpointStation, (-1)); 
	SO_KIT_ADD_FIELD(displayStation, (-1)); 

    viewpointTrakEngine = new SoTrakEngine;
    viewpointTrakEngine->station.connectFrom(&viewpointStation);
    viewpointTrakEngine->ref();
	displayTrakEngine = new SoTrakEngine;
    displayTrakEngine->station.connectFrom(&displayStation);
    displayTrakEngine->ref();
}

//----------------------------------------------------------------------------
// The destructor unrefs the internal SoTrakEngine
SoTrackedViewpointMobileDisplayControlMode::~SoTrackedViewpointMobileDisplayControlMode()
{
	viewpointTrakEngine->unref();
    displayTrakEngine->unref();
}

//----------------------------------------------------------------------------
// Returns the station used by the internal SoTrakEngine for the viewpoint

int
SoTrackedViewpointMobileDisplayControlMode::getViewpointTrackerStation()
{
    return viewpointTrakEngine->station.getValue();
}

//----------------------------------------------------------------------------
// Returns the station used by the internal SoTrakEngine for the display.

int
SoTrackedViewpointMobileDisplayControlMode::getDisplayTrackerStation()
{
    return displayTrakEngine->station.getValue();
}


//----------------------------------------------------------------------------
// Hooks up the tracker to the eyepointPositions of the left and right camera.
// And does the display connections.

void
SoTrackedViewpointMobileDisplayControlMode::setStereoCameraKit(SoStereoCameraKit* aCamKit)
{
//--first remember the camera kit and cameras
    SoCameraControlMode::setStereoCameraKit(aCamKit);

//--now just hook it up to the trackers
	 camKit->connectHeadTracker(viewpointTrakEngine);
	 camKit->connectDisplayTracker(displayTrakEngine);
}

//----------------------------------------------------------------------------
//eof

/* ===========================================================================
    End of SoTrackedViewpointMobileDisplayControlMode.cxx
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
