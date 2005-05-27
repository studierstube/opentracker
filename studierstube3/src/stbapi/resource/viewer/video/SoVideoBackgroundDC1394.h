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
 * header file of the Linux video background class using libdc1394.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOVIDEOBACKGROUNDDC1394_H_
#define _SOVIDEOBACKGROUNDDC1394_H_

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include "SoVideoBackground.h"

#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Thread.h>
#include <ace/Synch.h>

#ifdef USE_DC1394

#include <string>
#include <cassert>
#include <iostream>
#include <deque>
#endif

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFString.h>

#ifdef USE_DC1394

#include <stbapi/stbapi.h>
#include "dc1394wrapper.h"


/**
 * VideoBackgroundDC1394: FireWire (IEEE1394) Linux Video-Background based 
 * on libdc1394
 * implementation.
 * Handles the video background for professional Fire Cams (not camcorders).
 * this only works under Linux.
 *
 * @ingroup video
 * @ingroup core
 */

class STBAPI_API SoVideoBackgroundDC1394 : public SoVideoBackground, ACE_Task<ACE_MT_SYNCH>
{
    SO_NODE_HEADER(SoVideoBackgroundDC1394);
    
    typedef ACE_Task<ACE_MT_SYNCH> inherited;

public:
    static void initClass();
    SoVideoBackgroundDC1394();
    virtual ~SoVideoBackgroundDC1394();
    
    virtual SbBool initVideoBackground(int width, int height);
    virtual SbBool initCamera();
    virtual void blitOverlay(int eye = STEREO_LEFT);
    virtual void setOverlaySize(int width, int height) {}
    
protected:
    inline int getTexSize(int w, int h);
    void initTexture(int size);
    virtual int open(void *);
    virtual int close(u_long);
    virtual int svc();
        
    
public:
    SoSFString cameradevice;
    SoSFInt32 cameramode;
    SoSFInt32 cameraframerate;

protected:
    ACE_RW_Mutex *bufferswitch_mutex;
    ACE_Mutex *camthread_mutex;
    ACE_Condition<ACE_Mutex> *camthread_finished;
    dc1394::Camera *camera;
    dc1394::Capture::DmaParams cam_params;
    dc1394::Capture::Capture *capture;
    GLuint videoTexture;
    unsigned char* rgbbuffers[2];
    int actbuffer;   
    bool havetodie;
    bool rendered;


private:
    
};
#else // dummy implementation
class STBAPI_API SoVideoBackgroundDC1394 : public SoVideoBackground, ACE_Task<ACE_MT_SYNCH>
{
    SO_NODE_HEADER(SoVideoBackgroundDC1394);
    
public:
    static void initClass();
    SoVideoBackgroundDC1394();
    ~SoVideoBackgroundDC1394();
};

#endif //USE_DC1394

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_VIDEOBACKGROUNDDC1394_H_

/* ===========================================================================
    End of SoVideoBackGroundDC1394.h
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
