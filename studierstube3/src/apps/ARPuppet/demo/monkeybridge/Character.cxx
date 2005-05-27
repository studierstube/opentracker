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
 * $Id: Character.cxx 4209 2005-05-19 09:06:39Z bara $
 * @file
 */
/* ======================================================================== */

// time function for random seed generation
#include <time.h>

// own includes
#include "Character.h"

SO_KIT_SOURCE(Character);

void Character::initClass()
{
    SO_KIT_INIT_CLASS(Character, SoCal3DPuppeteer, "SoCal3DPuppeteer");
}

Character::Character()
{
    SO_KIT_CONSTRUCTOR(Character);

    SO_KIT_ADD_FIELD(globalScale, (1.0f));

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
         pointAt %f %f %f %s %d(0) \
         state %s %d(0) \
         play %s %d(0) \
         blend %s %f \
         hesitate %d(0) %d(0)\
         win %d(0) \
         lose %d(0) \
         fall_board %f %f %f %d(0) \
         fall_water %d(0) \
         jump_up %f %f %f %d(0) \
         jump_down %f %f %f %d(0)";

    // seed for random number generator
    srand((unsigned)time(NULL));

    setUpConnections(TRUE,TRUE);
}

Character::~Character()
{
}

bool Character::implementCommand(SoNode *puppet)
{
    if (isType(puppet,"SoCal3DPuppet")){
        SoCal3DPuppet *cal3dPuppet=(SoCal3DPuppet*)foundNode;

        if (isCommandType("goTo")){
            char cmd1[255],cmd2[255],cmd3[255],cmd4[255];
            sprintf(cmd1,"turnTowards %f %f %f 500",getFloatParam(0),getFloatParam(1),getFloatParam(2));
            sprintf(cmd2,"state `%s`",getStringParam(0));
            int soundCode;
            if (!strcmp(getStringParam(0),"walk_up")) soundCode=WALK_UP_SOUND; // sound for walking upwards
            else soundCode=WALK_SOUND; // normal walking or walking down
            sprintf(cmd3,"sound `%d`",soundCode);
            sprintf(cmd4,"setPosition %f %f %f %d",getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
            addCompoundCommand(cmd1,cmd2,cmd3,cmd4,NULL);
        }
        else if (isCommandType("hesitate")){
            char cmd1[255],cmd2[255],cmd3[255];
            sprintf(cmd1,"sound `%d`",DUNNO_SOUND);
            sprintf(cmd2,"play `lookaround` %d",getIntParam(0));
            sprintf(cmd3,"state `dunno` %d",getIntParam(1));
            addCompoundCommand(cmd1,cmd2,cmd3,NULL);
        }
        else if (isCommandType("win")){
            char cmd1[255],cmd2[255];
            sprintf(cmd1,"sound `%d`",WIN_SOUND);
            sprintf(cmd2,"state `win` %d",getIntParam(0));
            addCompoundCommand(cmd1,cmd2,NULL);
        }
        else if (isCommandType("lose")){
            char cmd1[255],cmd2[255];
            sprintf(cmd1,"sound `%d`",LOSE_SOUND);
            sprintf(cmd2,"play `lose` %d",getIntParam(0));
            addCompoundCommand(cmd1,cmd2,NULL);
        }
        else if (isCommandType("fall_board")){
            char cmd1[255],cmd2[255],cmd3[255],cmd4[255],cmd5[255];
            sprintf(cmd1,"turnTowards %f %f %f 500",getFloatParam(0),getFloatParam(1),getFloatParam(2));
            sprintf(cmd2,"sound `%d`",FALLING_ONTO_BOARD_SOUND);
            sprintf(cmd3,"play `fall` 500");
            sprintf(cmd4,"setPosition %f %f %f %d",getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
            sprintf(cmd5,"wait 4000");            
            addCompoundCommand(cmd1,cmd2,cmd3,cmd4,cmd5,NULL);
        }
        else if (isCommandType("fall_water")){
            char cmd1[255],cmd2[255],cmd3[255],cmd4[255];
            SbVec3f currentPosOffset=positionOffset.getValue();
            sprintf(cmd1,"sound `%d`",FALLING_INTO_WATER_SOUND);
            sprintf(cmd2,"play `fall` 500");            
            sprintf(cmd3,"setPosition %f %f %f %d",currentPosOffset[0],currentPosOffset[1]-0.7f,currentPosOffset[2],getIntParam(0));
            sprintf(cmd4,"wait 2000");
            addCompoundCommand(cmd1,cmd2,cmd3,cmd4,NULL);
        }
        else if (isCommandType("jump_up") || isCommandType("jump_down")){
            char cmd1[255],cmd2[255],cmd3[255],cmd4[255],cmd5[255];
            SbVec3f currentPosOffset=positionOffset.getValue();
            SbVec3f endPosOffset(getFloatParam(0),getFloatParam(1),getFloatParam(2));
            SbVec3f midPosOffset=currentPosOffset*0.8f+endPosOffset*0.2f;
            sprintf(cmd1,"turnTowards %f %f %f 500",getFloatParam(0),getFloatParam(1),getFloatParam(2));
            sprintf(cmd2,"sound `%d`",isCommandType("jump_up") ? JUMP_UP_SOUND : JUMP_DOWN_SOUND);
            sprintf(cmd3,"play `jump` 1000");
            sprintf(cmd4,"setPosition %f %f %f 400",
                midPosOffset[0]+(endPosOffset[0]-currentPosOffset[0])*0.2f,
                midPosOffset[1]+0.022f*globalScale.getValue(),
                midPosOffset[2]+(endPosOffset[2]-currentPosOffset[2])*0.2f
            );
            sprintf(cmd5,"setPosition %f %f %f %d",endPosOffset[0],endPosOffset[1],endPosOffset[2],getIntParam(0));
            addCompoundCommand(cmd1,cmd2,cmd3,cmd4,cmd5,NULL);
        }
        else if (isCommandType("turnTowards")){
            SbVec3f pos=positionOffset.getValue();
            SbRotation rot=orientationOffset.getValue();
            SbVec3f transIn;
            rot.inverse().multVec(SbVec3f(getFloatParam(0),getFloatParam(1),getFloatParam(2))-pos,transIn);

            SbVec3f rotAxis(0,1,0);
            SbVec3f zero(0,0,1);
            float angle;
            SbVec3f transInProjected=transIn-(rotAxis.dot(transIn)/rotAxis.dot(rotAxis))*rotAxis;
            SbVec3f zeroProjected=zero-(rotAxis.dot(zero)/rotAxis.dot(rotAxis))*rotAxis;
            float lengthSquared=zeroProjected.length()*transInProjected.length();
            if (lengthSquared>0.0) angle=(float)acos(zeroProjected.dot(transInProjected)/lengthSquared);
            else angle=0;

            if (transInProjected.cross(zeroProjected).dot(rotAxis)>0) angle=(float)(6.28318530718-angle);

            float qx0,qy0,qz0,qw0,qx1,qy1,qz1,qw1;
            orientationOffset.getValue().getValue(qx0,qy0,qz0,qw0);
            SbRotation targetRot=rot*SbRotation(rotAxis,angle);
            targetRot.getValue(qx1,qy1,qz1,qw1);
            // if the rotation quaternions only very slightly differ from each other, skip it 
            // (this test is only applicable to this very simple case, where the character moves straight or turns by 90 degrees)
            if (fabs(qx0-qx1+qy0-qy1+qz0-qz1+qw0-qw1)>0.00001f){ // the value 0.00001f stands for a "kind of epsilon number"
                orientationInterpolator->input0.setValue(orientationOffset.getValue());
                orientationInterpolator->input1.setValue(targetRot);
                orientationTimer->duration=getTimeValue(500);
                orientationTimer->trigger.setValue();
                cal3dPuppet->state.setValue(getAnimationName("walk"));
                addDurationMonitor(getIntParam(0));
            }
            else noTrigger();
        }
        // if not implemented here, check parent class
        else return SoCal3DPuppeteer::implementCommand(puppet);
        return true;
    }
    return false;
}

void Character::idleCommand(SoNode *puppet)
{
    if (!idleEntered) appendCommand("state `idle`",NULL);
}
