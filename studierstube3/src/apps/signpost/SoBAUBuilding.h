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
  * $Id: SoBAUBuilding.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoBAUBuilding_H_
#define _SoBAUBuilding_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include "SoBAURoom.h"
#include "signpost.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * contains building data.
 * @ingroup signpost
 */
class SIGNPOST_API SoBAUBuilding : public SoBaseKit
{
    SO_KIT_HEADER(SoBAUBuilding);

    SO_KIT_CATALOG_ENTRY_HEADER(config);
    SO_KIT_CATALOG_ENTRY_HEADER(rooms);

public:
    /// type of polygon
    enum styleType 
    { 
        WALL = SoBAURoom::WALL,
        FLOOR = SoBAURoom::FLOOR,
        CEILING = SoBAURoom::CEILING,
        PORTAL = SoBAURoom::PORTAL,
        SPECIAL = SoBAURoom::SPECIAL,
        ALL = SoBAURoom::ALL
    };

    static void initClass();
    SoBAUBuilding();

    /// returns name of room, which the point <i>pt</i> is in.
    /// \param pt position, z value is ignored
    /// \return name of room
    const SbString *inRoom(const SbVec3f &pt);

    /// returns index for portal in room <i>name</i> between room <i>name</i> and room <i>neighbourname</i>
    /// \param name name of room
    /// \param neighbourname name of neighbor of room
    /// \return room node
    int getPortalIndex(const SbString &name, const SbString &neighborname);

    /// returns room node
    /// \param name room name
    /// \return room node
    SoBAURoom *getRoom(const SbString &name);

    /// name of building
    SoSFString buildingName;
    /// room names, same order as in 'config' node, <i>do not modify during runtime !</i>
    SoMFString roomNames;
    /// contains representative GroupGate names for each room
    SoMFString roomCells;
    /// room nodes, same order as in 'config' node, <i>do not modify during runtime !</i>
    SoMFNode roomNodes;

protected:
    SoPathList *searchBAURooms();
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);
}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

