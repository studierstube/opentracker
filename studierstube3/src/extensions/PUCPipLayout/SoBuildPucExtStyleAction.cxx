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
  * $Id: SoBuildPucExtStyleAction.cxx 3426 2004-07-14 12:11:27Z tamer $
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

#include "SoBuildPucExtStyleAction.h"

#include <iostream>

using namespace std;

#ifdef _DEBUG_PUC
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

SO_ACTION_SOURCE(SoBuildPucExtStyleAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoBuildPucExtStyleAction::initClass()
{

    SO_ACTION_INIT_CLASS(SoBuildPucExtStyleAction, SoAction);
 
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

SoBuildPucExtStyleAction::SoBuildPucExtStyleAction()
{
    SO_ACTION_CONSTRUCTOR(SoBuildPucExtStyleAction);
}


//
// destructor
//

SoBuildPucExtStyleAction::~SoBuildPucExtStyleAction()
{	
}


void SoBuildPucExtStyleAction::setCols(int cols)
{
	numOfCols = cols;
}

//
// begin the traversal of the scenegraph
//


void SoBuildPucExtStyleAction::beginTraversal(SoNode *node)
{	
	//Container to store the different rows (layout groups)
	mainLayoutGroup = new SoWidgetLayoutGroup();
	mainLayoutGroup->setUpConnections(TRUE, TRUE);
	mainLayoutGroup->numOfRows.setValue(1);
	mainNodeList = SO_GET_PART(mainLayoutGroup, "elements", SoNodeKitListPart);
	assert(mainNodeList != NULL);

	columnsHeight = SbIntList();
	numRows = SbIntList();

	for (int i= 0; i < numOfCols; i++)
	{
		columnsHeight[i] = 0;
		numRows[i] = 0;
		SoWidgetLayoutGroup *layoutGroup = new SoWidgetLayoutGroup();
		layoutGroup->numOfCols.setValue(1);
		mainNodeList->addChild(layoutGroup);
	}

	traverse(node);	
}


void SoBuildPucExtStyleAction::endTraversal(SoNode *node)
{	
	//fill columns up equally
	int maxHeight = columnsHeight[0];

	for (int i= 1; i< numOfCols; i++)
	{
		if (columnsHeight[i]>maxHeight) maxHeight=columnsHeight[i];
	}

	for (i= 0; i< numOfCols; i++)
	{
		if (columnsHeight[i]<maxHeight) 
		{
			layoutGroupPerCol = (SoWidgetLayoutGroup *)mainNodeList->getChild(i);
			layoutGroupPerCol->numOfRows.setValue(numRows[i]+2);
			layoutGroupPerCol->sizeOfRows.set1Value(numRows[i]+1, maxHeight-columnsHeight[i]);
		}
	}

}


static const std::string endlString() { return "\n"; }
#define ENDL endlString()


void SoBuildPucExtStyleAction::callDoAction(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucExtStyleAction::callDoAction %x,%s\n",node,node->getTypeId().getName().getString()));
	node->doAction(action);
}

//-------------------------------------------------------------------------------------------------------------------
int SoBuildPucExtStyleAction::getMinIndex(SbIntList list, int numCols)
{
	int index = 0;
	int minHeight = list[0];

	for (int i= 1; i< numCols; i++)
	{
		if (list[i]<minHeight) 
		{
			minHeight=list[i];
			index = i;
		}
	}

	return index;
}

//-------------------------------------------------------------------------------------------------------------------

void SoBuildPucExtStyleAction::commandAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucExtStyleAction::commandAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucExtStyleAction *action = (SoBuildPucExtStyleAction *)action_;
	SoPucCommand *state = (SoPucCommand *)node;

	//get layoutGroup with minimal columnsHeight
	int minIndex = action->getMinIndex(action->columnsHeight, action->numOfCols);
	action->layoutGroupPerCol = (SoWidgetLayoutGroup *)action->mainNodeList->getChild(minIndex);

	action->nodeList = SO_GET_PART(action->layoutGroupPerCol, "elements", SoNodeKitListPart);
	assert( action->nodeList != NULL );
	action->layoutGroupPerCol->sizeOfRows.set1Value(action->numRows[minIndex], 1);
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getPushButtonForCommand(state, SMALL_LABEL);
	widgetToAdd->labelPlacing.setValue("ONWIDGET");
	action->nodeList->addChild( widgetToAdd );

	action->columnsHeight[minIndex]++;
	action->numRows[minIndex]++;
}


//-------------------------------------------------------------------------------------------------------------------

void SoBuildPucExtStyleAction::stateBoolAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucExtStyleAction::stateBoolAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucExtStyleAction *action = (SoBuildPucExtStyleAction *)action_;
	SoPucStateBool *state = (SoPucStateBool *)node;
	
	//get layoutGroup with minimal columnsHeight
	int minIndex = action->getMinIndex(action->columnsHeight, action->numOfCols);
	action->layoutGroupPerCol = (SoWidgetLayoutGroup *)action->mainNodeList->getChild(minIndex);

	action->nodeList = SO_GET_PART(action->layoutGroupPerCol, "elements", SoNodeKitListPart);
	assert( action->nodeList != NULL );
	action->layoutGroupPerCol->sizeOfRows.set1Value(action->numRows[minIndex], 1);
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getToggleButtonForStateBool(state, SMALL_LABEL);
	widgetToAdd->labelPlacing.setValue("ONWIDGET");
	action->nodeList->addChild( widgetToAdd );

	action->columnsHeight[minIndex]++;
	action->numRows[minIndex]++;
}

//-------------------------------------------------------------------------------------------------------------------


void SoBuildPucExtStyleAction::stateFloatAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucExtStyleAction::stateFloatAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucExtStyleAction *action = (SoBuildPucExtStyleAction *)action_;
	SoPucStateFloat *state = (SoPucStateFloat *)node;

	
	//get layoutGroup with minimal columnsHeight
	int minIndex = action->getMinIndex(action->columnsHeight, action->numOfCols);
	action->layoutGroupPerCol = (SoWidgetLayoutGroup *)action->mainNodeList->getChild(minIndex);

	action->nodeList = SO_GET_PART(action->layoutGroupPerCol, "elements", SoNodeKitListPart);
	assert( action->nodeList != NULL );
	action->layoutGroupPerCol->sizeOfRows.set1Value(action->numRows[minIndex], 1);
	SoBehaviorKit *widgetToAdd = SoWidgetSelectionFromPuc::getSliderForStateFloat(state, SMALL_LABEL);
	widgetToAdd->labelPlacing.setValue("TOP");
	action->nodeList->addChild( widgetToAdd );

	action->columnsHeight[minIndex]+=1;
	action->numRows[minIndex]++;
}

//-------------------------------------------------------------------------------------------------------------------


void SoBuildPucExtStyleAction::stateFixedAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucExtStyleAction::stateFixedAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucExtStyleAction *action = (SoBuildPucExtStyleAction *)action_;
	SoPucStateFixed *state = (SoPucStateFixed *)node;

	//get layoutGroup with minimal columnsHeight
	int minIndex = action->getMinIndex(action->columnsHeight, action->numOfCols);
	action->layoutGroupPerCol = (SoWidgetLayoutGroup *)action->mainNodeList->getChild(minIndex);

	action->nodeList = SO_GET_PART(action->layoutGroupPerCol, "elements", SoNodeKitListPart);
	assert( action->nodeList != NULL );
	action->layoutGroupPerCol->sizeOfRows.set1Value(action->numRows[minIndex], 1);

	int num = (state->max.getValue() - state->min.getValue())/state->incr.getValue()+1;

	SoBehaviorKit *widgetToAdd;

	if (num>3)
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
	action->nodeList->addChild( widgetToAdd );

	action->columnsHeight[minIndex]+=1;
	action->numRows[minIndex]++;
}

//-------------------------------------------------------------------------------------------------------------------

void SoBuildPucExtStyleAction::stateIntAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucExtStyleAction::stateIntAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucExtStyleAction *action = (SoBuildPucExtStyleAction *)action_;
	SoPucStateInt *state = (SoPucStateInt *)node;

	//get layoutGroup with minimal columnsHeight
	int minIndex = action->getMinIndex(action->columnsHeight, action->numOfCols);
	action->layoutGroupPerCol = (SoWidgetLayoutGroup *)action->mainNodeList->getChild(minIndex);

	action->nodeList = SO_GET_PART(action->layoutGroupPerCol, "elements", SoNodeKitListPart);
	assert( action->nodeList != NULL );
	action->layoutGroupPerCol->sizeOfRows.set1Value(action->numRows[minIndex], 1);

	int num = (state->max.getValue() - state->min.getValue())/state->incr.getValue()+1;

	SoBehaviorKit *widgetToAdd;

	if (num>3)
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
	action->nodeList->addChild( widgetToAdd );

	action->columnsHeight[minIndex]+=1;
	action->numRows[minIndex]++;
}	

//-------------------------------------------------------------------------------------------------------------------

void SoBuildPucExtStyleAction::stateEnumAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPucExtStyleAction::stateEnumAction %x,%s\n",node,node->getTypeId().getName().getString()));
	
	SoBuildPucExtStyleAction *action = (SoBuildPucExtStyleAction *)action_;
	SoPucStateEnumerated *state = (SoPucStateEnumerated *)node;

	//get layoutGroup with minimal columnsHeight
	int minIndex = action->getMinIndex(action->columnsHeight, action->numOfCols);
	action->layoutGroupPerCol = (SoWidgetLayoutGroup *)action->mainNodeList->getChild(minIndex);

	action->nodeList = SO_GET_PART(action->layoutGroupPerCol, "elements", SoNodeKitListPart);
	assert( action->nodeList != NULL );

	int num = state->valueLabels.getNum();

	SoBehaviorKit *widgetToAdd;

	if (num<4)
	{
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, num, 1, 0, SMALL_LABEL);
		action->layoutGroupPerCol->sizeOfRows.set1Value(action->numRows[minIndex], 1);
		action->columnsHeight[minIndex]+=1;
	}
	else if (num<7)
	{
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, 2, 1, 5, SMALL_LABEL);
		action->layoutGroupPerCol->sizeOfRows.set1Value(action->numRows[minIndex], 1);
		action->columnsHeight[minIndex]+=1;
	}
	else 
	{
		widgetToAdd = SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, 2, 2, 2, SMALL_LABEL);
		action->layoutGroupPerCol->sizeOfRows.set1Value(action->numRows[minIndex], 2);
		action->columnsHeight[minIndex]+=2;
	}

	widgetToAdd->labelPlacing.setValue("TOP");
	action->nodeList->addChild( widgetToAdd );

	//action->columnsHeight[minIndex] = action->columnsHeight[minIndex] + 2;
	action->numRows[minIndex]++;
}	

//-------------------------------------------------------------------------------------------------------------------


void SoBuildPucExtStyleAction::pucGroupAction(SoAction *action_, SoNode *node_)
{
	DEBUG_CODE(printf("SoBuildPucExtStyleAction::pucGroupAction %x,%s\n",node_,node_->getTypeId().getName().getString()));

	SoBuildPucExtStyleAction *action = (SoBuildPucExtStyleAction *)action_;


	node_->doAction(action);
}

//-------------------------------------------------------------------------------------------------------------------