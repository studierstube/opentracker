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
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/Log_Msg.h>
#include <ace/Env_Value_T.h>
#include "../tool/OT_ACE_Log.h"

#include "ConfigurationParser.h"

#ifdef USE_XERCES
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLEntityResolver.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#endif //USE_XERCES

#include "DOMTreeErrorReporter.h"
#include "RefNode.h"

#include <iostream>

//using namespace std;

#ifdef USE_XERCES
XERCES_CPP_NAMESPACE_USE
#endif //USE_XERCES


namespace ot {

#ifdef USE_XERCES
const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull };

class OpenTrackerResolver : public XMLEntityResolver {
protected:
    Context * context;

public:
    OpenTrackerResolver ( Context * context_ ) : context(context_)
    {};

    virtual InputSource * resolveEntity( XMLResourceIdentifier * resourceIdentifier );
};

InputSource * OpenTrackerResolver::resolveEntity( XMLResourceIdentifier * resourceIdentifier )
{
    switch(resourceIdentifier->getResourceIdentifierType())
    {
    case XMLResourceIdentifier::ExternalEntity:
        XMLCh * test = XMLString::transcode("opentracker.dtd");
        if(XMLString::endsWith(resourceIdentifier->getSystemId(), test))
        {
            ACE_Env_Value<std::string> otroot(ACE_TEXT("OTROOT"), "");
            std::string otrootvalue = (std::string)otroot;
            std::string otdatadir;
            if( otrootvalue.compare("") != 0 )
            {
                otdatadir = otrootvalue + "/data";
                context->addDirectoryLast(otdatadir);
            }        
            char * file = XMLString::transcode(resourceIdentifier->getSystemId());
            std::string filename( file ), fullname;
            XMLString::release( &file );
            bool result = context->findFile( filename, fullname );
            if( otrootvalue.compare("") != 0 )
            {
                context->removeDirectory(otdatadir);
            }
            if( result == true )
            {
                XMLCh * file = XMLString::transcode( fullname.c_str());
                LocalFileInputSource * input = new LocalFileInputSource( file );
                XMLString::release( &file );
                XMLString::release( &test );
                return input;
            }
        }
        XMLString::release( &test );
    }
    return NULL;
}

#endif //USE_XERCES

// constructor method

ConfigurationParser::ConfigurationParser( Context & context_)
  : context( context_ )
{
#ifdef USE_XERCES
    // Initialize the XercesC system
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
		char * message = XMLString::transcode( toCatch.getMessage());
        LOG_ACE_ERROR( "ot: ConfigurationParser Error during initialization: %s\n", message );
		XMLString::release( &message );
        exit(1);
    }
#endif //USE_XERCES
}

// Destructor method.

ConfigurationParser::~ConfigurationParser()
{
    references.clear();
}

// builds a tree of configuration nodes. 

ConfigNode * ConfigurationParser::buildConfigTree( OT_DOMELEMENT * element )
{
#ifdef USE_XERCES
    std::auto_ptr<StringTable> map ( parseElement( element ));
	char * tempName = XMLString::transcode( element->getLocalName());
    std::string tagName = tempName;
	XMLString::release( &tempName );
    ConfigNode * config = new ConfigNode( *map );
    config->setParent( element );
    //auto_ptr<DOMNodeList> list( element->getChildNodes());
    DOMNodeList * list = element->getChildNodes();
    for( unsigned int i = 0; i < list->getLength(); i ++ )
    {
        if( list->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            OT_DOMELEMENT * childElement = (OT_DOMELEMENT *)list->item(i);
            ConfigNode * child = buildConfigTree( childElement );
        }
    }
    return config;
#endif //USE_XERCES

#ifdef USE_TINYXML
    std::auto_ptr<StringTable> map ( parseElement( element ));
    std::string tagName = element->Value();
    ConfigNode * config = new ConfigNode( *map );
    config->setParent( element );

	TiXmlElement * el = element->FirstChildElement();
	while( el != NULL )
	{
		ConfigNode * child = buildConfigTree( el);
		el = el->NextSiblingElement();
	}
    return config;
#endif //USE_TINYXML
}

// builds the tracker tree starting from a certain DOM_Element.

Node * ConfigurationParser::buildTree( OT_DOMELEMENT * element)
{
#ifdef USE_XERCES
    char * tempName = XMLString::transcode( element->getLocalName());
    std::string tagName = tempName;
	XMLString::release( &tempName );
    std::auto_ptr<StringTable> map ( parseElement( element ));
    // Test for a reference node
    if( tagName.compare("Ref") == 0 )
    {
        NodeMap::iterator find = references.find(map->get("USE"));
        if( find != references.end()){
			RefNode * ref = new RefNode( (*find).second );
            ref->type = tagName;
			ref->setParent( element );
			LOG_ACE_INFO("ot:Build Reference node -> %s\n", map->get("USE").c_str());
	        return ref;
        } else
        {
			LOG_ACE_ERROR("ot:Undefined reference %s\n", map->get("USE").c_str());
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
			LOG_ACE_INFO("ot:Storing Reference %s\n", map->get("USE").c_str());
        }
        //auto_ptr<DOMNodeList> list ( element->getChildNodes());
        DOMNodeList * list = element->getChildNodes();
        for( unsigned int i = 0; i < list->getLength(); i ++ )
        {
            if( list->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
            {
                OT_DOMELEMENT * childElement = (OT_DOMELEMENT *)list->item(i);
                Node * childNode = buildTree( childElement );
            }
        }
    }
    else
    {
		LOG_ACE_INFO("ot:Warning : Could not parse element %s\n", tagName.c_str());
    }
    return value;
#endif //USE_XERCES

#ifdef USE_TINYXML
    std::string tagName = element->Value();
    std::auto_ptr<StringTable> map ( parseElement( element ));
    // Test for a reference node
    if( tagName.compare("Ref") == 0 )
    {
        NodeMap::iterator find = references.find(map->get("USE"));
        if( find != references.end()){
			RefNode * ref = new RefNode( (*find).second );
            ref->type = tagName;
			ref->setParent( element );
			LOG_ACE_INFO("ot:Build Reference node -> %s\n", map->get("USE").c_str());
	        return ref;
        } else
        {
			LOG_ACE_ERROR("ot:Undefined reference %s\n", map->get("USE").c_str());
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
			LOG_ACE_INFO("ot:Storing Reference %s\n", map->get("USE").c_str());
        }

		TiXmlElement * el = element->FirstChildElement();
		while( el != NULL )
		{
			Node * childNode = buildTree( el );
			el = el->NextSiblingElement();
		}
    }
    else
    {
		LOG_ACE_INFO("ot:Warning : Could not parse element %s\n", tagName.c_str());
    }
    return value;
#endif //USE_TINYXML
}


// This method parses an XML configuration file.

Node * ConfigurationParser::parseConfigurationFile(const std::string& filename)
{
#ifdef USE_XERCES
    // read and parse configuration file
    XercesDOMParser * parser = new XercesDOMParser();
    parser->setValidationScheme( XercesDOMParser::Val_Auto );
    parser->setIncludeIgnorableWhitespace( false );
    parser->setDoNamespaces(true);
    parser->setDoSchema(true);

    OpenTrackerResolver resolver( &context);
    parser->setXMLEntityResolver( &resolver );
    
    DOMTreeErrorReporter errReporter;
    parser->setErrorHandler( &errReporter );
	
    try
    {
        parser->parse( filename.c_str() );
    }
    catch (const XMLException& e)
    {
		char * message = XMLString::transcode( e.getMessage());
		LOG_ACE_ERROR("ot:An error occured during parsing\n   Message: %s\n", message);
		XMLString::release( & message );
		exit(1);
    }
    catch (const DOMException& e)
    {
		char * message = XMLString::transcode( e.msg );
		LOG_ACE_ERROR("ot:An error occured during parsing\n   Message: %s\n", message);
		XMLString::release( & message );
        exit(1);
    }
    if( errReporter.errorsEncountered() > 0 )
    {
		LOG_ACE_ERROR("ot:There were non fatal errors in the configuration file !\n");
		LOG_ACE_ERROR("ot:Please check the file and start again.\n");
        exit(1);
    }

    DOMDocument * doc = parser->getDocument();
    DOMElement * root = doc->getDocumentElement();
    Node * node = new Node();
	node->setParent( root );
	char * tempName = XMLString::transcode( root->getLocalName());
	LOG_ACE_INFO("ot:Root node is %s\n", tempName);
	XMLString::release( & tempName );

    const XMLCh* xmlspace = root->getNamespaceURI();
    if (xmlspace != NULL) {
		char * tempSpace = XMLString::transcode( xmlspace );
		LOG_ACE_INFO("ot:Namespace is %s\n", tempSpace);
		XMLString::release( & tempSpace );
    }
    else {
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Not using namespaces!\n")));
    }

    // get the configuration part
	XMLCh * configurationCh = XMLString::transcode( "configuration" );
    //auto_ptr<DOMNodeList> list( root->getElementsByTagName( configurationCh.get() ));
    DOMNodeList * list = root->getElementsByTagNameNS(xmlspace, configurationCh);
	XMLString::release( & configurationCh );
    if( list->getLength() != 1 )
    {
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:not valid config file, not exactly one configuration tag\n")));
		LOG_ACE_INFO("ot:%d configurations.\n", list->getLength());
        exit(1);
    }

	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:parsing configuration section\n")));

    // parse configuration elements subelements
    DOMElement * config = (DOMElement *)list->item(0);
    //auto_ptr<DOMNodeList> configlist( config->getChildNodes());
    DOMNodeList * configlist = config->getChildNodes();
    unsigned int i;
    for( i = 0; i < configlist->getLength(); i ++ )
    {
        if( configlist->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            DOMElement * configElement = (DOMElement *)configlist->item(i);
            std::auto_ptr<StringTable> attributes( parseElement( configElement ));
			char * tempName = XMLString::transcode( configElement->getLocalName());
            std::string tagName = tempName;
			XMLString::release( &tempName );
			ConfigNode * base = new ConfigNode( *attributes );
			base->setParent( configElement );
            LOG_ACE_INFO("ot:config for %s\n", tagName.c_str());

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

	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:parsing tracker tree section\n")));

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
		char * tempTagName = XMLString::transcode(element->getLocalName());
        if( 0 == XMLString::compareString( tempTagName, "configuration" ))    // the configuration element, already handled
        {
			XMLString::release( &tempTagName );
            continue;
        }
		XMLString::release( &tempTagName );
		buildTree( element );
    }
    return node;
#endif //USE_XERCES


#ifdef USE_TINYXML
	TiXmlDocument* document = new TiXmlDocument();

	if(!document->LoadFile(filename.c_str()))
	{
		LOG_ACE_ERROR("ot:An error occured during parsing\n   Message: %s\n", document->ErrorDesc());
        exit(1);
	}

	TiXmlElement* root = document->RootElement();
    Node * node = new Node();
	node->setParent( root );
    const char* tempName = root->Value();

	LOG_ACE_INFO("ot:Root node is %s\n", tempName);

    const char* xmlspace = NULL; //root->getNamespaceURI();
    if (xmlspace != NULL) {
		LOG_ACE_INFO("ot:Namespace is %s\n", xmlspace);
    }
    else {
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Not using namespaces!\n")));
    }

    // get the configuration part
    const char* configurationCh = "configuration";

	TiXmlElement * config = root->FirstChildElement(configurationCh);
	if(config->NextSiblingElement(configurationCh))
	{
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:not valid config file, not exactly one configuration tag\n")));
        exit(1);
	}

	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:parsing configuration section\n")));

	TiXmlElement * configElement = config->FirstChildElement();
	while(configElement)
	{
        std::auto_ptr<StringTable> attributes( parseElement( configElement ));
        std::string tagName = configElement->Value();
		ConfigNode * base = new ConfigNode( *attributes );
		base->setParent( configElement );
		LOG_ACE_INFO("ot:config for %s\n", tagName.c_str());

		TiXmlElement * element = configElement->FirstChildElement();
		while(element)
		{
			ConfigNode * child = buildConfigTree( element ); 
			element = element->NextSiblingElement();
		}

        Module * module = context.getModule( tagName );
        if( module != NULL )
        {
            module->init( *attributes, base );
        }

		configElement = configElement->NextSiblingElement();
    }

	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:parsing tracker tree section\n")));

    // parse the rest of the elements

	TiXmlElement * element = root->FirstChildElement();
	while(element)
	{
		if(strcmp(element->Value(), "configuration")!=0)
			buildTree( element );

		element = element->NextSiblingElement();
	}

    return node;
#endif //USE_TINYXML
}

// parses an Elements attributes and returns a StringMap describing them.

StringTable * ConfigurationParser::parseElement( OT_DOMELEMENT * element)
{
#ifdef USE_XERCES
    StringTable * value = new StringTable;
    // auto_ptr<DOMNamedNodeMap> map( element->getAttributes());   // is it still owned by the library ?
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
    return value;
#endif //USE_XERCES


#ifdef USE_TINYXML
    StringTable * value = new StringTable;

	TiXmlAttribute* attribute = element->FirstAttribute();
	while(attribute)
	{
		value->put(attribute->Name(), attribute->Value());
		attribute = attribute->Next();
	}
    return value;
#endif //USE_TINYXML
}


} // namespace ot
