 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Merge Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/MergeNode.h,v 1.1 2001/01/28 16:48:14 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section merge Merge
 * A Merge node is an EventGenerator node that listens to several other
 * EventGenerator nodes and merges data from these. It has several inputs
 * that are marked with different wrapper tags. It stores an internal state
 * and updates it with parts of the data depending on the type of input. Then
 * it generates an event of its own. The following list shows the possible
 * inputs :
 * 
 * @li @c MergePosition only the position data is taken from events received
 *        from children of this wrapper element
 * @li @c MergeOrientation only the orientation data is taken
 * @li @c MergeButton only the button data is taken
 * @li @c MergeConfidence only the confidence data is taken
 * @li @c MergeTime only the time stamp data is taken
 * @li @c MergeDefault any data that is not set by a child of another
 *        wrapper element is used.
 * 
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <Merge>
    <MergeDefault>
        <One or more of any EventGenerator element type>
    </MergeDefault>
    <MergePosition>
        <One or more of any EventGenerator element type>
    </MergePosition>
    <MergeOrientation>
        <One or more of any EventGenerator element type>
    </MergeOrientation>
    <MergeButton>
        <One or more of any EventGenerator element type>
    </MergeButton>
    <MergeConfidence>
        <One or more of any EventGenerator element type>
    </MergeConfidence>
    <MergeTime>
        <One or more of any EventGenerator element type>
    </MergeTime>
 </Merge>@endverbatim
 */

#ifndef _MERGENODE_H
#define _MERGENODE_H

#include "../OpenTracker.h"

typedef map<EventGenerator *, unsigned> FlagMap;

/**
 * A MergeNode is an EventGenerator node that observes several marked input
 * nodes and merges the data provided by the input nodes. The mark decides
 * which part of the data is used in the merge. It is implemented using
 * wrapper nodes and apropriate DTD definitions.
 * @author Gerhard Reitmayr
 */
class MergeNode
    : public Node
    , public EventGenerator
    , public EventObserver
{

protected:
    /// State variable to put transformed state into
    State localState;
 
    /** definitions for flags defining whats happening for each
     * EventGenerator
     */
    static unsigned DEFAULT = 1, POSITION = 2, ORIENTATION = 4, BUTTON = 8,
               CONFIDENCE = 16, TIME = 32;
    
    /** the flag for nodes with the default wrapper tag, it is
     * the compliment of all other defined wrappers 
     */
    unsigned defaultFlags;
    /** maps from EventGenerator to a set of flags defining which
     * values of the EventGenerator to use 
     */
    FlagMap mergeFlags;        
    
public:
    /** constructor method
     */
    MergeNode();
    /**
     * adds a child to the Node. Sets the child member to the node.
     * If the node implements the EventGenerator interface, it registers
     * as an EventObserver with it.
     * @param node reference to the new child node.
     */
    virtual void addChild( Node& node);

    /** tests for EventGenerator interface being present and returns
     * a pointer to it, if present.
     * @return pointer to interface or NULL */
    EventGenerator * isEventGenerator()
    {
    	return this;
    }

    /**
     * this method is called by the EventGenerator to update it's observers.
     * This class computes a transformed state, stores it in its local variable
     * and notifies its observers in turn, propagating the change.
     */
    virtual void onEventGenerated( State& event,
                                   EventGenerator& generator)
    {
        FlagMap::iterator index = mergeFlags.find( &generator );
        if( index == mergeFlags.end())
            return;
        int flags = (*index)->second();
        if( flags & DEFAULT )
            flags |= defaultFlags;
        if( flags & POSITION )
        {
            localState.postion[0] = event.position[0];
            localState.postion[1] = event.position[1];
            localState.postion[2] = event.position[2];
        }
        if( flags & ORIENTATION )
        {
            localState.orientation[0] = event.orientation[0];
            localState.orientation[1] = event.orientation[1];
            localState.orientation[2] = event.orientation[2];
            localState.orientation[3] = event.orientation[3];
        }
        if( flags & BUTTON )        
            localState.button = event.button;        
        if( flags & CONFIDENCE )        
            localState.confidence = event.confidence;        
        if( flags & TIME )        
            localState.time = event.time;        
        updateObservers( localState );
    }                                  
};

#endif

