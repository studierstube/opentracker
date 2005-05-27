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
/* Cal3DAnimator.cxx contains the implementation of Cal3DAnimator
 *
 * @author Istvan Barakonyi
 *
 * $Id: Cal3DAnimator.cxx 3653 2004-09-03 15:44:26Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

// Inventor classes
#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoTransform.h"
#include "Inventor/nodes/SoCoordinate3.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// own classes
#include "Cal3DAnimator.h"

static const int cube_plane_index[] =
{
    0, 1, 3,
    5, 4, 6,
    1, 5, 7,
    4, 0, 2,
    4, 5, 1,
    2, 3, 7
};
// Cal3D puppet manipulation axes (X,Y and Z changed!)
static const SbVec3f boneRotationAxis[3]={
    SbVec3f(0,0,1),SbVec3f(1,0,0),SbVec3f(0,0,1)
};
static const int boneAxisIndex[3]={2,0,1};
static const int axisCode[3]={Cal3DAnimator::Z,Cal3DAnimator::X,Cal3DAnimator::Y};

SO_KIT_SOURCE(Cal3DAnimator);

void Cal3DAnimator::initClass()
{
   SO_KIT_INIT_CLASS(Cal3DAnimator, SoBaseKit, "BaseKit"); 
}

Cal3DAnimator::Cal3DAnimator()
{
    SO_KIT_CONSTRUCTOR(Cal3DAnimator);

    SO_KIT_ADD_CATALOG_ENTRY(root, SoSeparator, FALSE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(puppeteer, SoCal3DPuppeteer, FALSE, root, \0, TRUE);

    // display/interaction options
    SO_KIT_ADD_FIELD(enablePosing, (FALSE));
    SO_KIT_ADD_FIELD(cursorMode, (BONE)); // BONE is not an SFEnum but an integer!
    SO_KIT_ADD_FIELD(autoDisplayBB, (TRUE));
    SO_KIT_ADD_FIELD(showBoneBB, (TRUE));
    SO_KIT_ADD_FIELD(showBodyBB, (TRUE));

    // bone/body manipulation
    SO_KIT_ADD_FIELD(cursorPosition, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(cursorButton, (FALSE));
    SO_KIT_ADD_FIELD(insideBBTolerance, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(pickedBone, (-1));
    SO_KIT_ADD_FIELD(axisConstraint, (XYZ)); // XYZ is not an SFEnum but an integer!
    SO_KIT_ADD_FIELD(keepAspectRatio, (FALSE));
    SO_KIT_ADD_FIELD(rotationSensitivity, (1.0f));
    SO_KIT_ADD_FIELD(scaleSensitivity, (1.0f));

    // keyframing
    SO_KIT_ADD_FIELD(time, (0));

    SO_KIT_INIT_INSTANCE();

    cursorPositionSensor=new SoFieldSensor(cursorPositionCB,this);
    cursorButtonSensor=new SoFieldSensor(cursorButtonCB,this);
    pickedBoneSensor=new SoFieldSensor(pickedBoneCB,this);
    timeSensor=new SoFieldSensor(timeCB,this);

    puppet=NULL;
    oldCursorPosition=SbVec3f(0,0,0);
    oldBodyPosition=SbVec3f(0,0,0);
    oldBodyOrientation=SbRotation::identity();
    isDragging=FALSE;

    setUpConnections(TRUE,TRUE);
}

Cal3DAnimator::~Cal3DAnimator()
{
    delete cursorPositionSensor;
    delete cursorButtonSensor;
    delete pickedBoneSensor;
    delete timeSensor;
}

SbBool Cal3DAnimator::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        SoCal3DPuppeteer *c3dp=SO_GET_ANY_PART(this,"puppeteer",SoCal3DPuppeteer);
        for (int i=0;i<c3dp->getPuppetNum();i++){
            // find first Cal3D puppet
            if (c3dp->isType(c3dp->getPuppet(i),"SoCal3DPuppet")) {
                puppet=(SoCal3DPuppet*)(c3dp->getFoundNode());
                skeleton=puppet->getCalModel()->getSkeleton();
                puppet->directManipulation.connectFrom(&enablePosing);
                puppet->addBoneManipulationFunction(setPickedBoneRotation,this);
                puppet->getCalModel()->getMixer()->updateSkeleton();
                break;
            }
        }
        if (puppet!=NULL) printf("CAL3DANIMATOR: Cal3DPuppet loaded from '%s' has been found\n",puppet->fileName.getValue().getString());

        c3dp->disableIdleBehavior.connectFrom(&enablePosing);

        cursorPositionSensor->attach(&cursorPosition);
        cursorButtonSensor->attach(&cursorButton);
        pickedBoneSensor->attach(&pickedBone);
        timeSensor->attach(&time);

        cursorPositionCB(this,NULL);
        cursorButtonCB(this,NULL);
        pickedBoneCB(this,NULL);
        timeCB(this,NULL);
    }
    else {
        if (puppet!=NULL) puppet->removeBoneManipulationFunction(setPickedBoneRotation,this);

        cursorPositionSensor->detach();
        cursorButtonSensor->detach();
        pickedBoneSensor->detach();
        timeSensor->detach();
    }

    return !(connectionsSetUp = onOff);
}

void Cal3DAnimator::cursorPositionCB(void *data,SoSensor *sensor)
{
    Cal3DAnimator *ca=(Cal3DAnimator*)data;
    if (ca->puppet==NULL) return;

    // if we are currently dragging a bone, don't want to pick a new bone
    if (ca->isDragging) return;

    SoCal3DPuppeteer *cp=SO_GET_ANY_PART(ca,"puppeteer",SoCal3DPuppeteer);
    SoCoordinate3 *bbCoord=(SoCoordinate3*)(ca->puppet->getPart("bbCoord",FALSE));
    if (bbCoord==NULL) printf("CAL3DANIMATOR: error getting the bone bounding box parameter from the Cal3D puppet\n");

    bool inAABB=ca->isPointInAABB(
        ca->cursorPosition.getValue(),
        ca->cursorMode.getValue()==BONE ? cp->boundingBox[4]-ca->insideBBTolerance.getValue() : 
            cp->boundingBox[4], // boundingBox[4] = MIN
        ca->cursorMode.getValue()==BONE ? cp->boundingBox[3]+ca->insideBBTolerance.getValue() : 
            cp->boundingBox[3]  // boundingBox[3] = MAX
    );
    cp->boundingBoxOn=
        ca->cursorMode.getValue()!=BONE ||
        (ca->cursorMode.getValue()==BONE && ((ca->autoDisplayBB.getValue() && !inAABB) || (!ca->autoDisplayBB.getValue() && ca->showBodyBB.getValue())));
    ca->puppet->boneBoundingBoxOn=
        ca->cursorMode.getValue()==BONE && 
        ((ca->autoDisplayBB.getValue() && inAABB) || (!ca->autoDisplayBB.getValue() && ca->showBoneBB.getValue()));

    // if cursor is not inside the character's BB or we are not in bone manipulation mode, there's nothing left to do here
    if (!inAABB || ca->cursorMode.getValue()!=BONE) return;

    // check whether user picked a bone with the cursor
    int pickedNum=-1; // init to invalid bone ID
    bool testFailed;
    SbVec3f p[3];
    float bbDiagonalLength=0;
    for (int i=0;i<ca->puppet->boneNum.getValue();i++){
        testFailed=false;
        for (int j=0;j<6;j++){
            for (int k=0;k<3;k++){ 
                p[k]=bbCoord->point[i*8+cube_plane_index[j*3+k]]*cp->scale.getValue()[k];
                cp->orientation.getValue().multVec(p[k],p[k]);
                p[k]+=cp->position.getValue();
            }
            if (p[0]==p[1] || p[1]==p[2]) {
                testFailed=true;
                break;
            }
            SbPlane plane(p[0],p[1],p[2]);
            if (plane.isInHalfSpace(ca->cursorPosition.getValue())) {
                testFailed=true;
                break;
            }
        }
        if (!testFailed) {
            // test whether the currently tested bounding box is smaller (i.e. its diagonal is shorter) than a previously 
            // found matching bounding box. Thus we are still able to pick small BB's contained by a larger BB
            float tempDiagonalLength=(bbCoord->point[i*8]-bbCoord->point[i*8+7]).length();
            if (!bbDiagonalLength || tempDiagonalLength<bbDiagonalLength) {
                bbDiagonalLength=tempDiagonalLength;
                pickedNum=i;
            }
        }
    }

    ca->pickedBone=pickedNum;
}

void Cal3DAnimator::cursorButtonCB(void *data,SoSensor *sensor)
{
    Cal3DAnimator *ca=(Cal3DAnimator*)data;
    if (ca->puppet==NULL) return;
    SoCal3DPuppeteer *c3dp=SO_GET_ANY_PART(ca,"puppeteer",SoCal3DPuppeteer);

    if (ca->cursorMode.getValue()==BONE){
        if (ca->cursorButton.getValue() && ca->pickedBone.getValue()>=0 && ca->pickedBone.getValue()<ca->puppet->boneNum.getValue()){
            // capture initial cursor position
            if (!ca->isDragging){
                ca->oldCursorPosition=ca->cursorPosition.getValue();
                ca->oldBodyOrientation=c3dp->orientation.getValue();
                ca->oldBoneRotation=ca->skeleton->getBone(ca->pickedBone.getValue())->getRotation();
                ca->boneRotation=ca->oldBoneRotation;
                ca->isDragging=TRUE;
                c3dp->controlMode.set1Value(0,c3dp->controlMode[0]&~SoPuppeteerKit::DRAG);
                c3dp->controlMode.set1Value(1,c3dp->controlMode[1]&~SoPuppeteerKit::DRAG);
            }
            // rotate picked bone while dragging
            else {
                int i;
                float qx,qy,qz,qw;
                SbVec3f cursorOffset=ca->cursorPosition.getValue()-ca->oldCursorPosition;
                ca->oldBodyOrientation.inverse().multVec(cursorOffset,cursorOffset);
                cursorOffset[0]=-cursorOffset[0];
                float boneRotationAngle[3];
                SbRotation boneRotationOffset;
                for (i=0;i<3;i++) {
                    if (ca->axisConstraint.getValue() & axisCode[i]){
                        boneRotationAngle[i]=cursorOffset[boneAxisIndex[i]];
                        boneRotationOffset=
                            SbRotation(boneRotationAxis[i],sin(boneRotationAngle[i]*ca->rotationSensitivity.getValue())*2*M_PI)*
                            boneRotationOffset;
                    }
                }
                boneRotationOffset.getValue(qx,qy,qz,qw);
                CalQuaternion offset=CalQuaternion(qx,qy,qz,qw);
                ca->boneRotation=offset*ca->oldBoneRotation;
            }
        }
        else ca->isDragging=FALSE;
    }
    else if (ca->cursorMode.getValue()==BODY){
        if (ca->cursorButton.getValue()){
            if (!ca->isDragging){
                c3dp->controlMode.set1Value(0,c3dp->controlMode[0]|SoPuppeteerKit::DRAG);
                c3dp->controlMode.set1Value(1,c3dp->controlMode[1]|SoPuppeteerKit::DRAG);
                ca->pickedBone=-1;
                ca->isDragging=TRUE;
            }
        }
        else if (ca->isDragging) ca->isDragging=FALSE;
    }
    else { // SCALE
        if (ca->cursorButton.getValue()){
            if (!ca->isDragging){
                ca->oldCursorPosition=ca->cursorPosition.getValue();
                ca->oldBodyPosition=c3dp->position.getValue();
                ca->oldBodyOrientation=c3dp->orientation.getValue();
                ca->oldScale=c3dp->scale.getValue();
                ca->pickedBone=-1;
                ca->isDragging=TRUE;
                c3dp->controlMode.set1Value(0,c3dp->controlMode[0]&~SoPuppeteerKit::DRAG);
                c3dp->controlMode.set1Value(1,c3dp->controlMode[1]&~SoPuppeteerKit::DRAG);
            }
            else {
                SbVec3f oldScaleOffset=ca->oldCursorPosition-ca->oldBodyPosition;
                SbVec3f scaleOffset=ca->cursorPosition.getValue()-ca->oldBodyPosition;
                ca->oldBodyOrientation.inverse().multVec(oldScaleOffset,oldScaleOffset);
                ca->oldBodyOrientation.inverse().multVec(scaleOffset,scaleOffset);
                SbVec3f scale;
                if (ca->keepAspectRatio.getValue()){
                    float oldOffsetLength=0,offsetLength=0;
                    for (int i=0;i<3;i++){
                        if (ca->axisConstraint.getValue() & axisCode[i]){
                            offsetLength+=scaleOffset[i]*scaleOffset[i];
                            oldOffsetLength+=oldScaleOffset[i]*oldScaleOffset[i];
                        }
                    }
                    offsetLength=sqrt(offsetLength);
                    oldOffsetLength=sqrt(oldOffsetLength);
                    float factor=oldOffsetLength ? offsetLength/oldOffsetLength : 1.0f;
                    scale=ca->oldScale*factor*ca->scaleSensitivity.getValue();
                }
                else {
                    for (int i=0;i<3;i++){
                        scale[i]=
                            oldScaleOffset[i] && 
                            (ca->axisConstraint.getValue() & axisCode[i]) ? 
                                scaleOffset[i]/oldScaleOffset[i]*ca->oldScale[i]*ca->scaleSensitivity.getValue() : 1;
                        if (scale[i]<0) scale[i]=-scale[i];
                    }
                }
                c3dp->scaleSet.setValue(scale);
            }
        }
        else {
            if (ca->isDragging) ca->isDragging=FALSE;
        }
        
    }
}

void Cal3DAnimator::pickedBoneCB(void *data,SoSensor *sensor)
{
    Cal3DAnimator *ca=(Cal3DAnimator*)data;
    if (ca->puppet==NULL) return;

    for (int i=0;i<ca->puppet->boneNum.getValue();i++){
        ca->puppet->bbColorIndex.set1Value(i,(ca->pickedBone.getValue()==i) ? 1 : 0);
    }
}

void Cal3DAnimator::timeCB(void *data,SoSensor *sensor)
{
    Cal3DAnimator *ca=(Cal3DAnimator*)data;
}

bool Cal3DAnimator::isPointInAABB(SbVec3f point,SbVec3f bbMin,SbVec3f bbMax)
{
    return (
         bbMin[0]<=point[0] && bbMax[0]>=point[0] && 
         bbMin[1]<=point[1] && bbMax[1]>=point[1] && 
         bbMin[2]<=point[2] && bbMax[2]>=point[2]
    );
}

void Cal3DAnimator::setPickedBoneRotation(void *data,SoCal3DPuppet *cal3dPuppet)
{
    Cal3DAnimator *ca=(Cal3DAnimator*)data;

    if (ca->isDragging){
        int boneID=ca->pickedBone.getValue();
        if (boneID<0 || boneID>=ca->puppet->boneNum.getValue()) return;

        ca->skeleton->getBone(boneID)->clearState();
        ca->skeleton->getBone(boneID)->setRotation(ca->boneRotation);
        ca->skeleton->getBone(boneID)->lockState();
        ca->skeleton->getBone(boneID)->calculateState();
    }
}
