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
/** header file for NetworkSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSink.h,v 1.5 2001/03/27 06:08:51 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section networksink NetworkSink
 * The NetworkSink node is a simple EventObserver / EventGenerator that passes
 * events on and stores a copy of the last one. The NetworkSinkModule checks on
 * each pass, whether a NetworkSink node stored a new event and sends that data
 * as the specified station to the network. The NetworkSink element has the 
 * following attributes :
 * @li @c name the stations name
 * @li @c number the stations number
 * @li @c multicast-address the multicast group to send to
 * @li @c port port to send to
 *  
 * An example element looks like this :
 * @verbatim
<NetworkSink name="station name" number="1" multicast-address="224.0.0.10" port="12345">
    <Any EventGenerator element type>
</NetworkSink>@endverbatim
 */

#ifndef _NETWORKSINK_H
#define _NETWORKSINK_H

#include "../OpenTracker.h"
#include "Network.h"

struct MulticastGroup;

/**
 * This class implements a simple node that stores a copy of the last
 * event it received and passed on for output to the console. The 
 * associated ConsoleModule polls the nodes regularly and prints
 * out the last state stored.
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API NetworkSink : public Node
{
// Members
public:
    /// station name
    string stationName;
    /// station number
    short int stationNumber;
    /// multicast group pointer
    MulticastGroup * group;
    /// flag whether it was modified since last turn
    int modified;
    /// the state that is stored
    State state;

// Methods
public:
    /** constructor method,sets members 
     * @param name_ the station name 
     * @param number_ the station number
     * @param group_ pointer of the multicast group this station belongs to
     */
    NetworkSink( string & name_,short int number_, MulticastGroup * group_ ) :
        Node(),     
        stationName( name_ ),
        stationNumber( number_ ),
        group( group_ ),
        modified( 0 )
    {};
    
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    };
    
    /**
     * this method notifies the object that a new event was generated.
     * It stores a copy of the received event and passes the event on
     * to its observers.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State & event, Node & generator)
    {
        state = event;
        modified = 1;
        updateObservers( state );
    };
};

#endif
