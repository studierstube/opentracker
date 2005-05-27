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
/** Implementation of GLTweak node
  *
  * @author  Jan Prikryl
  *
  * $Id: GLTweak.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#endif

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoCacheElement.h>

#include <GL/gl.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "GLTweak.h"

SO_NODE_SOURCE(GLTweak);

void
GLTweak::initClass()
{
    SO_NODE_INIT_CLASS(GLTweak, SoNode, "Node");
}

GLTweak::GLTweak()
{
   SO_NODE_CONSTRUCTOR(GLTweak);
   SO_NODE_ADD_FIELD(depthTest,  (TRUE));
   SO_NODE_ADD_FIELD(writeColor, (TRUE));
   SO_NODE_ADD_FIELD(writeAlpha, (TRUE));
   SO_NODE_ADD_FIELD(writeDepth, (TRUE));
   SO_NODE_ADD_FIELD(offsetFactor, (0.0));
   SO_NODE_ADD_FIELD(offsetUnits, (0.0));
}

GLTweak::~GLTweak()
{
}

void
GLTweak::GLRender(SoGLRenderAction *action)
{
    GLTweak::doAction(action);
}

void
GLTweak::callback(SoCallbackAction *action)
{
    GLTweak::doAction(action);
}

void
GLTweak::doAction(SoAction *action)
{
    GLboolean writeColorUpdate = FALSE;
    GLboolean writeAlphaUpdate = FALSE;
    GLboolean writeColorValue = FALSE;
    GLboolean writeAlphaValue = FALSE;
 
	SoCacheElement::invalidate(action->getState());
    // fprintf(stderr, "GLTweak::doAction()\n");

    // Z-buffer depth test on/off
        
    if (! depthTest.isDefault())
    {
        // fprintf(stderr,"  zbuffer set\n");
        if (depthTest.getValue() == TRUE)
        {
            glEnable(GL_DEPTH_TEST);
            // fprintf(stderr,"    depth test on\n");
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
            // fprintf(stderr,"    depth test off\n");
        }
    }

    // Z-buffer writes on/off

    if (! writeDepth.isDefault())
    {
        // fprintf(stderr,"  write depth called\n");
        if (writeDepth.getValue() == TRUE)
        {
            glDepthMask(GL_TRUE);
            // fprintf(stderr,"    write depth on\n");
        }
        else
        {
            glDepthMask(GL_FALSE);
            // fprintf(stderr,"    write depth off\n");
        }
    }

    // Enable/disable writing colours and alpha channel values into 
    // the framebuffer
        
    if (! writeColor.isDefault())
    {
        writeColorUpdate = TRUE;
        writeColorValue  = (GLboolean) writeColor.getValue();
    }

    if (! writeAlpha.isDefault())
    {
        writeAlphaUpdate = TRUE;
        writeAlphaValue  = (GLboolean) writeAlpha.getValue();
    }

    if (writeColorUpdate && writeAlphaUpdate)
    {
        glColorMask(writeColorValue, writeColorValue, writeColorValue,
                    writeAlphaValue);
    }
    else
    {
        if (writeColorUpdate || writeAlphaUpdate)
        {
            // We have to update either alpha of colour, but not both.

            GLboolean mask[4];

            glGetBooleanv(GL_COLOR_WRITEMASK, mask);
            if (writeColorUpdate)
            {
                glColorMask(writeColorValue, writeColorValue, writeColorValue,
                            mask[3]);
            }
            else
            {
                glColorMask(mask[1], mask[2], mask[3], writeAlphaValue);
            }
        }
    }

	glPolygonOffset( offsetFactor.getValue(), offsetUnits.getValue());
	if(offsetFactor.isDefault() && offsetUnits.isDefault())
	{
		glDisable(GL_POLYGON_OFFSET_FILL); 
	}
	else
	{
		glEnable(GL_POLYGON_OFFSET_FILL); 
	}
}

/* ===========================================================================
   End of GLTweak.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
