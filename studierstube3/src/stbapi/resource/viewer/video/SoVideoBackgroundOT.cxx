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
 * implementation of the OpenTracker video background class.
 *
 * @author Jan Prikryl, Hannes Kaufmann, Gerhard Reitmayr, Tamer Fahmy
 *
 * $Id: SoVideoBackgroundOT.cxx 4180 2005-04-18 16:54:57Z schall $
 * @file                                                                   */
/* ======================================================================= */

#include <cassert>

#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <stbapi/util/GLtools.h>
#include <stbapi/tracker/TrackerOT.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

#include <iostream>

#include "SoVideoBackgroundOT.h"

using namespace std;

SO_NODE_SOURCE(SoVideoBackgroundOT);

void
SoVideoBackgroundOT::initClass()
{ 
   SO_NODE_INIT_CLASS(SoVideoBackgroundOT, SoVideoBackground, "SoVideoBackground");
}

SoVideoBackgroundOT::SoVideoBackgroundOT()
{
   SO_NODE_CONSTRUCTOR(SoVideoBackgroundOT);
}

#ifdef USE_VIDEOBACKGROUNDOT


SoVideoBackgroundOT::~SoVideoBackgroundOT()
{
    if( sensor != NULL )
    {
        if(sensor->isScheduled())
            sensor->unschedule();
        delete sensor;
        sensor = NULL;
    }
    cout << "INFO: closed video link to ARToolkit module" << endl;
}

SbBool 
SoVideoBackgroundOT::initVideoBackground(int width, int height)
{
   Context * context = ((TrackerOT *)Stb3DEventGenerator::getTracker())->getContext();

#ifdef ARTOOLKITPLUS_IS_VIDEOSOURCE
   toolkit = (ot::ARToolKitPlusModule*)context->getModule("ARToolKitPlusConfig");
#else
   toolkit = (ot::ARToolKitModule*)context->getModule("ARToolKitConfig");
#endif

   assert(toolkit);
    
   if (!toolkit->isInitialized()) {
      return FALSE;
   }

   cout << "INFO: Init video link to ARToolkit module with "
        << width << " " << height << endl;
    
   if (width <= 0 || height <= 0) {
      cout << "INFO: no useful video image size !" << endl;
      return FALSE;
   }

   for(int eye=0; eye <= (toolkit->isStereo() ? STEREO_RIGHT:STEREO_LEFT); eye++)
   {

      flip_h[eye] = false; flip_v[eye] = true;
      format[eye] = toolkit->getImageFormat(eye);
      toolkit->getFlipping(&flip_h[eye], &flip_v[eye], eye);

      this->width = toolkit->getSizeX(STEREO_LEFT);
      this->height = toolkit->getSizeY(STEREO_LEFT);

      float u_rt = (float)toolkit->getSizeX(eye) / TEXTURE_WIDTH;
      float v_rt = (float)toolkit->getSizeY(eye) / TEXTURE_HEIGHT;
    
      t_u0[eye] = (flip_h[eye] ? u_rt : 0   );
      t_u1[eye] = (flip_h[eye] ?    0 : u_rt);
      t_v0[eye] = (flip_v[eye] ? v_rt : 0   );
      t_v1[eye] = (flip_v[eye] ?    0 : v_rt);

      glEnable(GL_TEXTURE_2D);

      long data_size = 4 * sizeof(GLubyte) * TEXTURE_WIDTH * TEXTURE_HEIGHT;
      GLubyte *data = (GLubyte*)malloc(data_size);
      memset(data, 0x40, data_size);
    
      glGenTextures(1, &video_texture_id[eye]);
      glBindTexture(GL_TEXTURE_2D, video_texture_id[eye]);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

#ifdef ARTOOLKITPLUS_IS_VIDEOSOURCE
      glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
                      format[eye], GL_UNSIGNED_BYTE, data);
#else
      glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
                      format[eye], GL_UNSIGNED_BYTE, data);
#endif

      glDisable(GL_TEXTURE_2D);

      free(data);
    
      #ifdef WIN32
         sample_n[eye] = (handle[eye].n - 1);
      #endif
   }

   sensor = new SoTimerSensor;
   sensor->setFunction( SoVideoBackgroundOT::sensorCB );
   sensor->setData( this );
   // FIXME :
   // 30 times a second is probably the maximal video rate we have currently
   // should probably made configurable someday. GR 261103
   sensor->setInterval( SbTime(0.03333333)); 
   sensor->schedule();

   return TRUE;
}

void
SoVideoBackgroundOT::blitOverlay(int eye)
{
   preGLCalls();
    
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   pixel_buffer = toolkit->lockFrame(&handle[eye], eye);

   glEnable(GL_TEXTURE_2D);

#ifdef WIN32
   if (pixel_buffer != NULL && sample_n[eye] != handle[eye].n) {
      sample_n[eye] = handle[eye].n;
#else
   if (pixel_buffer != NULL) {
#endif
      glBindTexture(GL_TEXTURE_2D, video_texture_id[eye]);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, toolkit->getSizeX(eye),
					  toolkit->getSizeY(eye), format[eye], GL_UNSIGNED_BYTE,
					  (void*)pixel_buffer);
   }
   
   glBindTexture(GL_TEXTURE_2D, video_texture_id[eye]);
   glBegin(GL_QUADS);
      // video backplate
      glTexCoord2f(t_u0[eye], t_v0[eye]); glVertex3f(-1.0f, -1.0f,  0.0f);
      glTexCoord2f(t_u1[eye], t_v0[eye]); glVertex3f( 1.0f, -1.0f,  0.0f);
      glTexCoord2f(t_u1[eye], t_v1[eye]); glVertex3f( 1.0f,  1.0f,  0.0f);
      glTexCoord2f(t_u0[eye], t_v1[eye]); glVertex3f(-1.0f,  1.0f,  0.0f);
   glEnd();

   glDisable(GL_TEXTURE_2D);

   toolkit->unlockFrame(handle[eye], eye);
   
   postGLCalls();
}
   
void
SoVideoBackgroundOT::setOverlaySize(int width, int height)
{
   this->width = toolkit->getSizeX(STEREO_LEFT);
   this->height = toolkit->getSizeY(STEREO_LEFT);

   for(int eye=0; eye <= (toolkit->isStereo() ? STEREO_RIGHT:STEREO_LEFT); eye++)
   {
       float u_rt = (float)toolkit->getSizeX(eye) / TEXTURE_WIDTH;
       float v_rt = (float)toolkit->getSizeY(eye) / TEXTURE_HEIGHT;
   
       t_u0[eye] = (flip_h[eye] ? u_rt : 0   );
       t_u1[eye] = (flip_h[eye] ?    0 : u_rt);
       t_v0[eye] = (flip_v[eye] ? v_rt : 0   );
       t_v1[eye] = (flip_v[eye] ?    0 : v_rt);
   }
}

void SoVideoBackgroundOT::sensorCB( void * data, SoSensor * sensor )
{
    assert(data!=NULL);
    SoVideoBackgroundOT * self = (SoVideoBackgroundOT *)data;
    // trigger rerender by touching this
    // under windows we check, if we actually have a new background image
#ifdef WIN32
    MemoryBufferHandle test;
    unsigned char * buffer = self->toolkit->lockFrame( &test, STEREO_LEFT );
    if( buffer != NULL && test.n != self->handle[STEREO_LEFT].n )
        self->touch();
    self->toolkit->unlockFrame( test, STEREO_LEFT );
#else
    self->touch();
#endif
}
#endif //USE_VIDEOBACKGROUNDOT
