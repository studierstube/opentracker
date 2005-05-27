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
  * $Id: SoPucActiveIfOperator.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucActiveIfOperator.h"
#include "SoPucActiveIfClause.h"

#include <Inventor/nodes/SoGroup.h>
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucActiveIfOperator);

//----------------------------------------------------------------------------

void SoPucActiveIfOperator::initClass()
{
	SO_KIT_INIT_CLASS(SoPucActiveIfOperator, SoPucActiveIfBase, "SoPucActiveIfBase");
}

//----------------------------------------------------------------------------

SoPucActiveIfOperator::SoPucActiveIfOperator()
{
	 SO_KIT_CONSTRUCTOR(SoPucActiveIfOperator);
    
    //---                     partName, partClassName, nullByDefault, parentName, rightName, isPublicPart
	SO_KIT_ADD_CATALOG_LIST_ENTRY(clauses,	SoGroup, TRUE, this, \x0, SoPucActiveIfClause, FALSE);

	SO_NODE_DEFINE_ENUM_VALUE(ActiveIfOperator, OR);
    SO_NODE_DEFINE_ENUM_VALUE(ActiveIfOperator, AND);

    SO_NODE_SET_SF_ENUM_TYPE(op, ActiveIfOperator);

	SO_NODE_ADD_FIELD(op, (OR));	
		
	SO_KIT_INIT_INSTANCE();

	clausesSensor.setData(this);
	clausesSensor.setFunction(clausesChangedCB);
	// before SoPucActiveIfNode, but after SoPucActiveIfClause
	clausesSensor.setPriority(150);

	setUpConnections(TRUE, TRUE);
}

//----------------------------------------------------------------------------

SoPucActiveIfOperator::~SoPucActiveIfOperator()
{

}

// --------------------------------------------------------------------------

SbBool SoPucActiveIfOperator::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoPucActiveIfBase::setUpConnections(onOff, doItAlways);	
		// setup the widget
		clausesSensor.attach(&clauses);
		evaluateClause(true);
	}
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		clausesSensor.detach();
		
		SoPucActiveIfBase::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);	
}

//----------------------------------------------------------------------------

void SoPucActiveIfOperator::clausesChangedCB(void * data, SoSensor * sensor )
{

	SoPucActiveIfOperator *self = (SoPucActiveIfOperator *)data;
	assert(self != NULL);

	self->evaluateClause();
}

//----------------------------------------------------------------------------

void SoPucActiveIfOperator::evaluateClause(bool alwaysSetValue)
{
	SoNodeKitListPart *clausesList = (SoNodeKitListPart *)clauses.getValue();
	if (clausesList == NULL) return;
	int numentries =  clausesList->getNumChildren();
	
	int activeValue = TRUE;
	if (op.getValue() == OR)
	{
		// initialize OR operation with FALSE, to take the value from clause -> FALSE|(FALSE) == FALSE , FALSE|(TRUE) == TRUE
		activeValue = FALSE;
		for (int i=0;i< numentries; i++)
		{
			SoPucActiveIfBase* clause = (SoPucActiveIfBase*)clausesList->getChild(i);
			activeValue = activeValue || (clause->active.getValue() == TRUE);
		}
	}
	else
	{
		// initialize AND operation with TRUE, to take the value from clause  -> TRUE&(FALSE) == FALSE, TRUE&(TRUE) == TRUE
		activeValue = TRUE;
		for (int i=0;i< numentries; i++)
		{
			SoPucActiveIfBase* clause = (SoPucActiveIfBase*)clausesList->getChild(i);
			activeValue = activeValue && (clause->active.getValue() == TRUE);
		}
	}

	if (active.getValue() != activeValue || alwaysSetValue ) 
		active.setValue(activeValue);
}
