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
/** c++ source for XmlHandler
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: XmlHandler.cxx 3835 2004-10-06 10:08:47Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include "XmlHandler.h"
#include <iostream>
#include <sstream>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLString.hpp>

void XmlHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const Attributes& attr) {
	std::string str_uri = std::string(XMLString::transcode(uri));
	std::string str_localname = std::string(XMLString::transcode(localname));
	std::string str_qname = std::string(XMLString::transcode(qname));

	if (!this->elementStack.empty()) {

		if (this->elementStack.top() == "furniture") {   // <furniture>...</furniture>
			if (str_localname == "component") {   // New component started
				this->isCurrComponentInstance = true;
				this->currComponentInstance = ComponentInstance();
			}
			if (str_localname == "assembly") {   // Assembly started
				this->isCurrAssembly = true;
			}
		}
		
		if (this->elementStack.top() == "component") {   // <component>...</component>
			if (str_localname == "dimension") {   // Dimension started
				this->currUnit = str2unit(std::string(XMLString::transcode(attr.getValue(XMLString::transcode("unit")))));
				this->isCurrDimension = true;
			}
			if (str_localname == "connector") {   // New connector started
				this->currUnit = str2unit(std::string(XMLString::transcode(attr.getValue(XMLString::transcode("unit")))));
				this->isCurrConnector = true;
				this->currConnectorName = std::string(XMLString::transcode(attr.getValue(XMLString::transcode("name"))));
				this->currComponentInstance.getConnectors()[currConnectorName] = *new Connector;
			}
			if (str_localname == "instance") {   // Create new instance
				std::string componentInstanceName = std::string(XMLString::transcode(attr.getValue(XMLString::transcode("name"))));
				(*this->componentInstances)[componentInstanceName] = *new ComponentInstance(this->currComponentInstance);
				const XMLCh* tracker = attr.getValue(XMLString::transcode("tracker"));
				if (tracker) {
					(*this->componentInstances)[componentInstanceName].setTracker(XMLString::parseInt(tracker));
				}
			}
		}
		
		if (this->elementStack.top() == "shape") {   // <shape>...</shape>
			if (str_localname == "cube") {
				this->currComponentInstance.setShape(new Cube);
			}
		}

		if (this->elementStack.top() == "assembly") {   // <assembly>...</assembly>
			if (str_localname == "add-component") {   // New component addition started
				this->isCurrComponentInstanceAddition = true;
				this->currComponentInstanceAddition = *new ComponentInstanceAddition();
				this->currComponentInstanceAddition.setComponentInstanceName(std::string(XMLString::transcode(attr.getValue(XMLString::transcode("name")))));
			}
			if (str_localname == "rotate") {   // New rotation started
				this->isCurrRotation = true;
				this->currRotation = *new Rotation();
				this->currRotation.setAxis(str2axis(std::string(XMLString::transcode(attr.getValue(XMLString::transcode("axis"))))));
				this->currRotation.setAngle(str2angle(std::string(XMLString::transcode(attr.getValue(XMLString::transcode("angle"))))));
			}
		}

		if (this->elementStack.top() == "add-component") {   // <add-component>...</add-component>
			if (str_localname == "connect") {   // New connection started
				this->isCurrConnection = true;
                this->currComponentInstanceAddition.getConnections().push_back(Connection());
			}
		}

		if (this->elementStack.top() == "connect") {   // <connect>...</connect>
			if (str_localname == "connector-ref") {   // New connection started
				Connection& conn = this->currComponentInstanceAddition.getConnections().back();
				if (conn.connectorName1 == "") {
					conn.connectorName1 = std::string(XMLString::transcode(attr.getValue(XMLString::transcode("name"))));
					const XMLCh* componentRef = attr.getValue(XMLString::transcode("componentRef"));
					if (componentRef) {
						conn.componentInstanceName1 = std::string(XMLString::transcode(componentRef));
					} else {
						conn.componentInstanceName1 = this->currComponentInstanceAddition.getComponentInstanceName();
					}
				} else {
					conn.connectorName2 = std::string(XMLString::transcode(attr.getValue(XMLString::transcode("name"))));
					const XMLCh* componentRef = attr.getValue(XMLString::transcode("componentRef"));
					if (componentRef) {
						conn.componentInstanceName2 = std::string(XMLString::transcode(componentRef));
					} else {
						conn.componentInstanceName2 = this->currComponentInstanceAddition.getComponentInstanceName();
					}
				}
			}
		}

	}

	// Add element to stack
	this->elementStack.push(str_localname);
}

void XmlHandler::endElement(const XMLCh *const uri,const XMLCh *const localname,const XMLCh *const qname) {
	std::string str_uri = std::string(XMLString::transcode(uri));
	std::string str_localname = std::string(XMLString::transcode(localname));
	std::string str_qname = std::string(XMLString::transcode(qname));

	// Remove element from stack
	this->elementStack.pop();

	if (!this->elementStack.empty()) {

		if (this->elementStack.top() == "furniture") {   // <furniture>...</furniture>
			if (str_localname == "component") {
				this->isCurrComponentInstance = false;
			}
			if (str_localname == "assembly") {
				this->isCurrAssembly = false;
			}
		}

		if (this->elementStack.top() == "component") {   // <component>...</component>
			if (str_localname == "dimension") {
				this->isCurrDimension = false;
			}
			if (str_localname == "connector") {
				this->isCurrConnector = false;
			}
		}

		if (this->elementStack.top() == "assembly") {   // <assembly>...</assembly>
			if (str_localname == "add-component") {
				this->isCurrComponentInstanceAddition = false;
				this->assemblySteps->push_back(new ComponentInstanceAddition(this->currComponentInstanceAddition));
			}
			if (str_localname == "rotate") {
				this->isCurrRotation = false;
				this->assemblySteps->push_back(new Rotation(this->currRotation));
			}
		}

		if (this->elementStack.top() == "add-component") {   // <add-component>...</add-component>
			if (str_localname == "connect") {
				this->isCurrConnection = false;
			}
		}

	}
}

void XmlHandler::characters(const XMLCh *const chars,const unsigned int length) {
	std::string str_chars = std::string(XMLString::transcode(chars));
	std::istringstream iss(str_chars);
	double temp_double;

	if (this->isCurrComponentInstance && this->isCurrDimension) {
		if (this->elementStack.top() == "width") {
			iss >> temp_double;
			this->currComponentInstance.setWidth((float)temp_double, this->currUnit);
		}
		if (this->elementStack.top() == "height") {
			iss >> temp_double;
			this->currComponentInstance.setHeight((float)temp_double, this->currUnit);
		}
		if (this->elementStack.top() == "depth") {
			iss >> temp_double;
			this->currComponentInstance.setDepth((float)temp_double, this->currUnit);
		}
	}
	
	if (this->isCurrComponentInstance && this->isCurrConnector) {
		if (this->elementStack.top() == "x") {
			iss >> temp_double;
			this->currComponentInstance.getConnectors()[this->currConnectorName].setX((float)temp_double, this->currUnit);
		}
		if (this->elementStack.top() == "y") {
			iss >> temp_double;
			this->currComponentInstance.getConnectors()[this->currConnectorName].setY((float)temp_double, this->currUnit);
		}
		if (this->elementStack.top() == "z") {
			iss >> temp_double;
			this->currComponentInstance.getConnectors()[this->currConnectorName].setZ((float)temp_double, this->currUnit);
		}
	}

}

void XmlHandler::fatalError(const SAXParseException &e) {
	std::cerr << "\nFatal Error at file " << e.getSystemId()
	          << ", line " << e.getLineNumber()
	          << ", char " << e.getColumnNumber()
			  << "\n  Message: " << e.getMessage() << std::endl;
	throw e;
}

void XmlHandler::error(const SAXParseException &e) {
	std::cerr << "\nError at file " << e.getSystemId()
	          << ", line " << e.getLineNumber()
	          << ", char " << e.getColumnNumber()
	          << "\n  Message: " << e.getMessage() << std::endl;
	throw e;
}

void XmlHandler::warning(const SAXParseException &e) {
	std::cerr << "\nWarning at file " << e.getSystemId()
	          << ", line " << e.getLineNumber()
	          << ", char " << e.getColumnNumber()
	          << "\n  Message: " << e.getMessage() << std::endl;
}
