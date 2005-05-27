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
  * $Id: SoBAUInstanceKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoBAUInstanceKit_H_
#define _SoBAUInstanceKit_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFBool.h>
#include <stbapi/context/SoContext.h>

#include "signpost.h"
#include "SoBAUBuilding.h"
#include "SoBAUStyleKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * This class is used internally by SoBAUClientKit and SoBAUKit.
 * <i>Do not use it like an ordinary OpenInventor node class !</i> 
 * It contains an instance of a structure containing a BAUBuilding node
 * @ingroup signpost
 */
class SIGNPOST_API SoBAUInstanceKit : public SoBaseKit
{
    SO_KIT_HEADER(SoBAUInstanceKit);

	SO_KIT_CATALOG_ENTRY_HEADER(context);
	SO_KIT_CATALOG_ENTRY_HEADER(style);
	SO_KIT_CATALOG_ENTRY_HEADER(rooms);
	SO_KIT_CATALOG_ENTRY_HEADER(building);

public:
    static void initClass();
    SoBAUInstanceKit();
    ~SoBAUInstanceKit();

    /// unique id for instance, used as value for <i>child</i> param in setStyleGroup() of SoBAURoom and SoBAUBuilding
    SoSFInt32 contextNum;

    /// the room list, which defines the rooms to be displayed
    SoSFBool useRoomList;

    /// the room list, which defines the rooms to be displayed
    SoMFString roomList;

    /// name of the portal to be displayed in current room
    SoSFString highlightedPortal;

    /// called by SoBAUKit. <i>Do not use it !</i>
    void setup(int contextNumber, SoSeparator *building);

    /// sets style node
    /// \param sk style node
    void setStyle(SoBAUStyleKit *sk);

    /// returns containing SoBAUBuilding
    /// \return pointer to a SoBAUBuilding node
    SoBAUBuilding *getBAUBuilding();

protected:
    SoFieldSensor *roomListSensor;
    SoFieldSensor *useRoomListSensor;
    SoFieldSensor *styleSensor;
    SoFieldSensor *highlightedPortalSensor;
    SoBAUBuilding *BAUBuilding;

    static void styleSensorCB(void *data, SoSensor *sensor);
    static void roomListSensorCB(void *data, SoSensor *sensor);
    static void highlightedPortalSensorCB(void *data, SoSensor *sensor);


}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

