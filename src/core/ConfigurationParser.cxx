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
/** source file for class ConfigurationParser.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigurationParser.cxx,v 1.13 2001/06/13 19:58:35 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ConfigurationParser.h"
#ifdef WIN32
#include <iostream>    // VisualC++ uses the STL based iostream lib
#else
#include <iostream.h>
#endif

#include <util/PlatformUtils.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_Node.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_Text.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include "DOMTreeErrorReporter.h"

// constructor method

ConfigurationParser::ConfigurationParser( NodeFactory& factory_)
  : factory( factory_ )
{
    // Initialize the XML4C system
    try {
        XMLPlatformUtils::Initialize();
    }
    catch ( XMLException& toCatch){
        cout << "Error during initialization! :\n"
             << DOMString(toCatch.getMessage()).transcode() << endl;
        exit(1);
    }
}

// Destructor method.

ConfigurationParser::~ConfigurationParser()
{
  modules.clear();
  references.clear();
}

// adds a named module to the internal ModuleMap.

void ConfigurationParser::addModule(const string& name, Module& module)
{
    modules[name] = &module;
}

// builds a tree of configuration nodes. 

ConfigNode * ConfigurationParser::buildConfigTree( DOM_Element & element )
{
    StringTable & map = parseElement( element );
    string tagName = element.getTagName().transcode();
    ConfigNode * config = new ConfigNode( tagName, &map );
	config->setParent( element );
    DOM_NodeList list = element.getChildNodes();
    for( int i = 0; i < list.getLength(); i ++ )
    {
        if( list.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
        {
            DOM_Element childElement = (const DOM_Element &)list.item(i);
            ConfigNode * child = buildConfigTree( childElement );      
        }
    }
    return config;
}

// builds the tracker tree starting from a certain DOM_Element.

Node * ConfigurationParser::buildTree( DOM_Element& element)
{
    string tagName = element.getTagName().transcode();
	StringTable & map = parseElement( element );
    // Test for a reference node
    if( tagName.compare("Ref") == 0 )
    {
        NodeMap::iterator find = references.find(map.get("USE"));
        if( find != references.end()){
			RefNode * ref = new RefNode( (*find).second );
			ref->setParent( element );
			cout << "Build Reference node -> " << map.get("USE") << "." << endl;
			delete & map;			
            return ref;
        } else
        {
            cout << "Undefined reference " << map.get("USE") << " !" << endl;
			delete & map;
            return NULL;
        }
    }

    Node * value = factory.createNode( tagName , map );
    if( value != NULL )
    {
		value->setParent( element );
        // Test for ID 
        if( map.containsKey("DEF"))
        {
            references[map.get("DEF")] = value;
            cout << "Storing Reference " << map.get("DEF") << endl;
        }
        DOM_NodeList list = element.getChildNodes();
        for( int i = 0; i < list.getLength(); i ++ )
        {
            if( list.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
            {
                DOM_Element childElement = (const DOM_Element &)list.item(i);
                Node * childNode = buildTree( childElement );
            }
        }
    }
    else
    {
        cout << "Warning : Could not parse element " <<
            element.getTagName().transcode() << endl;
    }
    delete & map;
    return value;
}

// This method parses an XML configuration file.

Node * ConfigurationParser::parseConfigurationFile(const string& filename)
{
    // read and parse configuration file
    DOMParser parser;
    parser.setDoValidation( true );
    DOMTreeErrorReporter errReporter;
    parser.setErrorHandler(&errReporter);
	parser.setIncludeIgnorableWhitespace( false );
    try
    {
        parser.parse( filename.c_str() );
    }
    catch ( XMLException& e)
    {
        cout << "An error occured during parsing\n   Message: "
             << DOMString(e.getMessage()).transcode() << endl;
        exit(1);
    }
    if( errReporter.errorsEncountered() > 0 )
    {
        cout << "There were non fatal errors in the configuration file !\n"
             << "Please check the file and start again." << endl;
        exit(1);
    }

    DOM_Document doc = parser.getDocument();
	document = new DOM_Document( doc );
    DOM_Element root = doc.getDocumentElement();
	cout << "Root node is " << root.getTagName().transcode() << endl;

    // get the configuration part
    DOM_NodeList list = (DOM_NodeList)root.getElementsByTagName( "configuration" );
    if( list.getLength() != 1 )
    {
        cout << "not valid config file, not exactly one configuration tag" << endl;
        cout << list.getLength() << " configurations." << endl;
        exit(1);
    }

    cout << "parsing configuration section" << endl;

    // parse configuration elements subelements
    DOM_Element config = (const DOM_Element &)list.item(0);
    DOM_NodeList configlist = (DOM_NodeList)config.getChildNodes();
    int i;
    for( i = 0; i < configlist.getLength(); i ++ )
    {
        if( configlist.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
        {
            DOM_Element configElement = (const DOM_Element &)configlist.item(i);
            StringTable & attributes = parseElement( configElement );
            string tagName = configElement.getTagName().transcode();
			ConfigNode * base = new ConfigNode( tagName, & attributes );
			base->setParent( configElement );
            cout << "config for " << tagName  << endl;

            DOM_NodeList nodelist = (DOM_NodeList)configElement.getChildNodes();
            
            for( int j = 0; j < nodelist.getLength(); j++ )
            {
                if( nodelist.item(j).getNodeType() == DOM_Node::ELEMENT_NODE )
                {        
                    DOM_Element element = (const DOM_Element &)nodelist.item(j);                    
                    ConfigNode * child = buildConfigTree( element );              
                }
            }	
            if( modules.find(tagName) != modules.end())
            {
                modules.find(tagName)->second->init( attributes, base );
            }
            delete &attributes;
        }
    }

    cout << "parsing tracker tree section" << endl;

    // parse the rest of the elements
    DOM_NodeList rootlist = (DOM_NodeList)root.getChildNodes();
	Node * node = new Node();
	node->setParent( root );
    for( i = 0; i < rootlist.getLength(); i++ )
    {
        if( rootlist.item(i).getNodeType() != DOM_Node::ELEMENT_NODE )   // not an element node !
        {
            continue;
        }
        DOM_Element element = (const DOM_Element &)rootlist.item(i);
        if( element.getTagName().equals("configuration" ))    // the configuration element, allready handled
        {
            continue;
        }
		buildTree( element );
    }
    return node;
}

// parses an Elements attributes and returns a StringMap describing them.

StringTable & ConfigurationParser::parseElement( DOM_Element& element)
{
    StringTable * value = new StringTable;
    DOM_NamedNodeMap map = element.getAttributes();
    for( int i = 0; i < map.getLength(); i++ )
    {
        DOM_Attr attribute = (const DOM_Attr &)map.item( i );
        value->put( attribute.getName().transcode(), attribute.getValue().transcode());
    }
    return *value;
}

// removes a module with the given name from the ModuleMap

void ConfigurationParser::removeModule(const string& name)
{
    if( modules.find(name) != modules.end())
    {
        modules.erase( name );
    }
}
