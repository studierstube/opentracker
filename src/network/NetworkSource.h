 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for NetworkSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSource.h,v 1.2 2001/01/03 14:45:30 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section networksource NetworkSource
 * The NetworkSource node is a simple  EventGenerator that inserts events from
 * the network into the tracker tree. The NetworkSource element has the 
 * following attributes :
 * @li @c number the stations number
 * @li @c multicast-address the multicast group to send to
 * @li @c port port to send to
 *  
 * An example element looks like this :
 * @verbatim
<NetworkSource number="1" multicast-address="224.0.0.10" port="12345"/>@endverbatim
 */

#ifndef _NETWORKSOURCE_H
#define _NETWORKSOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that inserts new events from the network
 * into the tracker tree.
 * @author Gerhard Reitmayr
 */
class NetworkSource : public Node, public EventGenerator
{
// Members
public:
    /// the new state
    State state;

// Methods
public:
    /** constructor */
    NetworkSource() : Node(), EventGenerator()
    {}
                
    /** tests for EventGenerator interface being present. Is overriden to
     * return this always.
     * @return always this */
    EventGenerator * isEventGenerator()
    {
        return this;
    }
    
    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator */
    void push()
    {
        updateObservers( state );
    }
};

#endif
