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
/** header file for TCPSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/TCPSink.h,v 1.3 2003/07/18 18:23:25 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section tcpsink TCPSink
 * The TCPSink node is a simple sink for the @ref tcpmodule module. It resembles 
 * a single station in the stream of data that is served to clients by the @ref
 * tcpmodule module. The node has the following elements :
 * @li @c station number of the station
 * @li @c position on|off flag to specify whether position information is to be send
 * @li @c orientation on|off flag to specify whether orientation information is to be send
 * @li @c button on|off flag to specify whether button information is to be send
 * @li @c time on|off flag to specify whether timestamp information is to be send
 *
 * An example element looks like this :
 * @verbatim
<TCPSink station="0" position="on" rotation="off" >
    <Any EventGenerator element type>
</TCPSink>@endverbatim
 */

#ifndef _TCPSINK_H
#define _TCPSINK_H

#include "../OpenTracker.h"

/** This class is a simple sink that stores a copy of all data passing through it
 * and marks itself as changed for the TCPModule to process the new data. It also
 * stores the protocol information necessary to send the right data.
 * @author Gerhard Reitmayr
 * @ingroup network
 */
class OPENTRACKER_API TCPSink : public Node
{
// Members
public:
    /// station number
    int station;
    /// flag whether it was modified since last turn
    int changed;
    /// the state that is stored
    State state;
    /// flags whether what to output
    int posFlag, orientFlag, buttonFlag,timeFlag;

// Methods
protected:
    /** constructor method,sets members 
     * @param number_ the station number
     */
    TCPSink(  int number_, int pos_, int rot_, int but_, int time_ ) :
       Node(),
       station( number_ ),
       changed( 0 ),
       posFlag( pos_ ),
       orientFlag( rot_ ),
       buttonFlag( but_ ),
       timeFlag( time_ )
    {}
    
public:
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
    
    /**
     * this method notifies the object that a new event was generated.
     * It stores a copy of the received event, marks itself as changed
     * and passes the event on to its observers.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State & event, Node & generator)
    {
        state = event;
        changed = 1;
        updateObservers( state );
    }

    friend class TCPModule;
};

#endif
