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
/** header file for FastTrakSource Node.
  *
  * @author Rainer Splechtna
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/FastTrakSource.h,v 1.3 2003/07/18 17:27:58 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section fasttraksource FastTrakSource 
 *
 * The FastTrakSource node is a simple EventGenerator that inserts events generated from
 * the tracker-device data into the tracker tree. The FastTrakSource element has the 
 * following attributes :
 * @li @c number the stations number
 *
 * An example element looks like this :
 * @verbatim
<FastTrakSource number="1"/>@endverbatim
 */

#ifndef _FASTTRAKSOURCE_H
#define _FASTTRAKSOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple EventGenerator. It is updated by the
 * FastTrakModule.
 * @author Rainer Splechtna
 * @ingroup input
 */
class OPENTRACKER_API FastTrakSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;
    /// number of station
    int station;
// Methods
protected:
    /** simple constructor, sets members to initial values */
    FastTrakSource(int station_) : Node(), station(station_)
    {}

public:            
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    friend class FastTrakModule;
};

#endif
