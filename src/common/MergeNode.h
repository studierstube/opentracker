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
/** header file for Merge Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/MergeNode.h,v 1.7 2001/07/16 21:43:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section merge Merge
 * A Merge node is an EventGenerator node that listens to several other
 * EventGenerator nodes and merges data from these. It has several inputs
 * that are marked with different wrapper tags. It stores an internal state
 * and updates it with parts of the data depending on the type of input. Then
 * it generates an event of its own. The following list shows the possible
 * inputs :
 * 
 * @li @c MergePosition only the position data is taken from events received
 *        from children of this wrapper element
 * @li @c MergeOrientation only the orientation data is taken
 * @li @c MergeButton only the button data is taken
 * @li @c MergeConfidence only the confidence data is taken
 * @li @c MergeTime only the time stamp data is taken
 * @li @c MergeDefault any data that is not set by a child of another
 *        wrapper element is used.
 * 
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <Merge>
    <MergeDefault>
        <One or more of any EventGenerator element type>
    </MergeDefault>
    <MergePosition>
        <One or more of any EventGenerator element type>
    </MergePosition>
    <MergeOrientation>
        <One or more of any EventGenerator element type>
    </MergeOrientation>
    <MergeButton>
        <One or more of any EventGenerator element type>
    </MergeButton>
    <MergeConfidence>
        <One or more of any EventGenerator element type>
    </MergeConfidence>
    <MergeTime>
        <One or more of any EventGenerator element type>
    </MergeTime>
 </Merge>@endverbatim
 */

#ifndef _MERGENODE_H
#define _MERGENODE_H

#include "../OpenTracker.h"

/**
 * A MergeNode is an EventGenerator node that observes several marked input
 * nodes and merges the data provided by the input nodes. The mark decides
 * which part of the data is used in the merge. It is implemented using
 * wrapper nodes and apropriate DTD definitions.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API MergeNode : public Node   
{

protected:
    /// State variable to put transformed state into
    State localState;
    
    /** constructor method
     */
    MergeNode()
    {}

public: 

    /** tests for EventGenerator interface being present and returns
     * 1, if present.
     * @return always 1 */
    virtual int isEventGenerator()
    {
    	return 1;
    }

    /**
     * This method is called by any child node. It updates the local
	 * state with the right part of the passed event and notifies
	 * it's parent and references in turn.
	 * @param event the event value passed
	 * @param generator the node generating the event
     */
    virtual void onEventGenerated( State& event, Node & generator);                                

    friend class CommonNodeFactory;
};

#endif

