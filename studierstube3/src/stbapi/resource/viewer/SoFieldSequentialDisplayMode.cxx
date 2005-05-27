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
/** SoFieldSequentialDisplayMode.cxx contains the implementation of class 
  * SoFieldSequentialDisplayMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna, Jan Prikryl
  *
  * $Id: SoFieldSequentialDisplayMode.cxx 3713 2004-09-15 13:35:27Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/errors/SoDebugError.h>

#include "SoFieldSequentialDisplayMode.h"

#ifndef WIN32
#ifdef _SGI_SOURCE
#  define INT8 dummy_INT8
#  define INT32 dummy_INT32
#  include <GL/glx.h>
#  undef INT8
#  undef INT32
#  include <X11/extensions/SGIStereo.h>
#  endif
#include <unistd.h>
#else
#include <windows.h>
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <GL/gl.h>

SO_NODE_SOURCE(SoFieldSequentialDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization

void
SoFieldSequentialDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoFieldSequentialDisplayMode, SoDisplayMode, "SoDisplayMode");
}

//-----------------------------------------------------------------------------
// The constructor inits the fields of the node. 

SoFieldSequentialDisplayMode::SoFieldSequentialDisplayMode()
{
    SO_NODE_CONSTRUCTOR(SoFieldSequentialDisplayMode);

    SO_NODE_ADD_FIELD(stereoMode, (QUAD_BUFFER));

    SO_NODE_DEFINE_ENUM_VALUE(StereoMode, QUAD_BUFFER);
    SO_NODE_DEFINE_ENUM_VALUE(StereoMode, SPLIT_STEREO);
    SO_NODE_SET_SF_ENUM_TYPE(stereoMode, StereoMode);
}

//-----------------------------------------------------------------------------

SoFieldSequentialDisplayMode::~SoFieldSequentialDisplayMode()
{
    //nil
}

//-----------------------------------------------------------------------------
// Calls the inherited method from SoDisplayMode and enables quad-buffering
  
void
SoFieldSequentialDisplayMode::setStereoMode(SoStudierstubeViewer* aViewer)
{
    //--delegate setting of viewer characteristics
    SoDisplayMode::setStereoMode(aViewer);

	if(stereoMode.getValue() == QUAD_BUFFER)
	{
		// FIXME: check for other toolkits how to set stereo mode ! 20021228 gerhard.
		aViewer->setQuadBufferStereo(TRUE);
	}
}

//-----------------------------------------------------------------------------
// Switches to left back buffer (or STEREO_BUFFER_LEFT).

void
SoFieldSequentialDisplayMode::selectBufferLeft()
{
    if(stereoMode.getValue() == QUAD_BUFFER)
    {
        glFlush();
        glDrawBuffer(GL_BACK_LEFT);
    }
    else if(stereoMode.getValue() == SPLIT_STEREO)
    {
#ifdef _SGI_SOURCE

        static Display* d = viewer->getWidget()->x11Display();
        XSGISetStereoBuffer(d, viewer->getWidget()->winId(), STEREO_BUFFER_LEFT);
        XSync(d, False);
#else
        SoDebugError::postWarning("SoFieldSequentialDisplayMode::selectBufferLeft", 
            "SPLIT_STEREO is only supported on SGI hardware !");
#endif
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glFlush();
}

//-----------------------------------------------------------------------------
// Switches to right back buffer (or STEREO_BUFFER_RIGHT).

void
SoFieldSequentialDisplayMode::selectBufferRight()
{
    if(stereoMode.getValue() == QUAD_BUFFER)
    {
        glFlush();
        glDrawBuffer(GL_BACK_RIGHT);
    }
    else if(stereoMode.getValue() == SPLIT_STEREO)
    {
#ifdef _SGI_SOURCE
        static Display* d = viewer->getWidget()->x11Display();
        XSGISetStereoBuffer(d, viewer->getWidget()->winId(), STEREO_BUFFER_RIGHT);
        XSync(d, False);
#endif
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glFlush();
}

/* ===========================================================================
    End of SoFieldSequentialDisplayMode.cxx
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
