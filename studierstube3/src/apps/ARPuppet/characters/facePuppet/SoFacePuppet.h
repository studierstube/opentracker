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
/* SoFacePuppet.h contains the definition of class SoFacePuppet,
 * an affective talking head puppet
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoFacePuppet.h 3729 2004-09-19 21:49:18Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _SOFACEPUPPET_H_
#define _SOFACEPUPPET_H_

#include "facepuppet.h"

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFEnum.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/sensors/SoFieldSensor.h>

//#include <GL/gl.h>
//#include <GL/glu.h>
#include "Behavior.h"
#include "utils.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class FACEPUPPET_API SoFacePuppet : public SoNode
{
    SO_NODE_HEADER(SoFacePuppet);

 public:

    SoFacePuppet();
    ~SoFacePuppet();

    SoMFEnum animation;
    SoMFString parameter;
    SoMFInt32 intensity;
    SoMFInt32 duration;
    SoSFTrigger trigger;
    SoSFString character;
    SoSFTrigger finishedAnimation;
    SoSFEnum finishedAnimationData;
    SoSFString speechText;

    SoFinishCB *finishCB;
    void *finishCBData;
    void setFinishCallback(SoFinishCB *cb, void* userData);

    CBehavior* GetBehavior();
    bool isShowing();

    int Show();
    int Hide();
    int Play(const char *name, int intensity=100, int timeScale=100);
    int SetConformation(const char *name);
    int SetActionUnit(int number,int intensity=100,int transitionTime=0);
    int SetExpression(const char *name,int intensity=100,int transitionTime=0);
    int SetViseme(int num,int intensity=100,int transitionTime=0);
    int SetColor(int num,const char *name,int transitionTime=0);
    int SetColorRGB(int num,int r,int g,int b,int transitionTime=0);
    int SetEye(int num,int value,int transitionTime=0);
    int SetHead(int num,int value,int transitionTime=0);
    int BlendExpressions(const char *exp1,const char *exp2,int factor,int intensity=100,int transitionTime=0);
    int Speak(const char *text);
    int Sound(const char *fileName);
    int Think(const char *text);
    int ShutUp();
    int TurnToUser(int transitionTime=0);
    int TurnFromUser(int transitionTime=0);
    void  SetVariable(const char *name,const char *value);
    char* GetVariable(const char *name);

SoINTERNAL public:
    
    static void initClass();

 protected:

    virtual void GLRender(SoGLRenderAction *action);
    virtual void computeBBox(SoAction *action,SbBox3f &box, SbVec3f &center);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void rayPick(SoRayPickAction *action);

    void TestDraw();

 private:

    CBehavior *m_behavior;
    SoFieldSensor *m_characterSensor;
    SoFieldSensor *m_triggerSensor;
    bool m_visible;

    void Init();

    virtual void Draw(SoGLRenderAction *action);
    virtual void Timer();

    static void characterCB(void *data,SoSensor *sensor);
    static void triggerCB(void *data,SoSensor *sensor);
    static void finishInternalCB(void* userData, int animationID);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOFACEPUPPET_H_
