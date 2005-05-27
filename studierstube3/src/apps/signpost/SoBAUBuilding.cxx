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
/** SignPost BAU
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: SoBAUBuilding.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/misc/SoMultiSwitch.h>
#include "SoBAURoom.h"
#include "SoBAUBuilding.h"

SO_KIT_SOURCE(SoBAUBuilding);

//----------------------------------------------------------------------------

void
SoBAUBuilding::initClass(void)
{
    SO_KIT_INIT_CLASS(SoBAUBuilding, SoBaseKit, "BaseKit");
    SoBAURoom::initClass();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoBAUBuilding::SoBAUBuilding()
{
    SO_KIT_CONSTRUCTOR(SoBAUBuilding);

	SO_KIT_ADD_FIELD(buildingName, ("***"));
	SO_KIT_ADD_FIELD(roomNames, ("***"));
	SO_KIT_ADD_FIELD(roomCells, ("***"));
    SO_KIT_ADD_CATALOG_ENTRY(config,  SoGroup,  FALSE, this, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(rooms,  SoMultiSwitch,    FALSE, this, \x0, TRUE);

    SO_KIT_INIT_INSTANCE();

    setUpConnections( TRUE, TRUE );
}

//----------------------------------------------------------------------------

SbBool 
SoBAUBuilding::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((!doitalways) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

        // do your own special setup here !
        SoMultiSwitch *ms = (SoMultiSwitch *)getPart("rooms", FALSE);

        SoPathList *pl = searchBAURooms();
        for (int i=0; i<pl->getLength(); i++)
        {
            SoFullPath *fp =  (SoFullPath *)((*pl)[i]);
            SoBAURoom *room = (SoBAURoom *)(fp->getTail());
            roomNames.set1Value(i, room->roomName.getValue());
            roomNodes.set1Value(i, room);
            printf("Room: %s\n", room->roomName.getValue().getString());
        }
        delete pl;

        if (ms != 0) ms->whichChildren.setValue(SO_SWITCH_INHERIT);
    }
    else 
    {
        // We disconnect BEFORE base class.
        // do your own special setup here !

        SoBaseKit::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}

//----------------------------------------------------------------------------

SoPathList *
SoBAUBuilding::searchBAURooms()
{
    SoNode *n = getPart("rooms", TRUE);
    SoSearchAction sa;
    sa.setInterest(SoSearchAction::ALL);
    sa.setType(SoBAURoom::getClassTypeId(), TRUE);
    sa.setSearchingAll(TRUE);
    sa.apply(n);
    SoPathList *pl = new SoPathList(sa.getPaths());
    return pl;
}

const SbString *
SoBAUBuilding::inRoom(const SbVec3f &pt)
{
    static SoBAURoom * lastRoom = NULL;

    // test the common case first
    if( lastRoom == NULL || !lastRoom->inRoom(pt) )
    {
        lastRoom = NULL;
        for (int i=0; i<roomNodes.getNum(); i++)
        {
            SoBAURoom *room = (SoBAURoom *)(roomNodes[i]);
            if (room->inRoom(pt)) 
            {
                lastRoom = room;
                break;
            }
        }
    }

    if( lastRoom != NULL )
        return &(lastRoom->roomName.getValue());

    return NULL;
}

int 
SoBAUBuilding::getPortalIndex(const SbString &name, const SbString &neighborname)
{
    int ri = roomNames.find(name);
    if (ri < 0) return -1;
    SoBAURoom *room = (SoBAURoom *)(roomNodes[ri]);
    if (room == NULL) return -1;
    return room->getPortalIndex(neighborname);
}

SoBAURoom *
SoBAUBuilding::getRoom(const SbString &name)
{
    return (SoBAURoom *)(roomNodes[roomNames.find(name)]);
}

//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

