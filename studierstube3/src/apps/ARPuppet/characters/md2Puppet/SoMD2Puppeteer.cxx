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
/* SoMD2Puppeteer.h contains the definition of class SoMD2Puppeteer
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoMD2Puppeteer.cxx 4127 2005-01-20 14:03:28Z bara $
 * @file
 */
/* ======================================================================== */

#include "SoMD2Puppeteer.h"

SO_KIT_SOURCE(SoMD2Puppeteer);

void SoMD2Puppeteer::initClass()
{
    SO_KIT_INIT_CLASS(SoMD2Puppeteer, SoPuppeteerKit, "SoPuppeteerKit");
#ifdef DEBUG_ARPUPPET
    printf("MD2PUPPETEER: initialized\n");
#endif
}

SoMD2Puppeteer::SoMD2Puppeteer()
{
#ifdef DEBUG_ARPUPPET
    printf("MD2PUPPETEER: constructor\n");
#endif

    SO_KIT_CONSTRUCTOR(SoMD2Puppeteer);

    // idle, run, point
    SO_KIT_ADD_FIELD(animationMap, (""));
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
         pathMove %s \
         goTo %f %f %f %d(0) %s(run) \
         speak %s \
         sound %s %d(0) %d(-1)\
         shutup %s \
         wait %d \
         pointAt %f %f %f %s %d(0) \
         state %s %d(0) \
         play %s %d(0) \
         command %s %f(1.0)";
    
    setUpConnections(TRUE,TRUE);
}

SoMD2Puppeteer::~SoMD2Puppeteer()
{
}

bool SoMD2Puppeteer::implementCommand(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("MD2PUPPETEER: implement command\n");
#endif
    if (isType(puppet,"SoMD2NodeKit")){
        SoMD2NodeKit *md2NodeKit=(SoMD2NodeKit*)foundNode;

        if (isCommandType("state")){
            char str[255];
            if (getIntParam(0)) sprintf(str,"play `%s` %d",getStringParam(0),getIntParam(0));
            else {
                std::string cmd; float time;
                getAnimationName(getStringParam(0),&cmd,&time);            
                sprintf(str,"command `%s` %f",cmd.c_str(),time);
            }
            addCompoundCommand(str,NULL);
            goIdle=false;
        }
        else if (isCommandType("play")){
            std::string cmd; float time;
            getAnimationName(getStringParam(0),&cmd,&time);
            char str1[255],str2[255];
            sprintf(str1,"command `%s` %f",cmd.c_str(),time);
            sprintf(str2,"wait %d",getIntParam(0));
            addCompoundCommand(str1,str2,NULL);
        }
        else if (isCommandType("command")){
            md2NodeKit->animname.setValue(getStringParam(0));
            md2NodeKit->animtime.setValue(getFloatParam(0));
            noTrigger();
        }
        else if (isCommandType("goTo")){
            char cmd1[255],cmd2[255],cmd3[255],cmd4[255];
            sprintf(cmd1,"state `run`");
            sprintf(cmd2,"turnTowards %f %f %f 500",getFloatParam(0),getFloatParam(1),getFloatParam(2));
            sprintf(cmd3,"state `%s`",getStringParam(0));
            sprintf(cmd4,"setPosition %f %f %f %d",getFloatParam(0),getFloatParam(1),getFloatParam(2),getIntParam(0));
            addCompoundCommand(cmd1,cmd2,cmd3,cmd4,NULL);
        }
        else if (isCommandType("pointAt")){
            char str1[255],str2[255];
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
            sprintf(str2,"play `point` %d",getIntParam(0));
            addCompoundCommand(str1,str2,NULL);
        }
        else return false;
        return true;
    }
    return false;
}

void SoMD2Puppeteer::idleCommand(SoNode *puppet)
{
    if (!idleEntered){
        appendCommand("state `idle`",NULL);
    }
}

void SoMD2Puppeteer::getAnimationName(const char *inputName,std::string *outputName,float *outputTime)
{
    for (int i=0;i<animationMap.getNum();i++){
       char current[255],mapped[255];
       float time;
       int result=sscanf(animationMap[i].getString(),"%s %s %f",&current,&mapped,&time);
       if (result==3 && !strcmp(current,inputName)) {
           *outputName=mapped;
           *outputTime=time;
           return;
       }
    }
    *outputName=inputName;
    *outputTime=1.0f;
}