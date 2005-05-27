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
/** SoTrackedViewpointMobileDisplayControlMode.h contains the definition of class 
  * SoTrackedViewpointMobileDisplayControlMode
  *
  * @author Axel Gross
  *
  * $Id: SoTrackedViewpointMobileDisplayControlMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOTRACKEDVIEWPOINTMOBILEDISPLAYCONTROLMODE_H_
#define _SOTRACKEDVIEWPOINTMOBILEDISPLAYCONTROLMODE_H_

#include <Inventor/nodes/SoSubNode.h>

#include "SoCameraControlMode.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoTrakEngine;

/**
SoTrackedViewpointMobileDisplayControlMode provides camera control for a mobile (stereo) 
display looked at with head tracking (usually the tracker will be mounted 
on LCD shutter glasses) while also the display is being tracked.
@author Axel Gross
@ingroup camera
*/

class STBAPI_API SoTrackedViewpointMobileDisplayControlMode
:
    public SoCameraControlMode
{
SO_NODE_HEADER(SoTrackedViewpointMobileDisplayControlMode);
public:
    /** Specifies the station of the SoTrakEngine, which shall "move" the 
        viewpoint.
    */
    SoSFInt32 viewpointStation; 
	/** Specifies the station of the SoTrakEngine, which shall "move" the 
        display plane.
    */
    SoSFInt32 displayStation; 
    /** Inventor class initialization */
    static void initClass();
    /** The constructor creates and stores a reference to two SoTrakEngines 
        and connects their station to the station field of this class.
    */
    SoTrackedViewpointMobileDisplayControlMode(   
        );
    /** Returns the display station used by the internal SoTrakEngine.
        @return the value of the station field
    */
    int getDisplayTrackerStation(  
        );
	/** Returns the viewpoint station used by the internal SoTrakEngine.
        @return the value of the station field
    */
    int getViewpointTrackerStation(  
        );
    /** Calls StbCameraControlMode::setStereoCameraKit(aCamKit) and uses the 
        SoStereoCamerKit::connectHeadTracker() and 
		SoStereoCamerKit::connectDisplayTracker() method to hook up the two
		SoTrakEngines to the SoSteroCameraKit.
        @param aCamKit the SoSteroCameraKit to be used by this class
    */
    void setStereoCameraKit(          
        SoStereoCameraKit* aCamKit    
        );

private:
    /** The destructor unrefs the internal SoTrakEngine.
    */
    ~SoTrackedViewpointMobileDisplayControlMode(   
        );
    /** Provides access to internal SoTrakEngine for viewpoint position.
    */
    SoTrakEngine* viewpointTrakEngine;       
    /** Provides access to internal SoTrakEngine for display position.
    */
    SoTrakEngine* displayTrakEngine;    
};
	
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOTRACKEDVIEWPOINTMOBILEDISPLAYCONTROLMODE_H_

