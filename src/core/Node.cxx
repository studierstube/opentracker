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
/** The source file for the basic Node class.
  *
  * @author Gerhard Reitmayr
  * @todo add exception handling and error code returns
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Node.cxx,v 1.13 2001/08/07 13:28:04 reitmayr Exp $
  * @file                                                                   */  
 /* ======================================================================= */

#include "Node.h"
#include "StringTable.h"
#include "Context.h"

#ifdef WIN32
#include <iostream>// VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include <algorithm>

#include <dom/DOM_Node.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_DOMException.hpp>

using namespace std;

// constructor
Node::Node() : name("")
{
	parent = new DOM_Element();	
}

// destructor

Node::~Node()
{
	references.clear();
	*parent = 0;
	delete parent;
}

// sets the DOM_Element this node belongs to ( not the parent in OT )

void Node::setParent( DOM_Element & parElement )
{
	*parent = parElement;
	parent->setUserData( this );
    type = parent->getTagName().transcode();
}

// adds a reference to the node

void Node::addReference( Node * reference )
{
	references.push_back( reference );
}

// removes a reference from the node

void Node::removeReference( Node * reference )
{
	NodeVector::iterator result = find( references.begin(), references.end(), 
		reference );
	if( result != references.end())
	{
		references.erase( result );
	}
}

// returns a pointer to the parent node

Node * Node::getParent()
{
    const DOM_Node & parentElement = parent->getParentNode();
	if( parentElement != 0 )
	{
		return (Node *)parentElement.getUserData();
	}
    return NULL;
}

// returns the number of children

unsigned int Node::countChildren()
{
    DOM_Node node = parent->getFirstChild();
    unsigned int count = 0;
    while( !node.isNull())
    {
        Node * myNode = (Node *)node.getUserData();
        if( myNode != NULL )
        {
            if( myNode->isNodePort() == 0 )
                count ++;
        }
        node = node.getNextSibling();
    }
    return count;
}

// adds a new child

Node::error Node::addChild(Node & child)
{
    try {
        parent->appendChild( *(child.parent));
    }
    catch( DOM_DOMException& e )
    {}
    return OK;
}

// removes a child

Node::error Node::removeChild(Node & child)
{
    try {
        parent->removeChild( *(child.parent));
    }
    catch( DOM_DOMException e )
    {}
    return OK;
}

// iterates through the children by returning the child by index

Node * Node::getChild( unsigned int index )
{
    Node * myNode = NULL;
    DOM_Node node = parent->getFirstChild();
    while( !node.isNull())
    {
        myNode = (Node *)node.getUserData();
        if( myNode != NULL )		
            if( myNode->isNodePort() == 0 )
				if( index == 0 )
					return myNode;
				else 
					index--;
        node = node.getNextSibling();                
    }
    return NULL;
}

// returns number of NodePorts present on this Node

unsigned int Node::countPorts()
{
    Node * myNode = NULL;
    unsigned int count = 0;
    DOM_Node node = parent->getFirstChild();
    while( !node.isNull())
    {
        myNode = (Node *)node.getUserData();
        if( myNode != NULL )		
            if( myNode->isNodePort() == 1 )
                count++;
        node = node.getNextSibling();                
    }
	return count;
}

// returns a NodePort child object indexed by Name

NodePort * Node::getPort( const string & name )
{
	DOM_NodeList list = parent->getElementsByTagName( name.c_str() );
	if( list.getLength() > 0 )
	{
		DOM_Element portElement = (const DOM_Element &) list.item(0);
		Node * port = (Node *)portElement.getUserData();
        if( port != NULL )
            if( port->isNodePort() == 1 )
                return (NodePort *)port;
	}
	return NULL;
}

// returns a NodePort child object by index

NodePort * Node::getPort( unsigned int index )
{
    Node * myNode = NULL;
    DOM_Node node = parent->getFirstChild();
    while( !node.isNull())
    {
        myNode = (Node *)node.getUserData();
        if( myNode != NULL )		
            if( myNode->isNodePort() == 1 )
				if( index == 0 )
					return (NodePort *) myNode;
				else 
					index--;
        node = node.getNextSibling();                
    }
    return NULL;
}

// creates and adds a new child NodePort object of the given name.

Node::error Node::addPort( const std::string & name )
{
    if( parent->getElementsByTagName( name.c_str()).getLength() > 0 )
        return GRAPH_CONSTRAINT;
    DOM_Document doc = parent->getOwnerDocument();    
    Context * context = (Context *)doc.getUserData();
    StringTable table;
    Node * node = context->createNode( name, table);
    if( node == NULL )
        return GRAPH_CONSTRAINT;
    return addChild( *node );    
}

// removes a child NodePort object indexed by name

Node::error Node::removePort( const std::string & name )
{
    NodePort * port = getPort( name );
    if( port == NULL )
        return NOT_FOUND;
    return removePort( *port );
}

// removes a child NodePort object by index

Node::error Node::removePort( unsigned int index )
{
    NodePort * port = getPort( index );
    if( port == NULL )
        return NOT_FOUND;
    return removePort( *port );
}

Node::error Node::removePort( NodePort & port)
{
    return removeChild((Node &) port );
}

// updates any observers ( the parent and the references ) 

void Node::updateObservers( State &data )
{
	if( isEventGenerator() == 1 || isNodePort() == 1 )
	{
		const DOM_Node & parentElement = parent->getParentNode();
		if( parentElement != 0 )
		{
			((Node *)parentElement.getUserData())->onEventGenerated( data, *this );
		}
		for( NodeVector::iterator it = references.begin(); it != references.end(); it++ )
		{
			(*it)->onEventGenerated( data, *this );
		}
	}
}
