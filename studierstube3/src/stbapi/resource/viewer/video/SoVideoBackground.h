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
/**
 * header file of the SoVideoBackground node.
 *
 * @author Tamer Fahmy
 *
 * $Id: SoVideoBackground.h 4194 2005-05-06 06:50:24Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOVIDEOBACKGROUND_H_
#define _SOVIDEOBACKGROUND_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#endif

#include <stbapi/stbapi.h>

#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include <GL/gl.h>


#include <stbapi/util/GLtools.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoSubNode.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#define STEREO_LEFT  0
#define STEREO_RIGHT 1

/**
 * SoVideoBackground is the base class for all the video background implementations.
 *
 * The following video background implementations are available:
 *
 * @ingroup video
 * @ingroup core
 */
class STBAPI_API SoVideoBackground : public SoNode
{
   SO_NODE_HEADER(SoVideoBackground);

  public:
   static void initClass();
   SoVideoBackground();
   virtual ~SoVideoBackground() {}

   /** 
    * initializes the video overlay to a given window size.
    * @param width width of the window
    * @param height height of the window
    * @param mirrorMode mirror video image
    * @return TRUE if video background could be created.
    */
   virtual SbBool initVideoBackground(int width, int height) { return FALSE; }

   /**
    * blits the actual video image into the frame buffer. This needs to be
    * implemented to draw the actual video background. It is called for each
    * frame and each viewer.
    */
   virtual void blitOverlay(int eye = STEREO_LEFT) {}

   /**
    * resets the size of the overlay. needs to be called if the window
    * changes.
    * @param width width of the window
    * @param height height of the window
    */
   virtual void setOverlaySize(int width, int height) {}

   /**
    * Specifies if the video background should be mirrored
    */
   SoSFBool mirrorMode;

  protected:
   /**
    * prelude of GL handling stuff.
    */
   inline void preGLCalls()
   {
      checkGLErrors();
      
      glPushMatrix();
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      
      checkGLErrors ();
      
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      
      glDisable(GL_DEPTH_TEST);
      glDepthMask(GL_FALSE);
      glDisable(GL_LIGHTING);
      glDisable(GL_ALPHA_TEST);
      glDisable(GL_BLEND);
      
      checkGLErrors();
   }

   /**
    * sequel of GL handling stuff.
    */
   inline void postGLCalls()
   {
      glPopAttrib();
      
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      
      checkGLErrors();
   }

   int width, height;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_SOVIDEOBACKGROUND_H_

/* ===========================================================================
    End of SoVideoBackGround.h
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
