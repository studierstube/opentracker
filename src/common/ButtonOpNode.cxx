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
/** source file for ButtonOp Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ButtonOpNode.cxx,v 1.1 2002/02/11 10:41:04 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ButtonOpNode.h"

using namespace std;

// constructor method.

ButtonOpNode::ButtonOpNode( const Op & op )
    : Node(), operation( op ), arg1(0), arg2(0)
{
}

int ButtonOpNode::isEventGenerator()
{
    return 1;
}

// this method is called by the EventGenerator to update it's observers.

void ButtonOpNode::onEventGenerated( State& event, Node& generator)
{
    if( generator.isNodePort() == 1 )
    {
        if( generator.getType().compare("Arg1") == 0 )
            arg1 = event.button;
        else
            arg2 = event.button;
        result = event;
        switch( operation )
        {
        case OR :
            result.button = arg1 | arg2;
            break;
        case AND :
            result.button = arg1 & arg2;
            break;
        }
        updateObservers( result );
    }
}
