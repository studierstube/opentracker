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
  * $Id: SoBAUClientKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoBAUClientKit_H_
#define _SoBAUClientKit_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoOneShotSensor.h>

#include "signpost.h"
#include "SoBAUInstanceKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The class which communicates with the SoBAUKit. The initialization is done automatically.
 * Manages an instance of a SoBAUBuilding and its display style.
 * @ingroup signpost
 */
class SIGNPOST_API SoBAUClientKit : public SoBaseKit
{
    SO_KIT_HEADER(SoBAUClientKit);

	SO_KIT_CATALOG_ENTRY_HEADER(instance);
	SO_KIT_CATALOG_ENTRY_HEADER(style);

public:
    /// type of polygon
    enum styleType 
    { 
        WALL = SoBAUBuilding::WALL,
        FLOOR = SoBAUBuilding::FLOOR,
        CEILING = SoBAUBuilding::CEILING,
        PORTAL = SoBAUBuilding::PORTAL,
        SPECIAL = SoBAUBuilding::SPECIAL
    };

    static void initClass();
    SoBAUClientKit();
    ~SoBAUClientKit();

    /// determines, whether <i>roomList</i> is used or not
    SoSFBool useRoomList;

    /// the room list, which defines the rooms to be displayed
    SoMFString roomList;

    /// name of the portal to be displayed in current room
    SoSFString highlightedPortal;

    /// returns group for the polygon type
    /// \param style polygon type 
    /// \return group node
    SoGroup *getStyle(int style);

    /// <i>do not call</i>, only called by SoBAUInstanceKit
    void setup();

protected:
    SoOneShotSensor *setupSensor;
    static void setupSensorCB(void *data, SoSensor *sensor);
}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

