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
  * $Id: SoSpeechOutput.cxx 4033 2004-11-23 11:06:09Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoSpeechOutput.h"
#include "SoEmotionaliseSpeechText.h"
#include <iostream>

using namespace std;
//----------------------------------------------------------------------------

SO_NODE_SOURCE(SoSpeechOutput);

//----------------------------------------------------------------------------

void SoSpeechOutput::initClass()
{
	SoEmotionaliseSpeechText::initClass();
	SO_NODE_INIT_CLASS(SoSpeechOutput, SoNode, "SoNode");
}

//----------------------------------------------------------------------------

SoSpeechOutput::SoSpeechOutput()
{
	SO_NODE_CONSTRUCTOR(SoSpeechOutput);
	
	SO_NODE_ADD_FIELD(voiceName, (""));				
	SO_NODE_ADD_FIELD(gender,	(""));
	SO_NODE_ADD_FIELD(language,	(""));
	
	//  lip synch
	SO_NODE_ADD_FIELD(currentViseme, (-1));	
	SO_NODE_ADD_FIELD(nextViseme,	 (-1));	
	SO_NODE_ADD_FIELD(duration,		 (-1.0));
	SO_NODE_ADD_FIELD(rate,			 (-1));	
	SO_NODE_ADD_FIELD(speechPosition,(-1));

	SO_NODE_DEFINE_ENUM_VALUE(EventType, NONE);
	SO_NODE_DEFINE_ENUM_VALUE(EventType, VISEME);
	SO_NODE_DEFINE_ENUM_VALUE(EventType, WORD_BOUNDARY);
	SO_NODE_DEFINE_ENUM_VALUE(EventType, SENTENCE_BOUNDARY);
	SO_NODE_DEFINE_ENUM_VALUE(EventType, START_STREAM);
	SO_NODE_DEFINE_ENUM_VALUE(EventType, END_STREAM);

    SO_NODE_SET_SF_ENUM_TYPE(eventType, EventType);

	SO_NODE_ADD_FIELD(eventType,	 (NONE));	
	SO_NODE_ADD_FIELD(eventTrigger , ());
    
	//  speech position
	SO_NODE_ADD_FIELD(textLength,		(-1));
	SO_NODE_ADD_FIELD(isActive,			(FALSE));
	
	SO_NODE_ADD_FIELD(stop,				());
	SO_NODE_ADD_FIELD(start,			());
	SO_NODE_ADD_FIELD(restartOnStart,	(FALSE));
	SO_NODE_ADD_FIELD(startOnSetText,	(TRUE));		
	SO_NODE_ADD_FIELD(stopped,			());			

	SO_NODE_ADD_FIELD(text,				(""));

	startSensor.setFunction(startTriggerCB);
	startSensor.setData(this);
	startSensor.attach( &start);
	
	stopSensor.setFunction(stopTriggerCB);
	stopSensor.setData(this);
	stopSensor.attach( &stop);
	
	textSensor.setFunction(textCB);
	textSensor.setData(this);
	textSensor.attach( &text);

	voiceNameSensor.setFunction(voiceNameCB);
	voiceNameSensor.setData(this);
	voiceNameSensor.attach( &voiceName);
	
	voiceGenderSensor.setFunction(voiceNameCB);
	voiceGenderSensor.setData(this);
	voiceGenderSensor.attach( &gender);

	voiceLanguageSensor.setFunction(voiceNameCB);
	voiceLanguageSensor.setData(this);
	voiceLanguageSensor.attach( &language);


#if defined(USE_SAPI_SPEECHIMPL)
	speechImpl = new SAPISpeechImpl(this); 
#elif defined(USE_EMPTY_SPEECHIMPL)
	speechImpl = new EmptySpeechImpl(this); 
#endif

	speechImpl->init();

	// SUPER HACK !!!!
	calledCounter = 0;
}

//----------------------------------------------------------------------------

SoSpeechOutput::~SoSpeechOutput()
{

}

//----------------------------------------------------------------------------

// callback function mapper

void SoSpeechOutput::startTriggerCB(void *data, SoSensor *sensor)
{
	SoSpeechOutput * node = (SoSpeechOutput *)data;
	
	// SUPER HACK !!!!
	node->calledCounter ++;
	// somehow the trigger is fired on makeing a connection -> COIN ISSUE ?!?
	if (node->calledCounter < 2) return;

	if (node->restartOnStart.getValue() == TRUE)
	{
		node->speechImpl->shutUp();
	}
	node->speakText();
}

void SoSpeechOutput::speakText()
{
	SbString fullText;
	for(int i=0; i<text.getNum(); i++)
	{
		SbString textValue;
		text.get1(i, textValue);
		int length = textValue.getLength();
		if (length > 2)
			fullText += textValue.getSubString(1,length-2) ;
		fullText += " ";
		printf("%s\n",fullText );
	}
	speechImpl->speak(fullText.getString());
}

void SoSpeechOutput::stopTriggerCB(void *data, SoSensor *sensor)
{
	((SoSpeechOutput *)data)->speechImpl->shutUp();
}

void SoSpeechOutput::textCB(void *data, SoSensor *sensor)
{
	SoSpeechOutput * node = (SoSpeechOutput *)data;

	if (node->startOnSetText.getValue() == TRUE)
	{
		node->speakText();
	}
}

void SoSpeechOutput::voiceNameCB(void *data, SoSensor *sensor)
{
	SoSpeechOutput * node = (SoSpeechOutput *)data;
	node->speechImpl->initializeVoice(
		node->gender.getValue().getString(),
		node->language.getValue().getString(),
		node->voiceName.getValue().getString());
}

//----------------------------------------------------------------------------

