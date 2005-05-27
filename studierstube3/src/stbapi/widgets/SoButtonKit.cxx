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
/** source file for SoButtonKit class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoButtonKit.cxx 4047 2004-11-25 12:34:27Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <iostream>

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/interaction/SoButtonReleaseGesture.h>

#include <stbapi/widgets/SoButtonKit.h>

#include <stbapi/widgets/SoLabelKit.h>

#include <stbapi/widgets/buttonGeometry/onString.h>
#include <stbapi/widgets/buttonGeometry/offString.h>
#include <stbapi/widgets/buttonGeometry/highlightedOnString.h>
#include <stbapi/widgets/buttonGeometry/highlightedOffString.h>
#include <stbapi/widgets/buttonGeometry/disabledOnString.h>
#include <stbapi/widgets/buttonGeometry/disabledOffString.h>



#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoButtonKit);

void SoButtonKit::initClass()
{
	SO_BEHAVIOR_INIT_CLASS(SoButtonKit, SoBehaviorKit, "SoBehaviorKit");
}

// the constructor

SoButtonKit::SoButtonKit()
{
	SO_BEHAVIOR_CONSTRUCTOR(SoButtonKit);

	//-------------------------+------------+------+---------------+-------+---------------
	//                         |            |      |               | right |
	//                         |            | null |               | Sib-  | is
	//                         |            | By   |               | ling  | public
	//         name            | className  | Def. |  parent Name  | Name  | part
	//-------------------------+------------+------+---------------+-------+---------------
	ADD(onGeometry,             SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	ADD(offGeometry,            SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	ADD(highlightedOnGeometry,  SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	ADD(highlightedOffGeometry, SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	ADD(externalOnGeometry,		SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	ADD(disabledOnGeometry,     SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	ADD(disabledOffGeometry,    SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);

    SO_KIT_ADD_FIELD(actionStartGestures, (new SoButtonPressGesture()));
    SO_KIT_ADD_FIELD(actionEndGestures, (new SoButtonReleaseGesture()));
    SO_KIT_ADD_FIELD(actionEnterGestures, (new SoEnterGesture()));
    SO_KIT_ADD_FIELD(actionExitGestures, (new SoExitGesture()));

	SO_BEHAVIOR_ADD_FIELD(on, (false) );
	SO_BEHAVIOR_ADD_FIELD(highlighted, (false) );
	SO_BEHAVIOR_ADD_FIELD(externalActivation, (false));

	SO_BEHAVIOR_ADD_FIELD(enableVisualFeedback, (false));
	SO_BEHAVIOR_ADD_FIELD(visualFeedbackHoldTime, (0.7));
	

	SO_BEHAVIOR_INIT_INSTANCE();

	on.setValue(false);
	highlighted.setValue(false);

	adaptGeometryToState();

    pressCallback = NULL;
    releaseCallback = NULL;

    onSensor.setFunction( SoButtonKit::stateChangedCB );
    onSensor.setData( this );
	onSensor.setPriority( 1 );

    hlSensor.setFunction( SoButtonKit::stateChangedCB );
    hlSensor.setData( this );
    hlSensor.setPriority( 1 );

    enableSensor.setPriority( 1 );

    timerSensor.setData( this );
	timerSensor.setFunction(timerCB);
	setUpConnections(TRUE, TRUE);
}

// the destructor

SoButtonKit::~SoButtonKit()
{
}

SbBool SoButtonKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{

		// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBehaviorKit::setUpConnections(onOff, doItAlways);
        // setup the widget
		onSensor.attach( &on );
		hlSensor.attach( &highlighted );
 
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.

		onSensor.detach(  );
		hlSensor.detach( );
 
	    SoBehaviorKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);

}

/**
 * the handle3DEvent method
 */
void SoButtonKit::handle3DEvent(SoHandle3DEventAction *h3a)
{
}

/**
 * adapts the button's geometry to the button's state
 */

void SoButtonKit::adaptGeometryToState()
{
	// We could directly calculate the value of 'whichChild' from
	// the values of 'highlighted' and 'on', but to separate
	// into several cases is clearer.

	if (!externalActivation.getValue())
	{
		if (enable.getValue())
		{
			if (on.getValue())
				if (highlighted.getValue()) 
					getGeometrySwitch()->whichChild = HIGHLIGHTED_ON;
				else 
					getGeometrySwitch()->whichChild = ON;
			else
				if (highlighted.getValue()) 
					getGeometrySwitch()->whichChild = HIGHLIGHTED_OFF;
				else 
					getGeometrySwitch()->whichChild = OFF;
		}
		else
		{
			if (on.getValue())
				getGeometrySwitch()->whichChild = DISABLED_ON;
			else
				getGeometrySwitch()->whichChild = DISABLED_OFF;

		}

		if (on.getValue() )
		{
			SoLabelKit *label = (SoLabelKit *)getPart("label", FALSE);
			if (label) label->setTranslation(1);
		}
		else
		{
			SoLabelKit *label = (SoLabelKit *)getPart("label", FALSE);
			if (label) label->setTranslation(0);
		}
	
	}
	else
		getGeometrySwitch()->whichChild = EXTERNAL_ON;
}

void SoButtonKit::turnOff()
{
	on = false;
    if(releaseCallback) releaseCallback(releaseCallbackData, this);
}

void SoButtonKit::setPressCallback(SoButtonKitCB* cb, void* userData)
{
    pressCallback = cb;
    pressCallbackData = userData;
}

void SoButtonKit::setReleaseCallback(SoButtonKitCB* cb, void* userData)
{
    releaseCallback = cb;
    releaseCallbackData = userData;
}

void SoButtonKit::removePressCallback()
{
    setPressCallback(NULL);
}

void SoButtonKit::removeReleaseCallback()
{
    setReleaseCallback(NULL);
}


void SoButtonKit::stateChangedCB( void * userdata, SoSensor * sensor )
{
    assert( userdata != NULL );
    SoButtonKit * self = (SoButtonKit *) userdata;
    self->adaptGeometryToState();
}

void SoButtonKit::adaptToEnable()
{
	adaptGeometryToState();
}


void  SoButtonKit::timerCB(void * data, SoSensor *sensor)
{
	SoButtonKit *self = (SoButtonKit *) data;
	// todo : check if consistent with user !!
	self->externalActivation.setValue(FALSE);
	self->adaptGeometryToState();
}


// -------------------------------------------------------------------------------
