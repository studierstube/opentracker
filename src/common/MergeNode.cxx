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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Merge Node.
  *
  * @author Gerhard Reitmayr
  * @todo optimize nodeport test, maybe by implementing special nodeports ?
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/MergeNode.cxx,v 1.9 2001/10/20 17:20:11 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "MergeNode.h"

/** definitions for flags defining whats happening for each
 * EventGenerator. These are binary flags that can be or'd
 * together.
 */
unsigned DEFAULT = 1, 
         POSITION = 2, 
         ORIENTATION = 4, 
         BUTTON = 8,
         CONFIDENCE = 16, 
         TIME = 32;

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
			if((getPort("MergeTime") == NULL) || 
               (getPort("MergeTime")->countChildren() == 0 ))
				flag |= TIME;
		} 
		else if( wrap.getType().compare("MergePosition") == 0 )
			flag |= POSITION;
		else if( wrap.getType().compare("MergeOrientation") == 0 )
			flag |= ORIENTATION;
		else if( wrap.getType().compare("MergeButton") == 0 )
			flag |= BUTTON;
		else if( wrap.getType().compare("MergeConfidence") == 0 )
			flag |= CONFIDENCE;
		else if( wrap.getType().compare("MergeTime") == 0 )
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
