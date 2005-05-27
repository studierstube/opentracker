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
 * implementation for the video background functions with the
 * OpenH323 net-enabled videostream
 *
 * @author Werner Frieb, Istvan Barakonyi, Tamer Fahmy
 *
 * $Id: SoVideoBackgroundH323.cxx 3586 2004-08-05 11:15:04Z bara $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#include <windows.h>
#endif

#include "SoVideoBackgroundH323.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// Linker settings for Win32 by pragmas (need to modify Makefile manually under Linux)
#ifdef USE_H323
#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib,"ptlibd.lib")
#pragma comment(lib,"pwlibd.lib")
#pragma comment(lib,"openh323d.lib")
#else
#pragma comment(lib,"ptlib.lib")
#pragma comment(lib,"pwlib.lib")
#pragma comment(lib,"openh323.lib")
#endif // _DEBUG
#endif // WIN32
#endif // USE_H323

SO_NODE_SOURCE(SoVideoBackgroundH323);

void
SoVideoBackgroundH323::initClass()
{ 
#ifdef USE_H323
   SO_NODE_INIT_CLASS(SoVideoBackgroundH323, SoVideoBackgroundOT, "SoVideoBackgroundOT");
#else
   SO_NODE_INIT_CLASS(SoVideoBackgroundH323, SoVideoBackground, "SoVideoBackground");
#endif
}

SoVideoBackgroundH323::SoVideoBackgroundH323()
{
   SO_NODE_CONSTRUCTOR(SoVideoBackgroundH323);
   SO_NODE_ADD_FIELD(ipAddress, (""));
   SO_NODE_ADD_FIELD(videoOn, (TRUE));
   SO_NODE_ADD_FIELD(audioOn, (TRUE));
}

#ifdef USE_H323
// init static members
H323Utils_App *SoVideoBackgroundH323::app = NULL;
H323Utils_EndPoint *SoVideoBackgroundH323::endpoint = NULL;

SoVideoBackgroundH323::~SoVideoBackgroundH323()
{
   endpoint->ClearCall(endpoint->currentCallToken);
   delete endpoint;
   delete app;
}

SbBool 
SoVideoBackgroundH323::initVideoBackground(int width, int height)
{
   SoVideoBackgroundOT::initVideoBackground(width, height);
   if (!app) app = new H323Utils_App;
   if (!endpoint) endpoint = new H323Utils_EndPoint;

   local = isLocal();
   endpoint->Init(local, (char *)ipAddress.getValue().getString(),videoOn.getValue(),audioOn.getValue());

   // if local, then it will listen to incoming calls
   // if remote, it makes a call
   if (!local && isValidIP())
   { 
      H323Connection *connection = endpoint->MakeCall(endpoint->ipAddress,
                                                      endpoint->currentCallToken);
      if (!connection) {
         return FALSE;
      }
   }

   this->width  = toolkit->getSizeX();
   this->height = toolkit->getSizeY();
   
   flip_h[0] = !flip_h[0];

   if (!local) {
      glEnable(GL_TEXTURE_2D);
      
      long data_size = 3 * sizeof(GLubyte) * 1024 * 1024;
      GLubyte *data = (GLubyte*)malloc(data_size);
      memset(data, 0x40, data_size);
            
      glGenTextures(1, &(video_texture_id[STEREO_LEFT]));
      glBindTexture(GL_TEXTURE_2D, video_texture_id[STEREO_LEFT]);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);      
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#if WIN32
      glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 1024, 0,
                   GL_BGR_EXT,
                   GL_UNSIGNED_BYTE, data);
#else
      glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 1024, 0,
                   GL_RGB,
                   GL_UNSIGNED_BYTE, data);
#endif

      glDisable(GL_TEXTURE_2D);
      
      free(data);
   }

   float u_rt = (float)VIDX / TEXTURE_WIDTH;
   float v_rt = (float)VIDY / TEXTURE_HEIGHT;

   t_v0[0] = (flip_v[0] ? v_rt : 0   );
   t_v1[0] = (flip_v[0] ?    0 : v_rt);
   t_u0[0] = (flip_h[0] ? u_rt : 0   );
   t_u1[0] = (flip_h[0] ?    0 : u_rt);

   return TRUE;
}

SbBool
SoVideoBackgroundH323::isLocal() const
{
   const char *ip = ipAddress.getValue().getString();
   return !ip || !strlen(ip) || !strcmp(ip, "local");
}

SbBool
SoVideoBackgroundH323::isValidIP() const
{ 
   const char *ip = ipAddress.getValue().getString();
   return ip && strlen(ip) && strcmp(ip, "local") && strcmp(ip, "remote");
}

void
SoVideoBackgroundH323::blitOverlay(int eye)
{
   if (endpoint) {    
      preGLCalls();
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   
      if (local) {
         pixel_buffer = toolkit->lockFrame(&handle[STEREO_LEFT]);
         glEnable(GL_TEXTURE_2D);
                  
#ifdef WIN32
         if (pixel_buffer != NULL && sample_n[STEREO_LEFT] != handle[STEREO_LEFT].n) {
            sample_n[STEREO_LEFT] = handle[STEREO_LEFT].n;
#else
         if (pixel_buffer != NULL) {
#endif
            glBindTexture(GL_TEXTURE_2D, video_texture_id[STEREO_LEFT]);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                            format[0], GL_UNSIGNED_BYTE, (void*)pixel_buffer);

            glBindTexture(GL_TEXTURE_2D, video_texture_id[STEREO_LEFT]);

            glBegin(GL_QUADS);
            // video backplate
            glTexCoord2f(t_u0[0], t_v0[0]); glVertex3f(-1.0f, -1.0f,  0.0f);
            glTexCoord2f(t_u1[0], t_v0[0]); glVertex3f( 1.0f, -1.0f,  0.0f);
            glTexCoord2f(t_u1[0], t_v1[0]); glVertex3f( 1.0f,  1.0f,  0.0f);
            glTexCoord2f(t_u0[0], t_v1[0]); glVertex3f(-1.0f,  1.0f,  0.0f);
            glEnd();

            glDisable(GL_TEXTURE_2D);
         }
         toolkit->unlockFrame(handle[0]);
      } 
      else { // handle the remote part
         H323Utils_VideoBuffer* videoBuffer = &endpoint->remoteVideoBuffer;
         videoBuffer->Lock();

         if (videoBuffer->data) {          
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, video_texture_id[0]);

#if WIN32
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            endpoint->videoWidth, endpoint->videoHeight,
                            GL_BGR_EXT,
                            GL_UNSIGNED_BYTE, (void*)videoBuffer->data);
#else
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            endpoint->videoWidth, endpoint->videoHeight,
                            GL_RGB,
                            GL_UNSIGNED_BYTE, (void*)videoBuffer->data);
#endif
       
            glBindTexture(GL_TEXTURE_2D, video_texture_id[STEREO_LEFT]);
            glBegin(GL_QUADS);
            // video backplate
            glTexCoord2f(t_u0[0], t_v0[0]); glVertex3f(-1.0f, -1.0f,  0.0f);
            glTexCoord2f(t_u1[0], t_v0[0]); glVertex3f( 1.0f, -1.0f,  0.0f);
            glTexCoord2f(t_u1[0], t_v1[0]); glVertex3f( 1.0f,  1.0f,  0.0f);
            glTexCoord2f(t_u0[0], t_v1[0]); glVertex3f(-1.0f,  1.0f,  0.0f);
            glEnd();
      
            glDisable(GL_TEXTURE_2D);
         }

         videoBuffer->Unlock();
      }

      postGLCalls();
   }
}

void 
SoVideoBackgroundH323::setOverlaySize(int width, int height) 
{
    if (local) {
       SoVideoBackgroundOT::setOverlaySize(width, height);
    }
    else {
       this->width = toolkit->getSizeX(); this->height = toolkit->getSizeY();

       float u_rt = (float)VIDX / TEXTURE_WIDTH;
       float v_rt = (float)VIDY / TEXTURE_HEIGHT;
      
       t_u0[0] = (flip_h[0] ? u_rt : 0   );
       t_u1[0] = (flip_h[0] ?    0 : u_rt);
       t_v0[0] = (flip_v[0] ? v_rt : 0   );
       t_v1[0] = (flip_v[0] ?    0 : v_rt);      
    }
}
#endif //USE_H323
