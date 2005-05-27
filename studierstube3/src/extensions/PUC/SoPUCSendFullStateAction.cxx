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
/** Implementation of class SoPucSendFullStateAction
  *
  * @author Thomas Psik
  *
  * $Id: SoPUCSendFullStateAction.cxx 3823 2004-10-05 12:27:08Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#include "PUCBasicLib/PUCDeviceServer.h"

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/misc/SoChildList.h>

#include "SoPucStateBase.h"

#include "SoPucSendFullStateAction.h"

#include <iostream>

using namespace std;

SO_ACTION_SOURCE(SoPucSendFullStateAction);

#if defined(_DEBUG_PUC) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif
//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoPucSendFullStateAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoPucSendFullStateAction, SoAction);

    // enable elements

    // register static methods to be called for a specific node

	SO_ACTION_ADD_METHOD(SoNode,			callDoAction);
	SO_ACTION_ADD_METHOD(SoNodeKitListPart,	callDoAction);
	SO_ACTION_ADD_METHOD(SoGroup,			callDoAction);
	
	SO_ACTION_ADD_METHOD(SoPucStateBase,    stdStateBaseNotification);
    // init EventClass
}

//
// constructor
//

SoPucSendFullStateAction::SoPucSendFullStateAction()
{
    SO_ACTION_CONSTRUCTOR(SoPucSendFullStateAction);
	deviceServer = NULL; 
}

void SoPucSendFullStateAction::setServer(PUCDeviceServer *deviceServer_)
{
	deviceServer = deviceServer_;
}

//
// destructor
//

SoPucSendFullStateAction::~SoPucSendFullStateAction()
{	
}

//
// begin the traversal of the scenegraph
//

void SoPucSendFullStateAction::beginTraversal(SoNode *node)
{
    traverse(node);		

}

void SoPucSendFullStateAction::callDoAction(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoPucSendFullStateAction::callDoAction %x,%s\n",node,node->getTypeId().getName().getString()));
	node->doAction(action);
}


void SoPucSendFullStateAction::stdStateBaseNotification(SoAction *action, SoNode *node)
{
    DEBUG_CODE(printf("SoPucSendFullStateAction::stdStateBaseNotification %x,%s\n",node,node->getTypeId().getName().getString()));
	SoPucStateBase *elm = (SoPucStateBase *)node;
	((SoPucSendFullStateAction *)action)->deviceServer->sendMessageStateChangeNotification(elm->pucName, elm->getStateValue());
//	const char* stName  =  elm->getStateName().c_str();
//	const char* stValue = elm->getStateValue().c_str();
//	if (stName && stValue)
	DEBUG_CODE(cout << "state change notification [" << elm->getStateName()<< "] [" << elm->getStateValue()<< "] \n";)

}
	