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
/** c++ header for Base3D class
  *
  * @author   Andreas Zajic, Markus Krutz, Gerd Hesina
  *
  * $Id: Base3D.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOBASE3D_H__
#define __SOBASE3D_H__

#include <stbapi/stbapi.h>
#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoNode.h>

#include <stbapi/event3d/SoHandle3DEventAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


#define BASE3D_CONSTRUCTOR	\
	SO_NODE_ADD_FIELD(eventMask, (BM_ALL_EVENTS));				\
	SO_NODE_ADD_FIELD(stations, (-1));			\
    SO_NODE_ADD_FIELD(blocked, (TRUE));             \
	SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_NO_EVENT);			\
	SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_MOVE_EVENT);			\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_BUTTON0_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_BUTTON1_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_BUTTON2_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_BUTTON3_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_BUTTON4_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_BUTTON5_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_BUTTON6_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_BUTTON7_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_MASTERMODE_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_SLAVEMODE_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_NORMALMODE_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_EXIT_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_ENTER_EVENT);		\
    SO_NODE_DEFINE_ENUM_VALUE(EventTypeMask, BM_ALL_EVENTS);			\
    SO_NODE_SET_SF_ENUM_TYPE(eventMask, EventTypeMask);					
	

#define BASE3D_INIT_CLASS(nodeClass) \
	SoHandle3DEventAction::addMethod(nodeClass::getClassTypeId(), nodeClass::callHandle3DEvent);

#define BASE3D_HEADER \
    static void callHandle3DEvent(SoAction*,SoNode*)

#define BASE3D_SOURCE(nodeClass) \
void nodeClass::callHandle3DEvent(SoAction *action, SoNode *node) { \
/*	printf("callHandle3DEvent %s\n",node->getTypeId().getName().getString());*/ \
	if (node->isOfType(nodeClass::getClassTypeId())) {				\
		nodeClass *mynode=(nodeClass*)node;							\
		SoHandle3DEventAction *myaction=(SoHandle3DEventAction*)action;	\
/*		So3DEvent *myevent = (So3DEvent*)myaction->getEvent(); */\
		int eventType=myaction->getEventType(); \
        if(eventType == So3DEvent::ET_MASTERMODE_EVENT) { \
		    mynode->setBase3DMode(Base3D::BASE3D_MASTERMODE); \
			mynode->doAction(action);			\
			return;                                                              \
		}                                                                        \
        else if(eventType == So3DEvent::ET_SLAVEMODE_EVENT) {                    \
		    mynode->setBase3DMode(Base3D::BASE3D_SLAVEMODE); \
			mynode->doAction(action);			\
			return;                                                              \
		}                                                                        \
        else if(eventType == So3DEvent::ET_NORMALMODE_EVENT) {                    \
		    mynode->setBase3DMode(Base3D::BASE3D_NORMALMODE); \
			mynode->doAction(action);			\
			return;                                                              \
		}                                                                        \
/*		else if(eventType == So3DEvent::ET_EXIT_EVENT) {						*/\
/*				if (myevent->getDestination()->getTail() != mynode) return;		*/\
/*		}																		*/\
        if((mynode->blocked.getValue() == TRUE && mynode->stations.find(myaction->getStation()) >= 0 ) || \
           (mynode->blocked.getValue() == FALSE && mynode->stations.find(myaction->getStation()) == -1 )) { \
			return;															\
		}																	\
		if (!((mynode->eventMask.getValue())&(1 << eventType))) {		\
			return;															\
		}																	\
		if (myaction->getGrabber()==node) {			\
			/*mynode->doAction(action); no Subgraphtraverse*/				\
			/*if (myaction->isHandled()==FALSE) {	*/	\
				mynode->handle3DEvent(myaction);		\
			/*}	*/															\
		} else if (mynode->isInterested(myaction)==TRUE) {\
			mynode->doAction(action);										\
			if (myaction->isHandled()==FALSE) {		\
				mynode->handle3DEvent(myaction);		\
			}																\
		}																	\
	}	\
}


/**
 * Base3D: base class for all 3D aware nodes. Documented at 
 * http://www.studierstube.org/stbapi/Base3D.html
 * http://www.studierstube.org/stbapi/3daware.html.
 * The abstract Base3D class is used to add 3D event handling capabilities to scenegraph nodes. 
 * These nodes are able to implement their own 3D event handling behaviour.
 * Note: The Base3D class is not an Open Inventor class due to Inventor design reasons. 
 * It just adds functionality to nodes.
 * @ingroup event
 */
class STBAPI_API Base3D 
{
public:
    /// Base3D modi (Base3D defaults to BASE3D_NORMALMODE)
    enum ModeEnums
    {
        BASE3D_SLAVEMODE  = 0,
        BASE3D_MASTERMODE = 1,
        BASE3D_NORMALMODE = 2
    };

	enum EventTypeMask {
		BM_NO_EVENT			=0x0000,
		BM_MOVE_EVENT		=0x0001,
		BM_BUTTON0_EVENT	=0x0002,
		BM_BUTTON1_EVENT	=0x0004,
		BM_BUTTON2_EVENT	=0x0008,
		BM_BUTTON3_EVENT	=0x0010,
		BM_BUTTON4_EVENT	=0x0020,
		BM_BUTTON5_EVENT	=0x0040,
		BM_BUTTON6_EVENT	=0x0080,
		BM_BUTTON7_EVENT	=0x0100,
		BM_MASTERMODE_EVENT	=0x0200,
		BM_SLAVEMODE_EVENT  =0x0400,
		BM_NORMALMODE_EVENT =0x0800,
		BM_EXIT_EVENT		=0x1000,
		BM_ENTER_EVENT      =0x2000,
		BM_ALL_EVENTS		=0x3FFF
	};

    /// Defines which types of 3D events are handled by the node.
	SoSFBitMask eventMask;
    /** defines together with blocked which stations events are handled by the node.
     * stations defines a set of stations by listing their numbers. */
    SoMFInt32 stations;
    /** defines together with stations which stations events are handled by the node.
     * If blocked == @c TRUE then the stations listed in stations are blocked, all 
     * others are handled. If blocked == @c FALSE the stations listed are handled, all
     * others are blocked. */
    SoSFBool blocked;

	/** This function is called by the SoHandle3DEventAction during scenegraph
     * traversal for every 3D event aware node. It states the interest of a 
     * node in the current event. The default evalution of isInterested() 
     * performs a bounding box check wether the event occured inside the node 
     * geometry or not.
     *
     * @param h3a pointer to the action evaluating the node
     * @return TRUE handle3dEvent() is called. FALSE SoHandle3DEventAction 
     * continues its traversal of the scenegraph without calling the 
     * handle3DEvent() function of the node.
     */
	virtual SbBool isInterested(
        SoHandle3DEventAction* h3a
        );

	/** This function is called when a 3D aware node is interested in handling
     * a particular event (i.e. isInterested() returns TRUE). It contains the 
     * event handling algorithms of the node. The event handling behaviour of 
     * a node has to be implemented by overwriting this function.
     * @param h3a pointer to the action evaluating the node.
     */
	virtual void handle3DEvent(
        SoHandle3DEventAction* h3a
        );

    void setBase3DMode(int mode) { base3DMode=mode; }
    int getBase3DMode() { return base3DMode; } 

protected:
    int base3DMode;
	static void callHandle3DEvent(
        SoAction *action,
        SoNode *node
        );
	Base3D(
        );
	virtual ~Base3D(
        );

}; //Base3D


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif


#endif//__SOBASE3D_H__
