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
/** headerfile for onscreen renderer utility
  *
  * @author Anton Fuhrmann, Jan Prikryl
  *
  * $Id: OnScreenRenderer.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  * @ingroup camera
 ** ======================================================================= */

#ifndef _ONSCREENRENDERER_H_
#define _ONSCREENRENDERER_H_

#include <stbapi/stbapi.h>

#include SOGUI_H
#include SOGUI_EXAMINERVIEWER_H

STBAPI_API extern void onScreenRenderScene(
    SoNode* sceneGraph, 
    SbViewportRegion oldViewportRegion,
    SoTexture2 *texture,
    int width, 
    int height,
    SbColor background = SbColor(0,0,0)
    );

/** 
  replaces SoOffscreenRenderer, since this uses software
  rendering and is dead slow.

  @param viewer current view of this viewer is rendered
      must be instanced & visible & bigger than desired texture size
  @param texture output texture node
  @param width horizontal size of output texture inpixel
  @param height vertical size of output texture inpixel

  @author Anton Fuhrmann
*/
STBAPI_API extern void onScreenRender(
    SoGuiViewer *viewer,
    SoTexture2 *texture, 
    int width, 
    int height
    );

#endif//_ONSCREENRENDERER_H_
