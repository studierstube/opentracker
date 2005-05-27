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
/** source file for SoLineConstraint class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoLineConstraint.cxx 4028 2004-11-22 10:20:09Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <stbapi/misc/SoLineConstraint.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

SO_ENGINE_SOURCE(SoLineConstraint);

/**
 * Init the class
 */
void
SoLineConstraint::initClass()
{
	SO_ENGINE_INIT_CLASS(SoLineConstraint, SoConstraint, "SoConstraint");
}

/**
 * the constructor
 */
SoLineConstraint::SoLineConstraint()
{
	SO_ENGINE_CONSTRUCTOR(SoLineConstraint);

	SO_ENGINE_ADD_INPUT(lineStart, (0.0, 0.0, 0.0) );
	SO_ENGINE_ADD_INPUT(lineEnd, (10.0, 0.0, 0.0) );
	SO_ENGINE_ADD_INPUT(rasterLength, (0) );

	SO_ENGINE_ADD_OUTPUT(alpha, SoSFFloat);
}

/**
 * the destructor
 */
SoLineConstraint::~SoLineConstraint()
{
}

void SoLineConstraint::evaluate()
{
	SbVec3f lnStart = lineStart.getValue();
	SbVec3f lnEnd	= lineEnd.getValue();
	SbVec3f line = lnEnd - lnStart;
	SbVec3f transIn = translationIn.getValue();
	float raster = rasterLength.getValue();
	float a;

	// intersect a plane normal through the given point
	// with the line defined through lineStart and lineEnd

	SbVec3f proj = lnStart + line * ( line.dot(transIn - lnStart) / line.dot(line) );

	// if the point projects beyond the line's bounds,
	// it is mapped onto the closest end point.

	if ( (proj - lnStart).dot(line)<0 )
		proj = lnStart;
	else if ( (lnEnd - proj).dot(line)<0 )
		proj = lnEnd;

	// the distance of proj to lineStart in units of the
	// total line length:
	// (proj - lineStart) = a * (lineEnd - lineStart);
	a = (proj - lnStart).length() / line.length();

	// now quantize the projection to match the value
	// of rasterLength

	if (raster>0)
	{

		// the number of the raster unit, the point is inside
		int rasterNumber = (int) (a/raster + 0.5);

		// the point maps onto the center of the raster unit just found
		proj = lnStart + raster * line * (float) rasterNumber;
	}

	SO_ENGINE_OUTPUT(alpha, SoSFFloat, setValue(a) );
	SO_ENGINE_OUTPUT(translationOut, SoSFVec3f, setValue(proj) );
	SO_ENGINE_OUTPUT(rotationOut, SoSFRotation, setValue(SbRotation(0,1,0,0)) );

}


