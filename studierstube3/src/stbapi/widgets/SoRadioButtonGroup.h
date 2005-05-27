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
/** radio button group implementation
  *
  * implements a radio button group, to ease up the ways of using RadioButtons.
  * Disabeling the group will disable all buttons. Setting the value of the group will change the 
  * button states.
  *
  * @author  Thomas Psik
  *
  * $Id: SoRadioButtonGroup.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SO_RADIO_BUTTON_GROUP
#define _SO_RADIO_BUTTON_GROUP

#include <Inventor/nodes/SoGroup.h>

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoRadioButton;
/**
  * implements a radio button group, is derived from SoGroup, basically adding another interface for radiobuttons.
  * The old interfaces for SoRadioButton is still working. 

   File Format / defaults:

@verbatim

SoRadioButtonGroup {
	SoSFBool    enable			TRUE	# if true all radio buttons are enabled, if false all radiobuttons will be disabled
  	SoSFInt32   value			  -1	# value of the RadioButton group starting with 0 (only output field !! do not connect to this field !!)
	SoSFInt32   valueIn			  -1    # other scene object must set this field to change the visual representation of the group, the "value" field will also change
}
  
@endverbatim

Examples
SoRadioButtonGroup {
	valueIn 0
	enable TRUE
	SoRadioButton {}
	Translation {translation 0 0 2}
	SoRadioButton {}
	Translation {translation 0 0 2}
	SoRadioButton {}
}	


 * @ingroup widgets
 */
class STBAPI_API SoRadioButtonGroup : public SoGroup
{
	
	SO_NODE_HEADER(SoRadioButtonGroup );

public:
	SoSFInt32 value;
	SoSFInt32 valueIn;
	SoSFBool enable;


	/**
	 * the class initialization
	 */
	static void initClass();

	/**
	 * the constructor
	 */
	SoRadioButtonGroup();

	/**
	 * the destructor
	 */
	~SoRadioButtonGroup();

	void findThisButtonAndActivate(SoRadioButton *radioButton);
protected:


	SoFieldSensor enableSensor;
	SoFieldSensor valueInSensor;

	static void stateChangedCB( void * userdata, SoSensor * sensor ) ;
	static void valueChangedCB( void * userdata, SoSensor * sensor ) ;

	void adaptButtonsToState();
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SO_RADIO_BUTTON_GROUP

