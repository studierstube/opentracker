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
/** SoTrackedViewpointControlMode.cxx contains the implementation of class 
  * SoTrackedViewpointControlMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoTrackedViewpointControlMode.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <cassert>
#include <Inventor/nodes/SoTransform.h>

#include <stbapi/tracker/SoTrakEngine.h>

#include "SoTrackedViewpointControlMode.h"


SO_NODE_SOURCE(SoTrackedViewpointControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoTrackedViewpointControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoTrackedViewpointControlMode, SoCameraControlMode, "SoCameraControlMode");
}
//----------------------------------------------------------------------------
// The constructor creates and stores a reference to a new SoTrakEngine.

SoTrackedViewpointControlMode::SoTrackedViewpointControlMode()
{
    SO_NODE_CONSTRUCTOR(SoTrackedViewpointControlMode);

    SO_KIT_ADD_FIELD(station, (-1)); 

    trakEngine = new SoTrakEngine;
    trakEngine->station.connectFrom(&station);
    trakEngine->ref();
}

//----------------------------------------------------------------------------
// The destructor unrefs the internal SoTrakEngine.

SoTrackedViewpointControlMode::~SoTrackedViewpointControlMode()
{
    trakEngine->unref();
}

//----------------------------------------------------------------------------
// Returns the station used by the internal SoTrakEngine.

int
SoTrackedViewpointControlMode::getTrackerStation()
{
    return trakEngine->station.getValue();
}

//----------------------------------------------------------------------------
// Hooks up the tracker to the eyepointPositions of the left and right camera.

void
SoTrackedViewpointControlMode::setStereoCameraKit(SoStereoCameraKit* aCamKit)
{
//--first remember the camera kit and cameras
    SoCameraControlMode::setStereoCameraKit(aCamKit);

//--now just hook it up to the tracker
    assert(camKit != NULL);
	 camKit->connectHeadTracker(trakEngine);

}

//----------------------------------------------------------------------------
//eof

/* ===========================================================================
    End of SoTrackedViewpointControlMode.cxx
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
