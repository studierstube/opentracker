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
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Node.cxx,v 1.22 2003/02/18 02:12:51 tamer Exp $
  * 
  * @file                                                                   */  
 /* ======================================================================= */

#include <cstdio>
#include <iostream>

#include <memory>
#include <algorithm>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include "Node.h"
#include "StringTable.h"
#include "Context.h"

using namespace std;
XERCES_CPP_NAMESPACE_USE

// emtpy string to be returned, if key is not in the map
// the object itself is part of StringTable.cxx
extern const string empty("");

const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull };

// constructor
Node::Node() : name("")
{
	parent = NULL;	
}

// destructor

Node::~Node()
{
	references.clear();
    if( parent != NULL )
        parent->release();
}

// sets the DOM_Element this node belongs to ( not the parent in OT )

void Node::setParent( DOMElement * parElement )
{
	parent = parElement;
	parent->setUserData( ud_node, this, NULL );
    auto_ptr<char> temp( XMLString::transcode(parent->getTagName()));
    type = temp.get();
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
    DOMNode * parentElement = parent->getParentNode();
	if( parentElement != 0 )
	{
		return (Node *)parentElement->getUserData(ud_node);
	}
    return NULL;
}

// returns the number of children

unsigned int Node::countChildren()
{
    DOMNode * node = parent->getFirstChild();
    unsigned int count = 0;
    while( node != NULL )
    {
        Node * myNode = (Node *)node->getUserData(ud_node);
        if( myNode != NULL )
        {
            if( myNode->isNodePort() == 0 )
                count ++;
        }
        node = node->getNextSibling();
    }
    return count;
}

// adds a new child

Node::error Node::addChild(Node & child)
{
    try {
        parent->appendChild( child.parent );
    }
    catch( DOMException e )
    {
        return GRAPH_CONSTRAINT;
    }
    return OK;
}

// removes a child

Node::error Node::removeChild(Node & child)
{
    try {
        parent->removeChild( child.parent );
    }
    catch( DOMException e )
    {
        return NOT_FOUND;
    }
    return OK;
}

// iterates through the children by returning the child by index

Node * Node::getChild( unsigned int index )
{
    Node * myNode = NULL;
    DOMNode * node = parent->getFirstChild();
    while( node != NULL )
    {
        myNode = (Node *)node->getUserData(ud_node);
        if( myNode != NULL )		
            if( myNode->isNodePort() == 0 )
				if( index == 0 )
					return myNode;
				else 
					index--;
        node = node->getNextSibling();                
    }
    return NULL;
}

// returns number of NodePorts present on this Node

unsigned int Node::countPorts()
{
    Node * myNode = NULL;
    unsigned int count = 0;
    DOMNode * node = parent->getFirstChild();
    while( node != NULL )
    {
        myNode = (Node *)node->getUserData(ud_node);
        if( myNode != NULL )		
            if( myNode->isNodePort() == 1 )
                count++;
        node = node->getNextSibling();                
    }
	return count;
}

// returns a NodePort child object indexed by Name

NodePort * Node::getPort( const string & name )
{
    auto_ptr<XMLCh> temp( XMLString::transcode( name.c_str()));
	//auto_ptr<DOMNodeList> list( parent->getElementsByTagName( temp.get() ));
    DOMNodeList * list = parent->getElementsByTagName( temp.get() );
    if( list->getLength() > 0 )
	{
		DOMElement * portElement = (DOMElement *) list->item(0);
		Node * port = (Node *)portElement->getUserData(ud_node);
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
    DOMNode * node = parent->getFirstChild();
    while( NULL != node )
    {
        myNode = (Node *)node->getUserData(ud_node);
        if( myNode != NULL )		
            if( myNode->isNodePort() == 1 )
				if( index == 0 )
					return (NodePort *) myNode;
				else 
					index--;
        node = node->getNextSibling();                
    }
    return NULL;
}

// creates and adds a new child NodePort object of the given name.

Node::error Node::addPort( const std::string & name )
{
    auto_ptr<XMLCh> temp( XMLString::transcode( name.c_str()));
	//auto_ptr<DOMNodeList> list( parent->getElementsByTagName( temp.get() ));
    DOMNodeList * list = parent->getElementsByTagName( temp.get() );
    if( list->getLength() > 0 )
    {
        return GRAPH_CONSTRAINT;
    }
    DOMDocument * doc = parent->getOwnerDocument();    
    Context * context = (Context *)doc->getUserData( ud_node );
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
		DOMNode * parentElement = parent->getParentNode();
		if( NULL != parentElement  )
		{
			((Node *)parentElement->getUserData( ud_node ))->onEventGenerated( data, *this );
		}
		for( NodeVector::iterator it = references.begin(); it != references.end(); it++ )
		{
			(*it)->onEventGenerated( data, *this );
		}
	}
}

// returns a value to a given key

string Node::get( const string & key )
{
    auto_ptr<XMLCh> temp ( XMLString::transcode( key.c_str() ));
    const XMLCh * res = parent->getAttribute( temp.get());
    auto_ptr<char> cres ( XMLString::transcode( res ));
    return  string( cres.get() );
}

// stores a key, value pair

void Node::put( const string & key, const string & value )
{
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    auto_ptr<XMLCh> tempValue ( XMLString::transcode( value.c_str()));
    parent->setAttribute( tempKey.get(), tempValue.get());
}

// removes a key, value pair

void Node::remove( const string & key )
{
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    parent->removeAttribute( tempKey.get() );
}

// some put and get methods

void Node::put(const string & key, const int value)
{
    char buffer[20];
    
    sprintf( buffer, "%i", value );
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    auto_ptr<XMLCh> tempValue ( XMLString::transcode( buffer ));
    parent->setAttribute( tempKey.get(), tempValue.get());
}

void Node::put(const string & key, const float value)
{
    char buffer[20];
    
    sprintf( buffer, "%f", value );
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    auto_ptr<XMLCh> tempValue ( XMLString::transcode( buffer ));
    parent->setAttribute( tempKey.get(), tempValue.get());
}

void Node::put(const string & key, const double value)
{
    char buffer[30];
    
    sprintf( buffer, "%lf", value );
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    auto_ptr<XMLCh> tempValue ( XMLString::transcode( buffer ));
    parent->setAttribute( tempKey.get(), tempValue.get());
}

void Node::put(const string & key, const int * value, int len)
{
    char buffer[20];
    string strvalue;
    
    sprintf(buffer, "%i", value[0] );
    strvalue.append(buffer);
    for( int i = 1; i < len; i++ )
    {
        sprintf(buffer, " %i", value[i] );
        strvalue.append(buffer);
    }
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    auto_ptr<XMLCh> tempValue ( XMLString::transcode( buffer ));
    parent->setAttribute( tempKey.get(), tempValue.get());
}

void Node::put(const string & key, const float * value, int len)
{
    char buffer[20];
    string strvalue;
    
    sprintf(buffer, "%f", value[0] );
    strvalue.append(buffer);
    for( int i = 1; i < len; i++ )
    {
        sprintf(buffer, " %f", value[i] );
        strvalue.append(buffer);
    }
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    auto_ptr<XMLCh> tempValue ( XMLString::transcode( buffer ));
    parent->setAttribute( tempKey.get(), tempValue.get());
}

void Node::put(const string & key, const double * value, int len)
{
    char buffer[20];
    string strvalue;
    
    sprintf(buffer, "%lf", value[0] );
    strvalue.append(buffer);
    for( int i = 1; i < len; i++ )
    {
        sprintf(buffer, " %lf", value[i] );
        strvalue.append(buffer);
    }
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    auto_ptr<XMLCh> tempValue ( XMLString::transcode( buffer ));
    parent->setAttribute( tempKey.get(), tempValue.get());
}

int Node::get(const string & key, int * value, int len )
{
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    const XMLCh * val = parent->getAttribute( tempKey.get() );
    auto_ptr<char> data ( XMLString::transcode( val ));
    char * start = data.get();
    char * end = data.get();
    int count = 0;
    value[count++] = strtol( start, &end, 0 );    
    while( end != start && count < len){        
        start = end;
        value[count++] = strtol( start, &end, 0 );
    }
    return count;
}

int Node::get(const string & key, float * value, int len )
{
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    const XMLCh * val = parent->getAttribute( tempKey.get() );
    auto_ptr<char> data ( XMLString::transcode( val ));
    char * start = data.get();
    char * end = data.get();
    int count = 0;
    value[count++] = strtod( start, &end );    
    while( end != start && count < len){        
        start = end;
        value[count++] = strtod( start, &end );
    }
    return count;
}

int Node::get(const string & key, double * value, int len )
{
    auto_ptr<XMLCh> tempKey ( XMLString::transcode( key.c_str() ));
    const XMLCh * val = parent->getAttribute( tempKey.get() );
    auto_ptr<char> data ( XMLString::transcode( val ));
    char * start = data.get();
    char * end = data.get();
    int count = 0;
    value[count++] = strtod( start, &end );    
    while( end != start && count < len){        
        start = end;
        value[count++] = strtod( start, &end );
    }
    return count;        
}
