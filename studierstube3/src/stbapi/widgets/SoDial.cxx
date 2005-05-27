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
/** source file for SoDial class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoDial.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/widgets/SoDial.h>
#include <stbapi/widgets/dialGeometry/normalString.h>
#include <stbapi/widgets/dialGeometry/highlightedString.h>
#include <stbapi/misc/SoConstraint.h>

#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/interaction/SoButtonReleaseGesture.h>

#define PI 3.1415926535897932384626433832795

SO_BEHAVIOR_SOURCE(SoDial);

/**
 * the open inventor initialization
 */
void SoDial::initClass()
{
    SO_BEHAVIOR_INIT_CLASS(SoDial, SoDraggerKit, "SoDraggerKit");
}

// callback, that reports, whenever a value has changed
static void	valueChangedCB(void *data, SoSensor *)
{
  // SoSFFloat *newValue = (SoSFFloat *) data;
}

/**
 * the constructor inits all fields
 */
SoDial::SoDial()
{

    SO_BEHAVIOR_CONSTRUCTOR(SoDial);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY( name,className,nullByDefault,parentName,rightSiblingName,isPublic  )

    SO_BEHAVIOR_ADD_CATALOG_ENTRY( staticGeometry, SoSeparator, TRUE, base, geometrySwitch, FALSE);

    // add fields
    SO_BEHAVIOR_ADD_FIELD(minValue, (-10.0) ); 
    SO_BEHAVIOR_ADD_FIELD(maxValue, (30.0) ); 
    SO_BEHAVIOR_ADD_FIELD(axis, (0.0, 1.0, 0.0) );
	SO_BEHAVIOR_ADD_FIELD(startAngle, (0.0) );
    SO_BEHAVIOR_ADD_FIELD(endAngle, (4*PI) );
    SO_BEHAVIOR_ADD_FIELD(value, (0.0) ); 

    SO_BEHAVIOR_INIT_INSTANCE();

	// setting up the connection between the value and the
	// valueChanged callback

	valueChangedSensor = new SoFieldSensor(valueChangedCB, &value);
	valueChangedSensor->attach(&value);

    currentAngle = startAngle.getValue();

    // set up the default geometry

    setPart("normalGeometry", getGeometryFromString(normalString));
    setPart("highlightedGeometry", getGeometryFromString(highlightedString));
    setPart("draggedGeometry", getGeometryFromString(highlightedString));

}

SoTransform *SoDial::getTransform()
{
    return (SoTransform *) constrainedTransform.getValue();
}


/**
 * deals with incoming 3D events
 */
void SoDial::handle3DEvent(SoHandle3DEventAction* h3a)
{

 	SoMoveGesture move;
	SoEnterGesture enter;
	SoExitGesture exit;
	SoButtonPressGesture buttonPress(dragButton.getValue());
	SoButtonReleaseGesture buttonRelease(dragButton.getValue());

  	if ( inspector->detect(&enter) )
	{
		highlighted.setValue(TRUE);
	} 
	else if (inspector->detect(&buttonPress) )
	{
		dragging.setValue(TRUE);

		inspector->setGrabber(h3a);

/*		kitInitialTransL	= translation.getValue();
		kitInitialRotL		= rotation.getValue();

		penInitialTransG	= inspector->getPenTranslation();
		penInitialRotG		= inspector->getPenRotation();

		// determine the pickOffset

		SbVec3f penInitialTransL;
		inspector->getGlobalToLocalMatrix().multVecMatrix(penInitialTransG, penInitialTransL);
		pickOffsetL = kitInitialTransL - penInitialTransL;*/


		// fire the startCallback
		if(startCallback) startCallback(startCallbackData, this);
	}
	else if(inspector->detect(&buttonRelease) )
	{

		if (!inspector->isInside()) highlighted = false;

		dragging.setValue(FALSE);
		inspector->releaseGrabber(h3a);


		// fire the finishCallback

		if(finishCallback) finishCallback(finishCallbackData, this);

	}
	else if( dragging.getValue() == TRUE && inspector->detect(&move) )
	{
        // determine the current angle

        currentAngle += inspector->getAxisRotationAngle(axis.getValue());

        if (currentAngle < startAngle.getValue()) currentAngle = startAngle.getValue();
        else if (currentAngle > endAngle.getValue()) currentAngle = endAngle.getValue();

        // apply it to the current rotation

        getTransform()->rotation.setValue( SbRotation(-axis.getValue(), currentAngle) );

        // reflect the current angle in the dial's value

        float start = startAngle.getValue();
        float end   = endAngle.getValue();
        float minVal = minValue.getValue();
        float maxVal = maxValue.getValue();

        value.setValue( (currentAngle-start)/(end-start) * (maxVal - minVal) + minVal);

		// fire the move callback

		if(moveCallback) moveCallback(moveCallbackData, this);

    }
	else if (inspector->detect(&exit))
	{
		highlighted.setValue(FALSE);
	}

    adaptGeometryToState();

}

SoDial::~SoDial()
{
}


