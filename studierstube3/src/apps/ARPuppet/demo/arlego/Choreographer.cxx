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
/* Choreographer.cxx contains the implementation of class Choreographer
 *
 * @author Istvan Barakonyi
 *
 * $Id: Choreographer.cxx 4127 2005-01-20 14:03:28Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/engines/SoSelectOne.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SbViewportRegion.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// own classes
#include "../../characters/cal3DPuppet/SoCal3DPuppeteer.h"
#include "../../characters/legoPuppet/SoLegoPuppeteer.h"
#include "Choreographer.h"
#include "Director.h"

static const int cube_plane_index[] =
{
    0, 1, 3,
    5, 4, 6,
    1, 5, 7,
    4, 0, 2,
    4, 5, 1,
    2, 3, 7
};

const SbString Choreographer::minifigIDString("minifig");
const SbString Choreographer::rcxIDString("rcx");
const SbString Choreographer::mptpIDString("mptp");

// TODO: currently hard-coded values!!!!!!!!!!!
const SbVec3f robotBBLimits(0.17,0.05,0.19);
//const SbVec3f minifigOffset(????????????????);

SO_KIT_SOURCE(Choreographer);

void Choreographer::initClass()
{
    SO_KIT_INIT_CLASS(Choreographer, SoChoreographerKit, "SoChoreographerKit");
}

Choreographer::Choreographer()
{
    SO_KIT_CONSTRUCTOR(Choreographer);

    SO_KIT_ADD_CATALOG_ENTRY(bbSwitch, SoSwitch, FALSE, this, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbOff, SoSeparator, FALSE, bbSwitch, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbOn, SoSeparator, FALSE, bbSwitch, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbTransformBase, SoTransform, FALSE, bbOn, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbTransformOffset, SoTransform, FALSE, bbOn, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbMaterial, SoMaterial, TRUE, bbOn, \0 , TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbDrawStyle, SoDrawStyle, TRUE, bbOn, \0 , TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbCube, SoCube, FALSE, bbOn, \0 , FALSE);

    SO_KIT_ADD_FIELD(minifigEvent, (NONE));
    SO_KIT_ADD_FIELD(insideBoundingVolume, (FALSE));
    SO_KIT_ADD_FIELD(handedness, (0));
    SO_KIT_ADD_FIELD(hsOffset, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(modelPartCenter, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(modelPartBoundingBox, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(showNextModelPartBB, (TRUE));
    SO_KIT_ADD_FIELD(minifigPath, (0.0f,0.0f,0.0f));

    SO_KIT_INIT_INSTANCE();

    // define command set
    commandString=
        "moveBack %d(0) \
         place %d %s %d %d(0) \
         explain %d \
         turn_upsidedown %d(0) \
         connect %d(0) \
         press_down %d(0) \
         sendTo %s";

    // field sensors
    insideBoundingVolumeSensor=new SoFieldSensor(insideBoundingVolumeCB,this);

    // init last minifig position snapped to the current model construction's bounding volume
    lastBVPos.setValue(0.17,0.05,0.19);

    // init puppeteer pointers to NULL
    minifig=NULL;
    rcx=NULL;
    mptp=NULL;

    // we'll initialize this value later
    originalMinifigStation=-1;

    setUpConnections(TRUE,TRUE);
}

Choreographer::~Choreographer()
{
    delete insideBoundingVolumeSensor;
}

SbBool Choreographer::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        // set up search structure to search for nodes that are important to the choreographer
        SoSeparator *puppeteerRoot=SO_GET_ANY_PART(this,"puppeteers",SoSeparator);
        puppeteerRoot->ref();
        SoSearchAction searchAction;
        SoPath *path;
        searchAction.setInterest(SoSearchAction::FIRST);
        searchAction.setSearchingAll(TRUE); // search every node in the scene graph regardless of switch settings   
        
        // set the name of the minifig Cal3D character's puppeteer
        searchAction.setType(SoCal3DPuppeteer::getClassTypeId(),TRUE);
        searchAction.apply(puppeteerRoot);
        path=searchAction.getPath();
        if (path!=NULL){
            minifig=(Character*)(((SoFullPath*)path)->getTail());
            minifig->id=minifigIDString;
            originalMinifigStation=minifig->station[0];
        }
        else printf("CHOREOGRAPHER: warning: no minifig puppeteer has been defined\n");

        // set the name of the LEGO RCX's puppeteer
        searchAction.setType(SoLegoPuppeteer::getClassTypeId(),TRUE);
        searchAction.apply(puppeteerRoot);
        path=searchAction.getPath();
        if (path!=NULL){
            rcx=(SoLegoPuppeteer*)(((SoFullPath*)path)->getTail());
            rcx->id=rcxIDString;
            rcx->boundingBoxMode=rcx->OBB;
        }
        else printf("CHOREOGRAPHER: warning: no lego robot puppeteer has been defined\n");

        // set the name of the puppeteer of the model part to place
        searchAction.setType(SoPuppeteerKit::getClassTypeId(),FALSE);
        searchAction.apply(puppeteerRoot);
        path=searchAction.getPath();
        if (path!=NULL){
            mptp=(SoPuppeteerKit*)(((SoFullPath*)path)->getTail());
            mptp->id=mptpIDString;
            mptp->boundingBoxMode=mptp->OBB;
        }
        else printf("CHOREOGRAPHER: warning: no 'model part to place' puppeteer has been defined\n");

        // init minifig path info
        for (int i=0;i<6;i++) minifigPath.set1Value(i,0,0,0);

        // set up visualization structure for the model parts (object center, BB size, BB color etc.)
        setupModelPartVisualization();

        // attach field sensors
        insideBoundingVolumeSensor->attach(&insideBoundingVolume);
    }
    else {
        insideBoundingVolumeSensor->detach();
    }
    
    // set up connections in parent class SoPuppeteerKit
    SoChoreographerKit::setUpConnections(onOff,doItAlways);

    return !(connectionsSetUp = onOff);
}

void Choreographer::insideBoundingVolumeCB(void *data,SoSensor *sensor)
{
    Choreographer *ch=(Choreographer*)data;

    for (int i=0;i<ch->insideBoundingVolume.getNum();i++){
        if (i==Choreographer::RCX && ch->insideBoundingVolume[i]) {
            ch->minifigEvent=NEAR_RCX;
            return;
        }
        else if (i==Choreographer::LOCALMONITOR && ch->insideBoundingVolume[i]) {
            ch->minifigEvent=NEAR_DISPLAY;
            return;
        }
    }

    ch->minifigEvent=NONE;
}

void Choreographer::setupModelPartVisualization()
{
    if (rcx==NULL) return;

    // calculate BB centers and min/max vertices
    for (int i=0;i<rcx->getPuppetNum();i++){
        SoNode *modelPartPointer=rcx->getPuppet(i);
        SoGetBoundingBoxAction *bbAction=new SoGetBoundingBoxAction(SbViewportRegion());
        modelPartPointer->ref();
        bbAction->apply(modelPartPointer);
        SbBox3f bb=bbAction->getBoundingBox();
        modelPartCenter.set1Value(i,bb.getCenter());
        modelPartBoundingBox.set1Value(i*2,bb.getMin());
        modelPartBoundingBox.set1Value(i*2+1,bb.getMax());
    }
    modelPartCenter.setNum(rcx->getPuppetNum());
    modelPartBoundingBox.setNum(rcx->getPuppetNum()*2);

    // set up bounding box visualization for the next model part to be place
    SoSwitch *bbs=SO_GET_ANY_PART(this,"bbSwitch",SoSwitch);
    SoMaterial *bbm=(SoMaterial*)getAnyPart("bbMaterial",FALSE);
    SoDrawStyle *bbds=(SoDrawStyle*)getAnyPart("bbDrawStyle",FALSE);
    SoTransform *bbtb=SO_GET_ANY_PART(this,"bbTransformBase",SoTransform);
    SoTransform *bbto=SO_GET_ANY_PART(this,"bbTransformOffset",SoTransform);
    SoCube *bbc=SO_GET_ANY_PART(this,"bbCube",SoCube);

    // field connection to turn BB on/off
    bbs->whichChild.connectFrom(&showNextModelPartBB);

    // if no material has been defined, define a default material
    if (bbm==NULL) {
        bbm=new SoMaterial;
        bbm->diffuseColor.setValue(1.0,0.0,0.0);
        bbm->ambientColor.setValue(1.0,0.0,0.0);
        bbm->emissiveColor.setValue(1.0,0.0,0.0);
        bbm->shininess.setValue(1.0);
        this->setPart("bbMaterial",bbm);
    }

    // if no draw style has been defined, define a default draw style
    if (bbds==NULL) {
        bbds=new SoDrawStyle;
        bbds->style.setValue(SoDrawStyle::LINES);
        this->setPart("bbDrawStyle",bbds);
    }

    // connect base transformation parameters from RCX's transformation parameters
    bbtb->translation.connectFrom(&rcx->position);
    bbtb->rotation.connectFrom(&rcx->orientation);
    bbtb->scaleFactor.connectFrom(&rcx->scale);

    // initialize cube to 0 size
    bbc->width=0; bbc->height=0; bbc->depth=0;
}

bool Choreographer::implementCommand()
{
    if (isCommandType("sendTo")){
        if (!strcmp(getStringParam(0),"RCX")){
            printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> sent to RCX\n");
            SbVec3f pos=minifig->position.getValue()-rcx->position.getValue();
            SbVec3f newPos;
            rcx->orientation.getValue().inverse().multVec(pos,newPos);
            minifig->station=rcx->station[0];
            minifig->positionOffset.setValue(newPos);
            minifig->orientationOffset.setValue(SbRotation::identity());
            lastBVPos=newPos;
        }
        else if (!strcmp(getStringParam(0),"PUC")){
            printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> sent to PUC\n");
            minifig->station=originalMinifigStation;
            minifig->snapToStation.setValue();
        }
        else printf("CHOREOGRAPHER: invalid parameter (%s) for sendTo command, should be RCX or PUC\n");
        return true;
    }
    else return SoChoreographerKit::implementCommand();
}

bool Choreographer::choreograph()
{
    // format: place %d(No. of hands = [1,2]) %s(side to mount = "bottom|top|front|back|right|left") %d(No. of model part = [0,...]) %d(animationModus),
    // where [0=INTRO_ONLY,1=PLACE_ONLY,2=INTRO_AND_PLACE]))
    if (isCommandType("place")){

        // check parameter validity
        int numHands=getIntParam(0); handedness=(numHands==1 || numHands==2) ? numHands-1 : 0;
        int modelPartIndex=getIntParam(1);
        int animationModus=getIntParam(2); // Director::INTRO_ONLY, PLACE_ONLY, INTRO_AND_PLACE
        SoNode *currentTilePointer;
        if (modelPartIndex>=modelPartCenter.getNum() || 
            rcx==NULL || 
            modelPartIndex>=rcx->getPuppetNum() ||
            modelPartIndex>=modelPartCenter.getNum() ||
            (modelPartIndex+1)*2>modelPartBoundingBox.getNum() ||
            (currentTilePointer=rcx->getPuppet(modelPartIndex))==NULL ||
            mptp==NULL
           ) return true;

        // set appropriate scene graph in the 'model part to place' puppeteer
        SoSeparator *sep=new SoSeparator;
        SoTransform *trans=new SoTransform;
        trans->translation.setValue(-modelPartCenter[modelPartIndex]);
        currentTilePointer=rcx->getPuppet(modelPartIndex);
        sep->addChild(trans);
        sep->addChild(currentTilePointer);
        if (mptp->getPuppetNum()) mptp->replacePuppet(0,sep);
        else mptp->addPuppet(sep);

        // set bounding box parameters and offset for minifig hotspots
        SoCube *cube=SO_GET_ANY_PART(this,"bbCube",SoCube);
        SbVec3f diff=modelPartBoundingBox[modelPartIndex*2+1]-modelPartBoundingBox[modelPartIndex*2];
        cube->width=diff[0];
        cube->height=diff[1];
        cube->depth=diff[2];

        // set transformation offset
        SoTransform *bbto=SO_GET_ANY_PART(this,"bbTransformOffset",SoTransform);
        bbto->translation.setValue(modelPartCenter[modelPartIndex]);

        // set target location
        SbVec3f pos=modelPartCenter[modelPartIndex];
        SbVec3f scale=rcx->scale.getValue();

        // set rotation and minifig hotspot offset for the model part to place
        std::string dirStr=getStringParam(0);
        float offset=0.0f;
        if (dirStr.find("front")!=dirStr.npos) {
            minifig->hotspotRot.setValue(SbRotation::identity());
            offset=diff[2]/2.0f*scale[2];
            pos+=SbVec3f(0,-0.11,0.12); // TODO: hard-coded
        }
        else if (dirStr.find("back")!=dirStr.npos) {
            minifig->hotspotRot.setValue(SbVec3f(0,1,0),3.14f);
            offset=diff[2]/2.0f*scale[2];
            //pos+=SbVec3f(-minifigOffset[0],minifigOffset[1],-minifigOffset[2]);
            pos+=SbVec3f(0,-0.11,-0.12); // TODO: hard-coded
        }
        else if (dirStr.find("left")!=dirStr.npos) {
            minifig->hotspotRot.setValue(SbVec3f(0,1,0),1.57f);
            offset=diff[0]/2.0f*scale[0];
            //pos+=SbVec3f(-minifigOffset[2],minifigOffset[1],-minifigOffset[0]);
            pos+=SbVec3f(0.12,-0.11,0); // TODO: hard-coded
        }
        else if (dirStr.find("right")!=dirStr.npos) {
            minifig->hotspotRot.setValue(SbVec3f(0,1,0),-1.57f);
            offset=diff[0]/2.0f*scale[0];
            //pos+=SbVec3f(minifigOffset[2],minifigOffset[1],-minifigOffset[0]);
            pos+=SbVec3f(-0.12,-0.11,0); // TODO: hard-coded
        }
        else {
            printf("CHOREOGRAPHER: unknow direction string ('%s') in animation hint for tile placement, using default rotation\n",getStringParam(0));
            minifig->hotspotRot.setValue(SbRotation::identity());
            offset=diff[2]/2.0f*scale[2];
            pos+=SbVec3f(0,-0.11,0.12); // TODO: hard-coded
        }
        hsOffset.setValue(0,0,offset);

        // show model part
        if (mptp!=NULL) mptp->showPuppeteer=TRUE;

        // choreograph command
        char str[255];
        char *path=new char[255];
        getPathString(pos[0]*scale[0],pos[1]*scale[1],pos[2]*scale[2],dirStr,path);
        sprintf(str,"place `%s` %d `%s` %d",path,numHands,getStringParam(0),animationModus);
        currentCommand=str;
    }
    else if (isCommandType("moveBack")){
        // hide model part
        if (mptp!=NULL) mptp->showPuppeteer=FALSE;
        char str[255];
        sprintf(str,"moveBack %f %f %f %d",lastBVPos[0],lastBVPos[1],lastBVPos[2],getGotoTime(minifig->position.getValue(),lastBVPos));
        currentCommand=str;
    }
    else if (isCommandType("explain")){
        // check parameter range
        int modelPartIndex=getIntParam(0);
        if (modelPartIndex>=modelPartCenter.getNum() || mptp==NULL) return true;

        // set target location
        SbVec3f pos=modelPartCenter[modelPartIndex];
        SbVec3f scale=rcx->scale.getValue();

        // hide model part
        if (mptp!=NULL) mptp->showPuppeteer=FALSE;

        // choreograph command
        char str[255];
        sprintf(str,"explain %f %f %f",pos[0]*scale[0],pos[1]*scale[1],pos[2]*scale[2]);
        currentCommand=str;
    }
    else if (isCommandType("turn_upsidedown")){
        // hide model part
        if (mptp!=NULL) mptp->showPuppeteer=FALSE;

        char str[255];
        sprintf(str,"play `turn_upsidedown` 1000");
        currentCommand=str;
    }
    else if (isCommandType("connect")){
        // hide model part
        if (mptp!=NULL) mptp->showPuppeteer=FALSE;

        char str[255];
        sprintf(str,"play `connect` 2000");
        currentCommand=str;
    }
    else if (isCommandType("press_down")){
        // hide model part
        if (mptp!=NULL) mptp->showPuppeteer=FALSE;

        char str[255];
        sprintf(str,"play `press_down` 2000");
        currentCommand=str;
    }

    return false;
}

void Choreographer::getPathString(float x,float y,float z,std::string &dirStr,char *path)
{
    // path planning:   | currentPos > p0 > p1 > p2 > p3 > target(x,y,z) |
    SbVec3f p0,p1,p2,p3;

    float velocity=10;

    // 1.) calculate p0
    p0=lastBVPos;

    // 2.) calculate p1
    if (dirStr.find("left")!=dirStr.npos || dirStr.find("right")!=dirStr.npos){
        p1[0] = p0[0];
        p1[1] = p0[1];
        p1[2] = (p0[2]>0) ? robotBBLimits[2] : -robotBBLimits[2];
    }
    else {
        p1[0] = (p0[0]>0) ? robotBBLimits[0] : -robotBBLimits[0];
        p1[1] = p0[1];
        p1[2] = p0[2];
    }
    
    // 3.) calculate p2
    if (dirStr.find("left")!=dirStr.npos || dirStr.find("right")!=dirStr.npos){
        p2[0] = (x>0) ? robotBBLimits[0] : -robotBBLimits[0];
        p2[1] = p1[1];
        p2[2] = p1[2];
    }
    else {
        p2[0] = p1[0];
        p2[1] = p1[1];
        p2[2] = (z>0) ? robotBBLimits[2] : -robotBBLimits[2];
    }

    // 4.) calculate p3
    if (dirStr.find("left")!=dirStr.npos || dirStr.find("right")!=dirStr.npos){
        p3[0] = (x>0) ? robotBBLimits[0] : -robotBBLimits[0];
        p3[1] = y;
        p3[2] = z;
    }
    else {
        p3[0] = x;
        p3[1] = y;
        p3[2] = (z>0) ? robotBBLimits[2] : -robotBBLimits[2];
    }

    minifigPath.set1Value(0,minifig->position.getValue());
    minifigPath.set1Value(1,p0);
    minifigPath.set1Value(2,p1);
    minifigPath.set1Value(3,p2);
    minifigPath.set1Value(4,p3);
    minifigPath.set1Value(5,SbVec3f(x,y,z));

    // store last point snapped to the bounding volume surface
    lastBVPos=p3;

    sprintf(path,"%f %f %f %d 0 , %f %f %f %d 0 , %f %f %f %d 0 , %f %f %f %d 0 , %f %f %f %d 0",
        p0[0],p0[1],p0[2],getGotoTime(minifig->position.getValue(),p0),
        p1[0],p1[1],p1[2],getGotoTime(p0,p1),
        p2[0],p2[1],p2[2],getGotoTime(p1,p2),
        p3[0],p3[1],p3[2],getGotoTime(p2,p3),
        x,y,z,getGotoTime(p3,SbVec3f(x,y,z))
    );
}

int Choreographer::getGotoTime(SbVec3f source,SbVec3f target)
{
    SbVec3f diff=source-target;
    return (int)(2500.0f/0.24*diff.length());
}