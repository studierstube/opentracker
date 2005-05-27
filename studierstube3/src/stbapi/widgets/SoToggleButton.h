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
/** class definition of SoToggleButton
  *
  * implements a push button, is derived from SoButtonKit, basically
  * lays down, which geometry to use for the four possible states and
  * fixes the gesture, that toggles the button states.
  *
  * @author  Zsolt Marx
  *
  * $Id: SoToggleButton.h 3505 2004-07-20 11:39:36Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SO_TOGGLE_BUTTON_
#define _SO_TOGGLE_BUTTON_

#include <stbapi/widgets/SoButtonKit.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
  * implements a push button, is derived from SoButtonKit, basically
  * lays down, which geometry to use for the four possible states and
  * fixes the gesture, that toggles the button states.
  * @ingroup widgets
  */
class STBAPI_API SoToggleButton : public SoButtonKit
{
	SO_BEHAVIOR_HEADER(SoToggleButton);

public:

	/**
	 * the class initialization
	 */
	static void initClass();

	/**
	 * the constructor
	 */
	SoToggleButton();

	/**
	 * the destructor
	 */
	~SoToggleButton();

	/**
	 * the button's state-machine
	 */
	void handle3DEvent(SoHandle3DEventAction *h3a);
	
	SoSFBool onIn; ///< field for field connections to widget

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);
protected:

	SoFieldSensor onInSensor;
	static void onInSensorCB(void * data, SoSensor *sensor);

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SO_TOGGLE_BUTTON_

