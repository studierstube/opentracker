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
/** header file for InterSenseSource Node.
  *
  * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/InterSenseSource.h,v 1.1 2001/04/23 14:32:54 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section intersensesource InterSenseSource
 * The InterSenseSource node is a simple EventGenerator that generates events
 * for an InterSense tracker. It only outputs data that is available from 
 * the tracker, for example an InterTrax device will not generate position 
 * data. It is associated with a tracker configuration element from the 
 * @ref intersensemodule via the @c id attribute. The element has the following
 * attributes :
 * @li @c id the id of the tracker as set in the configuration element
 * @li @c station the number of the station of the tracker to output. This 
 *        attribute is ignored for InterTrax devices. Between 0 and 7 
 *        inclusive.
 *
 * An example element looks like this :
 * @verbatim
<InterSenseSource id="IS600" station="1"/>@endverbatim
 */

#ifndef _INTERSENCESOURCE_H
#define _INTERSENCESOURCE_H

#include "../OpenTracker.h"

// #ifdef USE_INTERSENSE

/**
 * This class implements a simple EventGenerator. It is updated by the
 * InterSenseModule.
 * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API InterSenseSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;
    /// the station of the tracker
    unsigned station;


// Methods
public:
    /** simple constructor, sets members to initial values */
    InterSenseSource(unsigned station_) : Node(), station(station_)
    {};
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
};

#endif

// #endif
