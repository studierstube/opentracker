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
/** class definition of SoButtonKit
  *
  * class to be used when the application developer wants to add button-like
  * behavior to one of the scene objects. 
  *
  * @author  Zsolt Marx
  *
  * $Id: SoButtonKit.h 4047 2004-11-25 12:34:27Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SO_BUTTON_KIT_
#define _SO_BUTTON_KIT_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>


#include <stbapi/interaction/SoBehaviorKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoButtonKit;
class SoPipSheetKit;

typedef void SoButtonKitCB(void* userData, SoButtonKit* buttonKit);

/**
 * SoButtonKit is the class to be used, when
 * the application developer wants to add
 * button-like behavior to one of the scene objects
 *
 * The button's size can be determined with the width,
 * height and depth field. These fields expect the
 * supplied geometry to be in a unit cube centered
 * at (0,0,0), else the proportions do not match.

  CATALOG PARTS

  All Parts (from SoBehaviorKit)
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>label </td><td>SoLabelKit</td><td>SoLabelKit</td><td>YES</td></tr>
  <tr> <td>tooltip </td><td>SoTooltipKit</td><td>SoTooltipKit</td><td>YES</td></tr>
 </table>

File Format / defaults:

@verbatim

SoButtonKit {

  	SoSFBool	highlighted				FALSE	# highlight state (pen is within the button)
	SoSFBool	on						FALSE	# on state (button is pushed)

    SoSFBool    enableVisualFeedback	FALSE	# enables visual feedback
	SoSFTime	visualFeedbackHoldTime  0.7		# specify time of visual feedback (time that the button is displayed as pressed)

	# from SoBehaviorKit
  
  	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0

	SoSFFloat	hysteresisBoxScale		 1.0    # in percent of widget dimensions (1.0 means 100%)
	SoSFFloat   hysteresisBoxScaleHeight 1.0    # in percent of widget height (1.0 means 100%)
	SoSFBool	enable					TRUE	# if true the widget will be enabled else it is disabled (should not react on interaction)
	SoSFBool	applyWidgetStyle		TRUE    # if true a widget style will replace the geometry and parameters of this widget
												# else the widget will not be changed
  }
   
@endverbatim

Examples

  DO NOT INSTATIATE THIS CLASS !!
	 
 * @ingroup widgets
 */

class STBAPI_API SoButtonKit : public SoBehaviorKit
{
	friend class SoPipSheetKit;
	SO_BEHAVIOR_HEADER(SoButtonKit);

	/// SoSeparator - add normal on geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(onGeometry);

	/// SoSeparator - add normal off geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(offGeometry);

	/// SoSeparator - add highlighted on geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(highlightedOnGeometry);

	/// SoSeparator - add highlighted off geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(highlightedOffGeometry);

	/// SoSeparator - add external activation geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(externalOnGeometry);

	/// SoSeparator - add disabled on geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(disabledOnGeometry);

	/// SoSeparator - add disabled off geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(disabledOffGeometry);


public:

	/**
	 * 'highlighted' and 'on' describe the button's state
	 */
	SoSFBool highlighted;
	SoSFBool on;
	
	/**
	 * is true when the widget was triggered by an external event (means not by the user), most probably a PUC state
	 */
	SoSFBool	externalActivation;
	
	/**
	 * enables visual feedback from an external event (means not by the user), most probably a PUC state
	 */
	SoSFBool    enableVisualFeedback;	
	
	/**
	 * specify time of visual feedback (time that the button is displayed as pressed)
	 */
	SoSFTime	visualFeedbackHoldTime; 

	/**
	 * If the user enters the buttons volume, if becomes highlighted,
	 * Depending on its state before highlighting, it is either
	 * HIGHLIGHTED_ON or HIGHLIGHTED_OFF
	 */
	enum GeometryState
	{
		ON = 0,
		OFF,
		HIGHLIGHTED_ON,
		HIGHLIGHTED_OFF,
		EXTERNAL_ON,
		DISABLED_ON,
		DISABLED_OFF
	};

	/**
	 * the class initialization
	 */
	static void initClass();

	/**
	 * the constructor
	 */
	SoButtonKit();

	/**
	 * the destructor
	 */
	~SoButtonKit();

	/**
	 * our handle event method
	 */
	void handle3DEvent(SoHandle3DEventAction* h3a);

	void turnOff();

  void setPressCallback(SoButtonKitCB* cb,void* userData = NULL);
  void setReleaseCallback(SoButtonKitCB* cb, void* userData = NULL);

  void removePressCallback();
  void removeReleaseCallback();

  virtual void adaptToEnable();



	/**
	 * sets the gestures that toggle the button's on state
	 * 
	 * toggle button & radio button:
	 *                startGesture turns button on
	 *                startGesture turns button off
	 * push button:   
	 *                startGesture turns button on
	 *					endGesture turns button off
	 */
	//void setActionGestures(SoGesture *startGesture, SoGesture *endGesture);

	/**
	 * Adapt the button's geometry to the button's state
	 * described by the 'highlighted' and 'on' fields
	 */
	void adaptGeometryToState();

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

protected:

  SoMFNode actionStartGestures;
  SoMFNode actionEndGestures;
  SoMFNode actionEnterGestures;
  SoMFNode actionExitGestures;

  SoButtonKitCB * pressCallback;
  SoButtonKitCB * releaseCallback;

  void *pressCallbackData;
  void *releaseCallbackData;

  static char* buttonOnString;
  static char* buttonOffString;
  static char* buttonHighlightedOnString;
  static char* buttonHighlightedOffString;
  static char* disabledOnString;
  static char* disabledOffString;

  SoFieldSensor onSensor;
  SoFieldSensor hlSensor;

  static void stateChangedCB( void * userdata, SoSensor * sensor );

  SoAlarmSensor timerSensor;
  static void timerCB(void * data, SoSensor *sensor);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SO_BUTTON_KIT_

