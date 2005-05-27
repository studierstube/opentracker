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
/**
 * header file for SoHandle3DEventAction
 *
 * @author  Andreas Zajic, Markus Krutz, Gerd Hesina
 *
 * $Id: SoHandle3DEventAction.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SOSoHandle3DEventAction_H__
#define __SOSoHandle3DEventAction_H__

#include <stbapi/stbapi.h>


#include <Inventor/actions/SoSubAction.h>

#include <stbapi/event3d/So3DEvent.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * action class that traverses the scenegraph to send 3DEvents to the nodes. Documented at
 * http://www.studierstube.org/stbapi/SoHandle3DEventAction.html
 * @ingroup event
 */
class STBAPI_API SoHandle3DEventAction : public SoAction {
	SO_ACTION_HEADER(SoHandle3DEventAction);
	
public:
	
	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoHandle3DEventAction();
    /// destructor
	virtual ~SoHandle3DEventAction();

	/// set the event to be sent into the scenegraph
	void setEvent(const So3DEvent *ev);
    
    /// get the event to be sent into the scenegraph	
	const So3DEvent *getEvent() const;
	
	/** set an event as handled
	 * (should be called by the node that handled the event)
	 * this stops the traversal of the scenegraph
     */
	void setHandled();
	void setHandled(const SoFullPath *consumer);
	
	/// returns wether the event was handled during the last traversal or not
	SbBool isHandled();
	
    /**	set, get and restore the previous grabber 
	 * this is to be called only during traversal!
	 *
     * when a node grabs events ALL(!) the events from the station it grabs are sent
	 * to the subgraph rooted by this node
	 *
	 * the previous grabber should be set by using restoreGrabber(), because this
	 * is internally managed as a stack
	 */
	void setGrabber(SoNode *node);
    /// set grabber     
	void setGrabber(SoPath *path);
    /// set grabber (how does this work ?)
	void setGrabber();
	void releaseGrabber();
	SoNode *getGrabber();
	
	/** get the path from the rootnode that is set in the field of the corresponding
	 * TrakDevice object
	 * this is to be called only during traversal!
     */
	const SoPath *getPathFromRoot();
	void resetPathFromRoot();

	const SoNode *getCurrentConsumer();
	const SoNode *getPreviousConsumer();
	bool hasConsumerChanged();

    int getStation();
    int getEventType();


	///TRUE: ignore whichChild field of SoSwitch (just traverse)
	void setTraverseAll(SbBool traverseMode);

    SbBool getTraverseAll(); 

protected:

	virtual void beginTraversal(SoNode *node);

private:

	int station;
	int eventType;

	SoNode *eventGrabber;

	const So3DEvent *event;
	
	SoPath *pathFromRoot;

	SoNode *currentConsumer;
	SoNode *previousConsumer;

	
	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators	
     */
	static void callDoAction(SoAction *, SoNode *);
    /** function that is called for switch nodes 
     */
	static void callSwitchAction(SoAction *, SoNode *);

	/// function that does nothing
	static void mynullAction(SoAction *, SoNode *);

    /** 
     * FALSE if defaultBehaviour
     * TRUE: ignore whichChild field of SoSwitch (just traverse)
     */
    SbBool traverseAll;   
	                      
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
