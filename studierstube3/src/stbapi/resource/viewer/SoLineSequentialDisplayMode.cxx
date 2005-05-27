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
/** SoLineSequentialDisplayMode.cxx contains the implementation of class 
  * SoLineSequentialDisplayMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna, Anton Fuhrmann, Jan Prikryl
  *
  * $Id: SoLineSequentialDisplayMode.cxx 3714 2004-09-15 14:46:44Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "SoLineSequentialDisplayMode.h"

#ifndef WIN32
#  ifdef _SGI_SOURCE
#  define INT8 dummy_INT8
#  define INT32 dummy_INT32
#  include <GL/glx.h>
#  undef INT8
#  undef INT32
#  endif
#include <unistd.h>
#include <cstdlib>
#else
#include <windows.h>
#endif

#include <cassert>
#include <GL/gl.h>

SO_NODE_SOURCE(SoLineSequentialDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoLineSequentialDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoLineSequentialDisplayMode, SoDisplayMode, "SoDisplayMode");
}

//----------------------------------------------------------------------------
//mask data structures for line interleaved rendering

static GLubyte evenmask[] = 
{
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff
};

static GLubyte oddmask[] = 
{
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00
};

//----------------------------------------------------------------------------
// The constructor inits the field of the node.

SoLineSequentialDisplayMode::SoLineSequentialDisplayMode()
:
stencilBitMask(0x0), stencilInitialized(20)
{
    SO_NODE_CONSTRUCTOR(SoLineSequentialDisplayMode);
    
    SO_NODE_ADD_FIELD(useStencil, (FALSE));
}

//----------------------------------------------------------------------------

SoLineSequentialDisplayMode::~SoLineSequentialDisplayMode()
{
    // nil
}

//----------------------------------------------------------------------------
// Paints (inits) even/odd mask for stencil

void SoLineSequentialDisplayMode::paintStencilMask()
{    
    int stencilBits;
    glGetIntegerv(GL_STENCIL_BITS,&stencilBits);
    if (stencilBits == 0)
    {
        fprintf(stderr, "ERROR: stencil buffer not available -- exiting program\n");
        exit(-1);
    }
    else
        // use highest stencil bit plane
        stencilBitMask = 0x1 << (stencilBits - 1);
        
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0x0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
   
    glStencilFunc(GL_ALWAYS, stencilBitMask, stencilBitMask);
    glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);

    glPushMatrix();
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, height, 1,-1);
    glPushAttrib(GL_ALL_ATTRIB_BITS); 
// depending on position of window (y % 2), even or odd scanlines 
// are mapped to left eye 
// 
    int j;
    unsigned char *img = new unsigned char[width*height];
   
    for (j=0; j < height; j++)
    {
        if(j%2)
            memset(&img[j*width], stencilBitMask, width);
        else
            memset(&img[j*width], 0, width);
    }
    
    glRasterPos2i(0,0);
    glDrawPixels(width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, img);
    delete [] img;
    
    glFlush();
    
    glPopAttrib();
    
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
}

//----------------------------------------------------------------------------
// Calls the inherited method from SoDisplayMode and inits some paramters for 
// the stencil-buffering mode.

void 
SoLineSequentialDisplayMode::setStereoMode(SoStudierstubeViewer* aViewer)
{
    const char w = 0, h = 1;
    SbVec2s vSize = aViewer->getSize();

//--delegate setting of viewer characteristics
    SoDisplayMode::setStereoMode(aViewer);
    if(useStencil.getValue())
    {
	// FIXME: check for other toolkits how to set stereo mode ! 20021228 gerhard.
		aViewer->setStencilBuffer(TRUE);
        // we need ca. 20 frames delay until graphics are ready
        if( stencilInitialized > 0 ) // first time, wait for stencil buffer to appear
            stencilInitialized = 20;
        else
            stencilInitialized = 0;   // otherwise do it right away
        height=vSize[h];
        width=vSize[w];
    }
}

//----------------------------------------------------------------------------
// Switches to left buffer (using stipple mask or stencil pattern)    

void
SoLineSequentialDisplayMode::selectBufferLeft()
{
    if(useStencil.getValue())
    {
        if(stencilInitialized == 0) 
        {
            stencilInitialized = -1;
            paintStencilMask();
        }
        else if (stencilInitialized > 0) 
            stencilInitialized--;      

        if( stencilInitialized < 0 ){                
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, 0x0, stencilBitMask);
        }
    }
    else
    {
        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(evenmask);
    }    
}

//----------------------------------------------------------------------------
// Switches to right buffer (using stipple mask or stencil pattern)    

void
SoLineSequentialDisplayMode::selectBufferRight()
{
    if(useStencil.getValue())
    {
        if(stencilInitialized < 0)
        {
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, stencilBitMask, stencilBitMask);
        }
    }
    else
    {
        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(oddmask);
    }
}

/* ===========================================================================
    End of SoLineSequentialDisplayMode.cxx
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
