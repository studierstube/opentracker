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
  * $Id: SoPucStateBool.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucStateBool.h"

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucStateBool);

//----------------------------------------------------------------------------

void SoPucStateBool::initClass()
{
	SO_KIT_INIT_CLASS(SoPucStateBool, SoPucStateBase, "SoPucStateBase");
}

//----------------------------------------------------------------------------

SoPucStateBool::SoPucStateBool()
{
	SO_KIT_CONSTRUCTOR(SoPucStateBool);
   
    SO_NODE_ADD_FIELD(value,(false));

	SO_KIT_INIT_INSTANCE();	

	valueSensor.setData(this);
	valueSensor.attach(&value);

}

//----------------------------------------------------------------------------

SoPucStateBool::~SoPucStateBool()
{

}

//----------------------------------------------------------------------------

std::string SoPucStateBool::getType() const
{		
	return "boolean";
}

//----------------------------------------------------------------------------

std::string SoPucStateBool::getStateValue()
{
	SbString valueStr;
	value.get(valueStr);
	return valueStr.getString();
}

//----------------------------------------------------------------------------

bool SoPucStateBool::setStateValue(std::string value_)
{
	char *upper = strdup(value_.c_str());
	_strupr(upper);
	SbString t;
	value.get(t);
	if (t == upper) {
		return TRUE;
	}
	
	return (value.set(upper) == TRUE);
}
//----------------------------------------------------------------------------

void SoPucStateBool::valueSetCallback(SoSensor *sensor)
{
	// empty on purpose 	
}

//----------------------------------------------------------------------------
// HELPER
int convertStringToBool(const SbString &valueIn)
{
	if (valueIn == "TRUE" || valueIn == "1") 
		return TRUE;
	return FALSE;
}

//----------------------------------------------------------------------------
bool SoPucStateBool::isEqual(const SbString &valueIn) const
{
	int valueInBool = convertStringToBool(valueIn);
	return (value.getValue() == valueInBool);
}

//----------------------------------------------------------------------------
bool SoPucStateBool::isGreaterThan(const SbString &valueIn) const
{
	int valueInBool = convertStringToBool(valueIn);
	return (value.getValue() > valueInBool);
}

//----------------------------------------------------------------------------
bool SoPucStateBool::isLessThan(const SbString &valueIn) const
{
	int valueInBool = convertStringToBool(valueIn);
	return (value.getValue() < valueInBool);
}
