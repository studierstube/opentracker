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
  * $Id: SoWIMKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoWIMKit_H_
#define _SoWIMKit_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/widgets/SoButtonKit.h>

#include "signpost.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The World-in-Miniature Application (WIM)
 * @ingroup signpost
 */
class SIGNPOST_API SoWIMKit : public SoContextKit
{
    SO_KIT_HEADER(SoWIMKit);

    enum contentEnum { ROOM, PATH, ALL };

public:
    static void initClass();
    SoWIMKit();
    ~SoWIMKit();

    SoContextKit *factory();

    /// a flag that denotes whether the wim is attached to pip or floats in front of user
    SoSFBool fixed;
    /// a flag that denotes whether the wim is aligned to the world or aligned to the user
    SoSFBool worldAligned;
    /// scale of model in wim 
    SoSFFloat scale;
    /// current center of the model in the wim 
    SoSFVec3f center;
    /// defines whether to draw the geometry in the WIM as wireframe or not
    SoSFBool wireframe; 
    /// defines whether to draw the geometry in the WIM as translucent or not
    SoSFBool translucent; 
    /// defines what to render in the WIM. This can be either only the current room, all rooms along the desired path or the whole model.Possible values are ROOM, PATH, ALL 
    SoSFEnum content; 
    /// defines whether to render any auxiliary geometry in the common scene graph
    SoSFBool auxGeometry; 
    /// defines whether the WIM is visible or not at all 
    SoSFBool visible; 
    /// sets the rooms that belong to the current path. This is an input field for other components to use 
    SoMFString path; 
    /// sets the current room of the user. This is an input field for other components to use
    SoSFString currentRoom; 


    void checkPipMasterMode(SoNode *pipSheet, SbBool masterMode);


protected:
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

    SoFieldSensor *contentSensor;

    static void contentSensorCB(void *data, SoSensor *sensor);
}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

