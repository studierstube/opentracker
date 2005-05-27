// Action.h
//
// animation engine class representing an individual agent

#ifndef _ACTION_H_
#define _ACTION_H_

#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "utils.h"

class CKeyframe{
    
public:

    int   m_trackNumber;
    int   m_transitionTime;
    int   m_time;
    char* m_name;
    int   m_value;

    CKeyframe();
    CKeyframe(int trackNum,int time,int transitionTime,char *name,int value=-1);
    ~CKeyframe();
};

// **********************************************************

// *** request object to send RequestComplete events when animations finish
class CRequest{

protected:

    int  m_type;
    int  m_currentTime;
    int  m_transitionTime;

public:

    CRequest();
    CRequest(int type,int transitionTime);
    ~CRequest();

    void IncrementTimer(int value);
    int  Completed();
};

// **********************************************************

class CAction{

protected:

    char*    m_name;
    int      m_maxTime;
    LinkedList<CKeyframe*> m_keyframeList;  

public:

    CAction();
    ~CAction();

    void        SetName(char *name);
    char*       GetName();
    int         GetMaxTime();
    CKeyframe*  GetCurrentKeyframe();
    CKeyframe*  GetNextKeyframe();
    void        InitPosition();

    bool ReadDataFile(char *fileName);
};

// **********************************************************

class CSpeechAction{

public:

    int      m_position;
    char*    m_name;
    int      m_intensity;
    int      m_scale;

    CSpeechAction();
    CSpeechAction(int position,char *name,int intensity,int scale);
    ~CSpeechAction();
};

// **********************************************************

class CRandomAction{

public:

    int m_transitionTime;
    int m_timer;
    int m_info;
    int m_aux1;
    int m_aux2;

    CRandomAction();
    ~CRandomAction();
};

// **********************************************************

class CVariable{

public:

    char*    m_name;
    char*    m_value;

    CVariable();
    CVariable(char *name,char *value);
    ~CVariable();

    void SetValue(char *value);
};

#endif