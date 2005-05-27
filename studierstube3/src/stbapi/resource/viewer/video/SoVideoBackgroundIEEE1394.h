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
 * header file of the IEEE1394 Linux video background class.
 *
 * @author Alexander Bornik
 *
 * $Id: SoVideoBackgroundIEEE1394.h 4195 2005-05-06 06:50:57Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOVIDEOBACKGROUNDIEEE1394_H_
#define _SOVIDEOBACKGROUNDIEEE1394_H_

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/stbapi.h>

#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include "SoVideoBackground.h"


#ifdef USE_IEEE1394

#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h> 
#include <pthread.h> 

#include <sys/ioctl.h>
#include <sys/mman.h>

#include <libraw1394/raw1394.h>
#include <libdv/dv1394.h>

#include <libdv/dv.h>
#include <libdv/dv_types.h>

#include <string>
#include <cassert>
#include <iostream>
#include <deque>

#define FRAME_MAX_WIDTH 720
#define FRAME_MAX_HEIGHT 576

typedef struct Pack 
{
    /// the five bytes of a packet
unsigned char data[5];
} Pack;

typedef struct TimeCode
{
   int hour;
   int min;
   int sec;
   int frame;
} TimeCode;


typedef struct AudioInfo
{
   int frames;
   int frequency;
   int samples;
   int channels;
   int quantization;
} AudioInfo;


class VideoInfo
{
public:
   int width;
   int height;
   bool isPAL;
   TimeCode timeCode;
   struct tm	recDate;

   VideoInfo();
};


class Frame
{
public:
   /// enough space to hold a PAL frame
   unsigned char data[144000];
   /// the number of bytes written to the frame
   int bytesInFrame;

   dv_decoder_t *decoder;

public:
   Frame();
   ~Frame();

   bool GetSSYBPack(int packNum, Pack &pack) const;
   bool GetVAUXPack(int packNum, Pack &pack) const;
   bool GetAAUXPack(int packNum, Pack &pack) const;
   bool GetTimeCode(TimeCode &timeCode) const;
   bool GetRecordingDate(struct tm &recDate) const;
   std::string GetRecordingDate(void) const;
   bool GetAudioInfo(AudioInfo &info) const;
   bool GetVideoInfo(VideoInfo &info) const;
   int GetFrameSize(void) const;
   float GetFrameRate() const;
   bool IsPAL(void) const;
   bool IsNewRecording(void) const;
   bool IsComplete(void) const;
   int ExtractAudio(void *sound) const;
   void ExtractHeader(void);

   void SetPreferredQuality( );
   int ExtractAudio(int16_t **channels) const;
   int ExtractRGB(void *rgb);
   int ExtractPreviewRGB(void *rgb);
   int ExtractYUV(void *yuv);
   int ExtractPreviewYUV(void *yuv);
   bool IsWide(void) const;
   int GetWidth();
   int GetHeight();
   void SetRecordingDate( time_t *datetime, int frame );
   void SetTimeCode( int frame );
   void Deinterlace( void *image, int bpp );

private:
};

/**
 * VideoBackgroundIEEE1394: FireWire (IEEE1394) Linux Video-Background 
 * implementation.
 * Handles the video background for Fire Cams (Tested with Sony Camcorders).
 * this only works under Linux.
 *
 * @ingroup video
 * @ingroup core
 */
class STBAPI_API SoVideoBackgroundIEEE1394 : public SoVideoBackground
{
   SO_NODE_HEADER(SoVideoBackgroundIEEE1394);

  public:
   static void initClass();
   SoVideoBackgroundIEEE1394();
   virtual ~SoVideoBackgroundIEEE1394();

   virtual SbBool initVideoBackground(int width, int height);
   virtual void blitOverlay(int eye = STEREO_LEFT);
   virtual void setOverlaySize(int width, int height) {}

  protected:
   inline int getTexSize(int w, int h);
   void initTexture(int size);

  private:

   GLuint videoTexture;
   int workerID;
private:
    bool    g_buffer_underrun;
    volatile bool    g_reader_active;
    char    *g_dst_file_name;
    
    int           g_card;
    int           g_channel;
    std::string   g_dv1394;

    
    Frame *buffi;
    
    // texture buffer and texture pointer
    unsigned char* rgbbuffer;
    
    std::deque < Frame* > g_buffer_queue;
    std::deque < Frame* > g_output_queue;
    
    pthread_mutex_t g_mutex;
    pthread_t       g_thread;

    Frame	*g_current_frame;
    unsigned char *g_frame;
    static unsigned char *g_dv1394_map;
    
    void initCamera();
    Frame* GetNextFrame();
    void DoneWithFrame(Frame *frame);
    void capture();
    raw1394handle_t open_1394_driver();
    bool dv1394_loop_iterate(int handle);
    void close_1394_driver(raw1394handle_t handle);
    static void *read_frames(void *arg);

};
#else // dummy implementation
class STBAPI_API SoVideoBackgroundIEEE1394 : public SoVideoBackground
{
   SO_NODE_HEADER(SoVideoBackgroundIEEE1394);

  public:
   static void initClass();
   SoVideoBackgroundIEEE1394();
};
#endif //USE_IEEE1394

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_VIDEOBACKGROUNDIEEE1394_H_

/* ===========================================================================
    End of SoVideoBackGroundIEEE1394.h
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
