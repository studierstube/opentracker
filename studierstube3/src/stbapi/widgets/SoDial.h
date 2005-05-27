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
/** class definition of SoDial
  * 
  * @author  Zsolt Marx
  *
  * $Id: SoDial.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_DIAL_
#define _SO_DIAL_

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
 * @ingroup widgets
 */
class STBAPI_API SoDial : public SoDraggerKit
{
	SO_BEHAVIOR_HEADER(SoDial);

	//the rest of the geometry is added here: the track etc.
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(staticGeometry);

public:

	SoSFFloat value;				// the slider's current value

	SoSFFloat minValue;				// the value of the slider's minimum position
	SoSFFloat maxValue;				// the value of the slider's maximum position

    SoSFFloat startAngle;
    SoSFFloat endAngle;

	SoSFVec3f axis;
	SoSFVec3f zeroVector;

	static void initClass();

    void handle3DEvent(SoHandle3DEventAction *h3a);

	SoDial();
	~SoDial();

protected:

    SoTransform* getTransform();

private:

	// interpolator is used to interpolate between the slider's max value
	// and its min value using the alpha output of the SoConstrainToLine engine
	SoInterpolateFloat *interpolator;
	
	SoFieldSensor *valueChangedSensor;

    float currentAngle;

    static char *highlightedString;
    static char *normalString;

};

#endif

