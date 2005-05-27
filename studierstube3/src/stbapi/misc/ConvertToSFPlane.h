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

 /* ======================================================================= */
/** 
  * Defines an OIV Field converter, converts SoMFVec4f to SoSFPlane.
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: ConvertToSFPlane.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOCONVERTTOSFPLANE_H_
#define _SOCONVERTTOSFPLANE_H_

#include <Inventor/engines/SoFieldConverter.h>
#include <Inventor/fields/SoMFVec4f.h>
#include <Inventor/fields/SoSFPlane.h>

#include <stbapi/stbapi.h>

/**
OIV Field converter, converts SoMFVec4f to SoSFPlane. 
Used to set the plane equation of a SoClipPlane by connecting Engines to
calculate the plane equation.

@ingroup util
*/
class STBAPI_API ConvertToSFPlane : public SoFieldConverter {
public:
	SO_ENGINE_HEADER(ConvertToSFPlane);
	
public:
	/**
     * Static initializer. Registers the converter with the OIV runtime.
     * Call this in your program before you load an .iv file using this converter.
     */
	static void initClass();

	/**
     * The Input field, a SoMFVec4f. This is converted to the 4 fields of
     * a plane equation in Output. Only the first SbVec4f value of the MFVec4f is taken.
     */
	SoMFVec4f input;

	/**
     * The output Field, a SoSFPlane.
     */
	SoEngineOutput output; // (SoSFPlane)

	/**
     * Default constructor.
     */
	ConvertToSFPlane();

private:

	/**
     * Default destructor.
     */
	virtual ~ConvertToSFPlane();

	/**
     * Evaluation method.
     */
	virtual void evaluate();

	// These must be defined for a field converter. They return
	// the input and output connections of the given types. In
	// our case, we have only one input and one output, so we
	// know that those will be the given types.
	virtual SoField * getInput(SoType type);
	virtual SoEngineOutput * getOutput(SoType type);
};

#endif//_SOCONVERTTOSFPLANE_H_
