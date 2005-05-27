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
/** Implementation of class SoReApplyWidgetStyleAction
*
* @author Thomas Psik
*
* $Id: SoReApplyWidgetStyleAction.cxx 3425 2004-07-14 12:09:17Z tamer $
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
#include <stbapi/widgets/SoLabelListBox.h>

#include "SoReApplyWidgetStyleAction.h"
#include "SoWidgetStyleApply.h"
#include <iostream>

using namespace std;

//#define _DEBUG_STYLE_ACTION

#if defined(_DEBUG_STYLE_ACTION) && defined(_DEBUG)

#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif

SO_ACTION_SOURCE(SoReApplyWidgetStyleAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoReApplyWidgetStyleAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoReApplyWidgetStyleAction, SoAction);

	// enable elements
	//SO_ENABLE(SoReApplyWidgetStyleAction, SoModelMatrixElement);
    // register static methods to be called for a specific node
	SO_ACTION_ADD_METHOD(SoLabelListBox,		labelListBoxAction);

	// set createInstance value in Type (used to instatiate this class from a different dll e.g. stbapi.dll) 
	(void)SoType::overrideType(SoReApplyWidgetStyleAction::getClassTypeId(),
                               SoReApplyWidgetStyleAction::createInstance);
	// init EventClass
}

void * SoReApplyWidgetStyleAction::createInstance  ( )
{
	return new SoReApplyWidgetStyleAction();
}

//
// constructor
//

SoReApplyWidgetStyleAction::SoReApplyWidgetStyleAction()
{
    SO_ACTION_CONSTRUCTOR(SoReApplyWidgetStyleAction);
}

//
// destructor
//

SoReApplyWidgetStyleAction::~SoReApplyWidgetStyleAction()
{	
}

//
// begin the traversal of the scenegraph
//

void SoReApplyWidgetStyleAction::beginTraversal(SoNode *node)
{
   traverse(node);		
}

//----------------------------------------------------------------------------

void SoReApplyWidgetStyleAction::labelListBoxAction(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoReApplyWidgetStyleAction::labelListBoxAction %x,%s\n",node,node->getTypeId().getName().getString()));
	SoReApplyWidgetStyleAction *action = (SoReApplyWidgetStyleAction*) action_;
	SoLabelListBox *labelList = (SoLabelListBox  *)node;
	
	assert(labelList->buttonStyles != NULL);
	assert(labelList->currentStyleButton != NULL);

	SoWidgetStyleApply::applyStyleToWidget((SoWidgetStyleGroup*)(labelList->buttonStyles), labelList->currentStyleButton, labelList->currentStyleIdx);
}

//----------------------------------------------------------------------------
