// Behavior.cpp
//
// behavior engine class controlling objects in the scene

#include "Behavior.h"

CBehavior::CBehavior(){

    m_currentAction=NULL;
    m_actionTimer=0;
    m_actionIntensity=0;
    m_actionTimeScale=0;

    m_actionList.removeall();
    m_speechActionList.removeall();

    strcpy(m_prevExpName,"neutral");
    m_prevExpValue=0;
    for (int j=0;j<3;m_prevEyeValue[j++]=0);
    for (int k=0;k<4;m_prevHeadValue[k++]=0);
    for (int l=0;l<MAX_COLOR;strcpy(m_prevColName[l++],""));

    srand((unsigned int)time(NULL)); // *** random seed for pseudo-random number generation

    strcpy(m_conformation,"");

    m_initialized=false;
    m_visible=false;
    m_puppeteer=false;

    m_idleTime=0.0f;

    m_agentID=0;

    m_finishCBData=NULL;
    m_finishCB=NULL;
}

CBehavior::~CBehavior(){

    delete m_currentAction;
}

bool CBehavior::Init(){
    
    char path[_MAX_PATH];
    sprintf(path,"%s\\%s",getenv("STBROOT"),FILENAME_BASE_STB);

    // *** initializing agent
    if (!m_agent.Init(path)){
        m_initialized=false;
        return false;
    }
    
    // *** load user-defined actions
    char fileName[_MAX_PATH];
    sprintf(fileName,"%s\\%s",path,FILENAME_BASE_ACTION);
    if (!LoadActions(fileName)){
        ErrorMessage(ERR_INIT_BEHAVIOR_ACT);
    }

    // *** init speech
    m_speech.Init(this);
    strcpy(m_text,"");

    m_initialized=true;

    return true;
}

bool CBehavior::LoadActions(char *fileNameBase){

    char fileName[_MAX_PATH];
    WIN32_FIND_DATA findFileData;
    HANDLE hFindFile;

    sprintf(fileName,"%s*.act",fileNameBase);
    hFindFile=FindFirstFile(fileName,&findFileData);

    if (hFindFile!=INVALID_HANDLE_VALUE){
        
        // *** clean up previous expression list
        m_actionList.removeall();

        // *** add new action to the action list
        do {
            CAction *action=new CAction();
            char name_extension[_MAX_PATH];
            strcpy(name_extension,findFileData.cFileName); // convert TCHAR* to char*
            char name[_MAX_PATH];
            strncpy(name,name_extension,strlen(name_extension)-4);
            name[strlen(name_extension)-4]='\0';
            action->SetName(name);
            char longname[_MAX_PATH];
            strcpy(longname,fileNameBase);
            strcat(longname,name_extension);
            action->ReadDataFile(longname);

            m_actionList.addtail(action);
        } 
        while (FindNextFile(hFindFile,&findFileData));

        if (GetLastError()!=ERROR_NO_MORE_FILES){
            return false;
        }
        FindClose(hFindFile);
    }
    else {
        return false;
    }

    return true;
}

CAnimation* CBehavior::GetAgent(){

    return &m_agent;
}

CSpeech* CBehavior::GetSpeech(){

    return &m_speech;
}

char* CBehavior::GetSpeechText(){

    return m_text;
}

CAction* CBehavior::GetAction(const char *name){

    if (!m_actionList.isempty()){
        m_actionList.gohead();
        do{
            if (!strcmp(((CAction*)m_actionList.getnodeinfo())->GetName(),name)) 
                return (CAction*)(m_actionList.getnodeinfo());
        }
        while (m_actionList.gonext());
    }

    return NULL;
}

bool CBehavior::Draw(float zoom,bool smoothEnabled,bool coloringEnabled){

    if (m_visible) return m_agent.Draw(zoom,smoothEnabled,coloringEnabled);
    else return false;
}

void CBehavior::Animate(){

    if (!m_initialized) return;

    m_speech.SpeechEvent();

    if (!m_speechActionList.isempty()) {
        m_speechActionList.gohead();
        CSpeechAction *currentSpeechAction=(CSpeechAction*)m_speechActionList.getnodeinfo();
        /*if (currentSpeechAction->m_position<=m_speech.GetSpeechPosition()){
            this->Play(currentSpeechAction->m_name,currentSpeechAction->m_intensity,
                currentSpeechAction->m_scale);
            m_speechActionList.removehead();
        }*/
    }

    // *** execute action
    if (m_currentAction!=NULL){
        CKeyframe *keyframe=m_currentAction->GetCurrentKeyframe();
        while (keyframe!=NULL && keyframe->m_time*m_actionTimeScale/100<=m_actionTimer){
            if (keyframe->m_trackNumber==ANIM_EXP){
                this->SetExpression(
                    !strcmp(keyframe->m_name,"_prev_") ? m_prevExpName : keyframe->m_name,
                    keyframe->m_value==-100 ? m_prevExpValue : keyframe->m_value*m_actionIntensity/100,
                    keyframe->m_transitionTime*m_actionTimeScale/100,
                    false
                );
            }
            else if (keyframe->m_trackNumber<ANIM_EYE){ // *** ANIM_HEAD+i (0<=i<=2)
                this->SetHead(
                    keyframe->m_trackNumber-ANIM_HEAD,
                    keyframe->m_value==-100 ? m_prevHeadValue[keyframe->m_trackNumber-ANIM_HEAD] 
                        : keyframe->m_value*m_actionIntensity/100,
                    keyframe->m_transitionTime*m_actionTimeScale/100,
                    false
                );
            }
            else if (keyframe->m_trackNumber<ANIM_COL){ // *** ANIM_EYE+i  (0<=i<=3)
                this->SetEye(
                    keyframe->m_trackNumber-ANIM_EYE,
                    keyframe->m_value==-100 ? m_prevEyeValue[keyframe->m_trackNumber-ANIM_EYE] 
                        : keyframe->m_value*m_actionIntensity/100,
                    keyframe->m_transitionTime*m_actionTimeScale/100,
                    false
                );
            }
            else {                                      // *** ANIM_COL+i  (0<=i<=MAX_COLOR-1)
                this->SetColor(
                    keyframe->m_trackNumber-ANIM_COL,
                    !strcmp(keyframe->m_name,"_prev_") ? m_prevColName[keyframe->m_trackNumber-ANIM_COL]
                        : keyframe->m_name,
                    keyframe->m_transitionTime*m_actionTimeScale/100,
                    false
                );
            }
            keyframe=m_currentAction->GetNextKeyframe();
        }
        if (keyframe==NULL) m_currentAction=NULL;
    } // end of if m_currentAction

    // *** lip sync
    if (m_speech.GetLipSync()) LipSync();

    // *** increment timer
    m_timerCurrent=(double)clock();
    m_agent.IncrementTimer((int)(m_timerCurrent-m_timerStart));
    if (m_currentAction!=NULL) {
        m_idleTime=0;
        m_actionTimer+=(int)(m_timerCurrent-m_timerStart);
    }
    else {
        // *** generate automatic behavior
        AutomaticBehavior((int)(m_timerCurrent-m_timerStart));
    }

    // *** maintain request list
    int type;

    m_requestList.gohead();
    if (!m_requestList.isempty()){
        do {
            ((CRequest*)m_requestList.getnodeinfo())->IncrementTimer((int)(m_timerCurrent-m_timerStart));
            type=((CRequest*)m_requestList.getnodeinfo())->Completed();
            if (type!=ANIM_NULL){
                m_requestList.removecurrent();
                FireRequestComplete(type);
                return;
            }
        }
        while (m_requestList.gonext());
    }

    // *** for the next cycle
    m_timerStart=m_timerCurrent;
}

void CBehavior::AutomaticBehavior(int value){

    // *** blinking
    switch (m_randomAction[RND_BLINK].m_info){
        case 0:
            m_randomAction[RND_BLINK].m_transitionTime=rand()*10000/RAND_MAX;
            m_randomAction[RND_BLINK].m_timer=0;
            m_randomAction[RND_BLINK].m_info=1;
            break;
        case 1:
            if (m_randomAction[RND_BLINK].m_timer>=m_randomAction[RND_BLINK].m_transitionTime){
                m_randomAction[RND_BLINK].m_transitionTime=150;
                m_randomAction[RND_BLINK].m_timer=0;
                m_randomAction[RND_BLINK].m_info=2;
                m_agent.SetActionUnitTarget(6,100,
                    m_randomAction[RND_BLINK].m_transitionTime,true);
            }
            break;
        case 2:
            if (m_randomAction[RND_BLINK].m_timer>=m_randomAction[RND_BLINK].m_transitionTime){
                m_randomAction[RND_BLINK].m_transitionTime=50;
                m_randomAction[RND_BLINK].m_timer=0;
                m_randomAction[RND_BLINK].m_info=3;
            }
            break;
        case 3:
            if (m_randomAction[RND_BLINK].m_timer>=m_randomAction[RND_BLINK].m_transitionTime){
                m_randomAction[RND_BLINK].m_transitionTime=100;
                m_randomAction[RND_BLINK].m_timer=0;
                m_randomAction[RND_BLINK].m_info=4;
                m_agent.SetActionUnitTarget(6,0,
                    m_randomAction[RND_BLINK].m_transitionTime,true);
            }
            break;
        case 4:
            if (m_randomAction[RND_BLINK].m_timer>=m_randomAction[RND_BLINK].m_transitionTime){
                m_randomAction[RND_BLINK].m_timer=0;
                m_randomAction[RND_BLINK].m_info=0;
                m_randomAction[RND_BLINK].m_transitionTime=0;
            }
            break;
    }

    if (!m_speech.IsSpeaking() && m_requestList.isempty()){

        // *** swallow
        switch (m_randomAction[RND_SWALLOW].m_info){
            case 0:
                m_randomAction[RND_SWALLOW].m_transitionTime=rand()*30000/RAND_MAX;
                m_randomAction[RND_SWALLOW].m_timer=0;
                m_randomAction[RND_SWALLOW].m_info=1;
                break;
            case 1:
                if (m_randomAction[RND_SWALLOW].m_timer>=m_randomAction[RND_SWALLOW].m_transitionTime){
                    m_randomAction[RND_SWALLOW].m_transitionTime=800;
                    m_randomAction[RND_SWALLOW].m_timer=0;
                    m_randomAction[RND_SWALLOW].m_info=2;
                    m_agent.SetActionUnitTarget(22,30,
                        m_randomAction[RND_SWALLOW].m_transitionTime,true);
                }
                break;
            case 2:
                if (m_randomAction[RND_SWALLOW].m_timer>=m_randomAction[RND_SWALLOW].m_transitionTime){
                    m_randomAction[RND_SWALLOW].m_transitionTime=300;
                    m_randomAction[RND_SWALLOW].m_timer=0;
                    m_randomAction[RND_SWALLOW].m_info=3;
                }
                break;
            case 3:
                if (m_randomAction[RND_SWALLOW].m_timer>=m_randomAction[RND_SWALLOW].m_transitionTime){
                    m_randomAction[RND_SWALLOW].m_transitionTime=100;
                    m_randomAction[RND_SWALLOW].m_timer=0;
                    m_randomAction[RND_SWALLOW].m_info=4;
                    m_agent.SetActionUnitTarget(22,0,
                        m_randomAction[RND_SWALLOW].m_transitionTime,true);
                }
                break;
            case 4:
                if (m_randomAction[RND_SWALLOW].m_timer>=m_randomAction[RND_SWALLOW].m_transitionTime){
                    m_randomAction[RND_SWALLOW].m_timer=0;
                    m_randomAction[RND_SWALLOW].m_info=0;
                    m_randomAction[RND_SWALLOW].m_transitionTime=0;
                }
                break;
        }

        if (!m_puppeteer){

            // *** mouth
            switch (m_randomAction[RND_MOUTH].m_info){
                case 0:
                    m_randomAction[RND_MOUTH].m_transitionTime=1000+rand()*2000/RAND_MAX;
                    m_randomAction[RND_MOUTH].m_timer=0;
                    m_randomAction[RND_MOUTH].m_info=1;
                    m_agent.SetActionUnitTarget(9,rand()*100/RAND_MAX,
                            m_randomAction[RND_MOUTH].m_transitionTime,true);
                    break;
                case 1:
                    if (m_randomAction[RND_MOUTH].m_timer>=m_randomAction[RND_MOUTH].m_transitionTime){
                        m_randomAction[RND_MOUTH].m_info=0;
                        m_randomAction[RND_MOUTH].m_transitionTime=0;
                    }
                    break;
            }

            // *** eyebrow
            int random=0;
            switch (m_randomAction[RND_EYEBROW].m_info){
                case 0:
                    m_randomAction[RND_EYEBROW].m_transitionTime=rand()*2000/RAND_MAX;
                    m_randomAction[RND_EYEBROW].m_timer=0;
                    random=rand()*100/RAND_MAX; 
                    if (random<30) {
                        m_randomAction[RND_EYEBROW].m_info=1;
                        m_agent.SetActionUnitTarget(0,rand()*30/RAND_MAX,
                            m_randomAction[RND_EYEBROW].m_transitionTime,true);
                    }
                    else if (random<60) {
                        m_randomAction[RND_EYEBROW].m_info=2;
                        m_agent.SetActionUnitTarget(2,rand()*30/RAND_MAX,
                            m_randomAction[RND_EYEBROW].m_transitionTime,true);
                    }
                    else m_randomAction[RND_EYEBROW].m_info=3;
                    break;
                case 1:
                case 2:
                case 3:
                    if (m_randomAction[RND_EYEBROW].m_timer>=m_randomAction[RND_EYEBROW].m_transitionTime){
                        if (m_randomAction[RND_EYEBROW].m_info<3){
                            m_agent.SetActionUnitTarget(m_randomAction[RND_EYEBROW].m_info==1 ? 0 : 2,
                                rand()*30/RAND_MAX, m_randomAction[RND_EYEBROW].m_transitionTime,true);
                        }
                        m_randomAction[RND_EYEBROW].m_info=4;
                    }
                    break;
                case 4:
                    if (m_randomAction[RND_EYEBROW].m_timer>=m_randomAction[RND_EYEBROW].m_transitionTime){             
                        m_randomAction[RND_EYEBROW].m_info=0;
                        m_randomAction[RND_EYEBROW].m_transitionTime=0;
                    }
                    break;
            }
        } // *** endif (!m_puppeteer && m_requestList)

        // *** eye position
        if (m_idleTime>=IDLE_TIME && !(m_randomAction[RND_EYE].m_info) && !(m_randomAction[RND_ACTION].m_info)){
            m_idleTime=0;
            m_randomAction[RND_EYE].m_info=1;
            m_randomAction[RND_EYE].m_transitionTime=1000+rand()*600/RAND_MAX;
            m_randomAction[RND_EYE].m_timer=0;
            int rndAlpha=-10+rand()*20/RAND_MAX;
            int rndBeta=-10+rand()*20/RAND_MAX;
            this->SetEye(0,rndAlpha,m_randomAction[RND_EYE].m_transitionTime,false);
            this->SetEye(1,rndBeta,m_randomAction[RND_EYE].m_transitionTime,false);
            this->SetEye(2,rndAlpha,m_randomAction[RND_EYE].m_transitionTime,false);
            this->SetEye(3,rndBeta,m_randomAction[RND_EYE].m_transitionTime,false);
        }
        else if (m_randomAction[RND_EYE].m_info==1 && 
            m_randomAction[RND_EYE].m_timer>=m_randomAction[RND_EYE].m_transitionTime){
            m_randomAction[RND_EYE].m_info=2;
            m_randomAction[RND_EYE].m_transitionTime=2000+rand()*5000/RAND_MAX;
            m_randomAction[RND_EYE].m_timer=0;          
        }
        else if (m_randomAction[RND_EYE].m_info==2 &&
            m_randomAction[RND_EYE].m_timer>=m_randomAction[RND_EYE].m_transitionTime){
            m_randomAction[RND_EYE].m_info=3;
            m_randomAction[RND_EYE].m_transitionTime=700+rand()*600/RAND_MAX;;
            m_randomAction[RND_EYE].m_timer=0;
            for (int k=0;k<4;k++) this->SetEye(k,0,m_randomAction[RND_EYE].m_transitionTime,false);
        }
        else if (m_randomAction[RND_EYE].m_info==3 && 
            m_randomAction[RND_EYE].m_timer>=m_randomAction[RND_EYE].m_transitionTime){
            m_randomAction[RND_EYE].m_info=4;
            m_randomAction[RND_EYE].m_transitionTime=500+rand()*2000/RAND_MAX;
            m_randomAction[RND_EYE].m_timer=0;          
        }
        else if (m_randomAction[RND_EYE].m_info==4 &&
            m_randomAction[RND_EYE].m_timer>=m_randomAction[RND_EYE].m_transitionTime){
            m_randomAction[RND_EYE].m_info=0;
            m_randomAction[RND_EYE].m_timer=0;          
        }

        // *** head position
        if (m_idleTime>=IDLE_TIME && !(m_randomAction[RND_HEAD].m_info)) {
            m_randomAction[RND_HEAD].m_info=1;
            m_randomAction[RND_HEAD].m_transitionTime=400+rand()*300/RAND_MAX;
            m_randomAction[RND_HEAD].m_timer=0;         
            for (int l=0;l<2;l++) {
                m_randomAction[RND_HEAD].m_aux1=-4+rand()*8/RAND_MAX;
                m_randomAction[RND_HEAD].m_aux2=-4+rand()*8/RAND_MAX;
                this->SetHead(l,
                    !l ? m_randomAction[RND_HEAD].m_aux1 : m_randomAction[RND_HEAD].m_aux2,
                    m_randomAction[RND_HEAD].m_transitionTime,false);
            }
        }
        else if (m_randomAction[RND_HEAD].m_info==1 &&
            m_randomAction[RND_HEAD].m_timer>=m_randomAction[RND_HEAD].m_transitionTime){
            m_randomAction[RND_HEAD].m_info=2;
            m_randomAction[RND_HEAD].m_transitionTime=2000+rand()*2000/RAND_MAX;
            m_randomAction[RND_HEAD].m_timer=0;
        }
        else if (m_randomAction[RND_HEAD].m_info==2) {
            if (m_randomAction[RND_HEAD].m_timer>=m_randomAction[RND_HEAD].m_transitionTime){
                m_randomAction[RND_HEAD].m_info=3;
                m_randomAction[RND_HEAD].m_timer=0;
                m_randomAction[RND_HEAD].m_transitionTime=500+rand()*1000/RAND_MAX;
                for (int l=0;l<2;l++) {
                    this->SetHead(l,0,m_randomAction[RND_HEAD].m_transitionTime,false);
                }
            }
            else {
                for (int n=0;n<2;n++) {
                    this->SetHead(n,
                        !n ? m_randomAction[RND_HEAD].m_aux1+(-1+rand()*2/RAND_MAX) : 
                        m_randomAction[RND_HEAD].m_aux2+(-1+rand()*2/RAND_MAX),
                        500,false);
                }
            }
        }
        else if (m_randomAction[RND_HEAD].m_info==3){
            if (m_randomAction[RND_HEAD].m_timer>=m_randomAction[RND_EYE].m_transitionTime){
                m_randomAction[RND_HEAD].m_info=0;
                m_randomAction[RND_HEAD].m_timer=0;         
            }
            else {
                for (int p=0;p<2;p++) {
                    this->SetHead(p,(-1+rand()*2/RAND_MAX),500,false);
                }
            }
        }

    } // end isSpeaking

    for (int i=0;i<MAX_RND_ACTION;i++){
        if (m_randomAction[i].m_transitionTime>0) m_randomAction[i].m_timer+=value;
    }
    if (m_idleTime<IDLE_TIME) m_idleTime+=0.04f;
}

char* CBehavior::ExtractActionTags(char *text){
    
    char *parseString=text;
    int posBegin=-1;
    int posIntensity=-1;
    int posScale=-1;
    int posEnd=-1;
    char *strName,*strIntensity,*strScale,*strPosition,*temp,*temp2;

    int intensity=0;
    int scale=0;

    m_speechActionList.removeall();
    while ((strPosition=strstr(parseString,"\\act='"))!=NULL){
        posBegin=strPosition-parseString;

        strPosition=strstr(parseString,"' in=");
        posIntensity=strPosition-parseString;
        
        strPosition=strstr(parseString," sc=");
        posScale=strPosition-parseString;
        
        strPosition=strstr(parseString+posBegin+1,"\\");
        posEnd=(strPosition-parseString);
        
        strName=new char[posIntensity-(posBegin+6)+1];
        strncpy(strName,parseString+(posBegin+6),posIntensity-(posBegin+6));
        strName[posIntensity-(posBegin+6)]='\0';

        strIntensity=new char[posScale-(posIntensity+5)+1];
        strncpy(strIntensity,parseString+(posIntensity+5),posScale-(posIntensity+5));
        strIntensity[posScale-(posIntensity+5)]='\0';

        strScale=new char[posEnd-(posScale+4)+1];
        strncpy(strScale,parseString+(posScale+4),posEnd-(posScale+4));
        strScale[posEnd-(posScale+4)]='\0';

        sscanf((const char*)strIntensity,"%d",&intensity);
        sscanf((const char*)strScale,"%d",&scale);
//      cout << "*** name: " << strName << " int: " << intensity << " sc: " << scale;

        m_speechActionList.addtail(new CSpeechAction(posBegin,strName,intensity,scale));

        temp=new char[posBegin+1];
        temp2=new char[strlen(parseString)-posEnd];
        strncpy(temp,parseString,posBegin);
        if (strlen(parseString)>(unsigned int)posEnd+1){
            strncpy(temp2,parseString+posEnd+1,strlen(parseString)-posEnd-1);
        }
        temp[posBegin]='\0';
        temp2[strlen(parseString)-posEnd-1]='\0';
        strcat(temp,temp2);
        parseString=new char[posBegin+strlen(parseString)-posEnd];
        strcpy(parseString,temp);
//      cout << parseString << "\n";
    }

    return parseString;
}

void CBehavior::LipSync(){

    m_agent.SetVisemeTarget(
        m_speech.GetNextViseme(),
        60-m_speech.GetRate()*4,
        m_speech.GetDuration(),
        false
    );

    m_speech.SetLipSync(false);
}

void CBehavior::FireRequestComplete(int animationType){

    if (m_finishCB!=NULL){
        m_finishCB(m_finishCBData,animationType);
    }
}

void CBehavior::SetAgentID(int id){

    m_agentID=id;
}

void CBehavior::SetPuppeteer(bool puppeteer){

    m_puppeteer=puppeteer;
}

bool CBehavior::GetPuppeteer(){

    return m_puppeteer;
}

void CBehavior::RemoveTtsTags(char *text){

    char *parseString=new char[strlen(text)+1];
    strcpy(parseString,text);
    
    char *strPosition;
    int posBegin=-1;
    int posEnd=-1;
    
    while ((strPosition=strstr(parseString,"<"))!=NULL){
        posBegin=strPosition-parseString;
        strPosition=strstr(parseString+posBegin+1,">");
        posEnd=strPosition-parseString;
        char *temp=new char[posBegin+1];
        char *temp2=new char[strlen(parseString)-posEnd];
        strncpy(temp,parseString,posBegin);
        if (strlen(parseString)>(unsigned int)posEnd+1){
            strncpy(temp2,parseString+posEnd+1,strlen(parseString)-posEnd-1);
        }
        temp[posBegin]='\0';
        temp2[strlen(parseString)-posEnd-1]='\0';
        strcat(temp,temp2);
        parseString=new char[posBegin+strlen(parseString)-posEnd];
        strcpy(parseString,temp);
//      cout << parseString << "\n";
    }

    strcpy(text,parseString);
    delete[]parseString;
    delete[]strPosition;
}

void CBehavior::GetMinMax(float *min,float *max){

    m_agent.GetMinMax(min,max);
}

void CBehavior::setFinishCallback(SoFinishCB *cb, void* data){

    this->m_finishCB=cb;
    this->m_finishCBData=data;
}

// *** high-level agent commands ---------------------------------

void CBehavior::Show() {

    if (!m_initialized) return;

    if (!m_visible){
        m_visible=true;
        m_timerStart=0;
        m_timerCurrent=0;
        m_idleTime=0;
    }
}

void CBehavior::Hide() {

    if (!m_initialized) return;

    m_visible=false;
    this->ShutUp();
}

void CBehavior::Play(const char *name,int intensity,int timeScale){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_PLAY);
        return;
    }

    m_currentAction=this->GetAction(name);
    if (m_currentAction==NULL) {
        FireRequestComplete(ANIM_PLAY);
    }
    else {
        m_actionIntensity=intensity;
        m_actionTimeScale=timeScale;
        m_currentAction->InitPosition();
        m_actionTimer=0;

        m_requestList.addhead(new CRequest(ANIM_PLAY,m_currentAction->GetMaxTime()*m_actionTimeScale/100));
    }

    m_idleTime=0;
}

int CBehavior::Speak(const char *text){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_SPEAK);
        return ANIM_SPEAK;
    }

    for (int i=0;i<4;i++) this->SetEye(i,0,200,false);
    if (!m_speech.IsInitialized()){
        Think(text);
        return ANIM_THINK;
    }
    m_speech.Speak(text);
    strcpy(m_text,text);
    RemoveTtsTags(m_text);

    m_idleTime=0;

    return ANIM_SPEAK;
}

void CBehavior::Sound(const char *name){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_SND);
        return;
    }

    char fileName[_MAX_PATH];
    sprintf(fileName,"%s\\%s\\%s%s",getenv("STBROOT"),FILENAME_BASE_STB,FILENAME_BASE_VOICE,name);
    m_speech.PlayWAV(fileName);

/*  if (strlen(name)>=4) {
        name[strlen(name)-4]='\0';
        this->Play(name);
    }*/
}

void CBehavior::Think(const char *text){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_THINK);
        return;
    }

    strcpy(m_text,text);
//  for (int i=0;i<4;i++) this->SetEye(i,0,200,false);

    m_requestList.addhead(new CRequest(ANIM_THINK,strlen(m_text)*100));
}

void CBehavior::ShutUp(){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_SHUT);
        return;
    }

    m_speech.ShutUp();
    this->SetViseme(0,100,200);
    FireRequestComplete(ANIM_SHUT);
}

void CBehavior::TurnToUser(int transitionTime){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_EYE);
        return;
    }

    this->SetEye(0,0,transitionTime);
    this->SetEye(1,0,transitionTime,false);
    this->SetEye(2,0,transitionTime,false);
    this->SetEye(3,0,transitionTime,false);

    this->SetHead(0,0,transitionTime,false);
    this->SetHead(1,0,transitionTime,false);
}

void CBehavior::TurnFromUser(int transitionTime){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_EYE);
        return;
    }

    int rndEyeAlpha=-10+rand()*20/RAND_MAX;
    int rndEyeBeta=-10+rand()*20/RAND_MAX;
    int rndHeadAlpha=-4+rand()*8/RAND_MAX;
    int rndHeadBeta=-4+rand()*8/RAND_MAX;

    this->SetEye(0,rndEyeAlpha,transitionTime);
    this->SetEye(1,rndEyeBeta,transitionTime,false);
    this->SetEye(2,rndEyeAlpha,transitionTime,false);
    this->SetEye(3,rndEyeBeta,transitionTime,false);

    this->SetHead(0,rndHeadAlpha,transitionTime,false);
    this->SetHead(1,rndHeadBeta,transitionTime,false);
}

void CBehavior::SetVariable(const char *name,const char *value){

/*  for (int i=0;i<m_variableList.GetSize();i++){
        if (!strcmp(m_variableList.GetAt(i)->m_name,name)) {
            m_variableList.GetAt(i)->SetValue(value);
            return;
        }
    }

    m_variableList.Add(new CVariable(name,value));
*/
}

char* CBehavior::GetVariable(const char *name){

/*  for (int i=0;i<m_variableList.GetSize();i++){
        if (!strcmp(m_variableList.GetAt(i)->m_name,name)) {
            return m_variableList.GetAt(i)->m_value;
        }
    }
*/
    return NULL;
}

void CBehavior::SetConformation(const char *name){

    if (!m_initialized || !strlen(name)) return;

    char fileName[_MAX_PATH];
    sprintf(fileName,"%s\\%s\\%s\\%s.cof",getenv("STBROOT"),FILENAME_BASE_STB,FILENAME_BASE_CONFORMATION,name);
    bool result=m_agent.LoadConformation(fileName);
    for (int i=0;i<MAX_COLOR;strcpy(m_prevColName[i++],""));
    if (result) m_speech.SetTTSModeID(m_agent.GetTTSModeID());
}

char* CBehavior::GetConformation(){

    return m_conformation;
}

// *** low-level agent commands ---------------------------------

void CBehavior::SetActionUnit(int number,int intensity,int transitionTime,bool noninternal){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_AU1+number);
        return;
    }

    m_agent.SetActionUnitTarget(number,intensity,transitionTime);
    if (noninternal) m_requestList.addhead(new CRequest(ANIM_AU1+number,transitionTime));
}

void CBehavior::SetExpression(const char *name,int intensity,int transitionTime,bool noninternal){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_EXP);
        return;
    }

    m_agent.SetExpressionTarget(name,intensity,transitionTime);
    strcpy(m_prevExpName,name);
    m_prevExpValue=intensity;
    if (noninternal) m_requestList.addhead(new CRequest(ANIM_EXP,transitionTime));
}

void CBehavior::SetViseme(int number,int intensity,int transitionTime,bool noninternal){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_VIS);
        return;
    }

    m_agent.SetVisemeTarget(number,intensity,transitionTime);
    if (noninternal) m_requestList.addhead(new CRequest(ANIM_VIS,transitionTime));
}

void CBehavior::SetColor(int number,const char *name,int transitionTime,bool noninternal){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_COL+number);
        return;
    }

    m_agent.SetColorTarget(number,name,transitionTime);
    strcpy(m_prevColName[number],name);
    if (noninternal) m_requestList.addhead(new CRequest(ANIM_COL+number,transitionTime));
}

void CBehavior::SetColorRGB(int number,int r,int g,int b,int transitionTime,bool noninternal){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_COL+number);
        return;
    }

    m_agent.SetColorTarget(number,r,g,b,transitionTime);
    if (noninternal) m_requestList.addhead(new CRequest(ANIM_COL+number,transitionTime));
}

void CBehavior::SetEye(int number,int value,int transitionTime,bool noninternal){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_EYE+number);
        return;
    }

    m_agent.SetEyeTarget(number,value,transitionTime);
    
    // *** adjust eyelid to vertical eye position
    if (number==0 || number==2) {
        if (value>=0) {
            m_agent.SetActionUnitTarget(3,0,transitionTime);
            m_agent.SetActionUnitTarget(5,(int)((float)value*70.0f/30.0f),transitionTime);
        }
        else {
            m_agent.SetActionUnitTarget(3,-(int)((float)value*70.0f/30.0f),transitionTime);
            m_agent.SetActionUnitTarget(5,0,transitionTime);
        }
    }

    m_prevEyeValue[number]=value;
    if (noninternal) m_requestList.addhead(new CRequest(ANIM_EYE+number,transitionTime));
}

void CBehavior::SetHead(int number,int value,int transitionTime,bool noninternal){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_HEAD+number);
        return;
    }

    m_agent.SetHeadTarget(number,value,transitionTime);
    m_prevHeadValue[number]=value;
    if (noninternal) m_requestList.addhead(new CRequest(ANIM_HEAD+number,transitionTime));
}

void CBehavior::BlendExpressions(const char *exp1,const char *exp2,int factor,
                                 int intensity,int transitionTime,bool noninternal){

    if (!m_initialized || !m_visible) {
        FireRequestComplete(ANIM_EXP);
        return;
    }

    m_agent.BlendExpressions(exp1,exp2,factor,intensity,transitionTime);
    if (noninternal) m_requestList.addhead(new CRequest(ANIM_EXP,transitionTime));
}
