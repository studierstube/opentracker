// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      ConfigurationParser.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'ConfigurationParser'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#include <util/PlatformUtils.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_Node.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_Text.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#ifdef WIN32
#include <iostream>    // VisualC++ has two incompatible iostreams libraries !
#else
#include <iostream.h>
#endif
#include "DOMTreeErrorReporter.h"

//@END_USER2


// Static members


/*@NOTE_3864
Constructor method. The Xerces XML parser library is initialized here.
*/
ConfigurationParser::ConfigurationParser(NodeFactory* factory_) //@INIT_3864
  : factory( factory_ )
{//@CODE_3864
    ConstructorInclude();

    // Initialize the XML4C system
    try {
	    XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch){
    	cout << "Error during initialization! :\n"
	         << DOMString(toCatch.getMessage()).transcode() << endl;
    	exit(1);
    }
}//@CODE_3864


/*@NOTE_3734
Destructor method.
*/
ConfigurationParser::~ConfigurationParser()
{//@CODE_3734
    DestructorInclude();

    // Put in your own code
}//@CODE_3734


/*@NOTE_4629
adds a named module to the internal ModuleMap
*/
void ConfigurationParser::addModule(const char* name, Module& module)
{//@CODE_4629
    if( modules.find(name) == modules.end())
    {
        modules[name] = &module;
    }
}//@CODE_4629


/*@NOTE_3746
builds the tracker tree starting from a certain DOM_Element. Is used recoursively to walk
the DOMTree and create new nodes.
*/
Node* ConfigurationParser::buildTree(DOM_Element& element)
{//@CODE_3746
    Node* value;
    StringMap * map = parseElement( element );
    value = factory->createNode( element.getTagName().transcode(), *map );
    delete map;
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
	    	        ((TreeNode *)value)->addChild( *childNode );
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
}//@CODE_3746


/*@NOTE_4636
This method parses an XML configuration file. It returns a NodeVector with all the root nodes defined
in the configuration file.
*/
NodeVector* ConfigurationParser::parseConfigurationFile(const char* filename)
{//@CODE_4636
    NodeVector * value = new NodeVector;

     // read and parse configuration file
    DOMParser parser;
    parser.setDoValidation( true );
    DOMTreeErrorReporter errReporter;
    parser.setErrorHandler(&errReporter);
    try 
    {
        parser.parse( filename );
    }
    catch (const XMLException& e)
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
    DOM_NodeList &list = (DOM_NodeList&)root.getElementsByTagName( "configuration" );
    if( list.getLength() != 1 )
    {
        cout << "not valid config file, not exactly one configuration tag" << endl;
	    cout << list.getLength() << " configurations." << endl;
    	exit(1);
    }

    cout << "parsing config part" << endl;
    
    // parse configuration elements subelements    
    DOM_Element config = (DOM_Element &)list.item(0);
    list = (DOM_NodeList&)config.getChildNodes();
    for( int i = 0; i < list.getLength(); i ++ )
    {
        if( list.item(i).getNodeType() == DOM_Node::ELEMENT_NODE )
        {
    	    DOM_Element configElement = (DOM_Element &)list.item(i);
	        StringMap * attributes = parseElement( configElement );
            
            cout << "config for " << configElement.getTagName().transcode() << endl;
            
	        if( modules.find(configElement.getTagName().transcode()) != modules.end())
            {
   	            modules[configElement.getTagName().transcode()]->init( *attributes, NULL );
    	    }
	        delete attributes;
    	}
    }

    cout << "parsing children" << endl;
    
    // parse the rest of the elements
    list = (DOM_NodeList&)root.getChildNodes();
    for( i = 0; i < list.getLength(); i++ )
    {
        if( list.item(i).getNodeType() != DOM_Node::ELEMENT_NODE )   // not an element node !
        {
       	    continue;
        }
        DOM_Element element = (DOM_Element &)list.item(i);
        if( element.getTagName().equals("configuration" ))    // the configuration element, allready handled
        {
	        continue;
        }
    	Node * node = buildTree( element );
	    if( node != NULL ){
            value->push_back( node );
    	}
    }
    return value;
}//@CODE_4636


/*@NOTE_3738
parses an Elements attributes and returns a StringMap describing them.
*/
StringMap* ConfigurationParser::parseElement(DOM_Element& element)
{//@CODE_3738
    StringMap * value = new StringMap;
    DOM_NamedNodeMap map = element.getAttributes();
    for( int i = 0; i < map.getLength(); i++ )
    {
        DOM_Attr attribute = (DOM_Attr &)map.item( i );
    	(*value)[attribute.getName().transcode()] = attribute.getValue().transcode();
    }
    return value;
}//@CODE_3738


/*@NOTE_4632
removes a module with the given name from the ModuleMap
*/
void ConfigurationParser::removeModule(const char* name)
{//@CODE_4632
    if( modules.find(name) != modules.end())
    {
        modules.erase( name );
    }
}//@CODE_4632


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3733
Method which must be called first in a constructor.
*/
void ConfigurationParser::ConstructorInclude()
{
}


/*@NOTE_3735
Method which must be called first in a destructor.
*/
void ConfigurationParser::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
