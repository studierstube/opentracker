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
/** source file for Selection Node.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/SelectionNode.cxx,v 1.3 2001/09/26 13:33:39 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SelectionNode.h"

// constructor method
SelectionNode::SelectionNode( double t )
{
	timeOut = t;
	lastTimeStamp = 0;
} 

// generates a new data item upon receiving an event
void SelectionNode::onEventGenerated( State& event, Node & generator )
{
	if( generator.isNodePort() == 1 )
	{
		lastTimeStamp = event.time;
		updateObservers( event );
	}
	else if( timeOut < event.time - lastTimeStamp )
	{
		updateObservers( event );
	}		
}
