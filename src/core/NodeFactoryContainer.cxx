 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for NodeFactoryContainer class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/NodeFactoryContainer.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
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
