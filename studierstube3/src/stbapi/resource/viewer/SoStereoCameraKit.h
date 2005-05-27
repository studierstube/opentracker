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
/** SoStereoCameraKit.h contains the definition of class SoStereoCameraKit
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoStereoCameraKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOSTEREOCAMERAKIT_H_
#define _SOSTEREOCAMERAKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/nodes/SoSwitch.h>

#include "SoOffAxisCamera.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoTrakEngine;

/**
SoStereoCameraKit represents a node kit with two (stereo) SoOffAxisCameras 
(one for the left eye (camLeft part) and one for the right eye (camRight 
part)) "mounted" on a virtual "rack" that can be freely positioned 
(camXf part). Using this kit, a viewer can manipulate (move) two cameras 
simultaneously, and switch cameras at will for rendering (camSwitch part). 
Additionally a user can hook up trackers directly to the cameras, so that 
the eyepoint of the cameras and/or the display (display surface) is tracked. 
When using these direct connections, offsets for both cameras should be 
specified, otherwise no stereo effect will be achieved.
When using connectHeadTracker specify eyeOffsetXXXX. 
When using connectDisplayTracker specify displayOffsetXXXX and 
displayRotationXXXX.

There are also two context nodes included that will always set the Eye context
to either Left or Right, @see StudierstubContext for the specification of
these indices.

@warning: when using direct connections, certain fields of both cameras 
are overwritten: eypointPosition(head) and/or position, orientation(display), 
i.e. the initial values of these fields are not relevant.

SoStereoCameraKit structure:

                   (root)
                     |
                 +---+---------+
                 |             | 
         SoStereoCameraKit (userScene)
                 |
               xfSep
                 |
        +--------+-----------+
        |        |           |
   examinerCam camXf      camSwitch
                             |
                       +-----+-----+
                       |           |
                   groupLeft     groupRight
                    |                 |
              +-----+-----+     +-----+-----+
              |           |     |           |
         contextLeft  camLeft contextRight camRight


@warning The only parts that should be specified by the user are:
camLeft:  the SoOffAxisCamera used to render the view for the left eye,
camRight: the SoOffAxisCamera used to render the view for the right eye,
examinerCam: the camera "seen" and edited by the SoGuiExaminerViewer 
(the parent class of the SoStudierstubeViewer). Specifying this part is 
only useful for an SoMonoDisplayMode - SoDesktopControlMode combination.
@see SoDesktopControlMode
@ingroup camera
@ingroup core
*/
class STBAPI_API SoStereoCameraKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoStereoCameraKit);
    SO_KIT_CATALOG_ENTRY_HEADER(xfSep);
    SO_KIT_CATALOG_ENTRY_HEADER(camXf);
    SO_KIT_CATALOG_ENTRY_HEADER(camSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(groupLeft);
    SO_KIT_CATALOG_ENTRY_HEADER(contextLeft);
    SO_KIT_CATALOG_ENTRY_HEADER(camLeft);
    SO_KIT_CATALOG_ENTRY_HEADER(groupRight);
    SO_KIT_CATALOG_ENTRY_HEADER(contextRight);
    SO_KIT_CATALOG_ENTRY_HEADER(camRight);
    SO_KIT_CATALOG_ENTRY_HEADER(examinerCam);
		
public:

     /** Offset between head tracker and left eye
         (in head tracker space).
     */
	 SoSFVec3f eyeOffsetLeft;	
     /** Offset between head tracker and right eye
         (in head tracker space).
     */
	 SoSFVec3f eyeOffsetRight;	
     /** Offset between display tracker and left display surface center
         (in display tracker space).
     */
	 SoSFVec3f displayOffsetLeft;	
     /** Offset between display tracker and right display surface center
         (in display tracker space).
     */
	 SoSFVec3f displayOffsetRight;	
     /** Rotation from display tracker orientation to 
         left display surface orientation
     */
	 SoSFRotation displayRotationLeft;
     /** Rotation from display tracker orientation to 
         right display surface orientation
     */
	 SoSFRotation displayRotationRight;

    /** The constructor inits the fields of the kit */
    SoStereoCameraKit(          
        );
    /** The destructor does nothing */
    ~SoStereoCameraKit(               
        );
    /** Inventor class initialization */
    static void initClass(            
        );
    /** Switches to camera for left eye */
    void selectCameraLeft(             
        );
    /** Switches to camera for right eye */
    void selectCameraRight(         
        );
    /** Returns left camera.
        @return left camera
    */
    SoOffAxisCamera* getCameraLeft(    
        );
    /** Returns right camera.
        @return right camera
    */
    SoOffAxisCamera* getCameraRight(   
        );
    /** Returns TRUE, if left camera is active.
        @return TRUE, if left camera is active.
    */
    SbBool isActiveCameraLeft(    
        );
    /** Returns TRUE, if right camera is active.
        @return TRUE, if right camera is active.
    */
    SbBool isActiveCameraRight(    
        );
    /** Returns "the rack", i.e. the camXf part.
        @return the camXf part
    */
    SoTransform* getTransform(		
        );		
    /** Returns the camera, which is presented to the SoGuiExaminerViewer
        (parent class of SoStudierstubeViewer) as it's camera.
        @return the camera edited by the SoGuiExaminerViewer
    */
    SoCamera *getExaminerCamera(	
		  );
    /** Returns the camera switch.
        @return the camera switch
    */
    SoSwitch *getStereoSwitch(	
		  );

    /** By using the method connectHeadTracker, the given tracker is connected 
        to the eyepointPosition fields of both cameras using an engine 
        network, that calculates the current eyepointPositions, using input 
        from the tracker and the specified eyeOffsets.
        @param tracker the trakEngine ("head tracker") to connect from
    */
	void connectHeadTracker(SoTrakEngine *tracker);
    /** Alternative method of connecting the head tracker, by specifying
        a translation and a rotation field to connect from.
        @param trackerTranslation the translation to connect from
        @param trackerRotation the rotation to connect from
    */
	void connectHeadTracker(SoSFVec3f *trackerTranslation, 
									SoSFRotation *trackerRotation);
    /** By using the method connectDisplayTracker, the given tracker is 
        connected to the position and orientation fields of the cameras 
        using an engine network, that calculates the current position and 
        orientation of the left and right display surface, using input from 
        the tracker and the specified displayOffsets and displayRotations.
 
        @param tracker the trakEngine ("display tracker") to connect from
    */
	void connectDisplayTracker(SoTrakEngine *tracker);

    /** Alternative method of connecting the display tracker, by specifying
        a translation and a rotation field to connect from.
        @param trackerTranslation the translation to connect from
        @param trackerRotation the rotation to connect from
    */
	void connectDisplayTracker(SoSFVec3f *trackerTranslation,
										SoSFRotation *trackerRotation);
    /** Destructs the used engine network and hence breaks up the connection 
        between the head tracker and the fields of the cameras.
     */
	void disconnectHeadTracker();
    /** Destructs the used engine network and hence breaks up the connection 
        between the display tracker and the fields of the cameras.
    */
	void disconnectDisplayTracker();

private:
    /** Auxiliary method for setting up an engine network. 
        Used by the connectHeadTracker() methods.
        @param ctw engine calculating the tracker to world transformation 
               matrix
    */
	void connectHeadTrackerStep2(SoComposeMatrix *ctw);
    /** Auxiliary method for setting up an engine network. 
        Used by the connectDisplayTracker() methods.
        @param ctw engine calculating the tracker to world transformation 
               matrix
    */
	void connectDisplayTrackerStep2(SoComposeMatrix *ctw);

};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif


//----------------------------------------------------------------------------
#endif//_SOSTEREOCAMERAKIT_H_

