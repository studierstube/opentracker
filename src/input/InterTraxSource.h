 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for InterTraxSource Node.
  *
  * @author Ivan Viola, Matej 
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/Attic/InterTraxSource.h,v 1.3 2001/01/28 16:49:11 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section testsource TestSource
 * The TestSource node is a simple EventGenerator that fires in fixed intervalls
 * standard events. it can be configured with the following attributes. It has the 
 * following elements :
 * @li @c frequency every freq. cycle it fires
 * @li @c offset starting after offset cycles
 *
 * An example element looks like this :
 * @verbatim
<TestSource frequency="10" offset="5" />@endverbatim
 */
#ifdef USE_INTERTRAX

#ifndef _INTERTRAXSOURCE_H
#define _INTERTRAXSOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers.
 * @author Gerhard Reitmayr
 */
class InterTraxSource : public Node, public EventGenerator
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;

// Methods
public:
    /** simple constructor, sets members to initial values */
    InterTraxSource() : 
        Node(),
	EventGenerator()
    {};
        
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

#endif
