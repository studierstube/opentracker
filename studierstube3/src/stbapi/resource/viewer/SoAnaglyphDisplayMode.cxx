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
/** SoAnaglyphDisplayMode.cxx contains the implementation of class 
  * SoAnaglyphDisplayMode
  *
  * @author Rainer Splechtna
  *
  * $Id: SoAnaglyphDisplayMode.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#endif

#include <cassert>
#include <GL/gl.h>

#include "SoAnaglyphDisplayMode.h"

SO_NODE_SOURCE(SoAnaglyphDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoAnaglyphDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoAnaglyphDisplayMode, SoDisplayMode, "SoDisplayMode");
}

//----------------------------------------------------------------------------
// The constructor inits the node.

SoAnaglyphDisplayMode::SoAnaglyphDisplayMode()
{
    SO_NODE_CONSTRUCTOR(SoAnaglyphDisplayMode);
    SO_KIT_ADD_FIELD(leftLensColor, (1,0,0)); // default red
    SO_KIT_ADD_FIELD(rightLensColor,(0,1,0)); // default green
}

//----------------------------------------------------------------------------

SoAnaglyphDisplayMode::~SoAnaglyphDisplayMode()
{
    // nil
}

//----------------------------------------------------------------------------
// Calls the inherited method from SoDisplayMode.

void 
SoAnaglyphDisplayMode::setStereoMode(SoStudierstubeViewer* aViewer)
{
//--delegate setting of window size
    SoDisplayMode::setStereoMode(aViewer);
}

//----------------------------------------------------------------------------
// Switches to left buffer (using a color mask)    

void
SoAnaglyphDisplayMode::selectBufferLeft()
{
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glColorMask((GLboolean)leftLensColor.getValue()[0],
                (GLboolean)leftLensColor.getValue()[1],
                (GLboolean)leftLensColor.getValue()[2],
                GL_TRUE);
}

//----------------------------------------------------------------------------
// Switches to right buffer (using a color mask)

void
SoAnaglyphDisplayMode::selectBufferRight()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask((GLboolean)rightLensColor.getValue()[0],
                (GLboolean)rightLensColor.getValue()[1],
                (GLboolean)rightLensColor.getValue()[2],
                GL_TRUE);
}

/* ===========================================================================
    End of SoAnaglyphDisplayMode.cxx
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
