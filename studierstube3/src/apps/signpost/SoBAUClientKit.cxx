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
  * $Id: SoBAUClientKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

//#include <assert.h>
//#include <Inventor/nodes/SoSphere.h>
//#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
//#include <Inventor/actions/SoSearchAction.h>
//#include <stbapi/widgets/So3DButton.h>
//#include <stbapi/window3d/SoWindowKit.h>
//#include <Inventor/sensors/SoOneShotSensor.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
//#include <stbapi/util/ivio.h>
//#include <stbapi/misc/ivmacros.h>

#include "SoBAUKit.h"
#include "SoBAUClientKit.h"
//#include "SoBAUInstanceKit.h"
//#include "SoBAUBuilding.h"

SO_KIT_SOURCE(SoBAUClientKit);

//----------------------------------------------------------------------------

void
SoBAUClientKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoBAUClientKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoBAUClientKit::SoBAUClientKit()
{
    SO_KIT_CONSTRUCTOR(SoBAUClientKit);

	SO_KIT_ADD_FIELD(useRoomList, (FALSE));
	SO_KIT_ADD_FIELD(roomList, (""));
	SO_KIT_ADD_FIELD(highlightedPortal, (""));

    SO_KIT_ADD_CATALOG_ENTRY(instance,  SoBAUInstanceKit,    TRUE, this, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(style,  SoBAUStyleKit,    TRUE, this, \x0, TRUE);

    SO_KIT_INIT_INSTANCE();

    setupSensor = new SoOneShotSensor();
    setupSensor->setFunction(SoBAUClientKit::setupSensorCB);
    setupSensor->setData(this);
    setupSensor->schedule();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoBAUClientKit::~SoBAUClientKit()
{
    delete setupSensor;
}

//----------------------------------------------------------------------------

SoGroup * 
SoBAUClientKit::getStyle(int style)
{
    SoBAUInstanceKit *bi = (SoBAUInstanceKit *)getPart("instance", FALSE); 
    if (bi == 0) return 0;
    int child = bi->contextNum.getValue();
    SoGroup *g = 0;
    switch (style)
    {
        case FLOOR:   g = (SoGroup *)getPart("floorStyle",   TRUE); break;
        case WALL:    g = (SoGroup *)getPart("wallStyle",    TRUE); break;
        case CEILING: g = (SoGroup *)getPart("ceilingStyle", TRUE); break;
        case PORTAL:  g = (SoGroup *)getPart("portalStyle",  TRUE); break;
        case SPECIAL: g = (SoGroup *)getPart("specialStyle", TRUE); break;
    }
    return g;
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

void 
SoBAUClientKit::setup()
{
    SoContextManagerKit *cmk = SoContextManagerKit::getInstance();
    SoApplicationKit *ak = cmk->getAppByName("BAU");
    printf("SoBAUClientKit: setup %i\n", ak);
    if (ak == 0) return;
    SoBAUKit *bk = (SoBAUKit *)(ak->getContext());
    SoBAUInstanceKit *bi = bk->createBAUInstanceKit();
    if( bi == 0) 
    {
        printf("SoBAUClientKit: could not create BAU Instance !\n");
        return;
    }
    setPart("instance", bi);
    bi->setStyle((SoBAUStyleKit *)getPart("style", TRUE));
    bi->useRoomList.connectFrom(&useRoomList);
    bi->roomList.connectFrom(&roomList);
    bi->highlightedPortal.connectFrom(&highlightedPortal);
}

void 
SoBAUClientKit::setupSensorCB(void *data, SoSensor *sensor)
{
    SoBAUClientKit *bc = (SoBAUClientKit *)data;
    bc->setup();
}


//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

