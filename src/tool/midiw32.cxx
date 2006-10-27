/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for midi helper functions.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifdef USE_MIDI

//#  ifdef WIN32

#  include <OpenTracker/tool/midi.h>
#  include <string>

#  pragma message("MidiModule: linking against : Winmm.lib")
#  pragma comment( lib, "Winmm.lib" )

const unsigned char SHORTMSG_MASK = 15;
const unsigned char SHORTMSG_DATA1 = 8;
const unsigned char SHORTMSG_DATA2 = 16;




void unpackChannelMsg(const MIDISHORTMSG *msg, MIDISHORTMSG_CHANNEL * cmsg){
  cmsg->command = static_cast<byte>(msg->msg & ~SHORTMSG_MASK);
  cmsg->channel = static_cast<byte>(msg->msg &  SHORTMSG_MASK);
  cmsg->data1  =  static_cast<unsigned char>((msg->msg) >> SHORTMSG_DATA1);
  cmsg->data2  =  static_cast<unsigned char>((msg->msg) >> SHORTMSG_DATA2);
}

void unpackStatusMsg(const MIDISHORTMSG * msg, MIDISHORTMSG_STATUS * smsg){

  smsg->status = static_cast <unsigned char>(msg->msg);
  smsg->data1  = static_cast <unsigned char>((msg->msg) >> SHORTMSG_DATA1) ;
  smsg->data2  = static_cast <unsigned char>((msg->msg) >> SHORTMSG_DATA2) ;
}


/* Find out the number of input devices available */

unsigned long midiInDevCount(){
  return midiInGetNumDevs();
};

/* read capabilities of device id, devid can also be a casted handle,
   return values can */
unsigned long midiInDevCaps(unsigned long devid, MIDIINPUTCAPS * Outcaps, unsigned long sizeofcaps){
  MIDIINCAPS inCaps;
  unsigned long result = midiInGetDevCaps(devid, &inCaps, sizeof(inCaps));
  Outcaps->mMid = inCaps.wMid;
  Outcaps->mPid = inCaps.wPid;
  Outcaps->mDriverVersion = inCaps.vDriverVersion;
  for (int i = 0; i < MAXPNAMELEN; i++)
      Outcaps->mPname[i] = inCaps.szPname[i];
  return result;
  }



/* a handy function to be used as callback when a msg on an input midi arrives */
void CALLBACK handleFunc(MIDIINHANDLE hdl, unsigned long msg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 ){
    
	MidiInHandler * inHandler = (MidiInHandler *)(dwInstance);

	
    switch(msg){
      // Receive a short message 
    case MIM_DATA: {
		
      MIDISHORTMSG sMsg;
      sMsg.msg = dwParam1;
      sMsg.timestamp = dwParam2;
      
      inHandler->handleShortMsg(sMsg);
      break;
    }
      // ERROR 
    case MIM_ERROR: {
      MIDISHORTMSG sMsg;
      sMsg.msg = dwParam1;
      sMsg.timestamp = dwParam2;

      inHandler->onShortMsgError(sMsg);
      break;
    }
      // Receive all or part of some System Exclusive message
    case MIM_LONGDATA: {
      break;
    }
      //
    case MIM_LONGERROR: {
      printf("CALLBACK: received LONGERROR");
      break;
    }

      // A device is now open 
    case MIM_OPEN: {
      printf("CALLBACK: received open");
      break;
    }
      
      // A device is now close 
    case MIM_CLOSE: {
      printf("CALLBACK: received close");
      break;
    }
      //
    case MIM_MOREDATA: {
      printf("CALLBACK: received MOREDATA");
      break;
    }

  }
}


unsigned long midiOpenIn(MIDIINHANDLE * moutHnd, unsigned long devnum, MidiInHandler * inDevHandler){

  return midiInOpen(moutHnd, (UINT) devnum, (DWORD)handleFunc, (DWORD) inDevHandler, CALLBACK_FUNCTION);

}

unsigned long midiCloseIn(MIDIINHANDLE hnd){
  return midiInClose(hnd);
}

unsigned long midiStartIn(MIDIINHANDLE hdl){
  return midiInStart(hdl);
};

unsigned long midiStopIn(MIDIINHANDLE hdl){
  return midiInReset(hdl);
}

void midiFormatErrorString(unsigned long err, char * buf, unsigned long size){
  midiInGetErrorText(err, &buf[0], size);
}
std::string midiErrorString(unsigned long err){
#define BUFFERSIZE 200
  char buffer [BUFFERSIZE];
  midiFormatErrorString(err, &buffer[0], BUFFERSIZE);
  return std::string(buffer);
}


//#endif //WIN32

#endif // USE_MIDI
