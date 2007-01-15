/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** The source file for the basic Node class.
 *
 * @author Gerhard Reitmayr
 * @author Eduardo Veas
 *
 * $Id$
 *
 * @file                                                                   */
/* ======================================================================= */
 
#include <cstdio>
#include <iostream>

#include <memory>
#include <algorithm>

#include <OpenTracker/OpenTracker.h>

// selects between usage of XERCES and TinyXML
#include <OpenTracker/tool/XMLSelection.h>

#ifdef USE_XERCES
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#endif //USE_XERCES

#include <OpenTracker/core/Node.h>
#ifdef OT_LOCAL_GRAPH
#include <OpenTracker/core/StringTable.h>
#include <OpenTracker/core/Configurator.h>
#endif //OT_LOCAL_GRAPH
#include <OpenTracker/core/Context.h>

#ifdef USE_LIVE
#include <OpenTracker/skeletons/OTGraph.hh>
#endif

//using namespace std;

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
    extern const std::string empty("");

#ifdef USE_XERCES
    const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull };
#endif //USE_XERCES


    // constructor
    Node::Node() : name("")
    {
#ifndef OT_LOCAL_GRAPH
        parent = NULL;
#endif// OT_LOCAL_GRAPH
    }

    // destructor

    Node::~Node()
    {
        references.clear();
#ifndef OT_LOCAL_GRAPH
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
#endif // OT_LOCAL_GRAPH
    }

    // sets the DOM_Element this node belongs to ( not the parent in OT )

    void Node::setParent( void * parElement )
    {
#ifndef OT_LOCAL_GRAPH
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
#endif // OT_LOCAL_GRAPH
    }


    // adds a reference to the node

    void Node::addReference( Node * reference )
    {
        references.push_back( reference );
    }

    // removes a reference from the node

    void Node::removeReference( Node * reference )
    {
        NodeVector::iterator result = std::find( references.begin(), references.end(),
                                                 reference );
        if( result != references.end())
        {
            references.erase( result );
        }
#ifdef USE_LIVE
        // If there are no more references, then remove the DEF attribute
        if (references.size() == 0) {
            remove("DEF");
        }
#endif //USE_LIVE
    }

    // returns a pointer to the parent node

    Node * Node::getParent()
    {
#ifdef OT_LOCAL_GRAPH
        return parents[0];
#else // OT_LOCAL_GRAPH
        
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
#endif //OT_LOCAL_GRAPH        

    }

    // returns the Context this node lives in

    Context * Node::getContext() const
    {
#ifdef OT_LOCAL_GRAPH
        // fix for the moment
        // must be changed for a stronger statement
        return & (Configurator::instance() ->getContext());
#else // OT_LOCAL_GRAPH
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
#endif //OT_LOCAL_GRAPH
    }

    // returns the number of children

    unsigned int Node::countChildren()
    {
#ifdef OT_LOCAL_GRAPH
        unsigned int count = 0;
        for (NodeVector::iterator it = children.begin(); it != children.end(); it++)
            if ((*it)->isNodePort() == 0)
                count ++;

        return count;
#else // OT_LOCAL_GRAPH

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


#endif //OT_LOCAL_GRAPH

    }

    // adds a new child

    Node::error Node::addChild(Node & child)
    {
#ifdef OT_LOCAL_GRAPH
        //        logPrintI("setting the childs %s parent to %p\n", (child.getType()).c_str(),this);
        // add this to the parent list of the child
        child.addParent(this);
        // add the child to the children list of this node
        children.push_back(&child);

        return OK;
#else //OT_LOCAL_GRAPH
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
        TiXmlNode * result = ELEMENT(parent)->InsertEndChild( *ELEMENT(child.parent));
		//child.parent = NULL;
        return OK;
#endif //USE_TINYXML

#endif //OT_LOCAL_GRAPH
    }

    // removes a child

    Node::error Node::removeChild(Node & child)
    {
#ifdef OT_LOCAL_GRAPH
        Node * theNode = NULL;
        for (NodeVector::iterator it = children.begin(); it != children.end(); it++){
            if ((*it) == (&child)){
                children.erase(it);
                theNode = (*it);
            }
        }

        if (theNode)
            return OK;
        else
            return NOT_FOUND;
#else //OT_LOCAL_GRAPH            
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
//		child.parent = NULL;
        return OK;
#endif //USE_TINYXML

#endif //OT_LOCAL_GRAPH
    }

    // iterates through the children by returning the child by index

    Node * Node::getChild( unsigned int index )
    {
#ifdef OT_LOCAL_GRAPH
        Node * result=NULL;
        for (int i = 0; i < (int) children.size(); i++){
            Node * tmp = children[i];
            if (tmp->isNodePort() == 0){
                if (index == 0){
                    result = tmp;
                    break;
                }else index --;
            }
        }
        return result;
#else  //OT_LOCAL_GRAPH
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

#endif //OT_LOCAL_GRAPH
    }

    // returns number of NodePorts present on this Node

    unsigned int Node::countPorts()
    {
#ifdef OT_LOCAL_GRAPH
        unsigned int result = 0;
        for (NodeVector::iterator it = children.begin(); it != children.end(); it ++)
            if ((*it)->isNodePort() == 1)
                result++;

        return result;
#else //OT_LOCAL_GRAPH
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
#endif //OT_LOCAL_GRAPH
    }
            

    // returns a NodePort child object indexed by Name and index

    NodePort * Node::getPort( const std::string & name, unsigned int index )
    {
#ifdef OT_LOCAL_GRAPH
        for (NodeVector::iterator it = children.begin(); it != children.end(); it++){
            if ((*it)->isNodePort() == 1){
                if (name.compare((*it)->getName()) == 0){
                    if (index == 0)
                        return (NodePort *) ((Node*) *it);
                    else index --;
                }
            }
        }
        return NULL;
#else //OT_LOCAL_GRAPH
#ifdef USE_XERCES
        XMLCh * temp = XMLString::transcode( name.c_str());
        XMLCh * xmlspace = XMLString::transcode(getContext()->getRootNamespace().c_str());
        DOMNodeList * list = ELEMENT(parent)->getElementsByTagNameNS( xmlspace, temp );
        XMLString::release( &temp );
        XMLString::release( &xmlspace );
        if( list->getLength() > index )
        {
            DOMElement * portElement = (DOMElement *) list->item(index);
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
					// since there might be more than one Child with the same name
					// we use the index to select the one we need.
					if( index == 0 )
                        return (NodePort *) port;
                    else
                        index--;
            
        }

        return NULL;
        #endif //USE_TINYXML
#endif //OT_LOCAL_GRAPH
    }

    // returns a NodePort child object by index

    NodePort * Node::getPort( unsigned int index )
    {
#ifdef OT_LOCAL_GRAPH
        NodePort * result = NULL;
        for (int i = 0; i < (int)children.size(); i ++){
            Node * tmp = children[i];
            if (tmp->isNodePort() == 1)
                if (index == 0){
                    result = (NodePort*) tmp;
                    break;
                } else index--;
        }
        return result;
    }
#else //OT_LOCAL_GRAPH
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
#endif //OT_LOCAL_GRAPH
    // creates and adds a new child NodePort object of the given name.


    Node::error Node::addPort( const std::string & name )
    {
#ifdef OT_LOCAL_GRAPH
        // To create the node port, we can use any instance of the context
        StringTable table;
        Context * ctx = getContext();
        Node * node = ctx->createNode( name, table );
        if (node == NULL) 
            return GRAPH_CONSTRAINT;
        else
            return addChild(*node);
#else ////OT_LOCAL_GRAPH       
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
#endif //OT_LOCAL_GRAPH
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

    void Node::updateObservers( Event &data )
    {
#ifdef OT_LOCAL_GRAPH
        if (isEventGenerator() == 1 || isNodePort() == 1){
            for (NodeVector::iterator it = parents.begin() ; it != parents.end() ; it++){
                (*it)->onEventGenerated(data, *this);
            }

            for (NodeVector::iterator it = references.begin(); it != references.end() ; it++){

                (*it)->onEventGenerated(data, *this);
            }
        }
#else //OT_LOCAL_GRAPH
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
#endif //OT_LOCAL_GRAPH
    }

    // returns a value to a given key

    const std::string Node::get( const std::string & key ) const
    {
#ifdef OT_LOCAL_GRAPH
        Node * self = const_cast<Node*>(this);
        const std::string & result =  self->attributes.get(key) ;

        return result;
#else  //OT_LOCAL_GRAPH
#ifdef USE_XERCES
        XMLCh * temp = XMLString::transcode( key.c_str() );
        XMLCh * xmlspace = XMLString::transcode(getContext()->getRootNamespace().c_str());
        const XMLCh * res = ELEMENT(parent)->getAttributeNS( xmlspace, temp );
        XMLString::release( &temp );
        XMLString::release( &xmlspace );
        char * cres = XMLString::transcode( res );
        std::string result( cres );
        XMLString::release( &cres );
        return result;
#endif //USE_XERCES

#ifdef USE_TINYXML
        const char* temp  = key.c_str();
        const char* xmlspace = getContext()->getRootNamespace().c_str();
        const char* res = ELEMENT(parent)->Attribute(temp);
        return  std::string( res );
        #endif //USE_TINYXML
#endif //OT_LOCAL_GRAPH
    }

    // stores a key, value pair

    void Node::put( const std::string & key, const std::string & value )
    {
#ifdef OT_LOCAL_GRAPH
        if (key.compare("OtNodeType") == 0){

            this->type = value;
        } else
            attributes.put(key, value);
#else //OT_LOCAL_GRAPH
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
#endif //OT_LOCAL_GRAPH
    }

    // removes a key, value pair

    void Node::remove( const std::string & key )
    {
#ifdef OT_LOCAL_GRAPH
        attributes.remove(key);
#else //OT_LOCAL_GRAPH
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
#endif //OT_LOCAL_GRAPH
    }

    // some put and get methods

    void Node::put(const std::string & key, const int value)
    {
        char buffer[20];

        sprintf( buffer, "%i", value );
        put( key, buffer );
    }

    void Node::put(const std::string & key, const float value)
    {
        char buffer[20];

        sprintf( buffer, "%f", value );
        put( key, buffer );
    }

    void Node::put(const std::string & key, const double value)
    {
        char buffer[30];

        sprintf( buffer, "%lf", value );
        put( key, buffer );
    }

    void Node::put(const std::string & key, const int * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%i", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %i", value[i] );
            strvalue.append(buffer);
        }
        put( key, strvalue );
    }

    void Node::put(const std::string & key, const float * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%f", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %f", value[i] );
            strvalue.append(buffer);
        }
        put( key, strvalue );
    }

    void Node::put(const std::string & key, const double * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%lf", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %lf", value[i] );
            strvalue.append(buffer);
        }
        put( key, strvalue );
    }

    int Node::get(const std::string & key, int * value, int len ) 
    {
        std::string data = get( key );
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

    int Node::get(const std::string & key, float * value, int len ) 
    {
        std::string data = get( key );
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

    int Node::get(const std::string & key, double * value, int len ) 
    {
        std::string data = get( key );
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

#ifdef OT_LOCAL_GRAPH
    IMPLEMENT_IREFCOUNTED(Node);

    // add one parent node
    void Node::addParent( Node * parent ){
        parents.push_back( parent );
    }

    Node * Node::findNode(const std::string & key, const std::string & val){
        Node * result = NULL;

        if (attributes.containsKey(key)){
            const std::string & value = attributes.get(key);
            if (value.compare(val) == 0)
                result = this;
        }
        // search the children
        if (result == NULL){
            for (int i = 0; i < (int)children.size() ; i++){
                result = children[i] ->findNode(key, val);
            }
        }

        return result;
    };

    unsigned int Node::countAllChildren(){
        return (unsigned int) children.size();
    };

    /*
     * returns a child at index, no matter whether it is a wrapper or not
     * @return unsigned number of children */
    Node * Node::getAnyChild( unsigned int index){
        if (index >= children.size())
            return NULL;
        return children[(int)index];
    };


StringTable & Node::getAttributes(){
    return attributes;
};
#endif OT_LOCAL_GRAPH

#ifdef USE_LIVE
    char* Node::get_type() {
        return CORBA::string_dup(getType().c_str());
    }

    char* Node::get_name() {
        return CORBA::string_dup(getName().c_str());
    }

    char* Node::get_id() {
        return CORBA::string_dup(get("ID").c_str());
    }

    char* Node::get_attribute(const char* _key) {
        std::string key(CORBA::string_dup(_key));
        return CORBA::string_dup(get(key).c_str());
    }

    OTGraph::StringTable* Node::get_attributes() {
        StringTable* value = new StringTable;
        //DOMElement * element = ELEMENT(parent);
        DOMElement * element =  (DOMElement* ) parent;
        DOMNamedNodeMap * map = element->getAttributes();
        for( unsigned int i = 0; i < map->getLength(); i++ )
        {
            DOMAttr * attribute = (DOMAttr *)map->item( i );
            char * nameTemp = XMLString::transcode( attribute->getName());
            char * valueTemp = XMLString::transcode( attribute->getValue());
            value->put(nameTemp, valueTemp );
            XMLString::release( &valueTemp );
            XMLString::release( &nameTemp );
        }
        OTGraph::StringTable_var attributes = value->getStringTable();
        delete value;
        return attributes._retn();
   };



#endif

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of Node.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
