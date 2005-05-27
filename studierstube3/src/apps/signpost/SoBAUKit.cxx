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
  * $Id: SoBAUKit.cxx 4056 2004-11-26 10:43:19Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/util/ivio.h>
#include <stbapi/widgets/SoListBox.h>
#include <stbapi/tracker/TrackerOT.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <common/GroupGateModule.h>

#include "SoBAUKit.h"
#include "SoBAUBuilding.h"

SO_KIT_SOURCE(SoBAUKit);

//----------------------------------------------------------------------------

void
SoBAUKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoBAUKit, SoContextKit, "SoContextKit");
    SoBAUBuilding::initClass();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoBAUKit::SoBAUKit()
{
    SO_KIT_CONSTRUCTOR(SoBAUKit);

	SO_KIT_ADD_FIELD(bauFilename, (""));
	SO_KIT_ADD_FIELD(initRoom, (""));
	SO_KIT_ADD_FIELD(building, (NULL));

    SO_KIT_INIT_INSTANCE();

    contextCounter = 0;
    BAUBuilding = NULL;

    setupSensor = new SoOneShotSensor();
    setupSensor->setFunction(SoBAUKit::setupSensorCB);
    setupSensor->setData(this);

    initRoomSensor = new SoFieldSensor();
    initRoomSensor->setFunction(SoBAUKit::initRoomSensorCB);
    initRoomSensor->setData(this);

    setUpConnections( TRUE, TRUE );
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoBAUKit::~SoBAUKit()
{
    delete setupSensor;
    delete initRoomSensor;
}

//----------------------------------------------------------------------------

SoContextKit*
SoBAUKit::factory()
{
    return new SoBAUKit();
}

//----------------------------------------------------------------------------

SbBool 
SoBAUKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

        if( bauFilename.getValue() != "" )
        {
            // do your own special setup here !
            const char *filename = bauFilename.getValue().getString();
            printf("INFO: SoBAUKit: Loading Building IV-File: %s\n", filename);
            SoSeparator *sep = readFile(filename);
            building.setValue(sep);
        }
        if( building.getValue() != NULL )
        {
            // setupSensor->schedule();
            setup();
            initRoomSensor->attach(&initRoom);
        }
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
SoBAUKit::checkPipConnections(int uid, SoNode *pipSheetContents)
{
}

//----------------------------------------------------------------------------

void 
SoBAUKit::setup()
{
    printf("SoBAUKit: setup\n");
    SoSearchAction sa;
    sa.setInterest(SoSearchAction::FIRST);
    sa.setType(SoBAUBuilding::getClassTypeId(), TRUE);
    sa.setSearchingAll(TRUE);
    sa.apply(building.getValue());
    SoFullPath *fp = (SoFullPath *)(sa.getPath());
    if( fp != NULL )
    {
        BAUBuilding = (SoBAUBuilding *)(fp->getTail());
        SoListBox *lb = (SoListBox *)getByName("ROOMLISTBOX");
        if( lb != NULL && BAUBuilding != NULL )
            lb->values.connectFrom(&(BAUBuilding->roomNames));
    }
    else
    {
        SoDebugError::postWarning("SoBAUKit::setup","no building found in building scene graph.");
    }
}

void 
SoBAUKit::setupSensorCB(void *data, SoSensor *sensor)
{
    SoBAUKit *bk = (SoBAUKit *)data;
    bk->setup();
}

//----------------------------------------------------------------------------

void 
SoBAUKit::setActiveCell()
{
    int i = BAUBuilding->roomCells.find(initRoom.getValue());
    if (i < 0) return;
    ((GroupGateModule *)(Stb3DEventGenerator::getTracker()->getContext()->getModule("GroupGateConfig")))->activateGroupGate(BAUBuilding->buildingName.getValue().getString(), BAUBuilding->roomCells[i+1].getString());
printf("SoBAUKit: new active gate %s for room %s\n", BAUBuilding->roomCells[i+1].getString(), initRoom.getValue().getString());
}

void 
SoBAUKit::initRoomSensorCB(void *data, SoSensor *sensor)
{
    SoBAUKit *bk = (SoBAUKit *)data;
    bk->setActiveCell();
//    nk->calculatePath();
}

//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

