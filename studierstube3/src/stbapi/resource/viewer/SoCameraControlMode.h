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
/** SoCameraControlMode.h contains the definition of class SoCameraControlMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoCameraControlMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOCAMERACONTROLMODE_H_
#define _SOCAMERACONTROLMODE_H_

#include <Inventor/nodes/SoSubNode.h>

#include "SoStereoCameraKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
SoCameraControlMode is responsible for managing the way a user controls the 
camera in a Studierstube application. It is an abstract base class 
representing the camera control interface to SoStudierstubeViewer. 
The main task of this type of class is to manipulate the SoStereoCameraKit 
used by the invoking SoStudierstubeViewer, hence the camera control mode and 
the SoStereoCameraKit are closely linked together.

@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoCameraControlMode : public SoNode
{
SO_NODE_HEADER(SoCameraControlMode);

protected:
    /** Abstract constructor. */
    SoCameraControlMode(              
        );
    /** Abstract destructor. */
    virtual ~SoCameraControlMode(     
        );

public:
    /** Inventor class initialization */
    static void initClass();
    /** Sets camKit to aCamKit.
        @param aCamKit the SoSteroCameraKit to be used by this class
    */
    virtual void setStereoCameraKit(  
        SoStereoCameraKit* aCamKit    
        );
    /** Returns the SoSteroCameraKit used by this class.
        @return the SoSteroCameraKit used by this class.
    */
    virtual SoStereoCameraKit* getStereoCameraKit( 
        );
    /** Switch to the left camera view.
    */
    virtual void selectCameraLeft(     
        );
    /** Switch to the right camera view.
    */
    virtual void selectCameraRight(    
        );
    /** Returns TRUE, if the camera control mode does support the 
        SoGuiExaminerViewer viewing mode, i.e. if the SoStudierstubeViewer 
        shall start in viewing mode and shall allow viewer-decoration 
        (e.g. widgets for viewer control).
        @return FALSE by default
    */
    virtual SbBool isViewing(		
        );                       

    /** Returns the position of the left eye in world space.
        @return the value of the eyepointPosition-field of the left camera.
    */
    virtual SbVec3f getEyePositionLeft(
        );           				
    /** Returns the display position of the left camera in world space. 
        @return the value of the position-field of the left camera.
    */
    virtual SbVec3f getDisplayPositionLeft(
        );
    /** Returns the display orientation of the left camera in world space. 
        @return the value of the orientation-field of the left camera.
    */
    virtual SbRotation getDisplayOrientationLeft(
        );

    /** Returns the position of the right eye in world space.
        @return the value of the eyepointPosition-field of the right camera.
    */
    virtual SbVec3f getEyePositionRight( 
        );           	
    /** Returns the display position of the right camera in world space. 
        @return the value of the position-field of the right camera.
    */
    virtual SbVec3f getDisplayPositionRight(
        );  
    /** Returns the display orientation of the right camera in world space. 
        @return the value of the orientation-field of the right camera.
    */
    virtual SbRotation getDisplayOrientationRight(
        );

protected:
    /** The internal reference to the used SoStereoCameraKit 
        (the cameras that are controlled by this object)
    */
    SoStereoCameraKit* camKit;         
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOCAMERACONTROLMODE_H_

