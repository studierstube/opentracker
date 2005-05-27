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
/** SignPost WIM
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: SoWIMKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/util/ivio.h>

#include "SoWIMKit.h"

SO_KIT_SOURCE(SoWIMKit);

//----------------------------------------------------------------------------

void
SoWIMKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoWIMKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoWIMKit::SoWIMKit()
{
    SO_KIT_CONSTRUCTOR(SoWIMKit);

    SO_KIT_DEFINE_ENUM_VALUE(contentEnum, ALL);
    SO_KIT_DEFINE_ENUM_VALUE(contentEnum, PATH);
    SO_KIT_DEFINE_ENUM_VALUE(contentEnum, ROOM);
    SO_KIT_SET_SF_ENUM_TYPE(content, contentEnum);

    SO_KIT_ADD_FIELD(fixed, (FALSE));
    SO_KIT_ADD_FIELD(worldAligned, (TRUE)); 
    SO_KIT_ADD_FIELD(scale, (0.02));
    SO_KIT_ADD_FIELD(center, (0,0,0));
    SO_KIT_ADD_FIELD(wireframe, (TRUE)); 
    SO_KIT_ADD_FIELD(translucent, (TRUE)); 
    SO_KIT_ADD_FIELD(content, (ALL)); 
    SO_KIT_ADD_FIELD(path, ("")); 
    SO_KIT_ADD_FIELD(currentRoom, ("")); 

    SO_KIT_INIT_INSTANCE();

    contentSensor = new SoFieldSensor();
    contentSensor->setFunction(SoWIMKit::contentSensorCB);
    contentSensor->setData(this);
    contentSensor->attach(&content);

    setUpConnections( TRUE, TRUE );
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoWIMKit::~SoWIMKit()
{
    delete contentSensor;
}

//----------------------------------------------------------------------------

SoContextKit*
SoWIMKit::factory()
{
    return new SoWIMKit();
}

//----------------------------------------------------------------------------

SbBool 
SoWIMKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

        // do your own special setup here !
        //BAUClientKitWireframe = (SoBAUClientKit *)getByName(SbName("WIM_BAUCLIENTKITWIREFRAME"));
        // BAUClientKitTranslucent = (SoBAUClientKit *)getByName(SbName("WIM_BAUCLIENTKITTRANSLUCENT"));
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

void
SoWIMKit::checkPipMasterMode(SoNode *pipSheet, SbBool masterMode)
{
}

//----------------------------------------------------------------------------

void
SoWIMKit::contentSensorCB(void *data, SoSensor *sensor)
{
    printf("*** SoWIMKit::contentSensorCB\n");
    SoWIMKit *wk = (SoWIMKit *)data;
    /*
    switch (wk->content.getValue())
    {
        case ROOM:
        {
            printf("ROOM\n");
            wk->BAUClientKitWireframe->useRoomList.setValue(TRUE);
            wk->BAUClientKitTranslucent->useRoomList.setValue(TRUE);
            wk->BAUClientKitWireframe->roomList.setValue(wk->currentRoom.getValue());
            wk->BAUClientKitTranslucent->roomList.setValue(wk->currentRoom.getValue());
            break;
        }
        case PATH:
        {
            printf("PATH\n");
            wk->BAUClientKitWireframe->useRoomList.setValue(TRUE);
            wk->BAUClientKitTranslucent->useRoomList.setValue(TRUE);
            for (int i=0; i<wk->path.getNum(); i++)
            {
                wk->BAUClientKitWireframe->roomList.set1Value(i, wk->path[i]);
                wk->BAUClientKitTranslucent->roomList.set1Value(i, wk->path[i]);
            }
            break;
        }
        case ALL:
        {
            printf("ALL\n");
            wk->BAUClientKitWireframe->useRoomList.setValue(FALSE);
            wk->BAUClientKitTranslucent->useRoomList.setValue(FALSE);
            break;
        }     
    }
    */
}

//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

