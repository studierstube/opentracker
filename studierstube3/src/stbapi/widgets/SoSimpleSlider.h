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
/** class definition of SoSimpleSlider
  *
  * @author  Thomas Psik
  * $Id: SoSimpleSlider.h 4047 2004-11-25 12:34:27Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_SIMPLE_SLIDER_
#define _SO_SIMPLE_SLIDER_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/engines/SoInterpolate.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/widgets/SoSlider.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The SoSimpleSlider class implements a typical 2D slider widget. It allows to set 
 * a range of values and select from that by moving the slider knob. All of
 * the widgets geometry can be parametrized with your own scenegraphs.
 *
 * The minValue and maxValue fields set the value range and the value field
 * will output the current value set. The width, height and depth fields
 * from the super class SoBehaviorKit set the size of the slider. 
 *
 * File format :
@verbatim
SoSimpleSlider {
    SoSFFloat width				1
    SoSFFloat height			1
    SoSFFloat depth				1

    SoSFFloat minValue			0
    SoSFFloat maxValue			20
 
    SoSFFloat value				0
    SoSFFloat valueIn			0
    SoSFFloat alpha				0
    SoSFFloat alphaIn			0

	SoSFFloat widthDragger		0.05
	SoSFFloat scaleDraggerPath	0.8
 
}
@endverbatim
 *
 * @ingroup widgets
 */
class STBAPI_API SoSimpleSlider : public SoSlider
{
	SO_BEHAVIOR_HEADER(SoSimpleSlider);

public:

    /// the value output of the line constraint.
	SoSFFloat valueIn;

    /// the current alpha value of the slider. Is in [0;1] and represents a normalized form of the value. This is the right field for connections.
    SoSFFloat alphaIn;  
	
	/**represents the relative distance of the maximum position of the slider knob to the end of the back plane [0;0.5].
	This value should correspond to half the width of the slider knob. */
	SoSFFloat widthDragger;
	///scale value for the region of the slider path
	SoSFFloat scaleDraggerPath;

	///inventor class initialization
	static void initClass();

	///constructor
	SoSimpleSlider();
	///destructor
	~SoSimpleSlider();
	
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways); ///< sets up the connection of the sensors

protected:
	
//	SoFieldSensor widthSensor;
//	static void widthChangedCB(void *data, SoSensor *);
	/// adapts the geometry of the slider due to a change of the value or other parameters
	virtual void resizeWidgetCB();
	
	///recognises changes of the value of the slider
	SoFieldSensor valueInSensor;
	///function that is called when the value changes
	static void valueInChangedCB(void *data, SoSensor *sensor);

	///recognises changes of the alpha of the slider
	SoFieldSensor alphaInSensor;
	///function that is called when the alpha changes
	static void alphaInChangedCB(void *data, SoSensor *sensor);

    // these strings contain the default geometry
	///contains the default geometry for the state "normal"
    static char* normalString;
	///contains the default geometry for the state "highlighted"
    static char* highlightedString;
	///contains the default geometry for the state ""
    static char* staticString;
	///contains the default geometry for the state "disabled"
    static char* disabledString;


};

#endif

