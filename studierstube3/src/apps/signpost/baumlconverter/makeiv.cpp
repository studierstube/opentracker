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
  * $Id: makeiv.cpp 4221 2005-05-24 14:48:57Z schall $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <stbapi/context/SoContextSwitch.h>
#include <stbapi/util/ivutil.h>
#include "SoBAURoom.h"
#include "SoBAUBuilding.h"
#include <iostream>

#include "bauml.h"
using namespace std;


SoBAURoom *
initRoom(ARRoom *room)
{
    int j,k;
    printf("initRoom: %s\n", room->getRoomId());
    SoBAURoom *BAURoom = new SoBAURoom();
    double ox, oy, oz;
    room->getOrigin(ox, oy, oz);

    // name
    BAURoom->roomName.setValue(room->getRoomId());

    // coords
    SoCoordinate3 *roomCoords = (SoCoordinate3 *)(BAURoom->getPart("roomCoords", TRUE));
	for (j=0; j<room->getVertexNum(); j++)
	{
        ARRoom::ARVertex *vertex = room->getVertex(j);
    	roomCoords->point.set1Value(j, ox+vertex->x, oy+vertex->y, oz+vertex->z);
    }

    // polygons
    SoIndexedFaceSet *wallFaceSet = (SoIndexedFaceSet *)(BAURoom->getPart("wallFaceSet", TRUE));
    SoIndexedFaceSet *floorFaceSet = (SoIndexedFaceSet *)(BAURoom->getPart("floorFaceSet", TRUE));
    SoIndexedFaceSet *ceilingFaceSet = (SoIndexedFaceSet *)(BAURoom->getPart("ceilingFaceSet", TRUE));
//    SoIndexedFaceSet *portalFaceSet = (SoIndexedFaceSet *)(BAURoom->getPart("portalFaceSet", TRUE));
    SoIndexedFaceSet *specialFaceSet = (SoIndexedFaceSet *)(BAURoom->getPart("specialFaceSet", TRUE));

    wallFaceSet->coordIndex.setNum(0);
    floorFaceSet->coordIndex.setNum(0);
    ceilingFaceSet->coordIndex.setNum(0);
//    portalFaceSet->coordIndex.setNum(0);
    specialFaceSet->coordIndex.setNum(0);

    int floorcoordnum = 0;
    int portalidx = 0;
    for (j=0; j<room->getPolygonNum(); j++)
    {
        ARRoom::ARPolygon *polygon = room->getPolygon(j);
        SoIndexedFaceSet *faceset = 0;
        switch (polygon->type)
        {
            case ARRoom::wall: faceset = wallFaceSet; break;
            case ARRoom::floor: faceset = floorFaceSet; break;
            case ARRoom::ceiling: faceset = ceilingFaceSet; break;
//            case ARRoom::portal: faceset = portalFaceSet; break;
            case ARRoom::special: faceset = specialFaceSet; break;
        }
        if (faceset != 0)
        {
            int idx = faceset->coordIndex.getNum(); 
    	    for (k=0; k<polygon->indexlist.size(); k++)	faceset->coordIndex.set1Value(idx++, polygon->indexlist[k]);
    	    faceset->coordIndex.set1Value(idx++, SO_END_FACE_INDEX);

            if (polygon->type == ARRoom::floor)
            {
                SoCoordinate3 *coord = new SoCoordinate3();
        	    for (k=0; k<polygon->indexlist.size(); k++)	coord->point.set1Value(k, roomCoords->point[polygon->indexlist[k]]);
                BAURoom->floorCoordinates.set1Value(floorcoordnum, coord);
                floorcoordnum++;
            }
        }

        if (polygon->type == ARRoom::portal)
        {
            SoIndexedFaceSet *faceset = new SoIndexedFaceSet();
    	    for (k=0; k<polygon->indexlist.size(); k++)	faceset->coordIndex.set1Value(k, polygon->indexlist[k]);
    	    faceset->coordIndex.set1Value(k, SO_END_FACE_INDEX);
            SoContextSwitch *cs = (SoContextSwitch *)(BAURoom->getPart("portalGeometryContextSwitch", TRUE));
            cs->addChild(faceset);
            BAURoom->portalNames.set1Value(portalidx++, polygon->name.c_str());
        }
    }

    for (j=0; j<room->getPortalNum(); j++)
    {
        double x, y, z;
        ARRoom::ARPortal *portal = room->getPortal(j);
        ARRoom::ARPolygon *polygon = room->getPortalPolygon(portal);
        room->getPolygonCenterAbsolute(polygon, x, y, z);
        BAURoom->neighbors.set1Value(j, portal->room.c_str());
        BAURoom->portalPositions.set1Value(j, x, y, z);
/*
        // make portal geometry
        SoIndexedFaceSet *pifs = new SoIndexedFaceSet();
        for (int i=0; i<polygon->indexlist.size(); i++)
        {
            ARRoom::ARVertex *vertex = room->getVertex(polygon->indexlist[i]);
            pc->point.set1Value(i, vertex->x, vertex->y, vertex->z);
        }
        SoContextSwitch *cs = (SoContextSwitch *)(BAURoom->getPart("portalGeometryContextSwitch", TRUE));
        cs->addChild(pfs);
*/
    }

/*
    int i;

    SoIndexedFaceSet *ifs = (SoIndexedFaceSet *)getPart("portalFaceSet", FALSE);
    SoCoordinate3 *c = (SoCoordinate3 *)getPart("roomCoords", FALSE);
    if ((ifs == NULL) || (c == NULL)) return NULL;


    int ni = -1;
    for (i=0; i<neighbors.getNum(); i++)
    {
        if (neighbors[i] == neighborname)
        {
            ni = i;
            break;
        }
    }

    if (ni < 0) return 0;

    int poly = 0;
    for (i=0; i<ifs->coordIndex.getNum(); i++)
    {
        if (poly == ni)
        {
            SoIndexedFaceSet *pifs = new SoIndexedFaceSet();
            SoCoordinate3 *pc = new SoCoordinate3();
            pifs->vertexProperty.setValue(pc);
            int j = 0;
            while ((i<ifs->coordIndex.getNum()) && (ifs->coordIndex[i] != -1))
            {
                pc->point.set1Value(j, c->point[ifs->coordIndex[i]]);
                pifs->coordIndex.set1Value(j, j);
                i++;
                j++;
            }
            return pifs;
        }
        if (ifs->coordIndex[i] == -1) poly++;
    }
    return NULL;
*/
    return BAURoom;
}

SoBAUBuilding *
initBuilding(ARBuilding *building)
{
    printf("initBuilding: %s\n", building->getName());
    SoBAUBuilding *BAUBuilding = new SoBAUBuilding();
    BAUBuilding->buildingName.setValue(building->getName());
    for (int i=0; i<building->getRoomNum(); i++)
    {
        ARRoom *room = building->getRoom(i);
        SoBAURoom *BAURoom = initRoom(room);
        SoGroup *lp = (SoGroup *)(BAUBuilding->getPart("rooms", TRUE));
        lp->addChild(BAURoom);
        BAUBuilding->roomCells.set1Value(2*i, room->getRoomId());
        //BAUBuilding->roomCells.set1Value(2*i+1, room->cell.c_str()); // Gerhard Schall
    }
    return BAUBuilding;
}


void makeiv(ARBuilding *building, const char *filename)
{
    // QWidget *window = SoQt::init("Fullscreen");

    SoDB::init();
    SoNodeKit::init();
    initStbClasses();
    SoBAUBuilding::initClass();

    printf("Making IV file %s from %s\n", filename, building->getName());
    SoBAUBuilding *BAUBuilding = initBuilding(building);
    BAUBuilding->ref();
    BAUBuilding->setName("Model");
    SoOutput o;
    o.openFile(filename);
    SoWriteAction wa(&o);
    wa.apply(BAUBuilding);
    o.closeFile();
    BAUBuilding->unref();
}
