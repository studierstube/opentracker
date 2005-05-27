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
/** SoSelectConnectionForUser.cxx  - implementation of class SoSelectConnectionForUserr (version 2).
  * SoSelectConnectionForUser is the implementation of a Node that creates a 
  * pipDependend field connection.
  *
  * @author  Thomas Psik
  *
  * $Id: SoSelectConnectionForUser.cxx 3964 2004-10-19 13:43:28Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoSelectConnectionForUser.h"
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoGate.h>
#include <Inventor/engines/SoTriggerAny.h>

#include <stbapi/widgets/SoPushButton.h>

#include <stbapi/widgets/SoToggleButton.h>
#include <stbapi/widgets/SoLabelListBox.h>
#include <stbapi/widgets/SoIncrementalSlider.h>

#include <stbapi/misc/SoFanIn.h>
#include <stbapi/misc/SoEngineWrapper.h>


#include <extensions/PUC/SoPucStateEnumerated.h>
#include <extensions/PUC/SoPucCommand.h>
#include <extensions/PUC/SoPucStateBool.h>
#include <extensions/PUC/SoPucStateFixed.h>

#include <string>
#include <cstdarg>

using namespace std;

SO_NODE_SOURCE(SoSelectConnectionForUser);

void
SoSelectConnectionForUser::initClass()
{
    SO_NODE_INIT_CLASS(SoSelectConnectionForUser, SoNode, "Node");
}

SoSelectConnectionForUser::SoSelectConnectionForUser()
{
    SO_NODE_CONSTRUCTOR(SoSelectConnectionForUser);

    SO_NODE_ADD_FIELD(inputs, (""));
    SO_NODE_ADD_FIELD(widget, (""));
}


void SoSelectConnectionForUser::setupEnumToListBox(SoLabelListBox *listBox, SoPucStateEnumerated *stateEnum) 
{
	listBox->selectedIndicesIn.connectFrom(&(stateEnum->value));
	stateEnum->value.connectFrom(&(listBox->selectedIndices));

}
		
void SoSelectConnectionForUser::setupCommandToPushButton(SoPushButton *button,SoPucCommand* command)
{
	button->triggerIn.connectFrom(&(command->trigger));
	command->triggerIn.connectFrom(&(button->triggerOut));
	
	if (command->triggerIn.isConnectedFromField())
	{
		SoTriggerAny *fanIn = new SoTriggerAny();
		SoField *fromCon = NULL;
		
		// must be true, because pucFixed->value.isConnectedFromField()
		assert( (command->triggerIn.getConnectedField(fromCon )==TRUE));

		SoFanIn::getNextUnconnectedFieldFromEngine(fanIn)->connectFrom(fromCon);
		SoFanIn::getNextUnconnectedFieldFromEngine(fanIn)->connectFrom(&(button->triggerOut));
		command->triggerIn.connectFrom(&(fanIn->output));
	}
	else if (command->triggerIn.isConnectedFromEngine())
	{
		SoEngineOutput  *fanInOut = NULL;
		// must be true, because pucFixed->value.isConnectedFromEngine()
		assert(command->triggerIn.getConnectedEngine( fanInOut) == TRUE);
		SoEngine *eng = fanInOut->getContainer ();

		assert(eng!=NULL);
		assert(eng->getTypeId().isDerivedFrom(SoTriggerAny::getClassTypeId()));
		SoFanIn *fanIn = (SoFanIn *)eng;
		SoFanIn::getNextUnconnectedFieldFromEngine(fanIn)->connectFrom(&(button->triggerOut));
	}
	else
		command->triggerIn.connectFrom(&(button->triggerOut));
	
	button->triggerIn.connectFrom(&(command->trigger));

}

void SoSelectConnectionForUser::setupBoolToToggleButton(SoToggleButton *button, SoPucStateBool *pucBool)
{
//	button->onIn.connectFrom(&(pucBool->value));
//	pucBool->value.connectFrom(&(button->on));
	if (pucBool->value.isConnectedFromField())
	{
		SoFanIn *fanIn = new SoFanIn(SoMFFloat::getClassTypeId());
		SoField *fromCon = NULL;
		
		// must be true, because pucFixed->value.isConnectedFromField()
		assert( (pucBool->value.getConnectedField(fromCon )==TRUE));

		fanIn->getNextUnconnectedField()->connectFrom(fromCon);
		fanIn->getNextUnconnectedField()->connectFrom(&(button->on));
		pucBool->value.connectFrom((fanIn->out));
	}
	else if (pucBool->value.isConnectedFromEngine())
	{
		SoEngineOutput  *fanInOut = NULL;
		// must be true, because pucFixed->value.isConnectedFromEngine()
		assert(pucBool->value.getConnectedEngine( fanInOut) == TRUE);
		SoEngine *eng = fanInOut->getContainer ();

		assert(eng!=NULL);
		assert(eng->getTypeId().isDerivedFrom(SoFanIn::getClassTypeId()));
		SoFanIn *fanIn = (SoFanIn *)eng;
		fanIn->getNextUnconnectedField()->connectFrom(&(button->on));
	}
	else
		pucBool->value.connectFrom(&(button->on));
	
	button->onIn.connectFrom(&(pucBool->value));
}	

void SoSelectConnectionForUser::setupFixedToIncSlider(SoIncrementalSlider *slider, SoPucStateFixed *pucFixed)
{
	if (pucFixed->value.isConnectedFromField())
	{
		SoFanIn *fanIn = new SoFanIn(SoMFFloat::getClassTypeId());
		SoField *fromCon = NULL;
		
		// must be true, because pucFixed->value.isConnectedFromField()
		assert( (pucFixed->value.getConnectedField(fromCon )==TRUE));

		fanIn->getNextUnconnectedField()->connectFrom(fromCon);
		fanIn->getNextUnconnectedField()->connectFrom(&(slider->value));
		pucFixed->value.connectFrom((fanIn->out));
	}
	else if (pucFixed->value.isConnectedFromEngine())
	{
		SoEngineOutput  *fanInOut = NULL;
		// must be true, because pucFixed->value.isConnectedFromEngine()
		assert(pucFixed->value.getConnectedEngine( fanInOut) == TRUE);
		SoEngine *eng = fanInOut->getContainer ();

		assert(eng!=NULL);
		assert(eng->getTypeId().isDerivedFrom(SoFanIn::getClassTypeId()));
		SoFanIn *fanIn = (SoFanIn *)eng;
		fanIn->getNextUnconnectedField()->connectFrom(&(slider->value));
	}
	else
		pucFixed->value.connectFrom(&(slider->value));
	
	slider->valueIn.connectFrom(&(pucFixed->value));
}

// --------------------------------------------------------------------------
void SoSelectConnectionForUser::setup(int uid, SoNode *base) 
{
	SoNode *nodePtr = base->getByName(widget.getValue());
	
	SoPushButton *pushButPtr = NULL;
	SoToggleButton *toggleButPtr = NULL;
	SoLabelListBox *labelListBoxPtr = NULL;
	SoIncrementalSlider *sliderPtr = NULL;

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
		else if (nodePtr->getTypeId().isDerivedFrom(SoIncrementalSlider::getClassTypeId())) 
		{
			sliderPtr = (SoIncrementalSlider*)nodePtr;
		}
		else 
		{
			printf("SoSelectConnectionForUser::setup  widget type for [%s] could not be detected.\nNo connection established !!\n", widget.getValue().getString());
			return;
		}
	}
	else 
	{
		printf("SoSelectConnectionForUser::setup  widget could not be resolved [%s].\nNo connection established !!\n", widget.getValue().getString());
		return;
	}

	SbString stateName;
	if (inputs.getNum()-1 < uid) 
	{
		uid = inputs.getNum()-1;
		printf("SoSelectConnectionForUser::setup there are not enough inputs [%d] < for user #[%d].\nLast value is taken instead !!\n", 
			inputs.getNum(), uid+1);
	
	}

	stateName = inputs[uid];
	
	nodePtr = base->getByName(stateName );
	
	SoPucElementBase *pucElm = NULL;
	
	if (nodePtr->getTypeId().isDerivedFrom(SoPucElementBase::getClassTypeId())) 
		pucElm = (SoPucElementBase*)nodePtr;
	
	SoPucStateEnumerated *pucEnum = NULL;
	SoPucCommand *pucCom = NULL;
	SoPucStateBool *pucBool = NULL;
	SoPucStateFixed *pucFixed = NULL;

	if (pucElm != NULL) 
	{
		if (pucElm->getTypeId().isDerivedFrom(SoPucCommand::getClassTypeId())) 
		{
			pucCom = (SoPucCommand*)pucElm;
		} 
		else if (pucElm->getTypeId().isDerivedFrom(SoPucStateEnumerated::getClassTypeId())) 
		{
			pucEnum = (SoPucStateEnumerated*)pucElm;
		}
		else if (pucElm->getTypeId().isDerivedFrom(SoPucStateBool::getClassTypeId())) 
		{
			pucBool = (SoPucStateBool*)pucElm;
		}
		else if (pucElm->getTypeId().isDerivedFrom(SoPucStateFixed::getClassTypeId())) 
		{
			pucFixed = (SoPucStateFixed*)pucElm;
		}
		else 
		{
			printf("SoSelectConnectionForUser::setup  widget type for [%s] could not be detected.\nNo connection established !!\n", stateName.getString());
			return;
		}
	}
	else 
	{
		printf("SoSelectConnectionForUser::setup  inputs [%s] not connected to a SoPucElementBase node.\nNo connection established !!\n", stateName.getString());
		return;
	}

	if (labelListBoxPtr != NULL && pucEnum != NULL)
	{
		setupEnumToListBox(labelListBoxPtr, pucEnum);
	} 
	else if ( pushButPtr!= NULL && pucCom != NULL)
	{
		setupCommandToPushButton(pushButPtr, pucCom);	
	}
	else if ( toggleButPtr!= NULL && pucBool != NULL)
	{
		setupBoolToToggleButton(toggleButPtr, pucBool);	
	}
	else if ( sliderPtr!= NULL && pucFixed != NULL)
	{
		setupFixedToIncSlider(sliderPtr, pucFixed);	
	}


}


