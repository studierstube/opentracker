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
/** SoMonoDisplayMode.cxx contains the implementation of class SoMonoDisplayMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna, Jan Prikryl
  *
  * $Id: SoMonoDisplayMode.cxx 3713 2004-09-15 13:35:27Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "SoMonoDisplayMode.h"

#ifndef WIN32
#  ifdef _SGI_SOURCE
#  define INT8 dummy_INT8
#  define INT32 dummy_INT32
#  include <GL/glx.h>
#  undef INT8
#  undef INT32
#  endif
#include <unistd.h>
#else
#include <windows.h>
#endif

#include <cstdlib>
#include <cerrno>
#include <climits>
#include <cassert>


SO_NODE_SOURCE(SoMonoDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization 

void
SoMonoDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoMonoDisplayMode, SoDisplayMode, "SoDisplayMode");
}


//-----------------------------------------------------------------------------
// Constructor, inits the node.

SoMonoDisplayMode::SoMonoDisplayMode()
{
    SO_NODE_CONSTRUCTOR(SoMonoDisplayMode);
}

//-----------------------------------------------------------------------------

SoMonoDisplayMode::~SoMonoDisplayMode()
{
    // nil
}

//-----------------------------------------------------------------------------
// Sets the window size, enables viewer decoration and sets viewer characteristics
// hk 2003-04-04: viewer decoration can now also be turned off in MonoDisplayMode

void
SoMonoDisplayMode::setStereoMode(SoStudierstubeViewer* aViewer)
{
    viewer = aViewer;
    if (!viewer->isPopupMenuEnabled())
        viewer->setPopupMenuEnabled(TRUE);
    if( viewer->isAutoClipping())
        viewer->setAutoClipping(FALSE);
}


//-----------------------------------------------------------------------------
// Returns FALSE, because this mode does not support stereo.

SbBool
SoMonoDisplayMode::isStereoMode()
{
    return FALSE;
}

//----------------------------------------------------------------------------

void
SoMonoDisplayMode::setMonoMode()
{
    // do nothing! - override inherited setmono operation
}

/* ===========================================================================
    End of SoMonoDisplayMode.cxx
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
