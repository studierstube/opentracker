// Speech.cpp
//
// behavior engine class controlling objects in the scene

#include "EmptySpeechImpl.h"
#include "SoSpeechOutput.h"
//#include "Behavior.h"
#include <iostream>

EmptySpeechImpl::EmptySpeechImpl(SoSpeechOutput *sceneObject)
{
	m_sceneObject = sceneObject;
}

EmptySpeechImpl::~EmptySpeechImpl(){
}

bool EmptySpeechImpl::init()
{
	return true;
}

void EmptySpeechImpl::speak(const char *text)
{
	std::cout << "EmptySpeechImpl::speak speaking :";
	std::cout << text << std::endl;
	m_sceneObject->speechPosition.setValue(-1);
}

void EmptySpeechImpl::playWAV(const char *fileName)
{
	std::cout << "EmptySpeechImpl::playWAV play file:";
	std::cout << fileName<< std::endl;
	m_sceneObject->isActive.setValue(true);
}

void EmptySpeechImpl::shutUp()
{
	std::cout << "EmptySpeechImpl::shutUp" << std::endl;

}

void EmptySpeechImpl::initializeVoice(const char *gender, const char* language, const char *voiceName)
{
	std::cout <<  "EmptySpeechImpl::initializeVoice :";
	std::cout <<  " gender :";
	if (gender != NULL&& strlen(gender) > 0) 
	{
		std::cout <<  gender;
	}
	else
	{
		std::cout <<  "not set";
	}
	
	std::cout <<  " language :";
	if (language != NULL && strlen(language) > 0 ) 
	{
		std::cout <<  language;
	}
	else
	{
		std::cout <<  "not set";
	}
	
	std::cout <<  " voiceName :";
	if (voiceName != NULL&& strlen(voiceName) > 0 )
	{
		std::cout <<  voiceName;
	}
	else
	{
		std::cout <<  "not set";
	}

	std::cout <<  std::endl;

	if (strcmp(m_sceneObject->voiceName.getValue().getString(),"text output") == 0)
			return;

	m_sceneObject->voiceName.setValue("text output");

}
