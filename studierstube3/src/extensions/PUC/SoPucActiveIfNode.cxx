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
* $Id: SoPucActiveIfNode.cxx 3565 2004-08-02 14:47:20Z tomp $
* @file                                                                   */
/* ======================================================================= */

#include "SoPucActiveIfNode.h"
#include "SoPucActiveIfBase.h"

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucActiveIfNode);

//----------------------------------------------------------------------------

void SoPucActiveIfNode::initClass()
{
	SO_KIT_INIT_CLASS(SoPucActiveIfNode, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------

SoPucActiveIfNode::SoPucActiveIfNode()
{
	SO_KIT_CONSTRUCTOR(SoPucActiveIfNode);
    
	//-----------------------name-----|-------type-------|nuldef| par |rite|public
    SO_KIT_ADD_CATALOG_ENTRY(activeIf, SoPucActiveIfBase,  TRUE, this, \x0, TRUE);

	SO_NODE_DEFINE_ENUM_VALUE(ActiveIfIgnore, NONE);
	SO_NODE_DEFINE_ENUM_VALUE(ActiveIfIgnore, ALL);
	SO_NODE_DEFINE_ENUM_VALUE(ActiveIfIgnore, PARENT);
	
	SO_NODE_SET_SF_ENUM_TYPE(ignore, ActiveIfIgnore);
	
	SO_NODE_ADD_FIELD(ignore, (NONE));	
	SO_NODE_ADD_FIELD(active, (TRUE));	
	SO_NODE_ADD_FIELD(explanation, (""));
	SO_KIT_INIT_INSTANCE();

	activeIfSensor.setData(this);
	activeIfSensor.setFunction(activeIfChangedCB);
	// after SoPucActiveIfOperator, and after SoPucActiveIfClause
	activeIfSensor.setPriority(200);

	setUpConnections(TRUE, TRUE);

}

//----------------------------------------------------------------------------

SoPucActiveIfNode::~SoPucActiveIfNode()
{
	
}

//----------------------------------------------------------------------------

bool SoPucActiveIfNode::isIgnoredSet() const
{
	if (ignore.getValue() != NONE) return true;
	return false;
}

// --------------------------------------------------------------------------

SbBool SoPucActiveIfNode::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);	
		// setup the widget
		activeIfSensor.attach(&activeIf);
		activeIf.touch();
	}
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		activeIfSensor.detach();
		
		SoBaseKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);	
}

// --------------------------------------------------------------------------

void SoPucActiveIfNode::activeIfChangedCB(void * data, SoSensor * sensor )
{
	SoPucActiveIfNode *self = (SoPucActiveIfNode *)data;
	assert(self != NULL);
	if (!self->activeIf.getValue()->getTypeId().isDerivedFrom(SoPucActiveIfBase::getClassTypeId()))
	{
		printf("ERROR SoPucActiveIfNode::activeIfChangedCB: node in activeIf is not dervide from SoPucActiveIfBase\n");
		exit(2);
		return;
	}
	
	SoSField *activePtr = &((SoPucActiveIfBase *)(self->activeIf.getValue()))->active;
	
	SoField *connectedField = NULL;
	self->active.getConnectedField(connectedField);

	if ( connectedField!= activePtr)
		self->active.connectFrom(activePtr);
}