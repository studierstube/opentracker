 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for class ConfigurationParser.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigurationParser.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
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

/// ructor method

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

/// Destructor method.

ConfigurationParser::~ConfigurationParser()
{
  modules.clear();
}

/// adds a named module to the internal ModuleMap.

void ConfigurationParser::addModule( string& name, Module& module)
{
    modules[name] = &module;
}

/// builds the tracker tree starting from a certain DOM_Element.

Node * ConfigurationParser::buildTree( DOM_Element& element)
{
    StringMap & map = parseElement( element );
    string tagName = element.getTagName().transcode();
    Node * value = factory.createNode( tagName , map );
    delete & map;
    if( value != NULL )
    {
        DOM_NodeList list = (DOM_NodeList &)element.getChildNodes();
        for( int i = 0; i < list.getLength(); i ++ )
        {
            if( list.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
            {
                DOM_Element childElement = (DOM_Element&)list.item(i);
                Node * childNode = buildTree( childElement );
                if( childNode != NULL )
                {
                    if( childNode->isWrapperNode() != NULL )                 // if the child is only a wrapper node
                    {                                                     // we use addWrappedChild to add the
                        WrapperNode * wrapper = childNode->isWrapperNode(); // children to our current node.
                        string name = wrapper->getTagName();
                        NodeVector & nodes = wrapper->getChildren();
                        for( NodeVector::iterator index = nodes.begin(); index != nodes.end(); index++ )
                        {
                            value->addWrappedChild(*(*index), name );
                        }
                    }
                    else                                                  // otherwise we add the node itself
                    {
                        value->addChild( *childNode );
                    }
                }
            }
        }
    }
    else
    {
        cout << "Warning : Could not parse element " <<
            element.getTagName().transcode() << endl;
    }
    return value;
}

/// This method parses an XML configuration file.

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

            if( modules.find(tagName) != modules.end())
            {
                modules.find(tagName)->second->init( attributes, NULL );
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

/// parses an Elements attributes and returns a StringMap describing them.

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

/// removes a module with the given name from the ModuleMap

void ConfigurationParser::removeModule( string& name)
{
    if( modules.find(name) != modules.end())
    {
        modules.erase( name );
    }
}
