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
/** source file for PositionFilterNode.
  *
  * @author Thomas Psik tomp@ims.tuwien.ac.at
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */


#include "PositionFilterNode.h"

//using namespace std;


namespace ot {


// constructor method.

PositionFilterNode::PositionFilterNode( const float min_[3], const float max_[3] )
    : Node()
{
	min[0] = min_[0];
	min[1] = min_[1];
	min[2] = min_[2];

	max[0] = max_[0];
	max[1] = max_[1];
	max[2] = max_[2];


}

int PositionFilterNode::isEventGenerator()
{
	return 1;
}

// this method is called by the EventGenerator to update it's observers.

void PositionFilterNode::onEventGenerated( State& event, Node& generator)
{
    if(  
		min[0] < event.position[0] && event.position[0] < max[0] 
		&&
		min[1] < event.position[1] && event.position[1] < max[1] 		
		&&
		min[2] < event.position[2] && event.position[2] < max[2] 
		)
    {
        updateObservers( event );
    }
}


}  // namespace ot
