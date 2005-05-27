// Speech.h
//
// speech engine class

#ifndef _SPEECH_H_
#define _SPEECH_H_

// *** SAPI includes
#include <sapi.h>
#include <sphelper.h>

#include <wchar.h>
#include "utils.h"

class CBehavior;

class CSpeech{

protected:

    CComPtr<ISpVoice>   m_voice;
    CComPtr<ISpStream>  m_wavStream;
    char m_voiceName[MAX_TEXT];

    CBehavior *m_behavior;

    // *** lip synch
    bool    m_lipSync;
    int     m_nextViseme;
    double  m_duration;
    long    m_rate;

    // *** speech position
    int m_speechPosition;
    int m_textLength;
    bool m_isSpeaking;
    bool m_sound;

    bool m_initialized;

public:

    CSpeech();
    ~CSpeech();

    bool Init(CBehavior *behavior);
    void SpeechEvent();
    int  GetVisemeID(WPARAM param);

    void Speak(const char *text);
    void PlayWAV(const char *fileName);
    void ShutUp();

    void SetLipSync(bool value);
    bool GetLipSync();
    int  GetNextViseme();
    int  GetDuration();
    int  GetRate();
    int  GetSpeechPosition();
    bool IsSpeaking();
    bool IsInitialized();

    char* GetTTSModeID();
    void  SetTTSModeID(LPCTSTR lpszNewValue);
};

void WINAPI speechCallback(WPARAM wp,LPARAM lp);

#endif