/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** header file for Merge Tracker Node.
*
* @author Gratl/Werlberger VRVU
*
* $Id:$
* @file                                                                   */
/* ======================================================================= */

/**
* @page Nodes Node Reference
* @section TriangulateOrientation TriangulateOrientation
* TODO description
*
* inputs :
* 
* TODO
* 
* An example element using all wrapper elements looks like this :
* @verbatim
TODO example

This node calculates the changes in the orientation of 3 Source nodes.
The trackers providing data to the source nodes are mounted at the corners 
of a equilateral triangle. The node also calculates the average value of the 
positions of the 3 tracking sensors.

Order of elements on the triangle:
First element is the top element of wooden triangle
Second element is the left element of wooden triangle
Third element is the right element of wooden triangle


<TriangulateOrientation>
  <EventTransform DEF="top" scale="-1 1 -1" >
      <UbisenseSource DEF="testar1" object="Tag 016-000-002-104"/>
  </EventTransform>
  <EventTransform DEF="bLeft" scale="-1 1 -1" >
      <UbisenseSource DEF="testar6" object="Tag 016-000-002-111"/>
  </EventTransform>
  <EventTransform DEF="bRight" scale="-1 1 -1" >
      <UbisenseSource DEF="testar9" object="Tag 016-000-002-112"/>
  </EventTransform>
</TriangulateOrientation>


*/

#ifndef _TRIANGULATEORIENTATIONNODE_H
#define _TRIANGULATEORIENTATIONNODE_H

#include "../OpenTracker.h"

/**
*
*/

namespace ot
{

	class OPENTRACKER_API TriangulateOrientationNode : public Node   
	{

	protected:
		//! constructor method
		TriangulateOrientationNode();

		//! states of top tracking device
	    Event topState;

		//! states of top tracking device
		Event bLeftState;

		//! states of top tracking device
		Event bRightState;

		//! merged state
		Event mergedState;

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
		virtual void onEventGenerated( Event& event, Node & generator);                                

		friend class CommonNodeFactory;
	};

} // namespace ot

#endif
