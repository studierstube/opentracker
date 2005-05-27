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
/** SoDisplayMode.cxx contains the implementation of class SoDisplayMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna, Jan Prikryl
  *
  * $Id: SoDisplayMode.cxx 3713 2004-09-15 13:35:27Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "SoDisplayMode.h"

#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>

SO_NODE_SOURCE(SoDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization 

void
SoDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoDisplayMode, SoNode, "Node");
}

//----------------------------------------------------------------------------
// Abstract constructor. 

SoDisplayMode::SoDisplayMode()
    : viewer(NULL)
{
    // NIL
}

//----------------------------------------------------------------------------
// Returns TRUE. 

SbBool
SoDisplayMode::isStereoMode()
{
    return TRUE; // default is true
}

//----------------------------------------------------------------------------
// Sets viewer characteristics.

void 
SoDisplayMode::setStereoMode(SoStudierstubeViewer* aViewer)
{
    viewer = aViewer;
    viewer->setBorder(FALSE);
    if( viewer->isAutoClipping())
	    viewer->setAutoClipping(FALSE);
	viewer->setStereoOffset(0.0);
}

//----------------------------------------------------------------------------

void
SoDisplayMode::setMonoMode()
{
}

//----------------------------------------------------------------------------
//eof

/* ===========================================================================
    End of SoDisplayMode.cxx
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
