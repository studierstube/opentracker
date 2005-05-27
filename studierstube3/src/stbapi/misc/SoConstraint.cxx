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
/** source file for SoConstraint class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoConstraint.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <stbapi/misc/SoConstraint.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

SO_ENGINE_SOURCE(SoConstraint);

/**
 * the constructor
 */
SoConstraint::SoConstraint()
{
	SO_ENGINE_CONSTRUCTOR(SoConstraint);

	SO_ENGINE_ADD_INPUT(translationIn, (0.0, 0.0, 0.0));
	SO_ENGINE_ADD_INPUT(rotationIn, (0.0, 0.0, 0.0, 1.0));

	SO_ENGINE_ADD_OUTPUT(translationOut, SoSFVec3f);
	SO_ENGINE_ADD_OUTPUT(rotationOut, SoSFRotation);
}

/**
 * the destructor
 */
SoConstraint::~SoConstraint()
{
}

void SoConstraint::initClass()
{
	SO_ENGINE_INIT_CLASS(SoConstraint, SoEngine, "Engine");
}

/**
 * evaluate, output = input
 */
void SoConstraint::evaluate()
{
	SO_ENGINE_OUTPUT(translationOut,SoSFVec3f, setValue(translationIn.getValue()) );
	SO_ENGINE_OUTPUT(rotationOut,SoSFRotation, setValue(rotationIn.getValue()) );
}

