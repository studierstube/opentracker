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
/** source file for TimeGateNode
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/TimeGateNode.cxx,v 1.1 2002/09/26 21:32:17 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "TimeGateNode.h"
#include "math.h"

// constructor method
TimeGateNode::TimeGateNode( double timeframe_, Mode mode_ )
{
	timeframe = timeframe_;
    mode = mode_;
	lastTimeStamp = 0;
} 

// generates a new data item upon receiving an event
void TimeGateNode::onEventGenerated( State& event, Node & generator )
{
    // if it came from the gate, save the timestamp
	if( generator.isNodePort() == 1 )
	{
		lastTimeStamp = event.time;		
	}
	else
	{
        bool inTimeframe = (timeframe > fabs( lastTimeStamp - event.time ));
        if(( PASS == mode && inTimeframe ) ||
           ( BLOCK == mode && !inTimeframe ))
        {
		    updateObservers( event );
        }
	}		    
}
