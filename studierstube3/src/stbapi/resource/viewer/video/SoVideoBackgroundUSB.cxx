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
/** implementation of the USB Linux video background class.
 *
 * @author Alexander Bornik, Tamer Fahmy
 *
 * $Id: SoVideoBackgroundUSB.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "SoVideoBackgroundUSB.h"

SO_NODE_SOURCE(SoVideoBackgroundUSB);

void
SoVideoBackgroundUSB::initClass()
{ 
   SO_NODE_INIT_CLASS(SoVideoBackgroundUSB, SoVideoBackground, "SoVideoBackground");
}

SoVideoBackgroundUSB::SoVideoBackgroundUSB()
{
   SO_NODE_CONSTRUCTOR(SoVideoBackgroundUSB);
}

#ifdef PC_USBCAM

#include <errno.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

#include <../extras/usbwebcam/pwc-ioctl.h>
#include <../extras/usbwebcam/ccvt.h>

using namespace std;

/**
   \class VideoDevice
   \brief A device wrapper for Video4Linux devices
  
   This class wraps itself around a Video4Linux device; at the moment it
   is primarely oriented at webcams, but grabber cards are supported
   as well. This device can be 'opened' multiple times, so more than one
   class can use this device (of course they all get the same picture).
  
   The class can return video images in RGB, YUV or both formats; it also
   has an optional buffer scheme for the images; this can prevent the
   otherwise deep copy that is needed when, for example, image diffs
   need to be calculated.

   The class will use mmap() when it is supported by the device driver. Also
   some device support select(); if not, a QTimer is used. Use the Framerate
   functions to set the framerate or timer.
*/

/**
   \fn VideoDevice::VideoDevice(const QString &node_name)
   \param node_name The /dev device name of the video device
  
   The constructor will do some basic checks to see if this is a valid device.
   If yes, \ref IsValid() will return TRUE, otherwise false.
*/  
VideoDevice::VideoDevice(const string &node_name)
{
   struct video_capability vcap;
   int i;
    
   Opened = 0;
   Buffers = 0;
   FrameRate = 10;
   PalRGB = 0;
   Capturing = 0;
   rgb_vid_buffer = vid_io_buffer = NULL;
   vid_io_buffer_size = vid_io_image_size = 0;
   NodeName = node_name;
   validated = false;
   memset(&VMBuf, 0, sizeof(VMBuf));
   max_w = 0;
   max_h = 0;
   min_w = 0;
   min_h = 0;
    
   // Do a small test
   CamFD = ::open(node_name.c_str(), O_RDONLY);
   if (CamFD >= 0) 
   {
      if (ioctl(CamFD, VIDIOCGCAP, &vcap) < 0) 
      {
         cerr << "VideoDevice::VideoDevice()" 
              << "could not query capabilities; is this really a " 
              << "video device?" << endl;
      }
      else 
      {
         validated = true;
         max_w = vcap.maxwidth;
         max_h = vcap.maxheight;
         min_w = vcap.minwidth;
         min_h = vcap.minheight;
         vcap.name[31] = '\0';
         IntfName = vcap.name;
      }
      ::close(CamFD);
   }
   else 
      if (errno == EBUSY)
         validated = true;

   CamFD = -1;
}

/**
   \fn VideoDevice::~VideoDevice()
  
   Destroys the object; also closes the associated file descriptor
*/
VideoDevice::~VideoDevice()
{
   if (Opened > 1)
      cerr << "Warning: VideoDevice `%s' was destroyed when " 
           << "it was in use more than once." << NodeName << endl;
   CleanUp();
   if (CamFD >= 0) 
   {
      close(CamFD);
   }

   ResetImagesRGB();
}

// private


/** 
    \brief Gather data from video device, like name, size, etc. Also initializes buffers.
  
    This function must be called after every first open.
*/
void
VideoDevice::Init()
{
   struct video_window vwin;
   int i;
    
   cerr << "VideoDevice::Init()" << endl;
   if (CamFD < 0) 
   {
      cerr << "VideoDevice::Init: device not openend." << endl;
      return;
   }
   if (Opened > 1) 
   {
      cerr << "VideoDevice::Init: device already opened." << endl;
      return;
   }
   
   if (ioctl(CamFD, VIDIOCGPICT, &VPic) < 0) 
   {
      cerr << "VideoDevice::Init: could not get picture parameters. (duh?)" << endl;
      return;
   }
   Palette = VPic.palette; // To start with

   HasFramerate = false;
   memset(&vwin, 0, sizeof(struct video_window));
   if (ioctl(CamFD, VIDIOCGWIN, &vwin) == 0) 
   {
#if defined(PWC_FPS_SHIFT)
      if ((vwin.flags & PWC_FPS_FRMASK) >> PWC_FPS_SHIFT) 
      {
         HasFramerate = true;
//qDebug("VideoDevice supports framerate setting [%x].", vwin.flags);
      }
#endif
   }
   CurBuffer = 0;

   GetSize(); // Get current image size
    
   /* See if device has mmap(); */
   VMBuf.size = 0;
   VMBuf.frames = 0;
   vid_io_buffer_size = 0;
   if (ioctl(CamFD, VIDIOCGMBUF, &VMBuf) == 0) 
   {
      cerr << "Using mmap(), VMBuf.size = " <<  VMBuf.size << endl;
      vid_io_buffer_size = VMBuf.size;
      if (Buffers > 0) 
      {
         // User specified a number of buffers; see if we can match that.
         if (VMBuf.frames < Buffers) 
         {
            cerr << "VideoDevice::Init(): there are more buffers requested than MBUF can provide. Limiting buffers." << endl;
            Buffers = VMBuf.frames;
         }
      }
      else // We grab everything we can.
      {
         Buffers = VMBuf.frames;
      }
   }
   else {
      VMBuf.size = 0; // Just to be sure....
      VMBuf.frames = 0;
   }
   
   /* See if we can actually mmap() the memory */
   if (VMBuf.size > 0) 
   {
      vid_io_buffer = (unsigned char *)mmap(NULL, vid_io_buffer_size, PROT_READ, MAP_SHARED, CamFD, 0);
      if (vid_io_buffer == (unsigned char *)-1) 
      {
         cerr << "VideoDevice::Init(): mmap() failed " << errno 
              << ". Falling back to non-mmap()ed mode." << endl;
         VMBuf.size = 0;
         vid_io_buffer = NULL;
      }
      else 
      {
         vid_io_offsets.resize(Buffers);
         for (i = 0; i < Buffers; i++)
            vid_io_offsets[i] = VMBuf.offsets[i];
      }
   }

   if (VMBuf.size == 0)
   {  
      // No mmap or failed: allocate a buffer
      cerr << "Allocating own buffer." << endl;
      if (Buffers < 0)
         Buffers = 4; // In case the user didn't specify a buffer size, we make one.
      vid_io_buffer_size = Buffers * max_w * max_h * 4;
      vid_io_buffer = new unsigned char[vid_io_buffer_size];
      vid_io_offsets.resize(Buffers);
      for (i = 0; i < Buffers; i++)
         vid_io_offsets[i] = i * max_w * max_h * 4;
   }
   if (vid_io_buffer == NULL) 
   {
      cerr << "VideoDevice::Init() Failed to mmap/allocate memory!" << endl;
   }
}

/** 
    \brief Cleans up mmap()ed stuff and buffers

    This function must be called after the last Close()
*/
void
VideoDevice::CleanUp()
{
   if (VMBuf.size > 0) {
      MSync(); // Just to make sure
      munmap(vid_io_buffer, vid_io_buffer_size);
      VMBuf.size = 0;
   }
   else
      delete [] vid_io_buffer;
   vid_io_buffer = NULL;
   vid_io_buffer_size = 0;
}

/**
   \fn bool VideoDevice::TryPalette(int pal, int depth)
   \brief Tries to set a VIDEO_PALETTE_* palette.
   \param pal One of the VIDEO_PALETTE_* palettes.
   \param depth visual depth (?) [Okay, CPIA driver, have it your way *GRRR*]
   \return \b true on success, \b false on failure
*/
bool
VideoDevice::TryPalette(int pal, int depth)
{
   VPic.palette = pal;
   VPic.depth = depth;
   if (ioctl(CamFD, VIDIOCSPICT, &VPic) < 0)
      return false;
   /* Sigh. It was to be expected. The OV511 and IBMCam don't pay attention to the palette field */
   if (ioctl(CamFD, VIDIOCGPICT, &VPic) < 0)
      return false;
   if (VPic.palette == pal) {
      Palette = pal;
      return true;
   }
   return false;
}

void
VideoDevice::SetPalette()
{
   /* Determine most preferable palette... With more and more 
      color conversion going into the apps, we must try quite a few
      palettes before we hit one that we like.
      In case we want both YUV and RGB output, we prefer the YUV output.
   */
   char *pal_names[17] = 
      { "", "grey", "hi240", "rgb565" ,"rgb24", "rgb32", 
        "rgb555", "yuv422", "yuyv" , "uyvy", "yuv420", 
        "yuv411", "raw", "yuv422p", "yuv411p", "yuv420p", 
        "yuv410p" 
      };

   Palette = 0;
   if (CamFD < 0)
      return;

   if (PalRGB) {
      TryPalette(VIDEO_PALETTE_RGB32, 32)   ||
         TryPalette(VIDEO_PALETTE_RGB24, 24)   ||
         TryPalette(VIDEO_PALETTE_YUV422, 16)  ||
         TryPalette(VIDEO_PALETTE_YUYV, 16)    ||
         TryPalette(VIDEO_PALETTE_YUV420P, 16) ||
         TryPalette(VIDEO_PALETTE_GREY, 8);
   }
   cerr <<"VideoDevice::SetPalette picked palette "<< Palette << " [" 
        << pal_names[Palette] << "]" << endl;
   CalcVidIoSize();
}   


void
VideoDevice::CalcVidIoSize()
{
   switch (Palette) {
      case VIDEO_PALETTE_GREY:    vid_io_image_size = image_w * image_h; break;
      case VIDEO_PALETTE_YUV420P: vid_io_image_size = image_w * image_h * 3 / 2; break;
      case VIDEO_PALETTE_YUV422:
      case VIDEO_PALETTE_YUYV:    vid_io_image_size = image_w * image_h * 2; break;
      case VIDEO_PALETTE_RGB24:   vid_io_image_size = image_w * image_h * 3; break;
      case VIDEO_PALETTE_RGB32:   vid_io_image_size = image_w * image_h * 4; break;
      default:                    vid_io_image_size = 0; break;
   }
}

/** 
    \brief Do ioctl(MCAPTURE), start grabbing frame in buffer
    \param buf The mmap buffer (will be set modulo total buffers)
  
    This function will start the capture of an image into buffer \b buf
    with the current width & height
*/
int
VideoDevice::MCapture(int buf)
{
   struct video_mmap vm;
   if (VMBuf.size == 0)
      return 0; // ignore call

   assert(buf >= 0 && buf <= Buffers);
    
   vm.frame = buf;
   vm.format = Palette;
   vm.width = image_w;
   vm.height = image_h;
#ifdef TRACE_VIDEODEV_READ
   cerr << "VideoDevice::MCapture(): buffer " 
        << buf <<  ", format " << Palette
        << ", size (" << image_w << "x" << image_h << endl;
    
#endif  
   if (ioctl(CamFD, VIDIOCMCAPTURE, &vm) < 0) 
   {
      perror("VideoDevice::MCapture() ioctl");
      return -errno;
   }
   return 0;
}

/**
   \brief Do ioctl(SYNC), releasing buffer
*/
int
VideoDevice::MSync()
{
   if (VMBuf.size == 0)
      return 0; // ignore call

#ifdef TRACE_VIDEODEV_READ
   cerr << "VideoDevice::MSync()   : buffer " <<  CurBuffer << endl;
#endif  
   if (ioctl(CamFD, VIDIOCSYNC, &CurBuffer) < 0) 
   {
      perror("VideoDevice::MSync() ioctl");
      return -errno;
   }
   return 0;
}

void
VideoDevice::CreateImagesRGB()
{
#ifdef TRACE_VIDEODEV_MEM
   cerr << "VideoDevice::CreateImagesRGB()" << endl;
#endif
   /* If we have picked RGB32 as our input format, we map directly into
      our image buffer.
   */
   //rgb_image_size = image_w * image_h * 4;
   if (Palette == VIDEO_PALETTE_RGB32) 
   {
#ifdef TRACE_VIDEODEV_MEM
      cerr << " using pre-allocated memory" << endl;
#endif
      rgb_vid_buffer = vid_io_buffer;
      rgb_vid_offsets = vid_io_offsets;
      //vid_image_size = rgb_image_size;
   }
   else 
   {
#ifdef TRACE_VIDEODEV_MEM
      cerr << " allocating space for RGB";   
#endif
      rgb_vid_buffer = new unsigned char[Buffers * image_w * image_h * 4];
      rgb_vid_offsets.resize(Buffers);
      for (int i = 0; i < Buffers; i++) 
         rgb_vid_offsets[i] = i * image_w * image_h * 4;
   }

   /*
     RGB.resize((uint)Buffers);
     for (int i = 0; i < Buffers; i++)
     RGB.insert(i, 
     new QImage(rgb_vid_buffer + rgb_vid_offsets[i], 
     image_w, image_h, 32, NULL, 0, 
     QImage::LittleEndian)
     );
   */
}

void
VideoDevice::ResetImagesRGB()
{
#ifdef TRACE_VIDEODEV_MEM
   cerr << "VideoDevice::ResetImagesRGB()" << endl;
#endif
   /*
     RGB.clear();
   */
   rgb_vid_offsets.resize(0);
   if (rgb_vid_buffer != 0 && rgb_vid_buffer != vid_io_buffer) {
#ifdef TRACE_VIDEODEV_MEM
      cerr  << " freeing memory" << endl;
#endif
      delete [] rgb_vid_buffer;
   }
   rgb_vid_buffer = NULL;
}



void
VideoDevice::StartCapture()
{
   if (Capturing == 0) 
   {
      cerr << "VideoDevice::StartCapture() go!" << endl;     
      MCapture(CurBuffer); // Start capture for mmap()
   }

   Capturing++;
}

void
VideoDevice::StopCapture()
{
   if (Capturing == 0) 
   {
      cerr << "Duh? StopCapture while not capturing?" << endl;
      return;
   }
   Capturing--;
   if (Capturing == 0) 
   {
      cerr << "VideoDevice::StopCapture() halt!" << endl;;
      MSync();
   }
}


// public

bool
VideoDevice::IsValid()
{
   return validated;
}

/**
   \brief Open the device, even multiple times
   \param bufs Number of image buffers; -1 use device maximum
   \return 0 upon success, otherwise \b -errno .
  
   This function increments the usage counter of the device; the first time
   this function is called the device is really opened and initialized; all
   subsequent calls will return 0. You will need as many calls to Close() as
   you placed to Open() to get the device really closed.
  
   Open() will also start the automatic loading of images. In case the device
   supports select() a QSocketNotifier is installed, otherwise a timer
   is used.
  
   For double buffering purposes, use a \b buf value > 1. The \b buf parameter
   can only be set at the first Open(); subsequent calls will have no
   effect on the number of buffers. When bufs < 0, the device will be 
   queried for the maximum number of buffers and that amount will be used.
*/
int
VideoDevice::Open(int bufs)
{
   if (Opened++) 
   {
#ifdef TRACE_VIDEODEV_OPEN   
      cerr << "VideoDevice::Open() again (count = " 
           << Opened << ")." << endl;
#endif     
      return 0;
   }
     
   if (CamFD >= 0) {
      cerr << "Warning: VideoDevice already opened ?!?!" << endl;
      return 0;
   }

   CamFD = ::open(NodeName.c_str(), O_RDONLY);
   if (CamFD < 0) 
   {
      Opened = 0;
      return -errno;
   }

   Buffers = bufs;
   Init();

   /* Determine if we can use select() on this cam; still only the 
      Philips cams seem to have this.
   */
   UseSelect = false;
   if (IntfName.find("Philips") == 0) 
   {
      cerr <<  "Using select() call." << endl;
      UseSelect = true;
   }
   return 0;
}

/**
   \fn void VideoDevice::Close()
   \brief Closes the device provisionally
  
   This function decrements the usage counter of the VideoDevice. If the
   counter reaches 0, the device is really closed. See also \ref Open().
*/
void
VideoDevice::Close()
{
   if (Opened) 
   {
      if (Opened == 1) 
      {
#ifdef TRACE_VIDEODEV_OPEN
         cerr << "VideoDevice::Close(): last close." << endl;
#endif       
         CleanUp();
         close(CamFD);
         CamFD = -1;
      }
      Opened--;
   }
}


/** 
    \fn int VideoDevice::GetDescriptor() const
    \return file descriptor.
  
    This functions returns the file descriptor for this device. If the device
    is not opened, -1 is returned.
*/
int
VideoDevice::GetDescriptor() const
{
   return CamFD;
}



/** 
    \fn void VideoDevice::EnableRGB(bool isOn)
    \brief Enable/disable retrieval of RGB image(s)
  
    This tells the object if RGB images are desired. This will help in
    selecting the proper PALETTE for the device. Both RGB and YUV images
    may be enabled. See also \ref EnableYUV.
  
    Multiple calls to EnableRGB(true) will require the same amount of calls
    to EnableRGB(false) to turn RGB retrieval completely off.
*/
void
VideoDevice::EnableRGB(bool isOn)
{
   cerr << "EnableRGB: ";
   if (isOn)
      cerr << '+';
   else
      cerr << '-';
   cerr << endl;

   if (isOn) 
      PalRGB++;
   else
      PalRGB--;
   if (PalRGB < 0)
      cerr << "Warning: VideoDevice PalRGB is negative?" << endl;
   if (PalRGB == 0)
      ResetImagesRGB();
   SetPalette();
   if (isOn && PalRGB == 1)
      CreateImagesRGB();
   if (isOn)
      StartCapture();
   else
      StopCapture();
}


/** 
    \brief Returns device inode name
  
    This function returns the name of the device inode, like /dev/video0,
    /dev/video1, etc.
*/
string
VideoDevice::GetNodeName() const
{
   return NodeName;
}

/**
   \brief Returns internal name of device.
  
   This function returns the name of the device through the V4L interface.
*/
string
VideoDevice::GetIntfName() const
{
   return IntfName;
}
   

/**
   \fn QSize VideoDevice::GetMinSize() const
   \brief Return the minimum image size this device supports.
   \return an Object of type QSize
  
   With this function the minium image size in pixels is retrieved. 
*/
pair<int,int>
VideoDevice::GetMinSize() const
{
   return make_pair(min_w, min_h);
}

/**
   \fn QSize VideoDevice::GetSize()
   \return An object of type QSize.
   \brief Return current size from the driver.

   Returns the current image size as reported by the device. Returns a 
   size of (0, 0) when the device is closed or an error occured.
*/  
pair<int,int>
VideoDevice::GetSize()
{
   struct video_window vwin;

   image_w = 0;
   image_h = 0;
   if (CamFD >= 0 && ioctl(CamFD, VIDIOCGWIN, &vwin) == 0) {
      image_w = vwin.width;
      image_h = vwin.height;
   }

//qDebug("VideoDevice::GetSize() returns %dx%d", image_w, image_h);
//   yuv_image_size = image_w * image_h * 2; // for yuyv; yuv420p takes 1.5
//   rgb_image_size = image_w * image_h * 4;
   return make_pair(image_w, image_h);
}

/**
   \fn QSize VideoDevice::GetMaxSize() const
   \brief Return the maximum image size this device supports.
   \return an Object of type QSize
  
   With this function the maximum image size in pixels is retrieved. See
   also \ref GetMinSize and \ref SetSize
  
   Not all sizes between the minimum and maximum size may be allowed by the
   device; however, there is currently no way to retrieve a list of possible
   sizes. It's safest to stick to CIF (352x288) and SIF (320x240) formats
   and subsizes hereof, and VGA. Also it's wise to use to multiples of
   8 in both directions. \ref SetSize will return false when the size
   was rejected by the driver.
*/
pair<int,int>
VideoDevice::GetMaxSize() const
{
   return make_pair(max_w, max_h);
}

/**
   \fn bool VideoDevice::SetSize(int width, int height)
   \brief Set a new image size.
   \return false when the call failed, true when the new size was accepted by the device.

   This function will attempt to set a new image size; not all sizes between
   the minimum and maximum size may be allowed by the device; however, there
   is currently no way to retrieve a list of possible sizes. It is safest to
   stick to CIF (352x288) and SIF (320x240) formats and subsizes hereof, and
   VGA. Also it's wise to use to multiples of 8 in both directions. SetSize
   will return false when the size was rejected by the driver.
*/  
bool
VideoDevice::SetSize(int width, int height)
{
   struct video_window vwin;

   printf("VideoDevice::SetSize(%d, %d)\n", width, height);
   if (CamFD < 0 || width > max_w || height > max_h)
      return false;
   if (ioctl(CamFD, VIDIOCGWIN, &vwin) < 0) 
   {
      perror("GWIN: ");
      return false;
   }
   MSync(); // Drop current frame if we're mmap()ing

   memset(&vwin, 0, sizeof(vwin));
   vwin.width = width;
   vwin.height = height;
   vwin.clipcount = 0;
   if (ioctl(CamFD, VIDIOCSWIN, &vwin) < 0) {
      perror("SWIN");
   }
   else 
   {
      /* Read back; it might be the hardware decides a bit differently 
         (for example, multiples of 8)
      */
      GetSize();
      if (image_w == 0 && image_h == 0) /* woops */
         return false;
   }

   CalcVidIoSize();
   // Reset images
   if (PalRGB) 
   {
      ResetImagesRGB();
      CreateImagesRGB();
   }
   MCapture(CurBuffer);

   return true;
}

/**
   \override
*/
bool
VideoDevice::SetSize(const pair<int,int> &new_size)
{
   return SetSize(new_size.first, new_size.second);
}


/**
   \brief Returns the current framerate.
   \return The framerate in frames per second.
  
   This applies to some webcams that allow setting of a framerate. In 
   case of a device that does not support select() we use the framerate
   to set the timer. By default the framerate is set to 10.
  
   Returns -1 in case of error.
*/
int
VideoDevice::GetFramerate() const
{
   struct video_window vwin;
    
   if (CamFD < 0)
      return -1;
   if (HasFramerate) 
   {
      if (ioctl(CamFD, VIDIOCGWIN, &vwin) < 0)
         return -1;
#if defined(PWC_FPS_SHIFT)       
      return (vwin.flags & PWC_FPS_FRMASK) >> PWC_FPS_SHIFT;
#endif     
   }
   return FrameRate;
}

/**
   \brief Try to set a framerate
   \param fps The framerate, in frames per second.
   \return true if the framerate was accepted, false otherwise
  
   Some webcams allow their framerate to be set; this functions tries to do
   just that; in general, the camera will use the framerate closest to what it
   supports. In case a device does not support framerates or only a fixed
   framerate (grabber cards!) we use the framerate to set the timer.
*/
bool
VideoDevice::SetFramerate(int fps)
{
   struct video_window vwin;
   cerr << "VideoDevice::SetFramerate(" << fps << ")" << endl;
   if (CamFD < 0 || ioctl(CamFD, VIDIOCGWIN, &vwin) < 0)
      return false;

   if (HasFramerate) 
   {
      //     MSync(); // Drop current frame
      FrameRate = fps;
      if (FrameRate <= 0)
         FrameRate = 0;
      if (FrameRate > 63)
         FrameRate = 63;
#if defined(PWC_FPS_SHIFT)
      vwin.flags = (vwin.flags & ~PWC_FPS_MASK) | (FrameRate << PWC_FPS_SHIFT);
//qDebug("Setting framerate -> 0x%x\n", vwin.flags);
#endif     
      if (ioctl(CamFD, VIDIOCSWIN, &vwin) < 0)
         return false;
      //     MCapture(CurBuffer); // Try to grab new frame
   }
   else 
   {
      FrameRate = fps;
      if (FrameRate <= 0) 
         FrameRate = 1;
      if (FrameRate > 60)
         FrameRate = 60;
   }
   return true;
}

/**
   \brief Return number of mmaped() buffers.
   \return 0 if no mmap() support exists.
   \condition The device must be opened.

   In case the device supports mmap(), this returns the number of buffers
   that are available and mapped.
*/
int
VideoDevice::GetMBuffers() const
{
   if (VMBuf.size == 0)
      return 0;
   return VMBuf.frames;
}  

/**
   \brief Return available buffers
   \return The number of image buffers used internally.
   \condition The device must be opened.

   Returns the numbers of buffers in use for this device, regardless if it
   supports mmap() or not.
*/
int
VideoDevice::GetBuffers() const
{
   return Buffers;
}  

/**
   \brief Return brightness setting of device.
   \return unsigned integer in the range 0-65535. 65535 may indicate setting is not available.
*/
int
VideoDevice::GetBrightness() const
{
   return VPic.brightness;
}

/**
   \fn bool VideoDevice::SetBrightness(int val)
   \brief Set brightness in device
   \param val An integer in the range 0-65535.
   \return false if the device is closed or the call failed.
  
   The value returned by GetBrightness may be slightly different from 
   what is set with SetBrightness.
*/
bool
VideoDevice::SetBrightness(int val)
{
   if (CamFD < 0)
      return false;
    
   VPic.brightness = val & 0xffff;
   if (ioctl(CamFD, VIDIOCSPICT, &VPic) < 0)
      return false;
   return true;
}

/**
   \fn int VideoDevice::GetContrast() const
   \brief Return contrast setting of device.
   \return unsigned integer in the range 0-65535. 65535 may indicate setting is not available.
*/
int
VideoDevice::GetContrast() const
{
   return VPic.contrast;
}

/**
   \fn bool VideoDevice::SetContrast(int val)
   \brief Set contrast in device.
   \param val An integer in the range 0-65535.
   \return false if the device is closed or the call failed.
  
   The value returned by GetContrast may be slightly different from 
   what is set with SetContrast.
*/
bool
VideoDevice::SetContrast(int val)
{
   if (CamFD < 0)
      return false;

   VPic.contrast = val & 0xffff;
   if (ioctl(CamFD, VIDIOCSPICT, &VPic) < 0)
      return false;
   return true;
}

/**
   \fn int VideoDevice::GetHue() const
   \brief Return hue (color shift) setting of device.
   \return unsigned integer in the range 0-65535. 65535 may indicate setting is not available.
  
   Hue is a way to correct for colour deviations. 
   It is something different than \ref GetColour.
*/
int
VideoDevice::GetHue() const
{
   return VPic.hue;
}

/**
   \fn bool VideoDevice::SetHue(int val)
   \brief Set hue in device
   \param val An integer in the range 0-65535.
   \return false if the device is closed or the call failed.
  
   Hue is a way to correct for colour deviations.
   The value returned by GetHue may be slightly different from 
   what is set with SetHue.
*/
bool
VideoDevice::SetHue(int val)
{
   if (CamFD < 0)
      return false;
    
   VPic.hue = val & 0xffff;
   if (ioctl(CamFD, VIDIOCSPICT, &VPic) < 0)
      return false;
   return true;
}

/**
   \fn int VideoDevice::GetColour() const
   \brief Return colour saturation setting of device.
   \return unsigned integer in the range 0-65535. 65535 may indicate setting is not available.
  
   A colour saturation of 0 means no colour at all, so the returned
   images are grayscale.
*/
int
VideoDevice::GetColour() const
{
   return VPic.colour;
}

/**
   \fn bool VideoDevice::SetColour(int val)
   \brief Set colour saturation in device.
   \param val An integer in the range 0-65535.
   \return false if the device is closed or the call failed.
  
   Colour saturation sets how bright colours should appear. A 
   saturation of 0 yields grayscale images.

   The value returned by GetColour may be slightly different from 
   what is set with SetColour.
*/
bool VideoDevice::SetColour(int val)
{
   if (CamFD < 0)
      return false;
    
   VPic.colour = val & 0xffff;
   if (ioctl(CamFD, VIDIOCSPICT, &VPic) < 0)
      return false;
   return true;
}

/**
   \fn int VideoDevice::GetWhiteness() const
   \brief Return gamma setting of device.
   \return unsigned integer in the range 0-65535. 65535 may indicate setting is not available.
  
   Sometimes used as a brightness contrast, but more generally this returns
   the gamma correction the device applies to the image.
*/
int
VideoDevice::GetWhiteness() const
{
   return VPic.whiteness;
}

/**
   \fn bool VideoDevice::SetWhiteness(int val)
   \brief Set gamma value in device
   \param val An integer in the range 0-65535.
   \return false if the device is closed or the call failed.

   Whiteness is sometimes used as brightness, but usually this sets the
   gamma correction the device will apply to the image.
  
   The value returned by GetWhiteness may be slightly different from 
   what is set with SetWhiteness.
*/
bool
VideoDevice::SetWhiteness(int val)
{
   if (CamFD < 0)
      return false;
    
   VPic.whiteness = val & 0xffff;
   if (ioctl(CamFD, VIDIOCSPICT, &VPic) < 0)
      return false;
   return true;
}


/**
   \fn int VideoDevice::ReadImage()
   \brief Read image into Buffers.
   \return 0 on success, \b -errno otherwise.
  
   This function reads the raw data from the device and transforms it into
   RGB and/or YUV images, doing all necessary conversions.
*/
int
VideoDevice::ReadImage(const unsigned char* dst)
{
   unsigned char *src;
   int i, n;

   if (CamFD < 0)
      return -ENODEV;
   if (vid_io_buffer == NULL)
      return -ENOMEM;
    
   src = vid_io_buffer + vid_io_offsets[CurBuffer];
   if (VMBuf.size > 0) 
   { 
      // mmap()ed.
      i = MSync();
      if (i < 0)
         return i;
      if (Buffers > 1) // we have space, start capture immediately in next buffer. Otherwise, see end of function
         MCapture((CurBuffer + 1) % Buffers); 
   }
   else {
      if (read(CamFD, src, vid_io_image_size) < 0)
         return -errno;
   }
    
   /* At this point we have our (raw) data in the buffer; it could be we
      are finished now because we have put the data into place with
      PalRGB && VIDEO_PALETTE_RGB32 or PalYUV && VIDEO_PALETTE_YUV420P.
      But in most other cases we will have to do some conversion...
   */
   n = image_w * image_h;
    
   switch (Palette) 
   {
      case VIDEO_PALETTE_YUV420P:
         /* only RGB needs conversion :-) */
         if (PalRGB) 
         {
            ccvt_420p_rgb24(image_w, image_h, src, (void*)dst);
         }
         break;

#if 0
      case VIDEO_PALETTE_RGB32:
         /* Quick-n-dirty palette */
         if (PalRGB)
            memcpy(dst, src, n * 4);            
         break;

      case VIDEO_PALETTE_RGB24:
         /* "Broken" BGR palette */
         if (PalRGB) 
         {
            drgb = (QRgb *)dst;
            for (i = 0; i < n; i++) 
            {
               *drgb++ = qRgb(src[2], src[1], src[0]);
               src += 3;
            }
         }

         dy = src;
         du = src + n;
         dv = du + (n / 4);
            
#if 0
         /* some code to determine max/min values */
         int miny, maxy, minu, maxu, minv, maxv;
            
         miny = 255; maxy = 0;
         src = dy;
         for (i = 0; i < n; i++) 
         {
            if (*src < miny) miny = *src;
            if (*src > maxy) maxy = *src;
            src++;
         }
         n /= 4;
         minu = 255; maxu = 0;
         src = du;
         for (i = 0; i < n; i++) 
         {
            if (*src < minu) minu = *src;
            if (*src > maxu) maxu = *src;
            src++;
         }
         minv = 255; maxv = 0;
         src = dv;
         for (i = 0; i < n; i++) 
         {
            if (*src < minv) minv = *src;
            if (*src > maxv) maxv = *src;
            src++;
         }
         printf("Y: min = %3d, max = %3d | U: min = %3d, max = %3d  | V: min = %3d, max = %3d ", miny, maxy, minu, maxu, minv, maxv);
#endif       
#endif
      
      case VIDEO_PALETTE_YUV422:
         break;
      default:
         cerr << "should not be here: " << endl;
         exit(1);
         break;
   }
    

   /* Go to next buffer (including capture). In case of 1 buffer CurBuffer
      will get stuck at 0, so we do this after we processed the buffer
      content.
   */
   CurBuffer = (CurBuffer + 1) % Buffers;
   if (Buffers == 1) {
      if (MCapture(CurBuffer) < 0)
         return -errno;
   }

   return 0;
}


#define MAGIC 2    // indicates buffer size

VideoDevice videodev("/dev/video0");
static unsigned char** buffers;
int rIdx = 0;
int idx = 1;
Semaphore sem;

/** 
 * Read out video stream in background !
 */
THREADFUNC_RETURN_TYPE
readVideoFrames(void *data) 
{   
   cerr << "WORKER THREAD: started !" << endl;

   while (1) 
   {
      videodev.ReadImage(buffers[rIdx]);
      rIdx++;
      rIdx %= MAGIC;
      sem.wait();
      idx=(rIdx+1)%2;
      sem.signal();
   }

   return THREADFUNC_NULL;
}

SbBool
SoVideoBackgroundUSB::initVideoBackground(int width, int height)
{
   cerr << "InitOverlay with size: " << width << "x" << height << endl;
   videodev.Open();
   videodev.SetFramerate(15);
   videodev.SetSize(640,480);
   videodev.EnableRGB(true);
   field = new unsigned char[640*480*3*MAGIC];
   buffers = new unsigned char*[MAGIC];
   for (int i=0; i<MAGIC; i++) {
      buffers[i] = &field[640*480*3*i];
   }
   
   initTexture(getTexSize(640,480));

   // initialize mulit-threading stuff
   workerID = createThread(readVideoFrames, NULL);

   return TRUE;
}

SoVideoBackgroundUSB::~SoVideoBackgroundUSB()
{
   cerr << "endOverlay ..." << endl;
   videodev.EnableRGB(false);
   videodev.Close();
   killThread(workerID);
   delete [] buffers;
   delete [] field;
   glDeleteTextures(1, &videoTexture);
}

/**
 * Calculate minimal texture size
 */
int 
SoVideoBackgroundUSB::getTexSize(int w, int h)
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
SoVideoBackgroundUSB::initTexture(int size) 
{
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
SoVideoBackgroundUSB::blitOverlay(int eye)
{
   int w = 640;
   int h = 480;
   // videodev.ReadImage(field);

   preGLCalls();

   float m = mirrorMode.getValue() ? -1.0F : 1.0F;
    
   glPixelZoom (m, -1.0);
   glRasterPos2f (-m,1);
    
   glPixelTransferf (GL_RED_SCALE, 1.0);
   glPixelTransferf (GL_GREEN_SCALE, 1.0);
   glPixelTransferf (GL_BLUE_SCALE, 1.0);
    
   glPixelTransferi (GL_MAP_COLOR, GL_FALSE);
    
   glColor3f (1.0F, 0.1F, 1.0F);

   int size = getTexSize(w,h);

   glOrtho(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5);

   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glBindTexture(GL_TEXTURE_2D, videoTexture);

   // get the whole image from camera

   sem.wait();
   // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, field);
   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffers[idx]);
   sem.signal();

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
#endif //PC_USBCAM

/* ===========================================================================
    End of SoVideoBackGroundUSB.cxx
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
