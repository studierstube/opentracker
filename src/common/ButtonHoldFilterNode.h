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
/** header file for ButtonHoldFilter Node.
  *
  * @author Thomas Psik tomp@ims.tuwien.ac.at
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ButtonHoldFilterNode.h,v 1.1 2003/11/13 09:56:24 tomp Exp $ 
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section buttonholdfilter ButtonHoldFilter
 * ButtonHoldFilter simulates an time delayed release of a buttonsource. It was necessary to introduce this filter to 
 * filter out short drops when using a radio transmission. The filter will hold the "on" state even if there is a drop of 
 * &quot;n&quot; off's. After &quot;n&quot; off's have pssed through the filter it will change the state to off. If there is a "on" in between, 
 * the count will start again. Please notice that there will be a delay in the button-release action due to this filter.
 *

Example_ for one bit(n=3) - over time)
  @verbatim
   input : 11110111001101000011111
   output: 11111111111111111011111
  @endverbatim
Example_ for one bit(n=1) - over time)
  @verbatim
   input : 11110111001101000011111
   output: 11111111101111100011111
 @endverbatim

  Example usage/defaults:

  @verbatim
\<ButtonHoldFilter offDuration="2" >
    ... (any Event Source)
\</ButtonHoldFilter>
@endverbatim
*/

#ifndef _ButtonHoldFilterNode_H
#define _ButtonHoldFilterNode_H

#include "../OpenTracker.h"

/**
 * @author Thomas Psik
 * @ingroup common
 */
class OPENTRACKER_API ButtonHoldFilterNode : public Node   
{

protected:
    bool init;
    /// number of off's necessary to show off in outgoing state
    int offDuration;

	// internal variables
    State currentState; ///
    int targetButtonState; ///
	int offCounter[16]; /// counter for each bit

    /** constructor method. It sets initial values for the treshhold and type */
    ButtonHoldFilterNode( int offDuration = 2);

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

    friend class ButtonHoldFilterModule;
};

#endif
