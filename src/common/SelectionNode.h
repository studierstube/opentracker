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
/** header file for Selection Node.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/SelectionNode.h,v 1.3 2001/09/26 13:33:39 reitmayr Exp $ 
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section selection Selection
 * A Selection node is an EventGenerator node that listens to two other
 * EventGenerator nodes and selects preferred data by the following algorithm.
 * Data from the preferred input is always passed through. Moreover, the node
 * also remembers the timestamp of the last preferred event. If an event arrives
 * from the other input within a certain timeout period to the last preferred
 * event, it is not passed on. This way a stream of preferred events, with 
 * intermittend pauses smaller then the timeout will not be mixed with data
 * from the default source.
 * The preferred input EventGenerator is marked with wrapper tags.
 * It has the following attributes :
 * @li @c timeout, time period between two input events (in milliseconds)

 * and following inputs
 * @li @c Select marks the preferred EventGenerator
 * 
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <Selection timeout="100">
    <Select>
        <One of any EventGenerator element type>
    </Select>
    <One of any EventGenerator element type>
  </Selection>@endverbatim
 */

#ifndef _SELECTIONNODE_H
#define _SELECTIONNODE_H

#include "../OpenTracker.h"

/**
 * A SelectionNode is an EventGenerator node that observes two marked input
 * nodes and selects the data provided by the input nodes. The mark decides
 * from which input node the data should be prefered. It is implemented using
 * wrapper nodes, timeout period and apropriate DTD definitions.
 * @author Ivan Viola, Matej Mlejnek
 * @ingroup common
 */
class OPENTRACKER_API SelectionNode : public Node   
{

protected:
	/// Variable for timeout value (in miliseconds)
    double timeOut;
	/// Variable for the last event timestamp from the prefered input node
	double lastTimeStamp;
    
    /** constructor method. It sets initial values for the timeout. */
    SelectionNode( double t );

public:
    /** tests for EventGenerator interface being present and returns
     * 1, if present.
     * @return always 1 */
    virtual int isEventGenerator()
    {
    	return 1;
    }

    /**
     * This method is called by any child node. Checks whether the incoming
     * event is from the wrapper node, otherwise it compares it against the
     * last event from the wrapper node and if it was created later then
     * the specified time out, it is passed on.
	 * @param event the event value passed
	 * @param generator the node generating the event
     */
    virtual void onEventGenerated( State& event, Node & generator);                                

    friend class CommonNodeFactory;
};

#endif

