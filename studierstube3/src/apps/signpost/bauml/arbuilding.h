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
  * $Id: arbuilding.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __arbuilding_h__
#define __arbuilding_h__

#include <iostream>
#include <vector>
#include <string>

#include "arroom.h"

class ARBuilding
{
public:
    ARBuilding();
    virtual ~ARBuilding();

    void setName(const char *namestr);
    const char *getName();
    int getRoomNum();
    void addRoom(ARRoom *room);
    ARRoom *getRoom(int i);
    ARRoom *getRoom(const char *name);
    ARRoom *getPortalRoom(ARRoom::ARPortal *portal);
    ARRoom *getPortalPolygons(ARRoom *room, ARRoom::ARPortal *portal, ARRoom::ARPolygon *&polygon, ARRoom::ARPolygon *&polygonnb);
    ARRoomVec *getNeighborsIn(const char *name);
    ARRoomVec *getNeighborsOut(const char *name);

    void cleanUp();

    void setMarkerGridSize(float size);
    float getMarkerGridSize();

protected:
    std::string name;
    std::vector<ARRoom *> roomlist;
    float markergridsize;

private:

};

#endif

