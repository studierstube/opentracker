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
/** H323Utils header file for the OpenH323 handling
 *
 * @author Werner Frieb, Istvan Barakonyi, Tamer Fahmy
 *
 * $Id: H323Utils.h 3587 2004-08-05 11:17:11Z bara $
 * @file                                                                   */
/* ======================================================================= */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef USE_H323

#ifdef WIN32
#define P_WINSOCKv1 1 // because of a conflict with WinSock2
#include <windows.h>
#include <ptlib.h>
#include <ptlib/vconvert.h>
#include <h323.h>
#else
#include <ptlib.h>
#include <ptlib/vconvert.h>
#include <openh323/h323.h>
#endif

#include <input/ARToolKitModule.h>

class H323Utils_EndPoint;

///////////////////////////////////////////////////////////////////////////
class H323Utils_VideoInputDevice : public PVideoInputDevice
///////////////////////////////////////////////////////////////////////////
{
   PCLASSINFO(H323Utils_VideoInputDevice, PVideoInputDevice);
 
  public:
   /** Create a new (fake) video input device. */
   H323Utils_VideoInputDevice(H323Utils_EndPoint *endpoint);

   /** Open the device given the device name. */
   BOOL Open(const PString & devName, BOOL startImmediate) { return TRUE; }

   /** Determine of the device is currently open. */
   BOOL IsOpen() { return TRUE; }

   /** Determine if the video device I/O capture is in progress. */
   BOOL IsCapturing() { return IsOpen(); }

   /** Get a list of all of the drivers available. */
   static PStringList GetInputDeviceNames()
   {
      PStringList list;
      list.AppendString("H323Utils Video In");
      return list;
   }

   /**
    * Get the maximum frame size in bytes.
    *
    * Note a particular device may be able to provide variable length
    * frames (eg motion JPEG) so will be the maximum size of all frames.
    */
   virtual PINDEX GetMaxFrameBytes();

   /**
    * Grab a frame. 
    *
    * There will be a delay in returning, as specified by frame rate.
    */
   virtual BOOL GetFrameData(BYTE *buffer,
                             PINDEX *bytesReturned = NULL);

   /**
    * Grab a frame.
    * 
    * Do not delay according to the current frame rate.
    */
   virtual BOOL GetFrameDataNoDelay(BYTE *buffer,
                                    PINDEX *bytesReturned = NULL);
   
   /** Get the minimum & maximum size of a frame on the device. */
   BOOL GetFrameSizeLimits(unsigned &minWidth, unsigned &minHeight,
                           unsigned &maxWidth, unsigned &maxHeight);

   /** Set the video format to be used. */
   BOOL SetVideoFormat(VideoFormat newFormat)
   {
      return PVideoDevice::SetVideoFormat(newFormat);
   }

   /** Get the number of video channels available on the device. */
   virtual int GetNumChannels() { return 1; }
   
   /** Set the video channel to be used on the device. */
   virtual BOOL SetChannel(int channelNumber)
   {
      return PVideoDevice::SetChannel(channelNumber);
   }
 
   /** Set the colour format to be used. */
   virtual BOOL SetColourFormat(const PString & colourFormat)
   {
      return PVideoDevice::SetColourFormat(colourFormat);
   }

   /** Set the video frame rate to be used on the device. */
   virtual BOOL SetFrameRate(unsigned rate)
   {
      return PVideoDevice::SetFrameRate(rate);
   }

   /** Set the frame size to be used. */
   virtual BOOL SetFrameSize(unsigned width, unsigned height);

   /** 
    * Try all known video formats & see which ones are accepted by the 
    * video driver */
   virtual BOOL TestAllFormats() { return TRUE; }

  protected:
   H323Utils_EndPoint *endpoint;
   PINDEX frameBytes;
};

///////////////////////////////////////////////////////////////////////////
class H323Utils_VideoBuffer 
///////////////////////////////////////////////////////////////////////////
{
  public:
   BYTE* data;
   PSemaphore mutex;

   H323Utils_VideoBuffer() : mutex(1, 1) { data=NULL; }
   ~H323Utils_VideoBuffer() { if (data!=NULL) delete data; data=NULL; }

   void Lock()   { mutex.Wait(); }
   void Unlock() { mutex.Signal(); }
};


///////////////////////////////////////////////////////////////////////////
class H323Utils_VideoOutputDevice : public PVideoOutputDeviceRGB
///////////////////////////////////////////////////////////////////////////
{
   PCLASSINFO(H323Utils_VideoOutputDevice, PVideoOutputDeviceRGB);
 
  public:
   H323Utils_VideoOutputDevice(H323Utils_VideoBuffer* aBuffer, BOOL flip,
                            int x=0, int y=0);
   ~H323Utils_VideoOutputDevice() {}

   BOOL Open(const PString &, BOOL) { return TRUE; }
   BOOL IsOpen() { return TRUE; }

   PStringList GetDeviceNames() const { PStringList devlist; return devlist; }

   BOOL SetFrameData(unsigned x, unsigned y, unsigned width, unsigned height, 
                     const BYTE * data, BOOL endFrame)
   {
      if (x+width > frameWidth || y+height > frameHeight) {
         return FALSE;
      }
      
      if (!endFrame) {
         return FALSE;
      }
      
      if (converter) {
         converter->Convert (data, frameStore.GetPointer ());
      }
      
      EndFrame ();
      
      return TRUE;
   }
   
   BOOL EndFrame()
   { 
      buffer->Lock();
      
      if (buffer->data) {
         memcpy(buffer->data, frameStore, frameWidth * frameHeight * 3);            
      }
      
      buffer->Unlock();
      
      return TRUE; 
   }
   
   virtual BOOL SetFrameSize(unsigned width, unsigned height);

  protected:
   BOOL flip;
   H323Utils_VideoBuffer *buffer;
}; 


///////////////////////////////////////////////////////////////////////////
class H323Utils_EndPoint : public H323EndPoint
///////////////////////////////////////////////////////////////////////////
{
   PCLASSINFO(H323Utils_EndPoint, H323EndPoint);

  public:
   H323Utils_EndPoint();
   ~H323Utils_EndPoint();

   H323Utils_VideoBuffer localVideoBuffer;
   H323Utils_VideoBuffer remoteVideoBuffer;

   H323Utils_VideoOutputDevice *displayDevice;
   PVideoInputDevice *grabber;

   PIPSocket::Address router;
   int videoCodec;
   int videoWidth;
   int videoHeight;

   // overrides from H323EndPoint
   BOOL OnIncomingCall(H323Connection &, const H323SignalPDU &, H323SignalPDU &);
   void OnConnectionEstablished(H323Connection &, const PString &);
   void OnConnectionCleared(H323Connection &, const PString &);
   BOOL OnStartLogicalChannel(H323Connection & connection, H323Channel &channel);
   void OnClosedLogicalChannel(H323Connection &, const H323Channel &channel);
 
   H323Connection *CreateConnection(unsigned callReference);
   void TranslateTCPAddress(PIPSocket::Address &localAddr, const PIPSocket::Address &remoteAddr);
   BOOL OpenAudioChannel(H323Connection &connection, BOOL isEncoding, unsigned bufferSize, H323AudioCodec &codec);
   BOOL OpenVideoChannel(H323Connection &connection, BOOL isEncoding, H323VideoCodec &codec);

   void Init(BOOL local, char *ip,BOOL useVideo,BOOL useAudio);
   void StopCamera();
   unsigned char const *GetFrame();
   unsigned int GetImageFormat();

   PString imageFormat;
   int imageWidth;
   int imageHeight;

#ifdef USE_ARTOOLKIT
   ARToolKitModule * toolkit;
#endif
   BOOL local;
   PString currentCallToken; // H323 call token
   char ipAddress[16];
   BOOL videoOn;
   BOOL audioOn;
};

///////////////////////////////////////////////////////////////////////////
class H323Utils_Connection : public H323Connection
///////////////////////////////////////////////////////////////////////////
{
   PCLASSINFO(H323Utils_Connection, H323Connection);

  public:
   H323Utils_Connection(H323Utils_EndPoint &mgr, unsigned callReference);

   // overrides H323Connection::OnAnswerCall
   AnswerCallResponse OnAnswerCall(const PString &, const H323SignalPDU &, H323SignalPDU &);

  protected:
   H323Utils_EndPoint & myEndpoint;
};


///////////////////////////////////////////////////////////////////////////
class H323Utils_App : public PProcess // : public PApplication
///////////////////////////////////////////////////////////////////////////
{
   PCLASSINFO(H323Utils_App, PProcess);

  public:
   H323Utils_App();
   void Main() {};
};

#endif

