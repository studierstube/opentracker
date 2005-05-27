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
 * implementation of the FireWire Linux video background class.
 *
 * @author Alexander Bornik
 *
 * $Id: SoVideoBackgroundIEEE1394.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "SoVideoBackgroundIEEE1394.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>

SO_NODE_SOURCE(SoVideoBackgroundIEEE1394);

#ifdef USE_IEEE1394
unsigned char* SoVideoBackgroundIEEE1394::g_dv1394_map = NULL;
#endif

void
SoVideoBackgroundIEEE1394::initClass()
{ 
    SO_NODE_INIT_CLASS(SoVideoBackgroundIEEE1394, 
                       SoVideoBackground, 
                       "SoVideoBackground");
}

SoVideoBackgroundIEEE1394::SoVideoBackgroundIEEE1394()
{
    using namespace std;
    SO_NODE_CONSTRUCTOR(SoVideoBackgroundIEEE1394);
#ifdef USE_IEEE1394
    g_dv1394 = string("/dev/dv1394");
    buffi = NULL;
#endif
}

#ifdef USE_IEEE1394
void SoVideoBackgroundIEEE1394::initCamera() 
{
   g_card = 0;
   g_channel = 63;
   g_dst_file_name = NULL;
}

void SoVideoBackgroundIEEE1394::capture() 
{
    using namespace std;
    
    cerr << "grabbing ... " << endl;
    
    Frame *frame = NULL;
    
    g_buffer_underrun = false;
    
    for (int i = 0; i < 20; ++i) {
        frame = new Frame;
        g_buffer_queue.push_back(frame);
    }

    pthread_mutex_init(&g_mutex, NULL);
    g_reader_active = true;
    pthread_create(&g_thread, NULL, read_frames, this);
    
    while (g_reader_active == true) 
    {
        buffi = GetNextFrame();

        if (buffi == NULL)
            break;

        if (buffi->IsComplete()) 
        {
            break;
        } else 
        {
            DoneWithFrame(buffi);
        }
    }
}


void* SoVideoBackgroundIEEE1394::read_frames(void* arg)
{
    using namespace std;


    SoVideoBackgroundIEEE1394 *self = (SoVideoBackgroundIEEE1394*)(arg);

    arg = NULL;
    
    raw1394handle_t handle;
    sigset_t sigset;
    
    sigfillset(&sigset);
    pthread_sigmask(SIG_BLOCK, &sigset, 0);
    
    self->g_reader_active = true;
    self->g_current_frame = NULL;
    handle = NULL;
    
    handle = self->open_1394_driver();

    cerr << " read_frames starting ... " << endl;
    
    while (self->g_reader_active == true) 
    {        
        /* Poll the 1394 interface */
        
        if (self->g_dv1394 != "")
            self->g_reader_active = self->dv1394_loop_iterate( (int) handle);        
    }
    cerr << " read_frames done. " << endl;

    self->close_1394_driver( handle);
    
    /* if we are still using a frame, put it back */
    
    if (self->g_current_frame != NULL) 
    {
        pthread_mutex_lock(&(self->g_mutex));
        self->g_buffer_queue.push_back(self->g_current_frame);
        pthread_mutex_unlock(&(self->g_mutex));
    }

    return NULL;
}


raw1394handle_t SoVideoBackgroundIEEE1394::open_1394_driver()
{
    using namespace  std;
    cerr << "SoVideoBackgroundIEEE1394::open_1394_driver ... ";
		
    int dv1394_fd = -1;
    int n_frames = DV1394_MAX_FRAMES/4;
    
    struct dv1394_init init = 
        {
            DV1394_API_VERSION, g_channel, n_frames, DV1394_PAL, 0, 0, 0
        };
		
    dv1394_fd = open(g_dv1394.c_str(), O_RDWR);
    if(dv1394_fd == -1) {
        perror("dv1394 open");
        exit(EXIT_FAILURE);
    }
    
    if(ioctl(dv1394_fd, DV1394_INIT, &init)) {
        perror("dv1394 INIT ioctl");
        close(dv1394_fd);
        exit(EXIT_FAILURE);
    }
    
    g_dv1394_map = (unsigned char *) mmap( NULL, DV1394_PAL_FRAME_SIZE * n_frames,
                                           PROT_READ|PROT_WRITE, MAP_SHARED, dv1394_fd, 0);

    if(g_dv1394_map == MAP_FAILED) 
    {
        perror("mmap frame buffers");
        close(dv1394_fd);
        exit(EXIT_FAILURE);
    }
		
		
    if(ioctl(dv1394_fd, DV1394_START_RECEIVE, NULL)) {
        perror("dv1394 START_RECEIVE ioctl");
        close(dv1394_fd);
        exit(EXIT_FAILURE);
    }

    cerr << "done." << endl;    

    return (raw1394handle_t) dv1394_fd;

}

bool SoVideoBackgroundIEEE1394::dv1394_loop_iterate(int handle)
{
    struct dv1394_status dvst;
    int n_frames = DV1394_MAX_FRAMES/4;
    bool shouldread = true;
		
    if(ioctl(handle, DV1394_WAIT_FRAMES, n_frames - 1))
    {
        perror("error: ioctl WAIT_FRAMES");
        return false;
    }

    if (ioctl(handle, DV1394_GET_STATUS, &dvst))
    {
        perror("ioctl GET_STATUS");
        return false;
    }
	
    if (dvst.dropped_frames > 0)
    {
        fprintf( stderr, "dv1394 reported %d dropped frames. Stopping.\n", dvst.dropped_frames);
        return false;
    }
	
    for (unsigned int i = 0; i < dvst.n_clear_frames; i++)
    {
        pthread_mutex_lock(&g_mutex);
        if ((g_current_frame != NULL) && (shouldread))
        {
            g_output_queue.push_back(g_current_frame);
            g_current_frame = NULL;
            //printf("reader > out: buffer %d, output %d\n", g_buffer_queue.size(), g_output_queue.size());
            //fflush(stdout);
        }

        if (g_output_queue.size() > 1)
            shouldread = false;
        else
            shouldread = true;

        if ((g_buffer_queue.size() > 0) && (shouldread))
        {
            g_current_frame = g_buffer_queue.front();
            g_current_frame->bytesInFrame = 0;
            g_buffer_queue.pop_front();
            //printf("reader < buf: buffer %d, output %d\n",g_buffer_queue.size(), g_output_queue.size());
            //fflush(stdout);
        }
        else
            g_buffer_underrun = true;
       
        pthread_mutex_unlock(&g_mutex);

        if ( g_current_frame != NULL )
        {
            if (shouldread)
            {
                memcpy( g_current_frame->data, 
                        (g_dv1394_map + 
                         (dvst.first_clear_frame * DV1394_PAL_FRAME_SIZE)),
                        DV1394_PAL_FRAME_SIZE );
                g_current_frame->bytesInFrame = 
                    g_current_frame->GetFrameSize( );
            }
        }
       
       
        if(ioctl(handle, DV1394_RECEIVE_FRAMES, 1))
        {
            perror("error: ioctl RECEIVE_FRAMES");
            return false;
        }
         
        if (ioctl(handle, DV1394_GET_STATUS, &dvst))
        {
            perror("ioctl GET_STATUS");
            return false;
        }
         
        if (dvst.dropped_frames > 0)
        {
            fprintf( stderr, "dv1394 reported %d dropped frames. Stopping.\n", dvst.dropped_frames);
            return false;
        }
         
    }
    return true;
}

void SoVideoBackgroundIEEE1394::close_1394_driver(raw1394handle_t handle)
{
  if (g_dv1394_map != NULL)
    munmap(g_dv1394_map, DV1394_PAL_FRAME_SIZE * DV1394_MAX_FRAMES/4);
  
  close((int) handle);
}

Frame* SoVideoBackgroundIEEE1394::GetNextFrame() 
{
    using namespace std;

    Frame       *frame = NULL;
   
    pthread_mutex_lock(&g_mutex);
    
    if (g_output_queue.size() > 0) 
    {
        //cerr << g_output_queue.size() << endl;
        while (g_output_queue.size() > 1)
        {
            g_buffer_queue.push_back(g_output_queue[0]);
            
            g_output_queue.pop_front();
        }
        
        frame = g_output_queue[0];
        g_output_queue.pop_front();
    }
    
    pthread_mutex_unlock(&g_mutex);
    
    if (!g_reader_active)
            return NULL;
    
    if (frame != NULL)
        frame->ExtractHeader();
    
    return frame;
}


void SoVideoBackgroundIEEE1394::DoneWithFrame(Frame *frame) 
{
    using namespace std;
    //cerr << "d";
    pthread_mutex_lock(&g_mutex);
    g_buffer_queue.push_back(frame);
    pthread_mutex_unlock(&g_mutex);
}


SbBool
SoVideoBackgroundIEEE1394::initVideoBackground(int width, int height)
{
    using namespace std;
    cerr << "InitOverlay with size: " << width << "x" << height << endl;
    
    rgbbuffer = new unsigned char[FRAME_MAX_WIDTH*FRAME_MAX_HEIGHT*3];
    initTexture(getTexSize(720,576));
    
    initCamera();

    // initialize mulit-threading stuff
    capture();

    return TRUE;
}

SoVideoBackgroundIEEE1394::~SoVideoBackgroundIEEE1394()
{
    using namespace std;
   
    cerr << "endOverlay ..." << endl;

    if (rgbbuffer) delete [] rgbbuffer;

    glDeleteTextures(1, &videoTexture);
}

/**
 * Calculate minimal texture size
 */
int 
SoVideoBackgroundIEEE1394::getTexSize(int w, int h)
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
SoVideoBackgroundIEEE1394::initTexture(int size) 
{
    using namespace std;
    cout << "Init texture with size: " << size << "x" << size << endl;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &videoTexture);
    glBindTexture(GL_TEXTURE_2D, videoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    unsigned char* tmp = new unsigned char[size*size];
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, tmp);
    delete [] tmp;
}

/**
 * Draw texture fullscreen 
 */
void
SoVideoBackgroundIEEE1394::blitOverlay(int eye)
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
    
    int w = FRAME_MAX_WIDTH;
    int h = FRAME_MAX_HEIGHT;
    int size = getTexSize(w,h);
    

    
    buffi = GetNextFrame();

    if (buffi)
    {        
        if (buffi->IsComplete()) 
        {        
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            //       glOrtho(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5);
            
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, videoTexture);
            
            //cerr << buffi << endl;
            
            buffi->ExtractRGB(rgbbuffer);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
                            w, h, 
                            GL_RGB, GL_UNSIGNED_BYTE, rgbbuffer);
        }
        
        DoneWithFrame(buffi);
    }

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
}

Frame::Frame() : bytesInFrame(0)
{
   memset(data, 0, 144000);

   decoder = dv_decoder_new(0,0,0);
   decoder->quality = DV_QUALITY_COLOR | DV_QUALITY_AC_1;
   dv_set_audio_correction ( decoder, DV_AUDIO_CORRECT_NONE );
}


bool Frame::IsComplete(void) const
{
   return bytesInFrame == GetFrameSize();
}

void Frame::ExtractHeader(void)
{
   dv_parse_header(decoder, data);
   dv_parse_packs(decoder, data);
}

int Frame::ExtractRGB(void *rgb)
{
   unsigned char *pixels[3];
   int pitches[3];

   pixels[0] = (unsigned char*)rgb;
   pixels[1] = NULL;


   pixels[2] = NULL;

   pitches[0] = 720 * 3;
   pitches[1] = 0;
   pitches[2] = 0;

   dv_decode_full_frame(decoder, data, e_dv_color_rgb, pixels, pitches);
   return 0;
}

bool Frame::GetRecordingDate(struct tm &recDate) const
{
   return dv_get_recording_datetime_tm( decoder, (struct tm *) &recDate);
}


std::string Frame::GetRecordingDate(void) const
{
    using namespace std;
    string recDate;
    char s[64];
    if (dv_get_recording_datetime( decoder, s))
        recDate = s;
    else
        recDate = "0000-00-00 00:00:00";
    
    return recDate;
}

bool Frame::GetTimeCode(TimeCode &timeCode) const
{
   int timestamp[4];
	
   dv_get_timestamp_int( decoder, timestamp);
	
   timeCode.hour = timestamp[0];
   timeCode.min = timestamp[1];
   timeCode.sec = timestamp[2];
   timeCode.frame = timestamp[3];
   
   return true;
}

int Frame::GetFrameSize(void) const
{
   return IsPAL() ? 144000 : 120000;
}

bool Frame::IsPAL(void) const
{
   unsigned char dsf = data[3] & 0x80;
   bool pal = (dsf == 0) ? false : true;
   
   if (!pal) {
      pal = (dv_system_50_fields (decoder) == 1) ? true : pal;
   }

   return pal;
}
#endif 

/* ===========================================================================
    End of SoVideoBackGroundIEEE1394.cxx
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
