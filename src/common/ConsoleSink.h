 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ConsoleSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConsoleSink.h,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section consolesink ConsoleSink
 * The ConsoleSink node is a simple EventObserver / EventGenerator that passes
 * events on and stores a copy of the last one. It is polled in regular
 * intervalls by the ConsoleModule and its state is printed out to the console.
 * It has the following attributes :
 * @li @c comment a line of text describing the output
 *
 * An example element looks like this :
 * @verbatim
<ConsoleSink comment="my Console node">
    <Any EventGenerator element type>
</ConsoleSink>@endverbatim
 */

#ifndef _CONSOLESINK_H
#define _CONSOLESINK_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that stores a copy of the last
 * event it received and passed on for output to the console. The 
 * associated ConsoleModule polls the nodes regularly and prints
 * out the last state stored.
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API ConsoleSink : public Node
{
// Members
public:
    /// comment line
    string comment;
    /// the state that is stored
    State state;
    /// flag whether state was changed since last display
    int changed;

// Methods
public:
    /** constructor method,sets commend member
     * @param comment_ the comment line to use */
    ConsoleSink( string & comment_ ) :
        Node(), 
        comment( comment_ ),
        changed( 0 )
    {}

    
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
    
    /**
     * this method notifies the object that a new event was generated.
     * It stores a copy of the received event and passes the event on
     * to its observers.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event, Node& generator)
    {
        state = event;
        changed = 1;
        updateObservers( state );
    }
};

#endif
