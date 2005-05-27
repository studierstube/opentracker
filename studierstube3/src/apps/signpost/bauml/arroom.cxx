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
  * $Id: arroom.cxx 3801 2004-09-30 14:02:02Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <math.h>
#include "arroom.h"

ARRoom::ARVertex::ARVertex() 
{ 
    x = 0.0;
    y = 0.0;
    z = 0.0;
    index = -1;
};

void
ARRoom::ARVertex::setPosition(const char *positionstr) 
{ 
    int i = 0;
    char *endptr = 0;
    while ((i < 3) && (positionstr != 0))
    {
        comp[i++] = strtod(positionstr, &endptr);
        if (endptr != 0) if (*endptr == '\0') break;
        positionstr = endptr;
    }
};

//------------------------------------------------------------------------

ARRoom::ARPolygon::ARPolygon() 
{ 
    type = -1;
};

ARRoom::ARPolygon::~ARPolygon() 
{ 
}

void
ARRoom::ARPolygon::setVertices(const char *verticesstr) 
{ 
    char *endptr;
    endptr = 0;
    while ((verticesstr != 0))
    {
        indexlist.push_back(strtol(verticesstr, &endptr, 10) );
        if (endptr != 0) if (*endptr == '\0') break;
        verticesstr = endptr;
    }
};

void
ARRoom::ARPolygon::setType(const char *typestr) 
{ 
    if (strncmp(typestr, "wall",    255) == 0) type = wall;
    if (strncmp(typestr, "floor",   255) == 0) type = floor;
    if (strncmp(typestr, "ceiling", 255) == 0) type = ceiling;
    if (strncmp(typestr, "portal",  255) == 0) type = portal;
    if (strncmp(typestr, "special", 255) == 0) type = special;
}

void
ARRoom::ARPolygon::setName(const char *namestr) 
{ 
    name = namestr;   
}

//------------------------------------------------------------------------

ARRoom::ARMarker::ARMarker() 
{ 
    number = -1;
    size = 0;
    name = "UNDEFINED";
    group = "UNDEFINED";
    override = false;
};

ARRoom::ARMarker::~ARMarker() 
{ 
}

void
ARRoom::ARMarker::setName(const char *namestr) 
{ 
    if (namestr == 0) return;
    name = (namestr[0] == '\0') ? "UNDEFINED" : namestr;
}

void
ARRoom::ARMarker::setGroup(const char *groupstr) 
{ 
    if (groupstr == 0) return;
    group = (groupstr[0] == '\0') ? "UNDEFINED" : groupstr;
}

void
ARRoom::ARMarker::setNumber(const char *numberstr) 
{ 
    char *endptr;
    number = strtol(numberstr, &endptr, 10);
}

void
ARRoom::ARMarker::setSize(const char *sizestr) 
{ 
    char *endptr;
    size = strtod(sizestr, &endptr);
}

void 
ARRoom::ARMarker::setVertices(ARVertex *pointtl, ARVertex *pointtr, ARVertex *pointbl, ARVertex *pointbr)
{
    if (pointbr == NULL)
    {
        cenx = (pointtr->x + pointbl->x) * 0.50;
        ceny = (pointtr->y + pointbl->y) * 0.50;
        cenz = (pointtr->z + pointbl->z) * 0.50;
    }
    else
    {
        cenx = (pointtl->x + pointtr->x + pointbl->x + pointbr->x) * 0.25;
        ceny = (pointtl->y + pointtr->y + pointbl->y + pointbr->y) * 0.25;
        cenz = (pointtl->z + pointtr->z + pointbl->z + pointbr->z) * 0.25;
    }

    double len;

    horx = pointtr->x - pointtl->x;
    hory = pointtr->y - pointtl->y;
    horz = pointtr->z - pointtl->z;
    len = sqrt(horx*horx + hory*hory + horz*horz);
    horx /= len;
    hory /= len;
    horz /= len;
 
    verx = pointtl->x - pointbl->x; // (tl - bl) is intended because of marker coord-sys
    very = pointtl->y - pointbl->y;
    verz = pointtl->z - pointbl->z;
    len = sqrt(verx*verx + very*very + verz*verz);
    verx /= len;
    very /= len;
    verz /= len;

    dirx = (hory * verz - horz * very);
    diry = (horz * verx - horx * verz);
    dirz = (horx * very - hory * verx);              
}


void
ARRoom::ARMarker::setVertices(const char *verticesstr, ARRoom *owner, int offset) 
{ 
    std::vector<int> indexlist;
    char *endptr;
    endptr = 0;
    while ((verticesstr != 0))
    {
        indexlist.push_back(strtol(verticesstr, &endptr, 10) + offset);
        if (endptr != 0) if (*endptr == '\0') break;
        verticesstr = endptr;
    }
    if (indexlist.size() != 4) return;

    ARVertex *pointtl = owner->getVertex(indexlist[0]);
    ARVertex *pointtr = owner->getVertex(indexlist[1]);
    ARVertex *pointbl = owner->getVertex(indexlist[2]);
    ARVertex *pointbr = owner->getVertex(indexlist[3]);

    setVertices(pointtl, pointtr, pointbl, pointbr);
};

double 
ARRoom::ARMarker::getDeterminant()
{
    return (horx*very*dirz + hory*verz*dirx + horz*verx*diry - horx*verz*diry - hory*verx*dirz - horz*very*dirx);
}

void 
ARRoom::ARMarker::setOverride(bool or)
{
    override = or;
}

bool 
ARRoom::ARMarker::isOverrider()
{
    return override;
}



//------------------------------------------------------------------------

ARRoom::ARPortal::ARPortal() 
{ 
};

ARRoom::ARPortal::~ARPortal() 
{ 
}

//------------------------------------------------------------------------

ARRoom::ARRoom()
{
}

ARRoom::~ARRoom()
{
    vertexlist.clear();
    polygonlist.clear();
    markerlist.clear();
    portallist.clear();
/*
    int i;
    for (i=0; i<vertexlist.size(); i++) delete (vertexlist[i]);
    for (i=0; i<polygonlist.size(); i++) delete (polygonlist[i]);
    for (i=0; i<markerlist.size(); i++) delete (markerlist[i]);
    for (i=0; i<portallist.size(); i++) delete (portallist[i]);
*/
}

void 
ARRoom::setRoomId(const char *id)
{
    roomid = id;
}

void 
ARRoom::setOrigin(const char *position)
{
    origin.setPosition(position);
}

void 
ARRoom::setOrigin(double x, double y, double z)
{
    origin.x = x;
    origin.y = y;
    origin.z = z;
}

const char *
ARRoom::getRoomId()
{
    return roomid.c_str();
}

void 
ARRoom::getOrigin(double &x, double &y, double &z)
{
    x = origin.x;
    y = origin.y;
    z = origin.z;
}

int 
ARRoom::getVertexNum()
{
    return vertexlist.size();
}

int 
ARRoom::getPolygonNum()
{
    return polygonlist.size();
}

int 
ARRoom::getMarkerNum()
{
    return markerlist.size();
}

int 
ARRoom::getPortalNum()
{
    return portallist.size();
}

void
ARRoom::addVertex(ARVertex *vertex)
{
    vertex->index = vertexlist.size();
    vertexlist.push_back(vertex);
}

void
ARRoom::addPolygon(ARPolygon *polygon)
{
    polygonlist.push_back(polygon);
}

void
ARRoom::addMarker(ARMarker *marker)
{
    markerlist.push_back(marker);
}

void
ARRoom::addPortal(ARPortal *portal)
{
    portallist.push_back(portal);
}

ARRoom::ARVertex *
ARRoom::getVertex(int i)
{
    return vertexlist[i];
}

ARRoom::ARPolygon *
ARRoom::getPolygon(int i)
{
    return polygonlist[i];
}

ARRoom::ARPolygon *
ARRoom::getPolygon(const char *name)
{
    if (name == 0) return 0;
    for (unsigned int i=0; i<polygonlist.size(); i++)
    {
        ARPolygon *polygon = polygonlist[i];
        if (name == polygon->name) return polygon;
    }
    return 0;
}

ARRoom::ARMarker *
ARRoom::getMarker(int i)
{
    return markerlist[i];
}

ARRoom::ARPortal *
ARRoom::getPortal(int i)
{
    return portallist[i];
}

void 
ARRoom::getPolygonCenter(ARPolygon *polygon, double &x, double &y, double &z)
{
    x = 0;
    y = 0;
    z = 0;
    int n = polygon->indexlist.size();
    if (n == 0) return;
    for (int i=0; i<n; i++)
    {
        ARVertex *vertex = vertexlist[polygon->indexlist[i]];
        x += vertex->x;
        y += vertex->y;
        z += vertex->z;
    }
    x /= n;
    y /= n;
    z /= n;
}

void 
ARRoom::getPolygonCenterAbsolute(ARPolygon *polygon, double &x, double &y, double &z)
{
    getPolygonCenter(polygon, x, y, z);
    x += origin.x;
    y += origin.y;
    z += origin.z;
}

ARRoom::ARPolygon * 
ARRoom::getPortalPolygon(ARRoom::ARPortal *portal)
{
    return getPolygon(portal->polygon.c_str());
}

//------------------------------------------------------------------------

void 
ARRoom::cleanUp()
{
    unsigned int i,j;

    for (i=0; i<vertexlist.size(); i++)
    {
        ARVertex *vertex = vertexlist[i];
        vertex->index = -1;
    }

    for (i=0; i<polygonlist.size(); i++)
    {
        ARPolygon *polygon = polygonlist[i];
        unsigned int n = polygon->indexlist.size();
        for (j=0; j<n; j++)
        {
            ARVertex *vertex = vertexlist[polygon->indexlist[j]];
            vertex->index = -2;
        }
    }

    j = 0;
    for (i=0; i<vertexlist.size(); i++)
    {
        ARVertex *vertex = vertexlist[i];
        if (vertex->index == -2) vertex->index = j++;
    }
}

