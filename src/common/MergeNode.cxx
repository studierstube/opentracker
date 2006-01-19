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
/** source file for Merge Node.
  *
  * @author Gerhard Reitmayr
  * @todo optimize nodeport test, maybe by implementing special nodeports ?
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "MergeNode.h"

namespace ot {

/** definitions for flags defining whats happening for each
 * EventGenerator. These are binary flags that can be or'd
 * together.
 */
enum Flags { 
    DEFAULT     = 1, 
    POSITION    = 2, 
    ORIENTATION = 4, 
    BUTTON      = 8,
    CONFIDENCE  = 16, 
    TIME        = 32 
};

// generates a new data item upon receiving an event

void MergeNode::onEventGenerated( State& event, Node & generator)
{
	unsigned flag = 0;
	if( generator.isNodePort() == 1 )  // this should always be the case
	{
		NodePort & wrap = (NodePort &)generator;
		if( wrap.getType().compare("MergeDefault") == 0 )
		{
			if((getPort("MergePosition") == NULL) || 
               (getPort("MergePosition")->countChildren() == 0 ))
				flag |= POSITION;
			if((getPort("MergeOrientation") == NULL) || 
               (getPort("MergeOrientation")->countChildren() == 0 ))
				flag |= ORIENTATION;
			if((getPort("MergeButton") == NULL) || 
               (getPort("MergeButton")->countChildren() == 0 ))
				flag |= BUTTON;
			if((getPort("MergeConfidence") == NULL) || 
               (getPort("MergeConfidence")->countChildren() == 0 ))
				flag |= CONFIDENCE;			
		} 
		else if( wrap.getType().compare("MergePosition") == 0 )        
			flag |= POSITION;                    
		else if( wrap.getType().compare("MergeOrientation") == 0 )
			flag |= ORIENTATION | TIME;
		else if( wrap.getType().compare("MergeButton") == 0 )
			flag |= BUTTON | TIME;
		else if( wrap.getType().compare("MergeConfidence") == 0 )
			flag |= CONFIDENCE | TIME;
		else if( wrap.getType().compare("MergeTime") == 0 )
			flag |= TIME;			
        // if there is no special time node, always propagate time !
        if((getPort("MergeTime") == NULL) || (getPort("MergeTime")->countChildren() == 0 ))
	        flag |= TIME;
		if( flag & POSITION )
		{
			localState.position[0] = event.position[0];
			localState.position[1] = event.position[1];
			localState.position[2] = event.position[2];
		}	
		if( flag & ORIENTATION )
		{
			localState.orientation[0] = event.orientation[0];
			localState.orientation[1] = event.orientation[1];
			localState.orientation[2] = event.orientation[2];
			localState.orientation[3] = event.orientation[3];
		}
		if( flag & BUTTON )
			localState.button = event.button;
		if( flag & CONFIDENCE )
			localState.confidence = event.confidence;
		if( flag & TIME )        
		    localState.time = event.time;        
		updateObservers( localState );
	}
}                                  

} // namespace ot
