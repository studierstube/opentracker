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
/** header file for ConfidenceSelect Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConfidenceSelectNode.h,v 1.2 2001/10/21 22:10:56 reitmayr Exp $ 
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section confidenceselect ConfidenceSelect
 *
 * The ConfidenceSelect node will filter events based on their confidence value
 * relative to confidence values seen before. It can be used to select the
 * event with the highest confidence value within a short time interval. It is
 * a simple EventGenerator node.
 *
 * It will store the confidence value of an
 * event received and filter out any events that arrive within a certain time span
 * set by the timeout parameter and that have a confidence value below ( or higher )
 * than the stored one. After the timeout it will again store the next event received.
 * If an event passes the test, the events confidence value is stored and
 * the timeout starts again. Two modes are possible : @c high filters out lower values and
 * @c low filters out higher values. The element has the following attributes :
 * @li @c timeout the timeout value in milliseconds
 * @li @c type high either @c high or @c low to set the type of filtering
 *
 * An example element looks like this :
 * @verbatim
<ConfidenceSelect timeout="100" type="high" >
    ...any event node
</ConfidenceFilter>@endverbatim
 */

#ifndef _CONFIDENCESELECTNODE_H
#define _CONFIDENCESELECTNODE_H

#include "../OpenTracker.h"

/**
 * This class implements a filter based on the confidence value of events relative to
 * earlier confidence values seen. It is similar to the ConfidenceFilterNode.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API ConfidenceSelectNode : public Node   
{

protected:
	/// timeout value
    double timeout;
    /// last time stamp seen
    double lastTimeStamp;
    /// last confidence value seen
    float lastConfidence;
    /// the type of filter
    enum types { HIGH = 0, LOW } type;
    
    /** constructor method. It sets initial values for the treshhold and type */
    ConfidenceSelectNode( double timeout_ = 100, types type_ = HIGH );

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

