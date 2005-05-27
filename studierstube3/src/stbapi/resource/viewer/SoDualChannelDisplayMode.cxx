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
/** SoDualChannelDisplayMode.cxx contains the implementation of class SoDualChannelDisplayMode
  *
  * @author Rainer Splechtna, Jan Prikryl
  *
  * $Id: SoDualChannelDisplayMode.cxx 3713 2004-09-15 13:35:27Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "SoDualChannelDisplayMode.h"

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

#include <cassert>


SO_NODE_SOURCE(SoDualChannelDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization 

void
SoDualChannelDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoDualChannelDisplayMode, SoDisplayMode, "SoDisplayMode");
}

//----------------------------------------------------------------------------
// The constructor inits the field of the node 

SoDualChannelDisplayMode::SoDualChannelDisplayMode()
{
    SO_NODE_CONSTRUCTOR(SoDualChannelDisplayMode);

    SO_NODE_ADD_FIELD(splitMode, (VERTICAL));

    SO_NODE_DEFINE_ENUM_VALUE(SplitMode, VERTICAL);
    SO_NODE_DEFINE_ENUM_VALUE(SplitMode, HORIZONTAL);
    SO_NODE_SET_SF_ENUM_TYPE(splitMode, SplitMode);

}

//----------------------------------------------------------------------------

SoDualChannelDisplayMode::~SoDualChannelDisplayMode()
{
    // nil
}

//----------------------------------------------------------------------------
// Calls the inherited method from SoDisplayMode and calculates 
// the viewports used for the two channels.

void 
SoDualChannelDisplayMode::setStereoMode(SoStudierstubeViewer* aViewer)
{
    size = aViewer->getSize();
//--delegate setting of window size
    SoDisplayMode::setStereoMode(aViewer);	 
    setupViewports();
}

//----------------------------------------------------------------------------
// Switches to first viewport. 

void
SoDualChannelDisplayMode::selectBufferLeft()
{
    viewer->setViewportRegion(viewportChannelOne);
}

//----------------------------------------------------------------------------
// Switches to second viewport. 

void
SoDualChannelDisplayMode::selectBufferRight()
{
    viewer->setViewportRegion(viewportChannelTwo);
}

//----------------------------------------------------------------------------
// updates viewports based on size. 

void SoDualChannelDisplayMode::setupViewports(void)
{
    const char w = 0, h = 1;
    
    if (splitMode.getValue() == VERTICAL) {
        viewportChannelOne.setViewportPixels(0,0,
            size[w]/2, size[h]);
        viewportChannelTwo.setViewportPixels(size[w]/2, 0,
            size[w]/2, size[h]);
    }
    else {
        viewportChannelOne.setViewportPixels(0, 0,
            size[w], size[h]/2);
        viewportChannelTwo.setViewportPixels(0, size[h]/2,
            size[w], size[h]/2);
    }
}


/* ===========================================================================
    End of SoDualChannelDisplayMode.cxx
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
