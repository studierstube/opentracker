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
/* Character.cxx contains the implementation of class Character
 *
 * @author Istvan Barakonyi
 *
 * $Id: Character.cxx 3792 2004-09-29 22:39:35Z bara $
 * @file
 */
/* ======================================================================== */

// own includes
#include "Character.h"

// Inventor classes
#include "Inventor/actions/SoSearchAction.h"

SO_KIT_SOURCE(Character);

void Character::initClass()
{
    SO_KIT_INIT_CLASS(Character, SoCal3DPuppeteer, "SoCal3DPuppeteer");
}

Character::Character()
{
    SO_KIT_CONSTRUCTOR(Character);

    SO_KIT_ADD_FIELD(hotspotBoneID,(0));
    SO_KIT_ADD_FIELD(hsOffset,(0.0f,0.0f,0.0f));

    SO_KIT_INIT_INSTANCE();

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
         pathMove %s %s(walk) \
         goTo %f %f %f %d(0) %s(walk) \
         speak %s \
         sound %s %d(0) %d(-1) \
         shutup %s \
         wait %d \
         pointAt %f %f %f %s(front) %s(point) %d(0) \
         state %s %d(0) \
         play %s %d(0) \
         blend %s %f \
         place %s %d %s %d(2) \
         moveBack %f %f %f %d \
         explain %f %f %f";

    idleMode=DEFAULT;

    setUpConnections(TRUE,TRUE);
}

Character::~Character()
{
}

SbBool Character::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        // search for an SoCal3DPuppet node
        SoSearchAction searchAction;
        searchAction.setInterest(SoSearchAction::FIRST);
        searchAction.setSearchingAll(TRUE); // search every node in the scene graph regardless of switch settings
        SoType type=SoType::fromName("SoCal3DPuppet");
        searchAction.setType(type,TRUE);
        this->ref();
        searchAction.apply(this);
        SoPath *path=searchAction.getPath();
        if (path!=NULL) cal3dPuppet=(SoCal3DPuppet*)(((SoFullPath*)path)->getTail());
        else cal3dPuppet=NULL;
        if (cal3dPuppet!=NULL) skeleton=cal3dPuppet->getCalModel()->getSkeleton();
        else printf("CHARACTER: warning: couldn't find Cal3DPuppet model\n");
    }
    else {
    }
    
    // set up connections in parent class SoCal3DPuppeteer
    SoCal3DPuppeteer::setUpConnections(onOff,doItAlways);

    return !(connectionsSetUp = onOff);
}

void Character::commandLoopFunction()
{
    SoCal3DPuppeteer::commandLoopFunction();

    if (cal3dPuppet==NULL || skeleton==NULL) return;

    cal3dPuppet->getCalModel()->getMixer()->updateSkeleton();
    for (int i=0;i<hotspotBoneID.getNum();i++){
        SbVec3f offset=(i<hsOffset.getNum()) ? hsOffset[i] : (hsOffset.getNum() ? hsOffset[hsOffset.getNum()-1] : SbVec3f(0,0,0));
        if (hotspotBoneID[i]<skeleton->getVectorBone().size()){
            CalVector vec=skeleton->getBone(hotspotBoneID[i])->getTranslationAbsolute();
            hotspotCoord.set1Value(i,SbVec3f(vec.x/1000.0f,vec.z/1000.0f,-vec.y/1000.0f)+offset);
        }
        else hotspotCoord.set1Value(i,offset);
    }
}

bool Character::implementCommand(SoNode *puppet)
{
    if (isType(puppet,"SoCal3DPuppet")){
        SoCal3DPuppet *cal3dPuppet=(SoCal3DPuppet*)foundNode;

        if (isCommandType("goTo")){
            if (SbVec3f(getFloatParam(0),getFloatParam(1),getFloatParam(2))==positionOffset.getValue()) noTrigger();
            else {
                char cmd0[255],cmd1[255],cmd2[255];
                sprintf(cmd0,"state `%s`",getStringParam(0));
                sprintf(cmd1,"turnTowards %f %f %f 750",getFloatParam(0),getFloatParam(1),getFloatParam(2));
                sprintf(cmd2,"setPosition %f %f %f %d",getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
                addCompoundCommand(cmd0,cmd1,cmd2,NULL);
            }
        }
        else if (isCommandType("play")){
            if (!strcmp(getStringParam(0),"introduce_1")) idleMode=TILE_IN_1HAND;
            else if (!strcmp(getStringParam(0),"introduce_2")) idleMode=TILE_IN_2HANDS;
            else idleMode=DEFAULT;
            SoCal3DPuppeteer::implementCommand(puppet);
            // we want the SoCal3DPuppeteer component to process the animation part of the command, so don't do anything
        }
        else if (isCommandType("place")){
            char cmd1[255],cmd2[255],cmd3[255];
            sprintf(cmd1,"play `introduce_%d` 1000",getIntParam(0));
            sprintf(cmd2,"pathMove `%s` `walk_carry_%d`",getStringParam(0),getIntParam(0));
            sprintf(cmd3,"play `mount_side_%da` 1000",getIntParam(0)); // TODO: play command parameter should be dependant on getStringParam(0)!!!
            switch (getIntParam(1)){
                case 0: // INTRO_ONLY
                    addCompoundCommand(cmd1,NULL);
                    break;
                case 1: // PLACE_ONLY
                    addCompoundCommand(cmd2,cmd3,NULL);
                    break;
                case 2: // INTRO_AND_PLACE
                    addCompoundCommand(cmd1,cmd2,cmd3,NULL);
                    break;
            }
        }
        else if (isCommandType("moveBack")){
            char cmd1[255];
            sprintf(cmd1,"goTo %f %f %f %d",getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
            addCompoundCommand(cmd1,NULL);
        }
        else if (isCommandType("explain")){
            char cmd1[255];
            sprintf(cmd1,"pointAt %f %f %f `front` `point_forward` 1000",getFloatParam(0),getFloatParam(1),getFloatParam(2));
            addCompoundCommand(cmd1,NULL);
        }
        else if (isCommandType("pointAt")){
            char str1[255],str2[255],str3[255];
            SbVec3f src=SbVec3f(getFloatParam(0),getFloatParam(1),getFloatParam(2))-position.getValue();
            SbVec3f dst;
            SbRotation rot;
            if (!strcmp(getStringParam(0),"left")) {
                rot=SbRotation(SbVec3f(0,1,0),-1.57);
                rot.multVec(src,dst);
            }
            else if (!strcmp(getStringParam(0),"right")) {
                rot=SbRotation(SbVec3f(0,1,0),1.57);
                rot.multVec(src,dst);
            }
            else if (!strcmp(getStringParam(0),"back")) {
                rot=SbRotation(SbVec3f(0,1,0),3.14);
                rot.multVec(src,dst);
            }
            else dst=src;
            orientationBase.getValue().inverse().multVec(dst,dst);

            sprintf(str1,"turnTowards %f %f %f 500",dst[0],dst[1],dst[2]);
            sprintf(str2,"state `%s`",getStringParam(1));
            sprintf(str3,"wait %d",getIntParam(0));
            addCompoundCommand(str1,str2,str3,NULL);
        }
        // if not implemented here, check parent class
        else return SoCal3DPuppeteer::implementCommand(puppet);
        return true;
    }
    return false;
}

void Character::idleCommand(SoNode *puppet)
{
    if (!idleEntered) {
        switch (idleMode){
            case DEFAULT:
                appendCommand("state `idle`",NULL);
                break;
            case TILE_IN_1HAND:
                appendCommand("state `hold_1`",NULL);
                break;
            case TILE_IN_2HANDS:
                appendCommand("state `hold_2`",NULL);
                break;
        }
    }
}
