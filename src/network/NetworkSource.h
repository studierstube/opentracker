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
/** header file for NetworkSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSource.h,v 1.6 2001/07/16 21:43:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section networksource NetworkSource
 * The NetworkSource node is a simple EventGenerator that inserts events from
 * the network into the tracker tree. The NetworkSource element has the 
 * following attributes :
 * @li @c number the stations number
 * @li @c multicast-address the multicast group to send to
 * @li @c port port to send to
 *  
 * An example element looks like this :
 * @verbatim
<NetworkSource number="1" multicast-address="224.0.0.10" port="12345"/>@endverbatim
 */

#ifndef _NETWORKSOURCE_H
#define _NETWORKSOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that inserts new events from the network
 * into the tracker tree.
 * @author Gerhard Reitmayr
 * @ingroup network
 */
class OPENTRACKER_API NetworkSource : public Node
{
// Members
public:
    /// the new state
    State state;

// Methods
protected:
    /** constructor */
    NetworkSource() : Node()
    {}

public:                
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }  
    
    friend class NetworkSourceModule;
};

#endif
