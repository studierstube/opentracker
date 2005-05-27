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
  * $Id: arbuilding.cxx 3801 2004-09-30 14:02:02Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */


#include "arbuilding.h"

ARBuilding::ARBuilding()
{
    markergridsize = 4.0f;
}

ARBuilding::~ARBuilding()
{
    roomlist.clear();
}

void 
ARBuilding::setName(const char *namestr)
{
    name = namestr;
}

const char *
ARBuilding::getName()
{
    return name.c_str();
}

int 
ARBuilding::getRoomNum()
{
    return roomlist.size();
}

void 
ARBuilding::addRoom(ARRoom *room)
{
    roomlist.push_back(room);
}

ARRoom *
ARBuilding::getRoom(int i)
{
    return roomlist[i];
}

ARRoom *
ARBuilding::getRoom(const char *name)
{
    if (name == 0) return 0;
    for (unsigned int i=0; i<roomlist.size(); i++)
    {
        ARRoom *room = roomlist[i];
        if (name == room->roomid) return room;
    }
    return 0;
}

ARRoom *
ARBuilding::getPortalRoom(ARRoom::ARPortal *portal)
{
    return getRoom(portal->room.c_str());
}

ARRoom *
ARBuilding::getPortalPolygons(ARRoom *room, ARRoom::ARPortal *portal, ARRoom::ARPolygon *&polygon, ARRoom::ARPolygon *&polygonnb)
{
    polygon = room->getPolygon(portal->polygon.c_str());
    ARRoom *roomnb = getRoom(portal->room.c_str());
    if (roomnb == 0) return 0;
    polygonnb = roomnb->getPolygon(portal->polygonnb.c_str());
    return roomnb;
}

ARRoomVec *
ARBuilding::getNeighborsIn(const char *name)
{
    ARRoomVec *rooms = new ARRoomVec();
    ARRoom *room = getRoom(name);
    for (int i=0; i<room->getPortalNum(); i++)
    {
        ARRoom::ARPortal *portal = room->getPortal(i);
        ARRoom *roomnb = getRoom(portal->room.c_str());
        for (int i=0; i<roomnb->getPortalNum(); i++)
        {
            ARRoom::ARPortal *portal = roomnb->getPortal(i);
            if (portal->room == room->roomid) rooms->push_back(roomnb);
        }
    }
    return rooms;
}

ARRoomVec *
ARBuilding::getNeighborsOut(const char *name)
{
    ARRoomVec *rooms = new ARRoomVec();
    ARRoom *room = getRoom(name);
    for (int i=0; i<room->getPortalNum(); i++)
    {
        ARRoom::ARPortal *portal = room->getPortal(i);
        ARRoom *roomnb = getRoom(portal->room.c_str());
        rooms->push_back(roomnb);
    }
    return rooms;
}

void
ARBuilding::cleanUp()
{
    for (unsigned int i=0; i<roomlist.size(); i++)
    {
        ARRoom *room = roomlist[i];
        room->cleanUp();
    }
}

void 
ARBuilding::setMarkerGridSize(float size)
{
    markergridsize = size;
}

float 
ARBuilding::getMarkerGridSize()
{
    return markergridsize;
}

