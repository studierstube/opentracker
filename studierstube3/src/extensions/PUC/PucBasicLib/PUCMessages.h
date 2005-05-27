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
/** header file for PUCMessages.
  *
  * @author Thomas Psik
  *
  * $Id: PUCMessages.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _PUCMESSAGES_H
#define _PUCMESSAGES_H

#include <string>
#include <vector>
#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN
class DOMElement;
class DOMDocument;
XERCES_CPP_NAMESPACE_END


/**
 * @ingroup pucbasicLib
 */
 
 class PUCMessage
{
public:
	static const std::string ROOT_STRING ;
	static const std::string UNDEFINED_STRING ;
	static const std::string XML_START_MESSAGE;
	static const std::string XML_END_MESSAGE;

	enum PUCMessageType {
		NO_MESSAGE = 0,
		SERVER_INFORMATION_REQUEST,
		SERVER_INFORMATION,
		SPEC_REQUEST, 
		DEVICE_SPEC,

		FULL_STATE_REQ,
		STATE_CHANGE_REQ,
		STATE_CHANGE_NOT,
		COMMAND_INVOKE_REQ

	};
	
	PUCMessage() { valid = false; }

	static PUCMessage *Decode( std::string xmlMessage );

	bool isValid() const { return valid; }
	
	virtual std::string getXML() const {return "<message/>";};
	virtual std::string toString() const { return "not a message !!";}
	virtual int getType() const { return NO_MESSAGE ; }
protected:
	bool valid;
};

class PUCServerInformationRequest : public PUCMessage
{
public:

	static const std::string TYPE_STRING;
	
	PUCServerInformationRequest() { valid = true; }
	PUCServerInformationRequest( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { valid = true; }

	virtual std::string getXML() const;
	virtual std::string toString() const { return TYPE_STRING; }
	virtual int getType() const { return PUCMessage::SERVER_INFORMATION_REQUEST ; }

protected:
	
};


class PUCDeviceInfo
{
	static const std::string NAME_STRING;
	static const std::string PORT_STRING;

public:
	static const std::string DEVICE_STRING;

	PUCDeviceInfo(std::string name_, int port_) { name = name_; port = port_;}
	PUCDeviceInfo( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { readXml(xml);}

	void readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml);
	std::string getXML() const;

	bool isValid() const { return !name.empty(); }
protected:
	std::string name;
	int port;
};


typedef	std::vector<PUCDeviceInfo> PUCDeviceArray;

class PUCServerInformation : public PUCMessage
{
	static const std::string SERVER_NAME_STRING;

public:

	static const std::string TYPE_STRING;
	
	PUCServerInformation( const std::string serverName_, PUCDeviceArray &devices_ ) { serverName = serverName_; devices = devices_; }
	PUCServerInformation( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { readXml(xml);}

	virtual void readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml);

	virtual std::string getXML() const;
	virtual std::string toString() const { return TYPE_STRING; }
	virtual int getType() const { return PUCMessage::SERVER_INFORMATION; }

protected:
	std::string serverName;
	PUCDeviceArray devices;
};


class PUCSpecRequest : public PUCMessage
{
public:

	static const std::string TYPE_STRING;
	
	PUCSpecRequest () { valid = true; }
	PUCSpecRequest ( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { readXml(xml);}

	virtual void readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml);

	virtual std::string getXML() const;
	virtual std::string toString() const { return TYPE_STRING; }
	virtual int getType() const { return PUCMessage::SPEC_REQUEST; }

protected:
	
};

class PUCDeviceSpec : public PUCMessage
{
public:

	static const std::string TYPE_STRING;
	
	PUCDeviceSpec (std::string msg_) { storedSpec = msg_; valid = true; }
	PUCDeviceSpec ( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { readXml(xml);}

	virtual void readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml);

	virtual std::string getXML() const;
	virtual std::string toString() const { return TYPE_STRING; }
	virtual int getType() const { return PUCMessage::DEVICE_SPEC; }

protected:
	std::string storedSpec;
};

class PUCStateChangeRequest : public PUCMessage
{
public:

	static const std::string TYPE_STRING;
	
	PUCStateChangeRequest () { valid = true; }
	PUCStateChangeRequest ( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { readXml(xml);}

	virtual void readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml);

	virtual std::string getXML() const;
	virtual std::string toString() const { return TYPE_STRING; }
	virtual int getType() const { return PUCMessage::STATE_CHANGE_REQ; }
	
	std::string getStateName() const { return stateName; }
	std::string getStateValue() const { return stateValue; }

protected:
	std::string stateName;
	std::string stateValue;
};

class PUCFullStateRequest : public PUCMessage
{
public:

	static const std::string TYPE_STRING;
	
	PUCFullStateRequest () { valid = true; }
	PUCFullStateRequest ( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { readXml(xml);}

	virtual void readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml);

	virtual std::string getXML() const;
	virtual std::string toString() const { return TYPE_STRING; }
	virtual int getType() const { return PUCMessage::FULL_STATE_REQ; }

protected:
	
};

class PUCStateChangeNotification : public PUCMessage
{
public:
	static const std::string TYPE_STRING;
	
	PUCStateChangeNotification (std::string stateName_, std::string stateValue_) { stateName = stateName_; stateValue = stateValue_;valid = true; }
	PUCStateChangeNotification ( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { readXml(xml);}

	virtual void readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml);

	virtual std::string getXML() const;
	virtual std::string toString() const { return TYPE_STRING; }
	virtual int getType() const { return PUCMessage::STATE_CHANGE_NOT; }

protected:
	std::string stateName;
	std::string stateValue;
};



class PUCCommandInvokeRequest : public PUCMessage
{
public:
	static const std::string TYPE_STRING;
	
	PUCCommandInvokeRequest (std::string commandName_) { commandName = commandName_;valid = true; }
	PUCCommandInvokeRequest ( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml) { readXml(xml);}

	virtual void readXml(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * xml);

	virtual std::string getXML() const;
	virtual std::string toString() const { return TYPE_STRING; }
	virtual int getType() const { return PUCMessage::COMMAND_INVOKE_REQ; }

	std::string getCommandName() const { return commandName;}
protected:
	std::string commandName ;
};


#endif // !defined(_PUCMESSAGES_H)
