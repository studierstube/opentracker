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
/** implementation for the OpenH323 handling
 *
 * @author Werner Frieb, Istvan Barakonyi, Tamer Fahmy
 *
 * $Id: H323Utils.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include "H323Utils.h"

#ifdef USE_H323

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <stbapi/tracker/TrackerOT.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

///////////////////////////////////////////////////////////////////////////
// H323Utils_VideoOutputDevice
///////////////////////////////////////////////////////////////////////////
H323Utils_VideoInputDevice::H323Utils_VideoInputDevice(H323Utils_EndPoint *endpoint) :
   endpoint(endpoint)
{
   SetFrameRate(10);

   SetPreferredColourFormat(endpoint->imageFormat);
   SetColourFormatConverter("YUV420P");
}

PINDEX
H323Utils_VideoInputDevice::GetMaxFrameBytes()
{
   if (converter) {
      PINDEX bytes = converter->GetMaxDstFrameBytes();
      if (bytes > frameBytes) {
         return bytes;
      }
   }
   
   return frameBytes;
}

BOOL
H323Utils_VideoInputDevice::GetFrameData(BYTE *buffer,
                                         PINDEX *bytesReturned)
{    
   frameTimeError += msBetweenFrames;

   PTime now;
   PTimeInterval delay = now - previousFrameTime;
   frameTimeError -= (int)delay.GetMilliSeconds();
   previousFrameTime = now;

   if (frameTimeError > 0) {
      PThread::Current()->Sleep(frameTimeError);
   }
   GetFrameDataNoDelay(buffer, bytesReturned);
   return TRUE;
}

BOOL
H323Utils_VideoInputDevice::GetFrameDataNoDelay(BYTE *buffer,
                                                PINDEX *bytesReturned)
{
   if (endpoint->local && buffer && converter) {
      converter->Convert(endpoint->GetFrame(), buffer); 
   }
   *bytesReturned = frameBytes;
   return TRUE;
}

BOOL
H323Utils_VideoInputDevice::GetFrameSizeLimits(unsigned &minWidth,
                                               unsigned &minHeight,
                                               unsigned &maxWidth,
                                               unsigned &maxHeight)
{
   minWidth = minHeight = 1;
   maxWidth = maxHeight = 1024;
   return TRUE;
}

BOOL
H323Utils_VideoInputDevice::SetFrameSize(unsigned width, unsigned height)
{
   if (!PVideoDevice::SetFrameSize(width, height)) {
      return FALSE;
   }
   
   frameBytes = CalculateFrameBytes(frameWidth, frameHeight, colourFormat);
   converter->SetSrcFrameSize(endpoint->imageWidth, endpoint->imageHeight);
   converter->SetDstFrameSize(frameWidth, frameHeight, TRUE);
   return TRUE;
}


///////////////////////////////////////////////////////////////////////////
// H323Utils_VideoOutputDevice
///////////////////////////////////////////////////////////////////////////
H323Utils_VideoOutputDevice::H323Utils_VideoOutputDevice(H323Utils_VideoBuffer *aBuffer,
                                                         BOOL flipIt, int x, int y) :
   flip(flipIt),
   buffer(aBuffer)
{
   SetColourFormatConverter("YUV420P");
}

BOOL
H323Utils_VideoOutputDevice::SetFrameSize(unsigned w, unsigned h)
{
   unsigned int old_w, old_h;
   PVideoOutputDeviceRGB::GetFrameSize(old_w, old_h);

   if (old_w != w || old_h != h) 
   {
      if (!PVideoOutputDeviceRGB::SetFrameSize(w, h))
         return FALSE;
   }
   return TRUE;
}


///////////////////////////////////////////////////////////////////////////
// H323Utils_EndPoint
///////////////////////////////////////////////////////////////////////////
H323Utils_EndPoint::H323Utils_EndPoint() :
   displayDevice(NULL),
   grabber(NULL),
   router(0),
   local(TRUE)
{}


H323Utils_EndPoint::~H323Utils_EndPoint()
{ 
   StopCamera(); 
}

void
H323Utils_EndPoint::Init(BOOL local, char *ip,BOOL useVideo,BOOL useAudio)
{
   this->local = local;
   strcpy(ipAddress, ip);

   videoOn=useVideo;
   audioOn=useAudio;

   capabilities.RemoveAll();

   capabilities.AddAllCapabilities(*this, 0, 0, "*{sw}");

   // disable fast start, because if fast start is enabled 
   // the system will open 4 video channels (and then close 2 of them)
   DisableFastStart(true);

   videoCodec = 3;
   autoStartTransmitVideo = videoCodec > 1;
   autoStartReceiveVideo = TRUE;

   switch (videoCodec) {
      case 3 : // CIF
         SetCapability(0, P_MAX_INDEX, new H323_H261Capability(0, 1, FALSE, FALSE, 6217));
         SetCapability(0, P_MAX_INDEX, new H323_H261Capability(1, 0, FALSE, FALSE, 6217));
         videoWidth = 352;
         videoHeight = 288;
         break;

      case 2 : // QCIF
         SetCapability(0, P_MAX_INDEX, new H323_H261Capability(1, 0, FALSE, FALSE, 6217));
         SetCapability(0, P_MAX_INDEX, new H323_H261Capability(0, 1, FALSE, FALSE, 6217));
         videoWidth = 176;
         videoHeight = 144;
         break;

      case 1 :
         unsigned qcif = 1; 
         unsigned cif = 2; 
         if (qcif > 0)
            SetCapability(0, P_MAX_INDEX, new H323_H261Capability(qcif, 0));
         if (cif > 0)
            SetCapability(0, P_MAX_INDEX, new H323_H261Capability(0, cif));

         videoWidth = 50; videoHeight = 50;
   }

   // Audio Codecs
   SpeexNarrow3AudioCapability *speex3Cap; 
   SetCapability(0, 0, speex3Cap = new SpeexNarrow3AudioCapability());
   speex3Cap->SetTxFramesInPacket(5); // For Speex, 1 frame ~ 20 milliseconds
   H323_GSM0610Capability * gsmCap;
   SetCapability(0, 0, gsmCap = new H323_GSM0610Capability);
   gsmCap->SetTxFramesInPacket(4); // For GSM 06.10, 1 frame ~ 20 milliseconds 
   SetCapability(0, 0, new H323_G711Capability(H323_G711Capability::muLaw) );
   SetCapability(0, 0, new H323_G711Capability(H323_G711Capability::ALaw) );
   SetCapability(0, 0, new H323_LPC10Capability(*this) ); 

   H323_UserInputCapability::AddAllCapabilities(capabilities, 0, P_MAX_INDEX);

   // initialize video buffers for local and remote videos
   unsigned int bufSize = videoWidth*videoHeight*3;

   localVideoBuffer.Lock();
   localVideoBuffer.data = new BYTE[bufSize];
   localVideoBuffer.Unlock();

   remoteVideoBuffer.Lock();
   remoteVideoBuffer.data = new BYTE[bufSize];
   remoteVideoBuffer.Unlock();

   if (local) {
      // start the H323 stack listening for incoming calls
      printf("H323: start listening for incoming calls\n");
      PString interfaces = "*";
      StartListeners(interfaces.Tokenise(','));
      printf("H323: init ARToolkit\n");

      TrackerOT * tracker = (TrackerOT *)Stb3DEventGenerator::getTracker();
      Context * context = tracker->getContext();

#ifdef USE_ARTOOLKIT
      toolkit = (ARToolKitModule *)context->getModule("ARToolKitConfig");
      
      if (toolkit->isInitialized()) {
         printf("H323: Init video link to ARToolkit module with %i %i\n", 
                toolkit->getSizeX(), toolkit->getSizeY());
         if (toolkit->getSizeX() > 0 && toolkit->getSizeY() > 0) {
            imageWidth  = toolkit->getSizeX();
            imageHeight = toolkit->getSizeY();
         }
         else {
            printf("H323: no useful video image size !\n");
            return;
         }
      }
      
      if (toolkit->getImageFormat() == 0x1907) { // 0x1907 == GL_RGB from GL/gl.h
         imageFormat = "RGB24";
         printf("H323: image format: RGB24\n");
      }
      else {
         imageFormat = "RGB32";
         printf("H323: image format: RGB32\n");
      }
#endif
   }
   else { // remote
      printf("H323: direct init\n");
      imageWidth = videoWidth; imageHeight = videoHeight;
      imageFormat = "RGB32";
   }
}

void
H323Utils_EndPoint::StopCamera()
{
#ifdef USE_ARTOOLKIT
   printf("H323: closed video link to ARToolkit module\n");
#endif
}

BOOL
H323Utils_EndPoint::OnIncomingCall(H323Connection &connection,
                                   const H323SignalPDU &, H323SignalPDU &)
{ 
   return TRUE;
}

void
H323Utils_EndPoint::OnConnectionEstablished(H323Connection &, const PString &)
{
   printf("H323: connection established\n");
}

void
H323Utils_EndPoint::OnConnectionCleared(H323Connection & , const PString &)
{
   PString currentCallToken; // H323 call token
   if (!local) MakeCall(ipAddress, currentCallToken);
}

BOOL
H323Utils_EndPoint::OnStartLogicalChannel(H323Connection &, H323Channel &channel)
{
   return TRUE;
}

void
H323Utils_EndPoint::OnClosedLogicalChannel(H323Connection &,
                                           const H323Channel & channel)
{}

H323Connection *
H323Utils_EndPoint::CreateConnection(unsigned callReference)
{
   return new H323Utils_Connection(*this, callReference);
}

void
H323Utils_EndPoint::TranslateTCPAddress(PIPSocket::Address &localAddr,
                                        const PIPSocket::Address &remoteAddr)
{
   if (!router.IsValid())
      return;
   
   /* Check if the remote address is a private IP address.
    * RFC 1918 specifies the following private IP addresses
    * 10.0.0.0    - 10.255.255.255.255
    * 172.16.0.0  - 172.31.255.255
    * 192.168.0.0 - 192.168.255.255
    */
   if (remoteAddr.Byte1() == 10 ||
       (remoteAddr.Byte1() == 172 && remoteAddr.Byte2() >= 16 && remoteAddr.Byte2() <= 31) ||
       (remoteAddr.Byte1() == 192 && remoteAddr.Byte2() == 168)
      )
      return;

   /*
    * We are using NAT and the remote address is outside our LAN, so 
    * replace the local address with the IP address of the NAT box.
    */
   localAddr = router;
}

BOOL
H323Utils_EndPoint::OpenAudioChannel(H323Connection &connection, BOOL isEncoding, unsigned bufferSize, H323AudioCodec &codec)
{
   if (!audioOn) return FALSE;

   if (!H323EndPoint::OpenAudioChannel(connection, isEncoding, bufferSize, codec)) {
      return FALSE;
   }

   //codec.SetSilenceDetectionMode(H323AudioCodec::NoSilenceDetection);
   //codec.SetSilenceDetectionMode(H323AudioCodec::AdaptiveSilenceDetection);

   return TRUE;
}

BOOL
H323Utils_EndPoint::OpenVideoChannel(H323Connection &connection, BOOL isEncoding,
                                     H323VideoCodec &codec)
{
   if (!videoOn) return FALSE;

   PVideoChannel *channel = new PVideoChannel;

   if (isEncoding) {
      codec.SetTxQualityLevel(1); // high quality
      codec.SetBackgroundFill(2);

      // create grabber.
      grabber = new H323Utils_VideoInputDevice(this);

      if (!grabber->Open("", FALSE) || !grabber->SetVideoFormat(PVideoDevice::Auto) ||
          !grabber->SetChannel(0) || !grabber->SetFrameSize(videoWidth, videoHeight))
      {
         delete grabber;
         return FALSE;
      }

      grabber->Start();
      channel->AttachVideoReader(grabber);
      displayDevice = new H323Utils_VideoOutputDevice(&localVideoBuffer, FALSE, 0, 0);
   }
   else {
      displayDevice = new H323Utils_VideoOutputDevice(&remoteVideoBuffer, FALSE, 0, 0);
   }

   displayDevice->SetFrameSize(videoWidth, videoHeight);
   channel->AttachVideoPlayer(displayDevice);
   return codec.AttachChannel(channel, true);
}

unsigned char const *
H323Utils_EndPoint::GetFrame()
{
#ifdef USE_ARTOOLKIT
   MemoryBufferHandle handle[2];
   unsigned char *frameSrc = toolkit->lockFrame(&handle[0]);
   toolkit->unlockFrame(handle[0]);
   return frameSrc;

#if 0
/* FIXME: the following routing is best described as slowest dog in
 *        hell && as buggy as the devil that tries to do image downscaling...
 *        Until some workaround is found or OpenVideo gets ready set ARToolKit
 *        camera resolution to 320x240. SoVideoBackground is workarounded
 *        in this regard. this turned out to be the setting that gives the best
 *        results. 20030715 tamer.
 */
   unsigned char *frameSrc = toolkit->lockFrame(&handle[0]);
   static unsigned char frameDst[352*288*3];
   static float factorX = 640/352.0, factorY = 480/288.0;
   static int offsetDst = 352*3, offsetSrc = 640*3, idx = 3;
   
   for (register int y0=0, y1=0; y0 < 288; y0++) {
      for (register int x=0; x < 352*idx; x+=idx) {
         frameDst[x  + y0*offsetDst] = frameSrc[(int)round(factorX*x)    + y1*offsetSrc];
         frameDst[x+1+ y0*offsetDst] = frameSrc[(int)round(factorX*(x+1))+ y1*offsetSrc];
         frameDst[x+2+ y0*offsetDst] = frameSrc[(int)round(factorX*(x+2))+ y1*offsetSrc];
      }
      y1 = (int)round(factorY*y0);
   }
   
   toolkit->unlockFrame(handle[0]);

   return (unsigned char* const)frameDst;
#endif
   
#else
   return NULL;
#endif

}

unsigned int
H323Utils_EndPoint::GetImageFormat()
{
#ifdef USE_ARTOOLKIT
   return toolkit->getImageFormat();
#else
   return 0;
#endif
}

///////////////////////////////////////////////////////////////////////////
// H323Utils_Connection
///////////////////////////////////////////////////////////////////////////
H323Utils_Connection::H323Utils_Connection(H323Utils_EndPoint & ep,
                                           unsigned callReference) :
   H323Connection(ep, callReference),
   myEndpoint(ep)
{}

H323Connection::AnswerCallResponse
H323Utils_Connection::OnAnswerCall(const PString & /*caller*/,
                                   const H323SignalPDU & /*setupPDU*/,
                                   H323SignalPDU & /*connectPDU*/)
{
   return AnswerCallNow;
}


///////////////////////////////////////////////////////////////////////////
// H323Utils_App
///////////////////////////////////////////////////////////////////////////
H323Utils_App::H323Utils_App()
   : PProcess("studierstube", "H323Utils")
{}

#endif

