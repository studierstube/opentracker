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
/* Piano.h contains the Piano component
 *
 * @author Istvan Barakonyi
 *
 * The MIDI handling routines are based on the MidiStation program.
 * Copyright (c) 2002-2003 by Carlos Jiménez de Parga
 * http://www.codeproject.com/audio/MidiPorts.asp
 *
 * $Id: Piano.h 3910 2004-10-13 18:14:44Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _PIANO_H_
#define _PIANO_H_

// STD includes
#include <vector>
#include <list>

// Inventor includes
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include "Inventor/sensors/SoAlarmSensor.h"
#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SbIntList.h>

#include <windows.h>
//typedef DWORD* DWORD_PTR; // for VC6: uncomment this line, for Visual Studio.NET: comment this line out

// DirectMusic includes
#include <dmusicc.h>
#include <dmusici.h>
#include <dmksctrl.h>
#include <dmdls.h>
#include <tchar.h>

typedef std::vector<WORD> VECTOR;

// for the DLL export
#include "pianotutor.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** Piano is a nodekit representing a virtual keyboard capable of handling MIDI events
@author Istvan Barakonyi
*/
class PIANOTUTOR_API Piano : public SoBaseKit
{
    SO_KIT_HEADER(Piano);

    SO_KIT_CATALOG_ENTRY_HEADER(keyboardGeometry);

public:

    typedef struct INFOPORT {
        TCHAR szPortDescription[DMUS_MAX_DESCRIPTION]; // port description string in MBCS/UNICODE chars
        DWORD dwFlags;      // port characteristics
        DWORD dwClass;      // class of port (Input/Output) 
        DWORD dwType;       // type of port (See remarks)   
        DWORD dwMemorySize; // amount of memory available to store DLS instruments
        DWORD dwMaxAudioChannels;  // maximum number of audio channels that can be rendered by the port
        DWORD dwMaxVoices;        // maximum number of voices that can be allocated when this port is opened
        DWORD dwMaxChannelGroups ; // maximum number of channel groups supported by this port 
        DWORD dwSampleRate;     // desired Sample rate in Hz;   
        DWORD dwEffectFlags;    // flags indicating what audio effects are available on the port
        DWORD dwfShare;     // exclusive/shared mode
        DWORD dwFeatures;   // miscellaneous capabilities of the port
        GUID  guidSynthGUID;    // identifier of the port
    } *LPINFOPORT;

    Piano();

    /// MIDI output enabled/disabled
    SoSFBool midiOutEnabled;

    /// List of piano keys pressed down currently
    SoMFInt32 keyDown;

    /// The time value for which the current piano keys should be kept pressed down. This field is used for MIDI output.
    SoSFTime keyDownTime;

    /// Indicates whether there are any piano keys pressed currently
    SoSFBool keyPressed;

    /// List of piano key codes pressed currently
    SoMFString codePressed;

    /// Name of the MIDI file to play
    SoSFString filename;

    /// Play MIDI file specified in <em>filename</em>
    SoSFTrigger play;

    /// Pause playing the MIDI file specified in <em>filename</em>
    SoSFTrigger pause;

    /// Stop playing the MIDI file specified in <em>filename</em>
    SoSFTrigger stop;

    /// List of input ports
    SoMFString inputPortList;

    /// List of output ports
    SoMFString outputPortList;

    /// Number of selected input port
    SoSFInt32 inputPortSelected;

    /// Number of selected output port
    SoSFInt32 outputPortSelected;

    /// Preferred input port index
    SoSFInt32 inputPortPreferred;

    /// Preferred output port index
    SoSFInt32 outputPortPreferred;

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:

    /// Stores whether the MIDI interface has been initialized successfully.
    bool initialized;

    /// DirectMusic objects
    /*@{*/

    IDirectMusic8 *directMusic;
    IDirectSound8 *directSound;
    IDirectMusicPerformance8 *performance;
    IDirectMusicLoader8 *loader;
    IDirectMusicSegment *segment;
    IDirectMusicSegmentState *segmentState;
    IReferenceClock *clock;
    DMUS_PORTPARAMS8 inputPortParams;
    DMUS_PORTPARAMS8 outputPortParams;
    IDirectMusicPort8* inputPort;
    IDirectMusicPort8* outputPort;
    IDirectMusicBuffer8* inputBuffer;
    IDirectMusicBuffer8* outputBuffer;
    IDirectMusicPortDownload8* portDownload;
    IDirectMusicThru8* thru;

    /*@}*/

    /// Utility atributes
    /*@{*/

    std::list<VECTOR> channelList;
    //bool softwareSynth;
    SbPList outputPortData;
    HANDLE hEvent;
    HANDLE hTerminationEvent;
    HANDLE hThread;
    bool processActive;
    SbIntList keyDownList;
    bool paused;
    bool stopped;
    int timestamp;

    SbIntList outputNoteTSList;
    SbIntList noteOffTimerTSList;
    SbIntList outputNoteList;
    SbPList noteOffTimerList;

    /*@}*/

    /// Field sensors
    /*@{*/

    SoFieldSensor *midiOutEnabledSensor;
    SoFieldSensor *keyDownSensor;
    SoFieldSensor *playSensor;
    SoFieldSensor *pauseSensor;
    SoFieldSensor *stopSensor;
    SoFieldSensor *inputPortPreferredSensor;
    SoFieldSensor *outputPortPreferredSensor;

    /*@}*/

    /// Destructor
    virtual ~Piano();

    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Static callback for field and alarm sensors
    /*@{*/

    static void midiOutEnabledCB(void *data,SoSensor *sensor);
    static void keyDownCB(void *data,SoSensor *sensor);
    static void noteOffTimerCB(void *data,SoSensor *sensor);
    static void playCB(void *data,SoSensor *sensor);
    static void pauseCB(void *data,SoSensor *sensor);
    static void stopCB(void *data,SoSensor *sensor);
    static void inputPortPreferredCB(void *data,SoSensor *sensor);
    static void outputPortPreferredCB(void *data,SoSensor *sensor);

    /*@}*/
    
    /// Initialize MIDI interface. Returns true if successful, else false.
    bool initMIDI();

    /// MIDI input/output port routines
    /*@{*/

    DWORD getNumPorts(DWORD portType);
    bool getPortInfo(DWORD portType,DWORD dwNumPort,INFOPORT *lpInfoPort);
    void setPortParams(DMUS_PORTPARAMS8 &portParams,DWORD dwVoices,DWORD dwAudioChannels,DWORD dwChannelGroups,
        DWORD dwEffectFlags,DWORD dwSampleRate);
    bool activatePort(IDirectMusicPort8 *&port,DMUS_PORTPARAMS8 &portParams,IDirectMusicBuffer8 *&buffer,LPINFOPORT infoPort,DWORD dwSysExSize);
    bool releasePort(IDirectMusicPort8 *&port);
    bool setMIDIThru(DWORD dwSourceChannel,DWORD dwDestinationChannelGroup,DWORD dwDestinationChannel);
    bool deactivateMIDIThru(DWORD dwSourceChannel,DWORD dwDestinationChannelGroup,DWORD dwDestinationChannel);
    bool activateNotification();
    bool terminateInputNotification();

    /*@}*/

    /// MIDI message handler routines
    /*@{*/

    bool sendMidiMsg(LPBYTE lpMsg,DWORD dwLength,DWORD dwChannelGroup);
    bool sendMidiMsg(DWORD dwMsg,DWORD dwChannelGroup);
    void recvMidiMsg(REFERENCE_TIME rt,DWORD dwChannel,DWORD dwBytesRead,BYTE *lpBuffer);
    void recvMidiMsg(REFERENCE_TIME lprt,DWORD dwChannel,DWORD dwMsg);
    DWORD encodeMidiMsg(BYTE Status,BYTE DataByte1,BYTE DataByte2);
    DWORD encodeMidiMsg(BYTE Command,BYTE Channel,BYTE DataByte1,BYTE DataByte2);
    void decodeMidiMsg(DWORD dwMsg,BYTE *Status,BYTE *DataByte1,BYTE *DataByte2);
    void decodeMidiMsg(DWORD dwMsg,BYTE *Command,BYTE *Channel,BYTE *DataByte1,BYTE *DataByte2);
    static DWORD WINAPI waitForEvent(LPVOID lpv);

    /*@}*/
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _PIANO_H_
