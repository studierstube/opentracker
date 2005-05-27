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
/** class definition of SoAxisRotateConstraint
  *
  *
  * @author  Zsolt Marx
  *
  * $Id: SoAxisRotateConstraint.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SO_AXIS_ROTATE_CONSTRAINT
#define SO_AXIS_ROTATE_CONSTRAINT

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <stbapi/misc/SoConstraint.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * SoAxisRotateConstraint maps the pen's translation around the origin
 * onto a rotation around a specified axis through the origin. In order
 * to make the zero vector exactly follow the pen, the dragger kit's
 * drag point should be set to ORIGIN.
 * @ingroup util
 */
class STBAPI_API SoAxisRotateConstraint : public SoConstraint
{
	SO_ENGINE_HEADER(SoAxisRotateConstraint);

public:

	// Inputs

	SoSFVec3f	axis;
	SoSFVec3f	zeroVector;
	SoSFFloat	maxAngle;

	// Outputs:

	/**
	 * If lineStart accords 0 and lineEnd accords 1, what is the 
	 * pen's current position on the line? The alpha value
	 * is needed to use the SoAxisRotateConstraint with the SoDraggerKit
	 * to create a SoSlider and to conveniently determine it's
	 * current value. Alpha is of type SoSFFloat.
	 */
	//SoEngineOutput	alpha;	// (SoSFFloat)

	/**
	 * Initialization
	 */
	static void initClass();

	/** 
	 * the constructor
	 */
	SoAxisRotateConstraint();

	/**
	 * the destructor
	 */
	virtual ~SoAxisRotateConstraint();

private:


	/**
	 * the engine's evaluation method.
	 */
	virtual void evaluate();
};

#endif


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif
