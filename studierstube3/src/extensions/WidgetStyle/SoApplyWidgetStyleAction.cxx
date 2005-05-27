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
/** Implementation of class SoApplyWidgetStyleAction
*
* @author Thomas Psik
*
* $Id: SoApplyWidgetStyleAction.cxx 3425 2004-07-14 12:09:17Z tamer $
* @file                                                                   */
/* ======================================================================= */
#include <assert.h>
#include <Inventor/nodes/SoSwitch.h>

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoSwitchElement.h>

#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/interaction/SoLayoutKit.h>

#include "SoApplyWidgetStyleAction.h"
#include "SoWidgetStyleGroup.h"
#include <iostream>

using namespace std;

//#define _DEBUG_STYLE_ACTION

#if defined(_DEBUG_STYLE_ACTION) && defined(_DEBUG)

#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif

SO_ACTION_SOURCE(SoApplyWidgetStyleAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoApplyWidgetStyleAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoApplyWidgetStyleAction, SoAction);
	
    // enable elements
	
	//  SO_ENABLE(SoApplyWidgetStyleAction, SoModelMatrixElement);
	
    SO_ENABLE(SoApplyWidgetStyleAction, SoModelMatrixElement);
    SO_ENABLE(SoApplyWidgetStyleAction, SoSwitchElement);
	
    // register static methods to be called for a specific node
	
	SO_ACTION_ADD_METHOD(SoNode,			    endAction);
	SO_ACTION_ADD_METHOD(SoNodeKitListPart,	    endAction);
	SO_ACTION_ADD_METHOD(SoFile,				callDoFileAction);
	SO_ACTION_ADD_METHOD(SoGroup,				callDoAction);
	SO_ACTION_ADD_METHOD(SoSeparator,			group);
	SO_ACTION_ADD_METHOD(SoSwitch,			    switchAction);
	
    SO_ACTION_ADD_METHOD(SoLayoutKit,			layoutKit);
	SO_ACTION_ADD_METHOD(SoBehaviorKit,		     widget);
	SO_ACTION_ADD_METHOD(SoWidgetStyleGroup,	style);
	// init EventClass
}

//
// constructor
//

SoApplyWidgetStyleAction::SoApplyWidgetStyleAction()
{
    SO_ACTION_CONSTRUCTOR(SoApplyWidgetStyleAction);
	activeStyle = NULL;
	retrieveStyle = FALSE;
}

//
// destructor
//

SoApplyWidgetStyleAction::~SoApplyWidgetStyleAction()
{	
}

//
// begin the traversal of the scenegraph
//

void SoApplyWidgetStyleAction::beginTraversal(SoNode *node)
{
   inFile = FALSE;
   traverse(node);		
}


static const std::string endlString() { return "\n"; }
#define ENDL endlString()

void SoApplyWidgetStyleAction::callDoAction(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoApplyWidgetStyleAction::callDoAction %x,%s\n",node,node->getTypeId().getName().getString()));
	node->doAction(action);
}

void SoApplyWidgetStyleAction::callDoFileAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoApplyWidgetStyleAction::callDoFileAction %x,%s\n",node,node->getTypeId().getName().getString()));
	SoApplyWidgetStyleAction *action = (SoApplyWidgetStyleAction*) action_;
	action->inFile = TRUE;
	node->doAction(action);
	action->inFile = FALSE;
}

void SoApplyWidgetStyleAction::layoutKit(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoApplyWidgetStyleAction::layoutKit %x,%s\n",node,node->getTypeId().getName().getString()));
	SoApplyWidgetStyleAction *action = (SoApplyWidgetStyleAction*) action_;
	SoLayoutKit *layoutKit = (SoLayoutKit *)node;
	layoutKit->doStyleAction(action);
}

//----------------------------------------------------------------------------

void SoApplyWidgetStyleAction::group(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoApplyWidgetStyleAction::group %x,%s\n",node,node->getTypeId().getName().getString()));
	SoApplyWidgetStyleAction *action = (SoApplyWidgetStyleAction*) action_;
	// store style
	if (!action->inFile)
	{
		SoWidgetStyleGroup *store = action->activeStyle;
		SoWidgetStyleGroup *use = new SoWidgetStyleGroup(store);
		action->activeStyle = use;
		node->doAction(action);
		// re-store style
		delete use;
		action->activeStyle = store;
	}
	else
	{
	    DEBUG_CODE(printf("  SoApplyWidgetStyleAction::group %x, %s -> in File ignored\n",node,node->getTypeId().getName().getString()));
		node->doAction(action);
	}
}

//----------------------------------------------------------------------------

void SoApplyWidgetStyleAction::endAction(SoAction *action_, SoNode *node)
{
  DEBUG_CODE(printf("SoApplyWidgetStyleAction::endAction %x,%s\n",node,node->getTypeId().getName().getString()));
	// dont go further down
}

//----------------------------------------------------------------------------

void SoApplyWidgetStyleAction::switchAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoApplyWidgetStyleAction::switchAction %x,%s\n",node,node->getTypeId().getName().getString()));
	SoApplyWidgetStyleAction *action = (SoApplyWidgetStyleAction*) action_;
	int		numIndices;
	const int	*indices;
	if (action->retrieveStyle)
	{
		callDoAction(action, node);
	// does not work ??
	//	SoSwitch *sw = (SoSwitch *)node;
	//	printf("whichChild %d / %d\n",sw->whichChild.getValue(), sw->getNumChildren());
	//	if (sw->whichChild.getValue() > -1)
	//		sw->getChild(sw->whichChild.getValue())->doAction(action);
	}
	else 
	{
		if (action->getPathCode(numIndices, indices) == SoAction::IN_PATH)
		{
			((SoGroup*)node)->getChildren()->traverse(action, 0, indices[numIndices - 1]);
		}
		else
		{
			((SoGroup*)node)->getChildren()->traverse(action);
		}
	}
}

//----------------------------------------------------------------------------

void SoApplyWidgetStyleAction::widget(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoApplyWidgetStyleAction::widget %x,%s\n",node,node->getTypeId().getName().getString()));
	//	node->doAction(action_);
	assert(action_ != NULL);

	SoApplyWidgetStyleAction *action = (SoApplyWidgetStyleAction*) action_;
	if (action->activeStyle == NULL)
	{
		DEBUG_CODE(printf("  SoApplyWidgetStyleAction::widget nothing to do (WidgetStyle == NULL)\n"));
		return;
	}

	SoBehaviorKit *widget = (SoBehaviorKit *)node;
	if (widget->applyWidgetStyle.getValue() == FALSE)
	{
		DEBUG_CODE(printf("  SoApplyWidgetStyleAction::widget nothing to do (applyWidgetStyle == FALSE)\n"));
		return;
	}

	action->activeStyle->applyStyle(widget);
}

//----------------------------------------------------------------------------

void SoApplyWidgetStyleAction::style(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoApplyWidgetStyleAction::style %x,%s\n",node,node->getTypeId().getName().getString()));
	assert(action_ != NULL);

	SoApplyWidgetStyleAction *action = (SoApplyWidgetStyleAction*) action_;
	SoWidgetStyleGroup *style = (SoWidgetStyleGroup*)node;
	
	// insert style into group
	if (action->activeStyle != NULL)
		action->activeStyle->setStyleInGroup(style);
	else
		action->activeStyle = style;
}


//----------------------------------------------------------------------------

