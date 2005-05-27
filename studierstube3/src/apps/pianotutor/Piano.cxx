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
/* Piano.cxx contains the implementation of Piano
 *
 * @author Istvan Barakonyi
 *
 * $Id: Piano.cxx 3910 2004-10-13 18:14:44Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <stbapi/misc/SoMultiSwitch.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "Piano.h"

#define MULTI_TO_WIDE(x,y) MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,y,-1,x,_MAX_PATH);

const int THREAD_TIMEOUT = 30000;
const BYTE MIDI_STRUCTURED = 3;
const BYTE SHIFT_8=8;
const BYTE SHIFT_16=16;
const BYTE BITMASK=15;

// MIDI commands (incomplete, only those used in tbe project are defined!)
const BYTE NOTE_OFF			=	0x80;
const BYTE NOTE_ON			=	0x90;
const BYTE PATCH_CHANGE		=	0xC0;
const BYTE POLY_PRESSURE	=	0xA0;
const BYTE CONTROL_CHANGE	=	0xB0;
const BYTE ALLNOTES_OFF     =   0xB0;

template<class _II, class _Ty> inline
_II find(_II _F, _II _L, const _Ty& _V)
{
	for (; _F != _L; ++_F)
		if (*_F == _V) break;
	return (_F);
}

SO_KIT_SOURCE(Piano);

void Piano::initClass()
{
   SO_KIT_INIT_CLASS(Piano, SoBaseKit, "BaseKit");
}

Piano::Piano()
{
    SO_KIT_CONSTRUCTOR(Piano);
    
    SO_KIT_ADD_CATALOG_ENTRY(keyboardGeometry, SoMultiSwitch, FALSE, this, \0, TRUE);

    SO_KIT_ADD_FIELD(midiOutEnabled, (FALSE));
    SO_KIT_ADD_FIELD(keyDown, (0));
    SO_KIT_ADD_FIELD(keyDownTime, (SbTime(1.0f))); // default value is 1 sec
    SO_KIT_ADD_FIELD(keyPressed, (FALSE));
    SO_KIT_ADD_FIELD(filename, (""));
    SO_KIT_ADD_FIELD(play, ());
    SO_KIT_ADD_FIELD(pause, ());
    SO_KIT_ADD_FIELD(stop, ());
    SO_KIT_ADD_FIELD(inputPortList, (""));
    SO_KIT_ADD_FIELD(outputPortList, (""));
    SO_KIT_ADD_FIELD(inputPortSelected, (0));
    SO_KIT_ADD_FIELD(outputPortSelected, (0));
    SO_KIT_ADD_FIELD(inputPortPreferred, (0));
    SO_KIT_ADD_FIELD(outputPortPreferred, (0));
    
    SO_KIT_INIT_INSTANCE();

    // we haven't initialized the component yet
    initialized=false;

    // notification thread inactive
    processActive=false;

    // music not paused at the beginning
    paused=false;
    stopped=true;

    // init timestamp to 0
    timestamp=0;
    
    // init DirectMusic objects to NULL
    directMusic=NULL;
    directSound=NULL;
    performance=NULL;
    loader=NULL;
    segment=NULL;
    clock=NULL;
    inputPort=NULL;
    outputPort=NULL;
    portDownload=NULL;
    inputBuffer=NULL;
    outputBuffer=NULL;
    thru=NULL;

    // initialize input port
    ZeroMemory(&inputPortParams,sizeof(DMUS_PORTPARAMS8));
	inputPortParams.dwSize=sizeof(DMUS_PORTPARAMS8);
    
    // initialize output port
    ZeroMemory(&outputPortParams,sizeof(DMUS_PORTPARAMS8));
	outputPortParams.dwSize=sizeof(DMUS_PORTPARAMS8);

    //softwareSynth=false;

    // create field sensor
    midiOutEnabledSensor=new SoFieldSensor(midiOutEnabledCB,this);
    keyDownSensor=new SoFieldSensor(keyDownCB,this);
    playSensor=new SoFieldSensor(playCB,this);
    pauseSensor=new SoFieldSensor(pauseCB,this);
    stopSensor=new SoFieldSensor(stopCB,this);
    inputPortPreferredSensor=new SoFieldSensor(inputPortPreferredCB,this);
    outputPortPreferredSensor=new SoFieldSensor(outputPortPreferredCB,this);

    // set up field connections, etc.
    setUpConnections(TRUE,TRUE);
}

Piano::~Piano()
{
    delete midiOutEnabledSensor;
    delete keyDownSensor;
    delete playSensor;
    delete pauseSensor;
    delete stopSensor;
    delete inputPortPreferredSensor;
    delete outputPortPreferredSensor;

    sendMidiMsg(encodeMidiMsg(ALLNOTES_OFF,0,123,0),0);
    //unloadInstrument(Instrument);
	CloseHandle(hEvent);
    terminateInputNotification();
    deactivateMIDIThru(0,0,0);
    releasePort(inputPort);
    releasePort(outputPort);	

    // uninitialize COM
    CoUninitialize();
}

SbBool Piano::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        // attach field sensors
        midiOutEnabledSensor->attach(&midiOutEnabled);
        playSensor->attach(&play);
        pauseSensor->attach(&pause);
        stopSensor->attach(&stop);
        inputPortPreferredSensor->attach(&inputPortPreferred);
        outputPortPreferredSensor->attach(&outputPortPreferred);

        // init MIDI interface
        if (!initialized) {
            printf("Initializing DirectMusic objects ... ");
            initialized=initMIDI();
            printf("%s\n",initialized ? "OK" : "failed");
        }

        if (initialized) {
            // initial triggering of some callback functions
            midiOutEnabledCB(this,NULL);
            inputPortPreferredCB(this,NULL);
            outputPortPreferredCB(this,NULL);
        }
    }
    else {
        // detach field sensors
        midiOutEnabledSensor->detach();
        playSensor->detach();
        pauseSensor->detach();
        stopSensor->detach();
        inputPortPreferredSensor->detach();
        outputPortPreferredSensor->detach();
    }

    return !(connectionsSetUp = onOff);
}

void Piano::midiOutEnabledCB(void *data,SoSensor *sensor)
{
    Piano *piano=(Piano*)data;

    if (piano->midiOutEnabled.getValue()) {
        piano->deactivateMIDIThru(0,0,0);
        piano->keyDownSensor->attach(&piano->keyDown);
    }
    else {
        piano->setMIDIThru(0,0,0);
        piano->keyDownSensor->detach();
    }
}

void Piano::keyDownCB(void *data,SoSensor *sensor)
{
    Piano *piano=(Piano*)data;

    if (piano->timestamp<INT_MAX) piano->timestamp++;
    else piano->timestamp=0;
    for (int i=0;i<piano->keyDown.getNum();i++){
        piano->sendMidiMsg(piano->encodeMidiMsg(NOTE_ON,0,piano->keyDown[i]-1,60),0);
        piano->outputNoteList.append(piano->keyDown[i]-1);        
        piano->outputNoteTSList.append(piano->timestamp);
        piano->keyPressed=TRUE;
    }
    SoAlarmSensor *noteOffTimer=new SoAlarmSensor(piano->noteOffTimerCB,piano);
    noteOffTimer->setTimeFromNow(piano->keyDownTime.getValue());
    piano->noteOffTimerList.append(noteOffTimer);
    piano->noteOffTimerTSList.append(piano->timestamp);
    noteOffTimer->schedule();
}

void Piano::noteOffTimerCB(void *data,SoSensor *sensor)
{
    Piano *piano=(Piano*)data;

    for (int i=0;i<piano->noteOffTimerList.getLength();i++){
        if (piano->noteOffTimerList[i]==sensor) {
            int timestamp=piano->noteOffTimerTSList[i];
            int j=0;
            while (piano->outputNoteTSList.getLength()) {
                if (piano->outputNoteTSList[j]==timestamp){
                    piano->sendMidiMsg(piano->encodeMidiMsg(NOTE_OFF,0,piano->outputNoteList[j],60),0);
                    piano->outputNoteTSList.remove(j);
                    piano->outputNoteList.remove(j);
                }
                else j++;
            }
            piano->noteOffTimerTSList.remove(i);
            piano->noteOffTimerList.remove(i);
            break;
        }
    }
    if (!piano->noteOffTimerList.getLength()) piano->keyPressed=FALSE;
}

void Piano::playCB(void *data,SoSensor *sensor)
{
    Piano *piano=(Piano*)data;

    if (!piano->initialized) return;

    piano->paused=false;
    piano->stopped=false;

    DMUS_OBJECTDESC obj_desc;
	char directory[_MAX_PATH];
	WCHAR w_directory[_MAX_PATH];
	WCHAR w_filename[_MAX_PATH];

	if (GetCurrentDirectory(_MAX_PATH,directory)==0) return;
	MULTI_TO_WIDE(w_directory,directory);
	if (FAILED(piano->loader->SetSearchDirectory((REFGUID)GUID_DirectMusicAllTypes, w_directory, FALSE))) return;
	ZeroMemory (&obj_desc,sizeof(obj_desc));
	obj_desc.dwSize=sizeof(obj_desc);
	MULTI_TO_WIDE(w_filename,piano->filename.getValue().getString());
	obj_desc.guidClass = CLSID_DirectMusicSegment;
	wcscpy (obj_desc.wszFileName,w_filename);
	obj_desc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;
	if (
        SUCCEEDED(piano->loader->GetObject (&obj_desc,(REFIID)IID_IDirectMusicSegment,(LPVOID *)&(piano->segment))) &&
	    SUCCEEDED(piano->segment->SetParam((REFGUID)GUID_StandardMIDIFile,-1,0,0,(LPVOID)(piano->performance))) &&
        SUCCEEDED(piano->segment->SetParam((REFGUID)GUID_Download,-1,0,0,(LPVOID)(piano->performance))) &&
        SUCCEEDED(piano->performance->PlaySegmentEx(piano->segment,NULL,NULL,0,0,&piano->segmentState,NULL,NULL))
       ){
        piano->segmentState->QueryInterface(IID_IDirectMusicSegmentState,(LPVOID*)&(piano->segmentState));
    }
}

void Piano::pauseCB(void *data,SoSensor *sensor)
{
    Piano *piano=(Piano*)data;
    if (!piano->initialized || piano->stopped) return;

	MUSIC_TIME mtNow;

    if (!piano->paused){ // pause
        if (
            SUCCEEDED(piano->performance->Stop(NULL,NULL,0,0)) && 
            SUCCEEDED(piano->segmentState->GetSeek(&mtNow)) &&
            SUCCEEDED(piano->segment->SetStartPoint(mtNow))
            ){
            piano->paused=!piano->paused;
        }
    }
    else { // resume
        playCB(piano,NULL);
    }
}

void Piano::stopCB(void *data,SoSensor *sensor)
{
    Piano *piano=(Piano*)data;
    if (piano->initialized){
        piano->paused=false;
        piano->stopped=true;
        if (SUCCEEDED(piano->segment->SetStartPoint(0)) && SUCCEEDED(piano->performance->Stop(NULL,NULL,0,0))){
            piano->segment->SetStartPoint(0);
        }
    }
}

void Piano::inputPortPreferredCB(void *data,SoSensor *sensor)
{
    Piano *piano=(Piano*)data;

    if (!piano->initialized) return;

    printf("PIANO: selection of input port %d ... ",piano->inputPortPreferred.getValue());
	INFOPORT Info;
	if (piano->inputPortPreferred.getValue()!=piano->inputPortSelected.getValue()){
		if (
            piano->terminateInputNotification() &&
		    piano->deactivateMIDIThru(0,0,0) &&
		    piano->releasePort(piano->inputPort) &&
		    piano->getPortInfo(DMUS_PC_INPUTCLASS,piano->inputPortPreferred.getValue()+1,&Info) &&
            piano->activatePort(piano->inputPort,piano->inputPortParams,piano->inputBuffer,&Info,32) &&
		    piano->activateNotification() &&
		    piano->setMIDIThru(0,0,0)
           ){
            piano->inputPortSelected=piano->inputPortPreferred.getValue();
            printf("OK\n");
            return;
        }
    }    
    printf("failed\n");
}

void Piano::outputPortPreferredCB(void *data,SoSensor *sensor)
{
    Piano *piano=(Piano*)data;

    if (!piano->initialized) return;

    printf("PIANO: selection of output port %d ... ",piano->outputPortPreferred.getValue());
    INFOPORT Info;
    if (
        piano->deactivateMIDIThru(0,0,0) &&
        piano->releasePort(piano->outputPort) && 
        piano->getPortInfo(DMUS_PC_OUTPUTCLASS,piano->outputPortPreferred.getValue()+1,&Info)
       ){
        piano->setPortParams(piano->outputPortParams,Info.dwMaxVoices,Info.dwMaxAudioChannels,1,Info.dwEffectFlags,44100);
	    if (
            piano->activatePort(piano->outputPort,piano->outputPortParams,piano->outputBuffer,&Info,32) &&            
	        piano->setMIDIThru(0,0,0)
           ){
            piano->outputPortSelected=piano->outputPortPreferred.getValue();
            printf("OK\n");
            return;
        }
    }
    printf("failed\n");
}

bool Piano::initMIDI()
{
    // initialize COM
    CoInitialize(NULL);

    // init DirectMusic object
    if (FAILED(CoCreateInstance(CLSID_DirectMusic,NULL,CLSCTX_INPROC,IID_IDirectMusic8,(void**)&directMusic))) return false;		

    // set DirectSound object
	if (FAILED(directMusic->SetDirectSound(directSound,NULL))) return false;

    // set up a performance object
    if (FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,NULL,CLSCTX_INPROC,IID_IDirectMusicPerformance8,(void **)&performance))){
        return false;
    }
    // init performance object
    if (FAILED(performance->Init(NULL,NULL,0))) return false;

    // add default port to performance object
    if (FAILED(performance->AddPort(NULL))) return false;

    // init music loader object    
    if (FAILED(CoCreateInstance(CLSID_DirectMusicLoader,NULL,CLSCTX_INPROC,IID_IDirectMusicLoader8,(void **)&loader))){
        return false;
    }

    // enumerate ports
    DWORD dwNumInPorts=getNumPorts(DMUS_PC_INPUTCLASS);
    DWORD dwNumOutPorts=getNumPorts(DMUS_PC_OUTPUTCLASS);
    if (dwNumInPorts==-1 || dwNumOutPorts==-1) return false;

	DWORD dwCountInPorts=0;
    INFOPORT Info;
    DWORD nPortCount;
    bool selected=false;

	// list all output ports
    while (outputPortData.getLength()) outputPortData.remove(0); // reset output port data
	for (nPortCount=1;nPortCount<=dwNumOutPorts;nPortCount++){
		getPortInfo(DMUS_PC_OUTPUTCLASS,nPortCount,&Info);
		outputPortList.set1Value(nPortCount-1,Info.szPortDescription);
        DWORD *data=new DWORD; *data=Info.dwFlags;
		outputPortData.append(data);

        if (!selected) {
			Info.dwMaxChannelGroups=1;
			setPortParams(outputPortParams,Info.dwMaxVoices,Info.dwMaxAudioChannels,1,Info.dwEffectFlags,44100);
			if (!activatePort(outputPort,outputPortParams,outputBuffer,&Info,32)) return false;
			outputPortSelected.setValue(nPortCount-1);
			if ((Info.dwFlags & DMUS_PC_DLS) || (Info.dwFlags & DMUS_PC_DLS2)){	
				//softwareSynth=true; // we are working with a software synthesizer
            }
            selected=true;
		}
	}
    // set text field data
    outputPortList.setNum(dwNumOutPorts);

	// list all input ports
    selected=false;
	for (nPortCount=1;nPortCount<=dwNumInPorts;nPortCount++){
		getPortInfo(DMUS_PC_INPUTCLASS,nPortCount,&Info);
		if (Info.dwType != DMUS_PORT_KERNEL_MODE){
            inputPortList.set1Value(nPortCount-1,Info.szPortDescription);
            if (!selected) {
				if (!activatePort(inputPort,inputPortParams,inputBuffer,&Info,32)) return false;
                inputPortSelected.setValue(nPortCount-1);
                selected=true;
            }
			dwCountInPorts++;
		}
	}
    // set text field data
    inputPortList.setNum(dwNumInPorts);

    while (keyDownList.getLength()) keyDownList.remove(0);

    /// activate MIDI message notification
    activateNotification();

    /// activate the MIDI thru with the default output port
    setMIDIThru(0,0,0);

    // everything was successful, return true
    return true;
}

DWORD Piano::getNumPorts(DWORD portType)
{
    DMUS_PORTCAPS portinf;
	DWORD dwIndex=0,dwNum=0;
	HRESULT hr=-1;	
    ZeroMemory(&portinf,sizeof(portinf));
	portinf.dwSize = sizeof(DMUS_PORTCAPS);
    while ((hr=directMusic->EnumPort(dwIndex++,&portinf))==S_OK){
		if (portinf.dwClass==portType) dwNum++;
    }
	if ((hr!=S_FALSE) && (hr!=S_OK)) return -1;
	return dwNum;
}

bool Piano::getPortInfo(DWORD portType,DWORD dwNumPort,LPINFOPORT lpInfoPort)
{
    HRESULT hr=-1;
	DMUS_PORTCAPS portinf;
	DWORD dwIndex=0,dwNum=0;
	
	ZeroMemory(&portinf,sizeof(portinf));
	portinf.dwSize=sizeof(DMUS_PORTCAPS);
    
    while((hr=directMusic->EnumPort(dwIndex++,&portinf))==S_OK){	
		if (portinf.dwClass==portType) dwNum++;
		if (dwNum==dwNumPort) break;
    }
	
	if ((hr!=S_FALSE) && (hr!=S_OK)) return false;

	if (SUCCEEDED(hr)){
		ZeroMemory(lpInfoPort,sizeof(INFOPORT));
		CopyMemory(&(lpInfoPort->guidSynthGUID),&portinf.guidPort,sizeof(GUID));
		#ifdef _UNICODE
			_tcscpy(lpInfoPort->szPortDescription,portinf.wszDescription);
		#else
			wcstombs(lpInfoPort->szPortDescription,portinf.wszDescription,DMUS_MAX_DESCRIPTION);
		#endif
		lpInfoPort->dwClass					= portinf.dwClass;
		lpInfoPort->dwEffectFlags			= portinf.dwEffectFlags;
		lpInfoPort->dwFlags	 				= portinf.dwFlags;
		lpInfoPort->dwMemorySize			= portinf.dwMemorySize;
		lpInfoPort->dwMaxAudioChannels	    = portinf.dwMaxAudioChannels;
		lpInfoPort->dwMaxChannelGroups		= portinf.dwMaxChannelGroups;
		lpInfoPort->dwMaxVoices				= portinf.dwMaxVoices;
		lpInfoPort->dwType					= portinf.dwType;
    }
	
	return true;
}

void Piano::setPortParams(DMUS_PORTPARAMS8 &portParams,DWORD dwVoices,DWORD dwAudioChannels,DWORD dwChannelGroups,
                          DWORD dwEffectFlags,DWORD dwSampleRate)
{
	portParams.dwValidParams=0;

	if (dwVoices)		 portParams.dwValidParams  = DMUS_PORTPARAMS_VOICES;
	if (dwAudioChannels) portParams.dwValidParams |= DMUS_PORTPARAMS_AUDIOCHANNELS;
	if (dwChannelGroups) portParams.dwValidParams |= DMUS_PORTPARAMS_CHANNELGROUPS;
	if (dwEffectFlags)   portParams.dwValidParams |= DMUS_PORTPARAMS_EFFECTS; 
	if (dwSampleRate)    portParams.dwValidParams |= DMUS_PORTPARAMS_SAMPLERATE;
	
	portParams.dwVoices		 = dwVoices;
	portParams.dwAudioChannels = dwAudioChannels;
	portParams.dwChannelGroups = dwChannelGroups;
	portParams.dwEffectFlags   = dwEffectFlags;
	portParams.dwSampleRate    = dwSampleRate;
}

bool Piano::activatePort(IDirectMusicPort8 *&port,DMUS_PORTPARAMS8 &portParams,IDirectMusicBuffer8 *&buffer,LPINFOPORT infoPort,DWORD dwSysExSize)
{
    if (port==outputPort && clock) return false;
    if (FAILED(directMusic->CreatePort(infoPort->guidSynthGUID,&portParams,&port,NULL))) {
        return false;
    }
	if (FAILED(port->Activate(TRUE))) return false;

	DMUS_BUFFERDESC BufferDesc;
	ZeroMemory(&BufferDesc,sizeof(DMUS_BUFFERDESC));
	BufferDesc.dwSize=sizeof(DMUS_BUFFERDESC);
	BufferDesc.guidBufferFormat=GUID_NULL;
	BufferDesc.cbBuffer=DMUS_EVENT_SIZE(dwSysExSize);
	
	if (FAILED(directMusic->CreateMusicBuffer(&BufferDesc,&buffer,NULL))) return false;

    if (port==inputPort){
        if (FAILED(port->QueryInterface(IID_IDirectMusicThru8,(void**)&thru))) return false;
    }
    else {
	    if (FAILED(port->GetLatencyClock(&clock))) return false;	
	    port->QueryInterface(IID_IDirectMusicPortDownload8,(void**)&portDownload);
    }
	
	return true;
}

bool Piano::releasePort(IDirectMusicPort8 *&port)
{
    if (port){
        if (FAILED(port->Activate(FALSE))) return false;
        if (port==inputPort){
            if (inputPort) { 
                inputPort->Release();
                inputPort=NULL; 
            }
            if (inputBuffer) { 
                inputBuffer->Release(); 
                inputBuffer=NULL; 
            }
            if (thru) { 
                thru->Release(); 
                thru=NULL; 
            }
        }
        else {
            if (outputPort) { 
                outputPort->Release();
                outputPort=NULL; 
            }
            if (outputBuffer) { 
                outputBuffer->Release(); 
                outputBuffer=NULL; 
            }
            if (clock) { 
                clock->Release(); 
                clock=NULL; 
            }
            if (portDownload) { 
                portDownload->Release(); 
                portDownload=NULL; 
            }
        }
    }

    return true;
}

bool Piano::setMIDIThru(DWORD dwSourceChannel,DWORD dwDestinationChannelGroup,DWORD dwDestinationChannel)
{
	HRESULT hr=-1;
	
	if (thru){
        if (FAILED(thru->ThruChannel(1,dwSourceChannel,dwDestinationChannelGroup,dwDestinationChannel,outputPort))){
            return false;
        }
		
		VECTOR chvect(3);
		chvect[0] = static_cast<WORD>(dwSourceChannel);
		chvect[1] = static_cast<WORD>(dwDestinationChannelGroup);
		chvect[2] = static_cast<WORD>(dwDestinationChannel);

		if (channelList.end()==find(channelList.begin(),channelList.end(),chvect)) channelList.insert(channelList.end(),chvect);
	
	}
    else return false;
	
	return true;
}

bool Piano::deactivateMIDIThru(DWORD dwSourceChannel,DWORD dwDestinationChannelGroup,DWORD dwDestinationChannel)
{
	if (thru){		
		if (FAILED(thru->ThruChannel(1,dwSourceChannel,dwDestinationChannelGroup,dwDestinationChannel,NULL))) return false;	
		VECTOR chvect(3);
		chvect[0]=static_cast<WORD>(dwSourceChannel);
		chvect[1]=static_cast<WORD>(dwDestinationChannelGroup);
		chvect[2]=static_cast<WORD>(dwDestinationChannel);
		if (channelList.end()!=find(channelList.begin(),channelList.end(),chvect)) channelList.remove(chvect);
        return true;
    }
    else return false;
}    

bool Piano::activateNotification()
{
	HRESULT hr=-1;
	DWORD dwThId;

	if (hEvent=CreateEvent(NULL,FALSE,FALSE,NULL)){	
		if (inputPort){
			if (FAILED(inputPort->SetReadNotificationHandle(hEvent))) return false;
			if ((hTerminationEvent=CreateEvent(NULL,FALSE,FALSE,NULL))==NULL) return false;
			processActive=TRUE;
			if ((hThread=CreateThread(NULL,0,waitForEvent,this,0,&dwThId))==NULL) return false;
        }
        else return false;
    }
    else return false;
	
	return true;
} 

bool Piano::terminateInputNotification()
{
	if (inputPort){
		if (FAILED(inputPort->SetReadNotificationHandle(NULL))) return false;
		processActive=FALSE;
		SetEvent(hEvent);	 
		CloseHandle(hEvent);
		hEvent=NULL;
		WaitForSingleObject(hTerminationEvent,THREAD_TIMEOUT);
		CloseHandle(hTerminationEvent);
		hTerminationEvent=NULL;
    }
	return true;
}

DWORD Piano::waitForEvent(LPVOID lpv)
{
    REFERENCE_TIME rt;	// System time when the event is received
    DWORD dwGroup;		// Channel group
    DWORD dwcb,dwMsg;	// Byte counter, Midi message	
    BYTE *pb;			// Pointer to the data
		
	Piano *piano=(Piano*)(lpv);
	
	while (piano->processActive){
		WaitForMultipleObjects(1,&piano->hEvent,FALSE,1000);
		while (piano->processActive){
            if (piano->inputPort) {
                if (piano->inputPort->Read(piano->inputBuffer)==S_FALSE) break;
            }
			if (piano->inputBuffer) piano->inputBuffer->ResetReadPtr();
			while (piano->processActive){
                if (piano->inputBuffer) {
                    if (piano->inputBuffer->GetNextEvent(&rt,&dwGroup,&dwcb,&pb)==S_FALSE) break;
                    else {
                        // SysEx data
                        if (dwcb>MIDI_STRUCTURED) {
                            piano->recvMidiMsg(rt,dwGroup,dwcb,pb);
                        }
                        else { // structured data (standard format)
						    CopyMemory(&dwMsg,pb,dwcb);
						    piano->recvMidiMsg(rt,dwGroup,dwMsg);
                        }
                    }
                }
                else break;
            }				
        }
    }
	SetEvent(piano->hTerminationEvent);
	return 0;
}

bool Piano::sendMidiMsg(LPBYTE lpMsg,DWORD dwLength,DWORD dwChannelGroup)
{
	REFERENCE_TIME rt;

	if (FAILED(clock->GetTime(&rt))) return false;
	if (FAILED(outputBuffer->PackUnstructured(rt,dwChannelGroup,dwLength,lpMsg))) return false;
	if (FAILED(outputPort->PlayBuffer(outputBuffer))) return false;
	if (FAILED(outputBuffer->Flush())) return false;
	
	return true;
}	

bool Piano::sendMidiMsg(DWORD dwMsg,DWORD dwChannelGroup)
{
	REFERENCE_TIME rt;

	if (FAILED(clock->GetTime(&rt))) return false;
	if (FAILED(outputBuffer->PackStructured(rt,dwChannelGroup,dwMsg))) return false;
	if (FAILED(outputPort->PlayBuffer(outputBuffer))) return false;
	if (FAILED(outputBuffer->Flush())) return false;

	return true;
}

void Piano::recvMidiMsg(REFERENCE_TIME rt,DWORD dwChannel,DWORD dwBytesRead,BYTE *lpBuffer)
{
    // !!!
    printf("extended MIDI message\n");
}

void Piano::recvMidiMsg(REFERENCE_TIME lprt,DWORD dwChannel,DWORD dwMsg)
{
	BYTE Command,Channel,Note,Velocity;
	
	decodeMidiMsg(dwMsg,&Command,&Channel,&Note,&Velocity);
    printf("standard MIDI message --> command = %d, note = %d\n",Command,Note);
    int i;
	if ((Command == NOTE_ON) && (Velocity > 0)){
        for (i=0;i<keyDownList.getLength();i++){
            if (keyDownList[i]==Note) break;
        }
        if (i==keyDown.getNum()) keyDownList.append(Note);
	} 
    else if ((Command == NOTE_OFF) || ((Command == NOTE_ON) && (Velocity == 0))){
        for (i=0;i<keyDownList.getLength();i++){
            if (keyDownList[i]==Note) break;
        }
        if (i!=keyDown.getNum()) keyDownList.remove(i);
    }
    for (i=0;i<keyDownList.getLength();i++){
        keyDown.set1Value(i,keyDownList[i]);
    }
    keyDown.setNum(keyDownList.getLength());
    keyPressed=(keyDownList.getLength()>0);
}

DWORD Piano::encodeMidiMsg(BYTE Status,BYTE DataByte1,BYTE DataByte2)
{
	DWORD dwMsg;
	dwMsg = Status;
    dwMsg |= DataByte1 << SHIFT_8;
    dwMsg |= DataByte2 << SHIFT_16;
	return dwMsg;
}

DWORD Piano::encodeMidiMsg(BYTE Command,BYTE Channel,BYTE DataByte1,BYTE DataByte2)
{
    DWORD dwMsg;
	dwMsg = Command | Channel;
    dwMsg |= DataByte1 << SHIFT_8;
    dwMsg |= DataByte2 << SHIFT_16;
	return dwMsg;
}

void Piano::decodeMidiMsg(DWORD dwMsg,BYTE *Status,BYTE *DataByte1,BYTE *DataByte2)
{
	*Status = static_cast<BYTE>(dwMsg);
    *DataByte1 = static_cast<BYTE>(dwMsg >> SHIFT_8);
    *DataByte2 = static_cast<BYTE>(dwMsg >> SHIFT_16);
}

void Piano::decodeMidiMsg(DWORD dwMsg,BYTE *Command,BYTE *Channel,BYTE *DataByte1,BYTE *DataByte2)
{
    *Command = static_cast<BYTE>(dwMsg & ~BITMASK);
    *Channel = static_cast<BYTE>(dwMsg & BITMASK);
    *DataByte1 = static_cast<BYTE>(dwMsg >> SHIFT_8);
    *DataByte2 = static_cast<BYTE>(dwMsg >> SHIFT_16);
}

