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
/** class definition of SoSlider
  *
  * @author  Zsolt Marx
  *
  * $Id: SoSlider.h 4047 2004-11-25 12:34:27Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_SLIDER_
#define _SO_SLIDER_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/engines/SoInterpolate.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/interaction/SoDraggerKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The SoSlider class implements a typical 2D slider widget. It allows to set 
 * a range of values and select from that by moving the slider knob. All of
 * the widgets geometry can be parametrized with your own scenegraphs.
 *
 * The minValue and maxValue fields set the value range and the value field
 * will output the current value set. The width, height and depth fields
 * from the super class SoDraggerKit set the size of the slider. 
 * The minVector and maxVector define in what range the slider's knob can move.
 * These values are in the same coordinate system as the slider's geometry.
 *
 * The parts staticSeparator, staticGeometry and staticScale allow you to
 * specify how the static body part of the slider should look like. The
 * moving knob can be specified using the parts of the super class.
 *
 * File format :
@verbatim
SoSlider {
    SoSFFloat width     1
    SoSFFloat height    1
    SoSFFloat depth     1

    SoSFFloat minValue  0
    SoSFFloat maxValue  20

    SoSFVec3f minVector 0 0 -5.2
    SoSFVec3f maxVector 0 0 5.2
    SoSFFloat valueVec3f  0 0 0
 
    SoSFFloat value       0		# represents the value of the slider (field for connections in/out)
    SoSFFloat alpha       0		# represents the alpha of the slider (field for connections in/out)
 
}
@endverbatim
 *
 * @ingroup widgets
 */
class STBAPI_API SoSlider : public SoDraggerKit
{
	SO_BEHAVIOR_HEADER(SoSlider);

	//the rest of the geometry is added here: the track etc.
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(staticGeometry);
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(staticGeometryHighlight);
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(staticGeometryDisabled);
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(staticSwitch);
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(staticSeparator);
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(staticScale);


public:

    /// the value output of the line constraint.
	/// do not connect to or set this field, and do no in-routing (field-connection to this field) internal use only! 
	SoSFFloat valueOutput;

	/// the slider's current value (this is the right field for connections)
    SoSFFloat value;    

    /// the slider's knob current position
    SoSFVec3f valueVec3f;		  

    /// the value of the slider's minimum position
	SoSFFloat minValue;
	/// the value of the slider's maximum position			
	SoSFFloat maxValue;

    /// the vector of the slider's minimum position
	SoSFVec3f minVector;	
    /// the vector of the slider's maximum position
	SoSFVec3f maxVector;      

    /// the alpha output of the line constraint.
    /// Do not connect to or set this field, and do no in-routing (field-connection to this field) internal use only! 
    SoSFFloat alphaOutput;    

    /// the current alpha value of the slider [0;1] (this is the right field for connections)
    SoSFFloat alpha;          

	static void initClass();

	SoSlider();
	virtual ~SoSlider();


    void setAlpha(float p_Alpha, bool p_Force = false);  // set the alpha value of this slider in the range of [0;1]

    float getAlpha();

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);
    virtual void adaptGeometryToState();

protected:

	/** interpolator is used to interpolate between the slider's max value
	 * and its min value using the alpha output of the SoConstrainToLine engine */
	SoInterpolateFloat *interpolator;

    SoInterpolateVec3f *interpolatorVec3f;

	
	SoFieldSensor valueSensor;
	static void valueChangedCB(void *data, SoSensor *sensor);

	SoFieldSensor alphaSensor;
	static void alphaChangedCB(void *data, SoSensor *sensor);



    // these strings contain the default geometry
    static char* normalString;
    static char* highlightedString;
    static char* staticString;
    static char* disabledString;


};

#endif

