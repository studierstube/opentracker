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
/** header file for Filter Node.
  *
  * @author Gerhard Reitmayr
  * @todo add documentation for Filter node
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FilterNode.h,v 1.1 2001/10/20 17:20:11 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _FILTERNODE_H
#define _FILTERNODE_H

#include "../OpenTracker.h"

#include <vector>

/**
 * The Filter node generates a new state event by combining several past 
 * events in a linear fashion. The weights of the summands can be adjusted.
 * It needs a child that both implements the Event and the EventQueue interface
 * to have access to the required number of states and to be triggered to
 * generate a new one.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API FilterNode
    : public Node
{
// Members
protected:
    /// local state variable
    State localState;

    /// array of weights
    std::vector<float> weights;

// Methods
protected:

    /** constructor method
     */
    FilterNode( const std::vector<float> & weights_ );

public:

    /** tests for EventGenerator interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventGenerator, 0 otherwise */
    virtual int isEventGenerator() ;

    /**
     * this method is called by the EventGenerator to update it's observers.
     * This class computes a transformed state, stores it in its local variable
     * and notifies its observers in turn, propagating the change.
     */
    virtual void onEventGenerated( State& event, Node& generator);

    friend class CommonNodeFactory;
};

#endif
