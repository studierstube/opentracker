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
  * $Id: SoBAURoom.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoBAURoom_H_
#define _SoBAURoom_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include "signpost.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * contains room data.
 * @ingroup signpost
 */
class SIGNPOST_API SoBAURoom : public SoBaseKit
{
    SO_KIT_HEADER(SoBAURoom);

	SO_KIT_CATALOG_ENTRY_HEADER(room);
	SO_KIT_CATALOG_ENTRY_HEADER(roomCoords);
	SO_KIT_CATALOG_ENTRY_HEADER(allAppearance);

	SO_KIT_CATALOG_ENTRY_HEADER(wallSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(floorSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(ceilingSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(portalSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(specialSeparator);

  	SO_KIT_CATALOG_ENTRY_HEADER(wallAppearance);
  	SO_KIT_CATALOG_ENTRY_HEADER(floorAppearance);
  	SO_KIT_CATALOG_ENTRY_HEADER(ceilingAppearance);
  	SO_KIT_CATALOG_ENTRY_HEADER(portalAppearance);
  	SO_KIT_CATALOG_ENTRY_HEADER(specialAppearance);

  	SO_KIT_CATALOG_ENTRY_HEADER(wallFaceSet);
  	SO_KIT_CATALOG_ENTRY_HEADER(floorFaceSet);
  	SO_KIT_CATALOG_ENTRY_HEADER(ceilingFaceSet);
  	SO_KIT_CATALOG_ENTRY_HEADER(portalFaceSet);
  	SO_KIT_CATALOG_ENTRY_HEADER(specialFaceSet);

  	SO_KIT_CATALOG_ENTRY_HEADER(portalGeometryContextSwitch);

public:

    /// type of polygon
    enum styleType 
    {
        FLOOR = 0, 
        WALL = 1, 
        CEILING = 2, 
        PORTAL = 3, 
        SPECIAL = 4, 
        ALL = 5
    };

    static void initClass();
    SoBAURoom();
    ~SoBAURoom();

    /// checks wheter point <i>pt</i> is in room
    /// \param pt position
    /// \return true if in room
    bool inRoom(const SbVec3f &pt);

    /// returns index into port
    /// \param neighborname name of neighbor
    /// \return index into portalNames
    int getPortalIndex(const SbString &neighborname);

    /// room name
    SoSFString roomName;

    /// names of its neighbours, reachable through portals 
    SoMFString neighbors;

    /// names of portals, should have the same names a the neighbour
    SoMFString portalNames;

    /// point, which represents a portal
    SoMFVec3f portalPositions;

    /// contains a SoCoordinate3 node containing the floor coordinates. 
    /// The points must define the floor polygon. used in inRoom()
    SoMFNode floorCoordinates;

protected:

    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);
}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

