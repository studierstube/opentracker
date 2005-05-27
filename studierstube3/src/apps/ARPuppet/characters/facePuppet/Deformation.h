// Deformation.h
//
// stores deformation information - action units - class for the face

#ifndef _DEFORMATION_H_
#define _DEFORMATION_H_

#include <windows.h>
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"
#include "Topology.h"
#include "utils.h"

class CActionUnit {

protected:

    CVertex m_translation[MAX_VERTEX];

public:

    CActionUnit();
    ~CActionUnit();

    CVertex GetTranslationVector(int index);

    bool ReadDataFile(char *fileName);
};

//----------------------------------------------------------------------
class CExpression{

protected:

    char *m_name;
    int m_intensity[MAX_ACTION_UNIT];

public:

    CExpression();
    ~CExpression();
    
    void  SetName(char *name);
    void  SetIntensity(int actionUnitNumber,int value);
    char* GetName();
    int   GetIntensity(int actionUnitNumber);

    bool ReadDataFile(char *fileName);
};

//-------------------------------------------------------------------------------------------
class CColor{

protected:

    char *m_name;
    int m_red,m_green,m_blue;

public:

    CColor();
    ~CColor();
    
    void SetName(char *name);
    void SetColor(int red,int green,int blue);
    void SetColor(CColor *color);

    char *GetName();
    int GetRed();
    int GetGreen();
    int GetBlue();
};

//-------------------------------------------------------------------------------------------

class CTransition{

protected:

    // *** blending ON/OFF
    bool m_blend;

    // *** values of destination and target stages
    float m_startValue[2];
    float m_targetValue[2];
    float m_startGradient[2];
    float m_currentGradient[2]; 

    // *** time parameters
    int m_time[2]; // *** internal timer
    int m_transitionTime[2];

public:

    CTransition();
    ~CTransition() {};

    void Reset();

    void SetTarget(int target=0,int T=0,bool blend=false);

    void SetTimer(int time);        // *** for editing
    void IncrementTimer(int value); // *** for animation

    float GetValue();
    bool Finished(bool blend);
};

//----------------------------------------------------------------------

class CDeformation {

protected:

    CActionUnit m_actionUnit[MAX_ACTION_UNIT];  
    CExpression m_viseme[MAX_VISEME];
    LinkedList<CExpression*> m_expressionList;
    LinkedList<CColor*> m_colorList;

    bool m_initialized;

public:

    CDeformation();
    ~CDeformation() {};
    
    bool Init(char *path);

    bool LoadExpressions(char *fileNameBase);
    bool LoadColors(char *fileName);

    CVertex GetDeformationVector(int index,int* actionUnitAmount);
    CExpression* GetExpression(const char *name);
    CExpression* GetViseme(int num);
    CColor* GetColor(const char *name);
    void AddColor(char *name,int r,int g,int b);
};

#endif