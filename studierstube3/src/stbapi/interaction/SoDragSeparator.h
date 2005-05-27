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
//  PROJECT:    Studierstube
//  CONTENT:    Class definition of SoDragSeparator
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw      Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  $Id: SoDragSeparator.h 3425 2004-07-14 12:09:17Z tamer $
//  @file
// ===========================================================================


#ifndef _SODRAGSEPARATOR_H_
#define _SODRAGSEPARATOR_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <stbapi/event3d/Base3D.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/stbapi.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoDragSeparator;

// callback prototype for SoDragSeparator
typedef void SoDragSeparatorCB(void*, SoDragSeparator*);


/** Use this node if you need to move around geometry in your workspace. 
 * When the button of your input-device is held down inside the bounding box 
 * of the geometry, the node calculates translational and rotational values,
 * so that the geometry follows the movements of the device. 
 * To achieve this behaviour, add a SoTransform node as first child of the
 * SoDragSeparator and connect its fields translation and rotation with those
 * from the SoDragSeparator (this should be extended to a nodekit somewhen...)
 * As a last step, add the geometry to the separator node. 
 *
 * Do not use this node anymore. It does not work correctly, if used under
 * a transformation. See SoDragKit or SoDraggerKit for working versions.
 *
 * @ingroup interaction
 * @ingroup deprecated
 */
class STBAPI_API SoDragSeparator
:
     public SoSeparator, public Base3D 
{

    SO_NODE_HEADER(SoDragSeparator);
    BASE3D_HEADER;

public:
	SoSFVec3f translation;           //relative translation to drag start
	SoSFRotation rotation;           //relative rotation to drag start
	SoSFInt32 dragButton;            //number of drag button
    SoSFBool dragging;               //true if in dragging mode

	static void initClass(           //static class initialization
        );
	SoDragSeparator(                 //constructor
        );
	virtual SbBool isInterested(     //true if node is interested in 3d event
        SoHandle3DEventAction* h3a   //action owning 3d event
        );
	virtual void handle3DEvent(      //deal with 3d event
        SoHandle3DEventAction* h3a   //action owning 3d event
        );
    void setPressCallback(           //set callback for picking geometry
        SoDragSeparatorCB* func,     //callback function
        void* data=NULL              //user supplied
        );
    void removePressCallback(        //remove callback for picking geometry
        );
    void setReleaseCallback(         //set callback for releasing geometry
        SoDragSeparatorCB* func,     //callback function
        void* data=NULL              //user supplied
        );
    void removeReleaseCallback(      //remove callback for releasing geometry
        );
    void startDragging(              //immediately go into dragging mode
        );
    void stopDragging(               //immediately exit dragging mode
        );

    void updateFromNetwork( //set translation and rotation via network
		SbVec3f trans,
        SbRotation rot
		);
    static void snapBack(void *data,
		SoSensor *sensor
		);

private:
	~SoDragSeparator(                //destructor
        );

	//SbBool dragging;                 //true if in dragging mode
    SbBool startDrag;                //true if having to enter dragging mode
    SbBool stopDrag;                 //true if having to exit dragging mode
    SoDragSeparatorCB* pressFunc;    //pointer to press callback
    SoDragSeparatorCB* releaseFunc;  //pointer to release callback
    void* pressData;                 //user data for press callback
    void* releaseData;               //user data for release callback
    SbVec3f transOffset;             //translational offset when dragging
    SbVec3f oldTranslation;          //translation from last frame
    SbRotation oldRotation;          //rotation from last frame
    SbMatrix oldMatrixInverse;       //matrix inverse from last frame
	SbVec3f currentSetTranslation;   //current valid translation (from network)
	SbRotation currentSetRotation;   //current valid rotation (from network)
    SoAlarmSensor *snapBackSensor;
    SbBool validFlag;
}; //SoDragSeparator

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SODRAGSEPARATOR_H_
