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
/** class definition of SoPushButton
  *
  * implements a push button, is derived from SoButtonKit, basically
  * lays down, which geometry to use for the four possible states and
  * fixes the gesture, that toggles the button states.
  *
  * @author  Zsolt Marx, Thomas Psik
  *
  * $Id: SoPushButton.h 4047 2004-11-25 12:34:27Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SO_PUSH_BUTTON_
#define _SO_PUSH_BUTTON_

#include <stbapi/widgets/SoButtonKit.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
  * implements a push button, is derived from SoButtonKit, basically
  * lays down, which geometry to use for the five possible states and
  * fixes the gesture, that toggles the button states.

  CATALOG PARTS

  All Parts (from SoBehaviorKit)
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>label </td><td>SoLabelKit</td><td>SoLabelKit</td><td>YES</td></tr>
  <tr> <td>tooltip </td><td>SoTooltipKit</td><td>SoTooltipKit</td><td>YES</td></tr>
 </table>


   File Format / defaults:

@verbatim

SoPushButton {

    SoSFTrigger triggerOut							# trigger that will be fired if user activates button (depends on triggerOnUp)
	SoSFBool    triggerOnUp					TRUE	# if true the trigger will be fired on release, else the trigger will be fired on down

	SoSFBool    repeatTrigger				FALSE	# if true the trigger will be fired repeatently if the button is hold down, trigger interval can be set
	SoSFTime	repeatTriggerStartInterval	1		# if repeatTrigger is TRUE and the button is pressed longer than repeatTriggerStartInterval
													# the trigger will fire every repeatTriggerInterval
	SoSFTime    repeatTriggerInterval		0.5		# if repeatTrigger is TRUE and the button is pressed longer than repeatTriggerStartInterval
													# the trigger will fire every repeatTriggerInterval
	SoSFTrigger triggerIn							# can be used to inform the button that it was triggered external (eg. SoPucCommand) 

	# from SoButtonKit
  	SoSFBool	highlighted				FALSE	# highlight state (pen is within the button)
	SoSFBool	on						FALSE	# on state (button is pushed)

    SoSFBool    enableVisualFeedback	FALSE	# enables visual feedback
	SoSFTime	visualFeedbackHoldTime  0.7		# specify time of visual feedback (time that the button is displayed as pressed)

	# from SoBehaviorKit
  
  	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0

	SoSFFloat	hysteresisBoxScale		 1.0
	SoSFFloat   hysteresisBoxScaleHeigth 1.0
	SoSFBool	enable					TRUE	# if true the widget will be enabled else it is disabled (should not react on interaction)
	SoSFBool	applyWidgetStyle		TRUE    # if true a widget style will replace the geometry and parameters of this widget
												# else the widget will not be changed
												# else the widget will not be changed
}
  
@endverbatim

Examples

@verbatim
	# generates a PushButton
	SoPushButton { 
	
	} 

	# generates a bigger PushButton
	SoPushButton { 
		width 5 
		depth 3 
		heigth 1
	} 

	# generates a PushButton, that is pushed longer than 2 seconds, will fire the trigger every 0.1 seconds (10 times in a second)
	SoPushButton { 
		repeatTrigger TRUE 
		repeatTriggerStartInterval 2 
		repeatTriggerInterval .1 
	} 

  	# generates a disabled PushButton
	SoPushButton { 
		enabled FALSE
	} 

	# generates a PushButton that will fire the trigger when it is pressed, not waiting for a release action 
	SoPushButton { 
		triggerOnUp FALSE
	} 
@endverbatim

  * @ingroup widgets
  */
class STBAPI_API SoPushButton : public SoButtonKit
{
	SO_BEHAVIOR_HEADER(SoPushButton);

public:

	/**
	 * the class initialization
	 */
	static void initClass();

	/**
	 * the constructor
	 */
	SoPushButton();

	/**
	 * the destructor
	 */
	~SoPushButton();

	/**
	 * the button's state-machine
	 */
	void handle3DEvent(SoHandle3DEventAction *h3a);

	/// trigger that will be fired if user activates button (depends on triggerOnUp)
	SoSFTrigger triggerOut;				
	SoSFBool    triggerOnUp;			///< if true the trigger will be fired on release, else the trigger will be fired on down

	SoSFTrigger triggerIn;				///< can be used to inform the button that it was triggered external (eg. SoPucCommand) 

	SoSFBool    repeatTrigger;				///< if true the trigger will be fired repeatently if the button is hold down, trigger interval can be set
	SoSFTime    repeatTriggerStartInterval; ///< if repeatTrigger is TRUE and the button is pressed longer than repeatTriggerStartInterval, the trigger will fire every repeatTriggerInterval
	SoSFTime    repeatTriggerInterval;      ///< if repeatTrigger is TRUE and the button is pressed longer than repeatTriggerStartInterval, the trigger will fire every repeatTriggerInterval


protected:
	SoFieldSensor triggerInSensor;
	static void triggerInCB(void * data, SoSensor *sensor);
	
	SoAlarmSensor repeatAlarmSensor;
	static void repeatAlarmCB(void * data, SoSensor *sensor);
	
	 virtual void adaptToEnable();
	
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SO_PUSH_BUTTON_

