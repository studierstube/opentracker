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
 /* ======================================================================= */

#include "Node.h"

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
	// DOM_NodeList list = parent->getChildNodes();
	// return list.getLength();
    DOM_Node node = parent->getFirstChild();
    unsigned int count = 0;
    while( !node.isNull())
    {
        Node * myNode = (Node *)node.getUserData();
        if( myNode != NULL )
        {
            if( myNode->isWrapperNode() == 0 )
                count ++;
        }
        node = node.getNextSibling();
    }
    return count;
}

// adds a new child

void Node::addChild(Node & child)
{
//    try {
        parent->appendChild( *(child.parent));
/*    }
    catch( DOMException& e )
    {}*/
}

// removes a child

void Node::removeChild(Node & child)
{
    try {
        parent->removeChild( *(child.parent));
    }
    catch( DOM_DOMException& e )
    {}
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
            if( myNode->isWrapperNode() == 0 )
				if( index == 0 )
					return myNode;
				else 
					index--;
        node = node.getNextSibling();                
    }
    return NULL;
}

// returns number of wrapped children by name

unsigned int Node::countWrappedChildren( const string & name )
{
	DOM_NodeList list = parent->getElementsByTagName( name.c_str() );
	if( list.getLength() > 0 )
	{
		DOM_Element wrapElement = (DOM_Element &) list.item(0);
		Node * wrapper = (Node *)wrapElement.getUserData();
		if( wrapper->isWrapperNode() == 1 )
		{
			return wrapElement.getChildNodes().getLength();
		}
	}
	return 0;
}

// iterates through the children by returning the child by index and name

Node * Node::getWrappedChild( const string & name, unsigned int index )
{
	DOM_NodeList list = parent->getElementsByTagName( name.c_str() );
	if( list.getLength() > 0 )
	{
		DOM_Element wrapElement = (DOM_Element &) list.item(0);
		Node * wrapper = (Node *)wrapElement.getUserData();
		if( wrapper->isWrapperNode() == 1 )
		{
			DOM_NodeList wrapped = wrapElement.getChildNodes();
			if( index < wrapped.getLength() )
			{
				return (Node *)wrapped.item( index ).getUserData();
			}
		}
	}
	return NULL;
}

// adds a child to a marked input

void Node::addWrappedChild(const string & name, Node & child)
{
    DOM_NodeList list = parent->getElementsByTagName( name.c_str() );
	if( list.getLength() > 0 )
	{
		DOM_Element wrapElement = (DOM_Element &) list.item(0);
		try {
            wrapElement.appendChild( *(child.parent));
        }
        catch( DOM_DOMException& e )
        {}
	}
}

// removes a child from a marked input

void Node::removeWrappedChild(const string & name, Node & child)
{
    DOM_NodeList list = parent->getElementsByTagName( name.c_str() );
	if( list.getLength() > 0 )
	{
		DOM_Element wrapElement = (DOM_Element &) list.item(0);
		try {
            wrapElement.removeChild( *(child.parent));
        }
        catch( DOM_DOMException& e )
        {}
	}
}

// updates any observers ( the parent and the references ) 

void Node::updateObservers( State &data )
{
	if( isEventGenerator() == 1 || isWrapperNode() == 1 )
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
