/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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

 /* ======================================================================= */
/** BAUML Library
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: bauml.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __bauml_h__
#define __bauml_h__

#include "arroom.h"
#include "arbuilding.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
//#include <xercesc/dom/DOMElement.hpp>
//#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
//#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/util/XMLUni.hpp>

/*
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument DOMDocument;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMElement DOMElement;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMNode DOMNode;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementation DOMImplementation;
*/

#define XERCES XERCES_CPP_NAMESPACE_QUALIFIER 

#define MAKE_NODE(name, node) \
XERCES DOMElement *element_##name; \
{ \
    XMLCh xmlstr1[1000]; \
    XERCES XMLString::transcode(node, xmlstr1, 999); \
    element_##name = document->createElement(xmlstr1); \
}

#define MAKE_NODEP(name, node, parent) \
    MAKE_NODE(name, node) \
    element_##parent->appendChild(element_##name);

#define SET_ATTR(name, attr, value) \
{ \
    XMLCh xmlstr1[1000]; \
    XMLCh xmlstr2[1000]; \
    XERCES XMLString::transcode(attr, xmlstr1, 99); \
    XERCES XMLString::transcode(value, xmlstr2, 999); \
    element_##name->setAttributeNS(0, xmlstr1, xmlstr2); \
}

//XERCES_CPP_NAMESPACE_USE

class BAUML
{
private:
//    XercesDOMParser *parser;
//    ErrorHandler *errhandler;
    ARBuilding building;

protected:
    void parseBAUML(XERCES DOMDocument *document);
    void parseOCAR(XERCES DOMDocument *document);
    bool getElementAttribute(XERCES DOMElement *element, const char *name, char *value, int maxlen);
    void parseRoom_BAUML(XERCES DOMElement *element);
    void parseRoomGeometry(ARRoom *room, XERCES DOMElement *element, bool dec);
    void parseRoomTracking_BAUML(ARRoom *room, XERCES DOMElement *element);
    void parseRoomTrackingARToolkit(ARRoom *room, XERCES DOMElement *element);
    void parseRoomPortals(ARRoom *room, XERCES DOMElement *element);
    void parseTrackingSystem(XERCES DOMElement *element);

    XERCES DOMNode *createDocument(XERCES DOMImplementation *implementation);
    XERCES DOMElement *createRoom(XERCES DOMDocument *document, ARRoom *room);
    XERCES DOMElement *createTrackingSystem(XERCES DOMDocument *document);
    XERCES DOMElement *createGeometry(XERCES DOMDocument *document, ARRoom *room);
    XERCES DOMElement *createTracking(XERCES DOMDocument *document, ARRoom *room);
    XERCES DOMElement *createPortals(XERCES DOMDocument *document, ARRoom *room);

    void parseRoom_OCAR(XERCES DOMElement *element);
    void parseRoomTracking_OCAR(ARRoom *room, XERCES DOMElement *element);

    XERCES DOMElement *createRoom_ocar(XERCES DOMDocument *document, ARRoom *room);
    XERCES DOMNode *createDocument_ocar(XERCES DOMImplementation *implementation);

public:

    BAUML();
    virtual ~BAUML();

    void read_bauml(const char *xmlfile);
    void write_bauml(const char *xmlfile);
    void read_ocar(const char *xmlfile);
    void write_ocar(const char *xmlfile);

    ARBuilding *getBuilding();

};


#endif
