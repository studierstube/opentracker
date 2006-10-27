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
/** source file for MidiSource.
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifdef USE_MIDI

#include <OpenTracker/input/MidiSource.h>
#include <OpenTracker/tool/midi.h>

namespace ot{

MidiSource::MidiSource(unsigned int devid){
    unsigned long err;
    changed = 0;
	printf("MIDISOURCE:: opening devid %u\n", devid);
    if((err=midiOpenIn(&inHdl, (unsigned long)devid, dynamic_cast<MidiInHandler *>(this)))){
		printf("MIDISOURCE::error openning device %s\n", midiErrorString(err).c_str());
		//      throw MidiEx(midiErrorString(err));
      }  
};


MidiSource::~MidiSource(){
  printf("MIDISRC IS DESTROYED\n");
  stopRecording();
  close();
};

void MidiSource::startRecording(){
  unsigned long err = midiStartIn(inHdl);
  if (err != MIDINOERROR){
      //      throw MidiEx(midiErrorString(err));
    }

};

void MidiSource::stopRecording(){
  midiStopIn(inHdl);
};

int MidiSource::isEventGenerator(){
  return 1;
};
void MidiSource::pushEvent(){
	
  if (changed){
		  printf("MIDISOURCE::Updating observers\n");
    updateObservers(event);
    changed = 0;
  }
};


void MidiSource::handleShortMsg( MIDISHORTMSG & msg) {
  MIDISHORTMSG_STATUS statusMsg;
  unpackStatusMsg(& msg, &statusMsg);
  //printf("0x%08X 0x%02X 0x%02X 0x%02X\r\n", msg.timestamp, (statusMsg.status & 0x000000FF), (statusMsg.data1 & 0x000000FF), (statusMsg.data2 & 0x000000FF));
  event.setAttribute("status", statusMsg.status);
  event.setAttribute("data1", statusMsg.data1);
  event.setAttribute("data2", statusMsg.data2);
  event.setAttribute("midiTimestamp", msg.timestamp);
  changed = 1;

};

/* do something when an error from short messages*/
void MidiSource::onShortMsgError( MIDISHORTMSG & msg) {

};
/* receive a long message */
void MidiSource::handleLongMsg(  MIDILONGMSG & msg) {

};
/* do something when an error from long messages */
void MidiSource::onLongMsgError( MIDILONGMSG & msg) {

};

void MidiSource::close(){
  unsigned long err;
  while ((err = midiInClose(inHdl)) == MIDISTILLPLAYING){
    stopRecording();
    Sleep(0);
  }
  
  if (err){
    //      throw MidiEx(midiErrorString(err));
  }
};

}; // namespace ot

#endif //USE_MIDI
