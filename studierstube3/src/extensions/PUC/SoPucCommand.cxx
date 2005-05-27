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
  * $Id: SoPucCommand.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucCommand.h"

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucCommand);

//----------------------------------------------------------------------------

void SoPucCommand::initClass()
{
	SO_KIT_INIT_CLASS(SoPucCommand, SoPucElementBase, "SoPucElementBase");
}

//----------------------------------------------------------------------------

SoPucCommand::SoPucCommand()
{
	 SO_KIT_CONSTRUCTOR(SoPucCommand);
    
	 SO_NODE_ADD_FIELD(trigger, ());
	 SO_NODE_ADD_FIELD(triggerIn, ());

	 SO_KIT_INIT_INSTANCE();	

	 triggerSensor.setData(this);
	 triggerSensor.setFunction(triggerInCB);
	 triggerSensor.attach(&triggerIn);

	 fireTrigger = false;

}

//----------------------------------------------------------------------------

SoPucCommand::~SoPucCommand()
{
}

//----------------------------------------------------------------------------

void SoPucCommand::fireAtWill() 
{
	if (fireTrigger) trigger.touch();
	fireTrigger = false;
}

//----------------------------------------------------------------------------

void SoPucCommand::triggerInCB(void *data, SoSensor *sensor)
{
	// just "copy" the trigger event 
	((SoPucCommand *)data)->fireTrigger = true;
}

//----------------------------------------------------------------------------

std::string SoPucCommand::getElementName() const
{
	return "command";
}

//----------------------------------------------------------------------------

std::string SoPucCommand::getCommandName() const
{
	std::string ret;
	SbName name = getName();
	
	if (name.getLength() == 0)
		ret = "unnamed"; // TODO auto nameing ...
	else
		ret = getName().getString();
	
	return ret;
}

