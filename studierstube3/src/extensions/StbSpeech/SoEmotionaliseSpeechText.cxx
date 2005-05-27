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
/** Source file defines the SoPucServer class which is also the entry point of the
  * dll
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoEmotionaliseSpeechText.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoEmotionaliseSpeechText.h"
#include <iostream>

using namespace std;

float SoEmotionaliseSpeechText::rate[7]={  0,0,1,2,1,0,2};
float SoEmotionaliseSpeechText::volume[7]={0,0,2,3,2,0,-1};
float SoEmotionaliseSpeechText::pitch[7]={ 0,0,2,2,-1,0,3};

//----------------------------------------------------------------------------

SO_NODE_SOURCE(SoEmotionaliseSpeechText);

//----------------------------------------------------------------------------

void SoEmotionaliseSpeechText::initClass()
{
	SO_NODE_INIT_CLASS(SoEmotionaliseSpeechText, SoNode, "SoNode");
}

//----------------------------------------------------------------------------

SoEmotionaliseSpeechText::SoEmotionaliseSpeechText()
{
	SO_NODE_CONSTRUCTOR(SoEmotionaliseSpeechText);

	SO_NODE_ADD_FIELD(input,		(""));				
	SO_NODE_ADD_FIELD(output,		(""));
	SO_NODE_ADD_FIELD(intensity,	(0.0));

	SO_NODE_DEFINE_ENUM_VALUE(Emotion, NONE);
	SO_NODE_DEFINE_ENUM_VALUE(Emotion, NEUTRAL);
	SO_NODE_DEFINE_ENUM_VALUE(Emotion, HAPPINESS);
	SO_NODE_DEFINE_ENUM_VALUE(Emotion, SURPRISE);
	SO_NODE_DEFINE_ENUM_VALUE(Emotion, ANGER);
	SO_NODE_DEFINE_ENUM_VALUE(Emotion, DISGUST);
	SO_NODE_DEFINE_ENUM_VALUE(Emotion, FEAR);
	SO_NODE_DEFINE_ENUM_VALUE(Emotion, SADNESS);
	SO_NODE_DEFINE_ENUM_VALUE(Emotion, NOTUSED);

    SO_NODE_SET_SF_ENUM_TYPE(emotion, Emotion);
	
	SO_NODE_ADD_FIELD(emotion,		(NEUTRAL));
	SO_NODE_ADD_FIELD(emotionInt,	(NEUTRAL));

	textSensor.setFunction(textCB);
	textSensor.setData(this);
	textSensor.attach( &input);

	emotionSensor.setFunction(textCB);
	emotionSensor.setData(this);
	emotionSensor.attach( &emotion);

	emotionIntSensor.setFunction(textCB);
	emotionIntSensor.setData(this);
	emotionIntSensor.attach( &emotionInt);

	intensitySensor.setFunction(textCB);
	intensitySensor.setData(this);
	intensitySensor.attach( &intensity);

}

//----------------------------------------------------------------------------

SoEmotionaliseSpeechText::~SoEmotionaliseSpeechText()
{

}

//----------------------------------------------------------------------------

// callback function mapper

void SoEmotionaliseSpeechText::textCB(void *data, SoSensor *sensor)
{
	SoEmotionaliseSpeechText * node = (SoEmotionaliseSpeechText *)data;
	
	float inten = node->intensity.getValue();
	int emo = node->emotion.getValue();
	int emoInt = node->emotionInt.getValue();
	
	// integer input overrides 
	if (emo == NOTUSED) emo = emoInt;

	float intensityFactor = sqrt(inten*100.0f)/5.0f;
	
	char tag[1024];

	const char * text = node->input.getValue().getString();
	if (strlen(text) > 960)
	{
		std::cout << "[error] SoEmotionaliseSpeechText::textCB: text too long !! " << std::endl;
		return;
	}
	sprintf(tag,"<rate speed='%d'><volume level='%d'><pitch middle='%d'>%s</pitch></volume></rate>",
				(int)(-2.0f + 1.0f*rate[emo]   *intensityFactor),
				(int)(80.0f + 5.0f*volume[emo] *intensityFactor),
				(int)( 0.0f + 2.0f*pitch[emo]  *intensityFactor),
				text
			);

	node->output.setValue(tag);
}

//----------------------------------------------------------------------------

