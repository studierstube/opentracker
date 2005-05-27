/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* SoFacePuppet.cxx contains the implementation of class SoFacePuppet,
 * an affective talking head puppet
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoFacePuppet.cxx 3729 2004-09-19 21:49:18Z bara $
 * @file
 */
/* ======================================================================== */

#undef DEBUG_ARPUPPET

#include <Inventor/nodes/SoCube.h>
#include <Inventor/elements/SoComplexityTypeElement.h>

#include "SoFacePuppet.h"

SO_NODE_SOURCE(SoFacePuppet);

SoFacePuppet::SoFacePuppet()
{
    SO_NODE_CONSTRUCTOR(SoFacePuppet);

    SO_NODE_ADD_FIELD(animation, (ANIM_NULL));
    SO_NODE_ADD_FIELD(parameter, (""));
    SO_NODE_ADD_FIELD(intensity, (0));
    SO_NODE_ADD_FIELD(duration, (0));
    SO_NODE_ADD_FIELD(trigger, ());
    SO_NODE_ADD_FIELD(character, (""));
    SO_NODE_ADD_FIELD(speechText, (""));
    SO_NODE_ADD_FIELD(finishedAnimation,());
    SO_NODE_ADD_FIELD(finishedAnimationData,(ANIM_NULL));

    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_NULL);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_PLAY);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_SPEAK);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_SND);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_SHUT);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_THINK);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_EXP);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_VIS);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_HEAD_VER);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_HEAD_HOR);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_HEAD_ZOOM);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_EYE_L_VER);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_EYE_L_HOR);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_EYE_R_VER);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_EYE_R_HOR);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_FACE);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_LIP);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_NOSE);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_EYELID);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_EYEBROW);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_CHEEK);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_CHIN);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_MOUSTACHE);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_IRIS);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_EYEBALL);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_TEETH);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_GUM);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_HAIR);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_EYELASH);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_PUPIL);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_EAR);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_TONGUE);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_COL_GLASSES);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU1);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU2);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU3);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU4);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU5);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU6);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU7);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU8);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU9);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU10);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU11);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU12);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU13);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU14);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU15);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU16);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU17);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU18);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU19);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU20);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU21);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU22);
    SO_NODE_DEFINE_ENUM_VALUE(AnimationType,ANIM_AU23);
    SO_NODE_SET_MF_ENUM_TYPE(animation,AnimationType);

    finishCB=NULL;
    finishCBData=NULL;

    m_behavior=new CBehavior();
    m_visible=false;

    m_characterSensor=new SoFieldSensor(characterCB,this);
    m_triggerSensor=new SoFieldSensor(triggerCB,this);

    m_characterSensor->attach(&character);
    m_triggerSensor->attach(&trigger);

    Init();
}

SoFacePuppet::~SoFacePuppet()
{
    m_characterSensor->detach();
    m_triggerSensor->detach();

    delete m_characterSensor;
    delete m_triggerSensor;
    delete m_behavior;
}

void SoFacePuppet::initClass()
{
    SO_NODE_INIT_CLASS(SoFacePuppet, SoNode, "Node");
}

void
SoFacePuppet::Init()
{
    m_behavior->Init();
    m_behavior->SetAgentID(0); // !!!
    m_behavior->Show(); // !!!
    m_behavior->Speak("");
    m_behavior->setFinishCallback(&finishInternalCB,this);
}

void
SoFacePuppet::GLRender(SoGLRenderAction *action)
{
    SoState *state = action->getState();

    if (SoComplexityTypeElement::get(state)!=SoComplexityTypeElement::BOUNDING_BOX){
        Timer();
        this->touch();
        Draw(action);
#ifdef __COIN__
        glEnable(GL_COLOR_MATERIAL);
#endif
    }
    else {
        float min[3],max[3];
        m_behavior->GetMinMax(min,max);
        for (int i=0;i<3;i++){
            min[i]*=DEFAULT_ZOOM;
            max[i]*=DEFAULT_ZOOM;
        }
        SoCube *cube=new SoCube;
        cube->width=max[0]-min[0];
        cube->height=max[1]-min[1];
        cube->depth=max[2]-min[2];
        cube->GLRender(action);
    }
}

void SoFacePuppet::getBoundingBox(SoGetBoundingBoxAction *action){

    SbBox3f box;
    SbVec3f center;
    this->computeBBox(action,box,center);
    if (!box.isEmpty()) {
        action->setCenter(center,TRUE);
        action->extendBy(box);
    }
}

void SoFacePuppet::computeBBox(SoAction *action,SbBox3f &box, SbVec3f &center){

    float min[3],max[3];
    m_behavior->GetMinMax(min,max);
    for (int i=0;i<3;i++){
        min[i]*=DEFAULT_ZOOM;
        max[i]*=DEFAULT_ZOOM;
    }
    box.setBounds(min,max);
    center.setValue(0.0,0.0,0.0);
}

void SoFacePuppet::rayPick(SoRayPickAction *action){

    SbBox3f box;
    SbVec3f center;

    this->computeBBox(action, box, center);
    if (box.isEmpty()) return;
    action->setObjectSpace();
    
    SbVec3f isect;
#ifdef __COIN__
    if (action->intersect(box, isect, FALSE)) {
        if (action->isBetweenPlanes(isect)) {
        action->addIntersection(isect);
        }
    }
#endif
}

void SoFacePuppet::setFinishCallback(SoFinishCB *callback, void* userData){

    m_behavior->setFinishCallback(callback,userData);
}

void SoFacePuppet::finishInternalCB(void* userData, int animationID)
{
    // if speech has finished, clear the speech text
    SoFacePuppet *puppet=(SoFacePuppet*)userData;
    if (animationID==ANIM_SPEAK) puppet->speechText.setValue("");
#ifdef DEBUG_ARPUPPET    
    printf("FACEPUPPET: finished animation %d\n",animationID);
#endif
    puppet->finishedAnimation.setValue();
    puppet->finishedAnimationData.setValue(animationID);
}

void SoFacePuppet::triggerCB(void *data,SoSensor *sensor){

    SoFacePuppet *puppet=(SoFacePuppet*)data;

    char parameter[MAX_TEXT];
    int intensity=0;
    int duration=0;
    int type=ANIM_NULL;

    int j=puppet->animation.getNum();
    for (int i=0;i<puppet->animation.getNum();i++){
        strcpy(parameter,puppet->parameter[i].getString());
        intensity=puppet->intensity[i];
        duration=puppet->duration[i];

        type=puppet->animation[i];
        switch (puppet->animation[i]){
            case ANIM_NULL:
                break;
            case ANIM_EXP:
                puppet->GetBehavior()->SetExpression(parameter,intensity,duration);
                break;
            case ANIM_VIS:
                puppet->GetBehavior()->SetViseme(atoi(parameter),intensity,duration);
                break;
            case ANIM_PLAY:
                puppet->GetBehavior()->Play(parameter,intensity,duration);
                break;
            case ANIM_SPEAK:
                puppet->GetBehavior()->Speak(parameter);
                break;
            case ANIM_SND:
                puppet->GetBehavior()->Sound(parameter);
                break;
            case ANIM_SHUT:
                puppet->GetBehavior()->ShutUp();
                break;
            case ANIM_THINK:
                puppet->GetBehavior()->Think(parameter);
                break;
            default:
                if (type<ANIM_EYE && type>=ANIM_HEAD) {
                    puppet->GetBehavior()->SetHead(type-ANIM_HEAD,intensity,duration);
                }
                else if (type<ANIM_COL && type>=ANIM_EYE){
                    puppet->GetBehavior()->SetEye(type-ANIM_EYE,intensity,duration);
                }
                else if (type<ANIM_AU && type>=ANIM_COL) {
                    int r,g,b;
                    if (sscanf(parameter,"%d %d %d",&r,&g,&b)<3) puppet->GetBehavior()->SetColor(type-ANIM_COL,parameter,duration);
                    else puppet->GetBehavior()->SetColorRGB(type-ANIM_COL,r,g,b,duration);
                }
                else if (type<ANIM_VIS && type>=ANIM_AU) {
                    puppet->GetBehavior()->SetActionUnit(type-ANIM_AU,intensity,duration);
                }
                break;
        } // switch
    } // for
}

void SoFacePuppet::characterCB(void *data,SoSensor *sensor){

    SoFacePuppet *puppet=(SoFacePuppet*)data;
    puppet->GetBehavior()->SetConformation((char*)puppet->character.getValue().getString());
}

void 
SoFacePuppet::Draw(SoGLRenderAction *action)
{
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    m_behavior->Draw(DEFAULT_ZOOM,true,true);
    //TestDraw();
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

void 
SoFacePuppet::TestDraw(){

    // *** normals
    GLfloat n[6][3] = {
        {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} 
    };
    
    // *** faces
    GLint faces[6][4] = {
        {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
        {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} 
    };

    // *** vertices
    GLfloat v[8][3];
    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

    glRotatef(30,1.0,0.0,0.0);
    glColor3f(1.0,0.0,0.0);

    // *** draw cube
    for (int i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}

void SoFacePuppet::Timer(){

    m_behavior->Animate();
}

bool SoFacePuppet::isShowing()
{
    return m_visible;
}

// -----------------------------------------------------------------------------------------

int SoFacePuppet::Show(){

    m_behavior->Show();
    m_visible=true;
    return ANIM_NULL;
}

int SoFacePuppet::Hide(){

    m_behavior->Hide();
    m_visible=false;
    return ANIM_NULL;
}

int SoFacePuppet::Play(const char *name, int intensity, int timeScale){

    m_behavior->Play(name,intensity,timeScale);
    return ANIM_PLAY;
}

int SoFacePuppet::SetConformation(const char *name){

    m_behavior->SetConformation(name);
    return ANIM_NULL;
}

int SoFacePuppet::SetActionUnit(int number,int intensity,int transitionTime){

    m_behavior->SetActionUnit(number,intensity,transitionTime);
    return ANIM_AU1+number;
}

int SoFacePuppet::SetExpression(const char *name,int intensity,int transitionTime){

    m_behavior->SetExpression(name,intensity,transitionTime);
    return ANIM_EXP;
}

int SoFacePuppet::SetViseme(int num,int intensity,int transitionTime){

    m_behavior->SetViseme(num,intensity,transitionTime);
    return ANIM_VIS;
}

int SoFacePuppet::SetColor(int num,const char *name,int transitionTime){

    m_behavior->SetColor(num,name,transitionTime);
    return ANIM_COL+num;
}

int SoFacePuppet::SetColorRGB(int num,int r,int g,int b,int transitionTime){

    m_behavior->SetColorRGB(num,r,g,b,transitionTime);
    return ANIM_COL+num;
}

int SoFacePuppet::SetEye(int num,int value,int transitionTime){

    m_behavior->SetEye(num,value,transitionTime);
    return ANIM_EYE+num;
}

int SoFacePuppet::SetHead(int num,int value,int transitionTime){

    m_behavior->SetEye(num,value,transitionTime);
    return ANIM_HEAD+num;
}

int SoFacePuppet::BlendExpressions(const char *exp1,const char *exp2,int factor,int intensity,int transitionTime){

    m_behavior->BlendExpressions(exp1,exp2,factor,intensity,transitionTime);
    return ANIM_EXP;
}

int SoFacePuppet::Speak(const char *text){

    int result=m_behavior->Speak(text); // if speech is properly initialized, return ANIM_SPEAK otherwise ANIM_THINK
    speechText.setValue(m_behavior->GetSpeechText());
    return result;
}

int SoFacePuppet::Sound(const char *fileName){

    m_behavior->Sound(fileName);
    return ANIM_SND;
}

int SoFacePuppet::Think(const char *text){

    m_behavior->Think(text);
    return ANIM_THINK;
}

int SoFacePuppet::ShutUp(){

    m_behavior->ShutUp();
    return ANIM_SPEAK;
}

int SoFacePuppet::TurnToUser(int transitionTime){

    m_behavior->TurnToUser(transitionTime);
    return ANIM_EYE;
}

int SoFacePuppet::TurnFromUser(int transitionTime){

    m_behavior->TurnFromUser(transitionTime);
    return ANIM_EYE;
}

void SoFacePuppet:: SetVariable(const char *name,const char *value){

    m_behavior->SetVariable(name,value);
}

char* SoFacePuppet::GetVariable(const char *name){

    return m_behavior->GetVariable(name);
}

CBehavior* SoFacePuppet::GetBehavior(){

    return m_behavior;
}
