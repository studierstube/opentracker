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
/** source file for SoToggleButton class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoToggleButton.cxx 4037 2004-11-23 12:26:22Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/misc/SoChildList.h>

#include <stbapi/widgets/SoToggleButton.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoToggleButton);

void SoToggleButton::initClass()
{
	SO_BEHAVIOR_INIT_CLASS(SoToggleButton, SoButtonKit, "SoButtonKit");
}

// the constructor

SoToggleButton::SoToggleButton()
{
	SO_BEHAVIOR_CONSTRUCTOR(SoToggleButton);

	SO_BEHAVIOR_ADD_FIELD(onIn, (false));

	SO_BEHAVIOR_INIT_INSTANCE();

    // set the default geometry

    setPart("onGeometry",             getGeometryFromString(SoButtonKit::buttonOnString) );
    setPart("offGeometry",            getGeometryFromString(SoButtonKit::buttonOffString) );
    setPart("highlightedOnGeometry",  getGeometryFromString(SoButtonKit::buttonHighlightedOnString) );
    setPart("highlightedOffGeometry", getGeometryFromString(SoButtonKit::buttonHighlightedOffString) );
	setPart("externalOnGeometry",	  getGeometryFromString(SoButtonKit::buttonHighlightedOnString) );
	setPart("disabledOnGeometry",	  getGeometryFromString(SoButtonKit::disabledOnString) );
	setPart("disabledOffGeometry",	  getGeometryFromString(SoButtonKit::disabledOffString) );
	
	adaptGeometryToState();

	onInSensor.setFunction( onInSensorCB );
    onInSensor.setData( this );

	setUpConnections(TRUE, TRUE);
}

// the destructor

SoToggleButton::~SoToggleButton()
{
}

SbBool SoToggleButton::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoButtonKit::setUpConnections(onOff, doItAlways);
        // setup the widget
	    onInSensor.attach( &onIn );
 
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		onInSensor.detach();
 
	    SoButtonKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
}


void SoToggleButton::handle3DEvent(SoHandle3DEventAction *h3a)
{
	// treat the button's highlight state
	if(detectAction(actionEnterGestures))
	{
 		highlighted = true;
		setHysteresis(true);
	}
	else if(detectAction(actionExitGestures))
	{
 		highlighted = false;
		setHysteresis(false);
	}

	// should not handle events when disabled
	if (enable.getValue() == FALSE) 
	{
		if(detectAction(actionEndGestures)) 
		{
			inspector->releaseGrabber(h3a);
		}
		
		highlighted.setValue(FALSE);
		// todo check on state in actionEnterGestures !!
		setHysteresis(FALSE);
		return;
	}

  // toggle the button's on/off state, if
    if(detectAction(actionEndGestures))
    {
	     inspector->releaseGrabber(h3a);
	}

  // the actionStartGesture has been performed
    if(detectAction(actionStartGestures))
    {
    	selfActivation = TRUE;
	    on.setValue( !on.getValue());
       
		if (on.getValue()) 
        {
            if(pressCallback) pressCallback(pressCallbackData, this);
        }
        else
        {
            if(releaseCallback) releaseCallback(releaseCallbackData, this);
        }
      inspector->setGrabber(h3a);
    }  
}

void SoToggleButton::onInSensorCB(void * data, SoSensor *sensor)
{
	SoToggleButton *self = (SoToggleButton *) data;
	
	if (self->on.getValue() == self->onIn.getValue()) return;

	// duplicated on event
	self->on.setValue(self->onIn.getValue());

	if (self->selfActivation == TRUE)
	{
		self->selfActivation = FALSE;
		return;
	}

	if (self->enableVisualFeedback.getValue() == FALSE)
		return;

	self->externalActivation.setValue(TRUE);

	self->timerSensor.setTimeFromNow(self->visualFeedbackHoldTime.getValue());
	self->timerSensor.schedule();
	self->adaptGeometryToState();
}

