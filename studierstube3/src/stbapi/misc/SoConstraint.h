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
/** class definition of SoConstraint
  *
  * The base class for all constraints, which basically map a given translation
  * and rotation to an arbitrary translation and rotation. Thus they are
  * implemented as Open Inventor engines.
  *
  * @author  Zsolt Marx
  *
  * $Id: SoConstraint.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_CONSTRAINT_
#define _SO_CONSTRAINT_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <stbapi/stbapi.h>

// has to be put immediately before the class definition
#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


/**
 * The base class for all constraints. Basically maps a given translation
 * and rotation to an arbitrary translation and rotation.
 * @ingroup util
 */
class STBAPI_API SoConstraint : public SoEngine
{
	SO_ENGINE_HEADER(SoConstraint);

public:

	/**
	 * The incoming translation, that is the translation to be mapped,
	 * is connected to this field.
	 */
	SoSFVec3f		translationIn;

	/**
	 * The incoming rotation, that is the  to be mapped,
	 * is connected to this field.
	 */
	SoSFRotation	rotationIn;

	/**
	 * This is the resulting translation field, of type SoSFVec3f.
	 */
	SoEngineOutput	translationOut;	// (SoSFVec3f)

	/**
	 * This is the resulting rotation field, of type SoSFRotation.
	 */
	SoEngineOutput	rotationOut;	// (SoSFRotation)

	/**
	 * This is the static function, that is called to do class initialization.
	 */
	static void initClass();

	/**
	 * the constructor
	 */
	SoConstraint();

	/**
	 * the destructor
	 */
	virtual ~SoConstraint();

private:


	/**
	 * the engine's evaluate method, which does
	 * the constraint mapping.
	 */
	virtual void evaluate();

};

// .. and this one after the class definition
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
