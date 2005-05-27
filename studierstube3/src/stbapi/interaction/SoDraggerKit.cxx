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
/** source file for SoDraggerKit class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoDraggerKit.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <Inventor/SbBox.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/misc/SoChildList.h>

#include <stbapi/interaction/SoDraggerKit.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/interaction/SoButtonReleaseGesture.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY 

SO_KIT_SOURCE(SoDraggerKit);

/**
 * @todo: this is not a class member. where does this come from?
 */
void SoDraggerKit::handle3DEventS(SoAction *action, SoNode* node)
{
	SoHandle3DEventAction *h3a = (SoHandle3DEventAction *) action;
	SoDraggerKit *behavior = (SoDraggerKit *) node;

    // deliver the event to the event receiver geometries first

    behavior->getChildren()->traverse(h3a);

    // examine the event yourself, if it wasn't consumed by
    // the event receivers

    if (!h3a->isHandled())
    {
	    behavior->examineEvent( (SoHandle3DEventAction *) h3a );

	    if (behavior->isInterested())
	    {
		    behavior->handle3DEvent(h3a);
		    h3a->setHandled();
	    }
    }
}

void SoDraggerKit::initClass(void)
{
    SO_BEHAVIOR_INIT_CLASS(SoDraggerKit, SoBehaviorKit, "SoBehaviorKit");
}

/**
 * constructor
 */
SoDraggerKit::SoDraggerKit()
:
    startCallback(NULL),
    finishCallback(NULL),
    moveCallback(NULL),
    startCallbackData(NULL),
    finishCallbackData(NULL),
    moveCallbackData(NULL)
{
    SO_BEHAVIOR_CONSTRUCTOR(SoDraggerKit);

	//-----------------------+------------+------+---------------+-------+---------------
	//                       |            |      |               | right |
	//                       |            | null |               | Sib-  | is
	//                       |            | By   |               | ling  | public
	//         name          | className  | Def. |  parent Name  | Name  | part
	//-----------------------+------------+------+---------------+-------+---------------
    ADD(constrainedTransform, SoTransform, FALSE, base,           scale,  true);
    ADD(normalGeometry,       SoSeparator, FALSE, geometrySwitch, \x0, TRUE);
	ADD(highlightedGeometry,  SoSeparator, FALSE, geometrySwitch, \x0, TRUE);
	ADD(draggedGeometry,      SoSeparator, FALSE, geometrySwitch, \x0, TRUE);
    ADD(disabledGeometry,     SoSeparator, FALSE, geometrySwitch, \x0, TRUE);

    SO_KIT_DEFINE_ENUM_VALUE(DragPoint, PICKPOINT);
    SO_KIT_DEFINE_ENUM_VALUE(DragPoint, ORIGIN);
    SO_KIT_SET_SF_ENUM_TYPE(dragPoint, DragPoint);

    SO_BEHAVIOR_ADD_FIELD(translation, (0,0,0));
    SO_BEHAVIOR_ADD_FIELD(rotation, (0,0,0,1));
    SO_BEHAVIOR_ADD_FIELD(dragButton, (0));
    SO_BEHAVIOR_ADD_FIELD(dragPoint, (ORIGIN));
    SO_BEHAVIOR_ADD_FIELD(highlighted, (FALSE));
    SO_BEHAVIOR_ADD_FIELD(dragging,    (FALSE));


    SO_BEHAVIOR_INIT_INSTANCE();

	// set up the highlightSwitch
    SoSwitch* sw = (SoSwitch*)geometrySwitch.getValue();    
    sw->whichChild = NORMAL;

	// ensure that a search is performed inside the kit as well
    setSearchingChildren(TRUE);

	// init globalToLocal
    globalToLocal.makeIdentity();

	enableSensor.setFunction(stateChangedCB );
    enableSensor.setData( this );
    enableSensor.attach( &enable );
    enableSensor.setPriority( 1 );

}

/**
 * destructor
 */
SoDraggerKit::~SoDraggerKit()
{
}

/**
 * setting up the connections:
 *              ,----------,            ,-----------,
 * translation->|          |---->alpha->|           |
 *    rotation->|constraint|  minValue->|interpolate|->value
 *              |          |  maxValue->|           |
 *              |          |            `-----------´
 *              |          |-------------------------->[trafo->trans]
 *              |          |-------------------------->[trafo->rot]
 *              `----------´              
 */
void SoDraggerKit::connectConstraint()
{

	// create and connect constraint input to translation and rotation
	constraint->translationIn.connectFrom(&translation);
	constraint->rotationIn.connectFrom(&rotation);

	// connect constraint output to transform
    SoTransform* constrainedTrafo = (SoTransform*)constrainedTransform.getValue();
    constrainedTrafo->translation.connectFrom(&constraint->translationOut);
    constrainedTrafo->rotation.connectFrom(&constraint->rotationOut);
}

/**
 * deals with incoming 3D events
 */
void SoDraggerKit::handle3DEvent(SoHandle3DEventAction* h3a)
{

 	SoMoveGesture move;
	SoEnterGesture enter;
	SoExitGesture exit;
	SoButtonPressGesture buttonPress(dragButton.getValue());
	SoButtonReleaseGesture buttonRelease(dragButton.getValue());

	// treat the button's highlight state

	if( inspector->detect(&enter) )
	{
 		setHysteresis(true);
	}
	else if( inspector->detect(&exit) )
	{
 		setHysteresis(false);
	}

	// should not handle events when disabled
	if (enable.getValue() == FALSE) return;

	if ( inspector->detect(&enter) )
	{
		highlighted.setValue(TRUE );
	} 
	else if (highlighted.getValue() == TRUE && inspector->detect(&buttonPress) )
	{
		dragging = true;

		inspector->setGrabber(h3a);

		kitInitialTransL	= translation.getValue();
		kitInitialRotL		= rotation.getValue();

		penInitialTransG	= inspector->getPenTranslation();
		penInitialRotG		= inspector->getPenRotation();
        
		// determine the pickOffset

        if (dragPoint.getValue() == PICKPOINT)
        {
		    SbVec3f penInitialTransL;
		    inspector->getGlobalToLocalMatrix().multVecMatrix(penInitialTransG, penInitialTransL);

		    pickOffsetL = penInitialTransL - kitInitialTransL;
        }
        else if (dragPoint.getValue() == ORIGIN)
        {
            pickOffsetL.setValue(0,0,0);
        }


		// fire the startCallback
		if(startCallback) startCallback(startCallbackData, this);
	}
	else if(dragging.getValue() == TRUE && inspector->detect(&buttonRelease) )
	{

		if (!inspector->isInside()) highlighted.setValue(FALSE);
        dragging = false;

		inspector->releaseGrabber(h3a);

		// fire the finishCallback
		if(finishCallback) finishCallback(finishCallbackData, this);

	}
	else if( dragging.getValue() == TRUE && inspector->detect(&move) )
	{

	    inspector->alignWithPen(   rotation, 
		    					   translation,
								   kitInitialRotL,
                                   penInitialRotG,
								   pickOffsetL);

		//inspector->translateWithPen(translation);

		// fire the move callback
		if(moveCallback) moveCallback(moveCallbackData, this);

    }
	else if (inspector->detect(&exit))
	{
		highlighted.setValue(FALSE);
	}


    adaptGeometryToState();
}

void SoDraggerKit::stateChangedCB(void * data, SoSensor *sensor)
{
	assert(data);
	((SoDraggerKit *)data)->adaptGeometryToState();
}

/**
 * adapts the dragger's geometry to its state
 */
void SoDraggerKit::adaptGeometryToState()
{
	if (enable.getValue() == TRUE)
	{
		if (dragging.getValue() == TRUE) // either dragging or dragging and highlighted
			getGeometrySwitch()->whichChild = DRAGGED;
		else if (highlighted.getValue() == TRUE) // highlighted and not dragging
			getGeometrySwitch()->whichChild = HIGHLIGHTED;
		else
			getGeometrySwitch()->whichChild = NORMAL;
	}
	else
		getGeometrySwitch()->whichChild = DISABLED;

}


void SoDraggerKit::setStartCallback(SoDraggerKitCB* cb, void* userData)
{
    startCallback = cb;
    startCallbackData = userData;
}

void SoDraggerKit::setFinishCallback(SoDraggerKitCB* cb, void* userData)
{
    finishCallback = cb;
    finishCallbackData = userData;
}

void SoDraggerKit::setMoveCallback(SoDraggerKitCB* cb, void* userData)
{
    moveCallback = cb;
    moveCallbackData = userData;
}

void SoDraggerKit::removeStartCallback()
{
    setStartCallback(NULL);
}

void SoDraggerKit::removeFinishCallback()
{
    setFinishCallback(NULL);
}

void SoDraggerKit::removeMoveCallback()
{
    setMoveCallback(NULL);
}

