 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for TestSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/TestSource.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
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

#ifndef _TESTSOURCE_H
#define _TESTSOURCE_H

#include "Node.h"
#include "EventGenerator.h"

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers.
 * @author Gerhard Reitmayr
 */
class TestSource : public Node, public EventGenerator
{
// Members
public:
    /// frequency of updates
    int frequency;
    /// offset of first update relative to main loop start
    int offset;
    /// the state that is posted to the EventObservers
    State state;

// Methods
public:
    /** simple constructor, sets members to initial values
     * @param frequency_ initial value for member frequency 
     * @param offset_ initial value for member offset */
    TestSource( int frequency_, int offset_ ) : 
        Node(),
	EventGenerator(),
	frequency( frequency_ ),
	offset( offset_ )
    {}
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    EventGenerator * isEventGenerator()
    {
        return this;
    }
    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator */
    void TestSource::push()
    {
        state.timeStamp();
        updateObservers( state );
    }
};

#endif
