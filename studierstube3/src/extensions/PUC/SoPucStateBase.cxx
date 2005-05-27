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
  * $Id: SoPucStateBase.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucStateBase.h"

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucStateBase);

//----------------------------------------------------------------------------

void SoPucStateBase::initClass()
{
	SO_KIT_INIT_CLASS(SoPucStateBase, SoPucElementBase, "SoPucElementBase");
}

//----------------------------------------------------------------------------

SoPucStateBase::SoPucStateBase()
{
	 SO_KIT_CONSTRUCTOR(SoPucStateBase);
  
	 SO_NODE_DEFINE_ENUM_VALUE(AccessRestriction, ANY);
     SO_NODE_DEFINE_ENUM_VALUE(AccessRestriction, READ_ONLY);
     SO_NODE_DEFINE_ENUM_VALUE(AccessRestriction, WRITE_ONLY);

     SO_NODE_SET_SF_ENUM_TYPE(access, AccessRestriction);

	 SO_NODE_ADD_FIELD(access, (ANY));
	 SO_NODE_ADD_FIELD(valueLabels, (""));

	 valueLabels.setNum(0);
	 SO_KIT_INIT_INSTANCE();	

	 valueSensor.setFunction(valueSetCB);
	 valueSensor.setPriority(1000);
}

//----------------------------------------------------------------------------

SoPucStateBase::~SoPucStateBase()
{
}


//----------------------------------------------------------------------------

std::string SoPucStateBase::getElementName() const
{
	return "state";
}

//----------------------------------------------------------------------------

std::string SoPucStateBase::getType() const
{
	return "illigal Type";
}

//----------------------------------------------------------------------------

std::string SoPucStateBase::getStateValue()
{
	return "</undefined>";
}

//----------------------------------------------------------------------------

bool SoPucStateBase::setStateValue(std::string )
{
	return false;
//	cout << "unable to comply no state";
}

//----------------------------------------------------------------------------

bool SoPucStateBase::isValueLabelsSpecified() const
{
	return (valueLabels.getNum() > 0);
}

//----------------------------------------------------------------------------
// static	
void SoPucStateBase::valueSetCB(void *data, SoSensor *sensor)
{
	((SoPucStateBase *)data)->valueSetCallback(sensor);
}

//----------------------------------------------------------------------------

void SoPucStateBase::valueSetCallback(SoSensor *sensor)
{
}

//----------------------------------------------------------------------------
bool SoPucStateBase::isEqual(const SbString &value) const
{
	// has to be implemented by sub classes
	return FALSE;
}

//----------------------------------------------------------------------------
bool SoPucStateBase::isGreaterThan(const SbString &value) const
{
	// has to be implemented by sub classes
	return FALSE;
}

//----------------------------------------------------------------------------
bool SoPucStateBase::isLessThan(const SbString &value) const
{
	// has to be implemented by sub classes
	return FALSE;
}

