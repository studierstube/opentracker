 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for the WrapperNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/WrapperNode.h,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _WRAPPERNODE_H
#define _WRAPPERNODE_H

#include "../dllinclude.h"

#include "Node.h"

/**
 * This node implements a wrapper node. Its just there to signal the existence
 * of a wrapper tag. It is used in searching for wrapped children.
 * @note write explanation of wrapper tags here
 * @author Gerhard Reitmayr
 * @ingroup core
*/
class OPENTRACKER_API WrapperNode : public Node
{
protected:
	string tagname;

	virtual void setParent( DOM_Element & parElement );
// Methods
public:
    /**
     * constructor method */
    WrapperNode()
        : Node()
    {};
    /// destructor method
    virtual ~WrapperNode()
    {};
    /** tests whether the given node is a wrapper node.
     * @return always returns 1 */
    virtual int isWrapperNode()
    {
        return 1;
    };

	/**
     * this method notifies the object that a new event was generated.
     * It is called by an EventGenerator.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event, Node& generator)
	{
		updateObservers( event );
	}

	string & getTagName()
	{
		return tagname;
	}
};

#endif
