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
/** header file for ElasticFilter Node.
  *
  * @author Flo Ledermann ledermann@ims.tuwien.ac.at
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ElasticFilterNode.h,v 1.1 2003/03/24 12:28:46 flo Exp $ 
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section elasticfilter ElasticFilter
 * ElasticFilter simulates an elastic connection between the incoming tracking data and
 * the data that is sent out to the parent nodes. It also interpolates the values between two
 * updates from the source, so this is also useful if you have a low update rate from
 * your tracking and still want smooth animation.
*/

#ifndef _ELASTICFILTERNODE_H
#define _ELASTICFILTERNODE_H

#include "../OpenTracker.h"

/**
 * @author Flo Ledermann
 * @ingroup common
 */
class OPENTRACKER_API ElasticFilterNode : public Node   
{

protected:
    bool init;
    /// frequency of updates
    int frequency;
    /// offset of first update relative to main loop start
    int offset;
    /// the state that is posted to the EventObservers
    float damp;
    float force;

    State currentState;
    State targetState;
    State vState;

    /** constructor method. It sets initial values for the treshhold and type */
    ElasticFilterNode( float force_ = 0.02, float damp_ = 0.5, int frequency_ = 1, int offset_ = 0);

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

    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator */
    void push();

    friend class InterpolatorModule;
};

#endif

