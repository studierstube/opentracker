/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
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
  * Implements an OIV Field converter, converts SoMFVec4f to SoSFPlane.
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: ConvertToSFPlane.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SoDB.h>
#include "ConvertToSFPlane.h"

SO_ENGINE_SOURCE(ConvertToSFPlane);

// Initializes the ConvertSFShortToSFFloat class.

void ConvertToSFPlane::initClass()
{
	SO_ENGINE_INIT_CLASS(ConvertToSFPlane, SoFieldConverter, "FieldConverter");

	// Register this converter’s type with the Inventor database
	// to convert from a field (or engine output) of type
	// SoMFVec4f to a field of type SoSFPlane.
	// We only call this once, since this engine offers only one
	// type conversion.
	SoDB::addConverter(SoMFVec4f::getClassTypeId(), SoSFPlane::getClassTypeId(), getClassTypeId());
}

// Constructor

ConvertToSFPlane::ConvertToSFPlane()
{
	// Do standard constructor tasks
	SO_ENGINE_CONSTRUCTOR(ConvertToSFPlane);

	// Define input field and its default value
	SO_ENGINE_ADD_INPUT(input, (0.0, 0.0, 0.0, 0.0));

	// Define the output, specifying its type
	SO_ENGINE_ADD_OUTPUT(output, SoSFPlane);
}

// Destructor. Does nothing.

ConvertToSFPlane::~ConvertToSFPlane()
{
}

// This is the evaluation routine.

void ConvertToSFPlane::evaluate()
{
	// Get the input value as a Vec4f, convert it to a Plane, and
	// output it
	const float* vec4f = input.getValues(0)->getValue();

	SO_ENGINE_OUTPUT(output, SoSFPlane, setValue(SbPlane(vec4f, vec4f[3])));
}

// This returns the input field for the given type. Since we
// have only one input field, we don’t have to check the type.

SoField* ConvertToSFPlane::getInput(SoType)
{
	return &input;
}

// This does the same for the output.

SoEngineOutput* ConvertToSFPlane::getOutput(SoType)
{
	return &output;
}
