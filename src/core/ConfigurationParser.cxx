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
/** source file for class ConfigurationParser.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/Log_Msg.h>
#include <ace/Env_Value_T.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <OpenTracker/core/ConfigurationParser.h>

#ifdef USE_XERCES
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLEntityResolver.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#endif //USE_XERCES

#include <OpenTracker/core/DOMTreeErrorReporter.h>
#include <OpenTracker/core/RefNode.h>

#include <iostream>

//using namespace std;

#ifdef USE_XERCES
XERCES_CPP_NAMESPACE_USE
#endif //USE_XERCES


namespace ot {

#ifdef USE_XERCES
    const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull };
    static const char*  otMemBufId = "otgraph";

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
        if (resourceIdentifier->getResourceIdentifierType() == XMLResourceIdentifier::ExternalEntity)
        {
            XMLCh * test = XMLString::transcode("opentracker.dtd");
            if(XMLString::endsWith(resourceIdentifier->getSystemId(), test))
            {
                ACE_Env_Value<std::string> otroot(ACE_TEXT("OTROOT"), "");
                std::string otrootvalue = (std::string)otroot;
                std::string otdatadir;
                if( otrootvalue.compare("") != 0 )
                {
                    //otdatadir = otrootvalue + "/data";
                    otdatadir = otrootvalue;// + "/share";
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
            logPrintE("ConfigurationParser Error during initialization: %s\n", message );
            XMLString::release( &message );
            exit(1);
        }
#endif //USE_XERCES
    }

    // Destructor method.

    ConfigurationParser::~ConfigurationParser()
    {
        references.clear();
#ifdef fish
        // Deinitialize the XercesC system
        try {
            XMLPlatformUtils::Terminate();
        }
        catch (const XMLException& toCatch) {
            char * message = XMLString::transcode( toCatch.getMessage());
            logPrintE( "ConfigurationParser Error during deinitialization: %s\n", message );
            XMLString::release( &message );
            exit(1);
        }
#endif //USE_XERCES
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
#ifdef OT_LOCAL_GRAPH
        config->type = tagName;
#endif // OT_LOCAL_GRAPH
        config->setParent( element );
        //auto_ptr<DOMNodeList> list( element->getChildNodes());
        DOMNodeList * list = element->getChildNodes();
        for( unsigned int i = 0; i < list->getLength(); i ++ )
        {
            if( list->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
            {
                OT_DOMELEMENT * childElement = (OT_DOMELEMENT *)list->item(i);
                ConfigNode * child = buildConfigTree( childElement );
#ifdef OT_LOCAL_GRAPH
                        // OT_GRAPH
                config->addChild(*child);
                        // OT_GRAPH
#endif OT_LOCAL_GRAPH


            }
        }
        return config;
#endif //USE_XERCES

#ifdef USE_TINYXML
        std::auto_ptr<StringTable> map ( parseElement( element ));
        std::string tagName = element->Value();
        ConfigNode * config = new ConfigNode( *map );
        config->setParent( element );
#ifdef OT_LOCAL_GRAPH
        config->type = tagName;
#endif // OT_LOCAL_GRAPH

        TiXmlElement * el = element->FirstChildElement();
        while( el != NULL )
        {
            ConfigNode * child = buildConfigTree( el);
#ifdef OT_LOCAL_GRAPH
                        // OT_GRAPH
            config->addChild(*child);
                        // OT_GRAPH
#endif OT_LOCAL_GRAPH
            
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
#ifdef OT_LOCAL_GRAPH
        // OT_GRAPH
        map->put("OtNodeType", tagName);
        // OT_GRAPH
#endif // OT_LOCAL_GRAPH
        if( tagName.compare("Ref") == 0 )
        {
            NodeMap::iterator find = references.find(map->get("USE"));
            if( find != references.end()){

                RefNode * ref = new RefNode( (*find).second );
#ifdef OT_LOCAL_GRAPH
                KeyIterator keys(*map.get());
                while( keys.hasMoreKeys())
                {
                    const std::string & key = keys.nextElement();
                    ref->put( key, map->get( key ) );
                }

#endif // OT_LOCAL_GRAPH


                ref->type = tagName;
                ref->setParent( element );
                logPrintI("Built Reference node -> %s\n", map->get("USE").c_str());
                return ref;
            } else
            {
                logPrintE("Undefined reference %s\n", map->get("USE").c_str());
                return NULL;
            }
        }

#ifdef OT_LOCAL_GRAPH
        Node * value = context.createNode( tagName , *map );
#else // 
        Node * value = context.factory.createNode( tagName , *map );
#endif //OT_LOCAL_GRAPH

		// if the value is NULL, it might be the case that the Module has not yet being loaded,
		// a trick to force the context to load this module, is to ask for it.
		if (value == NULL) {
			//try loading the module, and then creating the node
			if (context.getModuleFromNodeType(tagName) != NULL){
                            //                            logPrintW("Context found module from %s\n", tagName.c_str());
                            // try creating the node again
#ifdef OT_LOCAL_GRAPH
                            value = context.createNode( tagName , *map );
#else // 
                            value = context.factory.createNode( tagName , *map );
#endif //OT_LOCAL_GRAPH
                            
       
			}
		}
        if( value != NULL )
        {

            value->setParent( element );
      
            // Test for ID
            if( map->containsKey("DEF"))
            {
                references[map->get("DEF")] = value;
                value->name = map->get("DEF");
                logPrintI("Storing Reference %s\n", (map->get("DEF").c_str()));
            }

            //auto_ptr<DOMNodeList> list ( element->getChildNodes());
            DOMNodeList * list = element->getChildNodes();
            for( unsigned int i = 0; i < list->getLength(); i ++ )
            {
                if( list->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
                {

                    OT_DOMELEMENT * childElement = (OT_DOMELEMENT *)list->item(i);
                    ///FIXXXME: what was the assignment for? 

                    Node * childNode = buildTree( childElement );
#ifdef OT_LOCAL_GRAPH
                    // OT_GRAPH
                    value->addChild(*childNode);
                    // OT_GRAPH
#endif // OT_LOCAL_GRAPH
                }
            }
        }
        else
        {
            logPrintW("Could not parse element %s\n", tagName.c_str());
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
#ifdef OT_LOCAL_GRAPH
                KeyIterator keys(*map.get());
                while( keys.hasMoreKeys())
                {
                    const std::string & key = keys.nextElement();
                    ref->put( key, map->get( key ) );
                }

#endif // OT_LOCAL_GRAPH

                ref->type = tagName;
                ref->setParent( element );
                logPrintI("Built Reference node -> %s\n", map->get("USE").c_str());
                return ref;
            } else
            {
                logPrintE("Undefined reference %s\n", map->get("USE").c_str());
                return NULL;
            }
        }

#ifdef OT_LOCAL_GRAPH
        map->put("OtNodeType", tagName);        
        Node * value = context.createNode( tagName , *map );
#else
        Node * value = context.factory.createNode( tagName , *map );
#endif
        if (value == NULL) {
            //try loading the module, and then creating the node
            if (context.getModuleFromNodeType(tagName) != NULL){
                // try creating the node again
                logPrintI("ConfigurationParser trying to create node %s again \n", tagName.c_str());
#ifdef OT_LOCAL_GRAPH
                value = context.createNode(tagName, *map);
#else
                value = context.factory.createNode(tagName, *map);
#endif
            }
        }
        if( value != NULL )
        {
            value->setParent( element );
            // Test for ID
            if( map->containsKey("DEF"))
            {
                references[map->get("DEF")] = value;
                value->name = map->get("DEF");
                logPrintI("Storing Reference %s\n", map->get("USE").c_str());
            }

            TiXmlElement * el = element->FirstChildElement();
            while( el != NULL )
            {
                Node * childNode = buildTree( el );
#ifdef OT_LOCAL_GRAPH
                // OT_GRAPH
                value->addChild(*childNode);
                // OT_GRAPH
#endif // OT_LOCAL_GRAPH
                el = el->NextSiblingElement();
            }
        }
        else
        {
            logPrintW("Could not parse element %s\n", tagName.c_str());
        }
        return value;
#endif //USE_TINYXML
    }

#ifdef USE_XERCES
    // This method takes an InputSource and configures the OT graph accordingly
    Node * ConfigurationParser::parseConfiguration(InputSource* input_source) 
    {
      
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
            parser->parse( *input_source );
        }
        catch (const XMLException& e)
        {
            char * message = XMLString::transcode( e.getMessage());
            logPrintE("An error occured during parsing\n   Message: %s\n", message);
            XMLString::release( & message );
            exit(1);
        }
        catch (const DOMException& e)
        {
            char * message = XMLString::transcode( e.msg );
            logPrintE("An error occured during parsing\n   Message: %s\n", message);
            XMLString::release( & message );
            exit(1);
        }
        if( errReporter.errorsEncountered() > 0 )
        {
            logPrintE("There were non fatal errors in the configuration file !\n");
            logPrintE("Please check the file and start again.\n");
            exit(1);
        }

        DOMDocument * doc = parser->getDocument();
        DOMElement * root = doc->getDocumentElement();
        Node * node = new Node();
        node->setParent( root );
        char * tempName = XMLString::transcode( root->getLocalName());
        logPrintI("Root node is %s\n", tempName);
        XMLString::release( & tempName );

        const XMLCh* xmlspace = root->getNamespaceURI();
        if (xmlspace != NULL) {
            char * tempSpace = XMLString::transcode( xmlspace );
            logPrintI("Namespace is %s\n", tempSpace);
            XMLString::release( & tempSpace );
        }
        else {
            logPrintW("Not using namespaces!\n");
        }


        // get the configuration part
        XMLCh * configurationCh = XMLString::transcode( "configuration" );
        //auto_ptr<DOMNodeList> list( root->getElementsByTagName( configurationCh.get() ));
        DOMNodeList * list = root->getElementsByTagNameNS(xmlspace, configurationCh);
        XMLString::release( & configurationCh );
        if( list->getLength() != 1 )
        {
            logPrintE("not valid config file, not exactly one configuration tag\n");
            logPrintI("%d configurations.\n", list->getLength());
            exit(1);
        }

        logPrintI("parsing configuration section\n");

        // parse configuration elements subelements
        DOMElement * config = (DOMElement *)list->item(0);
        //auto_ptr<DOMNodeList> configlist( config->getChildNodes());
        DOMNodeList * configlist = config->getChildNodes();
        unsigned int i;
#ifdef OT_LOCAL_GRAPH
        Node * configNode = new Node();
        configNode->type = "configuration";
#endif //OT_LOCAL_GRAPH

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
#ifdef OT_LOCAL_GRAPH
                base->type = tagName;
#endif // OT_LOCAL_GRAPH
                base->setParent( configElement );
                logPrintI("config for %s\n", tagName.c_str());

                //auto_ptr<DOMNodeList> nodelist( configElement->getChildNodes());
                DOMNodeList * nodelist = configElement->getChildNodes();

                unsigned int j;
#ifdef OT_LOCAL_GRAPH
                        // OT_GRAPH
                configNode->addChild(*base);
                        // OT_GRAPH
#endif OT_LOCAL_GRAPH
                
                for( j = 0; j < nodelist->getLength(); j++ )
                {
                    if( nodelist->item(j)->getNodeType() == DOMNode::ELEMENT_NODE )
                    {
                        DOMElement * element = (DOMElement *)nodelist->item(j);
                        ///FIXXXME: What was the assignment good for?
                        ConfigNode * child = buildConfigTree( element );
#ifdef OT_LOCAL_GRAPH
                        // OT_GRAPH
                        base->addChild(*child);
                        // OT_GRAPH
#endif OT_LOCAL_GRAPH
                    }
                }
                Module * module = context.getModule( tagName );
                if( module != NULL )
                {
                    module->init( *attributes, base );
                }
            }
        }

#ifdef OT_LOCAL_GRAPH
        logPrintE("Number of config nodes %d\n", (configNode->countAllChildren()));
        if (configNode->countAllChildren() > 0){
            
            node->addChild(*configNode);
        }


#endif //OT_LOCAL_GRAPH


        logPrintI("parsing tracker tree section\n");

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
            
            Node * child = buildTree( element );
#ifdef OT_LOCAL_GRAPH
            //OT_GRAPH
            node->addChild(*child);
            //OT_GRAPH
#endif // OT_LOCAL_GRAPH
        }
#ifdef OT_LOCAL_GRAPH

        delete parser;
        //        root->release();

#endif // OT_LOCAL_GRAPH


        return node;
#endif //USE_XERCES


#ifdef USE_TINYXML
    Node * ConfigurationParser::parseConfiguration(TiXmlDocument* document) 
    {

        TiXmlElement* root = document->RootElement();
        Node * node = new Node();
        node->setParent( root );
        const char* tempName = root->Value();

        logPrintI("Root node is %s\n", tempName);

        const char* xmlspace = NULL; //root->getNamespaceURI();
        if (xmlspace != NULL) {
            logPrintI("Namespace is %s\n", xmlspace);
        }
        else {
            logPrintW("Not using namespaces!\n");
        }

        // get the configuration part
        const char* configurationCh = "configuration";

        TiXmlElement * config = root->FirstChildElement(configurationCh);
        if(config->NextSiblingElement(configurationCh))
        {
            logPrintE("not valid config file, not exactly one configuration tag\n");
            exit(1);
        }

        logPrintI("parsing configuration section\n");

#ifdef OT_LOCAL_GRAPH
        Node * configNode = new Node();
        configNode->type = "configuration";
#endif //OT_LOCAL_GRAPH

        TiXmlElement * configElement = config->FirstChildElement();
        while(configElement)
        {
            std::auto_ptr<StringTable> attributes( parseElement( configElement ));
            std::string tagName = configElement->Value();

            ConfigNode * base = new ConfigNode( *attributes );
#ifdef OT_LOCAL_GRAPH
            base->type = tagName;
#endif // OT_LOCAL_GRAPH

            base->setParent( configElement );
            logPrintI("config for %s\n", tagName.c_str());

            TiXmlElement * element = configElement->FirstChildElement();
            while(element)
            {
                ConfigNode * child = buildConfigTree( element );
#ifdef OT_LOCAL_GRAPH
            //OT_GRAPH
                base->addChild(*child);
            //OT_GRAPH
#endif // OT_LOCAL_GRAPH
                
                element = element->NextSiblingElement();
            }

            Module * module = context.getModule( tagName );
            if( module != NULL )
            {
                module->init( *attributes, base );
            }
#ifdef OT_LOCAL_GRAPH
            configNode->addChild(*base);
#endif //OT_LOCAL_GRAPH


            configElement = configElement->NextSiblingElement();
        }

#ifdef OT_LOCAL_GRAPH
        node->addChild(*configNode);
#endif //OT_LOCAL_GRAPH


        logPrintI("parsing tracker tree section\n");

        // parse the rest of the elements

        TiXmlElement * element = root->FirstChildElement();
        while(element)
        {
            if(strcmp(element->Value(), "configuration")!=0){
               Node * child= buildTree( element );
#ifdef OT_LOCAL_GRAPH
               node->addChild(*child);
#endif // OT_LOCAL_GRAPH
            }

            element = element->NextSiblingElement();
        }

#ifdef OT_LOCAL_GRAPH
        delete root;
#endif // OT_LOCAL_GRAPH

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

    // This method parses an XML configuration file.
    Node * ConfigurationParser::parseConfigurationFile(const std::string& filename)
    {
#ifdef USE_XERCES
        ACE_Env_Value<std::string> otroot(ACE_TEXT("OTROOT"), "");
        std::string otrootvalue = (std::string)otroot;
        std::string otdatadir;
        if( otrootvalue.compare("") != 0 )
        {
            //otdatadir = otrootvalue + "/data";
            otdatadir = otrootvalue;// + "/share";
            //context->addDirectoryLast(otdatadir);
        }
        LocalFileInputSource input_source((const XMLCh*) XMLString::transcode(filename.c_str()));
        return parseConfiguration(&input_source);
#endif //USE_XERCES
#ifdef USE_TINYXML
        TiXmlDocument* document = new TiXmlDocument();

        if(!document->LoadFile(filename.c_str()))
        {
            logPrintE("An error occured during parsing\n   Message: %s\n", document->ErrorDesc());
            exit(1);
        }
		return parseConfiguration(document);
#endif //USE_TINYXML
    }

    // This method parses an XML configuration string.

    Node * ConfigurationParser::parseConfigurationString(const char* xmlstring)
    {
#ifdef USE_XERCES
		MemBufInputSource* input_source = new MemBufInputSource(
                                                                (const XMLByte*) xmlstring
                                                                , strlen(xmlstring)
                                                                , otMemBufId
                                                                , false
                                                                );
        Node* node = parseConfiguration(input_source);
        delete input_source;
        return node;
#endif //USE_XERCES
#ifdef USE_TINYXML
        TiXmlDocument* document = new TiXmlDocument();

        if(!document->Parse(xmlstring))
        {
            logPrintE("An error occured during parsing\n   Message: %s\n", document->ErrorDesc());
            exit(1);
        }
		return parseConfiguration(document);
#endif //USE_TINYXML
    }


} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of ConfigurationParser.cxx
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
