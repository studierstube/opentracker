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
/** header file for ARTDataTrackerSource Node.
  *
  * @author Christopher Schmidt
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section artdatatrackersource ARTDataTrackerSource
 * The ARTDataTrackerSource node is a simple EventGenerator that inputs the data 
 * from the ART Data Tracker.
 * @li @c number the body number of the specific source, starting with 0
 *
 * An example element looks like this :
 * @verbatim
<ARTDataTrackerSource number="0"/>@endverbatim
 */

#ifndef _ARTDATATRACKERSOURCE_H
#define _ARTDATATRACKERSOURCE_H

#include "../dllinclude.h"

#include "../OpenTracker.h"

/**
 * This class implements a simple source that inputs the data from the ART Data Tracker
 * and updates any EventObservers.
 * @ingroup input
 * @author Christopher Schmidt
 */
class OPENTRACKER_API ARTDataTrackerSource : public Node
{
// Members
public:
     /// body number
    int number;
    /// the state that is stored
    State state;
    /// a flag to indicate whether it was changed during processing
    int changed;

// Methods
public:
    /** simple constructor, sets members to initial values
     * @param number_ the body number of the specific source */
    ARTDataTrackerSource( int number_ ) : Node(), number( number_ ), changed( 1 )
    {
	};
	  
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    int isEventGenerator()
    {
        return 1;
    };
  
};

#endif
