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
/** source file for NodeFactoryContainer class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/NodeFactoryContainer.cxx,v 1.2 2001/03/27 06:08:50 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "NodeFactoryContainer.h"

#ifdef __sgi
#include <algo.h>
#else
#include <algorithm>
#endif

/// Destructor method.

NodeFactoryContainer::~NodeFactoryContainer()
{
    factories.clear();
}

/// adds a NodeFactory to the container.

void NodeFactoryContainer::addFactory(NodeFactory& factory)
{
    NodeFactoryVector::iterator result = std::find( factories.begin(), factories.end(), &factory );
    if( result == factories.end())
    {
        factories.push_back( &factory );
    }
}

/// ructs a new Node.

Node * NodeFactoryContainer::createNode( string& name,
                                         StringMap& attributes)
{
    Node * value = NULL;
    NodeFactoryVector::iterator it = factories.begin();
    while( it != factories.end() && value == NULL )
    {
        value = (*it)->createNode( name, attributes );
        it++;
    }
    return value;
}

/// removes a NodeFactory from the container.

void NodeFactoryContainer::removeFactory(NodeFactory& factory)
{
    NodeFactoryVector::iterator result = std::find( factories.begin(), factories.end(), &factory );
    if( result != factories.end())
    {
        factories.erase( result );
    }
}
