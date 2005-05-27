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
 * header file of the SGI video background class.
 *
 * @author Jan Prikryl, Hannes Kaufmann, Gerhard Reitmayr, Tamer Fahmy
 *
 * $Id: SoVideoBackgroundSGI.h 4193 2005-05-06 06:50:10Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOVIDEOBACKGROUNDSGI_H_
#define _SOVIDEOBACKGROUNDSGI_H_

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/stbapi.h>

#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include "SoVideoBackground.h"

#if defined(SGI_VIDEO) && defined(_SGI_SOURCE)
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <dmedia/vl.h>
#include <dmedia/vl_camera.h>

#include <stbapi/util/GLtools.h>

// #include "McastNet.h++"

#define INT8 dummy_INT8
#define INT32 dummy_INT32
#include <GL/glx.h>
#undef INT8
#undef INT32
#include <dmedia/vl.h>
#include <dmedia/vl_camera.h>

// #include "Camera.h++"

class VideoStuff
{
public:
  GLuint glTexName;
  GLubyte *glTexBuffer;
  GLint glTexHeight;
  GLint glTexWidth;
  
  enum { SYS_O2, SYS_OCTANE };

//  static Camera *camera;
  static int sysFlag;
  static VLServer vlServer;
  static int vlSourceNodeNum;
  static int vlNBuffers;

  VLNode vlSource;
  VLNode vlDrain;
  VLPath vlPath;
  VLBuffer vlBuffer;
  int vlCapSizeX, vlCapSizeY;

  void error(char *);
  void warn(char *);

  void cameraAutoGain(char flag);
  void createVideoPath(int sizeX, int sizeY, int num, int denom, int packing);
  ~VideoStuff();
};

/**
 * VideoBackgroundSGI: SGI Videobackground implementation.
 * Handles the SGI SDK vor Visual Workstation
 * this only works under Windows
 *
 * @ingroup video
 * @ingroup core
 */
class STBAPI_API SoVideoBackgroundSGI : public SoVideoBackground
{
   SO_NODE_HEADER(SoVideoBackground);

  public:
   static void initClass();
   SoVideoBackgroundSGI();
   virtual ~SoVideoBackgroundSGI();

   virtual void blitOverlay(int eye = STEREO_LEFT);
   virtual void setOverlaySize(int width, int height);
   virtual SbBool initVideoBackground(int width, int height);

  protected:
   virtual void updateTexture();

  private:
   VideoStuff olStuff;
};
#else //defined(SGI_VIDEO) && defined(_SGI_SOURCE)
class STBAPI_API SoVideoBackgroundSGI : public SoVideoBackground
{
   SO_NODE_HEADER(SoVideoBackgroundSGI);

  public:
   static void initClass();
   SoVideoBackgroundSGI();
};
#endif

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_SOVIDEOBACKGROUNDSGI_H_
