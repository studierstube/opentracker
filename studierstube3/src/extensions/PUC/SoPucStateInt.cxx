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
  * $Id: SoPucStateInt.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucStateInt.h"
#include <iostream>

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucStateInt);

//----------------------------------------------------------------------------

void SoPucStateInt::initClass()
{
	SO_KIT_INIT_CLASS(SoPucStateInt, SoPucStateBase, "SoPucStateBase");
}

//----------------------------------------------------------------------------

SoPucStateInt::SoPucStateInt()
{
	SO_KIT_CONSTRUCTOR(SoPucStateInt);
   
	SO_NODE_ADD_FIELD(min,(0));
    SO_NODE_ADD_FIELD(max,(0));
    SO_NODE_ADD_FIELD(incr,(1));
    SO_NODE_ADD_FIELD(value,(0));

	SO_KIT_INIT_INSTANCE();	

	valueSensor.setData(this);
	valueSensor.attach(&value);
}

//----------------------------------------------------------------------------

SoPucStateInt::~SoPucStateInt()
{

}

//----------------------------------------------------------------------------

std::string SoPucStateInt::getType() const
{		
	return "integer";
}

//----------------------------------------------------------------------------

std::string SoPucStateInt::getStateValue()
{
	SbString valueStr;
	value.get(valueStr);
	return valueStr.getString();
}

//----------------------------------------------------------------------------

bool SoPucStateInt::setStateValue(std::string value_)
{
	return (value.set(value_.c_str()) == TRUE);
}
//----------------------------------------------------------------------------

void SoPucStateInt::valueSetCallback(SoSensor *sensor)
{
	if (min.getValue() != max.getValue() )
	{
		if (value.getValue() < min.getValue()) 
		{
			std::cout << "[warning] SoPucStateInt::valueSetCallback with value:" << value.getValue() << " lower than min " << min.getValue()<<" value set to min" << std::endl;
			value = min;
		
		}
		if (value.getValue() > max.getValue()) 
		{
			std::cout << "[warning] SoPucStateInt::valueSetCallback with value:" << value.getValue() << " higher than max " << max.getValue()<<" value set to max" << std::endl;
			value = max;
		}
	}
}

//----------------------------------------------------------------------------
// HELPER
int convertStringToInt(const SbString &valueIn)
{
	const char*str = valueIn.getString();
	return atoi(str);
}

//----------------------------------------------------------------------------
bool SoPucStateInt::isEqual(const SbString &valueIn) const
{
	int valueInInt = convertStringToInt(valueIn);
	return (value.getValue() == valueInInt);
}

//----------------------------------------------------------------------------
bool SoPucStateInt::isGreaterThan(const SbString &valueIn) const
{
	int valueInInt  = convertStringToInt(valueIn);
	return (value.getValue() > valueInInt);
}

//----------------------------------------------------------------------------
bool SoPucStateInt::isLessThan(const SbString &valueIn) const
{
	int valueInInt  = convertStringToInt(valueIn);
	return (value.getValue() < valueInInt);
}
