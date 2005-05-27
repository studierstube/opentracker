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
/** Implementation of class SoPucBuildLayoutAction
  *
  * @author Valerie Maquil
  *
  * $Id: SoBuildPucStyleAction.cxx 3521 2004-07-21 08:42:40Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/misc/SoChildList.h>


#include "..\puc\SoPucDevice.h"
#include "..\puc\SoPucElementBase.h"
#include "..\puc\SoPucStateBase.h"
#include "..\puc\SoPucStateInt.h"
#include "..\puc\SoPucStateFloat.h"
#include "..\puc\SoPucStateFixed.h"
#include "..\puc\SoPucStateString.h"

#include "..\puc\SoPucStateEnumerated.h"
#include "..\puc\SoPucStateBool.h"
#include "..\puc\SoPucGroup.h"
#include "..\puc\SoPucCommand.h"


#include "..\puc\SoPucActiveIfNode.h"
#include "..\puc\SoPucActiveIfClause.h"
#include "..\puc\SoPucActiveIfOperator.h"

#include "SoBuildPucStyleAction.h"

#include <iostream>

using namespace std;

#ifdef _DEBUG_PUC
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

//const int SoBuildPucStyleAction::errorReturnCode = 1024;

SO_ACTION_SOURCE(SoBuildPucStyleAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoBuildPucStyleAction::initClass()
{

    SO_ACTION_INIT_CLASS(SoBuildPucStyleAction, SoAction);

  
    // register static methods to be called for a specific node

	SO_ACTION_ADD_METHOD(SoNode,			callDoAction);
	SO_ACTION_ADD_METHOD(SoNodeKitListPart,	callDoAction);
	SO_ACTION_ADD_METHOD(SoGroup,			callDoAction);

	SO_ACTION_ADD_METHOD(SoPucStateBool,    stateBoolAction);
	SO_ACTION_ADD_METHOD(SoPucStateFixed,   stateFixedAction);
	SO_ACTION_ADD_METHOD(SoPucStateFloat,   stateFloatAction);
	SO_ACTION_ADD_METHOD(SoPucStateInt,		stateIntAction);
	SO_ACTION_ADD_METHOD(SoPucStateString,	stateStringAction);

	SO_ACTION_ADD_METHOD(SoPucGroup,		pucGroupAction);
	SO_ACTION_ADD_METHOD(SoPucCommand,		commandAction);
	SO_ACTION_ADD_METHOD(SoPucStateEnumerated,		stateEnumAction);

}



//
// constructor
//

SoBuildPucStyleAction::SoBuildPucStyleAction()
{
    SO_ACTION_CONSTRUCTOR(SoBuildPucStyleAction);

}


//
// destructor
//

SoBuildPucStyleAction::~SoBuildPucStyleAction()
{	
}



//
// begin the traversal of the scenegraph
//


void SoBuildPucStyleAction::beginTraversal(SoNode *node)
{	
	mainLayoutGroup = new SoWidgetLayoutGroup;
	mainLayoutGroup->setUpConnections(TRUE, TRUE);
	mainLayoutGroup->numOfCols.setValue(1);
	mainNodeList = SO_GET_PART(mainLayoutGroup, "elements", SoNodeKitListPart);
	assert(mainNodeList != NULL);

	curNumOfRows = 0;

	traverse(node);	
}




static const std::string endlString() { return "\n"; }
#define ENDL endlString()


void SoBuildPucStyleAction::callDoAction(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucStyleAction::callDoAction %x,%s\n",node,node->getTypeId().getName().getString()));
	node->doAction(action);
}


//-------------------------------------------------------------------------------------------------------------------

void SoBuildPucStyleAction::commandAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucStyleAction::commandAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucStyleAction *action = (SoBuildPucStyleAction *)action_;
	SoPucCommand *state = (SoPucCommand *)node;

	//add widget to the row
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getPushButtonForCommand(state, LONG_LABEL);
	widgetToAdd->labelPlacing.setValue("LEFT");
	action->mainNodeList->addChild( widgetToAdd );
	action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 1);

	//increase counters
	action->curNumOfRows++;

}


//-------------------------------------------------------------------------------------------------------------------

void SoBuildPucStyleAction::stateBoolAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucStyleAction::stateBoolAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucStyleAction *action = (SoBuildPucStyleAction *)action_;
	SoPucStateBool *state = (SoPucStateBool *)node;

	//add widget to the row
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getToggleButtonForStateBool(state, LONG_LABEL);
	widgetToAdd->labelPlacing.setValue("LEFT");
	action->mainNodeList->addChild( widgetToAdd );

	action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 1);

	//increase counters
	action->curNumOfRows++;


}

//-------------------------------------------------------------------------------------------------------------------


void SoBuildPucStyleAction::stateFloatAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucStyleAction::stateFloatAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucStyleAction *action = (SoBuildPucStyleAction *)action_;
	SoPucStateFloat *state = (SoPucStateFloat *)node;

	//add widget to the row
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getSliderForStateFloat(state, LONG_LABEL);
	widgetToAdd->labelPlacing.setValue("LEFT");
	action->mainNodeList->addChild( widgetToAdd );
	action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 1);

	//increase counters
	action->curNumOfRows++;

}

//-------------------------------------------------------------------------------------------------------------------


void SoBuildPucStyleAction::stateFixedAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucStyleAction::stateFixedAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucStyleAction *action = (SoBuildPucStyleAction *)action_;
	SoPucStateFixed *state = (SoPucStateFixed *)node;

	int num = (state->max.getValue() - state->min.getValue())/state->incr.getValue()+1;

	SoBehaviorKit *widgetToAdd;

	if (num>4)
	{
		//add slider to the row
		widgetToAdd = SoWidgetSelectionFromPuc::getSliderForStateFixed(state, LONG_LABEL);
	}
	else
	{
		//add list box to the row
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateFixed(state, num, 1, 0, LONG_LABEL);
	}

	widgetToAdd->labelPlacing.setValue("LEFT");
	action->mainNodeList->addChild( widgetToAdd );
	action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 1);

	//increase counters
	action->curNumOfRows++;

}

//-------------------------------------------------------------------------------------------------------------------


void SoBuildPucStyleAction::stateIntAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucStyleAction::stateIntAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucStyleAction *action = (SoBuildPucStyleAction *)action_;
	SoPucStateInt *state = (SoPucStateInt *)node;

	int num = (state->max.getValue() - state->min.getValue())/state->incr.getValue()+1;

	SoBehaviorKit *widgetToAdd;

/*	if (num>4)
	{
*/
		//add slider to the row
		widgetToAdd = SoWidgetSelectionFromPuc::getSliderForStateInt(state, LONG_LABEL);
/*	}
	else
	{
		//add list box to the row
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateInt(state, num, 1, 0, LONG_LABEL);
	}
*/
	widgetToAdd->labelPlacing.setValue("LEFT");
	action->mainNodeList->addChild( widgetToAdd );
	action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 1);

	//increase counters
	action->curNumOfRows++;

}	

//-------------------------------------------------------------------------------------------------------------------

void SoBuildPucStyleAction::stateStringAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucStyleAction::stateStringAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucStyleAction *action = (SoBuildPucStyleAction *)action_;
	SoPucStateString *state = (SoPucStateString *)node;

	SoBehaviorKit *widgetToAdd;

	//add slider to the row
	widgetToAdd = SoWidgetSelectionFromPuc::getTextBoxForStateString(state, LONG_LABEL);
	
	widgetToAdd->labelPlacing.setValue("LEFT");
	action->mainNodeList->addChild( widgetToAdd );
	action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 1);

	//increase counters
	action->curNumOfRows++;

}	

//-------------------------------------------------------------------------------------------------------------------

void SoBuildPucStyleAction::stateEnumAction(SoAction *action_, SoNode *node)
{
	DEBUG_CODE(printf("SoBuildPucStyleAction::stateEnumAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucStyleAction *action = (SoBuildPucStyleAction *)action_;
	SoPucStateEnumerated *state = (SoPucStateEnumerated *)node;

	int num = state->valueLabels.getNum();

	SoBehaviorKit *widgetToAdd;

	if (num<5)
	{
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, num, 1, 0, LONG_LABEL);
		action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 1);

	}
	else if (num<9)
	{
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, 3, 1, 5, LONG_LABEL);
		action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 1);
	}
	else 
	{
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, 3, 2, 2, LONG_LABEL);
		action->mainLayoutGroup->sizeOfRows.set1Value(action->curNumOfRows, 2);
	}
	widgetToAdd->labelPlacing.setValue("LEFT");	
	action->mainNodeList->addChild( widgetToAdd );
	

	//increase counters
	action->curNumOfRows++;
}
//-------------------------------------------------------------------------------------------------------------------


void SoBuildPucStyleAction::pucGroupAction(SoAction *action_, SoNode *node_)
{
	DEBUG_CODE(printf("SoBuildPucStyleAction::pucGroupAction %x,%s\n",node_,node_->getTypeId().getName().getString()));

	node_->doAction(action_);

}

//-------------------------------------------------------------------------------------------------------------------