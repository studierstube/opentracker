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
/** header file for ButtonOp Node.
  *
  * @author Gerhard Reitmayr
  * 
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section buttonopnode ButtonOp
 * The ButtonOp node implements a boolean operation on the button states of 
 * its two input ports. It saves the last button states from each input and
 * outputs either the logical AND or OR result of the two states, whenever
 * a new event from either input is received.
 *
 * The element has the following attributes :
 * @li @c op (OR | AND) whether it should do an OR or AND.
 *
 * An example element looks like this :
 * @verbatim
<ButtonOp op="OR">
    <Arg1>
        <any event generator ...>
    </Arg1>
    <Arg2>
        <any event generator ...>
    </Arg2>
</ButtonOp>@endverbatim
 */

#ifndef _BUTTONOPNODE_H
#define _BUTTONOPNODE_H

#include "../OpenTracker.h"

/**
 * The ButtonOp node implements a boolean operation on two button states.
 * It uses two NodePorts named 'Arg1' and 'Arg2'.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API ButtonOpNode
    : public Node
{
// Members
protected:
    /// last saved state variable
    State result;
	/// last button states
	unsigned short arg1, arg2;
	/// the operation to perform
	enum Op { OR, AND } operation;

// Methods
protected:

    /** constructor method
     */
    ButtonOpNode( const Op & op_  );

public:

    /** tests for EventGenerator interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventGenerator, 0 otherwise */
    virtual int isEventGenerator() ;

    /**
     * this method is called by the EventGenerator to update it's observers.
     */
    virtual void onEventGenerated( State& event, Node& generator);

    friend class CommonNodeFactory;
};

} // namespace ot

#endif
