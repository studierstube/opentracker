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
  * @todo set attributes on the element in createNode
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Context.cxx,v 1.18 2002/05/28 14:54:21 reitmayr Exp $
  * @file                                                                   */     
 /* ======================================================================= */

#include "Context.h"
#include "../OpenTracker.h"

#include <xercesc/dom/DOM_Node.hpp>
#include <xercesc/dom/DOM_NodeList.hpp>
#include <xercesc/dom/DOM_Document.hpp>
#include <xercesc/dom/DOM_Element.hpp>

using namespace std;

// constructor method.

Context::Context( int init )
{
    if( init != 0 )
    {
        initializeContext( *this );
        cleanUp = 1;
    }
    else {
        cleanUp = 0;
    }
}

// Destructor method.
Context::~Context()
{
    if( cleanUp == 1 )
    {
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            delete (*it).second;
        }
    }
    modules.clear();
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

void Context::addModule(const string & name, Module & module)
{
    modules[name] = &module;
}

// returns a module indexed by its configuration elements name

Module * Context::getModule(const string & name)
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
}

// parses the file and builds the tree.

void Context::parseConfiguration(const string& filename)
{
    ConfigurationParser parser( *this );
    rootNode = parser.parseConfigurationFile( filename );
    DOM_Document doc = rootNode->parent->getOwnerDocument();
    doc.setUserData( this );
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

Node * Context::createNode( const string & name, StringTable & attributes)
{
    Node * value = factory.createNode( name , attributes );
    if( value != NULL )
    {
        // add a correctly created DOM_Element to the node here and return
        DOM_Document doc = rootNode->parent->getOwnerDocument();
        DOM_Element el = doc.createElement(name.c_str());
        value->setParent( el );        
        // set attributes on the element node
        KeyIterator keys(attributes);
        while( keys.hasMoreKeys())
        {
            const string & key = keys.nextElement();
            el.setAttribute( key.c_str(), attributes.get( key ).c_str());
        }
    }
    return value;
}

Node * Context::getRootNode()
{
    return rootNode;
}

Node * Context::findNode(const string & id)
{
    // search for the right node via the DOM_Document API
    DOM_Element el = rootNode->parent->getOwnerDocument().getElementById(id.c_str());
    if( el != 0 )
        return (Node *)el.getUserData();
    return NULL;
}
