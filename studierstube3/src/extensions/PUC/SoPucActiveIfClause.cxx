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
  * $Id: SoPucActiveIfClause.cxx 3823 2004-10-05 12:27:08Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucStateBase.h"
#include "SoPucActiveIfClause.h"

#if defined(_DEBUG_PUC) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucActiveIfClause);

//----------------------------------------------------------------------------

void SoPucActiveIfClause::initClass()
{
	SO_KIT_INIT_CLASS(SoPucActiveIfClause, SoPucActiveIfBase, "SoPucActiveIfBase");
}

//----------------------------------------------------------------------------

SoPucActiveIfClause::SoPucActiveIfClause()
{
	 SO_KIT_CONSTRUCTOR(SoPucActiveIfClause);
    
    //---                     partName, partClassName, nullByDefault, parentName, rightName, isPublicPart
	 
	 SO_NODE_DEFINE_ENUM_VALUE(ActiveIfClauseOperator, EQUALS);
     SO_NODE_DEFINE_ENUM_VALUE(ActiveIfClauseOperator, GREATER_THAN);
     SO_NODE_DEFINE_ENUM_VALUE(ActiveIfClauseOperator, LESS_THAN);
	 
     SO_NODE_SET_SF_ENUM_TYPE(op, ActiveIfClauseOperator);
	 
	 SO_NODE_ADD_FIELD(state, (NULL));
	 SO_NODE_ADD_FIELD(op, (EQUALS));	
	 SO_NODE_ADD_FIELD(value, (""));	
	 
	 SO_KIT_INIT_INSTANCE();

	 stateSensor.setFunction(evaluateActiveCB);
	 stateSensor.setData(this);

	 valueSensor.setFunction(evaluateActiveCB);
	 valueSensor.setData(this);

	 opSensor.setFunction(evaluateActiveCB);
	 opSensor.setData(this);

	 setUpConnections(TRUE, TRUE);
}

//----------------------------------------------------------------------------

SoPucActiveIfClause::~SoPucActiveIfClause()
{

}

// --------------------------------------------------------------------------

SbBool SoPucActiveIfClause::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoPucActiveIfBase::setUpConnections(onOff, doItAlways);	
		// setup the widget
		stateSensor.attach(&state);
		valueSensor.attach(&value);
		opSensor.attach(&op);
		evaluateActive(true);
	}
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		stateSensor.detach();
		valueSensor.detach();
		opSensor.detach();
		
		SoPucActiveIfBase::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);	
}

// --------------------------------------------------------------------------

void SoPucActiveIfClause::evaluateActiveCB( void * data, SoSensor * sensor  )
{
	SoPucActiveIfClause *self = (SoPucActiveIfClause *)data;
	assert(self != NULL);

	self->evaluateActive();
}

// --------------------------------------------------------------------------

void SoPucActiveIfClause::evaluateActive(bool alwaysSetValue)
{
	SoField *connectedField = NULL;
	bool evaluated = false;
	int activeValue = TRUE;

	if (state.getConnectedField(connectedField )) 
	{
		SoFieldContainer *container = connectedField->getContainer();
		if (container != NULL)
		{
			if (container->getTypeId().isDerivedFrom(SoPucStateBase::getClassTypeId()))
			{
				SoPucStateBase *state = (SoPucStateBase *)container;
				switch (op.getValue())
				{
				case EQUALS:
					activeValue	= state->isEqual(value.getValue());
					break;
				case GREATER_THAN:
					activeValue	= state->isGreaterThan(value.getValue());
					break;
				case LESS_THAN:
					activeValue	= state->isLessThan(value.getValue());
					break;
				}
				evaluated = true;
			}
		}
	}
	
	if (!evaluated)
	{
		
		float stateFloat;
		float valueFloat;

		int retState = sscanf(state.getValue().getString(),"%f",&stateFloat);
		int retValue = sscanf(value.getValue().getString(),"%f",&valueFloat);
		if (retState == 1 && retValue == 1)
		{
			switch (op.getValue())
			{
			case EQUALS:
				activeValue	= (stateFloat == valueFloat);
				break;
			case GREATER_THAN:
				activeValue	= (stateFloat > valueFloat);
				break;
			case LESS_THAN:
				activeValue	= (stateFloat < valueFloat);
				break;
			}
		} 
		else
		{
			DEBUG_CODE(printf("WARNING SoPucActiveIfClause::evaluateActive(): state and/or value can not be evaluated. performing strcmp \n"););
			int strcmpResult = strcmp(state.getValue().getString(),value.getValue().getString());

			switch (op.getValue())
			{
			case EQUALS:
				activeValue	= (strcmpResult == 0);
				break;
			case GREATER_THAN:
				activeValue	= (strcmpResult > 0);
				break;
			case LESS_THAN:
				activeValue	= (strcmpResult < 0);
				break;
			}
		}
	}

	if (activeValue != active.getValue() || alwaysSetValue)
		active.setValue(activeValue);

}
