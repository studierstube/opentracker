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
/** class definition of StbEventInspector
  *
  * The EventInspector is a helper class to be used
  * as a component in SoBehavior-Objects to handle
  * low level events delivered to the objects
  * in a hi level form.
  *
  * @author  Zsolt Marx
  *
  * $Id: StbEventInspector.h 4113 2004-12-21 14:27:35Z breiting $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STB_EVENT_INSPECTOR_
#define _STB_EVENT_INSPECTOR_

#include <stbapi/interaction/SoGesture.h>
#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/event3d/Sb3DEventHistory.h>

#include <Inventor/SbBasic.h>
#include <Inventor/nodes/SoTransform.h>

#include <queue>

class SoBehaviorKit;

/**
 * The EventInspector is a helper class to be used
 * as a component in SoBehavior-Objects to handle
 * low level events delivered to the objects
 * in a hi level form.
 * @ingroup event
 * @ingroup interaction
 */
class STBAPI_API StbEventInspector
{

public:

	/**
	 * the standard constructor
	 */
	StbEventInspector();

	/**
	 * the constructor
	 */
	StbEventInspector(SoBehaviorKit *behavKit);

	/**
	 * the destructor
	 */
	~StbEventInspector();

	/**
	 * returns whether the gesture was performed or not,
	 * only checks for gesture, if necessary (in case gesture
	 * needs an update)
	 */
	SbBool detect(SoGesture *gesture);

	/**
	 * hands the current event over to StbEventInspector
	 * for inspection.
	 */
	void examineEvent(SoHandle3DEventAction *action);

	/**
	 * returns the current pen translation extracted
	 * from the current event.
	 */
	SbVec3f getPenTranslation();

	/**
	 * returns the current pen rotation extracted
	 * from the current event.
	 */
	SbRotation getPenRotation();

	/**
	 * checks, if the event just handed, is inside the
	 * SoBehaviorKit's bounding box, result is cached
	 */
	SbBool isInside();

	SbBool isInside(SoHandle3DEventAction *h3a);

	/**
	 * returns whether the current BehaviorKit's bounding box
	 * has just been exited.
	 */
	SbBool justExited();

	/**
	 * returns whether the current BehaviorKit's bounding box
	 * has just been entered.
	 */
	SbBool justEntered();

	/**
	 * adds the given gesture to the gesture list
	 */
	void registerGesture(SoGesture &gesture);

	/**
	 * This function interprets the transition from the last tracker
	 * state to the current state ( we are informed about those state
	 * changes through events ) as dragging, that was just performed
	 * on our SoBehaviorKit. AlignWithPen() takes the kit's old transformation
	 * and changes it, to make the kit follow the pen and give the user
	 * the impression, that he has picked the kit and is dragging it around.
	 *
	 * @param kitOldTrafo our SoBehaviorKit's transformation at the time of
	 *                    the last event
	 * @param pickOffsetL the point where the SoBehaviorKit is held during 
	 *                    the dragging in the kit's local coordinates
	 * @param globalToLocal The matrix transforming from global coordinates to
	 *                      the kit's local coordinate system. We need this
	 *                      matrix, because the coordinates in the events are
	 *                      global.
	 *
	 * @todo change the SoHandle3DEventAction so that the globalToLocal matrix
	 *       is automatically assembled during the scene graph traversal. In
	 *       that case the third parameter would be redundant, since we
	 *       possess a pointer to the SoHandle3DEventAction.
	 */
	void alignWithPen(	SoSFRotation& kitCurrentRot,
						SoSFVec3f& kitCurrentTrans,
						SbRotation& kitInitRotL,
						SbRotation& penInitRotG,
						const SbVec3f& pickOffsetL);

    /**
     * makes kitCurrentTrans follow the pen's translation and ignores the pen's
     * rotation
     */
    void translateWithPen( SoSFVec3f& kitCurrentTrans);

	SbMatrix getGlobalToLocalMatrix();

    float getAxisRotationAngle(SbVec3f axis);
    
    float getAxisRotationAngle(SbVec3f axis, SbMatrix multMatrix);

	/**
	 * If the hysteresis effect is turned on, the inspector memorizes the path
	 * to the behavior kit's current geometry. From now on - independent from
	 * the kit's actual geometry displayed - the memorized geometry is used for
	 * bounding box calculations, until the hysteresis is turned off - then the bounding
	 * box comes back to the normal.
	 */
	void setHysteresis(bool on);

	void setGrabber(SoHandle3DEventAction *h3a);

	void releaseGrabber(SoHandle3DEventAction *h3a);

	SbBool isInterested();

private:

	/**
	 * contains the path from the root to the behavior
	 * kit, to which this eventInspector belongs.
	 */
	const SoPath *pathFromRoot;

	/**
	 * the behaviorKit, this EventInspector belongs to
	 */
	SoBehaviorKit *behaviorKit;

	/**
	 * is extended, when a new event arrives
	 */
	Sb3DEventHistory *history;

  /**
   * current SoHandle3DEventAction, that is set
   * handled if a gesture triggers
   */
  SoHandle3DEventAction *current3DEventAction;

	/**
	 * list of gestures, primarily to set them outdated,
	 * when a new event has arrived and to "not outdated",
	 * if one has been calculated
	 */
	SbPList gestureList;

	SbBool bboxNeedsUpdate;

	/**
	 * bbox caches the associated behavior kit's bounding
	 * box
	 */
	SbXfBox3f bbox;
	  
	/**
	 * returns bounding box and updates, if necessary
	 */
	SbXfBox3f& getBoundingBox();

	/**
	 * hysteresisGeometry contains the child's index that
	 * should be traversed by the behavior kit's geometry switch
	 * instead of the actual active child, if the hysteresis
	 * effect is turned on
	 */
	bool isHysteresisOn;
    float hysteresisScaleFactor;

	/**
	 * This is the number of past events, the event history
	 * object stores
	 */
	static const int historySize;

	static const int current;
	static const int previous;
	static const double epsilon;

	SbBool lastEventInside;
	SbBool entered;

	SbBool interest;


};

#endif //_STB_EVENT_INSPECTOR_
