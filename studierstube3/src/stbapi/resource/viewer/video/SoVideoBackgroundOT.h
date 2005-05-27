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
 * header file of the OpenTracker video background class.
 *
 * @author Jan Prikryl, Hannes Kaufmann, Gerhard Reitmayr, Tamer Fahmy
 *
 * $Id: SoVideoBackgroundOT.h 4183 2005-04-27 05:31:18Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOVIDEOBACKGROUNDOT_H_
#define _SOVIDEOBACKGROUNDOT_H_

//#define ARTOOLKITPLUS_IS_VIDEOSOURCE

#include <stbapi/stbapi.h>

#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#ifdef ARTOOLKITPLUS_IS_VIDEOSOURCE
#  include <input/ARToolKitPlusModule.h>
#else
#  include <input/ARToolKitModule.h>
#endif //#define ARTOOLKITPLUS_IS_VIDEOSOURCE

#include "SoVideoBackground.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoTimerSensor;
class SoSensor;

#ifdef USE_ARTOOLKIT
#  define USE_VIDEOBACKGROUNDOT
#endif

#if (defined(USE_ARTOOLKITPLUS) && defined(ARTOOLKITPLUS_FOR_STB3))
#  define USE_VIDEOBACKGROUNDOT
//   typedef int MemoryBufferHandle;
#endif


#ifdef USE_VIDEOBACKGROUNDOT

using namespace ot;

/**
 * SoVideoBackgroundOT: OpenTracker ARToolKitModule Videobackground implementation.
 * This can work in theory with any OS where ARToolKit is available. Depends on
 * ARToolKit support compiled into OpenTracker.
 *
 * @ingroup video
 * @ingroup core
 */
class STBAPI_API SoVideoBackgroundOT : public SoVideoBackground
{
   SO_NODE_HEADER(SoVideoBackgroundOT);

  public:
   static void initClass();
   SoVideoBackgroundOT();
   virtual ~SoVideoBackgroundOT();

   virtual SbBool initVideoBackground(int width, int height);
   virtual void blitOverlay(int eye = STEREO_LEFT);
   virtual void setOverlaySize(int width, int height);

  protected:

#ifdef ARTOOLKITPLUS_IS_VIDEOSOURCE
   ot::ARToolKitPlusModule* toolkit;
#else
   ot::ARToolKitModule* toolkit;
#endif

   MemoryBufferHandle handle[2];
   unsigned char* pixel_buffer;
   unsigned int video_texture_id[2];

   GLenum format[2];

   bool flip_h[2], flip_v[2];

   float t_u0[2], t_u1[2];
   float t_v0[2], t_v1[2];

   // texture should be large enough to store full-size
   // PAL video frames (720x576px)
   enum {TEXTURE_WIDTH = 1024, TEXTURE_HEIGHT = 1024};

#ifdef WIN32
   unsigned long sample_n[2];
#endif
   SoTimerSensor * sensor;
   static void sensorCB( void * data, SoSensor * sensor );
};
#else // USE_VIDEOBACKGROUNDOT
class STBAPI_API SoVideoBackgroundOT : public SoVideoBackground
{
   SO_NODE_HEADER(SoVideoBackgroundOT);

  public:
   static void initClass();
   SoVideoBackgroundOT();
};
#endif

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_SOVIDEOBACKGROUNDOT_H_
