// Action.cpp
//
// behavior engine class controlling objects in the scene

#include "Action.h"

CKeyframe::CKeyframe(){

    m_trackNumber=ANIM_NULL;
    m_transitionTime=0;
    m_time=0;
    m_name="";
    m_value=-1;
}

CKeyframe::~CKeyframe(){

    delete[]m_name;
}

CKeyframe::CKeyframe(int trackNum,int time,int transitionTime,char *name,int value){

    m_trackNumber=trackNum;
    m_time=time;
    m_transitionTime=transitionTime;
    if (name!=NULL) {
        m_name=new char[strlen(name)+1];
        strcpy(m_name,name);
    }
    m_value=value;
}

// **********************************************************

CRequest::CRequest(){

    m_type=ANIM_NULL;
    m_currentTime=m_transitionTime=0;
}

CRequest::CRequest(int type,int transitionTime){

    m_type=type;
    m_currentTime=0;
    m_transitionTime=transitionTime;
}

CRequest::~CRequest(){
}

void CRequest::IncrementTimer(int value){

    if (m_currentTime<m_transitionTime) m_currentTime+=value;
}

int CRequest::Completed(){

    if (m_currentTime<m_transitionTime) return ANIM_NULL;
    else return m_type;
}

// **********************************************************

CAction::CAction(){

    m_name="";
}

CAction::~CAction(){

    delete[]m_name;
}

void CAction::SetName(char *name){

    m_name=new char[strlen(name)+1];
    strcpy(m_name,name);
}

char* CAction::GetName(){
    
    return m_name;
}

int CAction::GetMaxTime(){

    return m_maxTime;
}

CKeyframe* CAction::GetCurrentKeyframe(){

    return (CKeyframe*)(m_keyframeList.getnodeinfo());
}

CKeyframe* CAction::GetNextKeyframe(){

    if (!m_keyframeList.gonext()) return NULL;
    else return (CKeyframe*)(m_keyframeList.getnodeinfo());
}

void CAction::InitPosition(){

    m_keyframeList.gohead();
}

bool CAction::ReadDataFile(char *fileName){
    
    // *** temporary variables for reading
    char line[128];
    int  trackNum,time,transitionTime,value;
    char name[255];

    if (fileName==NULL) return false;

    // *** open expression file
    FILE *stream=fopen(fileName,"r");
    if (!stream) return false;

    // *** read in data
    bool header=false;
    while (fgets(line,256,stream)){
        if (line[0]!='#'){
            if (!header) {
                sscanf(line,"%d",&value);
                m_maxTime=value;
                header=true;
            }
            else {
                sscanf(line,"%d",&trackNum);
                if (trackNum==ANIM_EXP){
                    sscanf(line,"%d %d %d %s %d",&trackNum,&time,&transitionTime,name,&value);
                    m_keyframeList.addtail(new CKeyframe(trackNum,time,transitionTime,name,value));
                }
                else if (trackNum<ANIM_EYE){ // *** ANIM_HEAD+i (0<=i<=2)
                    sscanf(line,"%d %d %d %d",&trackNum,&time,&transitionTime,&value);
                    m_keyframeList.addtail(new CKeyframe(trackNum,time,transitionTime,NULL,value));
                }
                else if (trackNum<ANIM_COL){ // *** ANIM_EYE+i  (0<=i<=3)
                    sscanf(line,"%d %d %d %d",&trackNum,&time,&transitionTime,&value);
                    m_keyframeList.addtail(new CKeyframe(trackNum,time,transitionTime,NULL,value));
                }
                else {                       // *** ANIM_COL+i  (0<=i<=MAX_COLOR-1)
                    sscanf(line,"%d %d %d %s",&trackNum,&time,&transitionTime,name);
                    m_keyframeList.addtail(new CKeyframe(trackNum,time,transitionTime,name,-1));
                }
            }
        }
    }
    // *** close file
    fclose(stream);
    
    return true;
}

// **********************************************************
 
CSpeechAction::CSpeechAction(){

    m_position=0;
    m_name="";
    m_intensity=0;
    m_scale=0;
}

CSpeechAction::CSpeechAction(int position,char *name,int intensity,int scale){

    m_position=position;
    m_name=new char[strlen(name)+1];
    strcpy(m_name,name);
    m_intensity=intensity;
    m_scale=scale;
}

CSpeechAction::~CSpeechAction(){

    delete[]m_name;
}

// **********************************************************

CRandomAction::CRandomAction(){

    m_transitionTime=m_timer=m_info=0;
    m_aux1=m_aux2=0;
}

CRandomAction::~CRandomAction(){
}

// **********************************************************

CVariable::CVariable(){

    m_name="";
    m_value="";
}

CVariable::CVariable(char *name,char *value){

    m_name=new char[strlen(name)+1];
    strcpy(m_name,name);
    m_value=new char[strlen(value)+1];
    strcpy(m_value,value);
}

CVariable::~CVariable(){

    delete[]m_name;
    delete[]m_value;
}

void CVariable::SetValue(char *value){

    m_value=new char[strlen(value)+1];
    strcpy(m_value,value);
}
