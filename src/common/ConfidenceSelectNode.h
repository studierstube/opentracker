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
  * @todo add documentation
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConfidenceSelectNode.h,v 1.1 2001/10/20 17:20:11 reitmayr Exp $ 
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section confidenceselect ConfidenceSelect
 *
 * An example element using all wrapper elements looks like this :
 * @verbatim
<ConfidenceFilter treshhold="0.8" type="high" >
    ...any event node
</ConfidenceFilter>@endverbatim
 */

#ifndef _CONFIDENCESELECTNODE_H
#define _CONFIDENCESELECTNODE_H

#include "../OpenTracker.h"

/**
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

