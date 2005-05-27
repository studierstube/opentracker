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
/** class definition of SoDraggerKit
  *
  * base class for objects, that can be grabbed,
  * and then shifted and rotated. SoDraggerKit allows
  * to specify a starting and and end gesture, i.e. a
  * certain sequence of events that must happen in order
  * to initiate or to end dragging. The scene object's
  * rotational and translational degrees of freedom
  * can be limited by using constraints.
  *
  * @author  Zsolt Marx
  *
  * $Id: SoDraggerKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SODRAGGERKIT_H_
#define _SODRAGGERKIT_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/event3d/Base3D.h>
#include <stbapi/misc/SoConstraint.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/interaction/SoSubBehavior.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class STBAPI_API SoDraggerKit;

typedef void SoDraggerKitCB(void* userData, SoDraggerKit* dragKit);

/**
 * base class for objects, that can be grabbed,
 * and then shifted and rotated. SoDraggerKit allows
 * to specify a starting and and end gesture, i.e. a
 * certain sequence of events that must happen in order
 * to initiate or to end dragging. The scene object's
 * rotational and translational degrees of freedom
 * can be limited by using constraints.
 * @ingroup interaction
 */
class STBAPI_API SoDraggerKit : public SoBehaviorKit
{
    SO_BEHAVIOR_HEADER(SoDraggerKit);    

    /**
	 * SoTransform - used for dragging
	 */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(constrainedTransform);

    /**
	 * SoSeparator - add your content here
	 */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(normalGeometry);

    /**
	 * SoSeparator - add content for highlighted display here
	 */
    SO_BEHAVIOR_CATALOG_ENTRY_HEADER(highlightedGeometry);

	/**
	 * SoSeparator - add geometry for grabbed dragger here
	 */
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(draggedGeometry);

	/**
	 * SoSeparator - add geometry for grabbed dragger here
	 */
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(disabledGeometry);


public:

    /**
     * specifies the point on the drag kit, that
     * touches the pen: either the pick point or
     * the kit's origin
     */
    SoSFEnum dragPoint;

	enum GeometryState
	{
		NORMAL = 0,
		HIGHLIGHTED,
		DRAGGED,
		DISABLED
	};

    enum DragPoint
    {
        PICKPOINT = 0,
        ORIGIN
    };

	/**
	 * static class initialization
	 */
    static void initClass();

	/**
	 * constructor
	 */
    SoDraggerKit();

	/**
	 * destructor
	 */
    ~SoDraggerKit();

	/**
	 * current translation in local coordinates
	 */
    SoSFVec3f translation;

	/**
	 * current rotation in local coordinates
	 */
    SoSFRotation rotation;

	/**
	 * button to drag with
	 */
    SoSFInt32 dragButton;

	SoSFBool     highlighted;				// true if geometry highlighted
    SoSFBool     dragging;				// true if manipulating the geometry

	/**
	 * deals with incoming 3D events
	 */
	void handle3DEvent(SoHandle3DEventAction* h3a);
        
    void setStartCallback(SoDraggerKitCB* cb,void* userData = NULL);
    void setFinishCallback(SoDraggerKitCB* cb, void* userData = NULL);
    void setMoveCallback(SoDraggerKitCB* cb, void* userData = NULL);

    void removeStartCallback();
    void removeFinishCallback();
    void removeMoveCallback();

protected:

	/**
	 * responsible for the mapping of the pen coordinates onto the
	 * dragged object's coordinates
	 */
	SoConstraint		*constraint;

	void connectConstraint();

    SbVec3f		kitInitialTransL;				// original kit translation, local
    SbRotation	kitInitialRotL;					// original kit rotation,    local
    SbVec3f		penInitialTransG;				// original pen translation, global
    SbRotation	penInitialRotG;					// original pen rotation,    global
    SbVec3f		pickOffsetL;				// offset to pen-kit origin, local
    SbMatrix	globalToLocal;				// global to local transformation

    SoDraggerKitCB * startCallback;
    SoDraggerKitCB * finishCallback;
    SoDraggerKitCB * moveCallback;


    void * startCallbackData;
    void * finishCallbackData;
    void * moveCallbackData;
    
	static void stateChangedCB(void * data, SoSensor *sensor);
    virtual void adaptGeometryToState();

}; //class SoDraggerKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SODRAGGERKIT_H_


