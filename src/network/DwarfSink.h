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
/** header file for DwarfSink Node.
  *
  * @author Gerhard Reitmayr, Christian Sandor, Martin Bauer
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section dwarfsink DwarfSink
 
 * An example element looks like this :
 * @verbatim
<DwarfSink frequency="10" offset="5" />@endverbatim
 */

#ifndef _DWARFSINK_H
#define _DWARFSINK_H

#include "../OpenTracker.h"

#ifdef USE_DWARF

/**
 * This class implements a simple source that is fired by its module in 
 * regular intervals and updates any EventObservers.
 * @ingroup network
 * @author Gerhard Reitmayr, Christian Sandor, Martin Bauer
 */
class OPENTRACKER_API DwarfSink : public Node
{
// Members
public:
    /// the original state 
    State state;
    /// new state arrived ?
    bool changed;
    /// name of associated dwarf ability
    std::string name;

// Methods
protected:
    /** simple constructor, sets members to initial values
     * @param frequency_ initial value for member frequency 
     * @param offset_ initial value for member offset */
    DwarfSink( const std::string & name_) : 
        Node(),
        changed( false ),
        name( name_ )
    {}

public:            
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    int isEventGenerator()
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
        changed = true;        
        updateObservers( event );
    }

    friend class DwarfModule;
};

#endif // USE_DWARF

#endif
