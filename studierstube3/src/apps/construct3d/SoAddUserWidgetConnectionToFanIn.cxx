/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoAddUserWidgetConnectionToFanIn.cxx  - implementation of class SoAddUserWidgetConnectionToFanInr (version 2).
  * SoAddUserWidgetConnectionToFanIn is the implementation of a Node that creates a 
  * pipDependend field connection.
  *
  * @author  Thomas Psik
  *
  * $Id: SoAddUserWidgetConnectionToFanIn.cxx 3964 2004-10-19 13:43:28Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoAddUserWidgetConnectionToFanIn.h"
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoGate.h>

#include <stbapi/widgets/SoPushButton.h>

#include <stbapi/widgets/SoToggleButton.h>
#include <stbapi/widgets/SoLabelListBox.h>
#include <stbapi/misc/SoFanIn.h>
#include <stbapi/misc/SoEngineWrapper.h>



#include <string>
#include <cstdarg>

using namespace std;

SO_NODE_SOURCE(SoAddUserWidgetConnectionToFanIn);

void
SoAddUserWidgetConnectionToFanIn::initClass()
{
    SO_NODE_INIT_CLASS(SoAddUserWidgetConnectionToFanIn, SoNode, "Node");
}

SoAddUserWidgetConnectionToFanIn::SoAddUserWidgetConnectionToFanIn()
{
    SO_NODE_CONSTRUCTOR(SoAddUserWidgetConnectionToFanIn);

    SO_NODE_ADD_FIELD(widget, (""));
    SO_NODE_ADD_FIELD(fanIn, (""));
    SO_NODE_ADD_FIELD(value, (""));

  triggerSensor.setData(this);
  triggerSensor.setFunction(triggerCB);
  triggerSensor.setPriority(0);
  nextIn  = NULL;
//	setup();
}


void SoAddUserWidgetConnectionToFanIn::triggerCB(void *data, SoSensor *sensor) 
{
	SoAddUserWidgetConnectionToFanIn*self = (SoAddUserWidgetConnectionToFanIn*)data;
	assert(self!= NULL);
	// touch value to send data to fanIn
	self->value.touch();
//	printf("SoAddUserWidgetConnectionToFanIn::triggerCB\n");
 }

void SoAddUserWidgetConnectionToFanIn::setupPushButton(SoPushButton *pushButPtr, SoFanIn *fanInPtr, int uid) 
{
	nextIn = fanInPtr->getNextUnconnectedField();
	if (nextIn == NULL) 
	{
		printf("SoAddUserWidgetConnectionToFanIn::setup  fanIn all in fields are used.\nNo connection established !!\n");
		return;
	}

 	trigger.connectFrom(&(pushButPtr->triggerOut));
    triggerSensor.attach(&trigger);

	char buf[200];
	sprintf(buf, "%d",uid);
	value.setValue(buf);

	nextIn->connectFrom(&value);
//	SbName  fName; if (fanInPtr->getFieldName(nextIn,  fName ) == TRUE) { printf("   found field\n");}
//  printf("SoAddUserWidgetConnectionToFanIn::setupPushButton route established to field %s in engine %p\n", fName.getString(), fanInPtr);
}
		
// --------------------------------------------------------------------------
void SoAddUserWidgetConnectionToFanIn::setup(int uid, SoNode *base) 
{
	SoNode *nodePtr = base->getByName(widget.getValue());
	
	
	SoPushButton *pushButPtr = NULL;
	SoToggleButton *toggleButPtr = NULL;
	SoLabelListBox *labelListBoxPtr = NULL;
	if (nodePtr != NULL) 
	{
		if (nodePtr->getTypeId().isDerivedFrom(SoPushButton::getClassTypeId())) 
		{
			pushButPtr = (SoPushButton*)nodePtr;
		} 
		else if (nodePtr->getTypeId().isDerivedFrom(SoToggleButton::getClassTypeId())) 
		{
			toggleButPtr = (SoToggleButton*)nodePtr;
		}
		else if (nodePtr->getTypeId().isDerivedFrom(SoLabelListBox::getClassTypeId())) 
		{
			labelListBoxPtr = (SoLabelListBox*)nodePtr;
		}
		else 
		{
			printf("SoAddUserWidgetConnectionToFanIn::setup  widget type could not be detected.\nNo connection established !!\n");
			return;
		}
	}

	SoNode *engWrapPtr = base->getByName(fanIn.getValue());
	SoEngine *engPtr = NULL;
	
	if (engWrapPtr->getTypeId().isDerivedFrom(SoEngineWrapper::getClassTypeId())) 
		engPtr = ((SoEngineWrapper *)engWrapPtr)->engine.getValue();
	

	if (engPtr == NULL) 
	{
		printf("SoAddUserWidgetConnectionToFanIn::setup  fanIn not connected to a SoEngineWrapper node.\nNo connection established !!\n");
		return;
	}

	// printf("SoAddUserWidgetConnectionToFanIn::setup SoEngineWrapper is %p\n", engWrapPtr);

	SoFanIn *fanInPtr = NULL;
	
	if (engPtr->getTypeId().isDerivedFrom(SoFanIn::getClassTypeId())) 
		fanInPtr = (SoFanIn *)engPtr;
	
	if (fanInPtr == NULL) 
	{
		printf("SoAddUserWidgetConnectionToFanIn::setup  fanIn not connected to a SoFanIn engine.\nNo connection established !!\n");
		return;
	}

	if (pushButPtr != NULL)
	{
		setupPushButton(pushButPtr, fanInPtr, uid);
	} 
}


