/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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
  * PROJECT: Studierstube
  * ======================================================================== */

 /* ======================================================================= */
/** Header file for API definitions for the SoSpeechOutput Node.
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoSpeechOutput.h 4032 2004-11-23 11:04:12Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoSpeechOutput_H
#define SoSpeechOutput_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoSubNode.h>

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoMFString.h>

#include <Inventor/sensors/SoFieldSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif


#include "StbSpeech.h"

#if !defined(USE_SAPI_SPEECHIMPL) && !defined(USE_EMPTY_SPEECHIMPL)
	#ifdef _WIN32
		#define USE_SAPI_SPEECHIMPL
	#endif

	#ifndef _WIN32
		#define USE_EMPTY_SPEECHIMPL
	#endif
#endif

#if defined(USE_SAPI_SPEECHIMPL)
	#include "SAPISpeechImpl.h"
#elif defined(USE_EMPTY_SPEECHIMPL)
	#include "EmptySpeechImpl.h"
#endif

/**
SoSpeechOutput is a SoNode that allows to output spoken text. It uses speech output implementations that forfill this task. Currently only a 
Windows implementation (based on MS SAPI 5.1) and an empty (just text output) implementation are availiable.

File Format / defaults:

@verbatim
SoSpeechOutput {
	SoSFString	voiceName		"" # name of the voice that is beeing used for output generation try "Mircosoft Mary"
	SoSFString	gender			"" # name of the gender that is searched for (see SAPI doc) eg "Male"
	SoSFString	language		"" # language that is searched for (see SAPI doc) eg "409"
	
	SoSFInt32	currentViseme	 -1    # is set by the speech implementation, information is based on MS SAPI visemes
	SoSFInt32	nextViseme		 -1    # is set by the speech implementation, information is based on MS SAPI visemes
	SoSFFloat	duration		 -1    # is set by the speech implementation, information is based on MS SAPI duration
	SoSFInt32	rate			 -1    # is set by the speech implementation, information is based on MS SAPI rate
	SoSFEnum	eventType		NONE   # is set by the speech implementation, to the type of the event that caused the trigger 
	SoSFTrigger eventTrigger 		   # is fired when the information is updated due to some speech implementation callbacks

    SoSFInt32	speechPosition	-1      # is set by the implementation, is set to the current position of the text (while speaking) refers to the character in the text
	SoSFInt32	textLength		-1      # is set by the implementation, is set to the length of the text (without annotation)
	SoSFBool	isActive		FALSE	# is set by the implementation, is true as long as the text to speech is speaking

	SoSFBool	startOnSetText	TRUE	# if true the speech output starts immediatly after text has been set
	SoSFBool	restartOnStart  FALSE   # if true the current speech (if in progress) is stopped before the next speech is outputted, else the speech output will be quequed
	
	  // TODO
#    SoMFString	pureText			""		# text feasible for use it as text output (just an output field)
	
	SoMFString	text				""		# speech text that can have some annotation to set pitch, rate etc. (see SAPI doc)

	SoSFTrigger	start					# if triggered: the output starts 
	SoSFTrigger	stop					# if triggered: the output stops immediatly
	SoSFTrigger	stopped					# is fired when speech output stops (the same as eventType==END_STREAM, eventTrigger)
  }
}
@endverbatim

An example of a working (under windows) SoSpeechOutput is:

  SoSpeechOutput {
	voiceName "Mircosoft Mary"
	text "Hello. The S.O. Speech Output is working !"
	}

See:
  @ingroup speech
 */

class STBSPEECH_API SoSpeechOutput : public SoNode
{
    SO_NODE_HEADER(SoSpeechOutput);

public:

	enum EventType {
		NONE = -1,
		VISEME,
		WORD_BOUNDARY,
		SENTENCE_BOUNDARY,
		START_STREAM,
		END_STREAM
	};

	SoSFString	voiceName;			///< name of the voice that is beeing used for output generation
	SoSFString	gender;				///< name of the voice that is beeing used for output generation
	SoSFString	language;			///< name of the voice that is beeing used for output generation
	
	// *** lip synch
	SoSFInt32	currentViseme;			///< is set by the implementation, information is based on MS SAPI visemes
	SoSFInt32	nextViseme;				///< is set by the implementation, information is based on MS SAPI visemes
	SoSFFloat	duration;				///< is set by the implementation, information is based on MS SAPI duration
	SoSFInt32	rate;					///< is set by the implementation, information is based on MS SAPI rate
	SoSFEnum	eventType;				///< is set by the implementation, to the type of the event that caused the trigger
	SoSFTrigger eventTrigger;			///< is fired if the information is updated due to some speech implementation callbacks

    // *** speech position
	SoSFInt32	speechPosition;		///< is set to the current position of the text (while speaking) refers to the character in the text
	SoSFInt32	textLength;			///< is set to the length of the text (without annotation)
	SoSFBool	isActive;			///< is true as long as the text-to-speech is speaking or the sound file is played

	SoSFTrigger	start;				///< if triggered the output restarts
	SoSFTrigger	stop;				///< if triggered the output stops
	SoSFTrigger	stopped;			///< is fired when speech output stops
	
	SoSFBool	startOnSetText;		///< if true the speech output starts after text has been set
	SoSFBool	restartOnStart;		///< if true the current speech is stopped start

	SoMFString	text;				///< speech text that can have some annotation to set ....
	SoSFString	soundFile;			///< a sound file to play - please do not use this field to replace a sound output node, but only to mix in 
									///  sounds that can not be reproduced by the text-to-speech engines eg. laughing ....
	
    /// static class initialization method of OpenInventor
	static void initClass();

    /// Constructor
	SoSpeechOutput();		

protected:
	virtual ~SoSpeechOutput();			///<

#if defined(USE_SAPI_SPEECHIMPL)
	SAPISpeechImpl *speechImpl;
#elif defined(USE_EMPTY_SPEECHIMPL)
	EmptySpeechImpl *speechImpl;
#endif

    /// variable containing the concatenated text element from the 'text' multiple string field
    SbString concatText;

	SoFieldSensor startSensor;		   ///< sensor that is fired if SoSpeechOutput::stop is changed calls SoSpeechOutput::Speak
	SoFieldSensor stopSensor;		   ///< sensor that is fired if SoSpeechOutput::start is changed calls SoSpeechOutput::ShutUp
	SoFieldSensor textSensor;		   ///< sensor that is fired if SoSpeechOutput::text is changed calls SoSpeechOutput::
	SoFieldSensor voiceNameSensor;	   ///< sensor that is fired if SoSpeechOutput::voiceName is changed calls SoSpeechOutput::
	SoFieldSensor voiceGenderSensor;
	SoFieldSensor voiceLanguageSensor;

	void speakText();				///< helper function that calls speechImpl->speak to output all the text within the SoMFString field

	int calledCounter;
protected:

	static void startTriggerCB(void *data, SoSensor *sensor);		///<
	static void stopTriggerCB(void *data, SoSensor *sensor);		///<
	static void textCB(void *data, SoSensor *sensor);				///<
	static void voiceNameCB(void *data, SoSensor *sensor);			///<
};

//----------------------------------------------------------------------------

#endif // SoPucServer_H