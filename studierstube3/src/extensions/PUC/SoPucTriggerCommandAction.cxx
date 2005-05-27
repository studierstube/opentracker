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
/** Implementation of class SoPucTriggerCommandAction
  *
  * @author Thomas Psik
  *
  * $Id: SoPucTriggerCommandAction.cxx 3823 2004-10-05 12:27:08Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#include "PUCBasicLib/PUCDeviceServer.h"

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/misc/SoChildList.h>

#include "SoPucCommand.h"

#include "SoPucTriggerCommandAction.h"

#include <iostream>

using namespace std;

SO_ACTION_SOURCE(SoPucTriggerCommandAction);

#if defined(_DEBUG_PUC) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif
//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoPucTriggerCommandAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoPucTriggerCommandAction, SoAction);

    // enable elements

    // register static methods to be called for a specific node

	SO_ACTION_ADD_METHOD(SoNode,			callDoAction);
	SO_ACTION_ADD_METHOD(SoNodeKitListPart,	callDoAction);
	SO_ACTION_ADD_METHOD(SoGroup,			callDoAction);
	
	SO_ACTION_ADD_METHOD(SoPucCommand,    stdCommand);
    // init EventClass
}

//
// constructor
//

SoPucTriggerCommandAction::SoPucTriggerCommandAction()
{
    SO_ACTION_CONSTRUCTOR(SoPucTriggerCommandAction);
	deviceServer = NULL; 
}

void SoPucTriggerCommandAction::setServer(PUCDeviceServer *deviceServer_)
{
	deviceServer = deviceServer_;
}

void SoPucTriggerCommandAction::setData(std::string commandName_)
{
	commandName = commandName_;
}

//
// destructor
//

SoPucTriggerCommandAction::~SoPucTriggerCommandAction()
{	
}

//
// begin the traversal of the scenegraph
//

void SoPucTriggerCommandAction::beginTraversal(SoNode *node)
{
    traverse(node);		

}

void SoPucTriggerCommandAction::callDoAction(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoPucTriggerCommandAction::callDoAction %x,%s\n",node,node->getTypeId().getName().getString()));
	node->doAction(action);
}


void SoPucTriggerCommandAction::stdCommand(SoAction *action_, SoNode *node)
{
    DEBUG_CODE(printf("SoPucTriggerCommandAction::stdCommand %x,%s\n",node,node->getTypeId().getName().getString()));
	SoPucCommand *elm = (SoPucCommand *)node;
	SoPucTriggerCommandAction *action = (SoPucTriggerCommandAction *)action_;


	int idx = action->commandName.find_last_of('.');
	string lastName = action->commandName.substr(idx+1);
// fire trigger 

	if (lastName == elm->getCommandName())
		elm->trigger.setValue();
}
	