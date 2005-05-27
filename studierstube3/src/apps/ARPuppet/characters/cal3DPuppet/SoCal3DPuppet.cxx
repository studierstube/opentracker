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
/* SoCal3DPuppet.cxx contains the implementation of the class SoCal3DPuppet, 
 * a puppet based on the Cal3D library
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoCal3DPuppet.cxx 4212 2005-05-19 11:09:56Z bara $
 * @file
 */
/* ======================================================================== */

/*
 * -------------------------------------------------------------------------------
 * This code is relying on the Cal3D library and uses some code snippets from the 
 * cally_3d demo examples.
 * 
 * Copyright (C) 2001 Bruno 'Beosil' Heidelberger
 * Copyright notice of Cal3D:
 * -------------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * -------------------------------------------------------------------------------*/

#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoSwitch.h"
#include "Inventor/nodes/SoTransform.h"
#include "Inventor/nodes/SoCoordinate3.h"
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/nodes/SoMaterialBinding.h"
#include "Inventor/nodes/SoLightModel.h"
#include "Inventor/nodes/SoDrawStyle.h"
#include "Inventor/nodes/SoIndexedFaceSet.h"
#include <Inventor/nodes/SoCube.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoLightModelElement.h>
/*#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/elements/SoShininessElement.h>
#include <Inventor/elements/SoSpecularColorElement.h>*/
#include <Inventor/fields/SoSFTime.h>
#include "tga.h"

#include "SoCal3DPuppet.h"

SO_KIT_SOURCE(SoCal3DPuppet);

SbPList SoCal3DPuppet::calCoreModelList=SbPList();
SbPList SoCal3DPuppet::animationIdList=SbPList();
SbPList SoCal3DPuppet::animationNameList=SbPList();

static int cube_face_index[] =
{
    0, 1, 3, 2,
    5, 4, 6, 7,
    1, 5, 7, 3,
    4, 0, 2, 6,
    4, 5, 1, 0,
    2, 3, 7, 6
}; 

void SoCal3DPuppet::initClass()
{
    SO_KIT_INIT_CLASS(SoCal3DPuppet, SoBaseKit, "BaseKit");
}

SoCal3DPuppet::SoCal3DPuppet()
{
    SO_KIT_CONSTRUCTOR(SoCal3DPuppet);

    SO_KIT_ADD_CATALOG_ENTRY(root, SoSeparator, FALSE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbSwitch, SoSwitch, FALSE, root, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbTrans, SoTransform, FALSE, bbSwitch, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbCoord, SoCoordinate3, FALSE, bbSwitch, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbMaterial, SoMaterial, FALSE, bbSwitch, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbMaterialBinding, SoMaterialBinding, FALSE, bbSwitch, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbLightModel, SoLightModel, FALSE, bbSwitch, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbDrawStyle, SoDrawStyle, FALSE, bbSwitch, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbIndexedFaceSet, SoIndexedFaceSet, FALSE, bbSwitch, \0, FALSE);

    SO_KIT_ADD_FIELD(pathName, (""));
    SO_KIT_ADD_FIELD(fileName, (""));
    SO_KIT_ADD_FIELD(visible, (TRUE));
    SO_KIT_ADD_FIELD(animate, (TRUE));
    SO_KIT_ADD_FIELD(lod, (100));
    SO_KIT_ADD_FIELD(skeleton, (FALSE));
    SO_KIT_ADD_FIELD(mesh, (TRUE));
    SO_KIT_ADD_FIELD(boneColor, (1.0,1.0,1.0));
    SO_KIT_ADD_FIELD(jointColor, (1.0,0.0,0.0));
    SO_KIT_ADD_FIELD(boneBoundingBoxOn, (FALSE));
    SO_KIT_ADD_FIELD(boneNum, (0));
    SO_KIT_ADD_FIELD(boneName, (""));
    SO_KIT_ADD_FIELD(bbColorIndex, (0));
    SO_KIT_ADD_FIELD(wireframe, (FALSE));
    SO_KIT_ADD_FIELD(execute, (""));
    SO_KIT_ADD_FIELD(state, (""));
    SO_KIT_ADD_FIELD(blend, (0.0));
    SO_KIT_ADD_FIELD(blendName, (""));
    SO_KIT_ADD_FIELD(clear, ());
    SO_KIT_ADD_FIELD(delayIn, (0.3f));
    SO_KIT_ADD_FIELD(delayOut, (0.3f));
    SO_KIT_ADD_FIELD(directManipulation, (FALSE));

    SO_KIT_INIT_INSTANCE();

    meshCount = 0;
    renderScale = 1.0f;
    name="";

    // default character boundaries
    for (int i=0;i<3;i++){
        minBounding[i]=0;
        maxBounding[i]=0;
    }

    ((SoMaterialBinding*)(SO_GET_ANY_PART(this,"bbMaterialBinding",SoMaterialBinding)))->value=SoMaterialBinding::PER_FACE_INDEXED;
    ((SoLightModel*)(SO_GET_ANY_PART(this,"bbLightModel",SoLightModel)))->model=SoLightModel::BASE_COLOR;
    SoTransform *bt=SO_GET_ANY_PART(this,"bbTrans",SoTransform);
    bt->rotation=SbRotation(SbVec3f(1,0,0),1.57f);
    bt->scaleFactor=SbVec3f(1000.0f,1000.0f,1000.0f);

    timerSensor=new SoTimerSensor(timerSensorCallback,this); // 1/30sec, start immediately
    fileNameSensor=new SoFieldSensor(fileNameCB,this);
    lodSensor=new SoFieldSensor(lodCB,this);
    animateSensor=new SoFieldSensor(animateCB,this);
    boneBoundingBoxOnSensor=new SoFieldSensor(boneBoundingBoxOnCB,this);
    bbColorIndexSensor=new SoFieldSensor(bbColorIndexCB,this);
    executeSensor=new SoFieldSensor(executeCB,this);
    stateSensor=new SoFieldSensor(stateCB,this);
    blendSensor=new SoFieldSensor(blendCB,this);
    clearSensor=new SoFieldSensor(clearCB,this);

    blendingFactor=NULL;
    calCoreModel=NULL;
    calModel=NULL;

    initialized=false;

    setUpConnections(TRUE,TRUE);
}

SoCal3DPuppet::~SoCal3DPuppet()
{
    if (timerSensor->isScheduled()) timerSensor->unschedule();

    if (calModel!=NULL) {
        delete calModel;
        calModel=NULL;
    }
    for (int i=0;i<calCoreModelList.getLength();i++){
        delete ((CalCoreModel*)(calCoreModelList[i]));
    }
    
    delete timerSensor;
    delete fileNameSensor;
    delete lodSensor;
    delete animateSensor;
    delete boneBoundingBoxOnSensor;
    delete bbColorIndexSensor;
    delete executeSensor;
    delete stateSensor;
    delete blendSensor;
    delete clearSensor;
}

SbBool SoCal3DPuppet::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        fileNameSensor->attach(&fileName);
        lodSensor->attach(&lod);
        animateSensor->attach(&animate);
        boneBoundingBoxOnSensor->attach(&boneBoundingBoxOn);
        bbColorIndexSensor->attach(&bbColorIndex);
        executeSensor->attach(&execute);
        stateSensor->attach(&state);
        blendSensor->attach(&blend);
        clearSensor->attach(&clear);

        fileNameCB(this,NULL);
        lodCB(this,NULL);
        animateCB(this,NULL);
        boneBoundingBoxOnCB(this,NULL);
        executeCB(this,NULL);
        stateCB(this,NULL);
        blendCB(this,NULL);
    }
    else {        
        fileNameSensor->detach();
        lodSensor->detach();
        animateSensor->detach();
        boneBoundingBoxOnSensor->detach();
        bbColorIndexSensor->detach();
        executeSensor->detach();
        stateSensor->detach();
        blendSensor->detach();
        clearSensor->detach();
    }

    return !(connectionsSetUp = onOff);
}

void SoCal3DPuppet::GLRender(SoGLRenderAction *action)
{
    SoState* state = action->getState();
    // disable caching
    SoCacheElement::invalidate(state);
    state->push(); // since we will change the state
    
    if (!visible.getValue()) return;

    if (!initialized || SoComplexityTypeElement::get(state)==SoComplexityTypeElement::BOUNDING_BOX){
        SoCube *cube=new SoCube;
        cube->width=maxBounding[0]-minBounding[0];
        cube->height=maxBounding[1]-minBounding[1];
        cube->depth=maxBounding[2]-minBounding[2];
        cube->GLRender(action);
    }
    else {
        glPushAttrib(GL_LIGHTING_BIT);
        glPushAttrib(GL_DEPTH_BUFFER_BIT);
#ifdef __COIN__
        glPushAttrib(GL_COLOR_MATERIAL);
#endif
        glRotatef(-90.0f,1.0f,0.0f,0.0f);
        glScalef(0.001f,0.001f,0.001f);

        if (skeleton.getValue()) renderSkeleton();
        if (mesh.getValue()) renderMesh(state);

        calculateBB();

#ifdef __COIN__
        glPopAttrib();
#endif
        glPopAttrib();
        glPopAttrib();
    }

    state->pop(); // since we pushed

    SoBaseKit::GLRender(action);
}

void SoCal3DPuppet::getBoundingBox(SoGetBoundingBoxAction *action)
{
    SbBox3f box;
    SbVec3f center;
    this->computeBBox(action,box,center);
    if (!box.isEmpty()) {
        action->setCenter(center,TRUE);
        action->extendBy(box);
    }
}

void SoCal3DPuppet::computeBBox(SoAction *action,SbBox3f &box, SbVec3f &center)
{
    // Y and Z coordinates are exchanged because of the -90` rotation in GLRender 
    // ==> minBounding and maxBounding 1 and 2 indices are swapped!!!
    float x1,y1,z1,x2,y2,z2;
    x1=minBounding[0];
    y1=minBounding[1];
    z1=minBounding[2];
    x2=maxBounding[0];
    y2=maxBounding[1];
    z2=maxBounding[2];

    box.setBounds(x1,y1,z1,x2,y2,z2);
    for (int i=0;i<3;i++) center[i]=(maxBounding[i]-minBounding[i])/2.0f;
}

void SoCal3DPuppet::calculateBB()
{
    // *************** TODO: if non-existing animation command is issued, the bounding box has weird values (x,y,z ~ 0)

    if (calModel==NULL) return;

    SoCoordinate3 *bbc=SO_GET_ANY_PART(this,"bbCoord",SoCoordinate3);
    SoIndexedFaceSet *bbifs=SO_GET_ANY_PART(this,"bbIndexedFaceSet",SoIndexedFaceSet);

    CalSkeleton *pCalSkeleton=calModel->getSkeleton();
    pCalSkeleton->calculateBoundingBoxes();
    std::vector<CalBone*> &vectorCoreBone=pCalSkeleton->getVectorBone();
    int i,j,k;
    
    // reset bounding box boundaries
    for (i=0;i<3;i++){
        minBounding[i]=0;
        maxBounding[i]=0;
    }

    for(size_t boneId=0;boneId<vectorCoreBone.size();++boneId){
        CalBoundingBox &calBoundingBox=vectorCoreBone[boneId]->getBoundingBox();
        CalVector p[8];
        calBoundingBox.computePoints(p);
        bool skip=(p[0]==p[1]); // safety check for hidden bones (workaround for a bug??)
        for (i=0;i<8;i++){
            // Inventor compatibility: scale bone BB coordinates and swap Y and Z values for -90` rotation around X axis
            SbVec3f bbPoint=SbVec3f(p[i].x/1000.0f,p[i].z/1000.0f,-p[i].y/1000.0f);
            if (boneBoundingBoxOn.getValue()) bbc->point.set1Value(boneId*8+i,bbPoint);
            if (!skip){
                for (int j=0;j<3;j++){
                    if (bbPoint[j]<minBounding[j]) minBounding[j]=bbPoint[j];
                    if (bbPoint[j]>maxBounding[j]) maxBounding[j]=bbPoint[j]; 
                }
            }
        }
    }

    if (boneBoundingBoxOn.getValue() && boneNum.getValue()!=vectorCoreBone.size()){
        boneNum=vectorCoreBone.size();
        bbc->point.setNum(boneNum.getValue()*8);
        // set face indices
        for (i=0;i<boneNum.getValue();i++){
            for (j=0;j<6;j++){
                for (k=0;k<4;k++) {
                    bbifs->coordIndex.set1Value(
                        (i*6+j)*5+k,
                        cube_face_index[j*4+k]+i*8
                    );
                }
                bbifs->coordIndex.set1Value((i*6+j)*5+4,-1);
            }
        }
        bbColorIndexCB(this,NULL);
    }
}

void SoCal3DPuppet::rayPick(SoRayPickAction *action)
{
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

void SoCal3DPuppet::timerSensorCallback(void *data, SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;

    double currentTime=cal3dmodel->getCurrentTime();
    cal3dmodel->update(currentTime-cal3dmodel->previousTime);
    cal3dmodel->previousTime=currentTime;
    cal3dmodel->touch();
}

void SoCal3DPuppet::fileNameCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (!(cal3dmodel->fileName.getValue().getLength())) {
        cal3dmodel->initialized=false;
        return;
    }
    
    if (cal3dmodel->timerSensor->isScheduled()) cal3dmodel->timerSensor->unschedule();

    SbString fullName=cal3dmodel->pathName.getValue();
    char lastChar=fullName.getString()[cal3dmodel->pathName.getValue().getLength()-1];
    if (lastChar!='/') fullName+="/";
    fullName+=cal3dmodel->fileName.getValue();
    cal3dmodel->name=cal3dmodel->fileName.getValue().getSubString(0,cal3dmodel->fileName.getValue().getLength()-5).getString();

    // initialize core model
    cal3dmodel->initialized=cal3dmodel->findCoreModel(&fullName) || cal3dmodel->initCoreModel(fullName.getString());

    if (cal3dmodel->initialized){
        cal3dmodel->initialized=cal3dmodel->initModel();
        if (cal3dmodel->blendingFactor!=NULL) delete[]cal3dmodel->blendingFactor;
        cal3dmodel->blendingFactor=new float[cal3dmodel->animationId->getLength()];
        cal3dmodel->lodCB(cal3dmodel,NULL);
        cal3dmodel->animateCB(cal3dmodel,NULL);
    }
    else {
        printf("CAL3DPUPPET: failed to initialize puppet from file '%s'\n",fullName.getString());
        return;
    }

    CalCoreSkeleton *coreSkeleton=cal3dmodel->calCoreModel->getCoreSkeleton();
    if (coreSkeleton!=NULL) coreSkeleton->calculateBoundingBoxes(cal3dmodel->calCoreModel);

    cal3dmodel->stateCB(cal3dmodel,sensor);
    cal3dmodel->executeCB(cal3dmodel,sensor);
    cal3dmodel->blendCB(cal3dmodel,sensor);

    cal3dmodel->calModel->update(1);

    // set bone name field
    int size=cal3dmodel->calModel->getSkeleton()->getVectorBone().size();
    for (int i=0;i<size;i++) {        
        cal3dmodel->boneName.set1Value(i,cal3dmodel->calModel->getSkeleton()->getBone(i)->getCoreBone()->getName().c_str());
    }
    cal3dmodel->boneName.setNum(size);
}

void SoCal3DPuppet::lodCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (cal3dmodel->initialized) cal3dmodel->calModel->setLodLevel(cal3dmodel->lod.getValue()/100.0f);
}

void SoCal3DPuppet::animateCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (!cal3dmodel->initialized) return;

    if (!cal3dmodel->animate.getValue()){
        if (cal3dmodel->timerSensor->isScheduled()) cal3dmodel->timerSensor->unschedule();
    }
    else {
        cal3dmodel->previousTime=cal3dmodel->getCurrentTime();
        if (!cal3dmodel->timerSensor->isScheduled()) cal3dmodel->timerSensor->schedule();
    }
}

void SoCal3DPuppet::boneBoundingBoxOnCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (!cal3dmodel->initialized) return;

    SoSwitch *bbsw=SO_GET_ANY_PART(cal3dmodel,"bbSwitch",SoSwitch);
    bbsw->whichChild=cal3dmodel->boneBoundingBoxOn.getValue() ? -3 : -1;
}

void SoCal3DPuppet::bbColorIndexCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (!cal3dmodel->initialized) return;

    SoIndexedFaceSet *bbifs=SO_GET_ANY_PART(cal3dmodel,"bbIndexedFaceSet",SoIndexedFaceSet);
    for (int i=0;i<cal3dmodel->boneNum.getValue();i++){
        int index=i<cal3dmodel->bbColorIndex.getNum() ? cal3dmodel->bbColorIndex[i] :
            cal3dmodel->bbColorIndex.getNum() ? cal3dmodel->bbColorIndex[cal3dmodel->bbColorIndex.getNum()-1] : 0;
        for (int j=0;j<6;j++) bbifs->materialIndex.set1Value(i*6+j,index);
    }
}

void SoCal3DPuppet::executeCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (!cal3dmodel->initialized) return;

    for (int i=0;i<cal3dmodel->animationId->getLength();i++){
        if (!strcmp(((*(cal3dmodel->animationName))[i])->getString(),cal3dmodel->execute.getValue().getString())) {
            cal3dmodel->calModel->getMixer()->executeAction(
                (*(cal3dmodel->animationId))[i],cal3dmodel->delayIn.getValue(),cal3dmodel->delayOut.getValue());
            if (!cal3dmodel->animate.getValue()) cal3dmodel->update(1);
        }
        else cal3dmodel->calModel->getMixer()->clearCycle((*(cal3dmodel->animationId))[i],cal3dmodel->delayOut.getValue());
    }
}

/*** TODO: the motion blending implementation in Cal3D is currently buggy:
If the selected animation (e.g. the 'wave' animation of Cally) does not control the root bone (bone 0), 
the blendCycle function crashes in the Mixer object. The 'blendCycle' requires an animation that controls all bones 
(or maybe just the root bone).
Waiting for the next release :-)
*/
void SoCal3DPuppet::stateCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (!cal3dmodel->initialized) return;

    for (int i=0;i<cal3dmodel->animationId->getLength();i++){
        cal3dmodel->blendingFactor[i]=(!strcmp(((*(cal3dmodel->animationName))[i])->getString(),cal3dmodel->state.getValue().getString())) ? 1.0f : 0.0f;
    }
    cal3dmodel->setMotionBlending();
    if (!cal3dmodel->animate.getValue()) cal3dmodel->update(1);
}

void SoCal3DPuppet::blendCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (!cal3dmodel->initialized) return;

    // return if there are no blending parameters specified
    if (cal3dmodel->blendName.getNum()==1 && !cal3dmodel->blendName[0].getLength()) return;

    for (int i=0;i<cal3dmodel->blendName.getNum();i++){
        for (int j=0;j<cal3dmodel->animationId->getLength();j++) {
            if (!strcmp(cal3dmodel->blendName[i].getString(),((*(cal3dmodel->animationName))[j])->getString())) {
                cal3dmodel->blendingFactor[(*(cal3dmodel->animationId))[j]]=
                    ((*(cal3dmodel->animationId))[j]<cal3dmodel->animationId->getLength()) ? cal3dmodel->blend[i] : 0;
                break;
            }
        }
    }
    /*if (cal3dmodel->animationId->getLength()>cal3dmodel->blend.getNum()){
        printf("CAL3DPUPPET: warning: no. of blend parameters (%d) < no. of defined animations in character (%d)\n",
            cal3dmodel->blend.getNum(),cal3dmodel->animationId->getLength());
    }*/
    cal3dmodel->setMotionBlending();
    if (!cal3dmodel->animate.getValue()) cal3dmodel->update(1);
}

void SoCal3DPuppet::clearCB(void *data,SoSensor *sensor)
{
    SoCal3DPuppet *cal3dmodel=(SoCal3DPuppet*)data;
    if (!cal3dmodel->initialized) return;

    for (int i=0;i<cal3dmodel->animationId->getLength();cal3dmodel->blendingFactor[i++]=0.0f);
    cal3dmodel->setMotionBlending();
    if (!cal3dmodel->animate.getValue()) cal3dmodel->update(1);
}

// initialize the SoCal3DPuppet
bool SoCal3DPuppet::initCoreModel(const std::string& strFileName)
{
    calCoreModel=new CalCoreModel(strFileName);

    // initialize animation parameter lists (they are all list items for appropriate SbPList's)
    animationId=new SbIntList;
    animationName=new SbStringList;
    
    // open the SoCal3DPuppet configuration file
    std::ifstream file;
    file.open(strFileName.c_str(), std::ios::in | std::ios::binary);
    if (!file){     
        printf("CAL3DPUPPET: failed to open SoCal3DPuppet configuration file '%s'\n",strFileName.c_str());
        return false;
    }

    // initialize the data path
    std::string strPath;

    // parse all lines from the SoCal3DPuppet configuration file
    int line;
    for (line = 1; ; line++){
        // read the next SoCal3DPuppet configuration line
        std::string strBuffer;
        std::getline(file, strBuffer);

        // stop if we reached the end of file
        if (file.eof()) break;

        // check if an error happend while reading from the file
        if (!file){
            printf("CAL3DPUPPET: error while reading from the SoCal3DPuppet configuration file '%s'\n",strFileName.c_str());
            return false;
        }

        // find the first non-whitespace character
        std::string::size_type pos;
        pos = strBuffer.find_first_not_of(" \t");

        // check for empty lines
        if ((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

        // check for comment lines
        if (strBuffer[pos] == '#') continue;

        // get the key
        std::string strKey;
        strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
        pos += strKey.size();

        // get the '=' character
        pos = strBuffer.find_first_not_of(" \t", pos);
        if ((pos == std::string::npos) || (strBuffer[pos] != '=')){
            printf("CAL3DPUPPET: %s (%d): invalid syntax\n",strFileName.c_str(),line);
            return false;
        }

        // find the first non-whitespace character after the '=' character
        pos = strBuffer.find_first_not_of(" \t", pos + 1);

        // get the data
        std::string strData;
        strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

        // handle the SoCal3DPuppet creation
        if (strKey == "scale"){
            // set rendering scale factor
            renderScale = atof(strData.c_str());
        }
        else if (strKey == "path"){
            // set the new path for the data files
            strPath = strData;
        }
        else if (strKey == "skeleton"){
            // load core skeleton
            printf("CAL3DPUPPET: loading skeleton '%s'...\n",strData.c_str());
            if (!calCoreModel->loadCoreSkeleton(strPath + strData)){
                CalError::printLastError();
                return false;
            }
        }
        else if (strKey == "animation"){
            // load core animation
            printf("CAL3DPUPPET: loading animation '%s'...\n",strData.c_str());
            int animId=calCoreModel->loadCoreAnimation(strPath + strData);
            SbString *animStr=new SbString(strData.substr(0,strData.length()-4).c_str());
            if (animId == -1){
                CalError::printLastError();
                return false;
            }
            animationId->append(animId);
            animationName->append(animStr);
        }
        else if (strKey == "mesh"){
            // load core mesh
            printf("CAL3DPUPPET: loading mesh '%s'...\n",strData.c_str());
            if (calCoreModel->loadCoreMesh(strPath + strData) == -1){
                CalError::printLastError();
                return false;
            }
        }
        else if (strKey == "material"){
            // load core material
            printf("CAL3DPUPPET: loading material '%s'...\n",strData.c_str());
            if (calCoreModel->loadCoreMaterial(strPath + strData) == -1){
                CalError::printLastError();
                return false;
            }
        }
        else {
            printf("CAL3DPUPPET: %s (%d): invalid syntax\n",strFileName.c_str(),line);
            return false;
        }
    }

    // explicitely close the file
    file.close();

    // store animation information for this core model
    animationIdList.append(animationId);
    animationNameList.append(animationName);

    // load all textures and store the opengl texture id in the corresponding map in the material
    int materialId;
    for (materialId = 0; materialId < calCoreModel->getCoreMaterialCount(); materialId++){
        // get the core material
        CalCoreMaterial *pCoreMaterial;
        pCoreMaterial = calCoreModel->getCoreMaterial(materialId);

        // loop through all maps of the core material
        int mapId;
        for (mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++){
            // get the filename of the texture
            std::string strFileName;
            strFileName = pCoreMaterial->getMapFilename(mapId);

            // load the texture from the file
            GLuint textureId;
            textureId = loadTexture(strPath + strFileName);

            // store the opengl texture id in the user data of the map
            pCoreMaterial->setMapUserData(mapId, (Cal::UserData)textureId);
        }
    }

    // make one material thread for each material
    // NOTE: this is not the right way to do it, but this viewer can't do the right
    // mapping without further information on the SoCal3DPuppet etc.
    for (materialId = 0; materialId < calCoreModel->getCoreMaterialCount(); materialId++){
        // create the a material thread
        calCoreModel->createCoreMaterialThread(materialId);

        // initialize the material thread
        calCoreModel->setCoreMaterialId(materialId, 0, materialId);
    }

    SbString *fullFileName=new SbString;
    *fullFileName=strFileName.c_str();
    calCoreModel->setUserData(fullFileName);
    calCoreModelList.append(calCoreModel);

    printf("CAL3DPUPPET: core model from file '%s' created\n",fullFileName->getString());

    return true;
}

bool SoCal3DPuppet::initModel()
{
    if (calModel!=NULL) {
        delete calModel;
    }
    // create the SoCal3DPuppet instance from the loaded core
    if (!(calModel=new CalModel(calCoreModel))){
        CalError::printLastError();
        return false;
    }

    // attach all meshes to the SoCal3DPuppet
    int meshId;
    for (meshId = 0; meshId < calCoreModel->getCoreMeshCount(); meshId++){
        calModel->attachMesh(meshId);
    }

    // set the material set of the whole SoCal3DPuppet
    calModel->setMaterialSet(0);

    return true;
}

// load and create a texture from a given file
int SoCal3DPuppet::loadTexture(const std::string& strFileName)
{
    GLuint pId = 0;

    if (!stricmp(strrchr(strFileName.c_str(),'.'),".raw")){
        // open the texture file
        std::ifstream file;
        file.open(strFileName.c_str(), std::ios::in | std::ios::binary);
        if(!file){
            printf("CAL3DPUPPET: ERROR: texture file '%s' not found\n",strFileName);
            return 0;
        }

        // load the dimension of the texture
        int width;
        file.read((char *)&width, 4);
        int height;
        file.read((char *)&height, 4);
        int depth;
        file.read((char *)&depth, 4);

        // allocate a temporary buffer to load the texture to
        unsigned char *pBuffer;
        pBuffer = new unsigned char[width * height * depth];
        if(pBuffer == 0){
            printf("CAL3DPUPPET: ERROR: memory allocation for texture '%s' failed\n",strFileName);
            return 0;
        }

        // load the texture
        file.read((char *)pBuffer, width * height * depth);

        // explicitely close the file
        file.close();

        // generate texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &pId);
        glBindTexture(GL_TEXTURE_2D, pId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, (depth == 3) ? GL_RGB : GL_RGBA, width, height, 0, (depth == 3) ? GL_RGB : GL_RGBA, 
            GL_UNSIGNED_BYTE, &pBuffer[0]);

        // free the allocated memory
        delete [] pBuffer;
    }
    else if (!stricmp(strrchr(strFileName.c_str(),'.'),".tga")){
        CTga *Tga;
        Tga = new CTga();

        // note: This will always make a 32-bit texture
        if(!Tga->ReadFile(strFileName.c_str())){
            Tga->Release();
            return false;
        }

        // bind texture
        int width = Tga->GetSizeX();
        int height = Tga->GetSizeY();
        int depth = Tga->Bpp() / 8;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &pId);
  
        glBindTexture(GL_TEXTURE_2D, pId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
        glTexImage2D(GL_TEXTURE_2D, 0, ((depth == 3) ? GL_RGB : GL_RGBA), width, height, 0, 
            ((depth == 3) ? GL_RGB : GL_RGBA) , GL_UNSIGNED_BYTE, (char*)Tga->GetPointer());

        Tga->Release();
    }

    return pId;
}

// render the skeleton of the SoCal3DPuppet
void SoCal3DPuppet::renderSkeleton()
{
    // draw the bone lines
    float lines[1024][2][3];
    int nrLines;
    nrLines =  calModel->getSkeleton()->getBoneLines(&lines[0][0][0]);
    // nrLines = calModel->getSkeleton()->getBoneLinesStatic(&lines[0][0][0]);

    glDisable(GL_LIGHTING);
    glLineWidth(3.0f);
    glColor3f(boneColor.getValue()[0],boneColor.getValue()[1],boneColor.getValue()[2]);
    glBegin(GL_LINES);
        int currLine;
        for (currLine = 0; currLine < nrLines; currLine++){
            glVertex3f(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
            glVertex3f(lines[currLine][1][0], lines[currLine][1][1], lines[currLine][1][2]);
        }
    glEnd();
    glLineWidth(1.0f);

    // draw the bone points
    float points[1024][3];
    int nrPoints;
    nrPoints =  calModel->getSkeleton()->getBonePoints(&points[0][0]);
    // nrPoints = calModel->getSkeleton()->getBonePointsStatic(&points[0][0]);

    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glColor3f(jointColor.getValue()[0],jointColor.getValue()[1],jointColor.getValue()[2]);
    int currPoint;
    for (currPoint = 0; currPoint < nrPoints; currPoint++){
        glVertex3f(points[currPoint][0], points[currPoint][1], points[currPoint][2]);
    }
    glEnd();
    glPointSize(1.0f);

    glEnable(GL_LIGHTING);
}

// render the mesh of the SoCal3DPuppet
void SoCal3DPuppet::renderMesh(SoState* state)
{
    // get the renderer of the SoCal3DPuppet
    CalRenderer *pCalRenderer;
    pCalRenderer = calModel->getRenderer();

    // begin the rendering loop
    if (!pCalRenderer->beginRendering()) return;

    // set wireframe mode if necessary
    if (wireframe.getValue()){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // we will use vertex arrays, so enable them
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // get the number of meshes
    int meshCount;
    meshCount = pCalRenderer->getMeshCount();

    // render all meshes of the SoCal3DPuppet
    int meshId;
    for (meshId = 0; meshId < meshCount; meshId++){
        // get the number of submeshes
        int submeshCount;
        submeshCount = pCalRenderer->getSubmeshCount(meshId);

        // render all submeshes of the mesh
        int submeshId;
        for (submeshId = 0; submeshId < submeshCount; submeshId++){
            // select mesh and submesh for further data access
            if (pCalRenderer->selectMeshSubmesh(meshId, submeshId)){
                unsigned char meshColor[4];
                GLfloat materialColor[4];

                // set the material ambient color
                pCalRenderer->getAmbientColor(&meshColor[0]);
                materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
                glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
/******************************************************
                SbColor col = SbColor(materialColor[0],materialColor[1],materialColor[2]);
                SoAmbientColorElement::set(state, this, 1, &col);
// ******************************************************/

                // set the material diffuse color
                pCalRenderer->getDiffuseColor(&meshColor[0]);
                materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
                glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
/******************************************************
                col = SbColor(materialColor[0],materialColor[1],materialColor[2]);
                SoDiffuseColorElement::set(state, this, 1, &col);
// ******************************************************/


                // set the material specular color
                pCalRenderer->getSpecularColor(&meshColor[0]);
                materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
                glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
/******************************************************
                col = SbColor(materialColor[0],materialColor[1],materialColor[2]);
                SoSpecularColorElement::set(state, this, 1, &col);
// ******************************************************/


                // set the material shininess factor
                float shininess;
                shininess = 50.0f; //TODO: pCalRenderer->getShininess();
                glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
/******************************************************
                SoShininessElement::set(state, this, 1, &shininess);
// ******************************************************/


                // get the transformed vertices of the submesh
                static float meshVertices[30000][3];
                int vertexCount;
                vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

                // get the transformed normals of the submesh
                static float meshNormals[30000][3];
                pCalRenderer->getNormals(&meshNormals[0][0]);

                // get the texture coordinates of the submesh
                static float meshTextureCoordinates[30000][2];
                int textureCoordinateCount;
                textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

                // get the faces of the submesh
                static CalIndex meshFaces[50000][3];
                int faceCount;
                faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

                // set the vertex and normal buffers
                glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
                glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

                // set the texture coordinate buffer and state if necessary
                if ((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0)){
                    glEnable(GL_TEXTURE_2D);
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    glEnable(GL_COLOR_MATERIAL);

                    // set the texture id we stored in the map user data
                    glBindTexture(GL_TEXTURE_2D, (GLuint)pCalRenderer->getMapUserData(0));

                    // set the texture coordinate buffer
                    glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
                    glColor3f(1.0f, 1.0f, 1.0f);
                }

                // draw the submesh
                if (sizeof(CalIndex)==2) {
                    glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
                }
                else {
                    glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);
                }

                // disable the texture coordinate state if necessary
                if ((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0)) {
                    glDisable(GL_COLOR_MATERIAL);
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    glDisable(GL_TEXTURE_2D);
                }

            } // end of if (pCalRenderer->selectMeshSubmesh)
        } // end of for (submeshId)
    } // end of for (meshId)

    // clear vertex array state
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_DEPTH_TEST);

    // reset wireframe mode if necessary
    if (wireframe.getValue()){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // end the rendering
    pCalRenderer->endRendering();
}

void SoCal3DPuppet::setMotionBlending()
{
    for (int i=0;i<animationId->getLength();i++){
        if (blendingFactor[i]<=0) calModel->getMixer()->clearCycle((*animationId)[i],delayOut.getValue());
        else calModel->getMixer()->blendCycle((*animationId)[i],blendingFactor[i],delayIn.getValue());
    }
}

bool SoCal3DPuppet::findCoreModel(SbString *fullFileName)
{
    calCoreModel=NULL;
    for (int i=0;i<calCoreModelList.getLength();i++){
        calCoreModel=(CalCoreModel*)(calCoreModelList[i]);
        SbString *userData=(SbString*)(calCoreModel->getUserData());
        if (*userData==*fullFileName) break;
    }
    printf("CAL3DPUPPET: core model in file '%s' %s, ",fullFileName->getString(),i==calCoreModelList.getLength() ? "does not exist" : "already exists");
    printf("%s...\n",i==calCoreModelList.getLength() ? "creating new" : "loading old");
    if (i==calCoreModelList.getLength()) {
        printf("CAL3DPUPPET: loading config file '%s'...\n",fullFileName->getString());
        return false;
    }
    else {
        animationId=(SbIntList*)(animationIdList[i]);
        animationName=(SbStringList*)(animationNameList[i]);
        return true;
    }
}

double SoCal3DPuppet::getCurrentTime()
{
    return ((SoSFTime*)SoDB::getGlobalField("realTime"))->getValue().getValue();
}

void SoCal3DPuppet::addBoneManipulationFunction(BoneManipulationCB *cb, void *data)
{
    boneManipCallback.push_back(cb);
    boneManipCallbackData.push_back(data);
}

void SoCal3DPuppet::removeBoneManipulationFunction(BoneManipulationCB *cb, void *data)
{
    for (int i=0;i<boneManipCallback.size();i++){
        if (cb==boneManipCallback[i] && data==boneManipCallbackData[i]){
            boneManipCallback[i]=boneManipCallback.back();
            boneManipCallbackData[i]=boneManipCallbackData.back();
            boneManipCallback.pop_back();
            boneManipCallbackData.pop_back();
        }
    }
}

void SoCal3DPuppet::update(float deltaTime)
{
    if (!directManipulation.getValue()) calModel->update(deltaTime);
    else {
        calModel->getMixer()->updateAnimation(deltaTime);
        for (int i=0;i<boneManipCallback.size();i++){
            boneManipCallback[i](boneManipCallbackData[i],this);
        }
        calModel->getMorphTargetMixer()->update(deltaTime);
        calModel->getPhysique()->update();
        calModel->getSpringSystem()->update(deltaTime);
    }
}