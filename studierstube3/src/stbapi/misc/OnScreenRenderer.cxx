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
/** Implementation of onscreen renderer utility
  *
  * @author Anton Fuhrmann, Jan Prikryl
  *
  * $Id: OnScreenRenderer.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cstdlib>

#ifdef WIN32
#include <windows.h>
#endif

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <GL/gl.h>
#include "OnScreenRenderer.h"

#include SOGUI_H
#include SOGUI_EXAMINERVIEWER_H

void onScreenRender(
    SoGuiViewer *viewer,
    SoTexture2 *texture, 
    int width, 
    int height
    )
{
    onScreenRenderScene(viewer->getSceneGraph(),viewer->getViewportRegion(),
        texture, width, height);
}

void onScreenRenderScene(
    SoNode* sceneGraph, 
    SbViewportRegion oldViewportRegion,
    SoTexture2 *texture,
    int width,
    int height,
    SbColor background
    )
{
//--switch off stencil mask if there is one, so we can use the onscreenrenderer
//  with the Studierstube viewer even if using line interleaved stencil
//  ds2000-09-14
    GLboolean stencilMode;
    glGetBooleanv(GL_STENCIL_TEST,&stencilMode);
    if(stencilMode) glDisable(GL_STENCIL_TEST);
    SbVec2s size;

    size.setValue(width, height);

// clear region		
    glClearColor(background[0],background[1],background[2],0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    SbViewportRegion viewportRegion(size);
    SoGLRenderAction RA(viewportRegion);
    RA.apply(sceneGraph);

    void     * pixels    = NULL;
    static int pixelSize = 0;
    int        newSize   = width*height*3;
    if(pixelSize != newSize)
    {
        if(pixelSize != 0) free(pixels);
        pixels = malloc(newSize);
    }
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    texture->image.setValue(size, 3, (unsigned char *)pixels);
    texture->touch();

//--rerender picture & reset viewport size (does not work w/o rerender)
    RA.setViewportRegion(oldViewportRegion);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    RA.setViewportRegion(oldViewportRegion);
    RA.apply(sceneGraph);
//--switch on stencil again if using line interleaved stencil  ds2000-09-14
    if(stencilMode) glEnable(GL_STENCIL_TEST);
}

//eof
