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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigurationParser.cxx,v 1.24 2003/11/08 15:43:39 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "ConfigurationParser.h"
#include "DOMTreeErrorReporter.h"
#include "RefNode.h"

#include <iostream>
#include <memory>

using namespace std;
XERCES_CPP_NAMESPACE_USE

const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull };

// constructor method

ConfigurationParser::ConfigurationParser( Context & context_)
  : context( context_ )
{
    // Initialize the XercesC system
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        auto_ptr<char> message( XMLString::transcode(toCatch.getMessage()));
        cout << "Error during initialization! :\n"
            << message.get() << "\n";
        exit(1);
    }    
}

// Destructor method.

ConfigurationParser::~ConfigurationParser()
{
    references.clear();
}

// builds a tree of configuration nodes. 

ConfigNode * ConfigurationParser::buildConfigTree( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * element )
{
    auto_ptr<StringTable> map ( parseElement( element ));
    auto_ptr<char> tempName( XMLString::transcode( element->getLocalName()));
    string tagName = tempName.get();
    ConfigNode * config = new ConfigNode( *map );
    config->setParent( element );
    //auto_ptr<DOMNodeList> list( element->getChildNodes());
    DOMNodeList * list = element->getChildNodes();
    for( unsigned int i = 0; i < list->getLength(); i ++ )
    {
        if( list->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            DOMElement * childElement = (DOMElement *)list->item(i);
            ConfigNode * child = buildConfigTree( childElement );
        }
    }
    return config;
}

// builds the tracker tree starting from a certain DOM_Element.

Node * ConfigurationParser::buildTree( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * element)
{
    auto_ptr<char> tempName( XMLString::transcode( element->getLocalName()));
    string tagName = tempName.get();
    auto_ptr<StringTable> map ( parseElement( element ));
    // Test for a reference node
    if( tagName.compare("Ref") == 0 )
    {
        NodeMap::iterator find = references.find(map->get("USE"));
        if( find != references.end()){
			RefNode * ref = new RefNode( (*find).second );
            ref->type = tagName;
			ref->setParent( element );
			cout << "Build Reference node -> " << map->get("USE") << "." << endl;
	        return ref;
        } else
        {
            cout << "Undefined reference " << map->get("USE") << " !" << endl;
	        return NULL;
        }
    }

    Node * value = context.factory.createNode( tagName , *map );
    if( value != NULL )
    {
		value->setParent( element );        
        // Test for ID 
        if( map->containsKey("DEF"))
        {
            references[map->get("DEF")] = value;
            value->name = map->get("DEF");
            cout << "Storing Reference " << map->get("DEF") << endl;
        }
        //auto_ptr<DOMNodeList> list ( element->getChildNodes());
        DOMNodeList * list = element->getChildNodes();
        for( unsigned int i = 0; i < list->getLength(); i ++ )
        {
            if( list->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
            {
                XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * childElement = (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *)list->item(i);
                Node * childNode = buildTree( childElement );
            }
        }
    }
    else
    {
        cout << "Warning : Could not parse element " << tagName << endl;
    }
    return value;
}

// This method parses an XML configuration file.

Node * ConfigurationParser::parseConfigurationFile(const string& filename)
{
    // read and parse configuration file
    XercesDOMParser * parser = new XercesDOMParser();
    parser->setValidationScheme( XercesDOMParser::Val_Auto );
    parser->setIncludeIgnorableWhitespace( false );
    parser->setDoNamespaces(true);
    parser->setDoSchema(true);

    DOMTreeErrorReporter errReporter;
    parser->setErrorHandler( &errReporter );
	
    try
    {
        parser->parse( filename.c_str() );
    }
    catch (const XMLException& e)
    {
        auto_ptr<char> message ( XMLString::transcode( e.getMessage()));
        cout << "An error occured during parsing\n   Message: "
            << message.get() << endl;
        exit(1);
    }
    catch (const DOMException& e)
    {
        auto_ptr<char> message ( XMLString::transcode( e.msg ));
        cout << "An error occured during parsing\n   Message: "
            << message.get() << endl;
        exit(1);
    }
    if( errReporter.errorsEncountered() > 0 )
    {
        cout << "There were non fatal errors in the configuration file !\n"
             << "Please check the file and start again." << endl;
        exit(1);
    }

    DOMDocument * doc = parser->getDocument();
    DOMElement * root = doc->getDocumentElement();
    Node * node = new Node();
	node->setParent( root );
    auto_ptr<char> tempName( XMLString::transcode( root->getLocalName()));
	cout << "Root node is " << tempName.get() << endl;

    const XMLCh* xmlspace = root->getNamespaceURI();
    if (xmlspace != NULL) {
        cout << "Namespace is " << XMLString::transcode( xmlspace ) << endl;
    }
    else {
        cout << "Not using namespaces!" << endl;
    }

    // get the configuration part
    auto_ptr<XMLCh> configurationCh( XMLString::transcode( "configuration" ));
    //auto_ptr<DOMNodeList> list( root->getElementsByTagName( configurationCh.get() ));
    DOMNodeList * list = root->getElementsByTagNameNS(xmlspace, configurationCh.get());    
    if( list->getLength() != 1 )
    {
        cout << "not valid config file, not exactly one configuration tag" << endl;
        cout << list->getLength() << " configurations." << endl;
        exit(1);
    }

    cout << "parsing configuration section" << endl;

    // parse configuration elements subelements
    DOMElement * config = (DOMElement *)list->item(0);
    //auto_ptr<DOMNodeList> configlist( config->getChildNodes());
    DOMNodeList * configlist = config->getChildNodes();
    unsigned int i;
    for( i = 0; i < configlist->getLength(); i ++ )
    {
        if( configlist->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * configElement = (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *)configlist->item(i);
            auto_ptr<StringTable> attributes( parseElement( configElement ));
            auto_ptr<char> tempName( XMLString::transcode( configElement->getLocalName()));
            string tagName = tempName.get();
			ConfigNode * base = new ConfigNode( *attributes );
			base->setParent( configElement );
            cout << "config for " << tagName  << endl;

            //auto_ptr<DOMNodeList> nodelist( configElement->getChildNodes());
            DOMNodeList * nodelist = configElement->getChildNodes();

            unsigned int j;
            for( j = 0; j < nodelist->getLength(); j++ )
            {
                if( nodelist->item(j)->getNodeType() == DOMNode::ELEMENT_NODE )
                {        
                    DOMElement * element = (DOMElement *)nodelist->item(j);
                    ConfigNode * child = buildConfigTree( element ); 
                }
            }
            Module * module = context.getModule( tagName );
            if( module != NULL )
            {
                module->init( *attributes, base );
            }            
        }
    }

    cout << "parsing tracker tree section" << endl;

    // parse the rest of the elements
    //auto_ptr<DOMNodeList> rootlist( root->getChildNodes());
    DOMNodeList * rootlist = root->getChildNodes();
    for( i = 0; i < rootlist->getLength(); i++ )
    {
        if( rootlist->item(i)->getNodeType() != DOMNode::ELEMENT_NODE )   // not an element node !
        {
            continue;
        }
        DOMElement * element = (DOMElement *)rootlist->item(i);
        auto_ptr<char> tempTagName( XMLString::transcode(element->getLocalName()));
        if( 0 == XMLString::compareString( tempTagName.get(), "configuration" ))    // the configuration element, already handled
        {
            continue;
        }
		buildTree( element );
    }
    return node;
}

// parses an Elements attributes and returns a StringMap describing them.

StringTable * ConfigurationParser::parseElement( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * element)
{
    StringTable * value = new StringTable;
    // auto_ptr<DOMNamedNodeMap> map( element->getAttributes());   // is it still owned by the library ?
    DOMNamedNodeMap * map = element->getAttributes();
    for( unsigned int i = 0; i < map->getLength(); i++ )
    {
        DOMAttr * attribute = (DOMAttr *)map->item( i );
        auto_ptr<char> nameTemp( XMLString::transcode( attribute->getName()));
        auto_ptr<char> valueTemp( XMLString::transcode( attribute->getValue()));
        value->put( nameTemp.get(), valueTemp.get() );
    }
    return value;
}
