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
/** header file for TestSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/TestSource.h,v 1.7 2001/10/21 22:30:54 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section testsource TestSource
 * The TestSource node is a simple EventGenerator that fires in fixed intervalls
 * standard events. The events can be customized to have other then the default 
 * values. It has the following elements :
 * @li @c frequency every freq. cycle it fires
 * @li @c offset starting after offset cycles
 * @li @c position position value of the event to fire as 3 floats
 * @li @c orientation orientation value of the event to fire as 4 floats representing a quaternion
 * @li @c button a 16 bit integer value representing the button states
 * @li @c confidence a float value in [0,1] to represent the confidence value
 *
 * An example element looks like this :
 * @verbatim
<TestSource frequency="10" offset="5" />@endverbatim
 */

#ifndef _TESTSOURCE_H
#define _TESTSOURCE_H

#include "../dllinclude.h"

#include "Node.h"

/**
 * This class implements a simple source that is fired by its module in 
 * regular intervals and updates any EventObservers.
 * @ingroup core
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API TestSource : public Node
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
protected:
    /** simple constructor, sets members to initial values
     * @param frequency_ initial value for member frequency 
     * @param offset_ initial value for member offset */
    TestSource( int frequency_, int offset_ ) : 
        Node(),
	    frequency( frequency_ ),
	    offset( offset_ )
    {}
public:            
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    int isEventGenerator()
    {
        return 1;
    }
    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator */
    void push()
    {
        state.timeStamp();
        updateObservers( state );
    }

    friend class TestModule;
};

#endif
