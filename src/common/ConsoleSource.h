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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ConsoleSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section consolesource ConsoleSource
 * The ConsoleSource node is a simple EventGenerator that generates new 
 * tracking events for one station based on keyboard input to the console.
 * Each ConsoleSource node has a station number that can be selected via
 * the keyboard. There can be only one node per station. It is also managed 
 * and driven by the @ref consolemodule.
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
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API ConsoleSource : public Node
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
protected:
    /** constructor method,sets commend member
     * @param comment_ the comment line to use */
    ConsoleSource( int number_ ) :
        Node(),    
        number( number_ ),
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
    
    friend class ConsoleModule;
};

} // namespace ot

#endif
