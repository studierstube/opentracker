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
/** header file for Callback node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/CallbackNode.h,v 1.3 2001/10/20 17:18:13 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section callbacknode Callback Node
 * The Callback node is a simple EventObserver / EventGenerator that passes
 * events on and calls a registered callback function everytime it 
 * receives a new event with the passed state. 
 * It has the following attributes :
 * @li @c name a unique name to identify it among all Callback nodes.
 *
 * An example element looks like this :
 * @verbatim
<Callback name="cb1">
    <Any EventGenerator element type>
</Callback>@endverbatim
 */

#ifndef _CALLBACKNODE_H
#define _CALLBACKNODE_H

#include "../OpenTracker.h"

typedef void CallbackFunction(const Node &, const State &, void *);

/**
 * This class implements a simple node that stores a function pointer
 * and calls it every time an event it received. Furthermore it passes 
 * the event on to its single child.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API CallbackNode : public Node
{
// Members
public:
    /// name
    std::string name;
    /// callback function
    CallbackFunction * function;
    /// data pointer
    void * data;

// Methods
protected:
/** constructor method,sets commend member
     * @param name_ the name of the Callback node */
    CallbackNode( const std::string & name_ ) :
        Node(), 
        name( name_ ),
        function( NULL ),
        data( NULL )
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
        if( function != NULL )
        {
            (*function)(*this,  event, data );
        }
        updateObservers( event );
    }

    friend class CallbackModule;
};

#endif
