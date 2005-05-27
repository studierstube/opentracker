       /* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: stbapi
 * ======================================================================== */
/** source file for SoFMODSound extension node
 *
 * @author Flo Ledermann ledermann@ims.tuwien.ac.at
 *
 * $Id: SoFMODSound.cxx 4202 2005-05-11 14:42:42Z flo $
 * @file                                                                   */
/* ======================================================================= */

#include "SoFMODSound.h"
#include <iostream>

SO_NODE_SOURCE(SoFMODSound);

void SoFMODSound::initClass()
{
    SO_NODE_INIT_CLASS(SoFMODSound, SoNode,"Node");

    FSOUND_Init(44100, 32, 0);
}

SoFMODSound::SoFMODSound()
{   
	SO_NODE_CONSTRUCTOR(SoFMODSound);

	SO_NODE_ADD_FIELD(play, (false));
	SO_NODE_ADD_FIELD(loop, (false));
	SO_NODE_ADD_FIELD(stream, (false));
	
	SO_NODE_ADD_FIELD(volume, (1.0));
	SO_NODE_ADD_FIELD(pitch, (1.0));
	
	SO_NODE_ADD_FIELD(filename, (""));

	filenameSensor.setFunction( SoFMODSound::fieldChangedCB );
	filenameSensor.setData( this );
    filenameSensor.attach(&filename);

	playSensor.setFunction( SoFMODSound::fieldChangedCB );
	playSensor.setData( this );
    playSensor.attach(&play);

	volumeSensor.setFunction( SoFMODSound::fieldChangedCB );
	volumeSensor.setData( this );
    volumeSensor.attach(&volume);

	pitchSensor.setFunction( SoFMODSound::fieldChangedCB );
	pitchSensor.setData( this );
    pitchSensor.attach(&pitch);

	timeSensor = new SoTimerSensor(timeChange, this);
	timeSensor->setInterval(0.1);
	timeSensor->schedule();
	

    currentSample = NULL;
    currentStream = NULL;
    currentChannel = -1;

	SO_NODE_ADD_FIELD(time, (0.0));
}


	
SoFMODSound::~SoFMODSound(){

    unload();
	
}

void SoFMODSound::timeChange(void* data, SoSensor *){
	try {
		SoFMODSound* sound = (SoFMODSound*)data;
		if (sound->currentChannel > -1){
			sound->time = ((float)FSOUND_GetCurrentPosition(sound->currentChannel))/SAMPLE_RATE;
		//	printf("in timeChange");
		}
		//std::cout << "FMODSound: " << sound->currentChannel << "\n";
	} catch(...) { std::cout << "FMODSound::timeChange error\n"; }	
}

void SoFMODSound::fieldChangedCB(void* data, SoSensor* which) {
    SoFMODSound * self = (SoFMODSound *)data;

    self->update(which);
}

void SoFMODSound::update(SoSensor * which) {

    if (which == &volumeSensor) {
        if (currentChannel > -1) {
            FSOUND_SetVolume( currentChannel, (int)(volume.getValue() * 255));
        }
    }
    else if (which == &pitchSensor) {
        if (currentChannel > -1) {
            FSOUND_SetFrequency( currentChannel, (int)(pitch.getValue() * SAMPLE_RATE));
			printf("FMOD: pitch: %d\n",((int)(pitch.getValue() * SAMPLE_RATE)));
        }
    }
    else if (which == &playSensor) {
        if (play.getValue() == FALSE) {
            stopPlay();
        }
        else {
            startPlay();
        }
    }
    else if (which == &filenameSensor){
        if (currentChannel > -1) {
            unload();
        }
        load();
        if (play.getValue() == FALSE) {
            stopPlay();
        }
        else {
            startPlay();
        }
    }
}

void SoFMODSound::startPlay() {
    if (currentChannel == -1) {
        if ((stream.getValue() && currentStream == NULL) || ( !stream.getValue() && currentSample == NULL)) {
            load();
        }

        if (currentSample != NULL) {
            currentChannel = FSOUND_PlaySound(FSOUND_FREE, currentSample);
            if (currentChannel == -1) {
                printf("SoFMODSound: couldn't play sample '%s'\n",filename.getValue().getString());
                return;
            }
        }
        else if (currentStream != NULL) {
            currentChannel = FSOUND_Stream_Play(FSOUND_FREE, currentStream);
            if (currentChannel == -1) {
                printf("SoFMODSound: couldn't play stream '%s'\n",filename.getValue().getString());
                return;
            }
        }

        if (currentChannel > -1) {
            if (loop.getValue()) {
                FSOUND_SetLoopMode( currentChannel, FSOUND_LOOP_NORMAL);
            }
            else {
                FSOUND_SetLoopMode( currentChannel, FSOUND_LOOP_OFF);
            }

            FSOUND_SetVolume( currentChannel, (int)(volume.getValue() * 255));
            FSOUND_SetFrequency( currentChannel, (int)(pitch.getValue() * SAMPLE_RATE));

            printf("SoFMODSound: started to play at volume %d\n",(int)(volume.getValue() * 255));
    
        }
    }
}

void SoFMODSound::stopPlay() {
    if (currentChannel > -1) {
        FSOUND_StopSound(currentChannel);
        currentChannel = -1;

        printf("SoFMODSound: stopped playing\n");
    }

}

void SoFMODSound::load() {
    if (!stream.getValue()) {

        currentSample = FSOUND_Sample_Load(FSOUND_FREE, filename.getValue().getString(), FSOUND_NORMAL, 0, 0);
        if (currentSample == NULL) {
            printf("SoFMODSound: couldn't load sample '%s'\n",filename.getValue().getString());
            return;
        }
        currentStream =  NULL;
        printf("SoFMODSound: loaded sample '%s'\n",filename.getValue().getString());
    }
    else {
        currentStream = FSOUND_Stream_Open(filename.getValue().getString(), FSOUND_NORMAL, 0, 0);
        if (currentStream == NULL) {
            printf("SoFMODSound: couldn't load stream '%s'\n",filename.getValue().getString());
            return;
        }
        currentSample = NULL;
        printf("SoFMODSound: loaded stream '%s'\n",filename.getValue().getString());

    }
}

void SoFMODSound::unload() {
    stopPlay();

    if (currentSample != NULL) {
        FSOUND_Sample_Free(currentSample);
        currentSample = NULL;
    }

    if (currentStream != NULL) {
        FSOUND_Stream_Close(currentStream);
        currentStream = NULL;
    }

    printf("SoFMODSound: unloaded.\n");

}

