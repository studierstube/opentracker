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
/** header file for ToolSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _TOOLSOURCE_H
#define _TOOLSOURCE_H

#include <OpenTracker.h>

/**
 * This class implements a simple node that generates tracking events
 * based on keyboard input.
 * @author Gerhard Reitmayr
 * @ingroup tool
 */
class ToolSource : public Node
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
    ToolSource( int number_ ) :
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
    
    friend class ToolIOModule;
};

#endif
