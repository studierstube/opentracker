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
/** Source file defines the entry point for the application dll.
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucStateFixed.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucStateFixed.h"
#include <iostream>

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucStateFixed);

//----------------------------------------------------------------------------

void SoPucStateFixed::initClass()
{
	SO_KIT_INIT_CLASS(SoPucStateFixed, SoPucStateBase, "SoPucStateBase");
}

//----------------------------------------------------------------------------

SoPucStateFixed::SoPucStateFixed()
{
	SO_KIT_CONSTRUCTOR(SoPucStateFixed);
   
	SO_NODE_ADD_FIELD(min,(0));
    SO_NODE_ADD_FIELD(max,(0));
    SO_NODE_ADD_FIELD(incr,(1.0));
    SO_NODE_ADD_FIELD(pointpos,(1));
    SO_NODE_ADD_FIELD(value,(0));

	SO_KIT_INIT_INSTANCE();	
	
	valueSensor.setData(this);
	valueSensor.attach(&value);
}

//----------------------------------------------------------------------------

SoPucStateFixed::~SoPucStateFixed()
{

}

//----------------------------------------------------------------------------

std::string SoPucStateFixed::getType() const
{		
	return "fixedpt";
}

//----------------------------------------------------------------------------

std::string SoPucStateFixed::getStateValue()
{
	char buffer[40];
	float val = value.getValue();
	int numPrec = pointpos.getValue();
	sprintf(buffer,"%.*f",numPrec,val);
	return buffer;
}

//----------------------------------------------------------------------------

bool SoPucStateFixed::setStateValue(std::string value_)
{
	return (value.set(value_.c_str()) == TRUE);
}

//----------------------------------------------------------------------------

void SoPucStateFixed::valueSetCallback(SoSensor *sensor)
{
	if (min.getValue() != max.getValue() )
	{
		if (value.getValue() < min.getValue()) 
		{
			std::cout << "[warning] SoPucStateFixed::valueSetCallback with value:" << value.getValue() << " lower than min " << min.getValue()<<" value set to min" << std::endl;
			value = min;
		
		}
		if (value.getValue() > max.getValue()) 
		{
			std::cout << "[warning] SoPucStateFixed::valueSetCallback with value:" << value.getValue() << " higher than max " << max.getValue()<<" value set to max" << std::endl;
			value = max;
		}
	}
}
//----------------------------------------------------------------------------
// HELPER
static float convertStringToFloat(const SbString &valueIn)
{
	const char*str = valueIn.getString();
	return atof(str);
}

//----------------------------------------------------------------------------
bool SoPucStateFixed::isEqual(const SbString &valueIn) const
{
	float valueInFloat = convertStringToFloat(valueIn);
	return (value.getValue() == valueInFloat);
}

//----------------------------------------------------------------------------
bool SoPucStateFixed::isGreaterThan(const SbString &valueIn) const
{
	float valueInFloat  = convertStringToFloat(valueIn);
	return (value.getValue() > valueInFloat);
}

//----------------------------------------------------------------------------
bool SoPucStateFixed::isLessThan(const SbString &valueIn) const
{
	float valueInFloat  = convertStringToFloat(valueIn);
	return (value.getValue() < valueInFloat);
}
