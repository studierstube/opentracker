// Speech.h
//
// speech engine class
// based on code from Istvan

#ifndef SAPISpeechImpl_H
#define SAPISpeechImpl_H

// *** SAPI includes
#include <sapi.h>
#include <sphelper.h>

#include <wchar.h>

class SoSpeechOutput;

#define ANIM_SPEAK 1
#define ANIM_SND 2
#define MAX_TEXT 1024

class SAPISpeechImpl{

protected:

	CComPtr<ISpVoice>	m_voice;
	CComPtr<ISpStream>	m_wavStream;
	char m_voiceName[MAX_TEXT];

	SoSpeechOutput *m_sceneObject; ///< connection to scene graph used for updating fields

	// *** lip synch
	int		m_nextViseme;
	double	m_duration;
	long	m_rate;

	// *** speech position
	int m_speechPosition;
	int m_textLength;
	bool m_isSpeaking;
	bool m_sound;

	bool m_initialized;
	void endOfStream();

	static void WINAPI speechCallback(WPARAM wp,LPARAM lp);
	
public:

	SAPISpeechImpl(SoSpeechOutput *sceneObject);
	~SAPISpeechImpl();

	void speechEvent();
	int  getVisemeID(WPARAM param);

	bool init();
	void speak(const char *text);
	void playWAV(const char *fileName);
	void shutUp();

	void  initializeVoice(const char *gender, const char* language, const char *voiceName);
};


#endif