 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for class ConfigurationParser.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigurationParser.cxx,v 1.4 2001/01/31 14:49:57 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ConfigurationParser.h"

#include <util/PlatformUtils.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_Node.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_Text.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#ifdef WIN32
#include <iostream>    // VisualC++ uses the STL based iostream lib
#else
#include <iostream.h>
#endif
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

void ConfigurationParser::addModule( string& name, Module& module)
{
    modules[name] = &module;
}

// builds a tree of configuration nodes. 

ConfigNode * ConfigurationParser::buildConfigTree( DOM_Element & element )
{
    StringMap & map = parseElement( element );
    string tagName = element.getTagName().transcode();
    ConfigNode * config = new ConfigNode( tagName, & map );
    DOM_NodeList list = (DOM_NodeList &)element.getChildNodes();
    for( int i = 0; i < list.getLength(); i ++ )
    {
        if( list.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
        {
            DOM_Element childElement = (DOM_Element&)list.item(i);
            ConfigNode * child = buildConfigTree( childElement );
            if( child != NULL )
            {
                config->addChild( * child );
            }
        }
    }
    return config;
}

// builds the tracker tree starting from a certain DOM_Element.

Node * ConfigurationParser::buildTree( DOM_Element& element)
{
    StringMap & map = parseElement( element );
    string tagName = element.getTagName().transcode();
    // Test for a reference node
    if( tagName.compare("Ref") == 0 )
    {
        NodeMap::iterator find = references.find(map["USE"]);
        if( find != references.end()){
            return (*find).second;
        } else
        {
            cout << "Undefined reference " << map["USE"] << " !" << endl;
            return NULL;
        }
    }
    Node * value = factory.createNode( tagName , map );
    if( value != NULL )
    {
        // Test for ID 
        if( map.find("REF") != map.end())
        {
            references[map["REF"]] = value;
        }
        DOM_NodeList list = (DOM_NodeList &)element.getChildNodes();
        for( int i = 0; i < list.getLength(); i ++ )
        {
            if( list.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
            {
                DOM_Element childElement = (DOM_Element&)list.item(i);
                Node * childNode = buildTree( childElement );
                if( childNode != NULL )
                {   
                    /*
                    // if the child is only a wrapper node
                    // we use addWrappedChild to add the
                    // children to our current node.
                    if( childNode->isWrapperNode() != NULL )              
                    {                                                     
                        WrapperNode * wrapper = childNode->isWrapperNode(); 
                        string name = wrapper->getTagName();
                        NodeVector & nodes = wrapper->getChildren();
                        for( NodeVector::iterator index = nodes.begin(); index != nodes.end(); index++ )
                        {
                            value->addWrappedChild(*(*index), name );
                        }
                    }
                    else  // otherwise we add the node itself
                    { */
                        value->addChild( *childNode );
                    // }
                }
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

Node * ConfigurationParser::parseConfigurationFile( string& filename)
{
    TreeNode * value = new TreeNode;

     // read and parse configuration file
    DOMParser parser;
    parser.setDoValidation( true );
    DOMTreeErrorReporter errReporter;
    parser.setErrorHandler(&errReporter);
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
    DOM_Element root = doc.getDocumentElement();

    // get the configuration part
    DOM_NodeList list = (DOM_NodeList)root.getElementsByTagName( "configuration" );
    if( list.getLength() != 1 )
    {
        cout << "not valid config file, not exactly one configuration tag" << endl;
        cout << list.getLength() << " configurations." << endl;
        exit(1);
    }

    cout << "parsing config part" << endl;

    // parse configuration elements subelements
    DOM_Element config = (DOM_Element &)list.item(0);
    DOM_NodeList configlist = (DOM_NodeList)config.getChildNodes();
    for( int i = 0; i < configlist.getLength(); i ++ )
    {
        if( configlist.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
        {
            DOM_Element configElement = (DOM_Element &)configlist.item(i);
            StringMap & attributes = parseElement( configElement );
            string tagName = configElement.getTagName().transcode();
            cout << "config for " << tagName  << endl;

            DOM_NodeList nodelist = (DOM_NodeList)configElement.getChildNodes();
            ConfigNode * base = NULL;
            for( int j = 0; j < nodelist.getLength(); j++ )
            {
                if( nodelist.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
                {
                    if( base == NULL )
                    {
                        base = new ConfigNode("", new StringMap );
                    }
                    DOM_Element element = (DOM_Element &)nodelist.item(i);                    
                    ConfigNode * child = buildConfigTree( element );
                    if( child != NULL )
                    {
                        base->addChild( *child );
                    }
                }
            }

            if( modules.find(tagName) != modules.end())
            {
                modules.find(tagName)->second->init( attributes, base );
            }
            delete &attributes;
        }
    }

    cout << "parsing children" << endl;

    // parse the rest of the elements
    DOM_NodeList rootlist = (DOM_NodeList)root.getChildNodes();
    for( i = 0; i < rootlist.getLength(); i++ )
    {
        if( rootlist.item(i).getNodeType() != DOM_Node::ELEMENT_NODE )   // not an element node !
        {
            continue;
        }
        DOM_Element element = (DOM_Element &)rootlist.item(i);
        if( element.getTagName().equals("configuration" ))    // the configuration element, allready handled
        {
            continue;
        }
        Node * node = buildTree( element );
        if( node != NULL ){
            value->addChild( *node );
        }
    }
    return value;
}

// parses an Elements attributes and returns a StringMap describing them.

StringMap & ConfigurationParser::parseElement( DOM_Element& element)
{
    StringMap & value = *(new StringMap);
    DOM_NamedNodeMap map = element.getAttributes();
    for( int i = 0; i < map.getLength(); i++ )
    {
        DOM_Attr attribute = (DOM_Attr &)map.item( i );
        value[attribute.getName().transcode()] = attribute.getValue().transcode();
    }
    return value;
}

// removes a module with the given name from the ModuleMap

void ConfigurationParser::removeModule( string& name)
{
    if( modules.find(name) != modules.end())
    {
        modules.erase( name );
    }
}
