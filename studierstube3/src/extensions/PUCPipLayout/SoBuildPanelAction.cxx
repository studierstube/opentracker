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
/** Implementation of class SoBuildPanelAction
  *
  * @author Valerie Maquil
  *
  * $Id: SoBuildPanelAction.cxx 3426 2004-07-14 12:11:27Z tamer $
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
#include "..\puc\SoPucCommand.h"


#include "..\puc\SoPucActiveIfNode.h"
#include "..\puc\SoPucActiveIfClause.h"
#include "..\puc\SoPucActiveIfOperator.h"
#include "SoPanelGroup.h"
#include "SoPanel.h"

#include "SoBuildPanelAction.h"


#include <iostream>

using namespace std;

#ifdef _DEBUG_PUC
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

//const int SoBuildPanelAction::errorReturnCode = 1024;

SO_ACTION_SOURCE(SoBuildPanelAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoBuildPanelAction::initClass()
{

    SO_ACTION_INIT_CLASS(SoBuildPanelAction, SoAction);

  
    // register static methods to be called for a specific node

	SO_ACTION_ADD_METHOD(SoNode,			callDoAction);
	SO_ACTION_ADD_METHOD(SoNodeKitListPart,	callDoAction);
	SO_ACTION_ADD_METHOD(SoGroup,			callDoAction);

	SO_ACTION_ADD_METHOD(SoPucStateEnumerated,	callDoAction);
	SO_ACTION_ADD_METHOD(SoPucGroup,		pucGroupAction);

	SoBuildPucStyleAction::initClass();
	SoBuildStbStyleAction::initClass();
	SoBuildPucExtStyleAction::initClass();


}



//
// constructor
//

SoBuildPanelAction::SoBuildPanelAction()
{
    SO_ACTION_CONSTRUCTOR(SoBuildPanelAction);

	pipLayout = new SoSeparator();

}


//
// destructor
//

SoBuildPanelAction::~SoBuildPanelAction()
{	
}


void SoBuildPanelAction::setDimensions(float width, float height, float depth)
{
	widthMain = width;
	depthMain = depth;
	heightMain = height;
}

void SoBuildPanelAction::setStyle(int style)
{
	actionStyle = style;
}

void SoBuildPanelAction::setUnits(int units_)
{
	units = units_;
}

void SoBuildPanelAction::setColumns(int col)
{
	columns = col;
}
//
// begin the traversal of the scenegraph
//


void SoBuildPanelAction::beginTraversal(SoNode *node)
{	

	traversalState = NORMAL;
	level = 0;

	traverse(node);	
}

void SoBuildPanelAction::endTraversal(SoNode *node)
{	
	//add panelGroup to the main Separator
	pipLayout->addChild(layoutGroup);
	layoutGroup->depth.setValue(depthMain);
	layoutGroup->height.setValue(heightMain);
	layoutGroup->width.setValue(widthMain);

}

static const std::string endlString() { return "\n"; }
#define ENDL endlString()


void SoBuildPanelAction::callDoAction(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoBuildPanelAction::callDoAction %x,%s\n",node,node->getTypeId().getName().getString()));
	node->doAction(action);
}


//-------------------------------------------------------------------------------------------------------------------


void SoBuildPanelAction::pucGroupAction(SoAction *action_, SoNode *node_)
{
	DEBUG_CODE(printf("SoBuildPanelAction::pucGroupAction %x,%s\n",node_,node_->getTypeId().getName().getString()));
	SoBuildPanelAction *action = (SoBuildPanelAction *)action_;
	SoPucGroup *node = (SoPucGroup *)node_;

	
	if (action->isPanelStructure(node))
	{
		action->level+=1;

		//create new SoPanelGroup
		SoPanelGroup *panelgroup = new SoPanelGroup();
		
		int res = action->level % 2;
		if (res == 1) panelgroup->navigationPlacing.setValue(1);
		else panelgroup->navigationPlacing.setValue(2);

		//remember old sheetsNodeList
		SoNodeKitListPart *oldSheetsNodeList = action->sheetsNodeList;

		if (action->traversalState==PANELGROUP) action->sheetsNodeList->addChild(panelgroup);

		//add TextListBox to the title-part
		SoNodeKitListPart *nodeList = SO_GET_PART(node, "members", SoNodeKitListPart);
		SoPucStateEnumerated *state = (SoPucStateEnumerated *)nodeList->getChild(0);
		int num = state->valueLabels.getNum();

		SoTextListBox *widgetToAdd = (SoTextListBox *)SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(state, num, 1, 0, NO_LABEL);
		if (state->valueLabels.getNum()<4) panelgroup->numOfButtons.setValue(state->valueLabels.getNum());

		assert(panelgroup->setPart("titles", widgetToAdd));

		///get next sheetsNodeList to add the children of this group
		action->traversalState=PANELGROUP;
    	action->sheetsNodeList = SO_GET_PART(panelgroup, "sheets", SoNodeKitListPart);
		assert(action->sheetsNodeList != NULL);

		//perform action on children (SoPucGroups)
		node->doAction(action);

		action->level-=1;

		panelgroup->setUpConnections(TRUE, TRUE);
	
		//come back to old sheetsNodeList
		action->sheetsNodeList = oldSheetsNodeList;
		action->layoutGroup = panelgroup;		
	}
	else if (action->traversalState==PANELGROUP) //case: 2nd hierarchy of panelGroup
	{

		//start action, get widgetLayoutGroup add it to sheets-part
		SoWidgetLayoutGroup *groupToAdd = action->applyLayoutAction(node);
		action->sheetsNodeList->addChild(groupToAdd);
	}

	else { //no structure of a PanelGroup
		
		action->layoutGroup = action->applyLayoutAction(node);
	}

}

//-------------------------------------------------------------------------------------------------------------------#

bool SoBuildPanelAction::isPanelStructure(SoPucGroup *group)
{
	bool result = FALSE;
	SoNodeKitListPart *nodeList = SO_GET_PART(group, "members", SoNodeKitListPart);
	int num = nodeList->getNumChildren();

	if (nodeList->getChild(0)->isOfType(SoPucStateEnumerated::getClassTypeId()))
	{
		result = TRUE;

		for (int i=1; i<num; i++)
		{
			if (!nodeList->getChild(i)->isOfType(SoPucGroup::getClassTypeId())) result = FALSE;
		}
	}
	DEBUG_CODE(printf("SoBuildPanelAction::isPanelStructure %x\n",result));

	return result;
}

//-------------------------------------------------------------------------------------------------------------------#

SoWidgetLayoutGroup *SoBuildPanelAction::applyLayoutAction(SoNode *node)
{
	if (actionStyle==PUC) 
	{
			SoBuildPucStyleAction pucLayoutAction;
			pucLayoutAction.apply(node);

			return pucLayoutAction.getLayoutGroup();
	}

	else if (actionStyle==STB)
	{
			SoBuildStbStyleAction stbLayoutAction;
			stbLayoutAction.setUnits(units);
			stbLayoutAction.apply(node);
			
			return stbLayoutAction.getLayoutGroup();
    }

	else if (actionStyle==PUCEXT)
	{
			SoBuildPucExtStyleAction pucExtLayoutAction;
			pucExtLayoutAction.setCols(columns);
			pucExtLayoutAction.apply(node);
			
			return pucExtLayoutAction.getLayoutGroup();
    }

	else return new SoWidgetLayoutGroup();
}

