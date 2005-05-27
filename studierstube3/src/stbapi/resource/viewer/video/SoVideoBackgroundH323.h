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
 * header file for video background functions with the OpenH323
 * netenabled videostream
 *
 * @author Werner Frieb, Istvan Barakonyi, Tamer Fahmy
 *
 * $Id: SoVideoBackgroundH323.h 4195 2005-05-06 06:50:57Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOVIDEOBACKGROUNDH323_H_
#define _SOVIDEOBACKGROUNDH323_H_

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#ifdef USE_H323
#include "H323Utils.h"
#include "SoVideoBackgroundOT.h"

/**
 * SoVideoBackgroundH323: a video background that gets fed by an H323 video stream
 */
class STBAPI_API SoVideoBackgroundH323 : public SoVideoBackgroundOT
{   
   SO_NODE_HEADER(SoVideoBackgroundH323);

  public:
   static void initClass();
   SoVideoBackgroundH323();
   virtual ~SoVideoBackgroundH323();

   virtual SbBool initVideoBackground(int width, int height);
   virtual void blitOverlay(int eye = STEREO_LEFT);
   virtual void setOverlaySize(int width, int height);

   SbBool isLocal() const;
   SbBool isValidIP() const;

   /** Specifies the ip address for the H323 video background mode */
   SoSFString ipAddress;

   /** Specifies whether to use the video channel or not */
   SoSFBool videoOn;

   /** Specifies whether to use the audio channel or not */
   SoSFBool audioOn;

  private:
   /** the main process for pwlib, needs be initialized first */
   static H323Utils_App* app;
   /** the instance of the endpoint */
   static H323Utils_EndPoint* endpoint;

   /** hardcoded video image size */
   enum {VIDX = 320, VIDY = 240};

   SbBool local;         // flag to indicate if we show the local or 
                         // remote video stream
};
#else // USE_H323

#include "SoVideoBackground.h"

class STBAPI_API SoVideoBackgroundH323 : public SoVideoBackground
{
   SO_NODE_HEADER(SoVideoBackgroundH323);

  public:
   static void initClass();
   SoVideoBackgroundH323();

   /** Specifies the ip address for the H323 video background mode */
   SoSFString ipAddress;
   
   /** Specifies whether to use the video channel or not */
   SoSFBool videoOn;
   
   /** Specifies whether to use the audio channel or not */
   SoSFBool audioOn;

};
#endif

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOVIDEOBACKGROUNDH323_H_
