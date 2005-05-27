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
  * $Id: SoBAUOKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoBAUOKit_H_
#define _SoBAUOKit_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/widgets/SoButtonKit.h>

#include "signpost.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoMultiSwitch;

/**
 * The Augmentation Application (AUG)
 * @ingroup signpost
 */
class SIGNPOST_API SoBAUOKit : public SoContextKit
{
    SO_KIT_HEADER(SoBAUOKit);

    enum contentEnum { ROOM, PATH, ALL };
    enum renderStyleEnum { INVISIBLE, WIREFRAME, TRANSLUCENT, WIREFRAME_TRANSLUCENT, HIDDENLINE };

public:
    static void initClass();
    SoBAUOKit();
    ~SoBAUOKit();

    SoContextKit *factory();

    /// defines which part of the model to render. The options are only the current room, the rooms along the path, or all. Possible values are ROOM, PATH, ALL 
    SoSFEnum content;
    /// defines how to render the room geometry. This includes not at all, as wireframes, hiddenlines or translucent walls.Only walls, floors, etc. are rendered but no portals ! Possible values are INVISIBLE, WIREFRAME, HIDDENLINE, TRANSLUCENT 
    SoSFEnum roomStyle;
    /// defines how to render the portal geometry of selected portals !. This includes not at all, as wireframes, hiddenlines or translucent. Possible values are values INVISIBLE, WIREFRAME, HIDDENLINE, TRANSLUCENT 
    SoSFEnum portalStyle;
    /// input field for the chosen path 
    SoMFString path;
    /// input field for current room 
    SoSFString currentRoom;

    void checkPipMasterMode(SoNode *pipSheet, SbBool masterMode);

protected:
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

    SoFieldSensor *contentSensor;
    SoFieldSensor *roomStyleSensor;
    SoFieldSensor *portalStyleSensor;

    SoSwitch *zbufferDrawSwitch;
    SoMultiSwitch *roomStyleMultiSwitch;

    static void contentSensorCB(void *data, SoSensor *sensor);
    static void roomStyleSensorCB(void *data, SoSensor *sensor);
    static void portalStyleSensorCB(void *data, SoSensor *sensor);

    static void wireframeButtonCB(void *data, SoButtonKit *button);
    static void hiddenlineButtonCB(void *data, SoButtonKit *button);
}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

