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
/* SoPuppeteerKit.cxx contains the implementation of class SoPuppeteerKit, 
 * the general Augmented Reality Agent class
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoPuppeteerKit.cxx 4007 2004-10-27 07:50:43Z bara $
 * @file
 */
/* ======================================================================== */

#undef DEBUG_ARPUPPET

// Inventor classes
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/actions/SoSearchAction.h>

// Stb specific classes
#include <stbapi/interaction/SoDragKit.h>
#include <stbapi/misc/SoStroke.h>
#include <stbapi/misc/SoSocketModule.h>

// own classes
#include "SoPuppeteerKit.h"
#include "SoViewingFrustum.h"

SO_KIT_SOURCE(SoPuppeteerKit);

void SoPuppeteerKit::initClass()
{
    SoCommandParserKit::initClass();
    SoViewingFrustum::initClass();
    SoTransformCapture::initClass();

    SO_KIT_INIT_CLASS(SoPuppeteerKit, SoCommandParserKit, "SoCommandParserKit");
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: SoPuppeteerKit initialized\n");
#endif
}

SoPuppeteerKit::SoPuppeteerKit()
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: constructor\n");
#endif

    SO_KIT_CONSTRUCTOR(SoPuppeteerKit);
    
    // readable/writable attributes:

    // node kit parts -> scene graphs to attach
    SO_KIT_ADD_CATALOG_ENTRY(rootSwitch, SoSwitch, FALSE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(puppetRoot, SoSeparator, FALSE, rootSwitch, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(auxRoot, SoSeparator, FALSE, rootSwitch, \0, TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(transformPosBase, SoTranslation, FALSE, puppetRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(transformPosOffset, SoTranslation, FALSE, puppetRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(transformRotBase, SoRotation, FALSE, puppetRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(transformRotOffset, SoRotation, FALSE, puppetRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(transformScale, SoScale, FALSE, puppetRoot, \0, FALSE);

    SO_KIT_ADD_CATALOG_ENTRY(puppets, SoDragKit, FALSE, puppetRoot, \0 ,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(dragTransform, SoTransform, FALSE, puppetRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(attributes, SoMultiSwitch, FALSE, puppetRoot, \0 , TRUE);
    
    SO_KIT_ADD_CATALOG_ENTRY(vfSwitch, SoSwitch, FALSE, puppetRoot, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(vfRoot, SoSeparator, FALSE, vfSwitch, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(vfMaterial, SoMaterial, TRUE, vfRoot, \0 , TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(vfDrawStyle, SoDrawStyle, TRUE, vfRoot, \0 , TRUE);
    SO_KIT_ADD_CATALOG_LIST_ENTRY(viewingFrustums, SoSeparator, FALSE, vfRoot, \0, SoViewingFrustum, TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(velSwitch, SoSwitch, FALSE, auxRoot, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(velRoot, SoSeparator, FALSE, velSwitch, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(velTransform, SoTransform, FALSE, velRoot, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(velMaterial, SoMaterial, TRUE, velRoot, \0 , TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(velDrawStyle, SoDrawStyle, TRUE, velRoot, \0 , TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(velocityVis, SoStroke, TRUE, velRoot, \0 , TRUE);

    SO_KIT_ADD_CATALOG_ENTRY(bbSwitch, SoSwitch, FALSE, auxRoot, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbRoot, SoSeparator, FALSE, bbSwitch, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbTransform, SoTransform, FALSE, bbRoot, \0 , FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(bbMaterial, SoMaterial, TRUE, bbRoot, \0 , TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbDrawStyle, SoDrawStyle, TRUE, bbRoot, \0 , TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(bbCube, SoCube, FALSE, bbRoot, \0 , FALSE);

    // representation
    SO_KIT_ADD_FIELD(whichPuppets, (-3));
    SO_KIT_ADD_FIELD(whichAttributes, (-3));
    SO_KIT_ADD_FIELD(hotspotName, (""));
    SO_KIT_ADD_FIELD(hotspotCoord, (0.0,0.0,0.0));
    SO_KIT_ADD_FIELD(hotspotRot, (SbRotation::identity()));
    SO_KIT_ADD_FIELD(hotspotAbsCoord, (0.0,0.0,0.0));
    SO_KIT_ADD_FIELD(hotspotAbsRot, (SbRotation::identity()));
    SO_KIT_ADD_FIELD(hotspotOffsetCoord, (0.0,0.0,0.0));
    SO_KIT_ADD_FIELD(hotspotOffsetRot, (SbRotation::identity()));
    SO_KIT_ADD_FIELD(showPuppeteer, (TRUE));
    SO_KIT_ADD_FIELD(showAttributes, (TRUE));
    SO_KIT_ADD_FIELD(controlMode, (INVALID));
    SO_KIT_ADD_FIELD(station, (-1));
    SO_KIT_ADD_FIELD(positionBase, (0.0,0.0,0.0));
    SO_KIT_ADD_FIELD(positionOffset, (0.0,0.0,0.0));
    SO_KIT_ADD_FIELD(orientationBase, (SbRotation::identity()));
    SO_KIT_ADD_FIELD(orientationOffset, (SbRotation::identity()));
    SO_KIT_ADD_FIELD(scaleSet, (1.0f,1.0f,1.0f));
    SO_KIT_ADD_FIELD(snapToStation, ());
    SO_KIT_ADD_FIELD(dragButton, (0));
    SO_KIT_ADD_FIELD(draggingStation, (-1));
    SO_KIT_ADD_FIELD(boundingBox, (0.0,0.0,0.0));
    SO_KIT_ADD_FIELD(boundingBoxMode, (AABB));
    SO_KIT_ADD_FIELD(boundingBoxOn, (FALSE));
    SO_KIT_ADD_FIELD(highlightDragOn, (FALSE));
    SO_KIT_ADD_FIELD(viewingFrustumOn, (FALSE));
    SO_KIT_ADD_FIELD(velocityVisOn, (FALSE));
    SO_KIT_ADD_FIELD(velocityScale, (2.0));
    SO_KIT_ADD_FIELD(fovHotspot, (""));
    SO_KIT_ADD_FIELD(pickupHotspot, (""));
    SO_KIT_ADD_FIELD(speechText, (""));
    SO_KIT_ADD_FIELD(soundPlay, (FALSE));
    SO_KIT_ADD_FIELD(soundLoop, (FALSE));
    SO_KIT_ADD_FIELD(soundFileName, (""));
    SO_KIT_ADD_FIELD(stopSpeech, ());
    SO_KIT_ADD_FIELD(disableIdleBehavior, (FALSE));

    // read-only attributes
    SO_KIT_ADD_FIELD(position, (0.0,0.0,0.0));
    SO_KIT_ADD_FIELD(orientation, (SbRotation::identity()));
    SO_KIT_ADD_FIELD(scale, (1.0,1.0,1.0));
    SO_KIT_ADD_FIELD(velocity, (0.0));
    SO_KIT_ADD_FIELD(velocityDir, (0.0,0.0,0.0));
    SO_KIT_ADD_FIELD(dragging, (FALSE));

    // enum values
    SO_KIT_DEFINE_ENUM_VALUE(ControlMode,INVALID);
    SO_KIT_DEFINE_ENUM_VALUE(ControlMode,SCRIPT);
    SO_KIT_DEFINE_ENUM_VALUE(ControlMode,STATION);
    SO_KIT_DEFINE_ENUM_VALUE(ControlMode,DRAG);
    SO_KIT_DEFINE_ENUM_VALUE(ControlMode,EXTERNAL);
    SO_KIT_SET_SF_ENUM_TYPE(controlMode,ControlMode);

    SO_KIT_DEFINE_ENUM_VALUE(BoundingBoxMode,NONE);
    SO_KIT_DEFINE_ENUM_VALUE(BoundingBoxMode,AABB);
    SO_KIT_DEFINE_ENUM_VALUE(BoundingBoxMode,OBB);
    SO_KIT_SET_SF_ENUM_TYPE(boundingBoxMode,BoundingBoxMode);
    
    SO_KIT_INIT_INSTANCE();

    // tracking
    trackerPosition=new SoTrakEngine;
    trackerOrientation=new SoTrakEngine;
    trackerPosition->ref();
    trackerOrientation->ref();
    
    // rotate offset translation by base rotation
    positionOffsetRotator=new SoRotateVector;

    // to capture puppeteer transformation after the dragger
    transformCapture=new SoTransformCapture;

    // calculate absolute hotspot coordinates and rotations
    hotSpotRotatorAbs=new SoRotateVector;
    hotSpotRotatorOffset=new SoRotateVector;
    hotspotPosCalculator=new SoCalculator;
    hotSpotRotMultAbs=new MultRotRot;
    hotSpotRotMultOffset=new MultRotRot;

    // position
    positionInterpolator=new SoInterpolateVec3f;
    positionInterpolator->ref();
    positionTimer=new SoOneShot();

    // orientation
    orientationInterpolator=new SoInterpolateRotation;
    orientationInterpolator->ref();
    orientationTimer=new SoOneShot();

    // scale
    scaleTimer=new SoOneShot();
    scaleInterpolator=new SoInterpolateVec3f;
    scaleInterpolator->ref();

    // velocity
    positionOld=SbVec3f(0.0f,0.0f,0.0f);
    timeOld=SbTime::zero();
    for (int i=0;i<3;i++){
        velocityOld[i]=0;
        velocityDirOld[i]=SbVec3f(0,0,0);
    }

    // bounding box calculation
    computeBoundingBox=new SoComputeBoundingBox;
    bbCalculator=new SoCalculator;
    bbRotator=new SoRotateVector;
    bbRotator->ref();

    // field sensors
    whichPuppetsSensor=new SoFieldSensor(whichPuppetsCB,this);
    whichAttributesSensor=new SoFieldSensor(whichAttributesCB,this);
    controlModeSensor=new SoFieldSensor(controlModeCB,this);
    positionSensor=new SoFieldSensor(positionCB,this);
    orientationSensor=new SoFieldSensor(orientationCB,this);
    scaleSensor=new SoFieldSensor(scaleCB,this);
    showPuppeteerSensor=new SoFieldSensor(showPuppeteerCB,this);
    showAttributesSensor=new SoFieldSensor(showAttributesCB,this);
    snapToStationSensor=new SoFieldSensor(snapToStationCB,this);
    stationSensor=new SoFieldSensor(stationCB,this);
    draggingSensor=new SoFieldSensor(draggingCB,this);
    boundingBoxModeSensor=new SoFieldSensor(boundingBoxModeCB,this);
    boundingBoxOnSensor=new SoFieldSensor(boundingBoxOnCB,this);
    viewingFrustumOnSensor=new SoFieldSensor(viewingFrustumOnCB,this);
    velocityVisOnSensor=new SoFieldSensor(velocityVisOnCB,this);

    // list of commands
    commandString=
        "setPosition %f %f %f %d(0) \
         setPositionRel %f %f %f %d(0) \
         setOrientationQ %f %f %f %f %d(0) \
         setOrientationQRel %f %f %f %f %d(0) \
         setOrientationV %f %f %f %f %d(0) \
         setOrientationVRel %f %f %f %f %d(0) \
         setScale %f %f %f %d(0) \
         turnTowards %f %f %f %d(0) \
         lookAt %f %f %f %d(0) \
         pathMove %s \
         goTo %f %f %f %d(0) \
         speak %s \
         sound %s %d(0) %d(-1) \
         shutup %s \
         wait %d";

    setUpConnections(TRUE,TRUE);
}

SoPuppeteerKit::~SoPuppeteerKit()
{
    delete whichPuppetsSensor;
    delete whichAttributesSensor;
    delete controlModeSensor;
    delete positionSensor;
    delete orientationSensor;
    delete scaleSensor;
    delete showPuppeteerSensor;
    delete showAttributesSensor;
    delete snapToStationSensor;
    delete stationSensor;
    delete draggingSensor;
    delete boundingBoxOnSensor;
    delete viewingFrustumOnSensor;
    delete velocityVisOnSensor;
}

SbBool SoPuppeteerKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: setUpConnections %s %s for puppeteer %s\n",onOff ? "TRUE" : "FALSE",doItAlways ? "TRUE" : "FALSE",id.getValue().getString());
#endif
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        // attach sensors
        whichPuppetsSensor->attach(&whichPuppets);
        whichAttributesSensor->attach(&whichAttributes);
        controlModeSensor->attach(&controlMode);
        positionSensor->attach(&position);
        orientationSensor->attach(&orientation);
        scaleSensor->attach(&scale);
        showPuppeteerSensor->attach(&showPuppeteer);
        showAttributesSensor->attach(&showAttributes);
        snapToStationSensor->attach(&snapToStation);
        stationSensor->attach(&station);
        draggingSensor->attach(&dragging);
        boundingBoxModeSensor->attach(&boundingBoxMode);
        boundingBoxOnSensor->attach(&boundingBoxOn);
        viewingFrustumOnSensor->attach(&viewingFrustumOn);
        velocityVisOnSensor->attach(&velocityVisOn);

        // dragging
        SoDragKit *dragger=SO_GET_ANY_PART(this,"puppets",SoDragKit);
        dragger->dragButton.connectFrom(&dragButton);
        dragging.connectFrom(&dragger->dragging);
        draggingStation.connectFrom(&dragger->draggingStation);
        dragger->setPressCallback(pressCB,this);
        dragger->setReleaseCallback(releaseCB,this);
        dragger->setMoveCallback(moveCB,this);

        SoSeparator *sep=(SoSeparator*)(dragger->getPart("content",FALSE));

        // search for the first SoMultiSwitch node in the scenegraph attached to "puppeteers" 
        SoSearchAction searchAction;
        searchAction.setInterest(SoSearchAction::FIRST);
        searchAction.setSearchingAll(TRUE); // search every node in the scene graph regardless of switch settings   
        searchAction.setType(SoMultiSwitch::getClassTypeId(),TRUE);
        searchAction.apply(sep);
        SoPath *path=searchAction.getPath();
        if (path!=NULL) puppetSwitch=(SoMultiSwitch*)(((SoFullPath*)path)->getTail());
        else {
            puppetSwitch=new SoMultiSwitch;
            sep->addChild(puppetSwitch);
        }
        transformCapture->ref();
        sep->addChild(transformCapture);

        setCommandList();
        constructTrigger();

        stationCB(this,NULL);
        setupTransformation();
        setupBoundingBox();
        setupViewingFrustums();
        setupVelocityVisualization();
        controlModeCB(this,NULL);
        whichPuppetsCB(this,NULL);
        showPuppeteerCB(this,NULL);
        showAttributesCB(this,NULL);
        boundingBoxOnCB(this,NULL);
        boundingBoxModeCB(this,NULL);
        viewingFrustumOnCB(this,NULL);
        velocityVisOnCB(this,NULL);
    }
    else {
        whichPuppetsSensor->detach();
        whichAttributesSensor->detach();
        controlModeSensor->detach();
        positionSensor->detach();
        orientationSensor->detach();
        scaleSensor->detach();
        showPuppeteerSensor->detach();
        showAttributesSensor->detach();
        snapToStationSensor->detach();
        draggingSensor->detach();
        boundingBoxModeSensor->detach();
        boundingBoxOnSensor->detach();
        viewingFrustumOnSensor->detach();
        velocityVisOnSensor->detach();

        SoDragKit *dragger=SO_GET_ANY_PART(this,"puppets",SoDragKit);
        dragger->removePressCallback(pressCB,this);
        dragger->removeReleaseCallback(releaseCB,this);
        dragger->removeMoveCallback(moveCB,this);
        
        resetTriggerList();
    }
    return !(connectionsSetUp = onOff);
}

void SoPuppeteerKit::addPuppet(SoNode *puppet)
{
    puppetSwitch->addChild(puppet);
}

void SoPuppeteerKit::replacePuppet(int num,SoNode *puppet)
{
    if (num>=getPuppetNum()) printf("PUPPETEER: removePuppet index overflow\n");
    else puppetSwitch->replaceChild(num,puppet);
}

void SoPuppeteerKit::removePuppet(int num)
{
    if (num>=getPuppetNum()) printf("PUPPETEER: removePuppet index overflow\n");
    else puppetSwitch->removeChild(num);
}

SoNode* SoPuppeteerKit::getPuppet(int num)
{
    if (getActivePuppetNum()>getPuppetNum()) {
        printf("PUPPETEER: whichPuppets index overflow\n");
        return NULL;
    }
    if (num>=puppetSwitch->getNumChildren()){
        printf("PUPPETEER: getPuppet(%d) index overflow\n",num);
        return NULL;
    }
    return puppetSwitch->getChild(num);
}

SoNode* SoPuppeteerKit::getActivePuppet(int num)
{
    // traverse all children
    if (whichPuppets[0]==-1 || whichPuppets[0]==-2) return NULL;
    else if (whichPuppets[0]==-3) return getPuppet(num);
    return getPuppet(whichPuppets[num]);
}

int SoPuppeteerKit::getPuppetNum()
{
    return puppetSwitch->getNumChildren();
}

int SoPuppeteerKit::getActivePuppetNum()
{
    if (!whichPuppets.getNum() || whichPuppets[0]==-1) return 0; // -1 = SO_SWITCH_NONE = traverse none of the children
    else if (whichPuppets[0]==-2) return 0; // -2 = SO_SWITCH_INHERIT = inherit value from state --> TODO: not supported now
    else if (whichPuppets[0]==-3) return puppetSwitch->getNumChildren(); // -3 = SO_SWITCH_ALL = traverse all children
    else return whichPuppets.getNum();
}

bool SoPuppeteerKit::getHotspotCoord(const char *name,SbVec3f &coord,int mode)
{
    for (int i=0;i<hotspotName.getNum();i++){
        if (!strcmp(hotspotName[i].getString(),name)) {
            if (i<hotspotCoord.getNum()){
                switch (mode) {
                    case LOCAL_HS:
                        coord=hotspotCoord[i];
                        break;
                    case ABSOLUTE_HS:
                        coord=hotspotAbsCoord[i];
                        break;
                    case OFFSET_HS:
                        coord=hotspotOffsetCoord[i];
                        break;
                    default:
                        printf("PUPPETEER: bad hotspot query mode %d, using local mode\n",mode);
                        coord=hotspotCoord[i];
                        break;
                }
            }
            else {
                printf("PUPPETEER: no coordinates have been defined for hotspot '%s' in puppeteer '%s'\n",
                name,id.getValue().getString());
                coord[0]=0;coord[1]=0;coord[2]=0;
            }
            return true;
        }
    }
    printf("PUPPETEER: no hotspot called '%s' was found in puppeteer '%s'\n",name,id.getValue().getString());
    return false;
}

bool SoPuppeteerKit::getHotspotRot(const char *name,SbRotation &rot,int mode)
{
    for (int i=0;i<hotspotName.getNum();i++){
        if (!strcmp(hotspotName[i].getString(),name)) {
            if (i<hotspotRot.getNum()){
                switch (mode) {
                    case LOCAL_HS:
                        rot=hotspotRot[i];
                        break;
                    case ABSOLUTE_HS:
                        rot=hotspotAbsRot[i];
                        break;
                    case OFFSET_HS:
                        rot=hotspotOffsetRot[i];
                        break;
                    default:
                        printf("PUPPETEER: bad hotspot query mode %d, using local mode\n",mode);
                        rot=hotspotRot[i];
                        break;
                }
            }
            else printf("PUPPETEER: no coordinates have been defined for hotspot '%s' in puppeteer '%s'\n",
                name,id.getValue().getString());
            return true;
        }
    }
    printf("PUPPETEER: no hotspot called '%s' was found in puppeteer '%s'\n",name,id.getValue().getString());
    return false;
}

bool SoPuppeteerKit::getHotspotNum(const char *name,int &index)
{
    for (int i=0;i<hotspotName.getNum();i++){
        if (!strcmp(hotspotName[i].getString(),name)) {
            index=i;
            return true;
        }
    }
    printf("PUPPETEER: no hotspot called '%s' was found in puppeteer '%s'\n",name,id.getValue().getString());
    return false;
}

void SoPuppeteerKit::commandLoopFunction()
{
    // calculating bounding box
    if (showPuppeteer.getValue() && getActivePuppetNum()){
        SbVec3f min=bbCalculator->A[0];
        SbVec3f max=bbCalculator->B[0];
        SbVec3f bbPoint[8]={
            SbVec3f(min[0],min[1],max[2]),
            SbVec3f(max[0],min[1],max[2]),
            SbVec3f(min[0],max[1],max[2]),
            SbVec3f(max[0],max[1],max[2]),
            SbVec3f(min[0],min[1],min[2]),
            SbVec3f(max[0],min[1],min[2]),
            SbVec3f(min[0],max[1],min[2]),
            SbVec3f(max[0],max[1],min[2])
        };
        for (int i=0;i<8;i++) {
            if (boundingBoxMode.getValue()==OBB) {
                for (int j=0;j<3;j++) bbPoint[i][j]=bbPoint[i][j]*scale.getValue()[j];
                orientation.getValue().multVec(bbPoint[i],bbPoint[i]);
            }
            boundingBox.set1Value(i,bbPoint[i]+position.getValue());
        }
    }
    else for (int i=0;i<8;i++) boundingBox.set1Value(i,0,0,0);

    // calculating velocity
    long sec,microsec;
    SbTime timeNew=globalTime.getValue();
    if (timeOld==timeNew) return;
    (timeNew-timeOld).getValue(sec,microsec);
    SbVec3f posVector=position.getValue()-positionOld;
    float length=posVector.length();
    float vel=length/(((float)(sec+microsec))/1000000.0f);
    velocity.setValue(vel);
    if (length && vel) {
        posVector.normalize();
        velocityDir.setValue(posVector);
    }
    if (velocityVisOn.getValue()){
        SO_GET_ANY_PART(this,"velSwitch",SoSwitch)->whichChild=velocity.getValue() ? -3 : -1;
    }

    // TODO: calculating angular velocity + acceleration?
    // ...
    
    positionOld=position.getValue();
    timeOld=timeNew;
}

bool SoPuppeteerKit::executeCommand()
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: execute command for puppeteer %s\n",id.getValue().getString());
#endif
    
    bool allImplemented=true;
    for (int i=0;i<getActivePuppetNum();i++){
        SoNode *puppet=getActivePuppet(i);
        if (!implementCommand(puppet)) {
            bool implemented=defaultImplementation(puppet);
            if (!implemented) {
#ifdef DEBUG_ARPUPPET
                printf("PUPPETEER: command '%s' for puppet #%d of puppeteer '%s' has not been implemented\n",
                    currentType.c_str(),
                    i,
                    id.getValue().getString()
                );
#endif
                noTrigger();
            }
            else {
#ifdef DEBUG_ARPUPPET
                printf("PUPPETEER: command '%s' for puppet #%d of puppeteer '%s' has been implemented using the default implementation\n",
                    currentType.c_str(),
                    i,
                    id.getValue().getString()
                );
#endif
            }
            allImplemented&=implemented;
        }
#ifdef DEBUG_ARPUPPET
        else {
            printf("PUPPETEER: command '%s' for puppet #%d of puppeteer '%s' has been locally implemented\n",
                currentType.c_str(),
                i,
                id.getValue().getString()
            );
        }
#endif
    }

    return allImplemented;
}

void SoPuppeteerKit::idle()
{
    if (!disableIdleBehavior.getValue()){
        for (int i=0;i<getActivePuppetNum();i++){
            SoNode *puppet=getActivePuppet(i);
            idleCommand(puppet);
        }
    }
}

bool SoPuppeteerKit::defaultImplementation(SoNode *puppet)
{
    // treat the socket communication module distinctively
    if (isType(puppet,"SoSocketModule")){
        SoSocketModule *socketModule=(SoSocketModule*)puppet;
        socketModule->in.setValue(currentCommand.c_str());
    }

    // default behavior for all puppet types except the SoSocketModule
    if (isCommandType("setPosition")){
        setPosition(getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("setPositionRel")){
        SbVec3f pos=position.getValue();
        setPosition(pos[0]+getFloatParam(0),pos[1]+getFloatParam(1),pos[2]+getFloatParam(2),getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("setOrientationQ")){
        setOrientationQ(getFloatParam(0),getFloatParam(1),getFloatParam(2),getFloatParam(3),getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("setOrientationQRel")){
        SbRotation rotBase=orientation.getValue();
        SbRotation rotRel=SbRotation(getFloatParam(0),getFloatParam(1),getFloatParam(2),getFloatParam(3));
        SbRotation rotRes=rotBase*rotRel;
        float r0,r1,r2,r3;
        rotRes.getValue(r0,r1,r2,r3);
        setOrientationQ(r0,r1,r2,r3,getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("setOrientationV")){
        setOrientationV(getFloatParam(0),getFloatParam(1),getFloatParam(2),getFloatParam(3),getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("setOrientationVRel")){
        SbRotation rotBase=orientation.getValue();
        SbRotation rotRel=SbRotation(SbVec3f(getFloatParam(0),getFloatParam(1),getFloatParam(2)),getFloatParam(3));
        SbRotation rotRes=rotBase*rotRel;
        float r0,r1,r2,r3;
        rotRes.getValue(r0,r1,r2,r3);
        setOrientationQ(r0,r1,r2,r3,getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("setScale")){
        setScale(getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("turnTowards")){
        turnTowards(getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("lookAt")){
        lookAt(getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
        addDurationMonitor(getIntParam(0));
    }
    else if (isCommandType("pathMove")){
        // pathStr = ` pathSegment0 , pathSegment1 , ...  , pathSegmentN `
        // pathSegment = x y z moveDuration waitDuration
        string pathStr=getStringParam(0);
        string pathSegment;
        string targetPosString,moveDurationString,waitDurationString;
        int start=-1,end=-1;
        int seg_last,seg_lastbutone,seg_end;
        int count;
        vector<string> cmdlist;

        while (end!=pathStr.length()) {
            start=end;
            if ((end=pathStr.find(",",end+1))==pathStr.npos) end=pathStr.length();
            pathSegment=pathStr.substr(start+1,end-start-1);
            seg_last=-1;

            if ((seg_end=pathSegment.find_last_not_of(" "))==pathSegment.npos) break;
            if ((seg_end=pathSegment.rfind(" ",seg_end))==pathStr.npos) break;
            if ((seg_last=pathStr.find_first_not_of(" ",seg_last+1))==pathStr.npos) break;
            count=0;
            do {
                count++;
                seg_lastbutone=seg_last;
                if ((seg_last=pathSegment.find(" ",seg_last))==pathStr.npos) break;
                if ((seg_last=pathSegment.find_first_not_of(" ",seg_last))==pathStr.npos) break;
            } while (seg_last<seg_end);
            if (count>3) {
                targetPosString=pathSegment.substr(0,seg_lastbutone);
                moveDurationString=pathSegment.substr(seg_lastbutone,seg_last-seg_lastbutone);
                waitDurationString=pathSegment.substr(seg_last,seg_end-seg_last);
                string cmdStr1="goTo "+targetPosString+" "+moveDurationString;
                string cmdStr2="wait "+waitDurationString;
                cmdlist.push_back(cmdStr1);cmdlist.push_back(cmdStr2);
            }
        }
        if (cmdlist.size()) addCompoundCommand(&cmdlist);
        else {
            printf("PUPPETEER: the path '%s' for puppeteer '%s' could not be interpreted\n",
                pathStr.c_str(),id.getValue().getString());
            noTrigger();
        }
    }
    else if (isCommandType("goTo")){
        char cmd1[255],cmd2[255];
        sprintf(cmd1,"turnTowards %f %f %f 500",getFloatParam(0),getFloatParam(1),getFloatParam(2));
        sprintf(cmd2,"setPosition %f %f %f %d",getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
        addCompoundCommand(cmd1,cmd2,NULL);
    }
    else if (isCommandType("speak")){
        speechText.setValue(getStringParam(0));
        noTrigger();
    }
    else if (isCommandType("sound")){
        
        // time parameter used
        if (getIntParam(1)>=0){
            char cmd1[255],cmd2[255],cmd3[255];
            sprintf(cmd1,"sound `%s` %d -1",getStringParam(0),getIntParam(0));
            sprintf(cmd2,"wait %d",getIntParam(1));
            sprintf(cmd3,"shutup `sound`");
            addCompoundCommand(cmd1,cmd2,cmd3,NULL);
        }
        // time parameter ignored
        else {
            soundFileName.setValue(getStringParam(0));
            soundLoop.setValue(getIntParam(0) ? TRUE : FALSE);
            soundPlay.setValue(TRUE);
            noTrigger();
        }
    }
    else if (isCommandType("shutup")){
        if (!strlen(getStringParam(0)) || !strcmp(getStringParam(0),"speech")) stopSpeech.setValue();
        if (!strlen(getStringParam(0)) || !strcmp(getStringParam(0),"sound")) soundPlay.setValue(FALSE);
        noTrigger();
    }
    else if (isCommandType("wait")){
        addDurationMonitor(getIntParam(0));
    }
    else return false;
    return true;
}

bool SoPuppeteerKit::implementCommand(SoNode *puppet)
{
    // default implementation does nothing and returns 'false' to run the default implementation
    return false;
}

void SoPuppeteerKit::idleCommand(SoNode *puppet)
{
    // default implementation is empty
}

void SoPuppeteerKit::constructTrigger()
{
    resetTriggerList();
    
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: constructTrigger\n");
#endif
    for (int i=0;i<getPuppetNum();i++){
        SoNode *puppet=getPuppet(i);
        SoField *triggerField=getFinishedSubCommandTrigger(puppet);
        if (triggerField!=NULL) {
            SoFieldSensor *triggerSensor=new SoFieldSensor(finishedSubCommandCB,this);
            triggerSensor->attach(triggerField);
            finishedSubCommandSensorList.append(triggerSensor);
        }
        else finishedSubCommandSensorList.append(NULL);
    }
}

SoField* SoPuppeteerKit::getFinishedSubCommandTrigger(SoNode *puppet)
{
    // default implementation returns NULL
    return NULL;
}

const char* SoPuppeteerKit::getFinishedSubCommandData(SoNode *puppet)
{
    // default implementation returns NULL
    return NULL;
}

void SoPuppeteerKit::resetTriggerList()
{
    for (int i=0;i<finishedSubCommandSensorList.getLength();i++) 
        ((SoFieldSensor*)(finishedSubCommandSensorList[i]))->detach();
    removeAllListItems(&finishedSubCommandSensorList);
}

void SoPuppeteerKit::setupTransformation()
{
    positionTimer->flags.setValue(SoOneShot::HOLD_FINAL);
    positionInterpolator->input0.setValue(0,0,0);
    positionInterpolator->input1.setValue(0,0,0);
    positionInterpolator->alpha.connectFrom(&positionTimer->ramp);

    orientationInterpolator->input0.setValue(SbRotation::identity());
    orientationInterpolator->input1.setValue(SbRotation::identity());
    orientationInterpolator->alpha.connectFrom(&orientationTimer->ramp);

    SoTranslation *tPBase=SO_GET_ANY_PART(this,"transformPosBase",SoTranslation);
    SoTranslation *tPOffset=SO_GET_ANY_PART(this,"transformPosOffset",SoTranslation);
    SoRotation *tRBase=SO_GET_ANY_PART(this,"transformRotBase",SoRotation);
    SoRotation *tROffset=SO_GET_ANY_PART(this,"transformRotOffset",SoRotation);
    tPBase->ref();
    tPOffset->ref();
    tRBase->ref();
    tROffset->ref();

    tPBase->translation.connectFrom(&positionBase);
    positionOffsetRotator->input.connectFrom(&positionOffset);
    positionOffsetRotator->rotation.connectFrom(&tRBase->rotation);
    tPOffset->translation.connectFrom(&positionOffsetRotator->output);

    tRBase->rotation.connectFrom(&orientationBase);
    tROffset->rotation.connectFrom(&orientationOffset);

    orientationTimer->flags.setValue(SoOneShot::HOLD_FINAL);
    orientationInterpolator->input0.setValue(SbRotation(SbVec3f(0,0,1),0));
    
    // dragging
    SoTransform *moverDrag=SO_GET_ANY_PART(this,"dragTransform",SoTransform);
    SoDragKit *dragger=SO_GET_ANY_PART(this,"puppets",SoDragKit);
    moverDrag->translation.connectFrom(&dragger->translation);
    moverDrag->rotation.connectFrom(&dragger->rotation);
    moverDrag->ref();
    dragger->ref();

    // scaling
    SoScale *moverScale=SO_GET_ANY_PART(this,"transformScale",SoScale);
    moverScale->ref();

    scaleTimer->flags.setValue(SoOneShot::HOLD_FINAL);
    scaleInterpolator->input0.setValue(1,1,1);
    scaleInterpolator->alpha.connectFrom(&scaleTimer->ramp);
    moverScale->scaleFactor.connectFrom(&scaleSet);
    scale.connectFrom(&scaleSet);

    position.connectFrom(&transformCapture->translation);
    orientation.connectFrom(&transformCapture->rotation);

    // calculate absolute hotspot coordinates and rotations
    hotSpotRotatorAbs->input.connectFrom(&hotspotCoord);
    hotSpotRotatorAbs->rotation.connectFrom(&orientation);
    hotSpotRotatorOffset->input.connectFrom(&hotspotCoord);
    hotSpotRotatorOffset->rotation.connectFrom(&orientationOffset);
    hotspotPosCalculator->A.connectFrom(&hotSpotRotatorAbs->output);
    hotspotPosCalculator->B.connectFrom(&position);
    hotspotPosCalculator->C.connectFrom(&hotSpotRotatorOffset->output);
    hotspotPosCalculator->D.connectFrom(&positionOffset);
    hotspotPosCalculator->expression.set1Value(0,"oA=A+B");
    hotspotPosCalculator->expression.set1Value(1,"oB=C+D");
    hotspotAbsCoord.connectFrom(&hotspotPosCalculator->oA);
    hotspotOffsetCoord.connectFrom(&hotspotPosCalculator->oB);

    hotSpotRotMultAbs->rotationA.connectFrom(&hotspotRot);
    hotSpotRotMultAbs->rotationB.connectFrom(&orientation);
    hotSpotRotMultOffset->rotationA.connectFrom(&hotspotRot);
    hotSpotRotMultOffset->rotationB.connectFrom(&orientationOffset);
    hotspotAbsRot.connectFrom(&hotSpotRotMultAbs->product);
    hotspotOffsetRot.connectFrom(&hotSpotRotMultOffset->product);
}

void SoPuppeteerKit::setupBoundingBox()
{
    SoMaterial *bbm=(SoMaterial*)getAnyPart("bbMaterial",FALSE);
    SoDrawStyle *bbds=(SoDrawStyle*)getAnyPart("bbDrawStyle",FALSE);
    SoCube *bbc=SO_GET_ANY_PART(this,"bbCube",SoCube);

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

    bbRotator->input.connectFrom(&computeBoundingBox->boxCenter);
    bbRotator->rotation.connectFrom(&orientation);

    bbCalculator->A.connectFrom(&computeBoundingBox->min);
    bbCalculator->B.connectFrom(&computeBoundingBox->max);
    bbCalculator->C.connectFrom(&position);
    bbCalculator->D.connectFrom(&bbRotator->output);
    bbCalculator->expression.set1Value(0,"oa=(B[0]-A[0])*E[0]");
    bbCalculator->expression.set1Value(1,"ob=(B[1]-A[1])*E[1]");
    bbCalculator->expression.set1Value(2,"oc=(B[2]-A[2])*E[2]");
    bbCalculator->expression.set1Value(3,"oA[0]=C[0]+D[0]*E[0]");
    bbCalculator->expression.set1Value(4,"oA[1]=C[1]+D[1]*E[1]");
    bbCalculator->expression.set1Value(5,"oA[2]=C[2]+D[2]*E[2]");
    bbCalculator->ref();

    // setup bounding box cube
    bbc->width.connectFrom(&bbCalculator->oa);
    bbc->height.connectFrom(&bbCalculator->ob);
    bbc->depth.connectFrom(&bbCalculator->oc);
}

void SoPuppeteerKit::setupViewingFrustums()
{
    SoMaterial *vfm=(SoMaterial*)getAnyPart("vfMaterial",FALSE);
    SoDrawStyle *vfds=(SoDrawStyle*)getAnyPart("vfDrawStyle",FALSE);

    // if no material has been defined, define a default material
    if (vfm==NULL) {
        vfm=new SoMaterial;
        vfm->diffuseColor.setValue(0.0,0.0,1.0);
        vfm->ambientColor.setValue(0.0,0.0,1.0);
        vfm->emissiveColor.setValue(0.0,0.0,1.0);
        vfm->shininess.setValue(1.0);
        this->setPart("vfMaterial",vfm);
    }

    // if no draw style has been defined, define a default draw style
    if (vfds==NULL) {
        vfds=new SoDrawStyle;
        vfds->style.setValue(SoDrawStyle::LINES);
        this->setPart("vfDrawStyle",vfds);
    }

    SoNodeKitListPart *vfc=SO_GET_ANY_PART(this,"viewingFrustums",SoNodeKitListPart);
    if (!vfc->getNumChildren()) {
        SoViewingFrustum *defaultVF=new SoViewingFrustum();
        defaultVF->nearDistance=0.001f;
        defaultVF->farDistance=3.0f;
        defaultVF->heightAngle=30.0f/180.0f*(float)M_PI;
        vfc->addChild(defaultVF);
    }
}

void SoPuppeteerKit::setupVelocityVisualization()
{
    SoTransform *velt=SO_GET_ANY_PART(this,"velTransform",SoTransform);
    SoMaterial *velm=(SoMaterial*)getAnyPart("velMaterial",FALSE);
    SoStroke *vels=(SoStroke*)getAnyPart("velocityVis",FALSE);

    // if no material has been defined, define a default material
    if (velm==NULL) {
        velm=new SoMaterial;
        velm->diffuseColor.setValue(0.0,1.0,0.0);
        velm->emissiveColor.setValue(0.0,0.3,0.0);
        this->setPart("velMaterial",velm);
    }

    // setup transformation of the velocity vector
    velt->translation.connectFrom(&position);

    // no default draw style is defined --> solid object

    // if no scene graph has been defined for visualization, define default visualization scene graph
    if (vels==NULL){
        vels=new SoStroke;
        // set up scenegraph
        SoGroup *arrowRoot=new SoGroup;
        SoTransform *cylinderTrans=new SoTransform;
        cylinderTrans->rotation.setValue(SbVec3f(0,0,1),-1.57f);
        SoCylinder *cylinder=new SoCylinder;
        cylinder->radius=0.01f;
        cylinder->height=1.0f;
        SoTransform *coneTrans=new SoTransform;
        coneTrans->translation.setValue(0.0f,0.5f,0.0f);
        SoCone *cone=new SoCone;
        cone->bottomRadius=0.02f;
        cone->height=0.12;
        arrowRoot->addChild(cylinderTrans);
        arrowRoot->addChild(cylinder);
        arrowRoot->addChild(coneTrans);
        arrowRoot->addChild(cone);
        vels->setPart("geometry",arrowRoot);
        this->setPart("velocityVis",vels);
    }

    vels->scale.setValue(SoStroke::DIRECTION);
    SoConcatenate *conc=new SoConcatenate(SoMFVec3f::getClassTypeId());
    SoCalculator *arrowCalc=new SoCalculator;
    arrowCalc->A.connectFrom(&velocityDir);
    arrowCalc->a.connectFrom(&velocity);
    arrowCalc->b.connectFrom(&velocityScale);
    arrowCalc->expression.set1Value(0,"oA=A*a*b");
    ((SoMFVec3f*)conc->input[0])->setValue(0,0,0);
    conc->input[1]->connectFrom(&arrowCalc->oA);
    vels->point.connectFrom(conc->output);
}

void SoPuppeteerKit::finishedSubCommandCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;
    for (int i=0;i<puppeteerKit->finishedSubCommandSensorList.getLength();i++){
        if (((SoSensor*)(puppeteerKit->finishedSubCommandSensorList[i]))==sensor) {
            const char *cmd=puppeteerKit->getFinishedSubCommandData(puppeteerKit->getPuppet(i));
            puppeteerKit->commandTriggered(cmd);
#ifdef DEBUG_ARPUPPET
            printf("PUPPETEER: finishedSubCommandCB '%s'\n",cmd);
#endif
            break;
        }
    }
}

void SoPuppeteerKit::whichPuppetsCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    puppeteerKit->boundingBoxModeCB(puppeteerKit,NULL);
    puppeteerKit->puppetSwitch->whichChildren=puppeteerKit->whichPuppets;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->whichPuppetsChanged(puppet);
    }
}

void SoPuppeteerKit::whichAttributesCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    SO_GET_ANY_PART(puppeteerKit,"attributes",SoMultiSwitch)->whichChildren=puppeteerKit->whichAttributes;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->whichAttributesChanged(puppet);
    }
}

void SoPuppeteerKit::controlModeCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: positionControlMode changed to %d\n",puppeteerKit->controlMode.getValue());
#endif

    int i;

    // check whether control mode is valid
    for (i=0;i<3;i++){
        if (puppeteerKit->controlMode.getNum()<i+1 || puppeteerKit->controlMode[i] & ControlMode::INVALID){
            puppeteerKit->controlMode.set1Value(i,SCRIPT);
        }
    }

    SbVec3f oldPos=puppeteerKit->position.getValue();
    SbRotation oldRot=puppeteerKit->orientation.getValue();
    SbVec3f oldScale=puppeteerKit->scale.getValue();

    // station tracking --> translation
    SoEngineOutput *engineOutput;
    if (puppeteerKit->controlMode[0] & ControlMode::STATION){
        puppeteerKit->positionBase.connectFrom(&puppeteerKit->trackerPosition->translation);
        SoSFVec3f trans;
        trans.connectFrom(&puppeteerKit->trackerPosition->translation);
        if (!(puppeteerKit->controlMode[0] & ControlMode::EXTERNAL)) puppeteerKit->positionOffset.setValue(puppeteerKit->positionOffset.getValue()-trans.getValue());
    }
    else {
        if (puppeteerKit->positionBase.getConnectedEngine(engineOutput) && engineOutput==&(puppeteerKit->trackerPosition->translation)){
            puppeteerKit->positionBase.disconnect();
            if (!(puppeteerKit->controlMode[0] & ControlMode::EXTERNAL)) puppeteerKit->positionOffset.setValue(puppeteerKit->positionOffset.getValue()+oldPos);
        }
    }

    // station tracking --> rotation
    if (puppeteerKit->controlMode[1] & ControlMode::STATION){
        puppeteerKit->orientationBase.connectFrom(&puppeteerKit->trackerOrientation->rotation);
        SoSFRotation rot;
        rot.connectFrom(&puppeteerKit->trackerPosition->rotation);
        if (!(puppeteerKit->controlMode[1] & ControlMode::EXTERNAL)) puppeteerKit->orientationOffset.setValue(puppeteerKit->orientationOffset.getValue()*rot.getValue().inverse());
    }
    else {
        if (puppeteerKit->orientationBase.getConnectedEngine(engineOutput) && engineOutput==&(puppeteerKit->trackerPosition->rotation)){
            puppeteerKit->orientationBase.disconnect();
            if (!(puppeteerKit->controlMode[1] & ControlMode::EXTERNAL)) puppeteerKit->orientationOffset.setValue(puppeteerKit->orientationOffset.getValue()*oldRot);
        }
    }

    // scaling
    if (puppeteerKit->controlMode[2] & ControlMode::EXTERNAL){
        if (puppeteerKit->scaleSet.getConnectedEngine(engineOutput) && 
            engineOutput==&(puppeteerKit->scaleInterpolator->output)) {
            puppeteerKit->scaleSet.disconnect();
        }
    }
    else {
        puppeteerKit->scaleSet.connectFrom(&puppeteerKit->scaleInterpolator->output);
        puppeteerKit->scaleInterpolator->input0.setValue(oldScale);
        puppeteerKit->scaleInterpolator->input1.setValue(oldScale);
        puppeteerKit->scaleTimer->duration=puppeteerKit->getTimeValue(0);
        puppeteerKit->scaleTimer->trigger.setValue();
    }

    // dragging
    SoDragKit *dragger=SO_GET_ANY_PART(puppeteerKit,"puppets",SoDragKit);
    if ((puppeteerKit->controlMode[0] & ControlMode::DRAG)!=dragger->translationOn.getValue()){
        if (!(puppeteerKit->controlMode[0] & ControlMode::DRAG)){
            puppeteerKit->positionInterpolator->input0.setValue(puppeteerKit->positionOffset.getValue()+dragger->translation.getValue());
            puppeteerKit->positionInterpolator->input1.setValue(puppeteerKit->positionOffset.getValue()+dragger->translation.getValue());
        }
        dragger->translationOn.setValue(puppeteerKit->controlMode[0] & ControlMode::DRAG);
    }
    if ((puppeteerKit->controlMode[1] & ControlMode::DRAG)!=dragger->rotationOn.getValue()){
        dragger->rotationOn.setValue(puppeteerKit->controlMode[1] & ControlMode::DRAG);
        if (!(puppeteerKit->controlMode[1] & ControlMode::DRAG)){
            puppeteerKit->orientationInterpolator->input0.setValue(dragger->rotation.getValue()*puppeteerKit->orientationOffset.getValue());
            puppeteerKit->orientationInterpolator->input1.setValue(dragger->rotation.getValue()*puppeteerKit->orientationOffset.getValue());
        }
    }

    // scripting mode (if external, positionOffset and orientationOffset will be connected to external control fields)
    if (puppeteerKit->controlMode[0] & ControlMode::EXTERNAL){
        if (puppeteerKit->positionOffset.getConnectedEngine(engineOutput) && engineOutput==&(puppeteerKit->positionInterpolator->output)) {
            puppeteerKit->positionOffset.disconnect();
        }
    }
    else {        
        puppeteerKit->positionOffset.connectFrom(&puppeteerKit->positionInterpolator->output);
    }
    if (puppeteerKit->controlMode[1] & ControlMode::EXTERNAL){
        if (puppeteerKit->orientationOffset.getConnectedEngine(engineOutput) && engineOutput==&(puppeteerKit->orientationInterpolator->output)){
            puppeteerKit->orientationOffset.disconnect();
        }
    }
    else {
        puppeteerKit->orientationOffset.connectFrom(&puppeteerKit->orientationInterpolator->output);
    }

    for (i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->controlModeChanged(puppet);
    }
}

void SoPuppeteerKit::showPuppeteerCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;
    SoSwitch *rootSwitch=SO_GET_ANY_PART(puppeteerKit,"rootSwitch",SoSwitch);

    puppeteerKit->boundingBoxModeCB(puppeteerKit,NULL);
    rootSwitch->whichChild=puppeteerKit->showPuppeteer.getValue() ? -3 : -1;

    for (int i=0;i<puppeteerKit->getActivePuppetNum();i++){
        SoNode *puppet=puppeteerKit->getActivePuppet(i);
        puppeteerKit->showPuppeteerChanged(puppet);
    }
}

void SoPuppeteerKit::showAttributesCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    SoMultiSwitch *attributeSwitch=SO_GET_ANY_PART(puppeteerKit,"attributes",SoMultiSwitch);
    if (puppeteerKit->showAttributes.getValue()) {
        attributeSwitch->whichChildren=puppeteerKit->whichAttributes;
    }
    else attributeSwitch->whichChildren=-1;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->showAttributesChanged(puppet);
    }
}

void SoPuppeteerKit::positionCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->positionChanged(puppet);
    }
}

void SoPuppeteerKit::orientationCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->orientationChanged(puppet);
    }
}

void SoPuppeteerKit::scaleCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->scaleChanged(puppet);
    }
}

void SoPuppeteerKit::snapToStationCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    if (puppeteerKit->controlMode[0] & ControlMode::STATION){
        puppeteerKit->positionOffset.setValue(0,0,0);
    }
    if (puppeteerKit->controlMode[1] & ControlMode::STATION){
        puppeteerKit->orientationOffset.setValue(SbRotation::identity());
    }
}

void SoPuppeteerKit::stationCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    // tracking
    if (puppeteerKit->station.getNum()==0){
        puppeteerKit->trackerPosition->station=-1;
        puppeteerKit->trackerOrientation->station=-1;
    }
    else if (puppeteerKit->station.getNum()==1){
        puppeteerKit->trackerPosition->station=puppeteerKit->station[0];
        puppeteerKit->trackerOrientation->station=puppeteerKit->station[0];
    }
    else { // >1
        puppeteerKit->trackerPosition->station=puppeteerKit->station[0];
        puppeteerKit->trackerOrientation->station=puppeteerKit->station[1];
    }

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->stationChanged(puppet);
    }
}

void SoPuppeteerKit::draggingCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->draggingChanged(puppet);
    }
}

void SoPuppeteerKit::boundingBoxModeCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    SoTransform *bbt=SO_GET_ANY_PART(puppeteerKit,"bbTransform",SoTransform);
    bbt->ref();

    if (!puppeteerKit->showPuppeteer.getValue() || !puppeteerKit->getActivePuppetNum() || 
            puppeteerKit->boundingBoxMode.getValue()==BoundingBoxMode::NONE) {
        //puppeteerKit->computeBoundingBox->node.setValue(new SoSeparator);
        puppeteerKit->computeBoundingBox->node.setValue(NULL);
        puppeteerKit->computeBoundingBox->path.setValue(NULL);
        puppeteerKit->boundingBox.disconnect();
    }
    else {
        if (puppeteerKit->boundingBoxMode.getValue()==BoundingBoxMode::AABB){
            puppeteerKit->computeBoundingBox->node.setValue(new SoSeparator);
            SoFullPath *fullPath=(SoFullPath*)(puppeteerKit->createPathToPart("puppets",FALSE));
            fullPath->ref();
            puppeteerKit->computeBoundingBox->path.setValue(fullPath);
            // set calculator component
            puppeteerKit->bbCalculator->E.disconnect();
            puppeteerKit->bbCalculator->E.setValue(1,1,1);
            // setup transformation of the bounding box
            bbt->translation.connectFrom(&puppeteerKit->computeBoundingBox->boxCenter);
            bbt->rotation.disconnect();
        }
        else if (puppeteerKit->boundingBoxMode.getValue()==BoundingBoxMode::OBB) {
            SoSeparator *sep=(SoSeparator*)(SO_GET_ANY_PART(puppeteerKit,"puppets",SoDragKit)->getPart("content",FALSE));
            puppeteerKit->computeBoundingBox->node.setValue(sep->getChild(0));
            puppeteerKit->computeBoundingBox->path.setValue(NULL);
            // set calculator component
            puppeteerKit->bbCalculator->E.connectFrom(&puppeteerKit->scale);
            // setup transformation of the bounding box    
            bbt->translation.connectFrom(&puppeteerKit->bbCalculator->oA);
            bbt->rotation.connectFrom(&puppeteerKit->orientation);
        }
    }

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->boundingBoxOnModeChanged(puppet);
    }
}

void SoPuppeteerKit::boundingBoxOnCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;
    SoSwitch *bbs=SO_GET_ANY_PART(puppeteerKit,"bbSwitch",SoSwitch);

    if (puppeteerKit->boundingBoxOn.getValue() && bbs->getNumChildren()) bbs->whichChild=-3;
    else bbs->whichChild=-1;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->boundingBoxOnChanged(puppet);
    }
}

void SoPuppeteerKit::viewingFrustumOnCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;
    SoSwitch *vfs=SO_GET_ANY_PART(puppeteerKit,"vfSwitch",SoSwitch);

    if (puppeteerKit->viewingFrustumOn.getValue() && vfs->getNumChildren()) vfs->whichChild=-3;
    else vfs->whichChild=-1;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->viewingFrustumOnChanged(puppet);
    }
}

void SoPuppeteerKit::velocityVisOnCB(void *data,SoSensor *sensor)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;
    SoSwitch *vels=SO_GET_ANY_PART(puppeteerKit,"velSwitch",SoSwitch);

    if (puppeteerKit->velocityVisOn.getValue() && vels->getNumChildren()) vels->whichChild=-3;
    else vels->whichChild=-1;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->velocityVisOnChanged(puppet);
    }
}

void SoPuppeteerKit::pressCB(void* data, SoDragKit* dragKit)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    if (puppeteerKit->highlightDragOn.getValue()) {
        SO_GET_ANY_PART(puppeteerKit,"bbSwitch",SoSwitch)->whichChild=-3;
    }

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->pressed(puppet);
    }
}

void SoPuppeteerKit::releaseCB(void* data, SoDragKit* dragKit)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    if (!puppeteerKit->boundingBoxOn.getValue()) {
        SO_GET_ANY_PART(puppeteerKit,"bbSwitch",SoSwitch)->whichChild=-1;
    }

    // reset dragkit
    puppeteerKit->orientationOffset.setValue(puppeteerKit->orientation.getValue()*puppeteerKit->orientationBase.getValue().inverse());
    SbVec3f rotPos;
    puppeteerKit->orientationBase.getValue().inverse().multVec(puppeteerKit->position.getValue()-puppeteerKit->positionBase.getValue(),rotPos);
    puppeteerKit->positionOffset.setValue(rotPos);
    dragKit->translation.setValue(0,0,0);
    dragKit->rotation.setValue(SbRotation::identity());

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->released(puppet);
    }
}

void SoPuppeteerKit::moveCB(void* data, SoDragKit* dragKit)
{
    SoPuppeteerKit *puppeteerKit=(SoPuppeteerKit*)data;

    for (int i=0;i<puppeteerKit->getPuppetNum();i++){
        SoNode *puppet=puppeteerKit->getPuppet(i);
        puppeteerKit->moved(puppet);
    }
}

// virtual functions to override in the derived puppeteer classes

void SoPuppeteerKit::whichPuppetsChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: whichPuppets changed\n");
#endif
}

void SoPuppeteerKit::whichAttributesChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: whichAttributes changed\n");
#endif
}

void SoPuppeteerKit::controlModeChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: controlMode changed\n");
#endif
}

void SoPuppeteerKit::showPuppeteerChanged(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: showPuppeteer changed to %s\n", showPuppeteer.getValue() ? "true" : "false");
#endif
}

void SoPuppeteerKit::showAttributesChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: showAttributes changed to %s\n", showAttributes.getValue() ? "true" : "false");
#endif
}

void SoPuppeteerKit::positionChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    //printf("PUPPETEER: position changed\n");
#endif
}

void SoPuppeteerKit::orientationChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    //printf("PUPPETEER: orientation changed\n");
#endif
}

void SoPuppeteerKit::scaleChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    //printf("PUPPETEER: scale changed\n");
#endif
}

void SoPuppeteerKit::snapToStationChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: snapToStation changed\n");
#endif
}

void SoPuppeteerKit::stationChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: station changed\n");
#endif
}

void SoPuppeteerKit::draggingChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: dragging changed\n");
#endif
}

void SoPuppeteerKit::boundingBoxOnChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: boundingBoxOn changed\n");
#endif
}

void SoPuppeteerKit::boundingBoxOnModeChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: boundingBoxMode changed\n");
#endif
}

void SoPuppeteerKit::viewingFrustumOnChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: viewingFrustumOn changed\n");
#endif
}

void SoPuppeteerKit::velocityVisOnChanged(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: velocityVisOn changed\n");
#endif
}

void SoPuppeteerKit::pressed(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: pressed\n");
#endif
}

void SoPuppeteerKit::released(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: released\n");
#endif
}

void SoPuppeteerKit::moved(SoNode *node)
{
#ifdef DEBUG_ARPUPPET
    //printf("PUPPETEER: moved\n");
#endif
}

void SoPuppeteerKit::setPosition(float x,float y,float z,int duration)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: setPosition %f %f %f %d\n",x,y,z,duration);
#endif
    positionInterpolator->input0.setValue(positionOffset.getValue());
    positionInterpolator->input1.setValue(x,y,z);
    positionTimer->duration=getTimeValue(duration);
    positionTimer->trigger.setValue();
}

void SoPuppeteerKit::setOrientationQ(float qx,float qy,float qz,float w,int duration)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: setOrientation quaternion %f %f %f %f %d\n",qx,qy,qz,w,duration);
#endif
    orientationInterpolator->input0.setValue(orientationOffset.getValue());
    orientationInterpolator->input1.setValue(qx,qy,qz,w);
    orientationTimer->duration=getTimeValue(duration);
    orientationTimer->trigger.setValue();
}

void SoPuppeteerKit::setOrientationV(float vx,float vy,float vz,float a,int duration)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: setOrientation vector + angle (%f %f %f) %f %d\n",vx,vy,vz,a,duration);
#endif
    SbRotation rot(SbVec3f(vx,vy,vz),a);
    float qx,qy,qz,w;
    rot.getValue(qx,qy,qz,w);
    setOrientationQ(qx,qy,qz,w,duration);
}

void SoPuppeteerKit::setScale(float x,float y,float z,int duration)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: setScale %f %f %f %d\n",x,y,z,duration);
#endif
    scaleInterpolator->input0.setValue(scale.getValue());
    scaleInterpolator->input1.setValue(x,y,z);
    scaleTimer->duration=getTimeValue(duration);
    scaleTimer->trigger.setValue();
}

void SoPuppeteerKit::turnTowards(float x,float y,float z,int duration)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: turnTowards %f %f %f %d\n",x,y,z,duration);
#endif
    SbVec3f pos=positionOffset.getValue();
    SbRotation rot=orientationOffset.getValue();
    SbVec3f transIn;
    rot.inverse().multVec(SbVec3f(x,y,z)-pos,transIn);

    SbVec3f rotAxis(0,1,0);
    SbVec3f zero(0,0,1);
    float angle;
    SbVec3f transInProjected=transIn-(rotAxis.dot(transIn)/rotAxis.dot(rotAxis))*rotAxis;
    SbVec3f zeroProjected=zero-(rotAxis.dot(zero)/rotAxis.dot(rotAxis))*rotAxis;
    float lengthSquared=zeroProjected.length()*transInProjected.length();
    if (lengthSquared>0.0) angle=(float)acos(zeroProjected.dot(transInProjected)/lengthSquared);
    else angle=0;

    if (transInProjected.cross(zeroProjected).dot(rotAxis)>0) angle=(float)(6.28318530718-angle);

    orientationInterpolator->input0.setValue(orientationOffset.getValue());
    orientationInterpolator->input1.setValue(rot*SbRotation(rotAxis, angle));
    orientationTimer->duration=getTimeValue(duration);
    orientationTimer->trigger.setValue();
}

void SoPuppeteerKit::lookAt(float x,float y,float z,int duration)
{
#ifdef DEBUG_ARPUPPET
    printf("PUPPETEER: lookAt %f %f %f %d\n",x,y,z,duration);
#endif
    orientationInterpolator->input0.setValue(orientationOffset.getValue());
    orientationInterpolator->input1.setValue(SbRotation(SbVec3f(0.0f, 0.0f, 1.0f),SbVec3f(x,y,z)-position.getValue()));
    orientationTimer->duration=getTimeValue(duration);
    orientationTimer->trigger.setValue();
}
