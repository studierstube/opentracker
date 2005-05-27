 /* ========================================================================
  * Copyright (C) 2003  Vienna University of Technology
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
  * PROJECT: PUCServer
  * ======================================================================== */
/** source file for PUCMessages.
  *
  * @author Thomas Psik
  *
  * $Id: PUCMessages.cpp 4130 2005-02-03 18:42:36Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include <iostream>

#include "PUCBasicLib/PUCMessages.h"
#include "PUCBasicLib/DOMTreeErrorReporter.h"

using namespace std;
XERCES_CPP_NAMESPACE_USE

#if defined(_DEBUG_PUC) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

const string PUCMessage::ROOT_STRING = "message";
const string PUCMessage::UNDEFINED_STRING = "undefined";


const string PUCMessage::XML_START_MESSAGE = "<?xml version=\"1.0\" encoding=\"utf-16\"?><message>";
const string PUCMessage::XML_END_MESSAGE = "</message>";


const string PUCServerInformationRequest::TYPE_STRING = "server-information-request";


PUCMessage * PUCMessage::Decode( string xmlMessage )
{

	DOMTreeErrorReporter errReporter;
	XercesDOMParser * parser;

	MemBufInputSource memBuffer(
		(const unsigned char *)(xmlMessage.c_str()), // const XMLByte *const srcDocBytes, 
		xmlMessage.size(), // const unsigned int byteCount, 
		"xmlMessage", // const XMLCh *const bufId, 
		false // const bool adoptBuffer=false
	);
			
	try
    {
		parser = new XercesDOMParser();
		parser->setValidationScheme( XercesDOMParser::Val_Never );
		parser->setIncludeIgnorableWhitespace( false );
		parser->useScanner(XMLUni::fgWFXMLScanner);
		parser->setErrorHandler( &errReporter );
	
		 // parse PUC Message, returns a message of correct type
		
		parser->parse( memBuffer);
    }
    catch (const XMLException& e)
    {
        char * message = XMLString::transcode( e.getMessage());
        cout << "An error occured during parsing\n   Message: "
            << message << endl;
        XMLString::release( &message );
        exit(1);
    }
    catch (const DOMException& e)
    {
        char * message = XMLString::transcode( e.msg );
        cout << "An error occured during parsing\n   Message: "
            << message << endl;
        XMLString::release( &message );
        exit(1);
    }
    if( errReporter.errorsEncountered() > 1 )
    {
        cout << "There were non fatal errors in the configuration file !\n"
             << "Please check the file and start again." << endl;
        exit(1);
    }

    DOMDocument * doc = parser->getDocument();
    DOMElement * root = doc->getDocumentElement();

    char * tempName = XMLString::transcode( root->getTagName());
	DEBUG_CODE(cout << "Root node is " << tempName << endl);
	
	// read the first element to ensure this is a message
	if ( tempName != PUCMessage::ROOT_STRING)
	{
        XMLString::release( &tempName );
		return NULL;
	}
    XMLString::release( &tempName );    

	DOMNodeList * elmentList = root->getChildNodes();
    unsigned int i;
    for( i = 0; i < elmentList->getLength(); i ++ )
    {
        if( elmentList->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xmlElement = (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *)elmentList->item(i);
            char * tempName = XMLString::transcode( xmlElement->getTagName());
            string tagName = tempName;
            XMLString::release( &tempName );
            DEBUG_CODE(cout << "element " << tagName  << endl);
			// create a specific PUCMessage Object based on the tag name
			PUCMessage *result = NULL;

		/*	else 
			else if ( xml.Name == PUCDeviceSpec.TYPE_STRING )
				result = new PUCDeviceSpec( xml );
			else */
            if ( tagName == PUCServerInformationRequest::TYPE_STRING )
				result = new PUCServerInformationRequest( xmlElement  );
            else if ( tagName == PUCSpecRequest::TYPE_STRING )
				result = new PUCSpecRequest( xmlElement );
            else if ( tagName == PUCStateChangeRequest::TYPE_STRING )
				result = new PUCStateChangeRequest( xmlElement );
            else if ( tagName == PUCFullStateRequest::TYPE_STRING )
				result = new PUCFullStateRequest( xmlElement );
			else if ( tagName == PUCStateChangeNotification::TYPE_STRING )
				result = new PUCStateChangeNotification( xmlElement );
            else if ( tagName == PUCCommandInvokeRequest::TYPE_STRING )
				result = new PUCCommandInvokeRequest( xmlElement );
			/*
			else if ( xml.Name == PUCAlertInformation.TYPE_STRING )
				result = new PUCAlertInformation( xml );
			*/	        
			if ( result == NULL || !result->isValid() )
			{
				cerr <<  "Invalid Message Parsed: " << xmlMessage << endl;
				return NULL;
			}
			return result;
		}
	}
	return NULL;
}

string PUCServerInformationRequest::getXML() const
{
/*	StringWriter strout = new StringWriter();
	DOMWriter xml = new XmlTextWriter( strout );

	xml.WriteStartDocument();
	xml.WriteStartElement( ROOT_STRING );
	xml.WriteStartElement( TYPE_STRING );
	xml.WriteEndElement();
	xml.WriteEndElement();
	xml.WriteEndDocument();

	return strout.ToString();
*/

	return PUCMessage::XML_START_MESSAGE + 
		"<" + PUCServerInformationRequest::TYPE_STRING +"/>" +
		   PUCMessage::XML_END_MESSAGE;
}

const std::string PUCDeviceInfo::NAME_STRING = "name";
const std::string PUCDeviceInfo::PORT_STRING = "port";
const std::string PUCDeviceInfo::DEVICE_STRING = "device";

void PUCDeviceInfo::readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml)
{
	DEBUG_CODE(cout << "PUCDeviceInfo::readXml() not implemented"<<endl);
}

std::string PUCDeviceInfo::getXML() const
{
	char buffer [40];
	std::string xml;

	xml += "<" + PUCDeviceInfo::DEVICE_STRING +">";
	xml += "<" + PUCDeviceInfo::NAME_STRING +">"+name+"</" + PUCDeviceInfo::NAME_STRING +">";
	xml += "<" + PUCDeviceInfo::PORT_STRING +">";
	xml += itoa(port,buffer,10);
	xml += "</" + PUCDeviceInfo::PORT_STRING+">"; // device port is fixed !!
	xml += "</" + PUCDeviceInfo::DEVICE_STRING +">";
	return xml;
}

/*
*
*
*
*/


const std::string PUCServerInformation::TYPE_STRING = "server-information";
const std::string PUCServerInformation::SERVER_NAME_STRING = "server-name";


void PUCServerInformation::readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml)
{
	// not used by servers 
	DOMNodeList * elmentList = xml->getChildNodes();
    unsigned int i;
    for( i = 0; i < elmentList->getLength(); i ++ )
    {
        if( elmentList->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xmlElement = (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *)elmentList->item(i);
            char * tempName = XMLString::transcode( xmlElement->getTagName());
            string tagName = tempName;
            XMLString::release( &tempName );
            DEBUG_CODE(cout << "element " << tagName  << endl);

			if (tagName == PUCDeviceInfo::DEVICE_STRING) 
			{
				PUCDeviceInfo d(xmlElement);
				if (d.isValid()) 
				{
					devices.push_back(d);
				}
				else
				{
				   DEBUG_CODE(cout << "device error in [" << tagName  << "]" <<endl);
				   return; // message is not valid !
		
				}
			}
		}
	}
	valid = !serverName.empty();
}

std::string PUCServerInformation::getXML() const
{

	std::string msg;

	msg = PUCMessage::XML_START_MESSAGE;
	msg += "<" + TYPE_STRING +">";
	msg += "<" + SERVER_NAME_STRING + ">" + serverName + "</" + SERVER_NAME_STRING + ">" ;

	for (unsigned int i=0; i< devices.size(); i++)
	{
		msg += devices[i].getXML();
	}

	msg += "</" + TYPE_STRING +">";
	msg += PUCMessage::XML_END_MESSAGE;

	return msg;
}


/*
*
*
*
*/

const std::string PUCSpecRequest::TYPE_STRING="spec-request";

void PUCSpecRequest::readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml)
{
	// nothing to parse in this message
	valid = true;
}

std::string PUCSpecRequest::getXML() const
{
	// not used by servers
	return "not valid";
}


/*
*
*
*
*/

const std::string PUCDeviceSpec::TYPE_STRING = "device-spec";
	
void PUCDeviceSpec::readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml)
{
	// not used in server ...

}

std::string PUCDeviceSpec::getXML() const
{
	std::string msg;


	/*
	<spec name="WMP9" version="PUC/2.0">

    <labels>
       <label>Windows Media Player</label>
     </labels>

	msg = PUCMessage::XML_START_MESSAGE;
	msg += "<" + TYPE_STRING +">";
//	msg += "<spec name=\"STB\" version=\"PUC/2.0\">";
  //  msg += "<labels>";
//	msg += "<label>Windows Media Player</label>";
 //   msg += "</labels>";
	
	msg += storedSpec;
	
	
		
//	msg += "</spec>";
	msg += "</" + TYPE_STRING +">";
	msg += PUCMessage::XML_END_MESSAGE;
  */

	msg = PUCMessage::XML_START_MESSAGE;
	msg += "<" + TYPE_STRING +">";
	msg += "<spec>";
	
	msg += storedSpec; // got from interface
	
	msg += "</spec>";
	msg += "</" + TYPE_STRING +">";
	msg += PUCMessage::XML_END_MESSAGE;


	return msg;
}

// ---------------------------------------------------------------------------------------
/*
*
*
*
*/

const std::string PUCStateChangeRequest::TYPE_STRING = "state-change-request";
	
void PUCStateChangeRequest::readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml)
{
	DOMNodeList * elmentList = xml->getChildNodes();
    unsigned int i;
    for( i = 0; i < elmentList->getLength(); i ++ )
    {
        if( elmentList->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xmlElement = (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *)elmentList->item(i);
            char * tempName = XMLString::transcode( xmlElement->getTagName());
            string tagName = tempName;
            XMLString::release( &tempName );
            string text;
			
			DEBUG_CODE(cout << "element " << tagName  << endl);
			if (elmentList->item(i)->hasChildNodes()) 
			{
				const XMLCh*textValue = elmentList->item(i)->getFirstChild()->getNodeValue();
				if (textValue != NULL) 
				{
					char * textInput = XMLString::transcode(textValue );
					text = textInput;
                    XMLString::release( &textInput );
					DEBUG_CODE(cout << "  text [" << text <<"]" << endl);
				}
			}

			if (tagName == "state")
				stateName = text;
			else if (tagName == "value")
				stateValue = text;
		}
	}
	valid = (!stateName.empty()) && (!stateValue.empty());
}
// ---------------------------------------------------------------------------------------

std::string PUCStateChangeRequest::getXML() const
{
	// not used in server ...
	return "not needed";
}

// ---------------------------------------------------------------------------------------

/*
*
*
*
*/

const std::string PUCFullStateRequest::TYPE_STRING = "full-state-request";
	
void PUCFullStateRequest::readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml)
{
	// no information there
	// <full-state-request />
	valid = true;

}
// ---------------------------------------------------------------------------------------

std::string PUCFullStateRequest::getXML() const
{
	// not used in server ...
	return "not needed";
}


// ---------------------------------------------------------------------------------------

/*
*
*
*
*/

const std::string PUCStateChangeNotification::TYPE_STRING = "state-change-notification";
	
void PUCStateChangeNotification::readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml)
{
	// not used in server
	// no information there
	// <full-state-request />

}
// ---------------------------------------------------------------------------------------
/*
<?xml version="1.0" encoding="utf-16"?>
<message>
	<state-change-notification>
		<state>RADIUS</state>
		<value>3</value>
	</state-change-notification>
</message>
*/

/*
<?xml version="1.0" encoding="utf-16"?><message><state-change-notification><state>SIZE</state><value>2</value></state-change-notification></message>
<?xml version="1.0" encoding="utf-16"?><message><state-change-notification><state>RADI</state><value>3</value></state-change-notification></message>
*/

std::string PUCStateChangeNotification::getXML() const
{
	std::string msg;

	msg = PUCMessage::XML_START_MESSAGE;
	msg += "<" + TYPE_STRING +">";
	msg += "<state>";
	msg += stateName;
	msg += "</state>";
	msg += "<value>";
	msg += stateValue;
	msg += "</value>";
	msg += "</" + TYPE_STRING +">";
	msg += PUCMessage::XML_END_MESSAGE;

	return msg;
}




const std::string PUCCommandInvokeRequest::TYPE_STRING = "command-invoke-request";
	
void PUCCommandInvokeRequest::readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml)
{
	DOMNodeList * elmentList = xml->getChildNodes();
    unsigned int i;
    for( i = 0; i < elmentList->getLength(); i ++ )
    {
        if( elmentList->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xmlElement = (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *)elmentList->item(i);
            char * tempName = XMLString::transcode( xmlElement->getTagName());
            string tagName = tempName;
            XMLString::release( &tempName );
            string text;
			
			DEBUG_CODE(cout << "element " << tagName  << endl);
			if (elmentList->item(i)->hasChildNodes()) 
			{
				const XMLCh*textValue = elmentList->item(i)->getFirstChild()->getNodeValue();
				if (textValue != NULL) 
				{
					char * textInput = XMLString::transcode(textValue );
					text = textInput;
                    XMLString::release( &textInput );
					DEBUG_CODE(cout << "  text [" << text <<"]" << endl);
				}
			}

			if (tagName == "command")
				commandName = text;
		}
	}
	valid = !commandName.empty() ;
}
// ---------------------------------------------------------------------------------------

std::string PUCCommandInvokeRequest::getXML() const
{
	// not used in server ...
	return "not needed";
}

// ---------------------------------------------------------------------------------------
