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
/** Implementation of SoAppNode
  *
  * @author  Rainer Splechtna
  *
  * $Id: SoAppNode.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <stbapi/workspace/StbAppManager.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/ContextManager.h>
#include <stbapi/resource/SoUserKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoAppNode.h"

int SoAppNode::ignoreGLRenderAction = TRUE ;

SO_NODE_SOURCE(SoAppNode);

void
SoAppNode::initClass()
{
   SO_NODE_INIT_CLASS(SoAppNode, SoNode, "Node");
}

SoAppNode::SoAppNode():
appRunning(FALSE)
{
   SO_NODE_CONSTRUCTOR(SoAppNode);
   SO_NODE_ADD_FIELD(appName,  (""));
   SO_NODE_ADD_FIELD(workingDir,  (""));
   SO_NODE_ADD_FIELD(isContext, (TRUE));
}

SoAppNode::~SoAppNode()
{
}

void 
SoAppNode::setIgnoreGLRenderAction(SbBool truefalse)
{
    ignoreGLRenderAction = truefalse;
}

void
SoAppNode::GLRender(SoGLRenderAction *action)
{
	if (!appRunning)
	{
        if(isContext.getValue())
		{
		    SoContextKit *nc = ContextManager::getInstance()->
                newContext(appName.getValue().getString(),
				           workingDir.getValue().getString() ,
                           SoUserKit::getUserKitSequentially(0)->getUserID(),
						   TRUE,
						   "");
			appID = nc->getCommunicator()->appID;
		}
        else // old-style application
		{
            SbString f = workingDir.getValue();
            f += appName.getValue();      
            appID = StbAppManager::getInstance()->loadApplication(
				                     (char*)f.getString(),
				                     SoUserKit::getUserKitSequentially(0)->getUserID(),
                                     -1,
									 NULL);
		} 
		appRunning = TRUE;
        // removed rerootWindow 12.09.2001
        // reroot windows of this application to this node
		//StbWinManager::getInstance()->rerootWindow(this, appID);
	}
    else
        SoGroup::GLRender(action);
}
