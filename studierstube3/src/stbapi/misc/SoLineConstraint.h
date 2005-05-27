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
/** class definition SoLineConstraint
  *
  * SoLineConstraint maps an arbitray translation onto the closest point
  * on a line specified, but leaves the rotation unchanged. Thus the
  * user's movements are constrained to a line.
  *
  * @author  Zsolt Marx
  *
  * $Id: SoLineConstraint.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SO_LINE_CONSTRAINT
#define SO_LINE_CONSTRAINT

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <stbapi/misc/SoConstraint.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * SoLineConstraint maps an arbitray translation onto the closest point
 * on a line specified, but leaves the rotation unchanged. Thus the
 * user's movements are constrained to a line.
 * @ingroup util
 */
class STBAPI_API SoLineConstraint : public SoConstraint
{
	SO_ENGINE_HEADER(SoLineConstraint);

public:

	// Inputs

	/**
	 * lineStart is the point where the constraining line starts.
	 */
	SoSFVec3f	lineStart;

	/**
	 * lineEnd is the point where the onstraining line ends.
	 */
	SoSFVec3f	lineEnd;

	/**
     * Additionally the user's motion on the line can be quantized,
	 * i.e the pen's translation is restricted to distinct points
	 * on the line. rasterLength gives the distance between those
	 * points. Set rasterLength to zero to allow any point on the line.
	 */
	SoSFFloat	rasterLength;

	// Outputs:

	/**
	 * If lineStart accords 0 and lineEnd accords 1, what is the 
	 * pen's current position on the line? The alpha value
	 * is needed to use the SoLineConstraint with the SoDraggerKit
	 * to create a SoSlider and to conveniently determine it's
	 * current value. Alpha is of type SoSFFloat.
	 */
	SoEngineOutput	alpha;	// (SoSFFloat)

	/**
	 * Initialization
	 */
	static void initClass();

	/** 
	 * the constructor
	 */
	SoLineConstraint();

	/**
	 * the destructor
	 */
	virtual ~SoLineConstraint();

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
