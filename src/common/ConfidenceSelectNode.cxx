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
/** source file for ConfidenceSelectNode node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConfidenceSelectNode.cxx,v 1.2 2003/07/18 18:23:25 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ConfidenceSelectNode.h"

// constructor method
ConfidenceSelectNode::ConfidenceSelectNode( double timeout_, types type_ )
   : timeout( timeout_ ), type( type_ )
{
	lastConfidence = 0;
	lastTimeStamp = 0;
} 

// tests the confidence value and only forwards passing events
void ConfidenceSelectNode::onEventGenerated( State& event, Node & generator )
{
	if( timeout < event.time - lastTimeStamp )
	{
		lastConfidence = event.confidence;
		lastTimeStamp = event.time;
		updateObservers( event );		
	} else 
	{
		switch( type )
		{
		case HIGH : if( event.confidence >= lastConfidence )
					{
						lastConfidence = event.confidence;
						lastTimeStamp = event.time;
						updateObservers( event );
					}
					break;
		case LOW : if( event.confidence <= lastConfidence )
					{
						lastConfidence = event.confidence;
						lastTimeStamp = event.time;
						updateObservers( event );
					}
					break;
		}	
	}	
}
