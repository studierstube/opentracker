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
/** Class definition of SoRotateVector, an engine that rotates the input vector
  * with the given rotation. If invertRotation is TRUE, the vector will be rotated
  * with the inverse of the rotation. The result is stored in the output engine output.
  *
  * @author Istvan Barakonyi
  *
  * $Id: SoRotateVector.h 3425 2004-07-14 12:09:17Z tamer $
  * @file
  * ======================================================================= */

#ifndef _SOROTATEVECTOR_
#define _SOROTATEVECTOR_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/fields/SoMFBool.h>

#include <stbapi/misc/SoConstraint.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** This engine rotates its input vectors with the given input rotations.
    The engine operates on multiple-value fields. It outputs as many entries 
    as the maximum length input field contains. For the other input fields the 
    last value is repeated.
 
    File format syntax/defaults:
 
    @verbatim
    SoRotateVector {
       # inputs
       SoMFVec3f input 0 0 0
       SoMFRotation rotation 0 0 1 0 

       # output
       SoMFVec3f output
    }
    @endverbatim
    @author Istvan Barakonyi
    @ingroup util
  */
class STBAPI_API SoRotateVector : public SoEngine
{
    SO_ENGINE_HEADER(SoRotateVector);

public:

    /// This input field contains the vectors to rotate
    SoMFVec3f input;

    /// This input field contains the rotation to rotate the input vectors with
    SoMFRotation rotation;

    /// This input field contains the flags for each input vector whether to use inverse rotation or not when rotating
    SoMFBool invertRotation;

    /// The output field contains the rotated vectors
    SoEngineOutput output;

    static void initClass();

    SoRotateVector();
    virtual ~SoRotateVector();

private:

    virtual void evaluate();
};

#endif

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif
