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
/** header file for StbSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: StbSink.h 4055 2004-11-26 10:41:36Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __STBSINK_H__
#define __STBSINK_H__

#include <OpenTracker.h>
#include <stbapi/stbapi.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

using namespace ot;

class TrackerOT;

/**
 * This class implements a simple node that stores a copy of the last
 * event it received and passed on for output to Studierstube. The TrackerOT
 * class copies the data from this node into Studierstube data structures.
 * The corresponding OpenTracker configuration element  has the following attributes :
 * @li @c station integer starting from 0 giving the station number to use in Stb
 * @li @c event on|off whether this station should emit events or not, default is off
 *
 * An example element looks like this :
 * @verbatim
<StbSink station="0" event="on">
    <Any EventGenerator element type>
</StbSink>@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup tracking
 */
class STBAPI_API StbSink : public Node
{

public:
    /// event enabled or not
    int events;
    /// the state and station that is stored
    StationEventInfo state;
    /// pointer to parent TrackerOT class
    TrackerOT * parent;

// Methods
public:
    /** constructor sets the number of the station
     * @param parent_ the parent TrackerOT object 
     * @param number the station number we output to
     * @param events_ flag whether 3D events are enabled or not 
     */
    StbSink( TrackerOT * parent_, int number, int events_ ) : events( events_ ), parent( parent_ )
    {
        state.station = number;        
    }

    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator();
    
    /**
     * this method notifies the object that a new event was generated.
     * It stores a copy of the received event and passes the event on
     * to its observers.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event, Node& generator);
};

#endif
