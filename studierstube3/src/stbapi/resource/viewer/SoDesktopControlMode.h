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
/** SoDesktopControlMode.h contains the definition of class SoDesktopControlMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoDesktopControlMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SODESKTOPCONTROLMODE_H_
#define _SODESKTOPCONTROLMODE_H_

#include "SoCameraControlMode.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
SoDesktopControlMode allows to control the camera with the usual Inventor 
viewer functions. This is useful for "desktop VR" or "fishtank VR" - to use 
stereo rendering together with normal, non-tracked navigation, or even for 
viewing Studierstube applications without any VR hardware. 
note: this mode is the only displayControlMode supporting a non-SoOffAxisCamera,
i.e. if an SoPerspectiveCamera or an SoOrthographicCamera is specified as 
examinerCam of the SoStereoCameraKit, it will be used to render the scene
(the SoOffAxisCameras will not be used). When the examinerCam is used, no 
stereo viewing is possible, therefore only a combination of SoDesktopControlMode 
and SoMonoDisplayMode is useful.
@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoDesktopControlMode
:
    public SoCameraControlMode
{
SO_NODE_HEADER(SoDesktopControlMode);

public:
    /** Inventor class initialization */
    static void initClass();

    /** The constructor inits the node */
    SoDesktopControlMode(             
        );
    /** Sets camKit to aCamKit and decides on use of examinerMode
        (i.e. only render the examinerCam view).
        If used with SoMonoDisplayMode it is useful to specify an examinerCam.
        If used with stereo display modes, the examinerCam of the camKit is
        set by this method and is connected to camLeft and camRight of the 
        camKit, so that the SoOffAxisCameras are moved with the movement of
        the examinerCam, which is moved by using the usual Inventor viewer 
        functions (e.g. mouse input).
        @param aCamKit the SoSteroCameraKit to be used by this class
    */
    void setStereoCameraKit(  		  
        SoStereoCameraKit* aCamKit    
        );
    /** Returns TRUE, i.e. the camera control mode does support the 
        SoGuiExaminerViewer viewing mode, and the SoStudierstubeViewer 
        shall start in viewing mode and shall allow viewer-decoration 
        (e.g. widgets for viewer control).
        @return TRUE
    */
    SbBool isViewing(	
        );              

    /** Switch to the left camera view.
        @warning if examiner mode is used no SoOffAxisCamera will be selected,
                 therefore no stereo rendering is possible.
    */
    void selectCameraLeft( 
        );
    /** Switch to the right camera view.
        @warning if examiner mode is used no SoOffAxisCamera will be selected,
                 therefore no stereo rendering is possible.
    */
    void selectCameraRight(
        );
    /** Returns the position of the left eye in world space.
        @return examinerMode FALSE: the value of the eyepointPosition-field of 
                                    the left camera 
                examinerMode TRUE:  the position-field of the examinerCam.
    */
	SbVec3f getEyePositionLeft( 
        );      
    /** Returns the position of the right eye in world space.
        @return examinerMode FALSE: the value of the eyepointPosition-field of 
                                    the right camera 
                examinerMode TRUE:  the position-field of the examinerCam.    
    */
    SbVec3f getEyePositionRight( 
        );           			
    
private:
    /** The destructor does nothing */
    ~SoDesktopControlMode(            
        );
    /** Stores the state of the examinerMode. 
        @see setStereoCameraKit()
    */
    SbBool examinerMode;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SODESKTOPCONTROLMODE_H_

