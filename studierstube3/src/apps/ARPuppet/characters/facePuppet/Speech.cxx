// Speech.cpp
//
// behavior engine class controlling objects in the scene

#include "Speech.h"
#include "Behavior.h"

CSpeech::CSpeech(){

	m_behavior=NULL;

	m_lipSync=false;
	m_duration=0;
	m_nextViseme=m_rate=0;

	strcpy(m_voiceName,"");

	m_speechPosition=-1;
	m_textLength=0;
	m_isSpeaking=false;
	m_sound=false;

    m_initialized=false;
}

CSpeech::~CSpeech(){
}

bool CSpeech::Init(CBehavior *behavior){

	m_behavior=behavior;

	// *** initializing COM objects and TTS engine
	if ((HRESULT)(m_voice.CoCreateInstance(CLSID_SpVoice))<0) {

		ErrorMessage(ERR_INIT_VOICE);
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

bool CSpeech::IsInitialized(){

	return m_initialized;
}

void CSpeech::Speak(const char *text){

	if (!m_initialized || !(m_textLength=strlen(text))) {
		m_behavior->FireRequestComplete(ANIM_SPEAK);
		return;
	}

	m_speechPosition=-1;

	WCHAR *pwcMessage=new WCHAR[MAX_TEXT];
	mbstowcs(pwcMessage,text,strlen(text)+1);
	m_voice->Speak(pwcMessage,SPF_ASYNC|SPF_IS_XML,0);
	delete[]pwcMessage;
}

void CSpeech::PlayWAV(const char *fileName){

	if (!m_initialized || !strlen(fileName)) {
		m_behavior->FireRequestComplete(ANIM_SND);
		return;
	}

	m_wavStream=NULL;
    if ((HRESULT)(SPBindToFile(fileName,SPFM_OPEN_READONLY,&m_wavStream))>=0){
		m_sound=true;
        m_voice->SpeakStream(m_wavStream,SPF_ASYNC,NULL);
    }
}

void CSpeech::ShutUp(){

	m_voice->Speak(NULL,SPF_PURGEBEFORESPEAK,0);
}

void CSpeech::SpeechEvent(){

	CSpEvent eventItem; // *** CSpEvent is safer than SPEVENT because it releases 
						// *** any allocated memory in it's destructor
	if(!m_voice) return;

	while (eventItem.GetFrom(m_voice)==S_OK){

		switch(eventItem.eEventId){

			case SPEI_VISEME:
				m_nextViseme=GetVisemeID(eventItem.wParam);
				m_duration=(double)(HIWORD(eventItem.wParam));
				m_lipSync=true;
				break;

			case SPEI_WORD_BOUNDARY:
				m_voice->GetRate(&m_rate);
				m_speechPosition=eventItem.lParam;
				break;

			case SPEI_SENTENCE_BOUNDARY:
				break;			

			case SPEI_START_INPUT_STREAM:
				m_lipSync=true;
				m_isSpeaking=true;
				break;

			case SPEI_END_INPUT_STREAM:
				m_lipSync=false;
				m_isSpeaking=false;
				m_speechPosition=m_textLength;
				m_textLength=0;
				if (m_sound) {
					m_behavior->FireRequestComplete(ANIM_SND);
					m_sound=false;
				}
				else m_behavior->FireRequestComplete(ANIM_SPEAK);
				break;
		}
	}
}

int CSpeech::GetVisemeID(WPARAM param){

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

void CSpeech::SetLipSync(bool value){

	m_lipSync=value;
}

bool CSpeech::GetLipSync(){

	return m_lipSync;
}

int	CSpeech::GetNextViseme(){

	return m_nextViseme;
}

int	CSpeech::GetDuration(){

	return (int)m_duration;
}

int	CSpeech::GetRate(){

	return m_rate;
}

void CSpeech::SetTTSModeID(LPCTSTR lpszNewValue){

	CComPtr<ISpObjectToken> cpTokenEng;
	
	char name[MAX_TEXT];
	sprintf(name,"Name=%s",lpszNewValue);
	WCHAR *pwcName=new WCHAR[MAX_TEXT];
	mbstowcs(pwcName,name,strlen(name)+1);

	if ((HRESULT)(SpFindBestToken(SPCAT_VOICES,pwcName,NULL,&cpTokenEng))<0) return;
	if ((HRESULT)(m_voice->SetVoice(cpTokenEng))>=0) strcpy(m_voiceName,lpszNewValue);

	delete[]pwcName;
}


char* CSpeech::GetTTSModeID(){

	return m_voiceName;
}

int CSpeech::GetSpeechPosition(){

	return m_speechPosition;
}

bool CSpeech::IsSpeaking(){

	return m_isSpeaking;
}

void WINAPI speechCallback(WPARAM wp,LPARAM lp){

	CSpeech *speech=(CSpeech*)lp;
	speech->SpeechEvent();
}