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
  * $Id: SoBuildStbStyleAction.cxx 3426 2004-07-14 12:11:27Z tamer $
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
#include "..\puc\SoPucStateEnumerated.h"
#include "..\puc\SoPucStateBool.h"
#include "..\puc\SoPucGroup.h"
#include "..\puc\SoPucCommand.h"


#include "..\puc\SoPucActiveIfNode.h"
#include "..\puc\SoPucActiveIfClause.h"
#include "..\puc\SoPucActiveIfOperator.h"

#include "SoBuildStbStyleAction.h"

#include <iostream>

using namespace std;

#ifdef _DEBUG_PUC
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

SO_ACTION_SOURCE(SoBuildStbStyleAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoBuildStbStyleAction::initClass()
{

    SO_ACTION_INIT_CLASS(SoBuildStbStyleAction, SoAction);
 
    // register static methods to be called for a specific node

	SO_ACTION_ADD_METHOD(SoNode,			callDoAction);
	SO_ACTION_ADD_METHOD(SoNodeKitListPart,	callDoAction);
	SO_ACTION_ADD_METHOD(SoGroup,			callDoAction);

	SO_ACTION_ADD_METHOD(SoPucStateBool,    stateBoolAction);
	SO_ACTION_ADD_METHOD(SoPucStateFixed,    stateFixedAction);
	SO_ACTION_ADD_METHOD(SoPucStateFloat,    stateFloatAction);
	SO_ACTION_ADD_METHOD(SoPucStateInt,		stateIntAction);
	SO_ACTION_ADD_METHOD(SoPucGroup,		pucGroupAction);
	SO_ACTION_ADD_METHOD(SoPucCommand,		commandAction);
	SO_ACTION_ADD_METHOD(SoPucStateEnumerated,		stateEnumAction);
}



//
// constructor
//

SoBuildStbStyleAction::SoBuildStbStyleAction()
{
    SO_ACTION_CONSTRUCTOR(SoBuildStbStyleAction);
		
}


//
// destructor
//

SoBuildStbStyleAction::~SoBuildStbStyleAction()
{	
}


void SoBuildStbStyleAction::setUnits(int units)
{
	numOfUnits = units;
}

//
// begin the traversal of the scenegraph
//


void SoBuildStbStyleAction::beginTraversal(SoNode *node)
{	
	//Container to store the different rows (layout groups)
	mainLayoutGroup = new SoWidgetLayoutGroup;
	mainLayoutGroup->setUpConnections(TRUE, TRUE);
	mainLayoutGroup->numOfCols.setValue(1);
	mainNodeList = SO_GET_PART(mainLayoutGroup, "elements", SoNodeKitListPart);
	assert(mainNodeList != NULL);

	first = TRUE;

	traverse(node);	
}


void SoBuildStbStyleAction::endTraversal(SoNode *node)
{	
	int emptySpace = numOfUnits - curNumOfUnits;
	layoutGroupPerRow->numOfCols.setValue(curNumOfCols + 1);
	layoutGroupPerRow->sizeOfCols.set1Value(curNumOfCols, emptySpace);

}


static const std::string endlString() { return "\n"; }
#define ENDL endlString()


void SoBuildStbStyleAction::callDoAction(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildStbStyleAction::callDoAction %x,%s\n",node,node->getTypeId().getName().getString()));
	node->doAction(action);
}

//-------------------------------------------------------------------------------------------------------------------

void SoBuildStbStyleAction::initLayoutGroup()
{
	//fill the previous row up with an empty space
	if (first) first = FALSE;
	else
	{	
		int emptySpace = numOfUnits - curNumOfUnits;
		layoutGroupPerRow->numOfCols.setValue(curNumOfCols + 1);
		layoutGroupPerRow->sizeOfCols.set1Value(curNumOfCols, emptySpace);
	}

	//initialize new WidgetLayoutGroup for the new row, and set all the links
	layoutGroupPerRow = new SoWidgetLayoutGroup();
	layoutGroupPerRow->setUpConnections(TRUE, TRUE);
		
	mainNodeList->addChild(layoutGroupPerRow);
		
	layoutGroupPerRow->numOfRows.setValue(1);
	nodeList = SO_GET_PART(layoutGroupPerRow, "elements", SoNodeKitListPart);
	assert( nodeList != NULL );

	curNumOfUnits = 0;
	curNumOfCols = 0;
}

//-------------------------------------------------------------------------------------------------------------------

void SoBuildStbStyleAction::commandAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildStbStyleAction::commandAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildStbStyleAction *action = (SoBuildStbStyleAction *)action_;
	SoPucCommand *state = (SoPucCommand *)node;


	//if row is full create a new row
	if (action->curNumOfUnits + 1 > action->numOfUnits) action->initLayoutGroup();

	
	//add widget to the row
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getPushButtonForCommand(state, SMALL_LABEL);
	widgetToAdd->labelPlacing.setValue("ONWIDGET");
	action->nodeList->addChild( widgetToAdd );

	action->layoutGroupPerRow->sizeOfCols.set1Value(action->curNumOfCols, 1);

	//increase counters
	action->curNumOfCols++;
	action->curNumOfUnits += 1;

}


//-------------------------------------------------------------------------------------------------------------------

void SoBuildStbStyleAction::stateBoolAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildStbStyleAction::stateBoolAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildStbStyleAction *action = (SoBuildStbStyleAction *)action_;
	SoPucStateBool *state = (SoPucStateBool *)node;


	//if row is full create a new row
	if (action->curNumOfUnits + 1 > action->numOfUnits) action->initLayoutGroup();


	//add widget to the row
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getToggleButtonForStateBool(state, SMALL_LABEL);
	widgetToAdd->labelPlacing.setValue("ONWIDGET");
	action->nodeList->addChild( widgetToAdd );

	action->layoutGroupPerRow->sizeOfCols.set1Value(action->curNumOfCols, 1);

	//increase counters
	action->curNumOfCols++;
	action->curNumOfUnits += 1;

}

//-------------------------------------------------------------------------------------------------------------------


void SoBuildStbStyleAction::stateFloatAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildStbStyleAction::stateFloatAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildStbStyleAction *action = (SoBuildStbStyleAction *)action_;
	SoPucStateFloat *state = (SoPucStateFloat *)node;

	
	//if row is full create a new row
	if (action->curNumOfUnits + 2 > action->numOfUnits) action->initLayoutGroup();

	
	//add widget to the row
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getSliderForStateFloat(state, SMALL_LABEL);
	widgetToAdd->labelPlacing.setValue("TOP");
	action->nodeList->addChild( widgetToAdd );


	action->layoutGroupPerRow->sizeOfCols.set1Value(action->curNumOfCols, 2);

	//increase counters
	action->curNumOfCols++;
	action->curNumOfUnits += 2;

}

//-------------------------------------------------------------------------------------------------------------------


void SoBuildStbStyleAction::stateFixedAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildStbStyleAction::stateFixedAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildStbStyleAction *action = (SoBuildStbStyleAction *)action_;
	SoPucStateFixed *state = (SoPucStateFixed *)node;

	//if row is full create a new row
	if (action->curNumOfUnits + 2 > action->numOfUnits) action->initLayoutGroup();
	
	int num = (state->max.getValue() - state->min.getValue())/state->incr.getValue()+1;

	SoBehaviorKit *widgetToAdd;

	if (num>4)
	{
		//add slider to the row
		widgetToAdd = SoWidgetSelectionFromPuc::getSliderForStateFixed(state, SMALL_LABEL);
	}
	else
	{
		//add list box to the row
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateFixed(state, num, 1, 0, SMALL_LABEL);
	}

	widgetToAdd->labelPlacing.setValue("TOP");

	//add widget to the row
	action->nodeList->addChild(widgetToAdd);

	action->layoutGroupPerRow->sizeOfCols.set1Value(action->curNumOfCols, 2);

	//increase counters
	action->curNumOfCols++;
	action->curNumOfUnits += 2;

}

//-------------------------------------------------------------------------------------------------------------------


void SoBuildStbStyleAction::stateIntAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildStbStyleAction::stateIntAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildStbStyleAction *action = (SoBuildStbStyleAction *)action_;
	SoPucStateInt *state = (SoPucStateInt *)node;

	//if row is full create a new row
	if (action->curNumOfUnits + 2 > action->numOfUnits) action->initLayoutGroup();

	int num = (state->max.getValue() - state->min.getValue())/state->incr.getValue()+1;

	SoBehaviorKit *widgetToAdd;

	if (num>4)
	{
		//add slider to the row
		widgetToAdd = SoWidgetSelectionFromPuc::getSliderForStateInt(state, SMALL_LABEL);
	}
	else
	{
		//add list box to the row
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateInt(state, num, 1, 0, SMALL_LABEL);
	}

	widgetToAdd->labelPlacing.setValue("TOP");

	//add widget to the row
	action->nodeList->addChild(widgetToAdd);

	action->layoutGroupPerRow->sizeOfCols.set1Value(action->curNumOfCols, 2);

	//increase counters
	action->curNumOfCols++;
	action->curNumOfUnits += 2;
}	

//-------------------------------------------------------------------------------------------------------------------

void SoBuildStbStyleAction::stateEnumAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildStbStyleAction::stateEnumAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildStbStyleAction *action = (SoBuildStbStyleAction *)action_;
	SoPucStateEnumerated *state = (SoPucStateEnumerated *)node;

	//if row is full create a new row
	if (action->curNumOfUnits + 2 > action->numOfUnits) action->initLayoutGroup();

		int num = state->valueLabels.getNum();

	SoBehaviorKit *widgetToAdd;

	if (num<5)
	{
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, num, 1, 0, SMALL_LABEL);
	}
	else 
	{
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, 3, 1, 5, SMALL_LABEL);
	}

	widgetToAdd->labelPlacing.setValue("TOP");
	action->nodeList->addChild(widgetToAdd);
	action->layoutGroupPerRow->sizeOfCols.set1Value(action->curNumOfCols, 2);

	//increase counters
	action->curNumOfCols++;
	action->curNumOfUnits += 2;
}	

//-------------------------------------------------------------------------------------------------------------------


void SoBuildStbStyleAction::pucGroupAction(SoAction *action_, SoNode *node_)
{
	DEBUG_CODE(printf("SoBuildStbStyleAction::pucGroupAction %x,%s\n",node_,node_->getTypeId().getName().getString()));

	SoBuildStbStyleAction *action = (SoBuildStbStyleAction *)action_;
	SoPucGroup *node = (SoPucGroup *)node_;
	SoState *state = action->getState();
	

	//create a new row
	if (action->curNumOfUnits != 0) action->initLayoutGroup();
	

	node_->doAction(action);

}

//-------------------------------------------------------------------------------------------------------------------