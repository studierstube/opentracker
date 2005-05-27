// Animation.cpp
//
// animation engine class representing an individual agent

#include "Animation.h"

CAnimation::CAnimation(){
    
    for (int i=0;i<MAX_COLOR;i++){
        strcpy(m_colorName[i],"def_");
        strcat(m_colorName[i],colorGroupName[i]);
    }
    strcpy(m_TTSModeID,"");

    m_deform=new CVertex[MAX_VERTEX];
    m_transform=new float[8];
    m_color=new float[MAX_COLOR*3];
    
    m_initialized=false;
}

CAnimation::~CAnimation(){

    delete[]m_deform;
    delete[]m_transform;
    delete[]m_color;
}

bool CAnimation::Init(char *path){

    // *** initializing face model
    if (!m_faceModel.Init(path)){
        m_initialized=false;
        return false;
    }

    // *** initializing deformation parameters - action units, visemes, expressions, actions
    if (!m_deformation.Init(path)){
        m_initialized=false;
        return false;
    }

    // *** set default facial colors
    for (int i=0;i<MAX_COLOR;i++){
        SetColorTarget(i,m_colorName[i]);
    }

    // *** set default zoom
    m_headTransition[2].SetTarget(100);

    // *** the face is ready to use
    m_initialized=true;

    return true;
}

void CAnimation::Reset(){

    for (int k=0;k<MAX_ACTION_UNIT;k++){
        m_expTransition[k].Reset();
        m_visTransition[k].Reset();
    }

    for (int l=0;l<4;l++){
        m_eyeTransition[l].Reset();
    }

    for (int m=0;m<2;m++){
        m_headTransition[m].Reset();
    }
    m_headTransition[2].SetTarget(100);

    for (int n=0;n<MAX_COLOR;n++){
        SetColorTarget(n,m_colorName[n]);
    }
}

bool CAnimation::Draw(float zoom,bool smoothEnabled,bool coloringEnabled){

    // *** blend viseme and expression AU values
    for (int p=0;p<MAX_ACTION_UNIT;p++){
        m_actionUnitIntensity[p]= (!m_visTransition[p].GetValue()) ?
            (int)m_expTransition[p].GetValue() : (int)m_visTransition[p].GetValue();
    }

    // *** calculate deformation vector for each vertex
    for (int q=0;q<MAX_VERTEX;q++){
        m_deform[q]=m_deformation.GetDeformationVector(q,m_actionUnitIntensity);
    }

    // *** face transformation  
    m_zoomValue=zoom*m_headTransition[2].GetValue();
    if (m_zoomValue>(float)MAX_HEAD_ZOOM)   m_zoomValue=(float)MAX_HEAD_ZOOM;
    else if (m_zoomValue<(float)MIN_HEAD_ZOOM) m_zoomValue=(float)MIN_HEAD_ZOOM;
    m_zoomValue=m_zoomValue/100.0f;

    for (int i=0;i<7;i++){
        if (i<2) m_transform[i]=m_headTransition[i].GetValue(); // *** 0-1 : head ROT HOR/VER
        else if (i==2) m_transform[i]=m_zoomValue;                  // ***   2 : head ZOOM
        else m_transform[i]=m_eyeTransition[i-3].GetValue();        // *** 3-6 : eye ROT L/R, HOR/VER
    }
    m_transform[7]=1.0f; // *** for vertex (pupil) and line (eyelash) size in the renderer, used to be sizeRatio

    // *** facial coloration
    for (int j=0;j<MAX_COLOR;j++){
        for (int k=0;k<3;k++){
            m_color[j*3+k]=m_colTransition[j*3+k].GetValue();
        }
    }

    // *** render face
    return m_faceModel.Draw(m_deform,m_transform,m_color,smoothEnabled,coloringEnabled);
}

void CAnimation::IncrementTimer(int value){

    for (int i=0;i<MAX_ACTION_UNIT;i++){
        m_expTransition[i].IncrementTimer(value);
        m_visTransition[i].IncrementTimer(value);
    }
    for (int j=0;j<4;j++){
        m_eyeTransition[j].IncrementTimer(value);
    }
    for (int k=0;k<3;k++){
        m_headTransition[k].IncrementTimer(value);
    }   
    for (int l=0;l<MAX_COLOR*3;l++){
        m_colTransition[l].IncrementTimer(value);
    }
}

int CAnimation::GetActionUnitValue(int num){

    if (num>=MAX_ACTION_UNIT || num<0) return 0;
    else return m_expTransition[num].GetValue()>m_visTransition[num].GetValue() ?
            (int)m_expTransition[num].GetValue() : (int)m_visTransition[num].GetValue();
}

void CAnimation::GetMinMax(float *min,float *max){

    m_faceModel.GetMinMax(min,max);
}

void CAnimation::SetEyeTarget(int num,int value,int transitionTime,bool blend){

    if (num>3||num<0) return;
    if (value>MAX_EYE_ANGLE || value<-MAX_EYE_ANGLE) {
        if (value>MAX_EYE_ANGLE) value=MAX_EYE_ANGLE;
        else if (value<-MAX_EYE_ANGLE) value=-MAX_EYE_ANGLE;
    }
    m_eyeTransition[num].SetTarget(value,transitionTime,blend);
}

void CAnimation::SetHeadTarget(int num,int value,int transitionTime,bool blend){

    if (num>2||num<0) return;
    switch (num){
        case 0:
        case 1:
            if (value>MAX_HEAD_ANGLE || value<-MAX_HEAD_ANGLE ) {
                if (value>MAX_HEAD_ANGLE ) value=MAX_HEAD_ANGLE ;
                else if (value<-MAX_HEAD_ANGLE ) value=-MAX_HEAD_ANGLE ;
            }
            break;
        case 2:
            if (value>MAX_HEAD_ZOOM|| value<MIN_HEAD_ZOOM) {
                if (value>MAX_HEAD_ZOOM ) value=MAX_HEAD_ZOOM;
                else if (value<MIN_HEAD_ZOOM) value=MIN_HEAD_ZOOM;
            }
            break;
    }   
    m_headTransition[num].SetTarget(value,transitionTime,blend);
}

void CAnimation::SetExpressionTarget(const char *name,int intensity,int transitionTime,bool blend){

    CExpression *exp=m_deformation.GetExpression(name);

    if (exp==NULL) return;

    for (int i=0;i<MAX_ACTION_UNIT;i++){
        m_expTransition[i].SetTarget(exp->GetIntensity(i)*intensity/100,transitionTime,blend);
    }
}

void CAnimation::SetActionUnitTarget(int num,int value,int transitionTime,bool blend){

    if (num>=MAX_ACTION_UNIT || num<0) return;
    m_expTransition[num].SetTarget(value,transitionTime,blend);
}

void CAnimation::SetVisemeTarget(int num,int intensity,int transitionTime,bool blend){

    CExpression *vis=m_deformation.GetViseme(num);

    for (int i=0;i<MAX_ACTION_UNIT;i++){
        m_visTransition[i].SetTarget(vis->GetIntensity(i)*intensity/100,transitionTime,blend);
    }

}

void CAnimation::SetColorTarget(int num,const char *name,int transitionTime,bool blend){

    if (num<0 || num>=MAX_COLOR || name=="") return;

    CColor *defColor=m_deformation.GetColor(m_colorName[num]);
    CColor *color;
    if (!strcmp(name,"_def_")) color=m_deformation.GetColor(m_colorName[num]);
    else color=m_deformation.GetColor(name);
    CColor newColor,faceColor;

    int factor=0;
    switch (num){
        case COL_FACE:      
            factor=60;
            newColor.SetColor(
                (defColor->GetRed()*factor+color->GetRed()*(100-factor))/100,
                (defColor->GetGreen()*factor+color->GetGreen()*(100-factor))/100,
                (defColor->GetBlue()*factor+color->GetBlue()*(100-factor))/100
            );
            break;
        case COL_CHEEK:
            factor=40;
            newColor.SetColor(
                (defColor->GetRed()*factor+color->GetRed()*(100-factor))/100,
                (defColor->GetGreen()*factor+color->GetGreen()*(100-factor))/100,
                (defColor->GetBlue()*factor+color->GetBlue()*(100-factor))/100
            );
            break;
        case COL_NOSE:      
        case COL_CHIN:
        case COL_MOUSTACHE:
        case COL_EAR:
            faceColor.SetColor(m_deformation.GetColor(m_colorName[COL_FACE]));
            if (color->GetRed()==faceColor.GetRed() && 
                color->GetGreen()==faceColor.GetGreen() &&
                color->GetBlue()==faceColor.GetBlue()){
                    newColor.SetColor(&faceColor);
            }
            else newColor.SetColor(color);
            break;
        default:
            newColor.SetColor(color);
            break;
    }

    m_colTransition[num*3].SetTarget(newColor.GetRed(),transitionTime,blend);
    m_colTransition[num*3+1].SetTarget(newColor.GetGreen(),transitionTime,blend);
    m_colTransition[num*3+2].SetTarget(newColor.GetBlue(),transitionTime,blend);
}

void CAnimation::SetColorTarget(int num,int r,int g,int b,int transitionTime,bool blend){

    if (num<0 || num>=MAX_COLOR) return;

    m_colTransition[num*3].SetTarget(r,transitionTime,blend);
    m_colTransition[num*3+1].SetTarget(g,transitionTime,blend);
    m_colTransition[num*3+2].SetTarget(b,transitionTime,blend);
}

void CAnimation::BlendExpressions(const char *name1,const char *name2,int factor,int intensity,
                              int transitionTime,bool blend){

    CExpression *exp1=m_deformation.GetExpression(name1);
    CExpression *exp2=m_deformation.GetExpression(name2);

    if (exp1==NULL || exp2==NULL) return;

    for (int i=0;i<MAX_ACTION_UNIT;i++){
        m_expTransition[i].SetTarget(
            (exp1->GetIntensity(i)*(100-factor)+exp2->GetIntensity(i)*factor)*intensity/10000,
            transitionTime,
            blend
        );
    }
}

void CAnimation::SetExpressionTime(int time){

    for (int i=0;i<MAX_ACTION_UNIT;i++){
        m_expTransition[i].SetTimer(time);
    }
}

void CAnimation::SetVisemeTime(int time){

    for (int i=0;i<MAX_ACTION_UNIT;i++){
        m_visTransition[i].SetTimer(time);
    }
}

void CAnimation::SetColorTime(int num,int time){

    if (num<0 || num>=MAX_COLOR) return;

    for (int i=0;i<3;i++) m_colTransition[num*3+i].SetTimer(time);
}

bool CAnimation::LoadConformation(char *fileName){
    
    // *** temporary variables for reading
    char line[128];
    int serial,fid,transparency;
    float x,y,z;
    int r,g,b;
    char name[128];
    CVertex vertex[MAX_VERTEX];

    // *** open vertex file
    FILE *stream=fopen(fileName,"r");
    if (!stream) {
        ErrorMessage(ERR_INIT_FACEMODEL_CONFORMATION);
        return false;
    }

    int i,j;
    i=j=0;

    while (i<MAX_VERTEX+1 && fgets(line,256,stream)){
        if (line[0]!='#'){      
            if (!i) {
                strcpy(m_TTSModeID,line);
                i++;
            }
            else {
                sscanf(line,"%d %d %f %f %f",&serial,&fid,&x,&y,&z);
                vertex[i-1].Set(x,y,z);
                i++;
            }
        }
    }

    char colorName[255];
    while (j<MAX_COLOR && fgets(line,256,stream)){
        if (line[0]!='#'){
            sscanf(line,"%d %d %d",&r,&g,&b);
            sprintf(colorName,"agent_%s",colorGroupName[j]);
            m_deformation.AddColor(colorName,r,g,b);
            strcpy(m_colorName[j],colorName);
            SetColorTarget(j,r,g,b);
            j++;
        }
    }

    bool glasses;
    while (fgets(line,256,stream)){
        if (line[0]!='#'){
            sscanf(line,"%s %d",&name,&transparency);
            if (!strcmp(name,"ON")) glasses=true;
            else glasses=false;
        }
    }

    m_faceModel.SetConformation(vertex,glasses,transparency);

    fclose(stream);

    return true;
}

char* CAnimation::GetTTSModeID(){

    return m_TTSModeID;
}
