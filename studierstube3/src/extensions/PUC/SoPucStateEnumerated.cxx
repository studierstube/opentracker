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
  * $Id: SoPucStateEnumerated.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucStateEnumerated.h"

#include <iostream>

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucStateEnumerated);

//----------------------------------------------------------------------------

void SoPucStateEnumerated::initClass()
{
	SO_KIT_INIT_CLASS(SoPucStateEnumerated, SoPucStateBase, "SoPucStateBase");
}

//----------------------------------------------------------------------------

SoPucStateEnumerated::SoPucStateEnumerated()
{
	SO_KIT_CONSTRUCTOR(SoPucStateEnumerated);
   
    SO_NODE_ADD_FIELD(value,(0));

	SO_KIT_INIT_INSTANCE();	
	
	valueSensor.setData(this);
	valueSensor.attach(&value);
}

//----------------------------------------------------------------------------

SoPucStateEnumerated::~SoPucStateEnumerated()
{

}

//----------------------------------------------------------------------------

std::string SoPucStateEnumerated::getType() const
{		
	// since PUC/2.0 its "enumeration" instead of "enumerated"
	// -- change scince PUC/2.0 its "enumeration" instead of "enumerated
	return "enumerated";
}

//----------------------------------------------------------------------------

std::string SoPucStateEnumerated::getStateValue()
{
	char buffer[40];
	int v = value.getValue()+1;
	itoa(v,buffer,10);
	return buffer;
}

//----------------------------------------------------------------------------

bool SoPucStateEnumerated::setStateValue(std::string value_)
{
	int i = atoi(value_.c_str());
	value.setValue(i-1);

	return TRUE;
}

//----------------------------------------------------------------------------

void SoPucStateEnumerated::valueSetCallback(SoSensor *sensor)
{
	int i = value.getValue();
	int max = valueLabels.getNum();
	if (i >= max)
	{
		std::cout << "[warning] SoPucStateEnumerated::valueSetCallback with value:" << value.getValue() << " higher than number of valueStates " << max<<" value set to " << max<< std::endl;
		value.setValue(valueLabels.getNum()-1);
	}
	else if (i < 0)
	{
		std::cout << "[warning] SoPucStateEnumerated::valueSetCallback with value:" << value.getValue() << " lower than 1, value set to 1" << std::endl;
		value.setValue(1);
	}
	
}

//----------------------------------------------------------------------------
// HELPER
int convertStringToEnum(const SbString &valueIn)
{
	// TODO find string ??
	const char*str = valueIn.getString();
	return atoi(str);
}

//----------------------------------------------------------------------------
bool SoPucStateEnumerated::isEqual(const SbString &valueIn) const
{
	int valueInFloat = convertStringToEnum(valueIn);
	return (value.getValue() == valueInFloat);
}

//----------------------------------------------------------------------------
bool SoPucStateEnumerated::isGreaterThan(const SbString &valueIn) const
{
	int valueInFloat  = convertStringToEnum(valueIn);
	return (value.getValue() > valueInFloat);
}

//----------------------------------------------------------------------------
bool SoPucStateEnumerated::isLessThan(const SbString &valueIn) const
{
	int valueInFloat  = convertStringToEnum(valueIn);
	return (value.getValue() < valueInFloat);
}
