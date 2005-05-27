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
/** source file for SoBoxConstraint class.
  *
  * @author Zsolt Marx
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <stbapi/misc/SoBoxConstraint.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

SO_ENGINE_SOURCE(SoBoxConstraint);

/**
 * Init the class
 */
void SoBoxConstraint::initClass()
{
	SO_ENGINE_INIT_CLASS(SoBoxConstraint, SoEngine, "Engine");
}

/**
 * the constructor
 */
SoBoxConstraint::SoBoxConstraint()
{
	SO_ENGINE_CONSTRUCTOR(SoBoxConstraint);

	SO_ENGINE_ADD_INPUT(boxStart, (0.0, 0.0, 0.0) );
	SO_ENGINE_ADD_INPUT(boxEnd, (10.0, 0.0, 0.0) );
	SO_ENGINE_ADD_INPUT(translationIn, (0.0, 0.0, 0.0) );
	SO_ENGINE_ADD_INPUT(rotationIn, (0.0, 0.0, 0.0, 1.0) );

	SO_ENGINE_ADD_OUTPUT(alpha, SoSFVec3f);
	SO_ENGINE_ADD_OUTPUT(translationOut, SoSFVec3f);
	SO_ENGINE_ADD_OUTPUT(rotationOut, SoSFRotation);
}

/**
 * the destructor
 */
SoBoxConstraint::~SoBoxConstraint()
{
}

void SoBoxConstraint::evaluate()
{

	SbVec3f bxStart = boxStart.getValue();
	SbVec3f bxEnd	= boxEnd.getValue();
	SbVec3f transIn = translationIn.getValue();
    SbVec3f proj;
    int coord;
	SbVec3f a;

    for (coord = 0; coord < 3; coord++)
    {
        // determine the lower and the upper Bound
        // for the current coordinate

        float lowerBound;
        float upperBound;

        if (bxStart[coord] < bxEnd[coord]) 
        {
            lowerBound = bxStart[coord];
            upperBound  = bxEnd[coord];
        }
        else
        {
            lowerBound = bxEnd[coord];
            upperBound  = bxStart[coord];
        }

        // if the incoming translation is outside the bounds 
        // project it inside , else leave the translation unchanged

        if ( transIn[coord] < lowerBound )
        {
            proj[coord] = lowerBound;
        }
        else if ( transIn[coord] > upperBound )
        {
            proj[coord] = upperBound;
        }
        else
        {
            proj[coord] = transIn[coord];
        }

        // now determine the alpha value, i.e, a vector containing
        // float values in the range [0,1], describing the projection's
        // relative position inside the box: '0' accords the
        // boxStart and '1' the boxEnd value for the respective coordinate

        a[coord] = (proj[coord] - bxStart[coord]) / (bxEnd[coord] - bxStart[coord]);
    }


	SO_ENGINE_OUTPUT(alpha, SoSFVec3f, setValue(a) );
	SO_ENGINE_OUTPUT(translationOut, SoSFVec3f, setValue(proj) );
	SO_ENGINE_OUTPUT(rotationOut, SoSFRotation, setValue(rotationIn.getValue()) );

}


