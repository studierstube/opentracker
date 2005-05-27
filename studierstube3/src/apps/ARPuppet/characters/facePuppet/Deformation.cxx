// Deformation.cpp
//
// stores deformation information - action units - class for the face

#include "Deformation.h"

CActionUnit::CActionUnit(){

    for (int i=0;i<MAX_VERTEX;i++){
        m_translation[i].Set(0.0f,0.0f,0.0f);
    }
}

CActionUnit::~CActionUnit(){
}

CVertex CActionUnit::GetTranslationVector(int index){

    return m_translation[index];
}

bool  CActionUnit::ReadDataFile(char *fileName){
    
    // *** temporary variables for reading
    char line[128];
    // we use the 'serial' parameter just for reading in data, we won't store it!
    int index,faceIndex; 
    float x,y,z;

    if (fileName==NULL) return false;

    // *** open file
    FILE *stream=fopen(fileName,"r");
    if (!stream) return false;

    // *** read in data
    int i=0;
    while (i<MAX_VERTEX && fgets(line,256,stream)){
        if (line[0]!='#'){              
            sscanf(line,"%d %d %f %f %f",&index,&faceIndex,&x,&y,&z);
            m_translation[i].Set(x,y,z);
            i++;
        }           
    }
    
    // *** close file
    fclose(stream);     
    
    return true;
}

//----------------------------------------------------------------------

CExpression::CExpression(){

    for (int i=0;i<MAX_ACTION_UNIT;m_intensity[i++]=0);
    m_name="";
}

CExpression::~CExpression(){

    delete[]m_name;
}

void CExpression::SetName(char *name){

    m_name=new char[strlen(name)+1];
    strcpy(m_name,name);
}

void CExpression::SetIntensity(int actionUnitNumber,int value){

    m_intensity[actionUnitNumber]=value;
}
    
char* CExpression::GetName(){
    
    return m_name;
}

int CExpression::GetIntensity(int actionUnitNumber){

    return m_intensity[actionUnitNumber];
}

bool  CExpression::ReadDataFile(char *fileName){
    
    // *** temporary variables for reading
    char line[128];
    int auNum;
    int value;

    if (fileName==NULL) return false;

    // *** open expression file
    FILE *stream=fopen(fileName,"r");
    if (!stream) return false;

    // *** read in data
    int i=0;
    while (i<MAX_ACTION_UNIT && fgets(line,256,stream)){
        if (line[0]!='#'){
            sscanf(line,"%d %d",&auNum,&value);
            m_intensity[i]=value;
            i++;
        }
    }
    // *** close file
    fclose(stream);
    
    return true;
}

//----------------------------------------------------------------------

CColor::CColor(){

    m_red=m_green=m_blue=0;
    m_name="";
}

CColor::~CColor(){

    if (strlen(m_name)>1) delete[]m_name;
}
    
void CColor::SetName(char *name){

    m_name=new char[strlen(name)+1];
    strcpy(m_name,name);
}

void CColor::SetColor(int red,int green,int blue){

    m_red=red;
    m_green=green;
    m_blue=blue;
}

void CColor::SetColor(CColor *color){

    m_red=color->m_red;
    m_green=color->m_green;
    m_blue=color->m_blue;
}

char* CColor::GetName(){

    return m_name;
}

int CColor::GetRed(){

    return m_red;
}

int CColor::GetGreen(){
    
    return m_green;
}

int CColor::GetBlue(){

    return m_blue;
}

//-------------------------------------------------------------------------------------------

CTransition::CTransition(){

    for (int i=0;i<2;i++){
        m_startValue[i]=m_targetValue[i]=m_startGradient[i]=m_currentGradient[i]=0.0f;
        m_transitionTime[i]=m_time[i]=0;        
    }
    m_blend=false;
}

void CTransition::Reset(){

    for (int i=0;i<2;i++){
        m_startValue[i]=m_targetValue[i]=m_startGradient[i]=m_currentGradient[i]=0.0f;
        m_transitionTime[i]=m_time[i]=0;
    }
    m_blend=false;
}

void CTransition::SetTarget(int target,int T,bool  blend){

    int i=blend ? 1 : 0;
    
    m_startValue[i]=GetValue();
    m_startGradient[i]=(blend && !m_blend) ? m_currentGradient[0] : m_currentGradient[i];
    m_targetValue[i]=(float)target;
    m_time[i]=0;
    m_transitionTime[i]=T;

    if (blend) m_blend=true;
}

void CTransition::SetTimer(int time){

    m_time[0]=time;
    m_time[1]=time;
}

void CTransition::IncrementTimer(int value){

    if (m_blend && m_time[1]>=m_transitionTime[1]){
        
        if (m_time[0]<m_transitionTime[0]){         
            m_startValue[0]=GetValue();
            m_transitionTime[0]=m_transitionTime[0]-m_time[0];
            m_startGradient[0]=m_currentGradient[0]=m_currentGradient[1];
            m_time[0]=0;
        }
        else m_targetValue[0]=GetValue();
        m_blend=false;
    }   
    else {

        if (m_time[0]<m_transitionTime[0]) m_time[0]+=value;
        if (m_blend) m_time[1]+=value;
    }
}

float CTransition::GetValue(){

    float f;

    int i=m_blend ? 1 : 0;

    if (m_time[i]>=m_transitionTime[i]) {
        m_currentGradient[i]=0.0f;
        f=m_targetValue[i];
    }
    else {
        // *** Hermite function
        float t=(float)m_time[i]/(float)m_transitionTime[i];
        m_currentGradient[i]= ( 6*t*t - 6*t    ) * (m_startValue[i]-m_targetValue[i]) +
                              ( 3*t*t - 4*t + 1) * m_startGradient[i];
        f=  ( 2*t*t*t - 3*t*t + 1 )*m_startValue[i]  +
            (-2*t*t*t + 3*t*t     )*m_targetValue[i] +
            (   t*t*t - 2*t*t + t )*m_startGradient[i];
    }

    return f;
}

bool  CTransition::Finished(bool  blend){

    if (blend) return m_time[1]>=m_transitionTime[1];
    else return m_time[0]>=m_transitionTime[0];
}

//----------------------------------------------------------------------

CDeformation::CDeformation(){

    m_initialized=false;
}

bool  CDeformation::Init(char *path){

    char fileName[_MAX_PATH];

    // *** initializing action units
    for (int i=0;i<MAX_ACTION_UNIT;i++){
        sprintf(fileName,"%s\\%s%d.dat",path,FILENAME_BASE_ACTIONUNIT,i);
        if (!m_actionUnit[i].ReadDataFile(fileName)){
            ErrorMessage(ERR_INIT_DEFORMATION_AU);
            m_initialized=false;
            return false;
        }
    }
    
    // *** initializing visemes 
    m_viseme[0].SetName("silence"); // *** initializing silence viseme
    for (int j=1;j<MAX_VISEME;j++){
        // *** we start loading data into m_viseme[1] as m_viseme[0] is an empty
        // *** CExpression which conforms to the "silence" viseme
        sprintf(fileName,"%s\\%s%d.dat",path,FILENAME_BASE_VISEME,j-1);
        if (!m_viseme[j].ReadDataFile(fileName)){
            ErrorMessage(ERR_INIT_DEFORMATION_VIS);
            m_initialized=false;
            return false;
        }
        else m_viseme[j].SetName(visemeString[j]);
    }

    // *** load user-defined data
    sprintf(fileName,"%s\\%s",path,FILENAME_BASE_EXPRESSION);
    if (!LoadExpressions(fileName)){    // *** expressions
        ErrorMessage(ERR_INIT_DEFORMATION_EXP);
        return false;
    }

    sprintf(fileName,"%s\\%s",path,FILENAME_COLOR);
    if (!LoadColors(fileName)){         // *** colors
        ErrorMessage(ERR_INIT_DEFORMATION_COLOR);
        return false;
    }

    m_initialized=true;

    return true;
}

bool CDeformation::LoadExpressions(char *fileNameBase){

    char fileName[_MAX_PATH];
    WIN32_FIND_DATA findFileData;
    HANDLE hFindFile;

    sprintf(fileName,"%s*.exp",fileNameBase);
    hFindFile=FindFirstFile(fileName,&findFileData);

    if (hFindFile!=INVALID_HANDLE_VALUE){
        
        // *** clean up previous expression list
        m_expressionList.removeall();

        // *** add neutral expression
        CExpression *neutralExp=new CExpression();
        neutralExp->SetName("neutral");
        m_expressionList.addtail(neutralExp);

        // *** add new expression to the expression list
        do {
            CExpression *expression=new CExpression();
            char name[255],shortname[255],longname[255];
            strcpy(name,findFileData.cFileName);
            strncpy(shortname,name,strlen(name)-4);
            shortname[strlen(name)-4]='\0';
            expression->SetName(shortname);
            strcpy(longname,fileNameBase);
            strcat(longname,findFileData.cFileName);
            expression->ReadDataFile(longname);
            m_expressionList.addtail(expression);
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

bool  CDeformation::LoadColors(char *fileName){

    // *** temporary variables for reading
    char line[128]; 
    char name[255];
    int r,g,b;

    // *** open vertex file
    FILE *stream=fopen(fileName,"r");
    if (!stream) return false;
    
    // *** clean up previous expression list
    m_colorList.removeall();

    while (fgets(line,256,stream)){
        if (line[0]!='#'){      
            sscanf(line,"%s %d %d %d",&name,&r,&g,&b);
            CColor *color=new CColor();
            color->SetName(name);
            color->SetColor(r,g,b);
            m_colorList.addtail(color);
        }
    }
    fclose(stream);

    return true;
}

CVertex CDeformation::GetDeformationVector(int index,int* actionUnitAmount){

    CVertex vector;

    for (int i=0;i<MAX_ACTION_UNIT;i++){
        vector+=m_actionUnit[i].GetTranslationVector(index)*((float)actionUnitAmount[i]/100.0f);
    }

    return vector;
}

CExpression* CDeformation::GetExpression(const char *name){

    CExpression *expression=NULL;

    m_expressionList.gohead();
    do {
        if (!strcmp(((CExpression*)m_expressionList.getnodeinfo())->GetName(),name))
            // *** If name matches, we found the expression that was searched for.
            // *** If cycle ends, then there's no expression under that name and the value
            // *** of 'expression' remains the default i.e. and empty CExpression() object
            expression=(CExpression*)(m_expressionList.getnodeinfo());
    }
    while (m_expressionList.gonext());

    return expression;
}

CExpression* CDeformation::GetViseme(int num){

    CExpression *viseme=new CExpression();

    // *** If the viseme number is out of range then the value of 'viseme' remains 
    // *** the default i.e. and empty CExpression() object
    if (num>=0 && num<MAX_VISEME) {
        viseme=&(m_viseme[num]);
    }

    return viseme;
}

CColor* CDeformation::GetColor(const char *name){

    CColor *color=new CColor();

    m_colorList.gohead();
    do {
        if (!strcmp(((CColor*)m_colorList.getnodeinfo())->GetName(),name))
            // *** If name matches, we found the color that was searched for.
            // *** If cycle ends, then there's no color under that name and the value
            // *** of 'color' remains the default i.e. and empty CColor() object
            color=(CColor*)m_colorList.getnodeinfo();
    }
    while (m_colorList.gonext());

    return color;
}

void CDeformation::AddColor(char *name,int r,int g,int b){

    CColor *color=new CColor();
    color->SetName(name);
    color->SetColor(r,g,b);
    m_colorList.addtail(color);
}