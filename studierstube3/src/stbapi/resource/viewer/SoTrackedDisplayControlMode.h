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
/** SoTrackedDisplayControlMode.h contains the definition of class 
  * SoTrackedDisplayControlMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoTrackedDisplayControlMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOTRACKEDISPLAYCONTROLMODE_H_
#define _SOTRACKEDISPLAYCONTROLMODE_H_

#include "SoCameraControlMode.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoTrackedDisplayControlMode provides camera control via head tracking. 
The head tracker is attached to the head-mounted-display (HMD) which also 
contains the displays - hence the class name. This means that the display 
stays in a fixed relative position to the tracker and is moved together with 
the tracker. This mode is useful for all kinds of head-mounted-displays.
@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoTrackedDisplayControlMode
:
    public SoCameraControlMode
{
SO_NODE_HEADER(SoTrackedDisplayControlMode);

public:
    /** Specifys the station of the SoTrakEngine, which shall "move" the 
        display.
    */
    SoSFInt32 station; 
    /** Inventor class initialization */
    static void initClass();
    /** The constructor creates and stores a reference to a new SoTrakEngine 
        and connects their station to the station field of this class.
    */
    SoTrackedDisplayControlMode(     
        );
    /** Calls StbCameraControlMode::setStereoCameraKit(aCamKit) and hooks the 
        camKit (the SoTransform-node returned by the 
        SoStereoCamerKit::getTransform() method) up to the tracker.
        @param aCamKit the SoSteroCameraKit to be used by this class
    */
    void setStereoCameraKit(           
        SoStereoCameraKit* aCamKit     
        );
    /** Returns the station used by the internal SoTrakEngine.
        @return the value of the station field
    */
    int getTrackerStation(             
        );

    /** Calculates and returns the position of the left eye in world space.
        @return the position of the left eye in world space.
    */
    SbVec3f getEyePositionLeft( 
        );                      
    /** Calculates and returns the display position of the left camera 
        in world space.
        @return the display position of the left camera in world space.
    */
    virtual SbVec3f getDisplayPositionLeft(
        );
    /** Calculates and returns the display orientation of the left camera
        in world space.
        @return the display orientation of the left camera in world space.
    */
    virtual SbRotation getDisplayOrientationLeft(
        );

    /** Calculates and returns the position of the right eye in world space.
        @return the position of the right eye in world space.
    */
    SbVec3f getEyePositionRight(
        );    					
    /** Calculates and returns the display position of the right camera 
        in world space.
        @return the display position of the right camera in world space.
    */
    virtual SbVec3f getDisplayPositionRight(
        );  
    /** Calculates and returns the display orientation of the right camera
        in world space.
        @return the display orientation of the right camera in world space.
    */
    virtual SbRotation getDisplayOrientationRight(
        );

private:
    /** The destructor unrefs the internal SoTrakEngine. */
    ~SoTrackedDisplayControlMode(     
        );
    /** Provides access to internal SoTrakEngine. */
    SoTrakEngine* trakEngine;
    /** Transforms given 'src'-vertex from camera space to world space.
        @param src vertex to be transformed
        @return given 'src'-vertex transformed from camera space to world space.
    */
    SbVec3f transformPosition(SbVec3f src
        );
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOTRACKEDISPLAYCONTROLMODE_H_

