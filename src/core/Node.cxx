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
  * $Id$
  * 
  * @file                                                                   */  
 /* ======================================================================= */

#include <stdio.h>
#include <iostream>

#include <memory>
#include <algorithm>

// selects between usage of XERCES and TinyXML
#include "../tool/XMLSelection.h"

#ifdef USE_XERCES
  #include <xercesc/dom/DOM.hpp>
  #include <xercesc/util/XMLString.hpp>
  #include <xercesc/util/XMLUniDefs.hpp>
#endif //USE_XERCES

#include "../OpenTracker.h"
#include "Node.h"
#include "StringTable.h"
#include "Context.h"

using namespace std;

#ifdef USE_XERCES
  #define ELEMENT(a) ((XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *)a)
  XERCES_CPP_NAMESPACE_USE
#endif //USE_XERCES

#ifdef USE_TINYXML
  #define ELEMENT(a) ((TiXmlElement*)a)
#endif //USE_TINYXML

namespace ot {


// emtpy string to be returned, if key is not in the map
// the object itself is part of StringTable.cxx
extern const string empty("");

#ifdef USE_XERCES
  const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull }; 
#endif //USE_XERCES


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
	{
#ifdef USE_XERCES
		try {
			ELEMENT(parent)->release();
		}
		catch(DOMException &)
		{
		}
#endif //USE_XERCES

#ifdef USE_TINYXML
		delete ELEMENT(parent);
#endif //USE_TINYXML
	}
}

// sets the DOM_Element this node belongs to ( not the parent in OT )

void Node::setParent( void * parElement )
{
#ifdef USE_XERCES
	parent = parElement;
	ELEMENT(parent)->setUserData( ud_node, this, NULL );
    char * temp = XMLString::transcode(ELEMENT(parent)->getLocalName());
    type = temp;
	XMLString::release( &temp );
#endif //USE_XERCES

#ifdef USE_TINYXML
	parent = parElement;
	ELEMENT(parent)->SetUserData(this);
    const char* temp = ELEMENT(parent)->Value();
    type = temp;
#endif //USE_TINYXML
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
#ifdef USE_XERCES
    DOMNode * parentElement = ELEMENT(parent)->getParentNode();
	if( parentElement != 0 )
	{
		return (Node *)parentElement->getUserData(ud_node);
	}
    return NULL;
#endif //USE_XERCES

#ifdef USE_TINYXML
   TiXmlNode * parentElement = ELEMENT(parent)->Parent();
	if( parentElement != 0 )
	{
		return (Node *)parentElement->GetUserData();
	}
    return NULL;
#endif //USE_TINYXML
}

// returns the Context this node lives in

Context * Node::getContext() const
{    
#ifdef USE_XERCES
    DOMDocument * doc = ELEMENT(parent)->getOwnerDocument();
    if( doc != 0)
    {
        return (Context *)doc->getUserData(ud_node);
    }
    return NULL;
#endif //USE_XERCES

#ifdef USE_TINYXML
    TiXmlDocument * doc = ELEMENT(parent)->GetDocument();
    if( doc != 0)
    {
        return (Context *)doc->GetUserData();
    }
    return NULL;
#endif //USE_TINYXML
}

// returns the number of children

unsigned int Node::countChildren()
{
#ifdef USE_XERCES
    DOMNode * node = ELEMENT(parent)->getFirstChild();
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
#endif //USE_XERCES

#ifdef USE_TINYXML
    TiXmlNode * node = ELEMENT(parent)->FirstChild();
    unsigned int count = 0;
    while( node != NULL )
    {
        Node * myNode = (Node *)node->GetUserData();
        if( myNode != NULL )
        {
            if( myNode->isNodePort() == 0 )
                count ++;
        }
        node = node->NextSibling();
    }
    return count;
#endif //USE_TINYXML
}

// adds a new child

Node::error Node::addChild(Node & child)
{
#ifdef USE_XERCES
    try {
        ELEMENT(parent)->appendChild( ELEMENT(child.parent));
    }
    catch( DOMException e )
    {
        return GRAPH_CONSTRAINT;
    }
    return OK;
#endif //USE_XERCES

#ifdef USE_TINYXML
    ELEMENT(parent)->InsertEndChild( *ELEMENT(child.parent));
    return OK;
#endif //USE_TINYXML
}

// removes a child

Node::error Node::removeChild(Node & child)
{
#ifdef USE_XERCES
    try {
        ELEMENT(parent)->removeChild( ELEMENT(child.parent));
    }
    catch( DOMException e )
    {
        return NOT_FOUND;
    }
    return OK;
#endif //USE_XERCES

#ifdef USE_TINYXML
    ELEMENT(parent)->RemoveChild( ELEMENT(child.parent));
    return OK;
#endif //USE_TINYXML
}

// iterates through the children by returning the child by index

Node * Node::getChild( unsigned int index )
{
#ifdef USE_XERCES
    Node * myNode = NULL;
    DOMNode * node = ELEMENT(parent)->getFirstChild();
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
#endif //USE_XERCES

#ifdef USE_TINYXML
    Node * myNode = NULL;
    TiXmlNode * node = ELEMENT(parent)->FirstChild();
    while( node != NULL )
    {
        myNode = (Node *)node->GetUserData();
        if( myNode != NULL )		
            if( myNode->isNodePort() == 0 )
				if( index == 0 )
					return myNode;
				else 
					index--;
        node = node->NextSibling();                
    }
    return NULL;
#endif //USE_TINYXML
}

// returns number of NodePorts present on this Node

unsigned int Node::countPorts()
{
#ifdef USE_XERCES
    Node * myNode = NULL;
    unsigned int count = 0;
    DOMNode * node = ELEMENT(parent)->getFirstChild();
    while( node != NULL )
    {
        myNode = (Node *)node->getUserData(ud_node);
        if( myNode != NULL )		
            if( myNode->isNodePort() == 1 )
                count++;
        node = node->getNextSibling();                
    }
	return count;
#endif //USE_XERCES

#ifdef USE_TINYXML
    Node * myNode = NULL;
    unsigned int count = 0;
    TiXmlNode * node = ELEMENT(parent)->FirstChild();
    while( node != NULL )
    {
        myNode = (Node *)node->GetUserData();
        if( myNode != NULL )		
            if( myNode->isNodePort() == 1 )
                count++;
        node = node->NextSibling();                
    }
	return count;
#endif //USE_TINYXML
}

// returns a NodePort child object indexed by Name

NodePort * Node::getPort( const string & name )
{
#ifdef USE_XERCES
    XMLCh * temp = XMLString::transcode( name.c_str());
    XMLCh * xmlspace = XMLString::transcode(getContext()->getRootNamespace().c_str());
    DOMNodeList * list = ELEMENT(parent)->getElementsByTagNameNS( xmlspace, temp );
	XMLString::release( &temp );
	XMLString::release( &xmlspace );
    if( list->getLength() > 0 )
	{
		DOMElement * portElement = (DOMElement *) list->item(0);
		Node * port = (Node *)portElement->getUserData(ud_node);
        if( port != NULL )
            if( port->isNodePort() == 1 )
                return (NodePort *)port;
	}
    return NULL;
#endif //USE_XERCES

#ifdef USE_TINYXML
    const char* temp = name.c_str();
    const char* xmlspace = getContext()->getRootNamespace().c_str();
	TiXmlNode * currentNode = NULL;

	while(currentNode = ELEMENT(parent)->IterateChildren(temp, currentNode))
	{
		Node * port = (Node *)currentNode->GetUserData();
        if( port != NULL )
            if( port->isNodePort() == 1 )
                return (NodePort *)port;
	}

    return NULL;
#endif //USE_TINYXML
}

// returns a NodePort child object by index

NodePort * Node::getPort( unsigned int index )
{
#ifdef USE_XERCES
    Node * myNode = NULL;
    DOMNode * node = ELEMENT(parent)->getFirstChild();
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
#endif //USE_XERCES

#ifdef USE_TINYXML
    Node * myNode = NULL;
    TiXmlNode * node = ELEMENT(parent)->FirstChild();
    while( NULL != node )
    {
        myNode = (Node *)node->GetUserData();
        if( myNode != NULL )		
            if( myNode->isNodePort() == 1 )
				if( index == 0 )
					return (NodePort *) myNode;
				else 
					index--;
        node = node->NextSibling();                
    }
    return NULL;
#endif //USE_TINYXML
}

// creates and adds a new child NodePort object of the given name.

Node::error Node::addPort( const std::string & name )
{
#ifdef USE_XERCES
	XMLCh * temp = XMLString::transcode( name.c_str());
    XMLCh * xmlspace = XMLString::transcode(getContext()->getRootNamespace().c_str());
    DOMNodeList * list = ELEMENT(parent)->getElementsByTagNameNS( xmlspace, temp );
	XMLString::release( &temp );
	XMLString::release( &xmlspace );
    if( list->getLength() > 0 )
    {
        return GRAPH_CONSTRAINT;
    }
    DOMDocument * doc = ELEMENT(parent)->getOwnerDocument();    
    Context * context = (Context *)doc->getUserData( ud_node );
    StringTable table;
    Node * node = context->createNode( name, table);
    if( node == NULL )
        return GRAPH_CONSTRAINT;
    return addChild( *node );    
#endif //USE_XERCES

#ifdef USE_TINYXML
    const char* temp = name.c_str();
    const char* xmlspace = getContext()->getRootNamespace().c_str();
	TiXmlNode * currentNode = NULL;

	if(ELEMENT(parent)->FirstChild(temp))
		return GRAPH_CONSTRAINT;

    TiXmlDocument * doc = ELEMENT(parent)->GetDocument();    
    Context * context = (Context *)doc->GetUserData();
    StringTable table;
    Node * node = context->createNode( name, table);
    if( node == NULL )
        return GRAPH_CONSTRAINT;
    return addChild( *node );    
#endif //USE_TINYXML
}

// removes a child NodePort object indexed by name

Node::error Node::removePort( const std::string & name )
{
#ifdef USE_XERCES
    NodePort * port = getPort( name );
    if( port == NULL )
        return NOT_FOUND;
    return removePort( *port );
#endif //USE_XERCES

#ifdef USE_TINYXML
    NodePort * port = getPort( name );
    if( port == NULL )
        return NOT_FOUND;
    return removePort( *port );
#endif //USE_TINYXML
}

// removes a child NodePort object by index

Node::error Node::removePort( unsigned int index )
{
#ifdef USE_XERCES
    NodePort * port = getPort( index );
    if( port == NULL )
        return NOT_FOUND;
    return removePort( *port );
#endif //USE_XERCES

#ifdef USE_TINYXML
    NodePort * port = getPort( index );
    if( port == NULL )
        return NOT_FOUND;
    return removePort( *port );
#endif //USE_TINYXML
}

Node::error Node::removePort( NodePort & port)
{
    return removeChild((Node &) port );
}

// updates any observers ( the parent and the references ) 

void Node::updateObservers( State &data )
{
#ifdef USE_XERCES
	if( isEventGenerator() == 1 || isNodePort() == 1 )
	{
		DOMNode * parentElement = ELEMENT(parent)->getParentNode();
		if( NULL != parentElement  )
		{
			((Node *)parentElement->getUserData( ud_node ))->onEventGenerated( data, *this );
		}
		for( NodeVector::iterator it = references.begin(); it != references.end(); it++ )
		{
			(*it)->onEventGenerated( data, *this );
		}
	}
#endif //USE_XERCES

#ifdef USE_TINYXML
	if( isEventGenerator() == 1 || isNodePort() == 1 )
	{
		TiXmlNode * parentElement = ELEMENT(parent)->Parent();
		if( NULL != parentElement  )
		{
			((Node *)parentElement->GetUserData())->onEventGenerated( data, *this );
		}
		for( NodeVector::iterator it = references.begin(); it != references.end(); it++ )
		{
			(*it)->onEventGenerated( data, *this );
		}
	}
#endif //USE_TINYXML
}

// returns a value to a given key

string Node::get( const string & key )
{
#ifdef USE_XERCES
    XMLCh * temp = XMLString::transcode( key.c_str() );
    XMLCh * xmlspace = XMLString::transcode(getContext()->getRootNamespace().c_str());
    const XMLCh * res = ELEMENT(parent)->getAttributeNS( xmlspace, temp );
	XMLString::release( &temp );
	XMLString::release( &xmlspace );
    char * cres = XMLString::transcode( res );
	string result( cres );
	XMLString::release( &cres );
    return string( cres );
#endif //USE_XERCES

#ifdef USE_TINYXML
    const char* temp  = key.c_str();
    const char* xmlspace = getContext()->getRootNamespace().c_str();
    const char* res = ELEMENT(parent)->Attribute(temp);
    return  string( res );
#endif //USE_TINYXML
}

// stores a key, value pair

void Node::put( const string & key, const string & value )
{
#ifdef USE_XERCES
    XMLCh * tempKey = XMLString::transcode( key.c_str() );
    XMLCh * tempValue = XMLString::transcode( value.c_str());
    XMLCh * xmlspace = XMLString::transcode(getContext()->getRootNamespace().c_str());
    ELEMENT(parent)->setAttributeNS( xmlspace, tempKey, tempValue );
	XMLString::release( & tempKey );
	XMLString::release( & tempValue );
	XMLString::release( & xmlspace );
#endif //USE_XERCES

#ifdef USE_TINYXML
    const char* tempKey = key.c_str();
    const char* tempValue = value.c_str();
    const char* xmlspace = getContext()->getRootNamespace().c_str();
    ELEMENT(parent)->SetAttribute( tempKey, tempValue);
#endif //USE_TINYXML
}

// removes a key, value pair

void Node::remove( const string & key )
{
#ifdef USE_XERCES
    XMLCh * tempKey = XMLString::transcode( key.c_str() );
    XMLCh * xmlspace = XMLString::transcode(getContext()->getRootNamespace().c_str());
    ELEMENT(parent)->removeAttributeNS( xmlspace, tempKey );
	XMLString::release( & tempKey );
	XMLString::release( & xmlspace );
#endif //USE_XERCES

#ifdef USE_TINYXML
    const char* tempKey = key.c_str();
    const char* xmlspace = getContext()->getRootNamespace().c_str();
    ELEMENT(parent)->RemoveAttribute( tempKey );
#endif //USE_TINYXML
}

// some put and get methods

void Node::put(const string & key, const int value)
{
    char buffer[20];
    
    sprintf( buffer, "%i", value );
	put( key, buffer );
}

void Node::put(const string & key, const float value)
{
    char buffer[20];
    
    sprintf( buffer, "%f", value );
	put( key, buffer );
}

void Node::put(const string & key, const double value)
{
    char buffer[30];
    
    sprintf( buffer, "%lf", value );
	put( key, buffer );
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
	put( key, strvalue );
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
	put( key, strvalue );
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
	put( key, strvalue );
}

int Node::get(const string & key, int * value, int len )
{
	string data = get( key );
    const char * start = data.c_str();
    char * end = (char *)start;
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
	string data = get( key );
	const char * start = data.c_str();
    char * end = (char *)start;
    int count = 0;
    value[count++] = (float)strtod( start, &end );
    while( end != start && count < len){
        start = end;
        value[count++] = (float)strtod( start, &end );
    }
    return count;
}

int Node::get(const string & key, double * value, int len )
{
	string data = get( key );    
	const char * start = data.c_str();
    char * end = (char *)start;
    int count = 0;
    value[count++] = strtod( start, &end );    
    while( end != start && count < len){        
        start = end;
        value[count++] = strtod( start, &end );
    }
    return count;
}

} // namespace ot
