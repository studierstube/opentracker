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
/** source file for DynamicTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/DynamicTransformation.cxx,v 1.6 2001/04/12 06:41:38 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "DynamicTransformation.h"

#ifdef WIN32
#include <iostream>    // VisualC++ uses the STL based iostream lib
#else
#include <iostream.h>
#endif


#include <dom/DOM_Node.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NamedNodeMap.hpp>

// Constructor

DynamicTransformation::DynamicTransformation() : StaticTransformation()
{
    usePos = 1;
    useOrient = 1;
}

// this method is called by the EventGenerator to update it's observers.

void DynamicTransformation::onEventGenerated( State& event, Node& generator)
{
    if( generator.isWrapperNode() == 1 )  // if the event is from the wrapper 
    {									  // node, its a change to the base.
        for( int i = 0; i < 3; i ++ )
        {
            translation[i] = event.position[i];
            rotation[i] = event.orientation[i];
        }
        this->rotation[3] = event.orientation[3];
        StaticTransformation::onEventGenerated( store, generator );
    } else 
    {
        store = event;
        StaticTransformation::onEventGenerated( event, generator );
    }
}

// returns the number of children

unsigned int DynamicTransformation::countChildren()
{
	return 1;
}

// iterates through the children by returning the child by index

Node * DynamicTransformation::getChild( unsigned int index )
{
	DOM_NodeList list = parent->getChildNodes();
    for( int i = 0; i < list.getLength(); i++ )
	{
        if(((Node *)(list.item( i ).getUserData()))->isWrapperNode() == 1 )
            continue;
		return (Node *)list.item( i ).getUserData();

	}
	return NULL;
}