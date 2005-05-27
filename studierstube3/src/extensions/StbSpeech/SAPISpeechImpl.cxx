// Speech.cpp
//
// behavior engine class controlling objects in the scene

#include "SAPISpeechImpl.h"
#include "SoSpeechOutput.h"
//#include "Behavior.h"
#include <iostream>

SAPISpeechImpl::SAPISpeechImpl(SoSpeechOutput *sceneObject){

	m_sceneObject = sceneObject;

	m_duration=0;
	m_nextViseme=m_rate=0;

	strcpy(m_voiceName,"");

	m_speechPosition=-1;
	m_textLength=0;
	m_sound=false;

    m_initialized=false;
}

SAPISpeechImpl::~SAPISpeechImpl(){
}

bool SAPISpeechImpl::init(){

	// *** initializing COM objects and TTS engine
	if ((HRESULT)(m_voice.CoCreateInstance(CLSID_SpVoice))<0) {

//		ErrorMessage(ERR_INIT_VOICE);
		std::cout << "[error] SAPISpeechImpl::Init() could not initialize voice" << std::endl;
	    m_voice=NULL;
		m_initialized=false;
		return false;		
	}

	m_voice->SetNotifyCallbackFunction(&speechCallback,0,(LPARAM)this);
	unsigned __int64 ttsEvents=
		SPFEI(SPEI_WORD_BOUNDARY)|SPFEI(SPEI_SENTENCE_BOUNDARY)|
	SPFEI(SPEI_VISEME)|SPFEI(SPEI_START_INPUT_STREAM)|SPFEI(SPEI_END_INPUT_STREAM);
	m_voice->SetInterest(ttsEvents,ttsEvents);
//	m_voice->Speak(L" ",SPF_ASYNC,0); // *** to init speech with empty string

	m_initialized=true;

	return true;
}

void SAPISpeechImpl::endOfStream()
{
	m_sceneObject->currentViseme.setValue(0);
	m_sceneObject->nextViseme.setValue(0);
	m_sceneObject->rate.setValue(1);
	m_sceneObject->duration.setValue(1);
	m_sceneObject->isActive.setValue(FALSE);
	m_sceneObject->eventType.setValue(SoSpeechOutput::END_STREAM);
	m_sceneObject->eventTrigger.setValue();
}

void SAPISpeechImpl::speak(const char *text){

	if (!m_initialized || (m_textLength=strlen(text))==0) {
		endOfStream();
		return;
	}

	m_speechPosition=-1;
	m_sceneObject->textLength.setValue(m_textLength);
	m_sceneObject->speechPosition.setValue(m_speechPosition);
    
	WCHAR *pwcMessage=new WCHAR[MAX_TEXT];
	mbstowcs(pwcMessage,text,strlen(text)+1);
	m_voice->Speak(pwcMessage,SPF_ASYNC|SPF_IS_XML,0);
	delete[]pwcMessage;
}

void SAPISpeechImpl::playWAV(const char *fileName){

	if (!m_initialized || strlen(fileName)==0) {
		endOfStream();
		return;
	}

	m_wavStream=NULL;
    if ((HRESULT)(SPBindToFile(fileName,SPFM_OPEN_READONLY,&m_wavStream))>=0)
	{
		m_sceneObject->isActive.setValue(TRUE);
        m_voice->SpeakStream(m_wavStream,SPF_ASYNC,NULL);
    }
}

void SAPISpeechImpl::shutUp()
{
	m_voice->Speak(NULL,SPF_PURGEBEFORESPEAK,0);
}

void SAPISpeechImpl::speechEvent(){

	CSpEvent eventItem; // *** CSpEvent is safer than SPEVENT because it releases 
						// *** any allocated memory in it's destructor
	if(!m_voice) return;

	while (eventItem.GetFrom(m_voice)==S_OK){

		switch(eventItem.eEventId){

			case SPEI_VISEME:
				m_nextViseme = getVisemeID(eventItem.wParam);
				m_duration = (double)(HIWORD(eventItem.wParam));
				
				m_sceneObject->currentViseme.setValue(m_sceneObject->nextViseme.getValue());
				m_sceneObject->nextViseme.setValue(m_nextViseme);
				m_sceneObject->duration.setValue((float)m_duration);
				m_sceneObject->eventType.setValue(SoSpeechOutput::VISEME);
				m_sceneObject->eventTrigger.setValue(); // fire trigger
				break;

			case SPEI_WORD_BOUNDARY:
				m_voice->GetRate(&m_rate);
				m_speechPosition=eventItem.lParam;
				
				m_sceneObject->rate.setValue(m_rate);
				m_sceneObject->speechPosition.setValue(m_speechPosition);
				m_sceneObject->eventType.setValue(SoSpeechOutput::WORD_BOUNDARY);
				m_sceneObject->eventTrigger.setValue(); // fire trigger
				break;

			case SPEI_SENTENCE_BOUNDARY:
				m_sceneObject->eventType.setValue(SoSpeechOutput::SENTENCE_BOUNDARY);
				m_sceneObject->eventTrigger.setValue(); // fire trigger
				break;			

			case SPEI_START_INPUT_STREAM:
				m_sceneObject->isActive.setValue(TRUE);
				m_sceneObject->eventType.setValue(SoSpeechOutput::START_STREAM);
				m_sceneObject->eventTrigger.setValue(); // fire trigger
				break;

			case SPEI_END_INPUT_STREAM:
				m_speechPosition=m_textLength;
				m_textLength=0;
				
				m_sceneObject->speechPosition.setValue(m_speechPosition);
				// ???
				m_sceneObject->textLength.setValue(m_textLength);
				
				endOfStream();
				
				break;
		}
	}
}

int SAPISpeechImpl::getVisemeID(WPARAM param){

	int id;
	switch (LOWORD(param)){
		case SP_VISEME_0:
			id=0;
			break;
		case SP_VISEME_1:        
			id=1;
			break;
		case SP_VISEME_2:        
			id=2;
			break;
		case SP_VISEME_3:        
			id=3;
			break;
		case SP_VISEME_4:        
			id=4;
			break;
		case SP_VISEME_5:        
			id=5;
			break;
		case SP_VISEME_6:        
			id=6;
			break;
		case SP_VISEME_7:        
			id=7;
			break;
		case SP_VISEME_8:        
			id=8;
			break;
		case SP_VISEME_9:        
			id=9;
			break;
		case SP_VISEME_10:       
			id=10;
			break;
		case SP_VISEME_11:       
			id=11;
			break;
		case SP_VISEME_12:       
			id=12;
			break;
		case SP_VISEME_13:       
			id=13;
			break;
		case SP_VISEME_14:       
			id=14;
			break;
		case SP_VISEME_15:       
			id=15;
			break;
		case SP_VISEME_16:       
			id=16;
			break;
		case SP_VISEME_17:       
			id=17;
			break;
		case SP_VISEME_18:       
			id=18;
			break;
		case SP_VISEME_19:       
			id=19;
			break;
		case SP_VISEME_20:       
			id=20;
			break;
		case SP_VISEME_21:       
			id=21;
			break;
		default:
			id=-1;
			break;
	} 

	return id;
}

void SAPISpeechImpl::initializeVoice(const char *gender, const char* language, const char *voiceName)
{
	
	CComPtr<ISpObjectToken> cpTokenEng;
	
	char name[MAX_TEXT];
	name [0]= '\0';
	char sep[2];
	sep[0] = '\0';
	sep[1] = '\0';

	if (gender != NULL  && strlen(gender) > 0)
	{
		sprintf(name,"%sGender=%s",name, gender);
		sep[0] = ',';
	}
	
	if (language != NULL  && strlen(language) > 0)
	{
		sprintf(name,"%s%sLanguage=%s",name, sep, language);
		sep[0] = ',';
	}
	
	if (voiceName != NULL && strlen(voiceName) > 0)
	{
		sprintf(name,"%s%sName=%s",name, sep,voiceName);
	}
	

	WCHAR *pwcName=new WCHAR[MAX_TEXT];
	mbstowcs(pwcName,name,strlen(name)+1);

	// The format of selection criteria is "Attribute = Value" and "Attribute != Value." 
	// Voice attributes include "Name", "Gender," "Age," "Name," "Language," and "Vendor."


	/*
	{
	HRESULT                             hr = S_OK;
	CComPtr<ISpObjectToken>             cpVoiceToken;
	CComPtr<IEnumSpObjectTokens>        cpEnum;
	CComPtr<ISpVoice>                   cpVoice;
	ULONG                               ulCount = 0;
	// Create the SAPI voice
	if(SUCCEEDED(hr))
		hr = cpVoice.CoCreateInstance( CLSID_SpVoice ); 


	//Enumerate the available voices 
	if(SUCCEEDED(hr))
		hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);

	//Get the number of voices
	if(SUCCEEDED(hr))
		hr = cpEnum->GetCount(&ulCount);

	// Obtain a list of available voice tokens, set the voice to the token, and call Speak
	while (SUCCEEDED(hr) && ulCount -- )
	{
		cpVoiceToken.Release();
		if(SUCCEEDED(hr))
			hr = cpEnum->Next( 1, &cpVoiceToken, NULL );

		if(SUCCEEDED(hr))
			hr = cpVoice->SetVoice(cpVoiceToken);
	
//		if(SUCCEEDED(hr))	hr = cpVoice->Speak( L"How are you?", SPF_DEFAULT, NULL); 
		
		 cpVoice->Get
                       
	}
	
	}
	*/

	if ((HRESULT)(SpFindBestToken(SPCAT_VOICES,pwcName,NULL,&cpTokenEng))<0)
	{
		std::cout << "voice not found [" << name<< "] " << std::endl;
	}
	else
	{
		if ((HRESULT)(m_voice->SetVoice(cpTokenEng))>=0) 
		{
			CSpDynamicString	ppcDesciptionString;
			HRESULT hr = SpGetDescription( cpTokenEng, &ppcDesciptionString );
			_ASSERTE( SUCCEEDED( hr ) );
			char outpName[100];

			BOOL          match;
			WCHAR		attr[MAX_TEXT];
			mbstowcs(attr,"Gender=Male",4+7);
			cpTokenEng->MatchesAttributes(attr, &match);			
			std::cout << "is male: "<< match << std::endl;
			
			mbstowcs(attr,"Gender=Female",6+7);
			cpTokenEng->MatchesAttributes(attr, &match);			
			std::cout << "is female: "<< match << std::endl;
			
			mbstowcs(attr,"Gender=MALE",4+7);
			cpTokenEng->MatchesAttributes(attr, &match);			
			std::cout << "is MALE: "<< match << std::endl;

			mbstowcs(attr,"Gender=FEMALE",6+7);
			cpTokenEng->MatchesAttributes(attr, &match);			
			std::cout << "is FEMALE: "<< match << std::endl;


			wcstombs(outpName, ppcDesciptionString.Detach(), 100);
			std::cout << "cpVoiceName [" << outpName<< "] " << std::endl;


			strcpy(m_voiceName, outpName);
			if (strcmp(m_sceneObject->voiceName.getValue().getString(), m_voiceName) != 0) 
				m_sceneObject->voiceName.setValue(m_voiceName);
		}
		else
		{
			std::cout << "voice not found [" << name<< "] " << std::endl;
		}
	}

	m_initialized = true;

	delete[]pwcName;
}

void WINAPI SAPISpeechImpl::speechCallback(WPARAM wp,LPARAM lp){

	SAPISpeechImpl *speech=(SAPISpeechImpl*)lp;
	speech->speechEvent();
}