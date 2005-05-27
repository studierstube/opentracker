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
/** Implementation of class SoHandle3DEventAction
  *
  * @author Andreas Zajic, Markus Krutz, Gerd Hesina, Jan Prikryl, Zolt Marx
  *
  * $Id: SoHandle3DEventAction.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/misc/SoChildList.h>

#ifdef USE_TGS
#  include <Inventor/VRMLnodes/SoVRMLGroup.h>
#  include <Inventor/VRMLnodes/SoVRMLTransform.h>
#  include <Inventor/VRMLnodes/SoVRMLPROTOInstance.h>
#endif

#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/Base3D.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>

//#define _DEBUG_HANDLE3D_ACTION

#if defined( _DEBUG_HANDLE3D_ACTION) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

void printPath(const SoPath *p, const char *text = NULL)
{
	if (text != NULL) {
        printf("%s\n",text);
	}
    int i;
    int l=p->getLength();
    for (i=0;i<l;++i) {
        printf("%d\n",(int)p->getNode(i));
    }
}


SO_ACTION_SOURCE(SoHandle3DEventAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoHandle3DEventAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoHandle3DEventAction, SoAction);

    // enable elements

    SO_ENABLE(SoHandle3DEventAction, SoModelMatrixElement);
    SO_ENABLE(SoHandle3DEventAction, SoSwitchElement);

    // register static methods to be called for a specific node

    SO_ACTION_ADD_METHOD(SoNode,      mynullAction);
    SO_ACTION_ADD_METHOD(SoGroup,     callDoAction);
	SO_ACTION_ADD_METHOD(SoSwitch,    callSwitchAction);
    // also call doAction for SoNodeKitListParts
    SO_ACTION_ADD_METHOD(SoNodeKitListPart,     callDoAction);
    SO_ACTION_ADD_METHOD(SoFile,     callDoAction);
    SO_ACTION_ADD_METHOD(SoBaseKit,   callDoAction);

    SO_ACTION_ADD_METHOD(SoFile, callDoAction);    
    SO_ACTION_ADD_METHOD(SoWWWInline, callDoAction);    

#ifdef USE_TGS
    SO_ACTION_ADD_METHOD(SoVRMLGroup, callDoAction);
    SO_ACTION_ADD_METHOD(SoVRMLTransform, callDoAction);
    SO_ACTION_ADD_METHOD(SoVRMLPROTOInstance, callDoAction);
#endif

    // init EventClass
    So3DEvent::initClass();
}

//
// constructor
//

SoHandle3DEventAction::SoHandle3DEventAction()
:
traverseAll(FALSE)
{
    SO_ACTION_CONSTRUCTOR(SoHandle3DEventAction);

    pathFromRoot     = new SoPath();

    eventGrabber     = NULL;	
	currentConsumer  = NULL;
	previousConsumer = NULL;
}

//
// destructor
//

SoHandle3DEventAction::~SoHandle3DEventAction()
{	
}

//
// begin the traversal of the scenegraph
//

void SoHandle3DEventAction::beginTraversal(SoNode *node)
{
    //printf("begintraversal %x,%s\n",node,node->getTypeId().getName().getString());
    traverse(node);		

	if (!isHandled())
	{
		// move current Consumer to previous Consumer
        previousConsumer = currentConsumer;
		currentConsumer = NULL;
	}
}

//
// set the event
//

void SoHandle3DEventAction::setEvent(const So3DEvent *e)
{
    event = e;

    station   = event->getStation();
    eventType = event->getType();
}

//
// get the event
//

const So3DEvent *SoHandle3DEventAction::getEvent() const
{
    //printf("get event requested\n");
    return event;
}

//
// set an event as handled
// (should be called by the node that handled the event)
// this stops the traversal of the scenegraph
//

void SoHandle3DEventAction::setHandled()
{

    setTerminated(TRUE);

	if (event->getType() != So3DEvent::ET_EXIT_EVENT)
	{

		previousConsumer = currentConsumer;

		if (getCurPath()->getHead())
		{
			currentConsumer = (SoNode *)((SoFullPath*)getCurPath())->getTail();
		}
		else
            currentConsumer = NULL;
		DEBUG_CODE(
        if (currentConsumer)
            printNode(currentConsumer, "SETHANDLED new");
        if (previousConsumer)
            printNode(previousConsumer, "SETHANDLED old");
		);
	}
}

void SoHandle3DEventAction::setHandled(const SoFullPath *consumer)
{

	printPath(consumer, "SETHANDLED");

	previousConsumer = currentConsumer;

	
	if ( consumer->getHead() )
	{
		//printNode((SoNode *)((SoFullPath*)consumer)->getTail(), "SETHANDLED");
		currentConsumer = (SoNode *)((SoFullPath*)consumer)->getTail();
	}
	else
		currentConsumer = NULL;

    setTerminated(TRUE);
	
}

//
// returns wether the event was handled during the last traversal or not
//

SbBool SoHandle3DEventAction::isHandled()
{
    return hasTerminated();
}

//
// set up a node to grab events
//

void SoHandle3DEventAction::setGrabber()
{
	eventGrabber = getCurPath()->getTail();
    pathFromRoot->setHead(getCurPath()->getHead());
    pathFromRoot->append(getCurPath());
}

void SoHandle3DEventAction::setGrabber(SoNode *node)
{
    eventGrabber = node;
    pathFromRoot->setHead(getCurPath()->getHead());
    pathFromRoot->append(getCurPath());

}

void SoHandle3DEventAction::setGrabber(SoPath *path)
{
	if (path)
	{
		eventGrabber = ((SoFullPath*) path)->getTail();
		pathFromRoot = path->copy();
	}
	else
	{
		eventGrabber = NULL;
		pathFromRoot = NULL;
	}
}

//
// release the grabber
//

void SoHandle3DEventAction::releaseGrabber()
{
    eventGrabber = NULL;
    resetPathFromRoot();

    //setHandled();
}

//
// returns the current grabber (NULL=no grabber)
//

SoNode * SoHandle3DEventAction::getGrabber()
{
    return eventGrabber;
}

//
// returns the path from the scenegraph-root (set in the corresponding TrakDevice)
//

const SoPath *SoHandle3DEventAction::getPathFromRoot()
{
    if (eventGrabber)
    {
        return pathFromRoot;
    }
    else
    {
        return getCurPath();
    }
}

const SoNode *SoHandle3DEventAction::getCurrentConsumer()
{
	return currentConsumer;
}

const SoNode *SoHandle3DEventAction::getPreviousConsumer()
{
	return previousConsumer;
}

bool SoHandle3DEventAction::hasConsumerChanged()
{

    return !(previousConsumer == currentConsumer);
	//return  !( *(currentConsumer) == *(previousConsumer) );
}

//
// clear Path from Root
//
void SoHandle3DEventAction::resetPathFromRoot()
{
    if (pathFromRoot) pathFromRoot->truncate(0);
}

//
//
//
int SoHandle3DEventAction::getStation()
{
    return station;
}

int SoHandle3DEventAction::getEventType()
{
    return eventType;
}

//
// calls the default action behaviour
//

void SoHandle3DEventAction::callDoAction(SoAction *action, SoNode *node)
{
    node->doAction(action);
}

void SoHandle3DEventAction::callSwitchAction(SoAction *action, SoNode *node)
{
    if(((SoHandle3DEventAction*)action)->getTraverseAll())
	{
	    int		numIndices;
        const int	*indices;

        if (action->getPathCode(numIndices, indices) == SoAction::IN_PATH)
	        ((SoGroup*)node)->getChildren()->traverse(action, 0, indices[numIndices - 1]);
        else
	        ((SoGroup*)node)->getChildren()->traverse(action);
	}
    else
		callDoAction(action, node);
}

//
// null action (implemented here to get rid of the debug library messages)
//

void SoHandle3DEventAction::mynullAction(SoAction *, SoNode *)
{
}

void
SoHandle3DEventAction::setTraverseAll(SbBool traversalMode)
{
	traverseAll=traversalMode;
}

SbBool
SoHandle3DEventAction::getTraverseAll()
{
	return traverseAll;
}
/* ===========================================================================
    End of SoHandle3DEventAction.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
