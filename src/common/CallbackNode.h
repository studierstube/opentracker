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
  * $Id$
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

namespace ot {

class CallbackNode;

typedef void CallbackFunction(CallbackNode &, State &, void *);

/**
 * This class implements a simple node that stores a function pointer
 * and calls it every time an event it received. The event passed to the
 * can be changed by the function and the changes will propagate to the parent node.
 * Furthermore it passes the event on to its single child.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API CallbackNode : public Node
{
// Members
public:
    /** name of the CallbackNode for retrieving it from the module. 
     * Note that this is not the name returned by getName(), rather the value
     * set by the attribute name. 
     */
    std::string name;
    /// callback function
    CallbackFunction * function;
    /// data pointer
    void * data;
    /// the state passed to the function and the parent
    State state;

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
     * This method notifies the object that a new event was generated.
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
        if( function != NULL )
        {
            (*function)(*this,  state, data );
        }
        updateObservers( state );
    }

    /** 
     * This method returns the value set by the name attribute of the CallbackNode.
     * This is a different value then the one returned by getName() which is the
     * value set by the attribute DEF.
     * @return reference to the name string.
     */
    const std::string & getCallbackName(void) const
    {
        return CallbackNode::name;
    };

    friend class CallbackModule;
};

} // namespace ot

#endif
