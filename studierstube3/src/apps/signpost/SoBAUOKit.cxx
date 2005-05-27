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
/** SignPost Augmentation
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: SoBAUOKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/misc/SoMultiSwitch.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/util/ivio.h>

#include "SoBAUOKit.h"

SO_KIT_SOURCE(SoBAUOKit);

//----------------------------------------------------------------------------

void
SoBAUOKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoBAUOKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoBAUOKit::SoBAUOKit()
{
    SO_KIT_CONSTRUCTOR(SoBAUOKit);

    SO_KIT_DEFINE_ENUM_VALUE(renderStyleEnum, INVISIBLE);
    SO_KIT_DEFINE_ENUM_VALUE(renderStyleEnum, HIDDENLINE);
    SO_KIT_DEFINE_ENUM_VALUE(renderStyleEnum, WIREFRAME);
    SO_KIT_DEFINE_ENUM_VALUE(renderStyleEnum, TRANSLUCENT);
    SO_KIT_DEFINE_ENUM_VALUE(renderStyleEnum, WIREFRAME_TRANSLUCENT);
    SO_KIT_DEFINE_ENUM_VALUE(contentEnum, ALL);
    SO_KIT_DEFINE_ENUM_VALUE(contentEnum, PATH);
    SO_KIT_DEFINE_ENUM_VALUE(contentEnum, ROOM);

    SO_KIT_SET_SF_ENUM_TYPE(roomStyle, renderStyleEnum);
    SO_KIT_SET_SF_ENUM_TYPE(portalStyle, renderStyleEnum);
    SO_KIT_SET_SF_ENUM_TYPE(content, contentEnum);

    SO_KIT_ADD_FIELD(roomStyle, (WIREFRAME)); 
    SO_KIT_ADD_FIELD(portalStyle, (WIREFRAME)); 
    SO_KIT_ADD_FIELD(content, (ALL)); 
    SO_KIT_ADD_FIELD(path, ("")); 
    SO_KIT_ADD_FIELD(currentRoom, ("")); 

    SO_KIT_INIT_INSTANCE();

    contentSensor = new SoFieldSensor();
    contentSensor->setFunction(SoBAUOKit::contentSensorCB);
    contentSensor->setData(this);

    roomStyleSensor = new SoFieldSensor();
    roomStyleSensor->setFunction(SoBAUOKit::roomStyleSensorCB);
    roomStyleSensor->setData(this);

    portalStyleSensor = new SoFieldSensor();
    portalStyleSensor->setFunction(SoBAUOKit::portalStyleSensorCB);
    portalStyleSensor->setData(this);

    zbufferDrawSwitch = 0;
    roomStyleMultiSwitch = 0;

    setUpConnections( TRUE, TRUE );
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoBAUOKit::~SoBAUOKit()
{
    delete contentSensor;
    delete roomStyleSensor;
    delete portalStyleSensor;
}

//----------------------------------------------------------------------------

SoContextKit*
SoBAUOKit::factory()
{
    return new SoBAUOKit();
}

//----------------------------------------------------------------------------

SbBool 
SoBAUOKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

        // do your own special setup here !
        zbufferDrawSwitch = (SoSwitch *)getByName(SbName("AUG_ZBUFFERDRAWSWITCH"));
        roomStyleMultiSwitch = (SoMultiSwitch *)getByName(SbName("AUG_ROOMSTYLEMULTISWITCH"));

        contentSensor->attach(&content);
        roomStyleSensor->attach(&roomStyle);
        portalStyleSensor->attach(&portalStyle);
    }
    else 
    {
        // We disconnect BEFORE base class.
        // do your own special setup here !

        contentSensor->detach();
        roomStyleSensor->detach();
        portalStyleSensor->detach();
        
        SoBaseKit::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}

void
SoBAUOKit::wireframeButtonCB(void *data, SoButtonKit *button)
{
    SoBAUOKit *ak = (SoBAUOKit *)data;
    ak->roomStyle.setValue(WIREFRAME);
}

void
SoBAUOKit::hiddenlineButtonCB(void *data, SoButtonKit *button)
{
    SoBAUOKit *ak = (SoBAUOKit *)data;
    ak->roomStyle.setValue(HIDDENLINE);
}

void
SoBAUOKit::checkPipMasterMode(SoNode *pipSheet, SbBool masterMode)
{
    printf("WIMKit: checkPipMasterMode\n");

    if(masterMode)
    {
        SoPushButton *wfb = (SoPushButton *)findNode(pipSheet,"AUG_WIREFRAME");
        wfb->setPressCallback(wireframeButtonCB, this);
        SoPushButton *hlb = (SoPushButton *)findNode(pipSheet,"AUG_HIDDENLINE");
        hlb->setPressCallback(hiddenlineButtonCB, this);
    }
    else
    {
        SoPushButton *wfb = (SoPushButton *)findNode(pipSheet,"AUG_WIREFRAME");
        wfb->removePressCallback();
        SoPushButton *hlb = (SoPushButton *)findNode(pipSheet,"AUG_HIDDENLINE");
        hlb->removePressCallback();
    }

}

//----------------------------------------------------------------------------

void
SoBAUOKit::contentSensorCB(void *data, SoSensor *sensor)
{
    printf("*** SoBAUOKit::contentSensorCB\n");
    SoBAUOKit *bk = (SoBAUOKit *)data;
    /*
    switch (bk->content.getValue())
    {
        case ROOM:
        {
            printf("ROOM\n");
            bk->BAUClientKitWireframe->useRoomList.setValue(TRUE);
            bk->BAUClientKitTranslucent->useRoomList.setValue(TRUE);
            bk->BAUClientKitWireframe->roomList.setValue(bk->currentRoom.getValue());
            bk->BAUClientKitTranslucent->roomList.setValue(bk->currentRoom.getValue());
            break;
        }
        case PATH:
        {
            printf("PATH\n");
            bk->BAUClientKitWireframe->useRoomList.setValue(TRUE);
            bk->BAUClientKitTranslucent->useRoomList.setValue(TRUE);
            for (int i=0; i<bk->path.getNum(); i++)
            {
                bk->BAUClientKitWireframe->roomList.set1Value(i, bk->path[i]);
                bk->BAUClientKitTranslucent->roomList.set1Value(i, bk->path[i]);
            }
            break;
        }
        case ALL:
        {
            printf("ALL\n");
            bk->BAUClientKitWireframe->useRoomList.setValue(FALSE);
            bk->BAUClientKitTranslucent->useRoomList.setValue(FALSE);
            break;
        }
    }
    */
}

void
SoBAUOKit::roomStyleSensorCB(void *data, SoSensor *sensor)
{
    printf("*** SoBAUOKit::roomStyleSensorCB\n");
    SoBAUOKit *bk = (SoBAUOKit *)data;
    bk->roomStyleMultiSwitch->whichChildren.setValue(SO_SWITCH_NONE);
    switch (bk->roomStyle.getValue())
    {
        case WIREFRAME:
        {
            printf("WIREFRAME\n");
            bk->zbufferDrawSwitch->whichChild.setValue(SO_SWITCH_NONE);
            bk->roomStyleMultiSwitch->whichChildren.setValue(0);
            break;
        }
        case HIDDENLINE:
        {
            printf("HIDDENLINE\n");
            bk->zbufferDrawSwitch->whichChild.setValue(SO_SWITCH_ALL);
//        bk->roomStyleMultiSwitch->whichChildren.setValue(0);
            bk->roomStyleMultiSwitch->whichChildren.set1Value(0,0);
//          bk->roomStyleMultiSwitch->whichChildren.set1Value(1,1);
            break;
        }
        case TRANSLUCENT:
        {
            printf("TRANSLUCENT\n");
            bk->zbufferDrawSwitch->whichChild.setValue(SO_SWITCH_NONE);
            bk->roomStyleMultiSwitch->whichChildren.setValue(1);
            break;
        }
        case WIREFRAME_TRANSLUCENT:
        {
            printf("WIREFRAME_TRANSLUCENT\n");
            bk->zbufferDrawSwitch->whichChild.setValue(SO_SWITCH_NONE);
            bk->roomStyleMultiSwitch->whichChildren.set1Value(0,0);
            bk->roomStyleMultiSwitch->whichChildren.set1Value(1,1);
            break;
        }
    }

}

void
SoBAUOKit::portalStyleSensorCB(void *data, SoSensor *sensor)
{
    printf("*** SoBAUOKit::portalStyleSensorCB\n");
    SoBAUOKit *bk = (SoBAUOKit *)data;
}

//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

