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
/** class definition SoBoxConstraint
  *
  * SoBoxConstraint maps an arbitray translation onto the closest point
  * on a box specified, but leaves the rotation unchanged. Thus the
  * user's movements are constrained to a box.
  *
  * @author  Zsolt Marx
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SO_BOX_CONSTRAINT
#define SO_BOX_CONSTRAINT

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <stbapi/misc/SoConstraint.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * SoBoxConstraint maps an arbitray translation onto the closest point
 * in a box specified, but leaves the rotation unchanged. Thus the
 * user's movements are constrained to a box.
 * @ingroup util
 */
class STBAPI_API SoBoxConstraint : public SoConstraint
{
	SO_ENGINE_HEADER(SoBoxConstraint);

public:

	// Inputs

	/**
	 * boxStart is the point where the constraining box starts.
	 */
	SoSFVec3f	boxStart;

	/**
	 * boxEnd is the point where the onstraining box ends.
	 */
	SoSFVec3f	boxEnd;


	// Outputs:

	/**
	 * If boxStart accords (0,0,0) and boxEnd accords (1,1,1), what is the 
	 * pen's current position in the box?
     */
	SoEngineOutput	alpha;	// (SoSFVec3f)

	/**
	 * Initialization
	 */
	static void initClass();

	/** 
	 * the constructor
	 */
	SoBoxConstraint();

	/**
	 * the destructor
	 */
	virtual ~SoBoxConstraint();

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
