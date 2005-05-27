// Behavior.h
//
// behavior engine class controlling objects in the scene

#ifndef _BEHAVIOR_H_
#define _BEHAVIOR_H_

#include <string.h>
#include <time.h>

#include "Animation.h"
#include "Speech.h"
#include "Action.h"
#include "linkedlist.h"
#include "utils.h"


// *** Behavior Engine object
class CBehavior{

protected:
    
    // *** topology, deformation and agent data
    CAnimation  m_agent;    // *** agent object, fixed topology and deformation data
    CSpeech m_speech;   // *** object wrapping speech functions

    char m_text[MAX_TEXT];
    char m_conformation[MAX_TEXT];
    
    // *** timer
    double m_timerStart;
    double m_timerCurrent;
    long   m_actionTimer; 

    // *** request-related members
    LinkedList<CRequest*> m_requestList;

    // *** actions
    LinkedList<CAction*> m_actionList;
    LinkedList<CSpeechAction*> m_speechActionList;
    CRandomAction m_randomAction[MAX_RND_ACTION];
    CAction* m_currentAction;
    int m_actionIntensity;
    int m_actionTimeScale;
    float m_idleTime;

    // *** variable
    LinkedList<CVariable*> m_variableList;

    // *** save previous states for actions
    char  m_prevExpName[255];
    int   m_prevExpValue;
    int   m_prevEyeValue[4];
    int   m_prevHeadValue[3];
    char  m_prevColName[MAX_COLOR][255];

    // *** state variables      
    bool m_initialized;
    bool m_visible;
    bool m_puppeteer;

    // *** storing finish callback function and user data
    SoFinishCB *m_finishCB;
    void *m_finishCBData;

    // *** ID of the agent
    int m_agentID;

public:

    CBehavior();
    ~CBehavior();

    // *** initializing
    bool Init();
    bool LoadActions(char *fileNameBase);

    // *** high-level commands
    void Show();
    void Hide();
    void Play(const char *name, int intensity=100, int timeScale=100);
    void SetConformation(const char *name);
    int Speak(const char *text);
    void Sound(const char *name);
    void Think(const char *text);
    void ShutUp();
    void TurnToUser(int transitionTime=0);
    void TurnFromUser(int transitionTime=0);
    void  SetVariable(const char *name,const char *value);
    char* GetVariable(const char *name);

    // *** low-level commands
    void SetActionUnit(int number,int intensity=100,int transitionTime=0,bool noninternal=true);
    void SetExpression(const char *name,int intensity=100,int transitionTime=0,bool noninternal=true);
    void SetViseme(int num,int intensity=100,int transitionTime=0,bool noninternal=true);
    void SetColor(int num,const char *name,int transitionTime=0,bool noninternal=true);
    void SetColorRGB(int num,int r,int g,int b,int transitionTime=0,bool noninternal=true);
    void SetEye(int num,int value,int transitionTime=0,bool noninternal=true);
    void SetHead(int num,int value,int transitionTime=0,bool noninternal=true);
    void BlendExpressions(const char *exp1,const char *exp2,int factor,int intensity=100,int transitionTime=0,bool noninternal=true);

    // *** draw and animation functions
    bool Draw(float zoom,bool smoothEnabled,bool coloringEnabled);
    void Animate();
    void AutomaticBehavior(int value);

    // *** getter functions
    CAnimation*      GetAgent();
    CSpeech*     GetSpeech();
    char*        GetSpeechText();
    CAction*     GetAction(const char *name);
    char*        GetConformation();

    char* ExtractActionTags(char *text);
    void  LipSync();
    void  FireRequestComplete(int animationType);
    void  SetPuppeteer(bool puppeteer);
    bool  GetPuppeteer();
    void  RemoveTtsTags(char *text);
    void  GetMinMax(float *min,float *max);
    
    void setFinishCallback(SoFinishCB *cb, void* data);
    void SetAgentID(int id);
};

#endif