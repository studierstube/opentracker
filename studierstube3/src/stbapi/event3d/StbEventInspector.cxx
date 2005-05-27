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
/** source file for StbEventInspector class.
  *
  * @author Zsolt Marx
  *
  * $Id: StbEventInspector.cxx 4113 2004-12-21 14:27:35Z breiting $
  * @file                                                                   */
 /* ======================================================================= */
 
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetMatrixAction.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/event3d/StbEventInspector.h>
#include <stbapi/util/ivio.h>

const int StbEventInspector::historySize = 198;
const int StbEventInspector::current = 0;
const int StbEventInspector::previous = 2;
const double StbEventInspector::epsilon = 0.0000001;

/**
 * the standard constructor
 */
StbEventInspector::StbEventInspector()
{
	bboxNeedsUpdate = TRUE;


}

/**
 * the constructor
 */
StbEventInspector::StbEventInspector(SoBehaviorKit *behavKit)
{
	bboxNeedsUpdate = true;
	lastEventInside = false;
	isHysteresisOn = false;
	interest = false;
	entered = false;

	behaviorKit = behavKit;
	history = new Sb3DEventHistory(historySize);
    hysteresisScaleFactor = 4.0;

}

/**
 * the destructor
 */
StbEventInspector::~StbEventInspector()
{
	delete(history);
}

/**
 * returns whether the gesture was performed or not,
 * only checks for gesture, if necessary (in case gesture
 * needs an update)
 */
SbBool StbEventInspector::detect(SoGesture *gesture)
{
  SbBool val = gesture->detect(*history, bbox);
  if(val == TRUE)
    current3DEventAction->setHandled();
  return(val);

	//return gesture->detect(*history, bbox);
}

/**
 * checks, if the event just handed, is inside the
 * SoBehaviorKit's bounding box, result is cached
 */
SbBool StbEventInspector::isInside()
{


#ifdef DEBUG
	if (history->getEvent(current) == NULL)
	{
		SoDebugError::post("StbEventInspector::isInside","history is NULL");
		return;
	}
#endif

	if (history->getEvent(current))
		return getBoundingBox().intersect( history->getEvent(current)->getTranslation() );
	else
		return false;
}

SbBool StbEventInspector::isInside(SoHandle3DEventAction *h3a)
{
	So3DEvent *event = (So3DEvent *) h3a->getEvent();

#ifdef DEBUG
	if (event == NULL)
	{
		SoDebugError::post("StbEventInspector::isInside","history is NULL");
		return;
	}
#endif

	if (event)
		return getBoundingBox().intersect( event->getTranslation() );
	else
		return false;
}

/**
 * returns whether the current BehaviorKit's bounding box
 * has just been exited.
 */
SbBool StbEventInspector::justExited()
{
	return detect(new SoExitGesture());
}

/**
 * returns whether the current BehaviorKit's bounding box
 * has just been entered.
 */
SbBool StbEventInspector::justEntered()
{
	return detect(new SoEnterGesture());
}

/**
 * adds the given gesture to the gesture list
 */
void StbEventInspector::registerGesture(SoGesture &gesture)
{
}

/**
 * adds a copy of the given event to it's history
 * and sets it as the current event
 *
 * @todo only change if the new event has ocurred later than
 *       the current event.
 */
void StbEventInspector::examineEvent(SoHandle3DEventAction *h3a)
{
  current3DEventAction = h3a;

	So3DEvent *newEvent = (So3DEvent *) h3a->getEvent();
	int eventType = newEvent->getType();

	bboxNeedsUpdate = true;
	pathFromRoot = h3a->getPathFromRoot(); // is necessarily needed by getBoundingBox

	// check if the behavior is interested at all

	interest = true;

	getBoundingBox();

	// add current BoundingBox in the history to the event
	history->addEvent(newEvent, &bbox);
}

/**
 * calculates the kit's bounding box, if necessary,
 * caches the result
 */
SbXfBox3f& StbEventInspector::getBoundingBox()
{

#ifdef DEBUG
	if (behaviorKit == NULL)
	{
		SoDebugError::post("StbEventInspector::getBoundingBox","behaviorKit NULL");
		return;
	}
#endif

	SbViewportRegion vp;
	SoGetBoundingBoxAction bba(vp);

	if (bboxNeedsUpdate)
	{


		pathFromRoot->ref();
		bba.apply((SoFullPath *) pathFromRoot);
		pathFromRoot->unrefNoDelete();

    	bbox = bba.getXfBoundingBox();


		bboxNeedsUpdate = false;
	}

	return bbox;
}

/**
 * returns the current pen translation extracted
 * from the current event.
 */
SbVec3f StbEventInspector::getPenTranslation()
{
	return (history->getCurrentEvent()->getTranslation());
}

/**
 * returns the current pen rotation extracted
 * from the current event.
 */
SbRotation StbEventInspector::getPenRotation()
{
	return (history->getCurrentEvent()->getRotation());
}

void StbEventInspector::setHysteresis(bool on)
{
	isHysteresisOn = on;
}

 /**
  * alignWithPen() takes the kit's old transformation
  * and changes it, to make the kit follow the pen and give the user
  * the impression, that he has picked the kit and is dragging it around.
  */
void StbEventInspector::alignWithPen(	SoSFRotation& kitCurrentRot,
										SoSFVec3f& kitCurrentTrans,
										SbRotation& kitInitRotL,
				   	                 	SbRotation& penInitRotG,
										const SbVec3f& pickOffsetL)
{

	// get the global to local matrix

	SbMatrix globalToLocal = getGlobalToLocalMatrix();
	
	// get the relevant events

	const SbRotation penCurrentRotG = history->getCurrentEvent()->getRotation();

	// calculate the relative global rotation

	SbRotation relRotG = penInitRotG.inverse() * penCurrentRotG;

	// calculate the relative local rotation

	SbMatrix relRotGMatrix;
	relRotG.getValue(relRotGMatrix);

	SbMatrix relRotLMatrix;
	relRotLMatrix = globalToLocal.inverse() * relRotGMatrix * globalToLocal;

	SbVec3f t; SbVec3f s; SbRotation so; SbRotation relRotL;
	relRotLMatrix.getTransform(t,relRotL,s,so);

	// calculate the absolute local rotation of the kit

	kitCurrentRot = kitInitRotL * relRotL;

	// rotate the offset vector

	SbVec3f rotPickOffsetL;
	relRotL.multVec(pickOffsetL,rotPickOffsetL);

    // calculate the pen's current local translation

	SbVec3f penCurrentTransG = history->getCurrentEvent()->getTranslation();

    SbVec3f penCurrentTransL;
	globalToLocal.multVecMatrix(penCurrentTransG, penCurrentTransL);

    // calculate the kit's current local translation

    SbVec3f kitCurrentTransL;
	kitCurrentTransL.setValue(0.0, 0.0, 0.0);

    // kit translation caused by pen translation

	kitCurrentTransL += penCurrentTransL;
    kitCurrentTransL -= pickOffsetL;

    // kit translation caused by pen rotation

	kitCurrentTransL -= rotPickOffsetL;
	kitCurrentTransL += pickOffsetL;

	kitCurrentTrans = kitCurrentTransL;


}

void StbEventInspector::translateWithPen( SoSFVec3f& kitCurrentTrans )
{
	// global pen Translation

    SbVec3f penCurrentTransG = history->getCurrentEvent()->getTranslation();

	// local pen Translation

    SbVec3f penCurrentTransL;
    getGlobalToLocalMatrix().multVecMatrix(penCurrentTransG, penCurrentTransL);

    kitCurrentTrans = penCurrentTransL;
}

/**
 * determines the current globalToLocal matrix
 */
SbMatrix StbEventInspector::getGlobalToLocalMatrix()
{

		SbViewportRegion vp;
		SoGetMatrixAction ma(vp);

		pathFromRoot->ref();
		ma.apply((SoFullPath*)pathFromRoot);
		pathFromRoot->unrefNoDelete();

		return ma.getInverse();
}

                                     
float StbEventInspector::getAxisRotationAngle(SbVec3f rotAxis)
{
    if (history->getCurrentEvent() && history->getPreviousEvent())
    {
		SbMatrix globalToLocal = getGlobalToLocalMatrix();

        // determine the pen's current translation in local coordinates
        const SbVec3f endG = history->getCurrentEvent()->getTranslation();
        SbVec3f endL;
        globalToLocal.multVecMatrix(endG, endL);

        // determine the pen's previous translation in local coordinates
        const SbVec3f startG = history->getPreviousEvent()->getTranslation();
        SbVec3f startL;
        globalToLocal.multVecMatrix(startG, startL);

        // project start and end into axis plane
	    SbVec3f startProjected = startL - (rotAxis.dot(startL)/rotAxis.dot(rotAxis))*rotAxis;
	    SbVec3f endProjected = endL - (rotAxis.dot(endL)/rotAxis.dot(rotAxis))*rotAxis;

	    // calculate the rotation angle:
        //                ---->       -->
        //                start   *   end
        // cos(angle) = -------------------
        //              ||start|| * ||end||
        //

		float cosAngle = startProjected.dot(endProjected)/(startProjected.length() * endProjected.length());

		if (cosAngle > 1) cosAngle = 1.0;
		else if (cosAngle < -1) cosAngle = -1.0;

		float dAngle = acos(cosAngle);

	    if (endProjected.cross(startProjected).dot(rotAxis) < 0)
	    {

            // compare cross vector of zeroProjected and transInProjected to rotAxis
            // and adjust sign accordingly

		    dAngle = - dAngle;
	    }

        return dAngle;

    }
    else 
    {
        return 0;
    }
}

// [VC] added version with additional multMatrix, for NodeKits witch
// have an additional transform node

float StbEventInspector::getAxisRotationAngle(SbVec3f rotAxis, SbMatrix multMatrix)
{
    if (history->getCurrentEvent() && history->getPreviousEvent())
    {
        SbMatrix globalToLocal = getGlobalToLocalMatrix();
        globalToLocal *= multMatrix.inverse();

        // determine the pen's current translation in local coordinates
        const SbVec3f endG = history->getCurrentEvent()->getTranslation();
        SbVec3f endL;
        globalToLocal.multVecMatrix(endG, endL);

        // determine the pen's previous translation in local coordinates
        const SbVec3f startG = history->getPreviousEvent()->getTranslation();
        SbVec3f startL;
        globalToLocal.multVecMatrix(startG, startL);

        // project start and end into axis plane
	    SbVec3f startProjected = startL - (rotAxis.dot(startL)/rotAxis.dot(rotAxis))*rotAxis;
	    SbVec3f endProjected = endL - (rotAxis.dot(endL)/rotAxis.dot(rotAxis))*rotAxis;

	    // calculate the rotation angle:
        //                ---->       -->
        //                start   *   end
        // cos(angle) = -------------------
        //              ||start|| * ||end||
        //

		float cosAngle = startProjected.dot(endProjected)/(startProjected.length() * endProjected.length());

		if (cosAngle > 1) cosAngle = 1.0;
		else if (cosAngle < -1) cosAngle = -1.0;

		float dAngle = acos(cosAngle);

	    if (endProjected.cross(startProjected).dot(rotAxis) < 0)
	    {

            // compare cross vector of zeroProjected and transInProjected to rotAxis
            // and adjust sign accordingly

		    dAngle = - dAngle;
	    }

        return dAngle;

    }
    else 
    {
        return 0;
    }
}

void StbEventInspector::setGrabber(SoHandle3DEventAction *h3a)
{
   	h3a->setGrabber((SoPath*) pathFromRoot);
}

void StbEventInspector::releaseGrabber(SoHandle3DEventAction *h3a)
{
	h3a->releaseGrabber();
}

SbBool StbEventInspector::isInterested()
{
	return interest;
}
