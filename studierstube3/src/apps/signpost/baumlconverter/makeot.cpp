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
/** BAUML Converter
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: makeot.cpp 4221 2005-05-24 14:48:57Z schall $
  * @file                                                                   */
 /* ======================================================================= */


//#include "bauml2ot.h"



//for windows dialogs
//#include <windows.h>
//#include <commctrl.h> 
//#include "resource.h"
//--

//#include <gl/glut.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <math.h>
#include <float.h>
#include <iostream>
#include <list>

#include <Inventor/Sb.h>

#include "types.h"
#include "box.h"
#include "cells.h"

#include "bauml.h"

using namespace std;

//---------------------------------------------------------

ARBuilding *building;

list<markerinfo> markers;
list<markerinfo> usedmarkers;
//list<usedmarkerinfo> usedmarkers;
cells *cellmanager = 0;

box boundingbox;

bool showmarkernum = true;
bool shownumber = false;
bool showsinglenumber = false;
int singlenumber = 0;


float rasterscale = 1.0f;

//---------------------------------------------------------

bool markerused(markerinfo &mi)
{
    for (list<markerinfo>::iterator it = usedmarkers.begin(); it != usedmarkers.end(); it++)
    {
        markerinfo &mi2 = (*it);
        if (mi2.marker->name == mi.marker->name)
        {
            return true;
        }
    }
    usedmarkers.push_back(mi);
    return false;
}

//---------------------------------------------------------

void getmarkers()
{
    int rn = building->getRoomNum();
    for (int i=0; i<rn; i++)
    {
        ARRoom *room = building->getRoom(i);
        int mn = room->getMarkerNum();
        for (int j=0; j<mn; j++)
        {
            ARRoom::ARMarker *marker = room->getMarker(j);
            markerinfo mi;
            mi.num = -1;
            mi.marker = marker;
            mi.room = room; //->getRoomId();
            mi.assigned = false;
            markers.push_back(mi);
        }
    }
    markers.sort();
}

void showmarkers()
{
    cout << "==================================================" << endl;
    cout << "List of all markers" << endl;
    cout << "--------------------------------------------------" << endl;
    for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
        cout.setf(ios::right);   
        cout.width(5);
        cout << mi.marker->number;
        cout.setf(ios::left, 0);   
        cout << " - marker: " << mi.marker->name.c_str();
        cout << " - room: " << mi.room->getRoomId();
//        cout << " - num: " << mi.num;
        cout << endl;
    }
    cout << markers.size() << " Markers" << endl;
}

void getboundingbox()
{
    int rn = building->getRoomNum();
    for (int i=0; i<rn; i++)
    {
        ARRoom *room = building->getRoom(i);
        int pn = room->getPolygonNum();
        for (int j=0; j<pn; j++)
        {
            ARRoom::ARPolygon *polygon = room->getPolygon(j);
            int pon = polygon->indexlist.size();
            for (int k=0; k<pon; k++)
            {
                ARRoom::ARVertex *vertex = room->getVertex(polygon->indexlist[k]);
                if (vertex->x < boundingbox.x0) boundingbox.x0 = (float)vertex->x;
                if (vertex->y < boundingbox.y0) boundingbox.y0 = (float)vertex->y;
                if (vertex->z < boundingbox.z0) boundingbox.z0 = (float)vertex->z;
                if (vertex->x > boundingbox.x1) boundingbox.x1 = (float)vertex->x;
                if (vertex->y > boundingbox.y1) boundingbox.y1 = (float)vertex->y;
                if (vertex->z > boundingbox.z1) boundingbox.z1 = (float)vertex->z;
            }
        }
    }

    cout << "--------------------------------------------------" << endl;
    cout << "Bounding Box: (" << boundingbox.x0 << "," << boundingbox.y0 << "," << boundingbox.z0 << ")-(" << boundingbox.x1 << "," << boundingbox.y1 << "," << boundingbox.z1 << ")" << endl;
}

void makecells()
{
    cout << "--------------------------------------------------" << endl;
    printf("grid size: %f\n", building->getMarkerGridSize());
    cout << "--------------------------------------------------" << endl;

    cellmanager = new cells(boundingbox,building->getMarkerGridSize());

	printf("number of cells :%d\n", cellmanager->num());

    // put markers into cells
    for (list<markerinfo>::iterator it = markers.begin(); it != markers.end(); it++)
    {
        markerinfo &mi = (*it);
		printf("markerinfo :%d", mi.marker->number);
        cellmanager->addmarker(mi);
    }
    cellmanager->update();

}

void findroomcells()
{
    int rn = building->getRoomNum();

	printf("Number of rooms: %d\n\n", rn);

    for (int i=0; i<rn; i++)
    {
        ARRoom *room = building->getRoom(i);
        int mn = room->getMarkerNum();
        float mx = 0;
        float my = 0;
        float mz = 0;

		cout << "Number of markers: " << mn << " in room: " << room->getRoomId() << " in cell: " << cellmanager->get(mx, my, mz)->name << endl;

		for (int j=0; j<mn; j++)
        {
            ARRoom::ARMarker *marker = room->getMarker(j);
            mx += marker->cenx;
            my += marker->ceny;
            mz += marker->cenz;
        }
        mx /= mn;
        my /= mn;
        mz /= mn;

		room->cell = cellmanager->get(mx, my, mz)->name;



    }
}

//-----------------------------------------------------------------------------

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


XERCES DOMElement *
createMarker(XERCES DOMDocument *document, ARRoom *room, ARRoom::ARMarker *marker, bool makeref)
{
    XMLCh xmlstr1[1000];
    char chrstr[1000];

    double ox, oy, oz;
    room->getOrigin(ox, oy, oz);

    // ===== EventTransform =====
    MAKE_NODE(EventTransformRot, "EventTransform");
    sprintf(chrstr, "%f %f %f %f %f %f %f %f %f", marker->horx, marker->verx, marker->dirx, marker->hory, marker->very, marker->diry, marker->horz, marker->verz, marker->dirz);
    SET_ATTR(EventTransformRot, "rotation", chrstr);


SbMatrix mat(marker->horx, marker->verx, marker->dirx, 0,
             marker->hory, marker->very, marker->diry, 0,
             marker->horz, marker->verz, marker->dirz, 0,
                        0,            0,            0, 1);


SbRotation rot(mat);
SbVec3f vec(0, 1, 0);
SbVec3f res(0, 0, 0);
rot.multVec(vec, res);
printf("marker %i - %f %f %f\n", marker->number, res[0], res[1], res[2]);


    sprintf(chrstr, "%f %f %f", marker->cenx, marker->ceny, marker->cenz);
    SET_ATTR(EventTransformRot, "translation", chrstr);
    SET_ATTR(EventTransformRot, "rotationtype", "matrix");

    // ===== EventTransform/EventInvertTransform =====
    MAKE_NODEP(EventInvertTransform, "EventInvertTransform", EventTransformRot);

    // ===== EventTransform/EventInvertTransform/EventTransform =====
    MAKE_NODEP(EventTransform, "EventTransform", EventInvertTransform);
    SET_ATTR(EventTransform, "rotation", "1 0 0 0");
    SET_ATTR(EventTransform, "scale", "0.001 0.001 0.001");

    // ===== EventTransform/EventInvertTransform/EventTransform/Comment =====
    sprintf(chrstr, "Marker %i", marker->number);
    XERCES XMLString::transcode(chrstr, xmlstr1, 99);
    XERCES DOMComment *comment_Marker = document->createComment(xmlstr1);
    element_EventTransform->appendChild(comment_Marker);

//    XERCES DOMElement *element_root = element_EventTransform;

    if (makeref)
    {
        MAKE_NODEP(Ref, "Ref", EventTransform);
        strcpy(chrstr, "MARKER_");
        strcat(chrstr, marker->name.c_str());
        SET_ATTR(Ref, "USE", chrstr);
    }
    else
    {
        // ===== EventTransform/EventInvertTransform/EventTransform/ElasticFilter/RangeFilter =====
        /*
        MAKE_NODEP(ElasticFilter, "ElasticFilter", EventTransform);
        SET_ATTR(ElasticFilter, "force", "1");
        SET_ATTR(ElasticFilter, "damp", "0.5");
        */
        // ===== EventTransform/EventInvertTransform/EventTransform/ElasticFilter/RangeFilter =====
        
        MAKE_NODEP(Filter, "Filter", EventTransform);
        SET_ATTR(Filter, "weight", "0.2 0.2 0.2 0.2 0.2");
        MAKE_NODEP(Queue, "EventQueue", Filter);
        SET_ATTR(Queue, "length", "5");

        // ===== EventTransform/EventInvertTransform/EventTransform/RangeFilter =====
        MAKE_NODEP(RangeFilter, "RangeFilter", Queue);
        SET_ATTR(RangeFilter, "min", "0");
        SET_ATTR(RangeFilter, "max", "5000");

        // ===== EventTransform/EventInvertTransform/EventTransform/ElasticFilter/RangeFilter/ConfidenceFilter =====
        MAKE_NODEP(ConfidenceFilter, "ConfidenceFilter", RangeFilter);
        SET_ATTR(ConfidenceFilter, "treshhold", "0.5"); // yes, really "treshhold"
        SET_ATTR(ConfidenceFilter, "type", "high");

        // ===== ./ARToolKitSource =====
        MAKE_NODEP(ARToolKitSource, "ARToolKitSource", ConfidenceFilter);
        SET_ATTR(ARToolKitSource, "center", "0 0");
        sprintf(chrstr, "%f %f", marker->size*1000, marker->size*1000);
        SET_ATTR(ARToolKitSource, "size", chrstr);
        SET_ATTR(ARToolKitSource, "tag-file", marker->name.c_str());

        // DEF name 
        strcpy(chrstr, "MARKER_");
        strcat(chrstr, marker->name.c_str());
        SET_ATTR(Filter, "DEF", chrstr); // RangeFilter

    }
    return element_EventTransformRot;
}

XERCES DOMElement *createCell(XERCES DOMDocument *document, ARBuilding *building, cell *c)
{
    XMLCh xmlstr1[1000];
    XMLCh xmlstr2[1000];

    XERCES XMLString::transcode("GroupGate", xmlstr1, 999);
    XERCES DOMElement *element_GroupGate = document->createElement(xmlstr1);

    XERCES XMLString::transcode("group", xmlstr1, 999);
    XERCES XMLString::transcode(building->getName(), xmlstr2, 999);
    element_GroupGate->setAttributeNS(0, xmlstr1, xmlstr2);

    XERCES XMLString::transcode("DEF", xmlstr1, 999);
    XERCES XMLString::transcode(c->name.c_str(), xmlstr2, 999);
    element_GroupGate->setAttributeNS(0, xmlstr1, xmlstr2);

	printf("c->neighborcells.size: %d \n\n", c->neighborcells.size());
    string neighbors;
    for (unsigned int i=0; i<c->neighborcells.size(); i++) 
	{	neighbors = neighbors + c->neighborcells[i]->name + " ";
	}

    XERCES XMLString::transcode("neighbors", xmlstr1, 999);
    XERCES XMLString::transcode(neighbors.c_str(), xmlstr2, 999);
    element_GroupGate->setAttributeNS(0, xmlstr1, xmlstr2);


    XERCES DOMElement *element_Merge = NULL;
    XERCES DOMElement *element_Override = NULL;

/*
    XERCES XMLString::transcode("Override", xmlstr1, 99);
    XERCES DOMElement *element_Override = document->createElement(xmlstr1);
    element_GroupGate->appendChild(element_Override);
*/
	printf("\nc->markers.size:%d\n",c->markers.size());
    if (c->markers.size() > 0)
    {
        for (unsigned int j=0; j<c->markers.size(); j++)
        {
            markerinfo *mi = c->markers[j];
            ARRoom::ARMarker *marker = mi->marker;
            XERCES DOMElement *element_Marker = createMarker(document, mi->room, marker, markerused(*mi));
            if (marker->isOverrider()) 
            {
                if (element_Override == NULL)
                {
                    MAKE_NODEP(temp, "Override", GroupGate);
                    element_Override = element_temp;
                }
                element_Override->appendChild(element_Marker); 
            }
            else 
            {
                if (element_Merge == NULL)
                {
                    MAKE_NODEP(temp, "ConfidenceSelect", GroupGate);
                    SET_ATTR(temp, "timeout", "100");
                    SET_ATTR(temp, "type", "high");
                    element_Merge = element_temp;
                }
                element_Merge->appendChild(element_Marker);
            }
        }
    }
    return element_GroupGate;
}

XERCES DOMElement *createGroupGateConfig(XERCES DOMDocument *document)
{
    XMLCh xmlstr1[100];
    XERCES XMLString::transcode("GroupGateConfig", xmlstr1, 99);
    XERCES DOMElement *element_GroupGateConfig = document->createElement(xmlstr1);
    // ...
    return element_GroupGateConfig;
}

XERCES DOMElement *createConsoleSink(XERCES DOMDocument *document)
{
    // ===== OpenTracker/ConsoleSink =====
    MAKE_NODE(ConsoleSink, "ConsoleSink");
    SET_ATTR(ConsoleSink, "comment", building->getName());
    SET_ATTR(ConsoleSink, "active", "off");

    // ===== OpenTracker/ConsoleSink/Merge =====
    MAKE_NODEP(Merge, "Merge", ConsoleSink);
    SET_ATTR(Merge, "DEF", "RawTracking");

    // ===== OpenTracker/ConsoleSink/Merge/MergeDefault =====
    MAKE_NODEP(MergeDefault, "MergeDefault", Merge);

    // =====    

	printf("cellmanager->num : %d\n",cellmanager->num());
    for (int i=0; i<cellmanager->num(); i++)
    {
        cell *c = cellmanager->get(i);
        XERCES DOMElement *element_Cell = createCell(document, building, c);
        element_MergeDefault->appendChild(element_Cell);
    }

    return element_ConsoleSink;
}

XERCES DOMElement *
createMerge(XERCES DOMDocument *document)
{
    char chrstr[1000];
    // ===== OpenTracker/ConfidenceFilter =====
    MAKE_NODE(ConfidenceFilter, "ConfidenceFilter");
    SET_ATTR(ConfidenceFilter, "DEF", "AllMarkers");
    SET_ATTR(ConfidenceFilter, "type", "high");
    SET_ATTR(ConfidenceFilter, "treshhold", "0.5"); // "treshhold" instead of "threshold" !

    // ===== OpenTracker/ConfidenceFilter/Merge =====
    MAKE_NODEP(Merge, "Merge", ConfidenceFilter);

    // ===== OpenTracker/ConfidenceFilter/Merge/MergeDefault =====
    MAKE_NODEP(MergeDefault, "MergeDefault", Merge);

    // ===== OpenTracker/ConfidenceFilter/Merge/MergeDefault/Ref

    for (list<markerinfo>::iterator it = usedmarkers.begin(); it != usedmarkers.end(); it++)
    {
        markerinfo &mi = (*it);
        MAKE_NODEP(Ref, "Ref", MergeDefault);
        strcpy(chrstr, "MARKER_");
        strcat(chrstr, mi.marker->name.c_str());
        SET_ATTR(Ref, "USE", chrstr);
    }
    return element_ConfidenceFilter;

}

XERCES DOMDocument *createDocument(XERCES DOMImplementation *implementation)
{
    XMLCh xmlstr1[100];
    XMLCh xmlstr2[100];

    XERCES XMLString::transcode("OpenTracker", xmlstr1, 99);
    XERCES XMLString::transcode("opentracker.dtd", xmlstr2, 99);
    XERCES DOMDocument *document = implementation->createDocument();
    XERCES DOMDocumentType *documenttype = implementation->createDocumentType(xmlstr1, 0, xmlstr2);
    document->appendChild(documenttype);

    // ===== OpenTracker =====
    XERCES XMLString::transcode("OpenTracker", xmlstr1, 99);
    XERCES DOMElement *element_OpenTracker = document->createElement(xmlstr1);
    document->appendChild(element_OpenTracker);

    // ===== OpenTracker/configuration =====
    XERCES XMLString::transcode("configuration", xmlstr1, 99);
    XERCES DOMElement *element_Configuration = document->createElement(xmlstr1);
    element_OpenTracker->appendChild(element_Configuration);

    // ===== OpenTracker/configuration/GroupGateConfig =====
    XERCES DOMElement *element_GroupGateConfig = createGroupGateConfig(document);
    element_Configuration->appendChild(element_GroupGateConfig);

    // ===== OpenTracker/ConsoleSink =====
    XERCES DOMElement *element_ConsoleSink = createConsoleSink(document);
    element_OpenTracker->appendChild(element_ConsoleSink);

    // ===== OpenTracker/Merge =====
    XERCES DOMElement *element_Merge = createMerge(document);
    element_OpenTracker->appendChild(element_Merge);

    return document;
}

void writeot(const char *filename, const char *templatefilename = NULL)
{
    XMLCh xmlstr[100];
    XERCES XMLString::transcode("LS", xmlstr, 99);
    XERCES DOMImplementation *implementation = XERCES DOMImplementationRegistry::getDOMImplementation(xmlstr);

    XERCES DOMWriter *writer = implementation->createDOMWriter();
    XERCES XMLFormatTarget *formtarget = new XERCES LocalFileFormatTarget(filename);

    XERCES DOMErrorHandler* errhandler = (XERCES DOMErrorHandler *)new XERCES HandlerBase();
    writer->setErrorHandler(errhandler);

    XERCES XMLString::transcode("\r\n", xmlstr, 1023);
	writer->setNewLine(xmlstr);

    XERCES XMLString::transcode("UTF-8", xmlstr, 1023);
    writer->setEncoding(xmlstr);

    XERCES XMLString::transcode("format-pretty-print", xmlstr, 1023);
    if (writer->canSetFeature(xmlstr, true)) writer->setFeature(xmlstr, true);

    XERCES DOMDocument *document = NULL;

    if (templatefilename != NULL)
    {
        XERCES ErrorHandler *errhandler = (XERCES ErrorHandler *)new XERCES HandlerBase();
        XERCES XercesDOMParser *parser = new XERCES XercesDOMParser();
        parser->setValidationScheme(XERCES XercesDOMParser::Val_Always);    // optional.
        parser->setDoNamespaces(true);    // optional
        parser->setErrorHandler(errhandler);

        try 
        {
            parser->parse(templatefilename);
        }
        catch (const XERCES XMLException &toCatch) 
        {
            char *message = XERCES XMLString::transcode(toCatch.getMessage());
            cout << "XML Exception message is: \n" << message << "\n";
            delete []message;
            return;
        }
        catch (const XERCES DOMException &toCatch) 
        {
            char *message = XERCES XMLString::transcode(toCatch.msg);
            cout << "DOM Exception message is: \n" << message << "\n";
            delete []message;
            return;
        }
        catch (...) 
        {
            cout << "Unexpected Exception: Maybe file "<< templatefilename << " is not accessible\n" ;
            return;
        }

        document = parser->adoptDocument();

        delete parser;
        delete errhandler;


        XERCES XMLString::transcode("OpenTracker", xmlstr, 99);
        XERCES DOMNodeList *nodelist_OpenTracker = document->getElementsByTagName(xmlstr);
        XERCES DOMElement *element_OpenTracker = (XERCES DOMElement *)(nodelist_OpenTracker->item(0));

        XERCES XMLString::transcode("configuration", xmlstr, 99);
        XERCES DOMNodeList *nodelist_configuration = document->getElementsByTagName(xmlstr);
        XERCES DOMElement *element_configuration = (XERCES DOMElement *)(nodelist_configuration->item(0));
/*
        // ===== OpenTracker/configuration/GroupGateConfig =====
        XERCES DOMElement *element_GroupGateConfig = createGroupGateConfig(document);
        element_configuration->appendChild(element_GroupGateConfig);
*/
        // ===== OpenTracker/ConsoleSink =====
        XERCES DOMElement *element_ConsoleSink = createConsoleSink(document);
//        element_OpenTracker->appendChild(element_ConsoleSink);
         element_OpenTracker->insertBefore(element_ConsoleSink, element_configuration->getNextSibling());

        // ===== OpenTracker/Merge =====
        XERCES DOMElement *element_Merge = createMerge(document);
//        element_OpenTracker->appendChild(element_Merge);
        element_OpenTracker->insertBefore(element_Merge, element_ConsoleSink->getNextSibling());

    }
    else
    {
        document = createDocument(implementation);
    }

    try 
    {
        writer->writeNode(formtarget, *document);
        delete document;
    }
    catch (const XERCES XMLException &toCatch) 
    {
        char *message = XERCES XMLString::transcode(toCatch.getMessage());
        cout << "XML Exception message is: \n" << ((message != 0) ? message : "(null)") << "\n";
        delete []message;
        return;
    }
    catch (const XERCES DOMException &toCatch) 
    {
        char *message = XERCES XMLString::transcode(toCatch.msg);
        cout << "DOM Exception message is: \n" << ((message != 0) ? message : "(null)") << "\n";
        delete []message;
        return;
    }
    catch (...) 
    {
        cout << "Unexpeted Exception: Maybe file "<< filename << " is not accessible\n" ;
        return;
    }

    writer->release();
    delete formtarget;
    delete errhandler;
}


bool getElementAttribute(XERCES DOMElement *element, const char *name, char *value, int maxlen)
{
    XMLCh xmlstr[256];
    XERCES XMLString::transcode(name, xmlstr, 255);
    const XMLCh *value_xmlstr = element->getAttribute(xmlstr);
    if (value_xmlstr == 0) return false;
    XERCES XMLString::transcode(value_xmlstr, value, maxlen);
    return true;
}
/*
void swapyz(const char *inputfilename, const char *outputfilename)
{
    XMLCh xmlstr[100];
    XMLCh xmlstr1[100];
    XMLCh xmlstr2[100];
    char chrstr[256];

    XERCES XMLString::transcode("LS", xmlstr, 99);
    DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(xmlstr);

    DOMWriter *writer = implementation->createDOMWriter();
    XMLFormatTarget *formtarget = new LocalFileFormatTarget(outputfilename);

    DOMErrorHandler* errhandler = (DOMErrorHandler *)new HandlerBase();
    writer->setErrorHandler(errhandler);

    XERCES XMLString::transcode("\r\n", xmlstr, 1023);
	writer->setNewLine(xmlstr);

    XERCES XMLString::transcode("UTF-8", xmlstr, 1023);
    writer->setEncoding(xmlstr);

    XERCES XMLString::transcode("format-pretty-print", xmlstr, 1023);
    if (writer->canSetFeature(xmlstr, true)) writer->setFeature(xmlstr, true);

    DOMDocument *document = NULL;

    {
        ErrorHandler *errhandler = (ErrorHandler *)new HandlerBase();
        XercesDOMParser *parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
        parser->setDoNamespaces(true);    // optional
        parser->setErrorHandler(errhandler);

        try 
        {
            parser->parse(inputfilename);
        }
        catch (const XMLException &toCatch) 
        {
            char *message = XERCES XMLString::transcode(toCatch.getMessage());
            cout << "XML Exception message is: \n" << message << "\n";
            delete []message;
            return;
        }
        catch (const DOMException &toCatch) 
        {
            char *message = XERCES XMLString::transcode(toCatch.msg);
            cout << "DOM Exception message is: \n" << message << "\n";
            delete []message;
            return;
        }
        catch (...) 
        {
            cout << "Unexpected Exception: Maybe file "<< inputfilename << " is not accessible\n" ;
            return;
        }

        document = parser->adoptDocument();

        delete parser;
        delete errhandler;


        XERCES XMLString::transcode("Vertex", xmlstr, 99);
        DOMNodeList *vertex_nodelist = document->getElementsByTagName(xmlstr);

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
                    ARRoom::ARVertex point;
                    point.setPosition(chrstr);
                    sprintf(chrstr, "%f %f %f", point.x, point.z, -point.y);

                    XERCES XMLString::transcode("position", xmlstr1, 99);
                    XERCES XMLString::transcode(chrstr, xmlstr2, 99);
                    element->setAttributeNS(0, xmlstr1, xmlstr2);
                }
            }
        }


    }

    try 
    {
        writer->writeNode(formtarget, *document);
        delete document;
    }
    catch (const XMLException &toCatch) 
    {
        char *message = XERCES XMLString::transcode(toCatch.getMessage());
        cout << "XML Exception message is: \n" << ((message != 0) ? message : "(null)") << "\n";
        delete []message;
        return;
    }
    catch (const DOMException &toCatch) 
    {
        char *message = XERCES XMLString::transcode(toCatch.msg);
        cout << "DOM Exception message is: \n" << ((message != 0) ? message : "(null)") << "\n";
        delete []message;
        return;
    }
    catch (...) 
    {
        cout << "Unexpeted Exception: Maybe file "<< outputfilename << " is not accessible\n" ;
        return;
    }

    writer->release();
    delete formtarget;
    delete errhandler;
}
*/

void makeot(ARBuilding *b, const char *outputfilename, const char *templatefilename)
{
//    swapyz("csbuilding.xml", "csbuildingswap.xml");
    building = b;

    getmarkers();
    showmarkers();
    getboundingbox();
    makecells();
    findroomcells();

    printf("\nMaking OT file %s from %s\n", outputfilename, building->getName());
    if (templatefilename != NULL) 
    {
        printf("Using template %s\n", templatefilename);
        writeot(outputfilename, templatefilename);
    }
    else
    {
        printf("Using no template\n");
        writeot(outputfilename);
    }

  
}

