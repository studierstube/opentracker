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
 * header file of the USB Linux video background class.
 *
 * @author Alexander Bornik, Tamer Fahmy
 *
 * $Id: SoVideoBackgroundUSB.h 4191 2005-05-06 06:47:55Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOVIDEOBACKGROUNDUSB_H_
#define _SOVIDEOBACKGROUNDUSB_H_

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include "SoVideoBackground.h"

#ifdef PC_USBCAM

#include <stbapi/stbapi.h>

#include <sys/types.h>
#include <linux/videodev.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include <stbapi/util/zpthread.h>


#undef TRACE_VIDEODEV_READ
#define TRACE_VIDEODEV_MEM

#include <GL/gl.h>

class VideoDevice
{
  private:
   bool validated;
   std::string NodeName, IntfName;
   int CamFD, Opened;
   bool UseSelect, HasFramerate;
   int Capturing;
   int Palette, FrameRate;
   int PalRGB;
   struct video_picture VPic;
   struct video_mbuf VMBuf;
   int Buffers, CurBuffer;

   int image_w, image_h;
   int max_w, max_h;
   int min_w, min_h;
   unsigned char *vid_io_buffer;        // The buffer used for I/O; this can be mmap()ed or malloc()ed
   unsigned char *rgb_vid_buffer;       // The buffer for rgb images; may point to vid_io_buffer
   std::vector<int> vid_io_offsets, rgb_vid_offsets;
   int vid_io_buffer_size, vid_io_image_size;

   void Init();
   void CleanUp();

   bool TryPalette(int pal, int depth);
   void SetPalette();
   void CalcVidIoSize();
   
   int MCapture(int buf);
   int MSync();

   void CreateImagesRGB();
   void ResetImagesRGB();
   
   void StartCapture();
   void StopCapture();

  public:
   VideoDevice(const std::string &node_name);
   ~VideoDevice();

   bool IsValid();

   int Open(int buffers = 1);
   void Close();
   int GetDescriptor() const;
   
   void EnableRGB(bool isOn);

   std::string GetNodeName() const;
   std::string GetIntfName() const;
   
   std::pair<int,int> GetMinSize() const;
   std::pair<int,int> GetSize();
   std::pair<int,int> GetMaxSize() const;
   bool  SetSize(int width, int height);
   bool  SetSize(const std::pair<int,int> &new_size);
   int   GetFramerate() const;
   bool  SetFramerate(int fps);
   int   GetMBuffers() const;
   int   GetBuffers() const;

   int   GetVideoInputs() const;
   int   GetCurrentVideoInput() const;
   bool  SelectVideoInput(int number);
   
   int GetBrightness() const;
   bool SetBrightness(int val);
   int GetContrast() const;
   bool SetContrast(int val);
   int GetHue() const;
   bool SetHue(int val);
   int GetColour() const;
   bool SetColour(int val);
   int GetWhiteness() const;
   bool SetWhiteness(int val);
   
   int ReadImage(const unsigned char* dst);
};

/**
 * VideoBackgroundUSB: USB Linux Videobackground implementation.
 * Handles the video background for USB cams.
 * this only works under Linux.
 *
 * @ingroup video
 * @ingroup core
 */
class STBAPI_API SoVideoBackgroundUSB : public SoVideoBackground
{
    SO_NODE_HEADER(SoVideoBackgroundUSB);
    
public:
    static void initClass();
    SoVideoBackgroundUSB();
    virtual ~SoVideoBackgroundUSB();
    
    virtual SbBool initVideoBackground(int width, int height);
    virtual void blitOverlay(int eye = STEREO_LEFT);
    virtual void setOverlaySize(int width, int height) {}
    
protected:
    inline int getTexSize(int w, int h);
    void initTexture(int size);
    
private:
    
    unsigned char* field;
    GLuint videoTexture;
    int workerID;
};
#else
class STBAPI_API SoVideoBackgroundUSB : public SoVideoBackground
{
   SO_NODE_HEADER(SoVideoBackgroundUSB);

  public:
   static void initClass();
   SoVideoBackgroundUSB();
};
#endif //PC_USBCAM

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_VIDEOBACKGROUNDUSB_H_

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
