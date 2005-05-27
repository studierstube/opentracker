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
/* SoChoreographerKit.cxx contains the implementation of class SoChoreographerKit
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoChoreographerKit.cxx 3792 2004-09-29 22:39:35Z bara $
 * @file
 */
/* ======================================================================== */

#undef DEBUG_ARPUPPET

// general C++ includes
#include <ctype.h>

// Inventor classes
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/engines/SoSelectOne.h>

// Stb specific classes
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

// own classes
#include "SoChoreographerKit.h"
#include "SoViewingFrustum.h"

SO_KIT_SOURCE(SoChoreographerKit);

void SoChoreographerKit::initClass()
{
#ifdef DEBUG_ARPUPPET
    printf("CHOREOGRAPHER: SoChoreographerKit initialized\n");
#endif
    SoPuppeteerKit::initClass();
    SO_KIT_INIT_CLASS(SoChoreographerKit, SoCommandParserKit, "SoCommandParserKit");
}

SoChoreographerKit::SoChoreographerKit()
{
    SO_KIT_CONSTRUCTOR(SoChoreographerKit);

    SO_KIT_ADD_CATALOG_ENTRY(puppeteers, SoSeparator, FALSE, this, \0, TRUE);

    SO_KIT_ADD_FIELD(loadPuppeteers, ());    
    SO_KIT_ADD_FIELD(commandType, (""));
    SO_KIT_ADD_FIELD(targetList, (""));
    SO_KIT_ADD_FIELD(parameterList, (""));
    SO_KIT_ADD_FIELD(puppeteerName, (""));
    SO_KIT_ADD_FIELD(registerEvent, (""));
    SO_KIT_ADD_FIELD(deregisterEvent, (""));

    SO_KIT_INIT_INSTANCE();

    loadPuppeteersSensor=new SoFieldSensor(loadPuppeteersCB,this);
    registerEventSensor=new SoFieldSensor(registerEventCB,this);
    deregisterEventSensor=new SoFieldSensor(deregisterEventCB,this);

    // empty commandType, targetList and parameterList
    commandType.deleteValues(0);
    targetList.deleteValues(0);
    parameterList.deleteValues(0);

    // init macro parsing tokens
    parseToken[PUPPETEER]="puppeteer";
    parseToken[STATION]="station";
    parseToken[USER]="user";
    parseToken[PEN]="pen";
    parseToken[PIP]="pip";
    parseToken[NODE]="node";
    // !!! there is no token defined for HOTSPOT (puppeteer.h), HOTSPOTABSOLUTE (puppeteer.ah) and HOTSPOTOFFSET (puppeteer.fh) !!!

    commandString=
        "get %s \
         compare %s %s %s(equals) \
         collision %s %s \
         near %s %s %f \
         lookingAt %s %s \
         approach %s %s %f(0) %d(0) %f(0.7854) \
         pickup %s %s \
         drop %s %s \
         speak %s %s %d \
         goNear %s";
    // turningTowards %s %s %f(0) %d(0) turningAway %s %s %f(0) %d(0)

    setUpConnections(TRUE,TRUE);
}

SoChoreographerKit::~SoChoreographerKit()
{
    delete loadPuppeteersSensor;
    delete registerEventSensor;
    delete deregisterEventSensor;
}

SbBool SoChoreographerKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        loadPuppeteersSensor->attach(&loadPuppeteers);
        registerEventSensor->attach(&registerEvent);
        deregisterEventSensor->attach(&deregisterEvent);

        setCommandList();
        constructTrigger();
        
        loadPuppeteersChanged();
    }
    else {
        loadPuppeteersSensor->detach();
        registerEventSensor->detach();
        deregisterEventSensor->detach();
        
        resetTriggerList();
    }
    return !(connectionsSetUp = onOff);
}

void SoChoreographerKit::loadPuppeteersCB(void *data,SoSensor *sensor)
{
    SoChoreographerKit *choreographerKit=(SoChoreographerKit*)data;
    choreographerKit->loadPuppeteersChanged();
}

void SoChoreographerKit::registerEventCB(void *data,SoSensor *sensor)
{
    SoChoreographerKit *choreographerKit=(SoChoreographerKit*)data;
}

void SoChoreographerKit::deregisterEventCB(void *data,SoSensor *sensor)
{
    SoChoreographerKit *choreographerKit=(SoChoreographerKit*)data;
}

void SoChoreographerKit::finishedSubCommandCB(void *data,SoSensor *sensor)
{
    SoChoreographerKit *choreographerKit=(SoChoreographerKit*)data;
    for (int i=0;i<choreographerKit->finishedSubCommandSensorList.getLength();i++){
        if (((SoSensor*)(choreographerKit->finishedSubCommandSensorList[i]))==sensor) {
            SoCommandParserKit *target;
            if (i<choreographerKit->puppeteerInventory.getLength()) {
                target=(SoCommandParserKit*)(choreographerKit->puppeteerInventory[i]);
#ifdef DEBUG_ARPUPPET
                printf("CHOREOGRAPHER: finishedSubCommandCB '%s'\n",target->finishedSubCommandData.getValue().getString());
#endif
                choreographerKit->commandTriggered(target->finishedSubCommandData.getValue().getString());
                break;
            }
        }
    }
}

void SoChoreographerKit::loadPuppeteersChanged()
{
    SoSeparator *puppeteerRoot=SO_GET_ANY_PART(this,"puppeteers",SoSeparator);

    // remove all previous elements
    removeAllListItems(&puppeteerInventory);

    // reset field storing loaded puppeteers' name
    puppeteerName.deleteValues(0);

    // search for SoPuppeteerKit nodes in the scenegraph attached to "puppeteers" 
    SoSearchAction searchAction;
    searchAction.setInterest(SoSearchAction::ALL);
    searchAction.setSearchingAll(TRUE); // search every node in the scene graph regardless of switch settings   
    searchAction.setType(SoPuppeteerKit::getClassTypeId(),TRUE);
    searchAction.apply(puppeteerRoot);
    SoPathList pathList=searchAction.getPaths();
    for (int j=0;j<pathList.getLength();j++){
        SoPuppeteerKit *puppeteer=(SoPuppeteerKit*)(((SoFullPath*)(pathList[j]))->getTail());
        puppeteerInventory.append(puppeteer);
        puppeteerName.set1Value(puppeteerName.getNum(),puppeteer->id.getValue());
    }

    constructTrigger();
}

void SoChoreographerKit::addPuppeteer(SoPuppeteerKit *puppeteer)
{
    SoSeparator *root=(SoSeparator*)(this->getPart("puppeteers",FALSE));
    root->addChild(puppeteer);
    puppeteerInventory.append(puppeteer);
    puppeteerName.set1Value(puppeteerName.getNum(),puppeteer->id.getValue());

#ifdef DEBUG_ARPUPPET
    printf("CHOREOGRAPHER: added puppeteer: '%s'\n",puppeteer->id.getValue().getString());
#endif
}

bool SoChoreographerKit::removePuppeteer(SoPuppeteerKit *puppeteer)
{
    SoSeparator *root=(SoSeparator*)(this->getPart("puppeteers",FALSE));

    for (int i=0;i<puppeteerInventory.getLength();i++){
        if (((SoPuppeteerKit*)puppeteerInventory[i])->id.getValue()==puppeteer->id.getValue()){
#ifdef DEBUG_ARPUPPET
            printf("CHOREOGRAPHER: removed puppeteer: '%s'\n",puppeteer->id.getValue().getString());
#endif
            root->removeChild(i);
            puppeteerInventory.remove(i);
            puppeteerName.deleteValues(i,1);
            return true;
        }
    }

    return false;
}

bool SoChoreographerKit::removePuppeteer(int num)
{
    SoSeparator *root=(SoSeparator*)(this->getPart("puppeteers",FALSE));

    if (num<puppeteerInventory.getLength()) {
#ifdef DEBUG_ARPUPPET
        printf("CHOREOGRAPHER: added puppeteer: '%s'\n",
            ((SoPuppeteerKit*)(puppeteerInventory[num]))->id.getValue().getString());
#endif
        root->removeChild(num);
        puppeteerInventory.remove(num);
        puppeteerName.deleteValues(num,1);
        return true;
    }

    return false;
}

SoPuppeteerKit* SoChoreographerKit::getPuppeteer(const char *name)
{
    for (int i=0;i<puppeteerInventory.getLength();i++){
        SoPuppeteerKit *puppeteer=(SoPuppeteerKit*)puppeteerInventory[i];
        if (!strcmp(puppeteer->id.getValue().getString(),name)) return puppeteer;
    }
    return NULL;
}

SoPuppeteerKit* SoChoreographerKit::getPuppeteer(int num)
{
    if (num<puppeteerInventory.getLength()) return (SoPuppeteerKit*)(puppeteerInventory[num]);
    else return NULL;
}

int SoChoreographerKit::getPuppeteerNum()
{
    return puppeteerInventory.getLength();
}

void SoChoreographerKit::commandChanged()
{
    if (!dataNeedsValidation.getValue()){
#ifdef DEBUG_ARPUPPET
        printf("CHOREOGRAPHER: command changed\n");
#endif
        int num=command.getNum();
        if (num) {
            SbPList *list=new SbPList;
            bool parity=false;
            for (int i=0;i<num;i++){            
                string cmd=command[i].getString();
                int cmdPos=0,cmdPosOld=0;                
                do {
                    if (cmd[cmdPos]=='`') parity=!parity;
                    else if ((cmd[cmdPos]=='#' && !parity) || cmdPos==cmd.length()){
                        string cmdParsed=cmd.substr(cmdPosOld,cmdPos-cmdPosOld);
                        cmdPosOld=cmdPos+1;
                        SbStringList *subList=new SbStringList;
                        SbString *str=new SbString(cmdParsed.c_str());
                        subList->append(str);
                        list->append(subList);
                    }
                } while (cmdPos++<cmd.length());
            }
            commandQueue.append(list);
        }
    }
}

void SoChoreographerKit::dataValidChanged()
{
    if (dataNeedsValidation.getValue()){
#ifdef DEBUG_ARPUPPET
        printf("CHOREOGRAPHER: dataValid changed\n");
#endif
        int num=commandType.getNum();
        if (num){
            SbPList *list=new SbPList;
            for (int i=0;i<num;i++) {
                SbString *command=new SbString(targetList[i].getString());
                if (targetList[i].getLength()) *command+=": ";
                else *command+="";
                *command+=commandType[i].getString();
                *command+=" ";
                *command+=parameterList[i].getString();
                SbStringList *subList=new SbStringList;
                subList->append(command);
                list->append(subList);
            }
            commandQueue.append(list);
        }
        else {
            SbPList *list=new SbPList;
            bool parity=false;
            for (int i=0;i<command.getNum();i++){            
                string cmd=command[i].getString();
                int cmdPos=0,cmdPosOld=0;                
                do {
                    if (cmd[cmdPos]=='`') parity=!parity;
                    else if ((cmd[cmdPos]=='#' && !parity) || cmdPos==cmd.length()){
                        string cmdParsed=cmd.substr(cmdPosOld,cmdPos-cmdPosOld);
                        cmdPosOld=cmdPos+1;
                        SbStringList *subList=new SbStringList;
                        SbString *str=new SbString(cmdParsed.c_str());
                        subList->append(str);
                        list->append(subList);
                    }
                } while (cmdPos++<cmd.length());
            }
            commandQueue.append(list);
        }
    }
}

void SoChoreographerKit::processCommand(int index)
{
#ifdef DEBUG_ARPUPPET
    printf("CHOREOGRAPHER: process command '%s'\n",currentCommand.c_str());
#endif
    // remove previous parsing results
    removeAllListItems(&strParamList);
    removeAllListItems(&intParamList);
    removeAllListItems(&floatParamList);

    // reset targetNameList
    removeAllListItems(&targetNameList);

    // reset choreographer response
    removeAllListItems(&subResponseList);

    if (convertCommand()){
        // check whether it's a choreographer command or a puppeteer command to be choreographed
        if (parseCommandType(this)){
            if (!parseCommand()) return;
        }

        // execute command
        bool implemented=executeCommand();

        // set number of targets included in the current command
        int targetNum;
        if (!targetNameList.getLength()) targetNum=1;
        else targetNum=(*(targetNameList[0])=="*") ? puppeteerInventory.getLength() : targetNameList.getLength();
        if (index<targetNumberQueue.getLength()) targetNumberQueue[index]=targetNum;
        else targetNumberQueue.append(targetNum);
            
#ifdef DEBUG_ARPUPPET
        if (implemented) printf("CHOREOGRAPHER: execution of command #%d = '%s' started with %d target(s)\n",
            index,currentCommand.c_str(),targetNum);
        else printf("CHOREOGRAPHER: there was a problem while executing command #%d = '%s'\n",index,currentCommand.c_str());
#endif
    }
}

bool SoChoreographerKit::processFinishedCommand(const char *current,const char *finished,int currentIndex)
{
#ifdef DEBUG_ARPUPPET
    printf("CHOREOGRAPHER: processFinishedCommand '%s' -> current: '%s', finished: '%s', currentIndex: %d, translated: '%s'\n",
        id.getValue().getString(),current,finished,currentIndex,
        translateFinishedCommand(current,finished) ? "true" : "false"
    );
#endif
    string cur=current;
    string fin=finished;
    int markerCur=cur.find(":");
    int markerFin=fin.find(":");
    int targetFound;

    string targetCur=cur.substr(0,markerCur);
    string targetFin=fin.substr(0,markerFin);
    if (targetCur=="*" || (targetFound=targetCur.find(targetFin))!=targetCur.npos){
        int ix=targetNumberQueue[currentIndex];
        targetNumberQueue[currentIndex]=ix-1;
#ifdef DEBUG_ARPUPPET
        printf("CHOREOGRAPHER: target '%s' found in targetList '%s', targetNumberQueue = %d\n",targetFin.c_str(),targetCur.c_str(),ix-1);
#endif
        if (!(ix-1)) return true;
    }
    return false;
}

void SoChoreographerKit::resetCommandProcess()
{
    // reset output field values
    finishedCommandData.deleteValues(0);
    response.deleteValues(0);

    // empty choreographer response list
    removeAllListItems(&responseList);

    // reset target number list
    removeAllListItems(&targetNumberQueue);
}

void SoChoreographerKit::resetSubCommandProcess()
{
    // reset puppet command strings
    for (int i=0;i<puppeteerInventory.getLength();i++){
        ((SoPuppeteerKit*)puppeteerInventory[i])->command.deleteValues(0);
    }
}

bool SoChoreographerKit::convertCommand()
{
    // replace all valid macros found in the command
    replaceMacros();

    int marker1=0,marker2=0,marker3=0;
    bool paramListExists=true;

    // trim spaces from the head of the string
    if ((marker1=currentCommand.find_first_not_of(" "))==currentCommand.npos) {
        printf("CHOREOGRAPHER: parsing warning: no command found\n");
        return false;
    }
    
    // parsing target list
    string attribute;
    if ((marker2=currentCommand.find(":",marker1))!=currentCommand.npos){
        // TODO: target name may not contain any ` characters --> fix this!
        marker3=currentCommand.find("`",marker1);
        if (marker3==currentCommand.npos || marker3>marker2){
            if ((marker3=currentCommand.find(" ",marker1))==currentCommand.npos) marker3=currentCommand.length();
            if (marker3>marker2) marker3=marker2;
            attribute=currentCommand.substr(marker1,marker3-marker1);
            if (!parseTargetList(attribute.c_str())) return false;
            marker1=marker2+1;
        }
    }

    // parsing command type
    if ((marker1=currentCommand.find_first_not_of(" ",marker1))!=currentCommand.npos){
        currentCommand=currentCommand.substr(marker1);
    }
    else {
        printf("CHOREOGRAPHER: parsing warning: no command found after target list\n");
        return false;
    }

    return true;
}

void SoChoreographerKit::replaceMacros()
{
    int startPos=0,endPos=0,attrEndPos=0,nextPos=0;
    int typeOut=-1;
    void *resultObj=NULL;
    string replaceStr;
    bool dirtyFlag=false;

    // redo parsing with new command string if substitution has been made
    while (getMacroObject(currentCommand,resultObj,typeOut,startPos,endPos,nextPos)) {
        if (currentCommand[endPos]=='.') endPos++;
        else {
            nextPos=endPos;
            continue;
        }
        if ((nextPos=currentCommand.find(" ",endPos))==currentCommand.npos) nextPos=currentCommand.length();
        string attribute=currentCommand.substr(endPos,nextPos-endPos);
        if (getAttributeString(replaceStr,attrEndPos,attribute,resultObj,NULL,typeOut)){
            currentCommand=currentCommand.substr(0,startPos)+replaceStr+currentCommand.substr(endPos+attrEndPos);
            nextPos=0;
        }
        else nextPos=endPos;
    }
}

bool SoChoreographerKit::getMacroObject(string &macroString,void*& object,int &typeOut,int &startPos,int &endPos,int searchPos,int typeIn)
{
    int num;
    
    // HOTSPOT is an internal category, don't allow it here
    int i=(typeIn>=PUPPETEER && typeIn<HOTSPOT) ? typeIn : PUPPETEER;
    do {
        string parseString;        
        parseString=parseToken[i]+"(";
        int nextPos=searchPos;
        while ((startPos=macroString.find(parseString,nextPos))!=macroString.npos){
            if ((endPos=macroString.find(")",startPos+parseString.length()))!=macroString.npos){
                string id=macroString.substr(startPos+parseString.length(),endPos-startPos-parseString.length());
                switch (i){
                    case PUPPETEER:
                        if ((object=getPuppeteer(id.c_str()))!=NULL){
                            typeOut=PUPPETEER;
                            endPos++;
                            return true;
                        }
                        break;
                    case STATION:
                    case USER:
                    case PEN:
                    case PIP:
                        if (!sscanf(id.c_str(),"%d",&num)) break;
                        switch (i){
                            case USER:
                                object=new int;
                                *((int*)object)=SoUserManagerKit::getInstance()->getHmdFromUser(num);
                                break;
                            case PEN:
                                object=new int;
                                *((int*)object)=SoUserManagerKit::getInstance()->getPenFromUser(num);
                                break;
                            case PIP:
                                object=new int;
                                *((int*)object)=SoUserManagerKit::getInstance()->getPipFromUser(num);
                                break;
                            default:
                                object=new int;
                                *((int*)object)=num;
                                break;
                        }
                        if (num>=0) {
                            typeOut=STATION;
                            endPos++;
                            return true;
                        }
                        break;
                    case NODE:
                        if ((object=SoNode::getByName(id.c_str()))!=NULL){
                            typeOut=NODE;
                            endPos++;                        
                            return true;
                        }
                        break;
                    default:
                        break;
                }
            }
            nextPos+=parseString.length();
        } // end of while scanning the whole string of macroString
        i++;
    } while (typeIn>=0 || i<=NODE); // end of 'do...while' iterating through the macro categories 
                                    // (stop before HOTSPOT as it's already an internal category)
    return false;
}

bool SoChoreographerKit::getAttributeString(string &result,int &endPos,string &attrStr,void *object,void *object2,int type)
{
    if (!attrStr.length() || object==NULL) return false;
    
    if (type==PUPPETEER || type==HOTSPOT || type==HOTSPOTOFFSET || type==HOTSPOTABSOLUTE || type==STATION){
        
        SbVec3f vec,vec2;
        SbRotation rot;
        int buttonNum=0,buttonValue;
        float q0,q1,q2,q3;
        char printStr[255];
        int marker=0;

        switch (attrStr[0]) {
            case 'p':
            case 'P':
                if (type==PUPPETEER) vec=((SoPuppeteerKit*)object)->position.getValue();
                else if (type==HOTSPOT) {
                    if (!((SoPuppeteerKit*)object)->getHotspotCoord(((string*)object2)->c_str(),vec,SoPuppeteerKit::LOCAL_HS)) break;
                    delete object2;
                }
                else if (type==HOTSPOTABSOLUTE) {
                    if (!((SoPuppeteerKit*)object)->getHotspotCoord(((string*)object2)->c_str(),vec,SoPuppeteerKit::ABSOLUTE_HS)) break;
                    delete object2;
                }
                else if (type==HOTSPOTOFFSET) {
                    if (!((SoPuppeteerKit*)object)->getHotspotCoord(((string*)object2)->c_str(),vec,SoPuppeteerKit::OFFSET_HS)) break;
                    delete object2;
                }
                else {
                    vec=(Stb3DEventGenerator::getTranslationField(*((int*)object)))->getValue();
                    delete object;
                }
                if (attrStr.length()>2 && attrStr[1]=='.') {
                    if (getVectorString(result,endPos,attrStr.substr(2),vec)){
                        endPos=endPos+2;
                        return true; 
                    }
                } 
                else if (attrStr.length()<2 || !isalnum(attrStr[1])){
                    sprintf(printStr,"%f %f %f",vec[0],vec[1],vec[2]);
                    result=printStr;
                    endPos=1;
                    return true;
                }
                break;
            case 'o':
            case 'O':
                if (type==PUPPETEER) rot=((SoPuppeteerKit*)object)->orientation.getValue();
                else if (type==HOTSPOT) {
                    if (!((SoPuppeteerKit*)object)->getHotspotRot(((string*)object2)->c_str(),rot,SoPuppeteerKit::LOCAL_HS)) break;
                    delete object2;
                }
                else if (type==HOTSPOTABSOLUTE) {
                    if (!((SoPuppeteerKit*)object)->getHotspotRot(((string*)object2)->c_str(),rot,SoPuppeteerKit::ABSOLUTE_HS)) break;
                    delete object2;
                }
                else if (type==HOTSPOTOFFSET) {
                    if (!((SoPuppeteerKit*)object)->getHotspotRot(((string*)object2)->c_str(),rot,SoPuppeteerKit::OFFSET_HS)) break;
                    delete object2;
                }
                else {
                    rot=(Stb3DEventGenerator::getRotationField(*((int*)object)))->getValue();
                    delete object;
                }
                if (attrStr.length()>2 && attrStr[1]=='.') {
                    if (getRotationString(result,endPos,attrStr.substr(2),rot)){
                        endPos=endPos+2;
                        return true; 
                    }
                }
                else if (attrStr.length()<2 || !isalnum(attrStr[1])){
                    rot.getValue(q0,q1,q2,q3);
                    sprintf(printStr,"%f %f %f %f",q0,q1,q2,q3);
                    result=printStr;
                    endPos=1;
                    return true;
                }
                break;
            case 's':
            case 'S':
                if (type==PUPPETEER){
                    if (attrStr.length()>2 && attrStr[1]=='.') {
                        if (getVectorString(result,endPos,attrStr.substr(2),((SoPuppeteerKit*)object)->scale.getValue())) {
                            endPos=endPos+2;
                            return true; 
                        }
                    }
                    else if (attrStr.length()<2 || !isalnum(attrStr[1])){
                        vec=((SoPuppeteerKit*)object)->scale.getValue();
                        sprintf(printStr,"%f %f %f",vec[0],vec[1],vec[2]);
                        result=printStr;
                        endPos=1;
                        return true;
                    }
                }
                break;
            case 'b':
            case 'B':
                if (type==PUPPETEER && attrStr.length() && (attrStr[1]=='b' || attrStr[1]=='B')){
                    if (attrStr.length()<3 || !isalnum(attrStr[2])){
                        vec=((SoPuppeteerKit*)object)->boundingBox[0];
                        vec2=((SoPuppeteerKit*)object)->boundingBox[1];
                        if (((SoPuppeteerKit*)object)->boundingBoxMode.getValue()==SoPuppeteerKit::AABB){ // min, max
                            sprintf(printStr,"%f %f %f %f %f %f",vec[0],vec[1],vec[2],vec2[0],vec2[1],vec2[2]);
                        }
                        else if (((SoPuppeteerKit*)object)->boundingBoxMode.getValue()==SoPuppeteerKit::OBB){ // min, max, rotation
                            rot=((SoPuppeteerKit*)object)->orientation.getValue();
                            rot.getValue(q0,q1,q2,q3);
                            sprintf(printStr,"%f %f %f %f %f %f %f %f %f %f",vec[0],vec[1],vec[2],vec2[0],vec2[1],vec2[2],q0,q1,q2,q3);
                        }
                        // boundingBoxMode == NONE
                        else strcpy(printStr,"0 0 0 0 0 0"); // min, max (min=max=0)
                        result=printStr;
                        endPos=2;
                        return true;
                    }
                }
                else if (type==STATION){
                    buttonNum=0;
                    if (attrStr.length()>2 && attrStr[1]=='(' && (marker=attrStr.find(")"))!=attrStr.npos){
                        if (sscanf(attrStr.substr(2,marker-2).c_str(),"%d",&buttonNum)){
                            endPos=marker+1;
                        }
                    }
                    else endPos=1;
                    buttonValue=(Stb3DEventGenerator::getButtonField(*((int*)object),buttonNum))->getValue();
                    sprintf(printStr,"%d",buttonValue);
                    result=printStr;
                    return true;
                }
                break;
            case 'h':
            case 'H':
                if (type==PUPPETEER){
                    if (attrStr.length()>4 && attrStr[1]=='(' && (marker=attrStr.find(")."))!=attrStr.npos){
                        object2=new string;
                        *((string*)object2)=attrStr.substr(2,marker-2);
                        if (getAttributeString(result,endPos,attrStr.substr(marker+2),object,object2,HOTSPOT)){
                            endPos=endPos+marker+2;
                            return true;
                        }
                    }
                    break;
                }
                break;
            case 'a':
            case 'A':
                if (type==PUPPETEER && attrStr.length()){
                    if (attrStr[1]=='h' || attrStr[1]=='H'){
                        if (attrStr.length()>5 && attrStr[2]=='(' && (marker=attrStr.find(")."))!=attrStr.npos){
                            object2=new string;
                            *((string*)object2)=attrStr.substr(3,marker-3);
                            if (getAttributeString(result,endPos,attrStr.substr(marker+2),object,object2,HOTSPOTABSOLUTE)){
                                endPos=endPos+marker+2;
                                return true;
                            }
                        }
                    }
                }
                break;
            case 'f':
            case 'F':
                if (type==PUPPETEER && attrStr.length()){
                    if (attrStr[1]=='h' || attrStr[1]=='H'){
                        if (attrStr.length()>5 && attrStr[2]=='(' && (marker=attrStr.find(")."))!=attrStr.npos){
                            object2=new string;
                            *((string*)object2)=attrStr.substr(3,marker-3);
                            if (getAttributeString(result,endPos,attrStr.substr(marker+2),object,object2,HOTSPOTOFFSET)){
                                endPos=endPos+marker+2;
                                return true;
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
    
    if (type==NODE || type==PUPPETEER){
        int marker1,marker2;
        int num;
        if ((marker1=attrStr.find("["))!=attrStr.npos){
            if ((marker2=attrStr.find("]",marker1+1))!=attrStr.npos){
                if (sscanf(attrStr.substr(marker1+1,marker2-marker1-1).c_str(),"%d",&num)){
                    endPos=marker2+1;
                    return (getFieldValueString(result,((SoNode*)object)->getField(attrStr.c_str()),num));
                }
            }
            endPos=marker1;
        }
        else {
            for (int i=0;i<attrStr.length() && isalnum(attrStr[i]);i++);
            endPos=i;
        }
        return getFieldValueString(result,((SoNode*)object)->getField(attrStr.substr(0,endPos).c_str()));
    }

    return false;
}

bool SoChoreographerKit::getVectorString(string &result,int &endPos,string &attrStr,SbVec3f vec)
{
    char printStr[255];
    strcpy(printStr,"");
    
    if (attrStr.length()==1) {
        switch (attrStr[0]){
            case 'x':
                sprintf(printStr,"%f",vec[0]);
                break;
            case 'y':
                sprintf(printStr,"%f",vec[1]);
                break;
            case 'z':
                sprintf(printStr,"%f",vec[2]);
                break;
            default:
                break;
        }
    }

    if (strlen(printStr)) {
        result=printStr;
        endPos=1;
    }
    else {
        sprintf(printStr,"%f %f %f",vec[0],vec[1],vec[2]);
        result=printStr;
        endPos=0;
    }
    
    return true;
}

bool SoChoreographerKit::getRotationString(string &result,int &endPos,string &attrStr,SbRotation rot)
{
    char printStr[255];
    SbVec3f vector;
    float angle;
    float x,y,z,w;

    strcpy(printStr,"");
    rot.getValue(x,y,z,w);
    rot.getValue(vector,angle);
    
    if (attrStr.length()==1) {
        switch (attrStr[0]){
            case 'x':
                sprintf(printStr,"%f",x);
                break;
            case 'y':
                sprintf(printStr,"%f",y);
                break;
            case 'z':
                sprintf(printStr,"%f",z);
                break;
            case 'w':
                sprintf(printStr,"%f",w);
                break;
            case 'v':
                sprintf(printStr,"%f %f %f",vector[0],vector[1],vector[2]);
                break;
            case 'a':
                sprintf(printStr,"%f",angle);
                break;
            default:
                break;
        }
    }

    if (strlen(printStr)) {
        result=printStr;
        endPos=1;
    }
    else {
        sprintf(printStr,"%f %f %f %f",x,y,z,w);
        result=printStr;
        endPos=0;
    }

    return true;
}


bool SoChoreographerKit::getFieldValueString(string &result,SoField *field,int index)
{
    SbString value;
    if (field!=NULL) {
        if (field->isOfType(SoSField::getClassTypeId())){
            ((SoSField*)field)->get(value);
            if (field->isOfType(SoSFString::getClassTypeId())) value=value.getSubString(1,value.getLength()-2);
        }
        else {
            if (index>=0){
                if (index<((SoMField*)field)->getNum()) {
                    ((SoMField*)field)->get1(index,value);
                    if (field->isOfType(SoMFString::getClassTypeId())) value=value.getSubString(1,value.getLength()-2);
                }
            }
            else {
                SbString v;
                for (int i=0;i<((SoMField*)field)->getNum();i++) {
                    ((SoMField*)field)->get1(i,v);
                    if (v.getLength()){
                        if (value.getLength()) value+=SbString(" ");
                        if (field->isOfType(SoMFString::getClassTypeId())) value+=v.getSubString(1,v.getLength()-2);
                        else value+=v;
                    }
                }
            }
        }
        result=value.getString();
        return true;
    }
    else return false;
}

bool SoChoreographerKit::parseTargetList(const char *cmd)
{
    int marker1=0,marker2=0;
    string commandParsed=cmd;
    SoCommandParserKit *target;
    bool targetExists=false;

    do {
        marker2=commandParsed.find(',',marker1);
        SbString *targetNameStr=new SbString(commandParsed.substr(marker1,marker2-marker1).c_str());
        if (marker2!=commandParsed.npos) marker1=marker2+1;
        if ((target=getPuppeteer(targetNameStr->getString()))!=NULL || *targetNameStr=="*"){
            targetNameList.append(targetNameStr);
            targetExists=true;
#ifdef DEBUG_ARPUPPET
            printf("CHOREOGRAPHER: parsed target name: '%s'\n",targetNameStr->getString());
#endif
        }
        else printf("CHOREOGRAPHER: parsing error: puppeteer target '%s' not found\n",targetNameStr->getString());
    }
    while (marker2!=commandParsed.npos);

    return targetExists;
}

bool SoChoreographerKit::executeCommand()
{
    int i;
    SoCommandParserKit *target;
    
    bool handled=false;
    if (!targetNameList.getLength()) {
        printf("CHOREOGRAPHER: '%s' started executing command '%s'\n",
            id.getValue().getString(),currentCommand.c_str());
        handled=implementCommand();
    }
    else handled=choreograph();

    // if it's neither a valid choreographer command nor a compound puppeteer command (simple choreographing is OK), 
    // send it to the target puppeteers
    if (targetNameList.getLength() && !handled){
        // puppeteer command
        if (*(targetNameList[0])=="*"){
#ifdef DEBUG_ARPUPPET
            printf("CHOREOGRAPHER: sending command to all puppeteers\n");
#endif
            for (i=0;i<puppeteerInventory.getLength();i++) {
                // if there were already animation commands referring to the same puppeteer, simply add another entry
                // at the end of it's 'command' SoMFString attribute
                target=(SoCommandParserKit*)puppeteerInventory[i];
                if (target!=NULL) target->command.set1Value(target->command.getNum(),currentCommand.c_str());
                addTriggerMonitor();
            }
        }
        else if (targetNameList.getLength()){
            for (i=0;i<targetNameList.getLength();i++) {
#ifdef DEBUG_ARPUPPET
                printf("CHOREOGRAPHER: sending command '%s' to '%s'\n",currentCommand.getString(),targetNameList[i]->getString());
#endif
                // if there were already animation commands referring to the same puppeteer, simply add another entry
                // at the end of it's 'command' SoMFString attribute
                target=getPuppeteer(targetNameList[i]->getString());
                if (target!=NULL) target->command.set1Value(target->command.getNum(),currentCommand.c_str());
                addTriggerMonitor();
            }
        }
        else noTrigger();
    }

    return true;
}

void SoChoreographerKit::resetTriggerList()
{
    for (int i=0;i<finishedSubCommandSensorList.getLength();i++) ((SoFieldSensor*)(finishedSubCommandSensorList[i]))->detach();
    removeAllListItems(&finishedSubCommandSensorList);
}

void SoChoreographerKit::constructTrigger()
{
#ifdef DEBUG_ARPUPPET
    printf("CHOREOGRAPHER: construct trigger\n");
#endif
    resetTriggerList();

    for (int i=0;i<puppeteerInventory.getLength();i++){
        SoCommandParserKit *commandParserKit;
        commandParserKit=(SoCommandParserKit*)(puppeteerInventory[i]);
        SoFieldSensor *triggerSensor=new SoFieldSensor(finishedSubCommandCB,this);
        triggerSensor->attach(&commandParserKit->finishedSubCommand);
        finishedSubCommandSensorList.append(triggerSensor);
    }
}

bool SoChoreographerKit::collision_aabb(SbVec3f min0,SbVec3f max0,SbVec3f min1,SbVec3f max1)
{
    bool allZero= !min0[0] && !max1[0] && !max0[0] && !min1[0] && 
                  !min0[1] && !max1[1] && !max0[1] && !min1[1] && 
                  !min0[2] && !max1[2] && !max0[2] && !min1[2];
    return (
         !allZero &&          // both objects are visible
         min0[0]<=max1[0] &&  // x_min1 <= x_max2
         max0[0]>=min1[0] &&  // x_max1 >= x_min2
         min0[1]<=max1[1] &&  // y_min1 <= y_max2
         max0[1]>=min1[1] &&  // y_max1 >= y_min2
         min0[2]<=max1[2] &&  // z_min1 <= z_max2
         max0[2]>=min1[2]     // z_max1 >= z_min2
    );
}

bool SoChoreographerKit::collision_obb(SbVec3f min0,SbVec3f max0,SbRotation rot0,SbVec3f min1,SbVec3f max1,SbRotation rot1)
{
    return true; // TODO: *** add OBB intersection test ***
}

bool SoChoreographerKit::implementCommand()
{
#ifdef DEBUG_ARPUPPET
    printf("CHOREOGRAPHER: implementing command %s...\n",currentType.c_str());
#endif

    // simply set the response to the value of the string parameter
    if (isCommandType("get")){
        addResponse(getStringParam(0));
        noTrigger();
        return true;
    }

    // compare the values stored in the given parameters
    else if (isCommandType("compare")){
        if (!strcmp(getStringParam(2),"equals")){
            addResponse(strcmp(getStringParam(0),getStringParam(1)) ? "FALSE" : "TRUE");
            noTrigger();
            return true;
        }
        // else if ...
        return false;
    }

    /* a) collision `target0` `target1`: check a given bounding box or point intersecting with another bounding box or point
       b) near `target0` `target1` distance: check whether the center of a given bounding box or point is within a certain 
          distance from the center of another bounding box or another point */
    else if (isCommandType("collision") || isCommandType("near")){
        SbVec3f bb_min[2],bb_max[2];
        SbRotation bb_rot[2];
        bool obbTest=false;
        float x0,y0,z0,x1,y1,z1,qx,qy,qz,w;

        for (int i=0;i<2;i++){
            int result=sscanf(getStringParam(i),"%f %f %f %f %f %f %f %f %f %f",&x0,&y0,&z0,&x1,&y1,&z1,&qx,&qy,&qz,&w);
            if (result!=3 && result!=6 && result!=10) {
                printf("CHOREOGRAPHER: %s: insufficient number of parameters in '%s'\n",
                    currentType.c_str(),getStringParam(i));
                return false;
            }
            bb_min[i]=SbVec3f(x0,y0,z0);
            if (result==3) bb_max[i]=bb_min[i]; // point
            else bb_max[i]=SbVec3f(x1,y1,z1); // bounding box
            if (result==10) {
                bb_rot[i]=SbRotation(qx,qy,qz,w);
                obbTest=true;
            }
        }

        // 1.) Test AABB intersection first
        // 2.) If intersection has been detected and we deal with OBBs, refine collision detection by OBB intersection test
        if (
            collision_aabb(bb_min[0],bb_max[0],bb_min[1],bb_max[1]) &&
            (!obbTest || collision_obb(bb_min[0],bb_max[0],bb_rot[0],bb_min[1],bb_max[1],bb_rot[1]))
           ) {            
           addResponse("TRUE");
        }
        else {      
            if (isCommandType("collision")) addResponse("FALSE");
            else { // 'near'
                // distance between centers
                float distance=((bb_min[0]+bb_max[0])/2.0f-(bb_min[1]+bb_max[1])/2.0f).length();
                for (int j=0;j<2;j++){
                    // TODO: distance is currently calculated using bounding spheres --> change it to bounding box!!!
                    // min != max --> bounding box mode, otherwise point mode   
                    distance-=(bb_max[j]-bb_min[j]).length()/2.0f;
                }
                addResponse(fabs(distance)<=fabs(getFloatParam(0)) ? "TRUE" : "FALSE");
            }
        }
        noTrigger();
        return true;
    }
    
    /* a) lookingAt `target0` `target1`: check whether a given bounding box or point of target1 is intersecting the
          viewing frustum representing the field of view of target0 (target0 is an object, not a point of BB)
       b) approach `target0` `target1` [velocity] [minMax] [t] : check whether target0 is moving towards target1
          or its given hotpspot with at least (minMax = 0) or at most (minMax = 0) a certain speed --> the following conditions 
          need to be met:
           - target0 is looking at target1
           - the angle between the velocity vector of target0 and the vector going from target0's position to target1's
             position or it's given hotspot is within angleTolerance
           - the speed is within 'velocity' (if the given velocity is 0, it won't be considered) */
    else if (isCommandType("lookingAt") || isCommandType("approach")){
        void *object=NULL;
        int type;
        int start,end;
        string str=getStringParam(0);
        if (!getMacroObject(str,object,type,start,end) || (type!=PUPPETEER && type!=STATION)){
            printf("CHOREOGRAPHER: %s: could not parse target object (puppeteer or station) in '%s'\n",
                currentType.c_str(),getStringParam(0));
            return false;
        }
        float x0,y0,z0,x1,y1,z1;
        int result=sscanf(getStringParam(1),"%f %f %f %f %f %f",&x0,&y0,&z0,&x1,&y1,&z1);
        if (result!=3 && result!=6) {
            printf("CHOREOGRAPHER: %s: insufficient number of parameters in '%s'\n",
                currentType.c_str(),getStringParam(1));
            return false;
        }
        SbVec3f min,max;
        min=SbVec3f(x0,y0,z0);
        if (result==3) max=min; // point
        else max=SbVec3f(x1,y1,z1); // bounding box

        SoNodeKitListPart *vfl;
        SoViewingFrustum *vf;
        SbVec3f stationPos;
        SbRotation stationRot;
        int maxNum=0;
        if (type==PUPPETEER) {
            vfl=(SoNodeKitListPart*)(((SoPuppeteerKit*)object)->getPart("viewingFrustums",FALSE));
            maxNum=vfl->getNumChildren();
        }
        else {
            stationPos=(Stb3DEventGenerator::getTranslationField(*((int*)object)))->getValue();
            stationRot=(Stb3DEventGenerator::getRotationField(*((int*)object)))->getValue();
            maxNum=1;
        }
        
        for (int i=0;i<maxNum;i++){
            if (type==PUPPETEER) vf=(SoViewingFrustum*)(vfl->getChild(i));
            else {
                vf=new SoViewingFrustum;
                vf->nearDistance=0.001f;
                vf->farDistance=3.0f;
                vf->heightAngle=30.0f/180.0f*(float)M_PI;
                vf->positionOffset.setValue(stationPos);
                vf->orientationOffset.setValue(stationRot);
            }
            bool intersecting;
            if (min==max) intersecting=vf->intersectPoint(min); // point intersection
            else intersecting=vf->intersectBB(min,max); // BB intersection
            if (intersecting){
                if (isCommandType("approach")){
                    // ...
                    // angleTolerance --> radian or angle???
                }
                else {
                    addResponse("TRUE");
                    return true;
                }
            }
        }
        addResponse("FALSE");
        return true;
    }

    /* a) turningTowards `target0` `target1` [velocity] [minMax]: check whether target0 is turning towards target1 with at least 
          (minMax = 0) or at most (minMax = 0) a certain speed. If the given velocity is 0, it won't be considered
       b) turningAway `target0` `target1` [velocity] [minMax]: check whether target0 is turning away from target1 with at least 
          (minMax = 0) or at most (minMax = 0) a certain speed. If the given velocity is 0, it won't be considered */
    //else if (isCommandType("turningTowards") || isCommandType("turningAway")){}

    /* a) pickup `target0` `target1` 'transFieldName` `rotFieldName`: target0 (puppeteer with or without a specified hotspot) 
          picks up target1 with the specified hotspot i.e. connects the selected puppeteer or puppeteer hotspot pose to the 
          positionExt and orientationExt fields. If the specified hotspot name is invalid, puppeteer1's default pickup hotspot 
          is used, if no pickup hotspot has been defined, then puppeteer1's own position and orientation is used
       b) drop `p1` `p2`: puppeteer1 drops puppeteer2 whatever hotspot it is attached to */
    else if (isCommandType("pickup") || isCommandType("drop")){
        SoPuppeteerKit *object[2];
        int *type=new int[2];
        int start,end;
        string str[2];
        for (int i=0;i<2;i++){
            str[i]=getStringParam(i);
            object[i]=NULL;
            void *temp;
            // TODO: pickup ordinary nodes as well --> pickup %s %s %s %s
            if (!getMacroObject(str[i],temp,*(type+i),start,end) || type[i]!=PUPPETEER){ // && !(i==1 && type[i]==NODE))){
                printf("CHOREOGRAPHER: %s: could not parse target object in '%s'\n",
                    currentType.c_str(),getStringParam(i));
                return false;
            }
            object[i]=(SoPuppeteerKit*)temp;
        }
       
        // drop puppeteer2 if it has been picked up by puppeteer1
        char searchString[255];
        sprintf(searchString,"%s %s",object[0]->id.getValue().getString(),object[1]->id.getValue().getString());
        for (int j=0;j<hotspotInfoList.getLength();j++){
            if (!strcmp(hotspotInfoList[j]->getString(),searchString)){
                // administration
                hotspotSelectorList.remove(j*2);
                hotspotSelectorList.remove(j*2);
                hotspotInfoList.remove(j);
                // maintain control mode, position and orientation
                SbVec3f oldPos=object[1]->position.getValue();
                SbRotation oldRot=object[1]->orientation.getValue();
                float x,y,z,w; oldRot.getValue(x,y,z,w);
                object[1]->positionOffset.disconnect();
                object[1]->controlMode.set1Value(0,controlModeList[j]);
                object[1]->controlMode.set1Value(1,controlModeList[j+1]);
                controlModeList.remove(j*2);
                controlModeList.remove(j*2);
                char cmd1[255],cmd2[255];
                sprintf(cmd1,"%s: setPosition %f %f %f 0",object[1]->id.getValue().getString(),oldPos[0],oldPos[1],oldPos[2]);
                sprintf(cmd2,"%s: setOrientationQ %f %f %f %f 0",object[1]->id.getValue().getString(),x,y,z,w);
                addCompoundCommand(cmd1,cmd2,NULL);
                break;
            }
        }

        if (isCommandType("pickup")){
            int hotspotNum=-1;
            int marker1,marker2;
            if ((marker1=str[0].find(".h("))==str[0].npos ||
                (marker2=str[0].find(")",marker1+3))==str[0].npos ||
                !(object[0]->getHotspotNum(str[0].substr(marker1+3,marker2-marker1-3).c_str(),hotspotNum))
               ){
                if (object[0]->pickupHotspot.getNum()) object[0]->getHotspotNum((object[0])->pickupHotspot[0].getString(),hotspotNum);
            }

            if (hotspotNum>=0){
                SoSelectOne *selectorCoord=new SoSelectOne(SoMFVec3f::getClassTypeId());
                SoSelectOne *selectorRot=new SoSelectOne(SoMFRotation::getClassTypeId());
                selectorCoord->index=hotspotNum;
                selectorRot->index=hotspotNum;
                selectorCoord->input->connectFrom(&object[0]->hotspotAbsCoord);
                object[1]->positionOffset.connectFrom(selectorCoord->output);
                selectorRot->input->connectFrom(&object[0]->hotspotAbsRot);
                object[1]->orientationOffset.connectFrom(selectorRot->output);
                hotspotSelectorList.append(selectorCoord);
                hotspotSelectorList.append(selectorRot);
            }
            else {
                object[1]->positionOffset.connectFrom(&object[0]->position);
                object[1]->orientationOffset.connectFrom(&object[0]->orientation);
                hotspotSelectorList.append(NULL);
                hotspotSelectorList.append(NULL);                
            }
            hotspotInfoList.append(new SbString(searchString));
            controlModeList.append(object[1]->controlMode[0]);
            controlModeList.append(object[1]->controlMode[1]);
            object[1]->controlMode=SoPuppeteerKit::EXTERNAL;
        }

        return true;
    }

    // no matching command found
    else {
        printf("CHOREOGRAPHER: execution error: choreographer command '%s' not found\n",currentType.c_str());
        return false;
    }
}

bool SoChoreographerKit::choreograph()
{
#ifdef DEBUG_ARPUPPET
    printf("CHOREOGRAPHER: choreographing command\n");
#endif
    if (isCommandType("goNear")){
        // ...
    }
    // insert intonation tags for a TTS engine (currently only MS SAPI 5.x compliant engines are supported)
    else if (isCommandType("speak")){
        const char* emotionList[6]={"neutral", "happiness", "sadness", "anger", "surprise", "fear"};
        const int rate[6]={0,1,-2,1,2,2};
        const int volume[6]={0,2,-2,2,3,-1};
        const int pitch[6]={0,2,-2,-1,2,3};

        // compose new command string
        currentCommand="speak `";
        int emotion=-1;
        for (int i=0;i<6;i++){
            if (!strcmp(emotionList[i],getStringParam(1))) {
                emotion=i;
                break;
            }
        }        
        if (emotion>=0){
            float intensity=getIntParam(0)/100.0f;
            char *buffer=new char[255];
		    sprintf(buffer,"<rate speed='%d'><volume level='%d'><pitch middle='%d'> ",
		        (int)(-2.0f+rate[emotion]*sqrt(intensity)/5.0f),
			    (int)(80.0f+5.0f*volume[emotion]*sqrt(intensity)/5.0f),
			    (int)(0.0f+2.0f*pitch[emotion]*sqrt(intensity)/5.0f)
		    );
            currentCommand+=buffer;
        }
        currentCommand+=getStringParam(0);
        if (emotion>=0) currentCommand+=" </pitch></volume></rate>`";
        else currentCommand+="`";

        return false; // return false because it's not a compound command
    }

    return false;
}
