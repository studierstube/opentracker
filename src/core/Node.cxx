 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** The source file for the basic Node class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Node.cxx,v 1.1 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "Node.h"

#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include "algorithm"

#include <dom/DOM_Node.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NamedNodeMap.hpp>

static DOM_Element dummy;

Node::Node()
{
	parent = new DOM_Element();	
}

Node::~Node()
{
	references.clear();
	*parent = 0;
	delete parent;
}

void Node::setParent( DOM_Element & parElement )
{
	*parent = parElement;
	parent->setUserData( this );
}

void Node::addReference( Node * reference )
{
	references.push_back( reference );
}

void Node::removeReference( Node * reference )
{
	NodeVector::iterator result = find( references.begin(), references.end(), 
		reference );
	if( result != references.end())
	{
		references.erase( result );
	}
}

unsigned int Node::countChildren()
{
	DOM_NodeList list = parent->getChildNodes();
	return list.getLength();
}

Node * Node::getChild( unsigned int index )
{
	DOM_NodeList list = parent->getChildNodes();
	if( index < list.getLength())
	{
		return (Node *)list.item( index ).getUserData();
	}
	return NULL;
}

unsigned int Node::countWrappedChildren( string & name )
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

Node * Node::getWrappedChild( string & name, unsigned int index )
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

void Node::updateObservers( State &data )
{
	if( isEventGenerator() == 1 )
	{
		DOM_Node & parentElement = parent->getParentNode();
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