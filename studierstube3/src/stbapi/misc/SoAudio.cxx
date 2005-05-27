/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Implementation of Audio node
  *
  * @author  Jan Prikryl
  *
  * $Id: SoAudio.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "stbapi/util/sound.h"
#include "SoAudio.h"

SO_NODE_SOURCE(SoAudio);

void
SoAudio::initClass()
{
   SO_NODE_INIT_CLASS(SoAudio, SoNode, "Node");
}

SoAudio::SoAudio()
{
   SO_NODE_CONSTRUCTOR(SoAudio);
   SO_NODE_ADD_FIELD(fileName,  (""));
   SO_NODE_ADD_FIELD(trigger, (FALSE));
   SO_NODE_ADD_FIELD(risingEdgeActive, (TRUE));

   isPlaying  = FALSE;
   testActive = FALSE;
}

SoAudio::~SoAudio()
{
}

void
SoAudio::GLRender(SoGLRenderAction *action)
{
   SoAudio::doAction(action);
}

void
SoAudio::callback(SoCallbackAction *action)
{
   //SoAudio::doAction(action);
}

void
SoAudio::doAction(SoAction *action)
{
    int rea    = risingEdgeActive.getValue();
    int active = rea ? trigger.getValue() : ! (trigger.getValue());

    if ( !testActive )
    {
        if ( rea || (!rea && trigger.getValue() == TRUE))
        testActive = TRUE;
    }

    if ( testActive && !isPlaying && active )
    {
        // printf("risingEdgeActive %d\n", risingEdgeActive.getValue());
        // printf("trigger %d\n", trigger.getValue());
        // printf("playing %s\n\n", fileName.getValue().getString());
        sound(fileName.getValue());

    }
    isPlaying = active;
}
