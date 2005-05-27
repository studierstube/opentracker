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
/* SoFacePuppeteer.cxx contains the implementation of class SoFacePuppeteer
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoFacePuppeteer.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @file
 */
/* ======================================================================== */

#undef DEBUG_ARPUPPET

#include "SoFacePuppeteer.h"

SO_KIT_SOURCE(SoFacePuppeteer);

void SoFacePuppeteer::initClass()
{
    SO_KIT_INIT_CLASS(SoFacePuppeteer, SoPuppeteerKit, "SoPuppeteerKit");
    SoFacePuppet::initClass();
#ifdef DEBUG_ARPUPPET
    printf("FACEPUPPETEER: initialized\n");
#endif
}

SoFacePuppeteer::SoFacePuppeteer()
{
#ifdef DEBUG_ARPUPPET
    printf("FACEPUPPETEER: constructor\n");
#endif

    SO_KIT_CONSTRUCTOR(SoFacePuppeteer);
    SO_KIT_INIT_INSTANCE();

    whichPuppetsSensor=new SoFieldSensor(whichPuppetsCB,this);
    showPuppeteerSensor=new SoFieldSensor(showPuppeteerCB,this);

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
         play %s %d(100) %d(100) \
         speak %s \
         sound %s %d(0) \
         shutup \
         think %s %d(0) \
         wait %d \
         pointAt %f %f %f %s %d(0)";

    setUpConnections(TRUE,TRUE);
}

SoFacePuppeteer::~SoFacePuppeteer()
{
}

void SoFacePuppeteer::whichPuppetsChanged(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("FACEPUPPETEER: whichPuppet changed\n");
#endif
    int activePuppetNum=getActivePuppetNum();
    bool matchFound=false;

    for (int i=0;i<activePuppetNum;i++) {
        if (puppet==getPuppet(whichPuppets[i])){ // puppet is in the list of puppets to display 
            // choose between the diff. representations
            if (isType(puppet,"SoFacePuppet")){
                SoFacePuppet *facePuppet=(SoFacePuppet*)foundNode;
                if (!(facePuppet->isShowing())) facePuppet->Show();
            }
            // else if (isType(puppet,"SoFacePuppetIPAQ"){}
            matchFound=true;
            break;
        }
    }
    if (!matchFound) { // puppet is not in the list of puppets to display
        // choose between the diff. representations
        if (isType(puppet,"SoFacePuppet")){
            SoFacePuppet *facePuppet=(SoFacePuppet*)foundNode;
            if (facePuppet->isShowing()) facePuppet->Hide();
        }
        //else if (isType(puppet,"SoFacePuppetIPAQ")){}
    }
    else matchFound=false;
}

void SoFacePuppeteer::showPuppeteerChanged(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("FACEPUPPETEER: showPuppeteer changed to %s\n", showPuppeteer.getValue() ? "true" : "false");
#endif
    if (isType(puppet,"SoFacePuppet")){
        SoFacePuppet *facePuppet=(SoFacePuppet*)foundNode;
        if (showPuppeteer.getValue()) facePuppet->Show();
        else facePuppet->Hide();
    }
    //else if (isType(puppet,"SoFacePuppetIPAQ")){}
}

bool SoFacePuppeteer::implementCommand(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("FACEPUPPETEER: implement command\n");
#endif
    if (isType(puppet,"SoFacePuppet")){
        SoFacePuppet *facePuppet=(SoFacePuppet*)foundNode;

        if (isCommandType("play")){
            facePuppet->Play(getStringParam(0),getIntParam(0),getIntParam(1));
            addTriggerMonitor();
        }
        else if (isCommandType("speak")){
            facePuppet->Speak(getStringParam(0));
            SoField *field=NULL;
            speechText.getConnectedField(field);
            if (field!=&speechText)speechText.connectFrom(&facePuppet->speechText);
            addTriggerMonitor();
        }
        else if (isCommandType("sound")){
            facePuppet->Sound(getStringParam(0));
            addTriggerMonitor();
        }
        else if (isCommandType("think")){
            facePuppet->speechText.setValue(getStringParam(0));
            addDurationMonitor(getIntParam(0));
        }
        else if (isCommandType("shutup")){
            facePuppet->ShutUp();
            addTriggerMonitor();
        } 
        else if (isCommandType("lookAt") || isCommandType("pointAt")){
            char cmdstr1[255],cmdstr2[255];
            sprintf(cmdstr1,"turnTowards %f %f %f %d",getFloatParam(0),getFloatParam(1),getFloatParam(2),3*getIntParam(0)/4);
            float qx,qy,qz,w;
            SbRotation(SbVec3f(0,0,1),SbVec3f(getFloatParam(0),getFloatParam(1),getFloatParam(2))-position.getValue()).getValue(qx,qy,qz,w);
            sprintf(cmdstr2,"setOrientationQ %f %f %f %f %d",qx,qy,qz,w,getIntParam(0)/4);
            addCompoundCommand(cmdstr1,cmdstr2,NULL);
        }
        else return false;
        return true;
    } // end of SoFacePuppet command implementation
    // else if (isType(puppet,"SoFacePuppetIPAQ"){}

    return false;
}
    
SoField* SoFacePuppeteer::getFinishedSubCommandTrigger(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("FACEPUPPETEER: getFinishedSubCommand\n");
#endif
    if (isType(puppet,"SoFacePuppet")){
        return &(((SoFacePuppet*)foundNode)->finishedAnimation);
    }
    //else if (isType(puppet,"SoFacePuppetIPAQ")) {}
    return NULL;
}

const char* SoFacePuppeteer::getFinishedSubCommandData(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("FACEPUPPETEER: getFinishedSubCommandData\n");
#endif
    if (isType(puppet,"SoFacePuppet")){
        int cmdCode=((SoFacePuppet*)foundNode)->finishedAnimationData.getValue();
        char *str=new char[10];
        sprintf(str,"%d",cmdCode);
        return str;
    }
    //else if (isType(puppet,"SoFacePuppetIPAQ")) {}
    return "";
}

bool SoFacePuppeteer::translateFinishedCommand(const char *current,const char *finished)
{
#ifdef DEBUG_ARPUPPET
    printf("FACEPUPPETEER: translateFinishedCommand current: '%s', finished: '%s', ",current,finished);
#endif
    int numberParsed;
    char *str=new char[255];
    int result=sscanf(finished,"%d",&numberParsed);
    if (result) {
        result=sscanf(current,"%s",str);
        int cmp;
        switch (numberParsed){
            case 1:
                cmp=!strcmp(str,"play");
                break;
            case 2:
                cmp=!strcmp(str,"speak");
                break;
            case 3:
                cmp=!strcmp(str,"sound");
                break;
            case 4:
                cmp=!strcmp(str,"shutup");
                break;
            default:
                cmp=false;
        }
#ifdef DEBUG_ARPUPPET
        printf("(%s == %d) -> %s\n",str,numberParsed,cmp ? "true" : "false");
#endif
        delete[]str;
        return cmp ? true : false;
    }
    else {
#ifdef DEBUG_ARPUPPET
        printf("error while scanning %s\n",finished);
#endif
        return false;
    }
    delete[]str;
    return true;
}
