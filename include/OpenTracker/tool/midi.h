#ifndef TOOL_MIDI_H
#define TOOL_MIDI_H

#ifdef USE_MIDI

#include <string>

#ifdef __WIN32__
# include <OpenTracker/tool/midiw32.h>
#else

#endif // WIN32

typedef struct MIDIINPUTCAPS {
  int       mMid; // manufacturer id
  int       mPid; // product id
  unsigned int mDriverVersion;
  char     mPname[MAXPNAMELEN];
  //  DWORD     dwSupport;
}MIDIINPUTCAPS;

/* Find out the number of input devices available */
unsigned long midiInDevCount();
/* read capabilities of device id, devid can also be a casted handle,
   return values can */
unsigned long midiInDevCaps(unsigned long devid, MIDIINPUTCAPS * Outcaps, unsigned long sizeofcaps);

void midiFormatErrorString(unsigned long err, char * buf, unsigned long size);

std::string midiErrorString(unsigned long err);



typedef struct MIDISHORTMSG{
  unsigned long msg;
  //  unsigned long noStatus;
  unsigned long timestamp;
}MIDISHORTMSG;

typedef struct MIDISHORTMSG_CHANNEL{
  unsigned char command;
  unsigned char channel;
  unsigned char data1;
  unsigned char data2;
} MIDISHORTMSG_CHANNEL;

typedef struct MIDISHORTMSG_STATUS{
  unsigned char status;
  unsigned char data1;
  unsigned char data2;
}MIDISHORTMSG_STATUS;

void unpackChannelMsg(const MIDISHORTMSG *msg, MIDISHORTMSG_CHANNEL * cmsg);
void unpackStatusMsg(const MIDISHORTMSG * msg, MIDISHORTMSG_STATUS * smsg);

typedef struct MIDILONGMSG{

}MIDILONGMSG;

class MidiInHandler {
public:
  virtual ~MidiInHandler(){};
  /* receive a short message */
  virtual void handleShortMsg( MIDISHORTMSG & msg) {};
  /* do something when an error from short messages*/
  virtual void onShortMsgError( MIDISHORTMSG & msg) {};
  /* receive a long message */
  virtual void handleLongMsg(  MIDILONGMSG & msg) {};
  /* do something when an error from long messages */
  virtual void onLongMsgError( MIDILONGMSG & msg) {};
  
};

unsigned long midiOpenIn(MIDIINHANDLE * moutHnd, unsigned long devnum, MidiInHandler * midiInHandler);

unsigned long midiCloseIn(MIDIINHANDLE hnd);

unsigned long midiStartIn(MIDIINHANDLE hdl);

unsigned long midiStopIn(MIDIINHANDLE hdl);

#endif // USE_MIDI

#endif // TOOL_MIDI_H
