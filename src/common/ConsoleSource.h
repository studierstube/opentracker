 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ConsoleSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConsoleSource.h,v 1.2 2001/03/05 17:21:42 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section consolesource ConsoleSource
 * The ConsoleSource node is a simple EventGenerator that generates new 
 * tracking events for one station based on keyboard input to the console.
 * Each ConsoleSource node has a station number that can be selected via
 * the keyboard. There can be only one node per station. It is also managed 
 * and driven by the ConsoleModule.
 *
 * It has the following attributes :
 * @li @c number a number ( 0 - 9 ) giving the station number.
 *
 * An example element looks like this :
 * @verbatim
<ConsoleSource number="2"/>@endverbatim
 */

#ifndef _CONSOLESOURCE_H
#define _CONSOLESOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that generates tracking events
 * based on keyboard input.
 * @author Gerhard Reitmayr
 */
class ConsoleSource : public Node, public EventGenerator
{
// Members
public:
    /// station number
    int number;
    /// the state that is stored
    State state;
    /// a flag to indicate whether it was changed during processing
    int changed;

// Methods
public:
    /** constructor method,sets commend member
     * @param comment_ the comment line to use */
    ConsoleSource( int number_ ) :
        Node(), 
        EventGenerator(),
        number( number_ ),
        changed( 0 )
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
