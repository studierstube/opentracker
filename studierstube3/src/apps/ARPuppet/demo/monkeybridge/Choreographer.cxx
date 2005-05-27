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
 * $Id: Choreographer.cxx 4209 2005-05-19 09:06:39Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "Inventor/nodes/SoSeparator.h"
#include <Inventor/actions/SoSearchAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// own classes
#include "SceneManager.h"
#include "Character.h"
#include "Choreographer.h"

const SbString Choreographer::characterIDString("ch");

SO_KIT_SOURCE(Choreographer);

void Choreographer::initClass()
{
    SO_KIT_INIT_CLASS(Choreographer, SoChoreographerKit, "SoChoreographerKit");
}

Choreographer::Choreographer()
{
    SO_KIT_CONSTRUCTOR(Choreographer);

    SO_KIT_ADD_FIELD(init, ());
    SO_KIT_ADD_FIELD(characterNum,(0));
    SO_KIT_ADD_FIELD(fallenDown, (SceneManager::INVALID));
    // motion parameters
    SO_KIT_ADD_FIELD(heightThreshold,(0.0f));
    SO_KIT_ADD_FIELD(tileGapThreshold,(0.0f));
    SO_KIT_ADD_FIELD(speed, (0));
    SO_KIT_ADD_FIELD(globalScale, (1.0f));

    SO_KIT_INIT_INSTANCE();

    // define command set
    commandString="goTo %f %f %f";

    // set asynchronous mode
    synchronize=FALSE;

    // create field sensors
    initSensor=new SoFieldSensor(initCB,this);

    setUpConnections(TRUE,TRUE);
}

Choreographer::~Choreographer()
{
    delete initSensor;
}

SbBool Choreographer::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        // attach field sensors
        initSensor->attach(&init);

        // search for SoPuppeteerKit nodes in the scenegraph attached to "puppeteers" 
        SoSeparator *puppeteerRoot=SO_GET_ANY_PART(this,"puppeteers",SoSeparator);
        puppeteerRoot->ref();
        SoSearchAction searchAction;
        searchAction.setInterest(SoSearchAction::ALL);
        searchAction.setSearchingAll(TRUE); // search every node in the scene graph regardless of switch settings   
        searchAction.setType(Character::getClassTypeId(),TRUE);
        searchAction.apply(puppeteerRoot);
        SoPathList pathList=searchAction.getPaths();
        characterNum=pathList.getLength();
        for (int i=0;i<pathList.getLength();i++){
            Character *character=(Character*)(((SoFullPath*)(pathList[i]))->getTail());
            SbString name=characterIDString;
            name+=SbString(i);
            character->id=name;
        }
    }
    else {
        // detach field sensors
        initSensor->detach();
    }
    
    // set up connections in parent class SoPuppeteerKit
    SoChoreographerKit::setUpConnections(onOff,doItAlways);

    return !(connectionsSetUp = onOff);
}

void Choreographer::initCB(void *data,SoSensor *sensor)
{
    Choreographer *ch=(Choreographer*)data;
    // init character animation
    for (int i=0;i<ch->characterNum.getValue();i++){
        // init "character fallen down" parameter
        ch->fallenDown=SceneManager::INVALID;
        // init animation command
        char strCmd[255];
        sprintf(strCmd,"ch%d: state `idle`",i,characterIDString.getString());
        ch->command.set1Value(i,strCmd);
    }
}

bool Choreographer::implementCommand()
{
    if (SoChoreographerKit::implementCommand()) return true;
    return false;
}

bool Choreographer::choreograph()
{
    // get first target (discard others) and retreive it's ID
    int userID;
    if (!getUserIdFromString(targetNameList[0]->getString(),userID)) return false;

    char str[255];
    // choreograph movement based on the position difference between start and target
    if (isCommandType("goTo")){
        SbVec3f currentPos=getPuppeteer(targetNameList[0]->getString())->positionOffset.getValue();
        SbVec3f nextPos=SbVec3f(getFloatParam(0),getFloatParam(1),getFloatParam(2));
        SbVec3f offsetPos=(nextPos-currentPos);
        float walkDistance=sqrt(offsetPos[0]*offsetPos[0]+offsetPos[2]*offsetPos[2]);
        float heightDifference=fabs(offsetPos[1]);

        printf("CHOREOGRAPHER: choreograph goTo command: walk distance = %f, height difference = %f\n",walkDistance,heightDifference);

        // choreograph the goTo animation command based on the distance between the current and target position
        // 1.) no height difference and we are on a tile, not between tiles --> walk straight
        if (heightDifference<0.00001f*globalScale.getValue()){
            printf("CHOREOGRAPHER: character %d: no height difference --> walk along straight path\n",userID);
            sprintf(str," %d",walkDistance<=tileGapThreshold.getValue()*globalScale.getValue() ? getSpeed(WALK)/2 : getSpeed(WALK));
            currentCommand+=str;
        }
        // 2.) 1 level of height difference: a) jump over if between tiles, b) walk up/down if on tile
        else if (heightDifference<=heightThreshold.getValue()*globalScale.getValue()){
            if (walkDistance<=tileGapThreshold.getValue()*globalScale.getValue() || walkDistance>0.070f*globalScale.getValue()){
                printf("CHOREOGRAPHER: character %d: 1 level of height difference, we are between tiles --> jump up\n",userID);
                sprintf(str,"jump_up %f %f %f %d",nextPos[0],nextPos[1],nextPos[2],getSpeed(JUMP_UP));
                currentCommand=str;
            }
            else {
                printf("CHOREOGRAPHER: character %d: 1 level of height difference, we are on a tile --> walk up/down\n",userID);
                if (offsetPos[1]>0) sprintf(str," %d `walk_up`",getSpeed(WALK_UP));
                else sprintf(str," %d `walk_down`",getSpeed(WALK_DOWN));
                currentCommand+=str;
            }
        }
        // 3.) More than 1 level of height difference --> too big, cannot jump, fall into water and die
        else {
            printf("CHOREOGRAPHER: character %d: 2 levels of height difference, too big --> fall down into water\n",userID);
            // set die parameter and let the director trigger the 'fallen down' animation
            fallenDown=userID;
            noTrigger();
            return true; // command handling finished here, return true
        }
    }
    else if (isCommandType("hesitate")){
        sprintf(str,"hesitate %d %d",getSpeed(LOOKAROUND),getSpeed(DUNNO));
        currentCommand=str;
    }
    else if (isCommandType("win")){
        sprintf(str,"win %d",getSpeed(WIN));
        currentCommand=str;
    }
    else if (isCommandType("lose")){
        sprintf(str,"lose %d",getSpeed(LOSE));
        currentCommand=str;
    }
    else if (isCommandType("fall_water")){
        sprintf(str,"fall_water %d",getSpeed(FALL_WATER));
        currentCommand=str;
    }

    return false;
}

bool Choreographer::getUserIdFromString(const char *cmd,int &id)
{
    std::string search=cmd;
    int posPlayer;
    if ((posPlayer=search.find(characterIDString.getString()))!=search.npos){
        int userID;
        if (sscanf(search.substr(characterIDString.getLength(),1).c_str(),"%d",&userID)) {
            id=userID;
            return true;
        }
    }
    id=-1;
    return false;
}

int Choreographer::getSpeed(int animationID)
{
    if (animationID>=0 && animationID<speed.getNum()) return speed[animationID];
    else { 
        printf("CHOREOGRAPHER: no speed defined for motion#%d\n",animationID);
        return 0;
    }
}