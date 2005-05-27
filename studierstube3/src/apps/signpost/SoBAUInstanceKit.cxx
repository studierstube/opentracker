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
  * $Id: SoBAUInstanceKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/misc/SoMultiSwitch.h>
#include "SoBAUInstanceKit.h"

SO_KIT_SOURCE(SoBAUInstanceKit);

//----------------------------------------------------------------------------

void
SoBAUInstanceKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoBAUInstanceKit, SoBaseKit, "BaseKit");
    SoBAUStyleKit::initClass();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoBAUInstanceKit::SoBAUInstanceKit()
{
    SO_KIT_CONSTRUCTOR(SoBAUInstanceKit);

	SO_KIT_ADD_FIELD(contextNum, (-1));
	SO_KIT_ADD_FIELD(useRoomList, (FALSE));
	SO_KIT_ADD_FIELD(roomList, (""));
	SO_KIT_ADD_FIELD(highlightedPortal, (""));

    SO_KIT_ADD_CATALOG_ENTRY(context,   SoContext,      FALSE, this, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(style,  SoBAUStyleKit,     TRUE, this, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(rooms,  SoMultiSwitch,     FALSE, this, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(building,  SoSeparator,    TRUE, this, \x0, TRUE);

    SO_KIT_INIT_INSTANCE();

    styleSensor = new SoFieldSensor();
    styleSensor->setFunction(SoBAUInstanceKit::styleSensorCB);
    styleSensor->setData(this);

    roomListSensor = new SoFieldSensor();
    roomListSensor->setFunction(SoBAUInstanceKit::roomListSensorCB);
    roomListSensor->setData(this);

    useRoomListSensor = new SoFieldSensor();
    useRoomListSensor->setFunction(SoBAUInstanceKit::roomListSensorCB);
    useRoomListSensor->setData(this);

    highlightedPortalSensor = new SoFieldSensor();
    highlightedPortalSensor->setFunction(SoBAUInstanceKit::highlightedPortalSensorCB);
    highlightedPortalSensor->setData(this);

    styleSensor->attach(&style);
    roomListSensor->attach(&roomList);
    useRoomListSensor->attach(&useRoomList);
    highlightedPortalSensor->attach(&highlightedPortal);
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoBAUInstanceKit::~SoBAUInstanceKit()
{
    delete styleSensor;
    delete roomListSensor;
    delete useRoomListSensor;
    delete highlightedPortalSensor;
}

//----------------------------------------------------------------------------

void 
SoBAUInstanceKit::setup(int contextNumber, SoSeparator *building)
{
    printf("SoBAUInstanceKit: setup (%i)\n", contextNumber);

    contextNum.setValue(contextNumber);

    // context
    SoContext *c = new SoContext();
    c->mode.setValue(SoContext::ADD);
    c->index.set1Value(0, "BAU.style");
    c->value.set1Value(0, SbString(contextNumber));
    c->index.set1Value(1, "BAU.portal");
    c->value.set1Value(1, SbString(-3));
    setPart("context", c);

    printf("SoBAUInstance context %x\n", c);

    setPart("building", building);

    SoSearchAction sa;
    sa.setInterest(SoSearchAction::FIRST);
    sa.setType(SoBAUBuilding::getClassTypeId(), TRUE);
    sa.setSearchingAll(TRUE);
    sa.apply(building);
    SoFullPath *fp = (SoFullPath *)(sa.getPath());
    BAUBuilding = (SoBAUBuilding *)(fp->getTail());

    printf("SoBAUInstanceKit: setup (%i) building: %s\n", contextNumber, BAUBuilding->buildingName.getValue().getString());
}

void 
SoBAUInstanceKit::setStyle(SoBAUStyleKit *sk)
{
    setPart("style", sk);
    styleSensor->trigger();
}

SoBAUBuilding *
SoBAUInstanceKit::getBAUBuilding()
{
    return BAUBuilding;
}


//----------------------------------------------------------------------------

void
SoBAUInstanceKit::styleSensorCB(void *data, SoSensor *sensor)
{
//printf("*** SoBAUInstanceKit::styleSensorCB\n");
    SoBAUInstanceKit *ik = (SoBAUInstanceKit *)data;
    SoBAUStyleKit *sk = (SoBAUStyleKit *)(ik->getPart("style", FALSE));
    if (sk == 0) return;

    int child = ik->contextNum.getValue();
    SoGroup *g = 0;

    SoGroup *invisibleStyle = new SoGroup();
    invisibleStyle->ref();
    SoDrawStyle *ds = new SoDrawStyle();
    invisibleStyle->addChild(ds);
    ds->style.setValue(SoDrawStyle::INVISIBLE);

    g = (SoGroup *)(sk->getPart("floorStyle", FALSE));
    if (g == NULL) sk->setPart("floorStyle", g = (SoGroup *)invisibleStyle->copy());
    ik->BAUBuilding->setStyleGroup(SoBAUBuilding::FLOOR, child, g);

    g = (SoGroup *)(sk->getPart("wallStyle", FALSE));
    if (g == NULL) sk->setPart("wallStyle", g = (SoGroup *)invisibleStyle->copy());
    ik->BAUBuilding->setStyleGroup(SoBAUBuilding::WALL, child, g);

    g = (SoGroup *)(sk->getPart("ceilingStyle", FALSE));
    if (g == NULL) sk->setPart("ceilingStyle", g = (SoGroup *)invisibleStyle->copy());
    ik->BAUBuilding->setStyleGroup(SoBAUBuilding::CEILING, child, g);

    g = (SoGroup *)(sk->getPart("portalStyle", FALSE));
    if (g == NULL) sk->setPart("portalStyle", g = (SoGroup *)invisibleStyle->copy());
    ik->BAUBuilding->setStyleGroup(SoBAUBuilding::PORTAL, child, g);

    g = (SoGroup *)(sk->getPart("specialStyle", FALSE));
    if (g == NULL) sk->setPart("specialStyle", g = (SoGroup *)invisibleStyle->copy());
    ik->BAUBuilding->setStyleGroup(SoBAUBuilding::SPECIAL, child, g);

    g = (SoGroup *)(sk->getPart("allStyle", FALSE));
    if (g == NULL) sk->setPart("allStyle", g = (SoGroup *)invisibleStyle->copy());
    ik->BAUBuilding->setStyleGroup(SoBAUBuilding::ALL, child, g);
    invisibleStyle->unref();
}

void
SoBAUInstanceKit::roomListSensorCB(void *data, SoSensor *sensor)
{
//printf("*** SoBAUInstanceKit::roomListSensorCB\n");
    SoBAUInstanceKit *ik = (SoBAUInstanceKit *)data;
    if (ik == 0) return;

    SoMultiSwitch *ms = (SoMultiSwitch *)(ik->getPart("rooms", FALSE));
    if (ms == 0) return;

    if (ik->useRoomList.getValue() == TRUE)
    {
        ms->whichChildren.setValue(SO_SWITCH_NONE);
        int k = 0;
        for (int i=0; i<ik->roomList.getNum(); i++)
        {
//            printf("%s ", ik->roomList[i].getString()); 
            for (int j=0; j<ik->BAUBuilding->roomNames.getNum(); j++)
            {
                if (ik->roomList[i] == ik->BAUBuilding->roomNames[j])
                {
                    ms->whichChildren.set1Value(k++, j);
                    printf("[%i]=%i", k-1, j);
                }
            }
//            printf("\n");
        }
    }
    else
    {
        ms->whichChildren.setValue(SO_SWITCH_ALL);
    }
}

void
SoBAUInstanceKit::highlightedPortalSensorCB(void *data, SoSensor *sensor)
{
//printf("*** SoBAUInstanceKit::highlightedPortalSensorCB\n");
    SoBAUInstanceKit *ik = (SoBAUInstanceKit *)data;
    if (ik == 0) return;

    if (ik->useRoomList.getValue() == TRUE)
    {
        int pi = ik->BAUBuilding->getPortalIndex(ik->roomList[0], ik->highlightedPortal.getValue());
//printf("%i %s %s\n", pi, ik->roomList[0].getString(), ik->highlightedPortal.getValue().getString());
        if (pi >= 0)
        {
            SoContext *c = (SoContext *)(ik->getPart("context", FALSE));
            c->value.set1Value(1, pi);
        }
    }
}


//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

