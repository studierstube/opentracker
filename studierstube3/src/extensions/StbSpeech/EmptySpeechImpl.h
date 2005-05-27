// Speech.h
//
// speech engine class

#ifndef EmptySpeechImpl_H
#define EmptySpeechImpl_H

// *** SAPI includes

class SoSpeechOutput;

#define ANIM_SPEAK 1
#define ANIM_SND 2

class EmptySpeechImpl{

protected:

	SoSpeechOutput *m_sceneObject; ///< connection to scene graph used for updating fields
public:

	EmptySpeechImpl(SoSpeechOutput *sceneObject);
	~EmptySpeechImpl();

	bool init();
	void speak(const char *text);
	void playWAV(const char *fileName);
	void shutUp();
	void  initializeVoice(const char *gender, const char* language, const char *voiceName);
};


#endif