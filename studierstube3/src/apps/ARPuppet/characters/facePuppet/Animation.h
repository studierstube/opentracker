// Animation.h
//
// animation engine class representing an individual agent

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "FaceModel.h"
#include "Deformation.h"
#include "utils.h"

class CAnimation{

protected:

    // *** fixed topology and deformation data
    CFaceModel m_faceModel;     // *** face object - facial topology data
    CDeformation m_deformation; // *** deformation object - action unit, expression & viseme data

    // *** facial colors
    char m_colorName[MAX_COLOR][255];

    // *** transitions for animation of the expressions and the visemes
    CTransition m_expTransition[MAX_ACTION_UNIT];   // *** AU intensity:   0 - 100% (percent)
    CTransition m_visTransition[MAX_ACTION_UNIT];   // *** AU intensity:   0 - 100% (percent)
    CTransition m_eyeTransition[4];                 // *** eye angles:   -30 - 30`  (degree)
    CTransition m_headTransition[3];                // *** head angles: -180 - 180` (degree),
                                                    // *** head zoom:     10 - 500% (percent)
    CTransition m_colTransition[MAX_COLOR*3];       // *** facial color R,G,B

    char m_TTSModeID[255];

    // *** state variables
    bool m_initialized;

    // *** variables for animation
    CVertex *m_deform;
    int m_actionUnitIntensity[MAX_ACTION_UNIT];
    float m_zoomValue;
    float *m_transform;
    float *m_color;

public:

    CAnimation();
    ~CAnimation();

    bool Init(char *path);
    void Reset();

    bool Draw(float zoom,bool smoothEnabled,bool coloringEnabled);

    bool LoadConformation(char *fileName);

    void SetExpressionTarget(const char *name,int intensity=100,int transitionTime=0,bool blend=false);
    void SetActionUnitTarget(int num,int intensity=100,int transitionTime=0,bool blend=false);
    void SetVisemeTarget(int num,int intensity=100,int transitionTime=0,bool blend=false);
    void SetColorTarget(int num,const char *name,int transitionTime=0,bool blend=false);
    void SetColorTarget(int num,int r,int g,int b,int transitionTime=0,bool blend=false);
    void SetHeadTarget(int num,int value,int transitionTime=0,bool blend=false);
    void SetEyeTarget(int num,int value,int transitionTime=0,bool blend=false);
    void BlendExpressions(const char *name1,const char *name2,int factor,int intensity=100,
        int transitionTime=0,bool blend=false);

    void IncrementTimer(int value);

    // *** for editing
    void SetExpressionTime(int time);
    void SetVisemeTime(int time);
    void SetColorTime(int num,int time);
    int  GetActionUnitValue(int num);
    char* GetTTSModeID();

    void GetMinMax(float *min,float *max);
};

#endif // _ANIMATION_H_