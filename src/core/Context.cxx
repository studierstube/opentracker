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
/** source file for Context class
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */     
 /* ======================================================================= */


#include <ace/OS.h>
#include <ace/FILE.h>

#include <algorithm>

#include "../OpenTracker.h"

// selects between usage of XERCES and TinyXML
#include "../tool/XMLSelection.h"

#ifdef USE_XERCES
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#endif //USE_XERCES

#include "ConfigurationParser.h"


#ifdef USE_XERCES
XERCES_CPP_NAMESPACE_USE;
const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull };
#endif //USE_XERCES

//using namespace std;

// constructor method.

namespace ot {

Context::Context( int init ) :
    rootNode( NULL ),
    cleanUp( false ), 
    rootNamespace( "" )
{
    if( init != 0 )
    {
        initializeContext( *this );
        cleanUp = true;
    }
    else {
        cleanUp = false;
    }
    directories.push_back(".");
}

// Destructor method.
Context::~Context()
{
    if( cleanUp )
    {
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            delete (*it).second;
        }
    }
    modules.clear();
    if (rootNode != NULL) {
        delete rootNode;
    }
}

// adds a new factory to the NodeFactoryContainer

void Context::addFactory(NodeFactory& newfactory)
{
    factory.addFactory( newfactory );
}

// removes a factory from the NodeFactoryContainer

void Context::removeFactory(NodeFactory & oldfactory)
{
    factory.removeFactory( oldfactory );
}

// adds a module to the contexts collection

void Context::addModule(const std::string & name, Module & module)
{
    modules[name] = &module;
    module.context = this;
}

// returns a module indexed by its configuration elements name

Module * Context::getModule(const std::string & name)
{
    ModuleMap::iterator it = modules.find( name );
    if( it != modules.end())    
        return (*it).second;
    return NULL;
}

// removes a module

void Context::removeModule(Module & module)
{
    ModuleMap::iterator it = modules.begin();
    while( it != modules.end())
    {
        if((*it).second == &module )        
            modules.erase( it-- );
        it++;
    }  
}

// calls start on all modules to do some initialization.

void Context::start()
{
    for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        (*it).second->start();
    }
}

// calls close on all modules to close any resources.

void Context::close()
{
    for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        (*it).second->close();
    }
    // HACK: give some threads time to close down
    OSUtils::sleep(1000);
}

// parses the file and builds the tree.

void Context::parseConfiguration(const std::string& filename)
{
#ifdef USE_XERCES
    file = filename;
    std::string::size_type limit = file.find_last_of( "/\\" );
    if( limit != std::string::npos )
        addDirectoryFirst( file.substr(0, limit));
    
    ConfigurationParser parser( *this );
    rootNode = parser.parseConfigurationFile( filename );
    DOMDocument * doc = ((DOMNode *)(rootNode->parent))->getOwnerDocument();
    doc->setUserData( ud_node, this, NULL );

    const XMLCh* xmlspace = ((DOMNode *)(rootNode->parent))->getNamespaceURI();
    if (xmlspace != NULL) {
        char * tempName = XMLString::transcode( xmlspace );
        rootNamespace = tempName;
        XMLString::release( &tempName );
    }
    else {
        rootNamespace = "";
    }
#endif //USE_XERCES


#ifdef USE_TINYXML
    file = filename;
    std::string::size_type limit = file.find_last_of( "/\\" );
    if( limit != std::string::npos )
        addDirectoryFirst( file.substr(0, limit));
    
    ConfigurationParser parser( *this );
    rootNode = parser.parseConfigurationFile( filename );
    TiXmlDocument * doc = ((TiXmlNode *)(rootNode->parent))->GetDocument();
    doc->SetUserData(this);
#endif //USE_TINYXML
}

// calls pullState on all modules to get data out again.

void Context::pullStates()
{
    for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        (*it).second->pullState();
    }
}

// This method calls pushState on all modules to get new data into the shared data tree.

void Context::pushStates()
{
    for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        (*it).second->pushState();
    }
}

// This method implements the main loop and runs until it is stopped somehow.

void Context::run()
{
    start();
    while ( stop() == 0 )
    {
        // push and pull parts of the main loop
        pushStates();
        pullStates();
    }  
    close();   
}

// tests all modules for stopping

int Context::stop()
{
    int value = 0;
    for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        value |= (*it).second->stop();
    }
    return value;
}

// creates a new node from a given element name and an attribute table

Node * Context::createNode( const std::string & name, StringTable & attributes)
{
#ifdef USE_XERCES
    Node * value = factory.createNode( name , attributes );
    if( value != NULL )
    {
        // add a correctly created DOM_Element to the node here and return
        DOMDocument * doc = ((DOMNode *)(rootNode->parent))->getOwnerDocument();
        auto_ptr<XMLCh> tempName ( XMLString::transcode( name.c_str()));
        auto_ptr<XMLCh> tempNS ( XMLString::transcode(rootNamespace.c_str()));
        DOMElement * el = doc->createElementNS( tempNS.get(), tempName.get());
        value->setParent( el );        
        // set attributes on the element node
        KeyIterator keys(attributes);
        while( keys.hasMoreKeys())
        {
            const std::string & key = keys.nextElement();
            value->put( key, attributes.get( key ));
			auto_ptr<XMLCh> attName ( XMLString::transcode( key.c_str()));
			auto_ptr<XMLCh> attVal ( XMLString::transcode( attributes.get( key ).c_str()));
			el->setAttributeNS(tempNS.get(), attName.get(), attVal.get());
        }
    }
    return value;
#endif //USE_XERCES


#ifdef USE_TINYXML
    Node * value = factory.createNode( name , attributes );
    if( value != NULL )
    {
        // add a correctly created DOM_Element to the node here and return
        TiXmlDocument * doc = ((TiXmlNode *)(rootNode->parent))->GetDocument();
        const char * tempName = name.c_str();
        const char * tempNS = rootNamespace.c_str();
        TiXmlElement * el = new TiXmlElement(tempName);
        value->setParent( el );        
        // set attributes on the element node
        KeyIterator keys(attributes);
        while( keys.hasMoreKeys())
        {
            const std::string & key = keys.nextElement();
            value->put( key, attributes.get( key ));
			const char * attName = key.c_str();
			const char * attVal = attributes.get( key ).c_str();
			el->SetAttribute(attName, attVal);
        }
    }
    return value;
#endif //USE_TINYXML
}

#ifdef USE_TINYXML
TiXmlElement *
findElementRecursive(TiXmlElement* element, const std::string & id)
{
	// does this element have the attribute we search for?
	if(element->Attribute(id.c_str()))
		return element;

	// walk through all children
	TiXmlElement* child = element->FirstChildElement();
	while(child)
	{
		if(TiXmlElement * found = findElementRecursive(child, id))
			return found;
		child = child->NextSiblingElement();
	}

	// walk through all siblings
	TiXmlElement* sibling = element->NextSiblingElement();
	while(sibling)
	{
		if(TiXmlElement * found = findElementRecursive(sibling, id))
			return found;
		sibling = sibling->NextSiblingElement();
	}

	// finally give up
	return NULL;
}
#endif //USE_TINYXML

Node * Context::getRootNode()
{
    return rootNode;
}

Node * Context::findNode(const std::string & id)
{
#ifdef USE_XERCES
    // search for the right node via the DOM_Document API
    auto_ptr<XMLCh> tempId ( XMLString::transcode( id.c_str()));
    DOMElement * el = ((DOMNode *)(rootNode->parent))->getOwnerDocument()->getElementById( tempId.get());
    if( el != 0 )
        return (Node *)el->getUserData(ud_node);
    return NULL;
#endif //USE_XERCES


#ifdef USE_TINYXML
	TiXmlElement* el = findElementRecursive(((TiXmlNode*)rootNode->parent)->GetDocument()->RootElement(), id);
    if( el != 0 )
        return (Node *)el->GetUserData();
    return NULL;
#endif //USE_TINYXML
}

// add a directory to the front of the directory stack 

void Context::addDirectoryFirst( const std::string & dir )
{
    if( std::find( directories.begin(), directories.end(), dir) == directories.end())
    {
        directories.insert(directories.begin(), dir);
    }
}

// add a directory to the end of the directory stack 

void Context::addDirectoryLast( const std::string & dir )
{
    if( std::find( directories.begin(), directories.end(), dir) == directories.end())
    {
        directories.push_back(dir);
    }    
}

// remove a directory from the directory stack 

void Context::removeDirectory( const std::string & dir )
{
    std::vector<std::string>::iterator it = std::find( directories.begin(), directories.end(), dir);
    if( it != directories.end())
    {
        directories.erase(it);
    }        
}

// searches for a file by prepending a stack of directory names

bool Context::findFile( const std::string & filename, std::string & fullname )
{
    ACE_stat stat;
    if( filename.at(0) == '/' || filename.at(0) == '\\' ) // don't deal with paths from root ! 
    {
        if( ACE_OS::stat( ACE_TEXT_CHAR_TO_TCHAR(filename.c_str()), &stat ) != 0 )
        {
            return false;
        }
        fullname = filename;
        return true;
    }
    std::vector<std::string>::iterator it;
    for( it = directories.begin(); it != directories.end(); it++ )
    {
        std::string name = (*it) + "/" + filename;
        if( ACE_OS::stat( ACE_TEXT_CHAR_TO_TCHAR(name.c_str()), &stat ) == 0 )
        {
            fullname = name;
            return true;
        }
    }
    return false;
}

} // namespace ot
