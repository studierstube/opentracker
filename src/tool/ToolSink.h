 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ToolSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _TOOLSINK_H
#define _TOOLSINK_H

#include <OpenTracker.h>

/**
 * This class implements a simple node that stores a copy of the last
 * event it received and passed on for output to the console. The 
 * associated ConsoleModule polls the nodes regularly and prints
 * out the last state stored.
 * @author Gerhard Reitmayr
 * @ingroup tool
 */
class ToolSink : public Node
{
// Members
public:
    /// comment line
    std::string comment;
    /// the state that is stored
    State state;
    /// flag whether state was changed since last display
    int changed;

// Methods
protected:
    /** constructor method,sets commend member
     * @param comment_ the comment line to use */
    ToolSink( const std::string & comment_ ) :
        Node(), 
        comment( comment_ ),
        changed( 0 )
    {}

public:
    
    
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

    friend class ToolIOModule;
};

#endif
