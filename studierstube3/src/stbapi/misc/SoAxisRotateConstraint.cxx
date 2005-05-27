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
/** source file for SoAxisRotateConstraint class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoAxisRotateConstraint.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <stbapi/misc/SoAxisRotateConstraint.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#define PI (3.1415926535897932384626433832795)

SO_ENGINE_SOURCE(SoAxisRotateConstraint);

/**
 * Init the class
 */
void
SoAxisRotateConstraint::initClass()
{
	SO_ENGINE_INIT_CLASS(SoAxisRotateConstraint, SoEngine, "Engine");
}

/**
 * the constructor
 */
SoAxisRotateConstraint::SoAxisRotateConstraint()
{
	SO_ENGINE_CONSTRUCTOR(SoAxisRotateConstraint);

	// axis: the rotation axis

	// zeroVector: a vector lying in the plane, where the actual zeroVector
	// lies in. This plane is perpendicular with the plane, whose
	// normal vector is the axis. Ideally the value of zeroVector equals
	// the actual zeroVector, but due to eventual rounding problems,
	// we decided to allow for approximation.

	SO_ENGINE_ADD_INPUT(axis, (0.0, 1.0, 0.0) );
	SO_ENGINE_ADD_INPUT(zeroVector, (1.0, 0.0, 0.0) );
	SO_ENGINE_ADD_INPUT(maxAngle, (PI) );
	SO_ENGINE_ADD_INPUT(translationIn, (0.0, 0.0, 0.0) );
	SO_ENGINE_ADD_INPUT(rotationIn, (0.0, 0.0, 0.0, 1.0) );

	//SO_ENGINE_ADD_OUTPUT(alpha, SoSFFloat);
	SO_ENGINE_ADD_OUTPUT(translationOut, SoSFVec3f);
	SO_ENGINE_ADD_OUTPUT(rotationOut, SoSFRotation);
}

/**
 * the destructor
 */
SoAxisRotateConstraint::~SoAxisRotateConstraint()
{
}

void SoAxisRotateConstraint::evaluate()
{

	//                      rotAxis * t
	// tProjected = t - ------------------- * rotAxis
	//                   rotAxis * rotAxis
	//
	SbVec3f rotAxis = axis.getValue();
	float dAngle;

	SbVec3f transIn = translationIn.getValue();
	SbVec3f transInProjected = transIn - (rotAxis.dot(transIn)/rotAxis.dot(rotAxis)) * rotAxis;

	SbVec3f zero = zeroVector.getValue();
	SbVec3f zeroProjected = zero - (rotAxis.dot(zero)/rotAxis.dot(rotAxis)) * rotAxis;

	// calculate the rotation angle between zeroProjected and transInProjected
    //                ---->       -->
    //                start   *   end
    // cos(angle) = -------------------
    //              ||start|| * ||end||
    //

	float lengthSquared = zeroProjected.length() * transInProjected.length();

	if (lengthSquared > 0.0) // prevent division by zero
	{
		dAngle = (float)acos(zeroProjected.dot(transInProjected) / lengthSquared );
	}
	else
	{
		dAngle = 0;
	}

    // compare cross vector of zeroProjected and transInProjected to rotAxis
    // and adjust sign accordingly

	if (transInProjected.cross(zeroProjected).dot(rotAxis) > 0)
	{

		dAngle = (float)(2 * PI - dAngle);
	}

	// adjust value to be in given range

	if (maxAngle.getValue() > 0)
	{

		if (dAngle > maxAngle.getValue()) // we are in forbidden zone
		{
			// decide, whether it's closer to zeroVector or maxAngle

			float bannedRange = (float)(2 * PI - maxAngle.getValue());

			if (dAngle < maxAngle.getValue() + bannedRange/2)
				dAngle = maxAngle.getValue();
			else
				dAngle = 0;
		}
	}
	else // user wants the object to rotate into the other direction
	{	
		if (dAngle < 2*PI + maxAngle.getValue()) // we are in forbidden zone
		{
			// decide, whether it's closer to zeroVector or maxAngle

			float bannedRange = (float)(2*PI + maxAngle.getValue());

			if (dAngle < bannedRange/2)
				dAngle = 0;
			else 
				dAngle = (float)(2*PI + maxAngle.getValue());
		}
	}
      
	SO_ENGINE_OUTPUT(rotationOut, SoSFRotation, setValue(SbRotation(rotAxis, dAngle)) );

	SO_ENGINE_OUTPUT(translationOut, SoSFVec3f, setValue(0.0,0.0,0.0) );

}


