 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
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
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** c++ header for XmlHandler
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: XmlHandler.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _XMLHANDLER_H_
#define _XMLHANDLER_H_

#ifdef WIN32
#pragma warning (disable: 4786)
#pragma warning (disable: 4788)
#endif

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp> 
#include "ComponentInstance.h"
#include "AssemblyStep.h"

using namespace xercesc;

/**
 * 
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class XmlHandler : public DefaultHandler {

private:
	std::stack<std::string> elementStack;
	std::map<std::string,ComponentInstance>* componentInstances;
	std::vector<AssemblyStep*>* assemblySteps;

	enum_unit currUnit;

	bool isCurrComponentInstance;
	bool isCurrDimension;
	ComponentInstance currComponentInstance;
	bool isCurrConnector;
	std::string currConnectorName;

	bool isCurrAssembly;
	bool isCurrComponentInstanceAddition;
	ComponentInstanceAddition currComponentInstanceAddition;
	bool isCurrConnection;
	bool isCurrRotation;
	Rotation currRotation;

public:
	XmlHandler() : componentInstances(new std::map<std::string,ComponentInstance>), assemblySteps(new std::vector<AssemblyStep*>) { }

	void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const Attributes &attr);   // Content handler
    void endElement(const XMLCh *const uri,const XMLCh *const localname,const XMLCh *const qname);
	void characters(const XMLCh *const chars,const unsigned int length);

	void fatalError(const SAXParseException&);          // Error handler
	void error(const SAXParseException &exception);
	void warning(const SAXParseException &exception);

	std::map<std::string,ComponentInstance>* getComponentInstances() { return (this->componentInstances); }   // Getter methods
	std::vector<AssemblyStep*>* getAssemblySteps() { return (this->assemblySteps); }

};

#endif

/* ===========================================================================
    End of XmlHandler.h
=========================================================================== */
