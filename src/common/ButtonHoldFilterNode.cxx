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
/** source file for ElasticFilter node.
  *
  * @author Thomas Psik
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ButtonHoldFilterNode.cxx,v 1.1 2003/11/13 09:56:24 tomp Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ButtonHoldFilterNode.h"

// constructor method
ButtonHoldFilterNode::ButtonHoldFilterNode( int offDuration_)
   : currentState()
{
	offDuration = offDuration_;
    init = false;
} 

// tests the confidence value and only forwards passing events
void ButtonHoldFilterNode::onEventGenerated( State& event, Node & generator )
{
    //printf("ButtonHoldFilterNode::onEventGenerated() \n");
    if (!init){
        init = true;
    }

    // we have to pass on the information 
	targetButtonState = event.button;
	currentState = event;
}

void ButtonHoldFilterNode::push() {
    if (init) {
        //printf("ButtonHoldFilterNode::push() \n");
		int updatedButtonState = 0;

		for (int i=0; i< 8; i++)
		{
			unsigned char bit = (targetButtonState >>i) & 1;
			
			// if bit is off incr counter
			if (!bit) 
			{
				if (offCounter[i] < offDuration)
					updatedButtonState |= 1<<i;
				// else - button has been off at least offDuration times -> dont set bit -> button is off

				offCounter[i]++;

			} 
			else
			{
				offCounter[i] = 0; // reset counter on "on"
				// set bit in button state
				updatedButtonState |= 1<<i;
			}
		}
    
		currentState.button = updatedButtonState;
        currentState.timeStamp();
        updateObservers(currentState);
    }
}
