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
/* SoBoundingVolume.cxx contains the implementation of SoBoundingVolume
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoBoundingVolume.cxx 3852 2004-10-07 20:49:59Z bara $ 
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSwitch.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// own classes
#include "SoBoundingVolume.h"

SO_KIT_SOURCE(SoBoundingVolume);

void SoBoundingVolume::initClass()
{
    SO_KIT_INIT_CLASS(SoBoundingVolume, SoBaseKit, "BaseKit");
}

SoBoundingVolume::SoBoundingVolume()
{
    SO_KIT_CONSTRUCTOR(SoBoundingVolume);

    SO_KIT_ADD_CATALOG_ENTRY(root, SoSeparator, FALSE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(materialSwitch, SoSwitch, FALSE, root, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(materialPointOut, SoMaterial, FALSE, materialSwitch, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(materialPointIn, SoMaterial, FALSE, materialSwitch, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(drawStyle, SoDrawStyle, FALSE, root, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(volumeSwitch, SoSwitch, FALSE, root, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(sphereRoot, SoSeparator, FALSE, volumeSwitch, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(ifsRoot, SoSeparator, FALSE, volumeSwitch, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(sphere, SoSphere, FALSE, sphereRoot, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(coords, SoCoordinate3, FALSE, ifsRoot, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(indexedFaceSet, SoIndexedFaceSet, FALSE, ifsRoot, \0, FALSE);

    SO_KIT_ADD_FIELD(volumeType, (BOX));
    SO_KIT_ADD_FIELD(mode, (EXTERNAL));
    SO_KIT_ADD_FIELD(reset, ());
    SO_KIT_ADD_FIELD(cursorPosition, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(cursorButton, (FALSE));
    SO_KIT_ADD_FIELD(boundingVolume, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(boundingVolumeTest, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(boundingVolumeTestType, (NONE));
    SO_KIT_ADD_FIELD(boundingVolumeInside, (FALSE));
    SO_KIT_ADD_FIELD(visualization, (FALSE));
    SO_KIT_ADD_FIELD(eventGeneration, (FALSE));

    SO_KIT_DEFINE_ENUM_VALUE(VolumeType,POINT);
    SO_KIT_DEFINE_ENUM_VALUE(VolumeType,SPHERE);
    SO_KIT_DEFINE_ENUM_VALUE(VolumeType,RECTANGLE);
    SO_KIT_DEFINE_ENUM_VALUE(VolumeType,BOX);
    SO_KIT_SET_SF_ENUM_TYPE(volumeType, VolumeType);
    SO_KIT_SET_MF_ENUM_TYPE(boundingVolumeTestType, VolumeType);

    SO_KIT_DEFINE_ENUM_VALUE(Mode,DRAWN);
    SO_KIT_DEFINE_ENUM_VALUE(Mode,DRAWING);
    SO_KIT_DEFINE_ENUM_VALUE(Mode,EXTERNAL);
    SO_KIT_SET_SF_ENUM_TYPE(mode, Mode);

    SO_KIT_INIT_INSTANCE();

    resetSensor=new SoFieldSensor(resetCB,this);
    cursorButtonSensor=new SoFieldSensor(cursorButtonCB,this);
    cursorPositionSensor=new SoFieldSensor(cursorPositionCB,this);
    volumeTypeSensor=new SoFieldSensor(volumeTypeCB,this);
    boundingVolumeTestSensor=new SoFieldSensor(boundingVolumeTestCB,this);
    visualizationSensor=new SoFieldSensor(visualizationCB,this);

    ((SoSwitch*)(SO_GET_ANY_PART(this,"materialSwitch",SoSwitch)))->whichChild=0;

    // init variables
    clickCounter=0;
    faceIndex=NULL;
    plane=NULL;

    setUpConnections(TRUE,TRUE);
}

SoBoundingVolume::~SoBoundingVolume()
{
    cleanup();

    delete resetSensor;
    delete cursorButtonSensor;
    delete cursorPositionSensor;
    delete volumeTypeSensor;
    delete boundingVolumeTestSensor;
    delete visualizationSensor;
}

SbBool SoBoundingVolume::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        resetSensor->attach(&reset);
        cursorButtonSensor->attach(&cursorButton);
        cursorPositionSensor->attach(&cursorPosition);
        volumeTypeSensor->attach(&volumeType);
        boundingVolumeTestSensor->attach(&boundingVolumeTest);
        visualizationSensor->attach(&visualization);

        volumeTypeCB(this,NULL);
    }
    else {
        resetSensor->detach();
        cursorButtonSensor->detach();
        cursorPositionSensor->detach();
        volumeTypeSensor->detach();
        boundingVolumeTestSensor->detach();
        visualizationSensor->detach();
    }

    return !(connectionsSetUp = onOff);
}


void SoBoundingVolume::resetCB(void *data,SoSensor *sensor)
{
    SoBoundingVolume *bbd=(SoBoundingVolume*)data;

    bbd->clickCounter=0;
}

void SoBoundingVolume::cursorButtonCB(void *data,SoSensor *sensor)
{
    SoBoundingVolume *bbd=(SoBoundingVolume*)data;

    // return if dynamic drawing is inactive
    if (bbd->mode.getValue()!=DRAWING) return;

    if (bbd->cursorButton.getValue()) {
        if (bbd->clickCounter<bbd->controlPointNum) printf("BOUNDINGVOLUME: set BB vertex # %d\n",bbd->clickCounter);
        else if (bbd->clickCounter==bbd->controlPointNum) bbd->dumpResult();
        else return;
        bbd->clickCounter++;
    }
}

void SoBoundingVolume::cursorPositionCB(void *data,SoSensor *sensor)
{
    SoBoundingVolume *bbd=(SoBoundingVolume*)data;

    // return if dynamic drawing is inactive
    if (bbd->mode.getValue()!=DRAWING) return;

    int i;
    SoCoordinate3 *cd= SO_GET_ANY_PART(bbd,"coords",SoCoordinate3);

    SbVec3f cursorPos=bbd->cursorPosition.getValue();
    SbVec3f unitVector,projectedVector;
    switch (bbd->volumeType.getValue()){
        case SPHERE:
            // TODO: not implemented yet
            break;        
        case RECTANGLE:            
            switch (bbd->clickCounter){
                case 0:
                    for (i=0;i<4;i++) cd->point.set1Value(i,cursorPos);
                    break;
                case 1:
                    for (i=0;i<4;i++) {
                        if (i==1 || i==2) cd->point.set1Value(i,cursorPos);
                    }
                    break;
                case 2:
                    unitVector=cd->point[1]-cd->point[0]; unitVector.normalize();
                    projectedVector=cursorPos-(cd->point[0]+unitVector*(cursorPos-cd->point[0]).dot(unitVector));
                    for (i=0;i<4;i++) {
                        if (i>1) cd->point.set1Value(i,cd->point[i-2]+projectedVector);
                    }
                    break;
                default:
                    break;
            }
            break;
        case BOX:
            switch (bbd->clickCounter){
                case 0:
                    for (i=0;i<8;i++) cd->point.set1Value(i,cursorPos);
                    break;
                case 1:
                    for (i=0;i<8;i++) {
                        if (i==1 || i==2 || i==5 || i==6) cd->point.set1Value(i,cursorPos);
                    }
                    break;
                case 2:
                    unitVector=cd->point[1]-cd->point[0]; unitVector.normalize();
                    projectedVector=cursorPos-(cd->point[0]+unitVector*(cursorPos-cd->point[0]).dot(unitVector));
                    for (i=0;i<8;i++) {
                        if (i>3) cd->point.set1Value(i,cd->point[i-4]+projectedVector);
                    }     
                    break;
                case 3:
                    unitVector=SbPlane(cd->point[0],cd->point[1],cd->point[5]).getNormal(); unitVector.normalize();
                    projectedVector=(cursorPos-(cd->point[0]+cd->point[5])/2.0f).dot(unitVector)*unitVector;                    
                    for (i=0;i<8;i++) {
                        switch (i) {
                            case 2:
                                cd->point.set1Value(i,cd->point[1]+projectedVector);
                    	        break;
                            case 3:
                                cd->point.set1Value(i,cd->point[0]+projectedVector);
                    	        break;
                            case 6:
                                cd->point.set1Value(i,cd->point[5]+projectedVector);
                    	        break;
                            case 7:
                                cd->point.set1Value(i,cd->point[4]+projectedVector);
                    	        break;
                            default:
                                break;
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    bbd->calculateParams();
}

void SoBoundingVolume::volumeTypeCB(void *data,SoSensor *sensor)
{
    SoBoundingVolume *bbd=(SoBoundingVolume*)data;

    // clean up previous data if necessary
    bbd->cleanup();

    SoIndexedFaceSet *ifs=SO_GET_ANY_PART(bbd,"indexedFaceSet",SoIndexedFaceSet);
    SoCoordinate3 *cd=SO_GET_ANY_PART(bbd,"coords",SoCoordinate3);
    SoSwitch *vs=SO_GET_ANY_PART(bbd,"volumeSwitch",SoSwitch);

    switch (bbd->volumeType.getValue()){
        case POINT:
            // TODO: not implemented yet
            break;
        case SPHERE:
            bbd->controlPointNum=2;
            // TODO: not implemented yet
            break;
        case RECTANGLE:
            bbd->controlPointNum=2;
            bbd->faceIndex=new int[5];
            bbd->faceIndex[0]=0; bbd->faceIndex[1]=1; bbd->faceIndex[2]=3; bbd->faceIndex[3]=2;
            bbd->faceIndex[4]=SO_END_FACE_INDEX;

            ifs->coordIndex.setValues(0,5,bbd->faceIndex);
            ifs->coordIndex.setNum(5);
            cd->point.setNum(4);
            bbd->boundingVolume.connectFrom(&cd->point);

            bbd->plane=new SbPlane();
            break;
        case BOX:
            bbd->controlPointNum=4;
            bbd->faceIndex=new int[30];
            bbd->faceIndex[0]=0; bbd->faceIndex[1]=1; bbd->faceIndex[2]=2; bbd->faceIndex[3]=3;
            bbd->faceIndex[4]=SO_END_FACE_INDEX;
            // left plane
            bbd->faceIndex[5]=5; bbd->faceIndex[6]=6; bbd->faceIndex[7]=2; bbd->faceIndex[8]=1;
            bbd->faceIndex[9]=SO_END_FACE_INDEX;
            // right plane
            bbd->faceIndex[10]=4; bbd->faceIndex[11]=0; bbd->faceIndex[12]=3; bbd->faceIndex[13]=7;
            bbd->faceIndex[14]=SO_END_FACE_INDEX;    
            // bottom plane
            bbd->faceIndex[15]=7; bbd->faceIndex[16]=3; bbd->faceIndex[17]=2; bbd->faceIndex[18]=6;
            bbd->faceIndex[19]=SO_END_FACE_INDEX;
            // top plane
            bbd->faceIndex[20]=5; bbd->faceIndex[21]=1; bbd->faceIndex[22]=0; bbd->faceIndex[23]=4;
            bbd->faceIndex[24]=SO_END_FACE_INDEX;    
            // far plane
            bbd->faceIndex[25]=7; bbd->faceIndex[26]=6; bbd->faceIndex[27]=5; bbd->faceIndex[28]=4;
            bbd->faceIndex[29]=SO_END_FACE_INDEX;
            
            ifs->coordIndex.setValues(0,30,bbd->faceIndex);
            ifs->coordIndex.setNum(30);
            cd->point.setNum(8);
            bbd->boundingVolume.connectFrom(&cd->point);
            
            bbd->plane=new SbPlane[6];
            break;
        default:
            break;
    }

    if (bbd->mode.getValue()==SoBoundingVolume::DRAWN) bbd->calculateParams();
    bbd->visualizationCB(bbd,NULL);
}

void SoBoundingVolume::boundingVolumeTestCB(void *data,SoSensor *sensor)
{
    SoBoundingVolume *bbd=(SoBoundingVolume*)data;

    if (!bbd->eventGeneration.getValue()) return;

    bool inside=false;
    int vertexCounter=0;
    for (int i=0;i<bbd->boundingVolumeTestType.getNum();i++){
        if (bbd->boundingVolumeTestType[i]==SoBoundingVolume::NONE) continue;
        int vertexNum=bbd->getVertexNum(bbd->boundingVolumeTestType[i]);
        bool intersectTest=bbd->intersectBV(bbd->boundingVolumeTestType[i],vertexCounter,vertexNum);
        inside|=intersectTest;
        bbd->boundingVolumeInside.set1Value(i,intersectTest);
        vertexCounter+=vertexNum;
    }
    ((SoSwitch*)(SO_GET_ANY_PART(bbd,"materialSwitch",SoSwitch)))->whichChild=inside ? 1 : 0;
}

void SoBoundingVolume::visualizationCB(void *data,SoSensor *sensor)
{
    SoBoundingVolume *bbd=(SoBoundingVolume*)data;
    SoSwitch *vs=SO_GET_ANY_PART(bbd,"volumeSwitch",SoSwitch);

    if (bbd->visualization.getValue()){
        switch (bbd->volumeType.getValue()){
            case POINT:
                // TODO: not implemented yet
                break;
            case SPHERE:
                vs->whichChild=0;
                break;
            case RECTANGLE:
                vs->whichChild=1;
                break;
            case BOX:
                vs->whichChild=1;
                break;
            default:
                vs->whichChild=-1;
                break;
        }
    }
    else vs->whichChild=-1;
}

void SoBoundingVolume::cleanup()
{
    if (faceIndex) delete[]faceIndex;
    if (plane) delete[]plane;
}

void SoBoundingVolume::dumpResult()
{
    SoCoordinate3 *cd=SO_GET_ANY_PART(this,"coords",SoCoordinate3);

    printf("BOUNDINGVOLUME: ****** control point dump start ******\n");
    for (int i=0;i<cd->point.getNum();i++){
        SbVec3f p=cd->point[i];
        printf("%f %f %f",p[0],p[1],p[2]);
        if (i<controlPointNum-1) printf(",\n");
        else printf("\n");
    }
    printf("BOUNDINGVOLUME: ****** control point dump finish *****\n");
}

int SoBoundingVolume::getVertexNum(int type)
{
    switch (type) {
        case POINT:
            return 1;
        case SPHERE:
            return 2;
        case RECTANGLE:
            return 4;
        case BOX:
            return 8;
        default:
            return 0;
    }
}

void SoBoundingVolume::calculateParams()
{
    SoCoordinate3 *cd=SO_GET_ANY_PART(this,"coords",SoCoordinate3);

    int i;
    switch (volumeType.getValue()){
        case POINT:
            // TODO: not implemented yet
            break;
        case SPHERE:
            // TODO: not implemented yet
            break;
        case RECTANGLE:
            if (cd->point.getNum()<4) return;
            plane[0]=SbPlane(cd->point[0],cd->point[1],cd->point[2]);
            centerPoint=(cd->point[faceIndex[0]]+cd->point[faceIndex[2]])/2.0f;
            break;
        case BOX:
            if (cd->point.getNum()<8) return;
            for (i=0;i<6;i++){
                plane[i]=SbPlane(
                    cd->point[faceIndex[i*5]],
                    cd->point[faceIndex[i*5+1]],
                    cd->point[faceIndex[i*5+2]]
                );
            }
            centerPoint=(cd->point[faceIndex[3]]+cd->point[faceIndex[5]])/2.0f;
            break;
        default:
            break;
    }
}

bool SoBoundingVolume::intersectBV(int volumeTestType,int vertexCounter,int vertexNum)
{
    SoCoordinate3 *cd=SO_GET_ANY_PART(this,"coords",SoCoordinate3);

    if (mode.getValue()==EXTERNAL) calculateParams();
    
    int i;
    SbVec3f p;
    switch (volumeType.getValue()){
        case POINT:
            // TODO: not implemented yet
            break;
        case SPHERE:
            // TODO: not implemented yet
            break;
        case RECTANGLE:                
            plane[0]=SbPlane(cd->point[0],cd->point[1],cd->point[2]);
            // TODO: not implemented yet
            break;
        case BOX:
            switch (volumeTestType){
                case POINT:
                    p=boundingVolumeTest[vertexCounter];
                    for (i=0;i<6;i++){
                        // check whether vertices is inside or outside the currently examined plane while keeping in mind the plane's orientation
                        if (((plane[i].isInHalfSpace(centerPoint) ^ plane[i].isInHalfSpace(p)))) return false;
                    }
                    break;
                case SPHERE:
                    // TODO: not implemented yet
                    break;
                case RECTANGLE:
                    // TODO: not implemented yet
                    break;
                case BOX:
                    // TODO: not implemented yet
                    break;
                default:
                    return false;
                    break;
            }
            break;
        default:
            break;
    }

    return true;
}
