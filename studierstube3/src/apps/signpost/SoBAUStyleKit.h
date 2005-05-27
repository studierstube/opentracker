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
  * $Id: SoBAUStyleKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoBAUStyleKit_H_
#define _SoBAUStyleKit_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <Inventor/nodekits/SoBaseKit.h>
//#include <Inventor/nodes/SoSeparator.h>
//#include <Inventor/nodes/SoTransform.h>
//#include <Inventor/nodes/SoVertexProperty.h>
//#include <Inventor/engines/SoCompose.h>
//#include <Inventor/nodes/SoSphere.h>
//#include <Inventor/fields/SoSFString.h>
//#include <stbapi/workspace/SoStyleKit.h>
//#include <stbapi/window3d/SoWindowKit.h>
//#include <stbapi/util/ivio.h>
//#include <stbapi/misc/ivmacros.h>
//#include <stbapi/widgets/So3DButton.h>
//#include <stbapi/widgets/So3DSlider.h>
//#include <stbapi/context/SoContext.h>
//#include <stbapi/context/SoContextSwitch.h>
//#include <Inventor/fields/SoSFUInt32.h>

#include "signpost.h"
#include "SoBAURoom.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * @ingroup signpost
 */
class SIGNPOST_API SoBAUStyleKit : public SoBaseKit
{
    SO_KIT_HEADER(SoBAUStyleKit);
	SO_KIT_CATALOG_ENTRY_HEADER(styleSwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(allStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(wallStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(floorStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(ceilingStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(portalStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(specialStyle);

public:
    enum styleType 
    { 
        WALL = SoBAURoom::WALL,
        FLOOR = SoBAURoom::FLOOR,
        CEILING = SoBAURoom::CEILING,
        PORTAL = SoBAURoom::PORTAL,
        SPECIAL = SoBAURoom::SPECIAL
    };

    static void initClass();
    SoBAUStyleKit();
    ~SoBAUStyleKit();
}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

