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
/** Header file for API definitions for the SoEmotionaliseSpeechText Node.
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoEmotionaliseSpeechText.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoEmotionaliseSpeechText_H
#define SoEmotionaliseSpeechText_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoSubNode.h>

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFEnum.h>

#include <Inventor/sensors/SoFieldSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif


#include "StbSpeech.h"

/**
SoEmotionaliseSpeechText is a SoNode that allows to emotionalise the output of spoken text. It uses the speech annotation tags to change the way the speech output is generated. 
This means that you can generate sad, happy etc. speech output. This functionality was implemented in this class to result in a simpler interface fo the SoSpeechOutput (which sould
be used to generate the actual speech output).

@verbatim
    //         neutral happiness surprise anger disgust fear sadness 
	// speed    0       1         0        1     0       2   -2      
	// volume   0       2         3        2     0      -1   -2      
	// pitch    0       2         2       -1     0       3   -2      
@endverbatim


File Format / defaults:

@verbatim
SoEmotionaliseSpeechText {
    SoSFString	input     # neutral text that should be annotated
    SoSFString	output    # annotated text 

	SoSFEnum	emotion	  # emotions : NEUTRAL,HAPPINESS,SURPRISE,ANGER,DISGUST,FEAR,SADNESS 
	SoSFFloat	intensity # intensity of emotion [0..1] , if it is 0 the result will be the same as neutral 
  }
}
@endverbatim

See:

  @ingroup speech
 */

class STBSPEECH_API SoEmotionaliseSpeechText : public SoNode
{
    SO_NODE_HEADER(SoEmotionaliseSpeechText);

public:

	enum Emotion {
		NONE = 0,
		NEUTRAL,
		HAPPINESS,
		SURPRISE,
		ANGER,
		DISGUST,
		FEAR,
		SADNESS,
		NOTUSED
	};

	SoSFString	input;			///< name of the voice that is beeing used for output generation
	SoSFString	output;				///< name of the voice that is beeing used for output generation
	
	SoSFEnum	emotion;				///< is set by the implementation, to the type of the event that caused the trigger
	SoSFInt32	emotionInt;				///< is set by the implementation, to the type of the event that caused the trigger
	SoSFFloat	intensity;			///< clean ascii without the text-to-speech tags 
	
    /// static class initialization method of OpenInventor
	static void initClass();

    /// Constructor
	SoEmotionaliseSpeechText();		

protected:
	virtual ~SoEmotionaliseSpeechText();				///<


	SoFieldSensor textSensor;		///< sensor that is fired if SoEmotionaliseSpeechText::text is changed calls SoEmotionaliseSpeechText::
	SoFieldSensor intensitySensor;
	SoFieldSensor emotionSensor;
	SoFieldSensor emotionIntSensor;

	static void textCB(void *data, SoSensor *sensor);

	static float rate[7];
	static float volume[7];
	static float pitch[7];

};

//----------------------------------------------------------------------------

#endif // SoPucServer_H