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
/** source file for ConfidenceFilterNode node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "ConfidenceFilterNode.h"

namespace ot {

// constructor method
ConfidenceFilterNode::ConfidenceFilterNode( float treshhold_, types type_ )
    : type( type_ )
{
	if( treshhold_ < 0 )
        treshhold_ = 0;
    else if( treshhold_ > 1 )
        treshhold_ = 1;
    treshhold = treshhold_;
} 

// tests the confidence value and only forwards passing events
void ConfidenceFilterNode::onEventGenerated( State& event, Node & generator )
{
    switch( type )
    {
    case HIGH : if( event.confidence >= treshhold )
                    updateObservers( event );
                break;
    case LOW : if( event.confidence <= treshhold )
                    updateObservers( event );
                break;
    }	
}

} // namespace ot
