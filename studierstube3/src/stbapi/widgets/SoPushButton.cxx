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
/** source file for SoPushButton class.
  *
  * @author Zsolt Marx, Thomas Psik
  *
  * $Id: SoPushButton.cxx 4037 2004-11-23 12:26:22Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <assert.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/misc/SoChildList.h>

#include <stbapi/widgets/SoPushButton.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>

// #define _DEBUG_PUSH_BUTTON

#if defined(_DEBUG_PUSH_BUTTON) && defined(_DEBUG)
#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif


#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoPushButton);

void SoPushButton::initClass()
{
	SO_BEHAVIOR_INIT_CLASS(SoPushButton, SoButtonKit, "SoButtonKit");
}

// -------------------------------------------------------------------------------
// the constructor

SoPushButton::SoPushButton()
{
	SO_BEHAVIOR_CONSTRUCTOR(SoPushButton);

	SO_BEHAVIOR_ADD_FIELD(triggerOut, ());	
	SO_BEHAVIOR_ADD_FIELD(triggerIn, ());
	SO_BEHAVIOR_ADD_FIELD(triggerOnUp, (TRUE));

	SO_BEHAVIOR_ADD_FIELD(repeatTrigger, (FALSE));
	SO_BEHAVIOR_ADD_FIELD(repeatTriggerStartInterval, (1.0));
	SO_BEHAVIOR_ADD_FIELD(repeatTriggerInterval, (0.5));


	SO_BEHAVIOR_INIT_INSTANCE();

    // set the default geometry

    setPart("onGeometry",             getGeometryFromString(SoButtonKit::buttonOnString) );
    setPart("offGeometry",            getGeometryFromString(SoButtonKit::buttonOffString) );
    setPart("highlightedOnGeometry",  getGeometryFromString(SoButtonKit::buttonHighlightedOnString) );
    setPart("highlightedOffGeometry", getGeometryFromString(SoButtonKit::buttonHighlightedOffString) );
	setPart("externalOnGeometry",	  getGeometryFromString(SoButtonKit::buttonHighlightedOnString) );
	setPart("disabledOnGeometry",	  getGeometryFromString(SoButtonKit::disabledOnString) );
	setPart("disabledOffGeometry",	  getGeometryFromString(SoButtonKit::disabledOffString) );
    

	getGeometrySwitch()->whichChild = OFF;

    triggerInSensor.setFunction( SoPushButton::triggerInCB );
    triggerInSensor.setData( this );
    triggerInSensor.attach( &triggerIn );

	repeatAlarmSensor.setData(this);
	repeatAlarmSensor.setFunction(SoPushButton::repeatAlarmCB);

}

// -------------------------------------------------------------------------------
// the destructor

SoPushButton::~SoPushButton()
{
}

// -------------------------------------------------------------------------------
/**
 * The SoPushButton is a one shot button:
 * it is turned on, when the actionStartGesture
 * is executed and turned immediately off, when the 
 * actionEndGesture
 * is performed.
 */
void SoPushButton::handle3DEvent(SoHandle3DEventAction *h3a)
{

	// to prevent the button's geometry to switch abruptly
	// between different geometry states, while this method
	// is executed, we set the state variables (on,highlighted)
	// first and adapt the geometry finally.

	// treat the button's highlight state
	DEBUG_CODE(printf("SoPushButton::handle3DEvent %s\n",getName().getString()););
	
	if(detectAction(actionEnterGestures))
	{
		highlighted.setValue(TRUE);
		setHysteresis(TRUE);
	}
	else if(detectAction(actionExitGestures))
	{
		highlighted.setValue(FALSE);
//		on.setValue(false);   // todo check on state in actionEnterGestures !!
		setHysteresis(FALSE);
	}

	// should not handle events when disabled
	if (enable.getValue() == FALSE) 
	{
		// disable repeatAlarmSensor
		if (repeatAlarmSensor.isScheduled())
			  repeatAlarmSensor.unschedule();

		if(detectAction(actionEndGestures)) 
		{
			inspector->releaseGrabber(h3a);
		}
		
		highlighted.setValue(FALSE);
        on.setValue(FALSE);   // todo check on state in actionEnterGestures !!
	//	setHysteresis(FALSE);  // will stop the tooltip !!

		return;
	}

	// determine, if the button has been activated

  if(on.getValue() == FALSE)
  {
    if(detectAction(actionStartGestures))
    {
     // printf("SoPushButton::handle3DEvent, pushbutton %s pressed!\n", getName().getString());
      
      on.setValue(true);
  // do callback
      if(pressCallback) pressCallback(pressCallbackData, this);
	  // fire trigger if triggerOnUp is false
	  //if (highlighted.getValue() == TRUE && triggerOnUp.getValue() == FALSE)
	  if (triggerOnUp.getValue() == FALSE)
	  {
		//  printf("SoPushButton::trigger fired !!");
		  selfActivation = TRUE;
		  triggerOut.setValue();
	  }
	  
	  if (repeatTrigger.getValue() == TRUE) 
	  {
		if (repeatAlarmSensor.isScheduled()) repeatAlarmSensor.unschedule();
		repeatAlarmSensor.setTimeFromNow(repeatTriggerStartInterval.getValue());
		repeatAlarmSensor.schedule();
	  }
      inspector->setGrabber(h3a);
    }
  }
  else  
	  // if(on.getValue() == TRUE)
  {
    if(detectAction(actionEndGestures))
    {
      // printf("SoPushButton::handle3DEvent, pushbutton %s released!\n", getName().getString());
      
      on.setValue(FALSE);
   	  
	  // do callback
	  if(releaseCallback) releaseCallback(releaseCallbackData, this);
	  // trigger if triggerOnUp is false
	  //if (highlighted.getValue() == TRUE && triggerOnUp.getValue() == TRUE)
	  if (triggerOnUp.getValue() == TRUE)
	  {
		//  printf("SoPushButton::trigger fired !!");
		  selfActivation = TRUE;
		  triggerOut.setValue();
	  }
	 
	  // disable repeatAlarmSensor
	  if (repeatAlarmSensor.isScheduled())
		  repeatAlarmSensor.unschedule();

      inspector->releaseGrabber(h3a);
    }
  }
}

// -------------------------------------------------------------------------------

void SoPushButton::triggerInCB(void * data, SoSensor *sensor)
{
	SoPushButton *self = (SoPushButton *) data;
	assert(self);
	
	// duplicated trigger event == dont do this !!
//	self->triggerOut.setValue();

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

// -------------------------------------------------------------------------------

void SoPushButton::repeatAlarmCB(void * data, SoSensor *sensor)
{
	SoPushButton *self = (SoPushButton *) data;
	assert(self);

	// setup next fire event
	self->repeatAlarmSensor.setTimeFromNow(self->repeatTriggerInterval.getValue	());
	self->repeatAlarmSensor.schedule();
	
	// fire trigger
	self->triggerOut.setValue();

}

// -------------------------------------------------------------------------------

void SoPushButton::adaptToEnable() 
{
	if (enable.getValue() == FALSE) 
		if (repeatAlarmSensor.isScheduled()) {
			repeatAlarmSensor.unschedule();

	}
	SoButtonKit::adaptToEnable();
}

