/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
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
 * <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
 * Graz University of Technology, Inffeldgasse 16, A-8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/**
 * implementation of the FireWire Linux video background class based on 
 * libdc1394.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "SoVideoBackgroundDC1394.h"

#ifdef USE_DC1394

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>

#endif

SO_NODE_SOURCE(SoVideoBackgroundDC1394);

void
SoVideoBackgroundDC1394::initClass()
{ 
    SO_NODE_INIT_CLASS(SoVideoBackgroundDC1394, 
                       SoVideoBackground, 
                       "SoVideoBackground");
}

SoVideoBackgroundDC1394::SoVideoBackgroundDC1394()
{
    using namespace std;
    SO_NODE_CONSTRUCTOR(SoVideoBackgroundDC1394);

#ifdef USE_DC1394
    camera = NULL;
    capture = NULL;
    SO_NODE_ADD_FIELD(cameradevice, ("/dev/video1394/0"));    
    SO_NODE_ADD_FIELD(cameramode, (MODE_640x480_RGB));    
    SO_NODE_ADD_FIELD(cameraframerate, (FRAMERATE_15));    

    bufferswitch_mutex =NULL;

    havetodie = false;
    rendered = false;
    rgbbuffers[0] = NULL;
    rgbbuffers[1] = NULL;

    videoTexture = 0;
    actbuffer = 0;

    bufferswitch_mutex = new ACE_RW_Mutex;
    camthread_mutex = new ACE_Mutex;
    camthread_finished = new ACE_Condition<ACE_Mutex>((*camthread_mutex));


#endif
}

SoVideoBackgroundDC1394::~SoVideoBackgroundDC1394()
{
    using namespace std;

#ifdef USE_DC1394
    //cerr << "SoVideoBackgroundDC1394::~SoVideoBackgroundDC1394()" << endl;

    
    havetodie = true;

    cerr << " waiting for thread to finish ... ";

    camthread_finished->wait();
    
    cerr << "done." << endl;

    if (capture) delete capture;
    if (camera) delete camera;

    if (rgbbuffers[0]) delete [] rgbbuffers[0];
    if (rgbbuffers[1]) delete [] rgbbuffers[1];

    glDeleteTextures(1, &videoTexture);

    if (camthread_finished) delete camthread_finished;
    if (camthread_mutex) delete camthread_mutex;
    if (bufferswitch_mutex) delete bufferswitch_mutex;

#endif
}

#ifdef USE_DC1394
SbBool
SoVideoBackgroundDC1394::initVideoBackground(int width, int height)
{
    using namespace std;
    cerr << "InitOverlay with size: " << width << "x" << height << endl;
    
    rgbbuffers[0] = new unsigned char[640*480*3];
    rgbbuffers[1] = new unsigned char[640*480*3];
    
    initTexture(getTexSize(640,480));
    
    SbBool camsuccess = initCamera();

    if (!camsuccess) return FALSE;

    // initialize mulit-threading stuff

    this->open(0);

    return TRUE;
}

SbBool 
SoVideoBackgroundDC1394::initCamera()
{
    using namespace std;
    using namespace dc1394;
    
    try 
    {
        camera = new Camera(0,0);
        
        FeatureSet((*camera)).print();

        cam_params.dma_device_file = cameradevice.getValue().getString();
        cam_params.mode = cameramode.getValue();
        cam_params.frame_rate = cameraframerate.getValue();

        capture = new Capture((*camera), cam_params);

    }
    catch (const Error &e)
    {
        e.print();
        return FALSE;
    }
    
    return TRUE;
    
}

/**
 * Calculate minimal texture size
 */
int 
SoVideoBackgroundDC1394::getTexSize(int w, int h)
{
   int size=0;
   while (w > pow(2.f,size)) {size++;}
   if (pow(2.f,size) < h) {
      size++;
      while (h > pow(2.f,size)) {size++;}
   }
   size = (int)pow(2.f,size);
   return size;
}

/** 
 * Initialize Texture for video stream
 */
void 
SoVideoBackgroundDC1394::initTexture(int size) 
{
    using namespace std;
    cout << "Init texture with size: " << size << "x" << size << endl;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &videoTexture);
    cout << " texture ID: " << videoTexture << endl;

    glBindTexture(GL_TEXTURE_2D, videoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    unsigned char* tmp = new unsigned char[size*size*3];
    int i;

    for (i=0; i<size*size*3; i++) tmp[i] = 0;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, tmp);
    delete [] tmp;
}

/**
 * Draw texture fullscreen 
 */
void
SoVideoBackgroundDC1394::blitOverlay(int eye)
{
    using namespace std;

    preGLCalls();
    
    float m = mirrorMode.getValue() ? -1.0F : 1.0F;
    
    glPixelZoom (m, -1.0);
    glRasterPos2f (-m,1);
    
    glPixelTransferf (GL_RED_SCALE, 1.0);
    glPixelTransferf (GL_GREEN_SCALE, 1.0);
    glPixelTransferf (GL_BLUE_SCALE, 1.0);
    
    glPixelTransferi (GL_MAP_COLOR, GL_FALSE);
    
    glColor3f (1.0F, 0.1F, 1.0F);
    
    glOrtho(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, videoTexture);
   
    // get the whole image from camera
    
    int w = 640;
    int h = 480;
    int size = getTexSize(w,h);
    

    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //       glOrtho(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, videoTexture);
    

    // read in texture

    bufferswitch_mutex->acquire_read();

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
                    w, h, 
                    GL_RGB, GL_UNSIGNED_BYTE, rgbbuffers[actbuffer]);

    rendered = true;
    bufferswitch_mutex->release();


    // render polygon

    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(-0.50, 0.50);
    glTexCoord2f((float)w/(float)size, 0.0);
    glVertex2f(0.50, 0.50);
    glTexCoord2f((float)w/(float)size, (float)h/(float)size);
    glVertex2f(0.50, -0.50);
    glTexCoord2f(0.0, (float)h/(float)size);
    glVertex2f(-0.50, -0.50);
    glEnd();
    
    glFlush();
    glDisable(GL_TEXTURE_2D);
    
    postGLCalls();

    touch();
}

int 
SoVideoBackgroundDC1394::open(void*)
{
    using namespace std;
    
    //cerr << "SoVideoBackgroundDC1394::open()" << endl;

    return this->activate(THR_NEW_LWP);

}

int 
SoVideoBackgroundDC1394::close(u_long flags)
{
    using namespace std;
    //cerr << "SoVideoBackgroundDC1394::close()" << endl;
    
    camthread_finished->signal();

    return inherited::close(flags);
}

int 
SoVideoBackgroundDC1394::svc()
{
    using namespace std;

    camera->startIsoTransmission();

    while (!havetodie)
    {
        capture->dmaSingleCapture(); 
        
        bufferswitch_mutex->acquire_read();
        memcpy(rgbbuffers[(actbuffer+1)%2], capture->getBuffer(), 640*480*3);
        //cerr << "Pointer: " <<  << endl;
        capture->dmaDoneWithBuffer();
        
        //int sillycounter=0;
        while(!(bufferswitch_mutex->tryacquire_write_upgrade())) { /*++sillycounter;*/ };
        rendered = false;
        actbuffer = (actbuffer+1)%2;
        bufferswitch_mutex->release();

    }
   
    camera->stopIsoTransmission();
    //cerr << " Service loop finished!" << endl;

    return 0;
}

#endif // USE_DC1394


/* ===========================================================================
    End of SoVideoBackGroundDC1394.cxx
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
