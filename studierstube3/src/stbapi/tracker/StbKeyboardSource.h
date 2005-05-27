/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** header file for StbKeyboardSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: StbKeyboardSource.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __STBKEYBOARDSOURCE_H__
#define __STBKEYBOARDSOURCE_H__

#include <OpenTracker.h>

/**
 * This class implements a simple node that generates tracking events
 * based on keyboard input. It is controlled by the StbKeyboardModule class
 * that uses key events from the windowing system to drive tracking stations.
 * The corresponding OpenTracker configuration element has the following attributes :
 * @li @c number a number ( 0 - 9 ) giving the station number.
 *
 * An example element looks like this :
 * @verbatim
<StbKeyboardSource number="2"/>@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup tracking
 */
class STBAPI_API StbKeyboardSource : public Node
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
    /** constructor method.
     * @param number_ number of the station to use */
    StbKeyboardSource( int number_ ) :
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
    
    friend class StbInputModule;
};

#endif
