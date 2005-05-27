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
/** Definition of class SoRotateFromTo
 *
 *
 * @author Rainer Splechtna
 *
 * $Id: SoRotateFromTo.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOROTATEFROMTO_H_
#define _SOROTATEFROMTO_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** This engine calculates the rotation rotateFromTo, 
  * which rotates vectorFrom into vectorTo.
  * @ingroup util
  */
class STBAPI_API SoRotateFromTo : public SoEngine {

	SO_ENGINE_HEADER(SoRotateFromTo);

public:
	/// input field: vectorFrom and vectorTo
	SoSFVec3f   vectorFrom;
	SoSFVec3f	vectorTo;

	/// output is a rotation
	SoEngineOutput rotateFromTo;	// (SoSFRotation) rotateFromTo

	static void initClass();
	
	/// Constructor
	SoRotateFromTo();

private:
	/// Destructor
	virtual ~SoRotateFromTo();
	
	/// evaluation method
	virtual void evaluate();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif


#endif

