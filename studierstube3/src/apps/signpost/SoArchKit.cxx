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
  * $Id: SoArchKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

//#include <assert.h>
//#include <Inventor/nodes/SoSphere.h>
//#include <Inventor/nodes/SoCube.h>
//#include <Inventor/nodes/SoDrawStyle.h>
//#include <Inventor/nodes/SoMaterial.h>
//#include <stbapi/resource/SoTrackedArtifactKit.h>
//#include <stbapi/window3d/SoWindowKit.h>
//#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/util/ivio.h>

#include "SoBAUClientKit.h"
#include "SoArchKit.h"

SO_KIT_SOURCE(SoArchKit);

//----------------------------------------------------------------------------

void
SoArchKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoArchKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoArchKit::SoArchKit()
{
    SO_KIT_CONSTRUCTOR(SoArchKit);

	SO_KIT_ADD_CATALOG_ENTRY(architecture, SoSeparator, FALSE, this, \x0, FALSE);

    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoArchKit::~SoArchKit()
{
}

//----------------------------------------------------------------------------

SoContextKit*
SoArchKit::factory()
{
    return new SoArchKit();
}

//----------------------------------------------------------------------------

SbBool 
SoArchKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

        // do your own special setup here !
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
SoArchKit::checkPipMasterMode(SoNode *pipSheet, SbBool masterMode)
{
    printf("WIMKit: checkPipMasterMode\n");
}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

