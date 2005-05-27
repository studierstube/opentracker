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
/** header file for SoFMODSound extension node
 *
 *
 * @author Flo Ledermann ledermann@ims.tuwien.ac.at
 *
 * $Id: SoFMODSound.h 4202 2005-05-11 14:42:42Z flo $
 * @file                                                                   */
/* ======================================================================= */

#ifndef  _SO_FMODSOUND_
#define  _SO_FMODSOUND_

#include "FMODSoundExt.h"
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <fmod.h>

/**
 * A Node for playing sounds. This depends on the fmod library (http://www.fmod.org), which
 * supports everything we need for playing sound. Various file formats are supported (wav, aiff, mp3, ogg),
 * which can be streamed (loaded from disk on demand) or loaded as samples (loaded into memory upon
 * initialization. Samples (not streams) can be looped, and the playback volume can be adjusted for each sound.
 *
 * @todo add full cd-player style controls like pause and seekPosition
 * @bug streaming does not really work (at least on my machine)
 *
 * @author Flo Ledermann ledermann@ims.tuwien.ac.at
 * @ingroup media
 */
class FMODSOUND_API SoFMODSound : public SoNode {
    
    /// File name to load.
    SoSFString filename;
    
    /// If set to true, the playback starts. If set to false, playback stops and will resume at the start position.
    SoSFBool play;
    /// Whether the sample should be looped. Ignored if in streaming mode.
    SoSFBool loop;
    /**
    If the sound is streamed, it is not loaded into memory at startup but streamed from the
    media. This requires more processing power, but less memory. To be used for longer sounds. 
    */
    SoSFBool stream;
    
    /// Playback volume, normalized from 0..1
    SoSFFloat volume;

	/// elapsed time in seconds
	SoSFFloat time;

	/// Pitch, default is 1.0 (negative values play backwards)
	SoSFFloat pitch;
    
    SO_NODE_HEADER(SoFMODSound);
        
public:
    
    /// callback for the fields that can be changed interactively.
    static void fieldChangedCB(void* data, SoSensor* which);

    /// Instance callback
	static void timeChange(void *data, SoSensor *);

    // Constructor
    SoFMODSound();	

	/// initialization method for OpenInventor type system.
    static void		initClass();

	static const int SAMPLE_RATE = 44100;

protected:

    /// Instance callback, called from the static fieldChangedCB() function.
    void update(SoSensor * which);


    /// Sensor attached to the filename field.
   	SoFieldSensor filenameSensor;
    /// Sensor attached to the play field.
	SoFieldSensor playSensor;
    /// Sensor attached to the volume field.
	SoFieldSensor volumeSensor;
    /// Sensor attached to the pitch field.
	SoFieldSensor pitchSensor;

	SoTimerSensor* timeSensor;

    /// The currently loaded sample.
    FSOUND_SAMPLE * currentSample;
    /// The currently loaded stream.
    FSOUND_STREAM * currentStream;

    /** The channel that is currently used for playback. Every SoFMODSound instance uses it's own
    channel, acquired dynamically when it starts playing out of the available channels on the system.*/
    int currentChannel;

    /// Start playback.
    void startPlay();
    /// Stop playback.
    void stopPlay();
    /// Load the media.
    void load();
    /// Unload the media and free resources.
    void unload();

    virtual ~SoFMODSound();    
};

#endif /* _SO_FMODSOUND_ */
