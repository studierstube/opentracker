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
/* SoCal3DPuppeteer.cxx contains the implementation of class SoCal3DPuppeteer
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoCal3DPuppeteer.cxx 4127 2005-01-20 14:03:28Z bara $
 * @file
 */
/* ======================================================================== */

#include "SoCal3DPuppeteer.h"

SO_KIT_SOURCE(SoCal3DPuppeteer);

void SoCal3DPuppeteer::initClass()
{
    SO_KIT_INIT_CLASS(SoCal3DPuppeteer, SoPuppeteerKit, "SoPuppeteerKit");
    SoCal3DPuppet::initClass();
}

SoCal3DPuppeteer::SoCal3DPuppeteer()
{
    SO_KIT_CONSTRUCTOR(SoCal3DPuppeteer);

    // idle, walk, run, point
    SO_KIT_ADD_FIELD(animationMap, (""));
    SO_KIT_ADD_FIELD(animationPrefix, (""));
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
         blend %s %f";

    setUpConnections(TRUE,TRUE);
}

SoCal3DPuppeteer::~SoCal3DPuppeteer()
{
}

bool SoCal3DPuppeteer::implementCommand(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("CAL3DPUPPETEER: implement command\n");
#endif
    if (isType(puppet,"SoCal3DPuppet")){
        SoCal3DPuppet *cal3dPuppet=(SoCal3DPuppet*)foundNode;

        if (isCommandType("play")) {
            cal3dPuppet->execute.setValue(getAnimationName(getStringParam(0)));
            if (getIntParam(0)) addDurationMonitor(getIntParam(0));
            else noTrigger();
        }
        else if (isCommandType("state")){
            cal3dPuppet->state.setValue(getAnimationName(getStringParam(0)));
            if (getIntParam(0)) addDurationMonitor(getIntParam(0));
            else {
                goIdle=false;
                noTrigger();
            }
        }
        else if (isCommandType("blend")){
            // reset blending parameters
            cal3dPuppet->blendName.deleteValues(0);
            cal3dPuppet->blend.deleteValues(0);

            std::string blendStr=getStringParam(0);
            int start=0,end=0;
            bool parity=false;
            int count=0;
            while (
                (start=blendStr.find_first_not_of(" ",end))!=blendStr.npos && (end=blendStr.find(" ",start))!=blendStr.npos){
                parity=!parity;
                if (parity){
                    cal3dPuppet->blendName.set1Value(count,getAnimationName(blendStr.substr(start,end-start).c_str()));
                }
                else {
                    float value;
                    int result=sscanf(blendStr.substr(start,end-start).c_str(),"%f",&value);
                    if (result) cal3dPuppet->blend.set1Value(count,value);
                }
                count++;
            }
            goIdle=false;
            noTrigger();
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
                    string cmdStr1="goTo "+targetPosString+" "+moveDurationString+" `"+getStringParam(1)+"`";
                    cmdlist.push_back(cmdStr1);
                    int waitDuration;
                    if (sscanf(waitDurationString.c_str(),"%d",&waitDuration) && waitDuration){
                        string cmdStr2="wait "+waitDurationString;
                        cmdlist.push_back(cmdStr2);
                    }
                }
            }
            if (cmdlist.size()) addCompoundCommand(&cmdlist);
            else {
                printf("CHARACTER: the path '%s' for puppeteer '%s' could not be interpreted\n",
                    pathStr.c_str(),id.getValue().getString());
                noTrigger();
            }
        }
        else if (isCommandType("goTo")){
            char cmd1[255],cmd2[255],cmd3[255],cmd4[255];
            sprintf(cmd1,"state `walk`");
            sprintf(cmd2,"turnTowards %f %f %f 500",getFloatParam(0),getFloatParam(1),getFloatParam(2));
            sprintf(cmd3,"state `%s`",getStringParam(0));
            sprintf(cmd4,"setPosition %f %f %f %d",getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
            addCompoundCommand(cmd1,cmd2,cmd3,cmd4,NULL);
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
            sprintf(str1,"turnTowards %f %f %f 500",dst[0],dst[1],dst[2]);
            sprintf(str2,"state `%s`",getStringParam(1));
            sprintf(str3,"wait %d",getIntParam(0));
            addCompoundCommand(str1,str2,str3,NULL);
        }
        else return false;
        return true;
    }
    return false;
}

void SoCal3DPuppeteer::idleCommand(SoNode *puppet)
{
    if (!idleEntered){
        appendCommand("state `idle`",NULL);
    }
}

const char* SoCal3DPuppeteer::getAnimationName(const char *anim)
{
    std::string value=animationPrefix.getValue().getString();
    for (int i=0;i<animationMap.getNum();i++){
       char standard[255],mapped[255];
       int result=sscanf(animationMap[i].getString(),"%s %s",&standard,&mapped);
       if (result==2 && !strcmp(standard,anim)) {
           value+=mapped;
           return strdup(value.c_str());
       }
    }
    value+=anim;
    return strdup(value.c_str());
}
