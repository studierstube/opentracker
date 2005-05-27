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
/** SoTrackedViewpointControlMode.h contains the definition of class 
  * SoTrackedViewpointControlMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoTrackedViewpointControlMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOTRACKEDVIEWPOINTCONTROLMODE_H_
#define _SOTRACKEDVIEWPOINTCONTROLMODE_H_

#include "SoCameraControlMode.h"

class SoTrakEngine;

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
SoTrackedViewpointControlMode provides camera control for a fixed stereo 
display looked at with head tracking (usually the tracker will be mounted 
to LCD shutter glasses). The display surface stays at a fixed position, while 
the eye(s) move as indicated by the head tracker. This means that the position 
of the eyes constantly changes and the off-axis camera must be updated 
accordingly.

@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoTrackedViewpointControlMode
:
    public SoCameraControlMode
{
SO_NODE_HEADER(SoTrackedViewpointControlMode);
public:
    /** Specifys the station of the SoTrakEngine, which shall "move" the 
        viewpoint.
    */
    SoSFInt32 station; 
    /** Inventor class initialization */
    static void initClass();
    /** The constructor creates and stores a reference to a new SoTrakEngine 
        and connects their station to the station field of this class.
    */
    SoTrackedViewpointControlMode(   
        );
    /** Returns the station used by the internal SoTrakEngine.
        @return the value of the station field
    */
    int getTrackerStation(  
        );
    /** Calls StbCameraControlMode::setStereoCameraKit(aCamKit) and uses the 
        SoStereoCamerKit::connectHeadTracker() method to hook up the tracker 
        to the eyepointPositions of the left and right camera.
        @param aCamKit the SoSteroCameraKit to be used by this class
    */
    void setStereoCameraKit(          
        SoStereoCameraKit* aCamKit    
        );

private:
    /** The destructor unrefs the internal SoTrakEngine.
    */
    ~SoTrackedViewpointControlMode(   
        );
    /** Provides access to internal SoTrakEngine.
    */
    SoTrakEngine* trakEngine;         
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOTRACKEDVIEWPOINTCONTROLMODE_H_

