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
/** source file for DynamicTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "DynamicTransformation.h"

#include <iostream>

// Constructor

namespace ot {

DynamicTransformation::DynamicTransformation( int baseEvent_, bool usePos_, bool useOrient_ ) :
	StaticTransformation(),
		baseEvent( baseEvent_ )
{
    usePos = usePos_;
    useOrient = useOrient_;            
}

// this method is called by the EventGenerator to update it's observers.

void DynamicTransformation::onEventGenerated( State& event, Node& generator)
{
    if( generator.isNodePort() == 1 )     // if the event is from the NodePort 
    {	                                  // node, its a change to the base.
        for( int i = 0; i < 3; i ++ )
        {
            translation[i] = event.position[i];
            rotation[i] = event.orientation[i];
        }
        this->rotation[3] = event.orientation[3];

		confidence = event.confidence;

		if( baseEvent == 1 )
		{
            store.time = event.time;
			StaticTransformation::onEventGenerated( store, generator );
		}
    } else 
    {
        store = event;
        StaticTransformation::onEventGenerated( event, generator );
    }
}

} // namespace ot
