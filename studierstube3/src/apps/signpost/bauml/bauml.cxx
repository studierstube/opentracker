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
  * $Id: bauml.cxx 3801 2004-09-30 14:02:02Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <iostream>
#include <math.h>
#include "bauml.h"

using namespace std;
XERCES_CPP_NAMESPACE_USE

BAUML::BAUML()
{
    try 
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.getMessage());
        cout << "Error during initialization! :\n" << message << "\n";
        delete []message;
        return;
    }
}

BAUML::~BAUML()
{
    XMLPlatformUtils::Terminate();
}

// -----------------------------------------------------------------------

void
BAUML::parseBAUML(DOMDocument *document)
{
    XMLCh xmlstr[256];
    char chrstr[256];

//    DOMDocument *document = parser->getDocument();

    XMLString::transcode("Building", xmlstr, 255);
    DOMNodeList *building_nodelist = document->getElementsByTagName(xmlstr);
    if (building_nodelist != 0)
    {
        XMLSize_t length = building_nodelist->getLength();
        for (unsigned int i=0; i<length; i++)
        {
            DOMNode *node = building_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                if (getElementAttribute(element, "name", chrstr, 255)) building.setName(chrstr);
            }
        }
    }

    XMLString::transcode("Room", xmlstr, 255);
    DOMNodeList *room_nodelist = document->getElementsByTagName(xmlstr);
    if (room_nodelist != 0)
    {
        XMLSize_t length = room_nodelist->getLength();
        for (unsigned int i=0; i<length; i++)
        {
            DOMNode *node = room_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoom_BAUML((DOMElement *)node);
            }
        }
    }

    XMLString::transcode("TrackingSystem", xmlstr, 255);
    DOMNodeList *trackingsystem_nodelist = document->getElementsByTagName(xmlstr);
    if (trackingsystem_nodelist != 0)
    {
        XMLSize_t length = trackingsystem_nodelist->getLength();
        for (unsigned int i=0; i<length; i++)
        {
            DOMNode *node = trackingsystem_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseTrackingSystem((DOMElement *)node);
            }
        }
    }

}

bool BAUML::getElementAttribute(DOMElement *element, const char *name, char *value, int maxlen)
{
    XMLCh xmlstr[256];
    XMLString::transcode(name, xmlstr, 255);
    const XMLCh *value_xmlstr = element->getAttribute(xmlstr);
    if (value_xmlstr == 0) return false;
    XMLString::transcode(value_xmlstr, value, maxlen);
    return true;
}

void
BAUML::parseRoom_BAUML(DOMElement *element)
{
    if (element == 0) return;
    XMLCh xmlstr[256];
    char chrstr[256];

    ARRoom *room = new ARRoom();
    building.addRoom(room);

    // ----- room id -----

    if (getElementAttribute(element, "id", chrstr, 255)) room->setRoomId(chrstr);
    if (getElementAttribute(element, "origin", chrstr, 255)) room->setOrigin(chrstr);

    // ----- room geometry -----

    XMLString::transcode("Geometry", xmlstr, 255);
    DOMNodeList *geometry_nodelist = element->getElementsByTagName(xmlstr);
    if (geometry_nodelist != 0)
    {
        int length = geometry_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = geometry_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoomGeometry(room, (DOMElement *)node, true);
            }
        }
    }

    // ----- tracking -----

    XMLString::transcode("Tracking", xmlstr, 255);
    DOMNodeList *tracking_nodelist = element->getElementsByTagName(xmlstr);
    if (tracking_nodelist != 0)
    {
        int length = tracking_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = tracking_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoomTracking_BAUML(room, (DOMElement *)node);
            }
        }
    }

    // ----- portals -----

    XMLString::transcode("Portals", xmlstr, 255);
    DOMNodeList *portals_nodelist = element->getElementsByTagName(xmlstr);
    if (portals_nodelist != 0)
    {
        int length = portals_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = portals_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoomPortals(room, (DOMElement *)node);
            }
        }
    }

}

void
BAUML::parseTrackingSystem(DOMElement *element)
{
    if (element == 0) return;
    XMLCh xmlstr[256];
    char chrstr[256];

    XMLString::transcode("ARToolkitTracking", xmlstr, 255);
    DOMNodeList *tracking_nodelist = element->getElementsByTagName(xmlstr);
    if (tracking_nodelist != 0)
    {
        int length = tracking_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = tracking_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                if (getElementAttribute(element, "gridsize", chrstr, 255)) 
                {
                    char *end;
                    building.setMarkerGridSize((float)strtod(chrstr, &end));
                }
            }
        }
    }

}

void
BAUML::parseRoomGeometry(ARRoom *room, DOMElement *element, bool dec)
{
    if (element == 0) return;
    XMLCh xmlstr[256];
    char chrstr[256];

    // ----- room geometry vertex -----

    XMLString::transcode("Vertex", xmlstr, 255);
    DOMNodeList *vertex_nodelist = element->getElementsByTagName(xmlstr);
    if (vertex_nodelist != 0)
    {
        int length = vertex_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = vertex_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                ARRoom::ARVertex *vertex = new ARRoom::ARVertex();
                room->addVertex(vertex);
                if (getElementAttribute(element, "position", chrstr, 255)) vertex->setPosition(chrstr);
            }
        }
    }
  
    // ----- room geometry polygon -----

    XMLString::transcode("Polygon", xmlstr, 255);
    DOMNodeList *polygon_nodelist = element->getElementsByTagName(xmlstr);
    if (polygon_nodelist != 0)
    {
        int length = polygon_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = polygon_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                ARRoom::ARPolygon *polygon = new ARRoom::ARPolygon();
                room->addPolygon(polygon);
                if (getElementAttribute(element, "vertices", chrstr, 255)) polygon->setVertices(chrstr);
                if (getElementAttribute(element, "type", chrstr, 255)) polygon->setType(chrstr);
                if (getElementAttribute(element, "name", chrstr, 255)) polygon->setName(chrstr);
                if (dec) for (unsigned int k=0; k<polygon->indexlist.size(); k++) polygon->indexlist[k]--;
            }
        }
    }
}

void
BAUML::parseRoomTracking_BAUML(ARRoom *room, DOMElement *element)
{
    if (element == 0) return;
    XMLCh xmlstr[256];

    // ----- room tracking artoolkit -----

    XMLString::transcode("ARToolkit", xmlstr, 255);
    DOMNodeList *artoolkit_nodelist = element->getElementsByTagName(xmlstr);
    if (artoolkit_nodelist != 0)
    {
        int length = artoolkit_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = artoolkit_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoomTrackingARToolkit(room, (DOMElement *)node);
            }
        }
    } 
}

void
BAUML::parseRoomTrackingARToolkit(ARRoom *room, DOMElement *element)
{
    if (element == 0) return;
    XMLCh xmlstr[256];
    char chrstr[256];

    // ----- room tracking artoolkit marker4pnt -----

    XMLString::transcode("Marker4PNT", xmlstr, 255);
    DOMNodeList *marker4pnt_nodelist = element->getElementsByTagName(xmlstr);
    if (marker4pnt_nodelist != 0)
    {
        int length = marker4pnt_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = marker4pnt_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                ARRoom::ARMarker *marker = new ARRoom::ARMarker();
                room->addMarker(marker);

                XMLString::transcode("Vertex", xmlstr, 255);
                DOMNodeList *vertex_nodelist = element->getElementsByTagName(xmlstr);
                if (vertex_nodelist != 0)
                {
                    ARRoom::ARVertex pointtl;
                    ARRoom::ARVertex pointtr;
                    ARRoom::ARVertex pointbl;
                    ARRoom::ARVertex pointbr;

                    int length = vertex_nodelist->getLength();
                    for (int i=0; i<length; i++)
                    {
                        DOMNode *node = vertex_nodelist->item(i);
                        if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
                        {
                            DOMElement *element = (DOMElement *)node;
                            ARRoom::ARVertex *vertex = new ARRoom::ARVertex();
                            if (getElementAttribute(element, "position", chrstr, 255)) 
                            {
                                switch (i)
                                {
                                    case 0: pointtl.setPosition(chrstr); break;
                                    case 1: pointtr.setPosition(chrstr); break;
                                    case 2: pointbl.setPosition(chrstr); break;
                                    case 3: pointbr.setPosition(chrstr); break;
                                }
                            }
                        }
                    }


                    switch (length)
                    {
                        case 3:
                        {
                            marker->setVertices(&pointtl, &pointtr, &pointbl, NULL);
                            break;
                        }                       
                        case 4:
                        {
                            marker->setVertices(&pointtl, &pointtr, &pointbl, &pointbr);
                            break;
                        }                       
                        default:
                        {
                            cout << "ERROR: Wrong Number of Vertices in Marker4PNT\n";
                            exit(1);
                            break;
                        }
                    }

                    if (getElementAttribute(element, "number", chrstr, 255)) marker->setNumber(chrstr);
                    if (getElementAttribute(element, "name", chrstr, 255)) marker->setName(chrstr);
                    if (getElementAttribute(element, "size", chrstr, 255)) marker->setSize(chrstr);

                    double det = marker->getDeterminant();
                    if ((det < 0.98) || (det > 1.02))
                    {
                        std::cout << "Marker " << marker->number << " in Room " << room->roomid << ": " << marker->getDeterminant() << std::endl;
                    }

                }
            }
        }
    } 

    // ----- room tracking artoolkit marker6dof -----

    XMLString::transcode("Marker6DOF", xmlstr, 255);
    DOMNodeList *marker6dof_nodelist = element->getElementsByTagName(xmlstr);
    if (marker6dof_nodelist != 0)
    {
        int length = marker6dof_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = marker6dof_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                ARRoom::ARMarker *marker = new ARRoom::ARMarker();
                room->addMarker(marker);

                ARRoom::ARVertex center;
                ARRoom::ARVertex dir;
                ARRoom::ARVertex hor;
                ARRoom::ARVertex ver;

                if (getElementAttribute(element, "center", chrstr, 255)) center.setPosition(chrstr);
                if (getElementAttribute(element, "normal", chrstr, 255)) dir.setPosition(chrstr);
                if (getElementAttribute(element, "horizontal", chrstr, 255)) hor.setPosition(chrstr);
                if (getElementAttribute(element, "vertical", chrstr, 255)) ver.setPosition(chrstr);
//cout << chrstr << endl;

                marker->cenx = center.x;
                marker->ceny = center.y;
                marker->cenz = center.z;

                marker->dirx = dir.x;
                marker->diry = dir.y;
                marker->dirz = dir.z;

                marker->horx = hor.x;
                marker->hory = hor.y;
                marker->horz = hor.z;

                marker->verx = ver.x;
                marker->very = ver.y;
                marker->verz = ver.z;

                if (getElementAttribute(element, "number", chrstr, 255)) marker->setNumber(chrstr);
                if (getElementAttribute(element, "name", chrstr, 255)) marker->setName(chrstr);
                if (getElementAttribute(element, "size", chrstr, 255)) marker->setSize(chrstr);

                double det = marker->getDeterminant();
                if ((det < 0.98) || (det > 1.02))
                {
                    std::cout << "Marker " << marker->number << " in Room " << room->roomid << ": " << marker->getDeterminant() << std::endl;
                }
            }
        }
    } 

    // ----- room tracking artoolkit marker4idx -----

    XMLString::transcode("Marker4IDX", xmlstr, 255);
    DOMNodeList *marker4idx_nodelist = element->getElementsByTagName(xmlstr);
    if (marker4idx_nodelist != 0)
    {
        int length = marker4idx_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = marker4idx_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                ARRoom::ARMarker *marker = new ARRoom::ARMarker();
                room->addMarker(marker);
                if (getElementAttribute(element, "vertices", chrstr, 255)) marker->setVertices(chrstr, room, -1);
                if (getElementAttribute(element, "number", chrstr, 255)) marker->setNumber(chrstr);
                if (getElementAttribute(element, "name", chrstr, 255)) marker->setName(chrstr);
                if (getElementAttribute(element, "size", chrstr, 255)) marker->setSize(chrstr);
                double det = marker->getDeterminant();
                if ((det < 0.98) || (det > 1.02))
                {
                    std::cout << "Marker " << marker->number << " in Room " << room->roomid << ": " << marker->getDeterminant() << std::endl;
                }
            }
        }
    } 
}

void
BAUML::parseRoomPortals(ARRoom *room, DOMElement *element)
{
    if (element == 0) return;
    XMLCh xmlstr[256];
    char chrstr[256];

    // ----- room portal portals -----

    XMLString::transcode("Portal", xmlstr, 255);
    DOMNodeList *vertex_nodelist = element->getElementsByTagName(xmlstr);
    if (vertex_nodelist != 0)
    {
        int length = vertex_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = vertex_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                ARRoom::ARPortal *portal = new ARRoom::ARPortal();
                room->addPortal(portal);
                if (getElementAttribute(element, "room", chrstr, 255)) portal->room = chrstr;
                if (getElementAttribute(element, "polygon", chrstr, 255)) portal->polygon = chrstr;
                if (getElementAttribute(element, "polygonnb", chrstr, 255)) portal->polygonnb = chrstr;
            }
        }
    }
 
}

void
BAUML::read_bauml(const char *xmlfile)
{
    ErrorHandler *errhandler = (ErrorHandler *)new HandlerBase();
    XercesDOMParser *parser = new XercesDOMParser();
    parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
    parser->setDoNamespaces(true);    // optional
    parser->setErrorHandler(errhandler);

    try 
    {
        parser->parse(xmlfile);
    }
    catch (const XMLException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.getMessage());
        cout << "XML Exception message is: \n" << message << "\n";
        delete []message;
        return;
    }
    catch (const DOMException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.msg);
        cout << "DOM Exception message is: \n" << message << "\n";
        delete []message;
        return;
    }
    catch (...) 
    {
        cout << "Unexpected Exception: Maybe file is not accessible\n" ;
        return;
    }

    parseBAUML(parser->getDocument());

    delete parser;
    delete errhandler;
}

// -----------------------------------------------------------------------

DOMNode *
BAUML::createDocument(DOMImplementation *implementation)
{
    XMLCh xmlstr1[1024];
    XMLCh xmlstr2[1024];

    DOMDocument *document = implementation->createDocument();

    // ===== Building =====
    XMLString::transcode("Building", xmlstr1, 1023);
    DOMElement *element_Building = document->createElement(xmlstr1);
    document->appendChild(element_Building);

    XMLString::transcode("name", xmlstr1, 1023);
    XMLString::transcode(building.getName(), xmlstr2, 1023);
    element_Building->setAttributeNS(0, xmlstr1, xmlstr2);

    // =====    
    for (int i=0; i<building.getRoomNum(); i++)
    {
        ARRoom *room = building.getRoom(i); 
        DOMElement *element_Room = createRoom(document, room);
        element_Building->appendChild(element_Room);
    }

    DOMElement *element_TrackingSystem = createTrackingSystem(document);
    element_Building->appendChild(element_TrackingSystem);

    return document;
}

DOMElement *
BAUML::createRoom(DOMDocument *document, ARRoom *room)
{
    XMLCh xmlstr1[1024];
    XMLCh xmlstr2[1024];

    XMLString::transcode("Room", xmlstr1, 1023);
    DOMElement *element_Room = document->createElement(xmlstr1);

    XMLString::transcode("id", xmlstr1, 1023);
    XMLString::transcode(room->getRoomId(), xmlstr2, 1023);
    element_Room->setAttributeNS(0, xmlstr1, xmlstr2);

    DOMElement *element_Geometry = createGeometry(document, room);
    element_Room->appendChild(element_Geometry);

    DOMElement *element_Tracking = createTracking(document, room);
    element_Room->appendChild(element_Tracking);

    DOMElement *element_Portals = createPortals(document, room);
    element_Room->appendChild(element_Portals);

    return element_Room;
}

DOMElement *
BAUML::createTrackingSystem(DOMDocument *document)
{
    MAKE_NODE(TrackingSystem, "TrackingSystem");
    MAKE_NODEP(ARToolkitTracking, "ARToolkitTracking", TrackingSystem);
    char str[1024];
    sprintf(str, "%f", building.getMarkerGridSize());
    SET_ATTR(ARToolkitTracking, "gridsize", str);
    return element_TrackingSystem;
}

DOMElement *
BAUML::createGeometry(DOMDocument *document, ARRoom *room)
{
    XMLCh xmlstr1[1024];
    XMLCh xmlstr2[1024];

    XMLString::transcode("Geometry", xmlstr1, 1023);
    DOMElement *element_Geometry = document->createElement(xmlstr1);

    for (int i=0; i<room->getVertexNum(); i++)
    {
        ARRoom::ARVertex *vertex = room->getVertex(i);

        if (vertex->index >= 0)
        {
            XMLString::transcode("Vertex", xmlstr1, 1023);
            DOMElement *element_Vertex = document->createElement(xmlstr1);
            element_Geometry->appendChild(element_Vertex);

            char pos_str[1024];
            sprintf(pos_str, "%f %f %f", vertex->x, vertex->y, vertex->z);
            XMLString::transcode("position", xmlstr1, 1023);
            XMLString::transcode(pos_str, xmlstr2, 1023);
            element_Vertex->setAttributeNS(0, xmlstr1, xmlstr2);
        }
    }

    for (int j=0; j<room->getPolygonNum(); j++)
    {
        ARRoom::ARPolygon *polygon = room->getPolygon(j);

        XMLString::transcode("Polygon", xmlstr1, 1023);
        DOMElement *element_Polygon = document->createElement(xmlstr1);
        element_Geometry->appendChild(element_Polygon);

        char idx_str[1024];
        idx_str[0] = '\0';
        for (unsigned int k=0; k<polygon->indexlist.size(); k++)
        {
            char buf[20];
            ARRoom::ARVertex *vertex = room->getVertex(polygon->indexlist[k]);
            sprintf(buf, "%i ", vertex->index + 1);
            strcat(idx_str, buf);
        }
        idx_str[strlen(idx_str)-1] = '\0';
        XMLString::transcode("vertices", xmlstr1, 1023);
        XMLString::transcode(idx_str, xmlstr2, 1023);
        element_Polygon->setAttributeNS(0, xmlstr1, xmlstr2);

        XMLString::transcode("name", xmlstr1, 1023);
        XMLString::transcode(polygon->name.c_str(), xmlstr2, 1023);
        element_Polygon->setAttributeNS(0, xmlstr1, xmlstr2);

        XMLString::transcode("type", xmlstr1, 1023);
        XMLString::transcode("unknown", xmlstr2, 1023);
        switch (polygon->type)
        {
            case ARRoom::wall: XMLString::transcode("wall", xmlstr2, 1023); break;
            case ARRoom::floor: XMLString::transcode("floor", xmlstr2, 1023); break;
            case ARRoom::ceiling: XMLString::transcode("ceiling", xmlstr2, 1023); break;
            case ARRoom::portal: XMLString::transcode("portal", xmlstr2, 1023); break;
            case ARRoom::special: XMLString::transcode("special", xmlstr2, 1023); break;
        }
        element_Polygon->setAttributeNS(0, xmlstr1, xmlstr2);
    }

    return element_Geometry;
}

DOMElement *
BAUML::createTracking(DOMDocument *document, ARRoom *room)
{
    XMLCh xmlstr1[1024];

    XMLString::transcode("Tracking", xmlstr1, 1023);
    DOMElement *element_Tracking = document->createElement(xmlstr1);

    XMLString::transcode("ARToolkit", xmlstr1, 1023);
    DOMElement *element_ARToolkit = document->createElement(xmlstr1);
    element_Tracking->appendChild(element_ARToolkit);

    for (int i=0; i<room->getMarkerNum(); i++)
    {
        ARRoom::ARMarker *marker = room->getMarker(i);

        XMLString::transcode("Marker6DOF", xmlstr1, 1023);
        DOMElement *element_Marker = document->createElement(xmlstr1);
        element_ARToolkit->appendChild(element_Marker);

        char str[1024];

        sprintf(str, "%i", marker->number);
        SET_ATTR(Marker, "number", str);

        SET_ATTR(Marker, "name", marker->name.c_str());

        sprintf(str, "%f %f %f", marker->cenx, marker->ceny, marker->cenz);
        SET_ATTR(Marker, "center", str);

        sprintf(str, "%f %f %f", marker->dirx, marker->diry, marker->dirz);
        SET_ATTR(Marker, "normal", str);

        sprintf(str, "%f %f %f", marker->horx, marker->hory, marker->horz);
        SET_ATTR(Marker, "horizontal", str);

        sprintf(str, "%f %f %f", marker->verx, marker->very, marker->verz);
        SET_ATTR(Marker, "vertical", str);

        sprintf(str, "%f", marker->size);
        SET_ATTR(Marker, "size", str);

    }

    return element_Tracking;
}

DOMElement *
BAUML::createPortals(DOMDocument *document, ARRoom *room)
{
    XMLCh xmlstr1[1024];
    XMLCh xmlstr2[1024];

    XMLString::transcode("Portals", xmlstr1, 1023);
    DOMElement *element_Portals = document->createElement(xmlstr1);

    for (int i=0; i<room->getPortalNum(); i++)
    {
        ARRoom::ARPortal *portal = room->getPortal(i);

        XMLString::transcode("Portal", xmlstr1, 1023);
        DOMElement *element_Portal = document->createElement(xmlstr1);
        element_Portals->appendChild(element_Portal);

        XMLString::transcode("room", xmlstr1, 1023);
        XMLString::transcode(portal->room.c_str(), xmlstr2, 1023);
        element_Portal->setAttributeNS(0, xmlstr1, xmlstr2);

        XMLString::transcode("polygon", xmlstr1, 1023);
        XMLString::transcode(portal->polygon.c_str(), xmlstr2, 1023);
        element_Portal->setAttributeNS(0, xmlstr1, xmlstr2);

        XMLString::transcode("polygonnb", xmlstr1, 1023);
        XMLString::transcode(portal->polygonnb.c_str(), xmlstr2, 1023);
        element_Portal->setAttributeNS(0, xmlstr1, xmlstr2);
    }

    return element_Portals;
}

void
BAUML::write_bauml(const char *xmlfile)
{
    XMLCh xmlstr[1024];
    XMLString::transcode("LS", xmlstr, 1023);
    DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(xmlstr);

    DOMWriter *writer = ((DOMImplementationLS *)implementation)->createDOMWriter();
    XMLFormatTarget *formtarget = new LocalFileFormatTarget(xmlfile);
//    XMLFormatTarget *formtarget = new StdOutFormatTarget();

    DOMErrorHandler* errhandler = (DOMErrorHandler *)new HandlerBase();
    writer->setErrorHandler(errhandler);

    XMLString::transcode("\r\n", xmlstr, 1023);
	writer->setNewLine(xmlstr);

    XMLString::transcode("UTF-8", xmlstr, 1023);
    writer->setEncoding(xmlstr);

    XMLString::transcode("format-pretty-print", xmlstr, 1023);
    if (writer->canSetFeature(xmlstr, true)) writer->setFeature(xmlstr, true);


    try 
    {
        DOMNode *node = createDocument(implementation);
        writer->writeNode(formtarget, *node);
        delete node;
    }
    catch (const XMLException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.getMessage());
        cout << "XML Exception message is: \n" << ((message != 0) ? message : "(null)") << "\n";
        delete []message;
        return;
    }
    catch (const DOMException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.msg);
        cout << "DOM Exception message is: \n" << ((message != 0) ? message : "(null)") << "\n";
        delete []message;
        return;
    }
    catch (...) 
    {
        cout << "Unexpected Exception: Maybe file is not accessible\n" ;
        return;
    }

    writer->release();
    delete formtarget;
    delete errhandler;
}

// -----------------------------------------------------------------------

ARBuilding *
BAUML::getBuilding()
{
    return &building;
}

// -----------------------------------------------------------------------

DOMElement *
BAUML::createRoom_ocar(DOMDocument *document, ARRoom *room)
{
    int i;

    // ===== Room =====
    MAKE_NODE(Room, "Room");
    SET_ATTR(Room, "id", room->getRoomId());

    // ===== Room/representation =====
    MAKE_NODEP(representation, "representation", Room);
    for (i=0; i<room->getVertexNum(); i++)
    {
        ARRoom::ARVertex *vertex = room->getVertex(i);
        if (vertex->index >= 0)
        {
            MAKE_NODEP(Vertex, "Vertex", representation);
            char pos_str[1024];
            sprintf(pos_str, "%f %f %f", vertex->x, vertex->y, vertex->z);
            SET_ATTR(Vertex, "position", pos_str);
        }
    }

    for (int j=0; j<room->getPolygonNum(); j++)
    {
        ARRoom::ARPolygon *polygon = room->getPolygon(j);
        MAKE_NODEP(Polygon, "Polygon", representation);
        char idx_str[1024];
        idx_str[0] = '\0';
        for (unsigned int k=0; k<polygon->indexlist.size(); k++)
        {
            char buf[20];
            ARRoom::ARVertex *vertex = room->getVertex(polygon->indexlist[k]);
            sprintf(buf, "%i ", vertex->index);
            strcat(idx_str, buf);
        }
        idx_str[strlen(idx_str)-1] = '\0';
        SET_ATTR(Polygon, "vertices", idx_str);
        SET_ATTR(Polygon, "name", polygon->name.c_str());
        switch (polygon->type)
        {
            case ARRoom::wall: SET_ATTR(Polygon, "type", "wall"); break;
            case ARRoom::floor: SET_ATTR(Polygon, "type", "floor"); break;
            case ARRoom::ceiling: SET_ATTR(Polygon, "type", "ceiling"); break;
            case ARRoom::portal: SET_ATTR(Polygon, "type", "portal"); break;
            case ARRoom::special: SET_ATTR(Polygon, "type", "special"); break;
            default: SET_ATTR(Polygon, "type", "unknown"); break;

        }
    }

    // ===== Room/children =====
    MAKE_NODEP(children, "children", Room);
    for (i=0; i<room->getMarkerNum(); i++)
    {
        ARRoom::ARMarker *marker = room->getMarker(i);
        MAKE_NODEP(ARToolkitMarker, "ARToolkitMarker", children);
        char str[1024];
        SET_ATTR(ARToolkitMarker, "pattern", marker->name.c_str());
        sprintf(str, "%i", marker->number);
        SET_ATTR(ARToolkitMarker, "number", str);
        sprintf(str, "%f", marker->size);
        SET_ATTR(ARToolkitMarker, "size", str);
        SET_ATTR(ARToolkitMarker, "overrideTrigger", ((marker->isOverrider()) ? "true" : "false"));
        MAKE_NODEP(pose, "pose", ARToolkitMarker);
        MAKE_NODEP(Transformation, "Transformation", pose);
        sprintf(str, "%f %f %f", marker->cenx, marker->ceny, marker->cenz);
        SET_ATTR(Transformation, "translation", str);
        sprintf(str, "%f %f %f %f %f %f %f %f %f", marker->horx, marker->verx, marker->dirx, marker->hory, marker->very, marker->diry, marker->horz, marker->verz, marker->dirz);
        SET_ATTR(Transformation, "rotation", str);
        SET_ATTR(Transformation, "rotationType", "matrix");
//        MAKE_NODEP(annotation, "annotation", ARToolkitMarker);
//        MAKE_NODEP(Cell, "Cell", annotation);
//        SET_ATTR(Cell, "name", marker->cell.c_str());
    }

    // ===== Room/portals =====
    MAKE_NODEP(portals, "portals", Room);
    for (i=0; i<room->getPortalNum(); i++)
    {
        ARRoom::ARPortal *portal = room->getPortal(i);
        MAKE_NODEP(Portal, "Portal", portals);
        SET_ATTR(Portal, "room", portal->room.c_str());
        SET_ATTR(Portal, "polygon", portal->polygon.c_str());
        SET_ATTR(Portal, "polygonnb", portal->polygonnb.c_str());
    }
    return element_Room;
}

DOMNode *
BAUML::createDocument_ocar(DOMImplementation *implementation)
{
    DOMDocument *document = implementation->createDocument();
    DOMDocument *element_Document = document;

    // ===== SpatialContainer =====
    MAKE_NODEP(SpatialContainer, "SpatialContainer", Document);
    SET_ATTR(SpatialContainer, "id", building.getName());

    // ===== SpatialContainer/children =====
    MAKE_NODEP(children, "children", SpatialContainer);

    // ===== SpatialContainer/annotation =====
    MAKE_NODEP(annotation, "annotation", SpatialContainer);
    MAKE_NODEP(ARToolkitTracking, "ARToolkitTracking", annotation);
    char str[1024];
    sprintf(str, "%f", building.getMarkerGridSize());
    SET_ATTR(ARToolkitTracking, "gridsize", str);

    // =====    
    for (int i=0; i<building.getRoomNum(); i++)
    {
        ARRoom *room = building.getRoom(i); 
        DOMElement *element_Room = createRoom_ocar(document, room);
        element_children->appendChild(element_Room);
    }

    return element_Document;
}

void
BAUML::write_ocar(const char *xmlfile)
{
    XMLCh xmlstr[1024];
    XMLString::transcode("LS", xmlstr, 1023);
    DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(xmlstr);

    DOMWriter *writer = ((DOMImplementationLS *)implementation)->createDOMWriter();
    XMLFormatTarget *formtarget = new LocalFileFormatTarget(xmlfile);

    DOMErrorHandler* errhandler = (DOMErrorHandler *)new HandlerBase();
    writer->setErrorHandler(errhandler);

    XMLString::transcode("\r\n", xmlstr, 1023);
	writer->setNewLine(xmlstr);

    XMLString::transcode("UTF-8", xmlstr, 1023);
    writer->setEncoding(xmlstr);

    XMLString::transcode("format-pretty-print", xmlstr, 1023);
    if (writer->canSetFeature(xmlstr, true)) writer->setFeature(xmlstr, true);

    try 
    {
        DOMNode *node = createDocument_ocar(implementation);
        writer->writeNode(formtarget, *node);
        delete node;
    }
    catch (const XMLException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.getMessage());
        cout << "XML Exception message is: \n" << ((message != 0) ? message : "(null)") << "\n";
        delete []message;
        return;
    }
    catch (const DOMException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.msg);
        cout << "DOM Exception message is: \n" << ((message != 0) ? message : "(null)") << "\n";
        delete []message;
        return;
    }
    catch (...) 
    {
        cout << "Unexpected Exception: Maybe file is not accessible\n" ;
        return;
    }

    writer->release();
    delete formtarget;
    delete errhandler;
}

// -----------------------------------------------------------------------

void
BAUML::parseOCAR(DOMDocument *document)
{

    XMLCh xmlstr[256];
    char chrstr[256];

    XMLString::transcode("Building", xmlstr, 255);
    DOMNodeList *SpatialContainer_nodelist = document->getElementsByTagName(xmlstr);
    if (SpatialContainer_nodelist != 0)
    {
        XMLSize_t length = SpatialContainer_nodelist->getLength();
        for (unsigned int i=0; i<length; i++)
        {
            DOMNode *node = SpatialContainer_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                DOMElement *element = (DOMElement *)node;
                if (getElementAttribute(element, "id", chrstr, 255)) building.setName(chrstr);
            }
        }
    }

    XMLString::transcode("Room", xmlstr, 255);
    DOMNodeList *room_nodelist = document->getElementsByTagName(xmlstr);
    if (room_nodelist != 0)
    {
        XMLSize_t length = room_nodelist->getLength();
        for (unsigned int i=0; i<length; i++)
        {
            DOMNode *node = room_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoom_OCAR((DOMElement *)node);
            }
        }
    }

    XMLString::transcode("annotation", xmlstr, 255);
    DOMNodeList *annotation_nodelist = document->getElementsByTagName(xmlstr);
    if (annotation_nodelist != 0)
    {
        XMLSize_t length = annotation_nodelist->getLength();
        for (unsigned int i=0; i<length; i++)
        {
            DOMNode *node = annotation_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseTrackingSystem((DOMElement *)node);
            }
        }
    }

}

void
BAUML::parseRoom_OCAR(DOMElement *element)
{
    if (element == 0) return;
    XMLCh xmlstr[256];
    char chrstr[256];

    ARRoom *room = new ARRoom();
    building.addRoom(room);

    // ----- room id -----

    if (getElementAttribute(element, "id", chrstr, 255)) room->setRoomId(chrstr);
    if (getElementAttribute(element, "origin", chrstr, 255)) room->setOrigin(chrstr);

    // ----- room geometry -----

    XMLString::transcode("representation", xmlstr, 255);
    DOMNodeList *representation_nodelist = element->getElementsByTagName(xmlstr);
    if (representation_nodelist != 0)
    {
        int length = representation_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = representation_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoomGeometry(room, (DOMElement *)node, false);
            }
        }
    }

    // ----- tracking -----

    XMLString::transcode("children", xmlstr, 255);
    DOMNodeList *children_nodelist = element->getElementsByTagName(xmlstr);
    if (children_nodelist != 0)
    {
        int length = children_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = children_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoomTracking_OCAR(room, (DOMElement *)node);
            }
        }
    }

    // ----- portals -----

    XMLString::transcode("portals", xmlstr, 255);
    DOMNodeList *portals_nodelist = element->getElementsByTagName(xmlstr);
    if (portals_nodelist != 0)
    {
        int length = portals_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = portals_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
                parseRoomPortals(room, (DOMElement *)node);
            }
        }
    }

}

void
BAUML::parseRoomTracking_OCAR(ARRoom *room, DOMElement *element)
{
    if (element == 0) return;
    XMLCh xmlstr[256];
    char chrstr[256];

    // ----- room tracking artoolkit -----

    XMLString::transcode("ARToolkitMarker", xmlstr, 255);
    DOMNodeList *artoolkit_nodelist = element->getElementsByTagName(xmlstr);
    if (artoolkit_nodelist != 0)
    {
        int length = artoolkit_nodelist->getLength();
        for (int i=0; i<length; i++)
        {
            DOMNode *node = artoolkit_nodelist->item(i);
            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
            {
//                parseRoomTrackingARToolkit(room, (DOMElement *)node);
                DOMElement *element = (DOMElement *)node;
                ARRoom::ARMarker *marker = new ARRoom::ARMarker();
                room->addMarker(marker);

                // ----- Transformation ----
                XMLString::transcode("Transformation", xmlstr, 255);
                DOMNodeList *Transformation_nodelist = element->getElementsByTagName(xmlstr);
                if (Transformation_nodelist != 0)
                {
                    int length = Transformation_nodelist->getLength();
                    for (int i=0; i<length; i++)
                    {
                        DOMNode *node = Transformation_nodelist->item(i);
                        if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
                        {
                            DOMElement *element = (DOMElement *)node;
                            if (getElementAttribute(element, "translation", chrstr, 255)) sscanf(chrstr, "%lf %lf %lf", &marker->cenx, &marker->ceny, &marker->cenz);
                            if (getElementAttribute(element, "rotation", chrstr, 255)) sscanf(chrstr, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &marker->horx, &marker->verx, &marker->dirx, &marker->hory, &marker->very, &marker->diry, &marker->horz, &marker->verz, &marker->dirz);
                        }
                    }
                }

                // ----- Vertices ----
                XMLString::transcode("Vertex", xmlstr, 255);
                DOMNodeList *VertexList_nodelist = element->getElementsByTagName(xmlstr);
                if (VertexList_nodelist != 0)
                {
                    int length = VertexList_nodelist->getLength();
                    if( 3 <= length && length <= 4 )
                    {                        
                        ARRoom::ARVertex corners[4];
                        int i;
                        for (i=0; i<length; i++)
                        {
                            DOMNode *node = VertexList_nodelist->item(i);
                            if (node->getNodeType() == DOMNode::ELEMENT_NODE) 
                            {
                                DOMElement *element = (DOMElement *)node;
                                if (getElementAttribute(element, "position", chrstr, 255)) 
                                    corners[i].setPosition( chrstr );
                            }
                            else
                                break;
                        }
                        if( i == 3 )
                        {
                            marker->setVertices(&corners[0], &corners[1], &corners[2], NULL);
                        }
                        else if( i == 4)
                        {
                            marker->setVertices(&corners[0], &corners[1], &corners[2], &corners[3]);
                        }
                    }
                }

                if (getElementAttribute(element, "overrideTrigger", chrstr, 255)) marker->setOverride(std::string("true") == std::string(chrstr));
                if (getElementAttribute(element, "number", chrstr, 255)) marker->setNumber(chrstr);
                if (getElementAttribute(element, "pattern", chrstr, 255)) marker->setName(chrstr);
                if (getElementAttribute(element, "size", chrstr, 255)) marker->setSize(chrstr);

                double det = marker->getDeterminant();
                if ((det < 0.98) || (det > 1.02))
                {
                    std::cout << "Marker " << marker->number << " in Room " << room->roomid << ": " << marker->getDeterminant() << std::endl;
                }

            }
        }
    } 
}

void 
BAUML::read_ocar(const char *xmlfile)
{
    ErrorHandler *errhandler = (ErrorHandler *)new HandlerBase();
    XercesDOMParser *parser = new XercesDOMParser();
    parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
    parser->setDoNamespaces(true);    // optional
    parser->setErrorHandler(errhandler);

    try 
    {
        parser->parse(xmlfile);
    }
    catch (const XMLException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.getMessage());
        cout << "XML Exception message is: \n" << message << "\n";
        delete []message;
        return;
    }
    catch (const DOMException &toCatch) 
    {
        char *message = XMLString::transcode(toCatch.msg);
        cout << "DOM Exception message is: \n" << message << "\n";
        delete []message;
        return;
    }
    catch (...) 
    {
        cout << "Unexpected Exception: Maybe file is not accessible\n" ;
        return;
    }

    parseOCAR(parser->getDocument());

    delete parser;
    delete errhandler;
}

// -----------------------------------------------------------------------

