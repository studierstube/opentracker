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
/** header file for TimeGate Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section timegate TimeGate
 * This node acts as a gate for an event flow. It will only let events through, if
 * there was an event received through the Gate input some time before. The exact
 * time intervall is given by the attribute timeframe (in milliseconds). It also
 * works the opposite way by blocking all events coming in within the time frame.
 * In contrast to the @ref selection , it does not pass on event data from the Gate.
 * It has the following attributes :
 * @li @c timeframe, time period between two input events (in milliseconds)
 * @li @c mode (pass|block) in @c pass mode it will only let events pass, that
 *        fall within a timeframe interval of an event received through the Gate
 *        input. In block mode it will block these events.
 *
 * and following inputs
 * @li @c Gate marks the gate that opens or closes the node for some time
 * 
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <TimeGate timeframe="100" mode="pass">
    <Gate>
        <One of any EventGenerator element type>
    </Gate>
    <One of any EventGenerator element type>
  </TimeGate>@endverbatim
 */

#ifndef _TIMEGATENODE_H
#define _TIMEGATENODE_H

#include "../OpenTracker.h"

/**
 * A TimeGateNode is an EventGenerator node that observes two marked input
 * nodes and selects the data provided by the input nodes. The mark decides
 * from which input node the data should be prefered. It is implemented using
 * wrapper nodes, timeout period and apropriate DTD definitions.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API TimeGateNode : public Node   
{

protected:
	/// Variable for timeout value (in miliseconds)
    double timeframe;
	/// Variable for the last event timestamp from the prefered input node
	double lastTimeStamp;
    /// mode selection
    enum Mode { PASS , BLOCK } mode;
    
    /** constructor method. It sets initial values for the timeframe and mode. */
    TimeGateNode( double timeframe_, Mode mode_ );

public:
    /** tests for EventGenerator interface being present and returns
     * 1, if present.
     * @return always 1 */
    virtual int isEventGenerator()
    {
    	return 1;
    }

    /**
     * This method is called by any child node. 
	 * @param event the event value passed
	 * @param generator the node generating the event
     */
    virtual void onEventGenerated( State& event, Node & generator);                                

    friend class CommonNodeFactory;
};

#endif
