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
/* SoCommandParserKit.h contains the implementation of SoCommandParserKit, 
 * the general command parser class for puppeteers and utilities
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoCommandParserKit.cxx 3689 2004-09-10 14:02:12Z bara $
 * @file
 */
/* ======================================================================== */

#undef DEBUG_ARPUPPET

// general C++ includes
#include <ctype.h>

// Inventor classes
#include <Inventor/actions/SoSearchAction.h>

// own classes
#include "SoCommandParserKit.h"

SO_KIT_SOURCE(SoCommandParserKit);

void SoCommandParserKit::initClass()
{
    SO_KIT_INIT_CLASS(SoCommandParserKit, SoBaseKit, "BaseKit");
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: SoCommandParserKit initialized\n");
#endif
}

SoCommandParserKit::SoCommandParserKit()
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: constructor\n");
#endif

    SO_KIT_CONSTRUCTOR(SoCommandParserKit);

    // public fields
    SO_KIT_ADD_FIELD(id, (""));
    SO_KIT_ADD_FIELD(commandList, (""));
    SO_KIT_ADD_FIELD(commandFormatList, (""));
    SO_KIT_ADD_FIELD(command, (""));
    SO_KIT_ADD_FIELD(response, (""));
    SO_KIT_ADD_FIELD(responseIndex, (-1));
    SO_KIT_ADD_FIELD(dataNeedsValidation, (FALSE));
    SO_KIT_ADD_FIELD(dataValid, ());
    SO_KIT_ADD_FIELD(purgeQueue, ());
    SO_KIT_ADD_FIELD(synchronize, (TRUE)); // synchronous mode by default
    SO_KIT_ADD_FIELD(timerInterval, (SbTime(1.0f/30.0f)));
    SO_KIT_ADD_FIELD(finishedSubCommand,());
    SO_KIT_ADD_FIELD(finishedSubCommandData,(""));
    SO_KIT_ADD_FIELD(finishedCommand, ());
    SO_KIT_ADD_FIELD(finishedCommandData, (""));
    SO_KIT_ADD_FIELD(finishedAll, ());
    // private fields
    SO_KIT_ADD_FIELD(globalTime, (SbTime(0,0)));
    
    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    subCommandQueue=NULL;
    durationNum=0; triggerNum=0;

    setInternalCmds=false;

    goIdle=true;

    idSensor=new SoFieldSensor(idCB,this);
    commandSensor=new SoFieldSensor(commandCB,this);
    dataValidSensor=new SoFieldSensor(dataValidCB,this);
    purgeQueueSensor=new SoFieldSensor(purgeQueueCB,this);
    timerIntervalSensor=new SoFieldSensor(timerIntervalCB,this);
    
    // command loop timer -> time sensor: 1/30sec, start immediately
    timerSensor=new SoTimerSensor(timerSensorCB,this);

    // connect global time field
    globalTime.connectFrom(SoDB::getGlobalField("realTime"));
    
    setUpConnections(TRUE,TRUE);
}

SoCommandParserKit::~SoCommandParserKit()
{
    delete idSensor;
    delete commandSensor;
    delete dataValidSensor;
    delete purgeQueueSensor;
    delete timerIntervalSensor;
    delete timerSensor;
}

SbBool SoCommandParserKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: setUpConnections '%s' '%s' for SoCommandParserKit '%s'\n",
        onOff ? "TRUE" : "FALSE",doItAlways ? "TRUE" : "FALSE",id.getValue().getString());
#endif
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        idSensor->attach(&id);
        commandSensor->attach(&command);
        dataValidSensor->attach(&dataValid);
        purgeQueueSensor->attach(&purgeQueue);
        timerIntervalSensor->attach(&timerInterval);   

        setCommandList();
        constructTrigger();
        commandChanged();

        if (!timerSensor->isScheduled()) {
            if (timerInterval.getValue()!=SbTime()){
                timerSensor->setInterval(timerInterval.getValue());
                timerSensor->schedule();
            }
        }
    }
    else {
        if (timerSensor->isScheduled()) timerSensor->unschedule();

        idSensor->detach();
        commandSensor->detach();
        dataValidSensor->detach();
        purgeQueueSensor->detach();
        timerIntervalSensor->detach();
    }
    return !(connectionsSetUp = onOff);
}

void SoCommandParserKit::setCommandList()
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: set command list for '%s'\n",id.getValue().getString());
#endif
    if (!commandList.getNum() || (commandList.getNum()==1 && commandList[0]=="")) setInternalCmds=true;
    if (!setInternalCmds) return;
    
    // reset command and command format list
    commandList.deleteValues(0);
    commandFormatList.deleteValues(0);

    int marker1=0,marker2=0,marker3=0;
    if ((marker1=commandString.find_first_not_of(" "))==commandString.npos){
#ifdef DEBUG_ARPUPPET
        printf("COMMANDPARSER: warning: no commands found in '%s'\n",id.getValue().getString());
#endif
        return;
    }
    
    do {
        // try find the beginning of NEXT command format entry
        bool parity=false;
        marker2++;
        while (
            marker2<commandString.length() && 
            !(isalpha(commandString[marker2]) && (commandString[marker2-1]==' ' && !parity))
        ){
            if (commandString[marker2]=='(') parity=true;
            else if (commandString[marker2]==')') parity=false;
            marker2++;
        }

        // extract command name at current pointer
        if ((marker3=commandString.find(" ",marker1))==commandString.npos) marker3=commandString.length();
        commandList.set1Value(commandList.getNum(),commandString.substr(marker1,marker3-marker1).c_str());
#ifdef DEBUG_ARPUPPET
        printf("COMMANDPARSER: '%s' '%s'\n",id.getValue().getString(),commandString.substr(marker1,marker3-marker1).c_str());
#endif

        
        // extract command format string at current pointer if exists
        if (marker3<commandString.length()) marker3=commandString.find_first_not_of(" ",marker3+1);
        if (marker3!=commandString.length() && marker3!=commandString.npos){
            for (marker1=marker2-1;marker1>marker3 && commandString[marker1]==' ';marker1--);
            if (marker1>marker3) {
                commandFormatList.set1Value(commandList.getNum()-1,commandString.substr(marker3,marker1-marker3+1).c_str());
#ifdef DEBUG_ARPUPPET
                printf("COMMANDPARSER: '%s' '%s'\n",id.getValue().getString(),commandString.substr(marker3,marker1-marker3+1).c_str());
#endif
            }
            marker1=marker2;
        }
    } while (marker2<commandString.length());
}

void SoCommandParserKit::commandLoop()
{
    // run routines that are needed to be executed in each command loop
    commandLoopFunction();

    if (subCommandQueue==NULL) { // no command is being executed
        if (commandQueue.getLength()>0){ // check if there are new commands
            subCommandQueue=(SbPList*)(commandQueue[0]);
#ifdef DEBUG_ARPUPPET
            printf("COMMANDPARSER: %d command(s) in the subcommand queue for '%s'\n",subCommandQueue->getLength(),id.getValue().getString());
#endif
            // reset variables for command parsing and execution
            resetCommandProcess();
            // reset variables before processing next subcommand
            resetSubCommandProcess();

            // process commands
            for(int i=0;i<subCommandQueue->getLength();i++) {
                SbStringList *compoundCommandQueue=(SbStringList*)((*subCommandQueue)[i]);
                currentCommand=((*compoundCommandQueue)[0])->getString();
                currentQueueIndex=i;
                processCommand(i);
                evaluateResponse();
            }

            // if we are in asynchronous mode, we can proceed with the next item in the command queue
            if (!synchronize.getValue()){
                // reset subCommandQueue
                delete subCommandQueue;
                subCommandQueue=NULL;
                commandQueue.remove(0);
            }
        }
        else { // there are no user-issued commands, we are idle if allowed by the last executed command
            if (goIdle) idle();
            idleEntered=true; // indicate that we have already entered the idle state
        }
    }
    else {
        if (!(durationNum+triggerNum)) {
#ifdef DEBUG_ARPUPPET
            printf("COMMANDPARSER: command finished for '%s'\n",id.getValue().getString());
#endif
            if (synchronize.getValue()){
                // reset subCommandQueue
                delete subCommandQueue;
                subCommandQueue=NULL;
                commandQueue.remove(0);
            }

            // validate the data of fields 'finishedCommandData' and 'response' by firing 'finishedCommand'
            finishedCommand.setValue();
            if (!commandQueue.getLength()) {
#ifdef DEBUG_ARPUPPET
                printf("COMMANDPARSER: all commands finished for '%s'\n",id.getValue().getString());
#endif
                finishedAll.setValue();
            }
        }
        else {
#ifdef DEBUG_ARPUPPET
//            dumpMessageQueue();
#endif
            // check whether there is a duration command that has already finished
            SbTime currentTime=globalTime.getValue();
            int i;
            for (i=0;i<durationTimeList.getLength();i++){
                if (*((SbTime*)(durationTimeList[i]))<currentTime){
                    // decrease pending duration command counter
                    if (durationNum>0) durationNum--;
                    // process pending duration command lists + subcommand queue
                    subCommandFinished(durationCommandList[i]->getString());
                    durationTimeList.remove(i);
                    durationCommandList.remove(i);
                    break;
                }
            }
        }
    }
}

void SoCommandParserKit::dumpMessageQueue()
{
    printf("COMMANDPARSER: **************** command queue dump ***************\n");
    for (int i=0;i<commandQueue.getLength();i++){
        SbPList *subCmdQ=(SbPList*)(commandQueue[0]);
        for (int j=0;j<subCmdQ->getLength();j++) {
            SbStringList *compCmdQ=(SbStringList*)((*subCmdQ)[j]);
            for (int k=0;k<compCmdQ->getLength();k++) {
                printf("%s",((*compCmdQ)[k])->getString());
            }
            printf("\n");
        }
        printf("===========\n");
    }
    printf("COMMANDPARSER: **************** command queue end ****************\n");
}

void SoCommandParserKit::commandTriggered(const char *cmd)
{
    // process subcommand queue
    subCommandFinished(cmd);

    // decrease pending trigger command counter
    if (triggerNum) triggerNum--;
}

void SoCommandParserKit::subCommandFinished(const char *cmd)
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: subCommandFinished: '%s' -> '%s'\n", id.getValue().getString(),cmd);
#endif
    // if we are in asynchronous mode, do not process subcommands
    if (!synchronize.getValue()){
        printf("COMMANDPARSER: finishedCmd %s: -> '%s'\n", id.getValue().getString(),cmd);
        finishedCommandData.setValue(cmd);
        string str=id.getValue().getString();
        str+=": ";
        str+=cmd;
        finishedSubCommandData.setValue(str.c_str());
        finishedSubCommand.setValue();
        return;
    }

    // return if we have an invalid subcommand queue 
    if (subCommandQueue==NULL) return;

    for (int i=0;i<subCommandQueue->getLength();i++){
        SbStringList *subList=(SbStringList*)((*(subCommandQueue))[i]);
        if (subList->getLength()==1) {
            if (processFinishedCommand(((*subList)[0])->getString(),cmd,i)){
#ifdef DEBUG_ARPUPPET
                printf("COMMANDPARSER: subcommand '%s' finished\n",((*subList)[0])->getString());
#endif
                finishedCommandData.set1Value(i,*((*subList)[0]));
                // give feedback
                string str=id.getValue().getString();
                str+=": ";
                str+=((*subList)[0])->getString();    
                finishedSubCommandData.setValue(str.c_str());
                finishedSubCommand.setValue();
                subList->remove(0);
#ifdef DEBUG_ARPUPPET
                printf("COMMANDPARSER: subcommand finished: '%s'\n",str.c_str());
#endif
                break;
            }
        }
        else if (subList->getLength()>1) {
            if (processFinishedCommand(((*subList)[1])->getString(),cmd,i)){
#ifdef DEBUG_ARPUPPET
                printf("COMMANDPARSER: subcommand '%s' finished\n",((*subList)[1])->getString());
#endif
                subList->remove(1);
                // if there are component commands left
                if (subList->getLength()>1){
                    currentQueueIndex=i;
                    currentCommand=((*subList)[1])->getString();
                    resetSubCommandProcess();
                    processCommand(i);
                    evaluateResponse();
                }
                else {
                    finishedCommandData.set1Value(i,*((*subList)[0]));
                    // give feedback
                    string str=id.getValue().getString();
                    str+=": ";
                    str+=((*subList)[0])->getString();    
                    finishedSubCommandData.setValue(str.c_str());
                    finishedSubCommand.setValue();
                    subList->remove(0);
#ifdef DEBUG_ARPUPPET
                    printf("COMMANDPARSER: subcommand finished: '%s'\n",str.c_str());
#endif
                }
                break;
            }
        }
    }
}

void SoCommandParserKit::resetCommandProcess()
{
    // reset output field values
    finishedCommandData.deleteValues(0);
    response.deleteValues(0);

    // empty response and responseIndex list
    removeAllListItems(&responseList);
    removeAllListItems(&responseIndexList);
}

void SoCommandParserKit::resetSubCommandProcess()
{
    // default implementation is empty
}

void SoCommandParserKit::processCommand(int index)
{
    // remove previous parsing results
    removeAllListItems(&strParamList);
    removeAllListItems(&intParamList);
    removeAllListItems(&floatParamList);

    // reset own response
    removeAllListItems(&subResponseList);

    if (parseCommandType(this) && parseCommand()){
#ifdef DEBUG_ARPUPPET
        printf("COMMANDPARSER: '%s' started executing command '%s'\n",
            id.getValue().getString(),currentCommand.c_str());
#endif
        // reset attribute enabling/disabling idle behavior
        goIdle=true;

        // execute command
        bool implemented=executeCommand();
#ifdef DEBUG_ARPUPPET
        if (!implemented) {
            printf("COMMANDPARSER: the component '%s' or some of its subcomponents could not execute command '%s'\n",
                id.getValue().getString(),
                currentCommand.c_str()
            );
        }
#endif
    }
    else finishedCommandData.set1Value(index,currentCommand.c_str());
}

bool SoCommandParserKit::processFinishedCommand(const char *current,const char *finished,int currentIndex)
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: processFinishedCommand '%s' -> current: '%s', finished: '%s'\n",
        id.getValue().getString(),current,finished);
#endif
    return (
        !strcmp(current,finished) ||                // obvious case
        translateFinishedCommand(current,finished)  // customized comparison with the currently executed command 
                                                    // if object returns specific finishedCommand messages
    );
}

void SoCommandParserKit::addCompoundCommand(const char *cmd,...)
{
    if (!synchronize.getValue()){
        printf("COMMANDPARSER: you cannot add compound commands in asynchronous mode (set the 'synchronize' field to TRUE)\n");
        return;
    }

    SbStringList *compoundCommandQueue=(SbStringList*)((*subCommandQueue)[currentQueueIndex]);

    int count=0;
    SbString *cmdString;
    bool appendCmd=!(compoundCommandQueue->getLength()>1);
    if (!appendCmd) compoundCommandQueue->remove(1);
    
    va_list argList;
    va_start(argList,cmd);
    if (cmd!=NULL) {
        cmdString=new SbString(cmd);
        do {
            if (appendCmd) compoundCommandQueue->append(cmdString);
            else compoundCommandQueue->insert(cmdString,1+count);
            count++;
            char *str=va_arg(argList,char*);
            if (str!=NULL) cmdString=new SbString(str);
            else cmdString=NULL;
        } while (cmdString!=NULL);
    }
    va_end(argList);

#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: compound command got unpacked into %d commands and started\n",count);
#endif
    currentCommand=((*compoundCommandQueue)[1])->getString();
    processCommand(currentQueueIndex);
    evaluateResponse();
}

void SoCommandParserKit::addCompoundCommand(vector<string> *cmdList)
{
    if (!synchronize.getValue()){
        printf("COMMANDPARSER: you cannot add compound commands in asynchronous mode (set the 'synchronize' field to TRUE)\n");
        return;
    }

    SbStringList *compoundCommandQueue=(SbStringList*)((*subCommandQueue)[currentQueueIndex]);

    int count=0;
    SbString *cmdString;
    bool appendCmd=!(compoundCommandQueue->getLength()>1);
    if (!appendCmd) compoundCommandQueue->remove(1);
    
    for (int i=0;i<cmdList->size();i++){
        if ((*cmdList)[i].length()){
            cmdString=new SbString((*cmdList)[i].c_str());
            if (appendCmd) compoundCommandQueue->append(cmdString);
            else compoundCommandQueue->insert(cmdString,1+count);
            count++;
        }
        else break;
    }

#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: compound command got unpacked into %d commands and started\n",count);
#endif
    currentCommand=((*compoundCommandQueue)[1])->getString();
    processCommand(currentQueueIndex);
    evaluateResponse();
}

bool SoCommandParserKit::parseCommandType(SoCommandParserKit *commandParserKit)
{
    if (commandParserKit==NULL) {
#ifdef DEBUG_ARPUPPET
        printf("COMMANDPARSER: invalid commandParserKit to search for command type & format\n");
#endif
        return false;
    }

#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: command to parse: '%s'\n",currentCommand.c_str());
#endif

    int startPosCmd=0,endPosCmd=0;
    
    // return if command is empty
    if ((startPosCmd=currentCommand.find_first_not_of(" "))==currentCommand.npos) return false;
    
    if ((endPosCmd=currentCommand.find(" ",startPosCmd))==currentCommand.npos) endPosCmd=currentCommand.length();
    currentType=currentCommand.substr(startPosCmd,endPosCmd);

    int i;
    for (i=0;i<commandParserKit->commandList.getNum();i++){
        if (commandParserKit->commandList[i]==currentType.c_str()) {
            currentFormat=commandParserKit->commandFormatList[i].getString();
            break;
        }
    }
    if (i==commandParserKit->commandList.getNum()){
        printf("COMMANDPARSER: warning: command type '%s' not found in '%s'\n",currentType.c_str(),commandParserKit->id.getValue().getString());
        return false;
    }
#ifdef DEBUG_ARPUPPET
    else printf("COMMANDPARSER: command type '%s' found in '%s'\n",currentType.c_str(),commandParserKit->id.getValue().getString());
#endif

    return true;
}
 
bool SoCommandParserKit::parseCommand()
{   
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: parsing cmd '%s %s'...\n",
        currentType.c_str(),currentFormat.c_str());
#endif
    
    int formatPos=-1,paramPos=0,paramPosNext=0,paramStartPos=0,paramEndPos=0,nextFormatPos=0;
    int intParam;
    float floatParam;
    float *floatValue;
    int result;
    string resultStr;
    string defaultValue;
    SbString *appendStr=NULL;
    
    if ((paramPosNext=currentCommand.find_first_not_of(" "))!=currentCommand.npos){
        if ((paramPosNext=currentCommand.find(" ",paramPosNext))==currentCommand.npos) paramPosNext=currentCommand.length();
    }
    else return false;

    while ((formatPos=currentFormat.find("%",formatPos+1))!=currentFormat.npos) {
        // check whether the type indicator character exists in the command format
        if (formatPos+1>currentFormat.length()){
            printf("COMMANDPARSER: cmd '%s': param type missing in cmd format '%s %s'\n",
                currentType.c_str(),currentFormat.c_str());
            return false;
        }
        
        // check for default value in command format
        defaultValue="";
        if (formatPos+2<currentFormat.length()){
            if ((nextFormatPos=currentFormat.find("%",formatPos+2))==currentFormat.npos) nextFormatPos=currentFormat.length();
            if ((paramStartPos=currentFormat.find("(",formatPos+2))!=currentFormat.npos && paramStartPos<nextFormatPos){
                if ((paramEndPos=currentFormat.find(")",paramStartPos+1))!=currentFormat.npos && paramEndPos<nextFormatPos){
                    defaultValue=currentFormat.substr(paramStartPos+1,paramEndPos-paramStartPos-1);
                }
                else printf("COMMANDPARSER: error in default value in cmd format '%s %s'\n",
                    currentType.c_str(),currentFormat.c_str());
            }
        }

        // parse command string based the type and optional default value parsed in the command format
        paramPos=currentCommand.find_first_not_of(" ",paramPosNext+1);
        switch(currentFormat[formatPos+1]) {

            case 's':
                if (paramPos==currentCommand.npos){
                    appendStr=new SbString(defaultValue.c_str());
                    paramEndPos=paramPosNext+1;
                    printf("COMMANDPARSER: cmd '%s': str param missing, default value '%s' used\n",
                        currentCommand.c_str(),defaultValue.c_str());
                }
                else {
                    paramStartPos=paramPos-1;
                    do {
                        if ((paramStartPos=currentCommand.find("`",paramStartPos+1))==currentCommand.npos){
                            printf("COMMANDPARSER: parse error in cmd '%s': str param or terminator ` missing\n",
                                currentCommand.c_str());
                            return false;
                        }
                    } while (currentCommand[paramStartPos-1]=='\\');
                    paramEndPos=paramStartPos;
                    do {
                        if ((paramEndPos=currentCommand.find("`",paramEndPos+1))==currentCommand.npos) {
                            printf("COMMANDPARSER: parse error in cmd '%s': str end terminator ` missing\n",
                                currentCommand.c_str());
                            return false;
                        }
                    } while (currentCommand[paramEndPos-1]=='\\');
                    resultStr=currentCommand.substr(paramStartPos+1,paramEndPos-paramStartPos-1);
                    appendStr=new SbString(resultStr.c_str());
#ifdef DEBUG_ARPUPPET
                    printf("COMMANDPARSER: parsed string '%s'\n",appendStr->getString());
#endif
                }
                strParamList.append(appendStr);
                if ((paramPosNext=currentCommand.find(" ",paramEndPos+1))==currentCommand.npos) paramPosNext=currentCommand.length();
                break;

            case 'd':
                if (paramPos==currentCommand.npos){
                    if (!defaultValue.length()) intParam=0;
                    else if ((result=sscanf(defaultValue.c_str(),"%d",&intParam))!=1){
                        printf("COMMANDPARSER: error in int default value in cmd format '%s %s'\n",
                            currentType.c_str(),currentFormat.c_str());
                        intParam=0;
                    }
                    printf("COMMANDPARSER: cmd '%s': int param missing, default value %d used\n",
                        currentCommand.c_str(),intParam);
                }
                else {
                    if ((paramPosNext=currentCommand.find(" ",paramPos+1))==currentCommand.npos) paramPosNext=currentCommand.length();
                    result=sscanf(currentCommand.substr(paramPos,paramPosNext-paramPos).c_str(),"%d",&intParam);
                    if (result!=1) {
                        printf("COMMANDPARSER: int parsing error in cmd '%s'\n",
                            currentCommand.c_str());
                        return false;
                    }
                    else {
#ifdef DEBUG_ARPUPPET
                        printf("COMMANDPARSER: parsed integer %d\n",intParam);
#endif
                    }
                }
                intParamList.append(intParam);
                break;
                
            case 'f':
                if (paramPos==currentCommand.npos){
                    if (!defaultValue.length()) floatParam=0.0f;
                    else if ((result=sscanf(defaultValue.c_str(),"%f",&floatParam))!=1){
                        printf("COMMANDPARSER: error in float default value in cmd format '%s %s'\n",
                            currentType.c_str(),currentFormat.c_str());
                        floatParam=0.0f;
                    }
                    printf("COMMANDPARSER: cmd '%s': float param missing, default value %f used\n",
                        currentCommand.c_str(),floatParam);
                }
                else{
                    if ((paramPosNext=currentCommand.find(" ",paramPos+1))==currentCommand.npos) paramPosNext=currentCommand.length();
                    result=sscanf(currentCommand.substr(paramPos,paramPosNext-paramPos).c_str(),"%f",&floatParam);
                    if (result!=1) {
                        printf("COMMANDPARSER: float parsing error in cmd '%s'\n",
                            currentCommand.c_str());
                        return false;
                    }
                    else {
#ifdef DEBUG_ARPUPPET
                        printf("COMMANDPARSER: parsed float %f\n",floatParam);
#endif
                    }
                }
                floatValue=new float;
                *floatValue=floatParam;
                floatParamList.append(floatValue);
                break;
            default:
                printf("COMMANDPARSER: unknown param type '%c' in cmd format '%s %s'\n",
                    currentFormat[formatPos+1],currentType.c_str(),currentFormat.c_str());
                return false;
                break;
        } // end of switch

    } // end of while
    
    return true;
}

void SoCommandParserKit::addDurationMonitor(int duration)
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: duration monitor added with %d ms\n",duration);
#endif
    SbTime *time=new SbTime();
    *time=globalTime.getValue()+getTimeValue(duration);
    durationTimeList.append(time);
    SbString *str=new SbString(currentCommand.c_str());
    durationCommandList.append(str);
    durationNum++;
}

void SoCommandParserKit::addTriggerMonitor()
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: trigger monitor added\n");
#endif
    triggerNum++;
}

void SoCommandParserKit::noTrigger()
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: no trigger set for command\n");
#endif
    addDurationMonitor();
}

void SoCommandParserKit::addResponse(const char *str)
{
    SbString *s=new SbString(str);
    subResponseList.append(s);
}

const char* SoCommandParserKit::getStringParam(int i)
{
    if (i>=strParamList.getLength() || i<0) {
        printf("COMMANDPARSER: stringParam %d doesn't exist: using default value\n",i);
        return "";
    }
    return strParamList[i]->getString();
}

int SoCommandParserKit::getIntParam(int i)
{
    if (i>=intParamList.getLength() || i<0) {
        printf("COMMANDPARSER: intParam %d doesn't exist: using default value\n",i);
        return 0;
    }
    return intParamList[i];
}

float SoCommandParserKit::getFloatParam(int i)
{
    if (i>=floatParamList.getLength() || i<0) {
        printf("COMMANDPARSER: floatParam %d doesn't exist: using default value\n",i);
        return 0.0f;
    }
    return *((float*)(floatParamList[i]));
}

SbTime SoCommandParserKit::getTimeValue(int dur)
{
    int sec=dur/1000;
    return SbTime(sec,1000*(dur-1000*sec));
}

bool SoCommandParserKit::isCommandType(const char* cmd)
{
    return (!strcmp(currentType.c_str(),cmd)) ? true : false;
}

bool SoCommandParserKit::isType(SoNode *node,SbName name)
{
    SoSearchAction searchAction;
    searchAction.setInterest(SoSearchAction::FIRST);
    searchAction.setSearchingAll(TRUE); // search every node in the scene graph regardless of switch settings
    SoType type=SoType::fromName(name);
    if (type.isBad()) {
        foundNode=NULL;
        return false;
    }
    searchAction.setType(type,TRUE);
    searchAction.apply(node);
    SoPath *path=searchAction.getPath();
    if (path!=NULL) {
        foundNode=((SoFullPath*)path)->getTail();
        return true;
    }
    else {
        foundNode=NULL;
        return false;
    }
}

void SoCommandParserKit::removeAllListItems(SbPList *list)
{
    while (list->getLength()) list->remove(0);
}

void SoCommandParserKit::commandLoopFunction()
{
    // default implementation is empty
}

bool SoCommandParserKit::executeCommand()
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: execute command for SoCommandParserKit '%s'\n",id.getValue().getString());
#endif
    noTrigger();
    return false;
}

void SoCommandParserKit::idle()
{
    // default implementation is empty
}

void SoCommandParserKit::evaluateResponse()
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: evaluate response for '%s'\n",id.getValue().getString());
#endif
    int i;

    // set own response
    for (i=0;i<subResponseList.getLength();i++){
        SbString *str=new SbString(*(subResponseList[i]));
        responseList.append(str);
        responseIndexList.append(currentQueueIndex);
    }

    // reset response and responseIndex
    response.deleteValues(0);
    responseIndex.deleteValues(0);

    // set current response
    for (i=0;i<responseList.getLength();i++) {
        if (*(responseList[i])!=SbString()){
            response.set1Value(response.getNum(),*(responseList[i]));
            responseIndex.set1Value(responseIndex.getNum(),responseIndexList[i]);
        }
    }
}

void SoCommandParserKit::constructTrigger()
{
    // default implementation is empty
}

bool SoCommandParserKit::translateFinishedCommand(const char *current,const char *finished)
{
    return false;
}

void SoCommandParserKit::idCB(void *data,SoSensor *sensor)
{
    SoCommandParserKit *commandParserKit=(SoCommandParserKit*)data;
    commandParserKit->idChanged();
}

void SoCommandParserKit::commandCB(void *data,SoSensor *sensor)
{
    SoCommandParserKit *commandParserKit=(SoCommandParserKit*)data;
    commandParserKit->commandChanged();
}

void SoCommandParserKit::dataValidCB(void *data,SoSensor *sensor)
{
    SoCommandParserKit *commandParserKit=(SoCommandParserKit*)data;
    commandParserKit->dataValidChanged();
}

void SoCommandParserKit::idChanged()
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: id changed to '%s'\n",id.getValue());
#endif
}

void SoCommandParserKit::purgeQueueCB(void *data,SoSensor *sensor)
{
    SoCommandParserKit *commandParserKit=(SoCommandParserKit*)data;
    commandParserKit->removeAllListItems(&(commandParserKit->commandQueue));
    commandParserKit->finishedSubCommandData.setValue("");
    commandParserKit->finishedSubCommand.setValue();
    commandParserKit->finishedCommandData.deleteValues(0);
    commandParserKit->finishedCommand.setValue();
    commandParserKit->finishedAll.setValue();
#ifdef DEBUG_ARPUPPET
     printf("COMMANDPARSER: command finished for '%s'\n",commandParserKit->id.getValue().getString());
     printf("COMMANDPARSER: all commands finished for '%s'\n",commandParserKit->id.getValue().getString());
#endif
}

void SoCommandParserKit::timerIntervalCB(void *data, SoSensor *sensor)
{    
    SoCommandParserKit *commandParserKit=(SoCommandParserKit*)data;

    if (commandParserKit->timerSensor->isScheduled()) commandParserKit->timerSensor->unschedule();
    if (commandParserKit->timerInterval.getValue()!=SbTime()){
        commandParserKit->timerSensor->setInterval(commandParserKit->timerInterval.getValue());
        commandParserKit->timerSensor->schedule();
    }
}

void SoCommandParserKit::timerSensorCB(void *data, SoSensor *sensor)
{    
    SoCommandParserKit *commandParserKit=(SoCommandParserKit*)data;
    commandParserKit->commandLoop();
}

void SoCommandParserKit::commandChanged()
{
#ifdef DEBUG_ARPUPPET
    //printf("COMMANDPARSER: command changed\n");
#endif
    if (!dataNeedsValidation.getValue()) {
        // reset "we have just entered the idle state" state variable to prepare for next idle state
        idleEntered=false;

        // add commands to commandQueue
        int num=command.getNum();
        SbPList *list=new SbPList;
        if (num>0) {
#ifdef DEBUG_ARPUPPET
            printf("COMMANDPARSER: process %d command(s) for puppeteer '%s'\n",command.getNum(),id.getValue().getString());
#endif
            for (int i=0;i<num;i++){
                if (command[i]=="") continue;
                SbStringList *subList=new SbStringList;
                SbString *str=new SbString(command[i]);
                subList->append(str);
                list->append(subList);
            }
            if (list->getLength()) commandQueue.append(list);
        }
    }
}

void SoCommandParserKit::dataValidChanged()
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: dataValid changed\n",id.getValue());
#endif
    if (dataNeedsValidation.getValue()) {
        // reset "we have just entered the idle state" state variable to prepare for next idle state
        idleEntered=false;

        // add commands to commandQueue
        int num=command.getNum();
        SbPList *list=new SbPList;
        if (num) {
#ifdef DEBUG_ARPUPPET
            printf("COMMANDPARSER: process %d command(s) for puppeteer '%s'\n",command.getNum(),id.getValue().getString());
#endif
            for (int i=0;i<num;i++){
                if (command[i]=="") continue;
                SbStringList *subList=new SbStringList;
                SbString *str=new SbString(command[i]);
                subList->append(str);
                list->append(subList);
            }
            commandQueue.append(list);
        }
    }
}

void SoCommandParserKit::appendCommand(const char *cmd,...)
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: appendCommand\n");
#endif
    SbPList *list=new SbPList;

    SbString *cmdString;
    va_list argList;
    va_start(argList,cmd);
    if (cmd!=NULL) {
        cmdString=new SbString(cmd);
        do {
            SbStringList *subList=new SbStringList;
            subList->append(cmdString);
            list->append(subList);
            char *str=va_arg(argList,char*);
            if (str!=NULL) cmdString=new SbString(str);
            else cmdString=NULL;
        } while (cmdString!=NULL);
    }
    va_end(argList);
    commandQueue.append(list);
}

void SoCommandParserKit::appendCommand(vector<string> *cmdList)
{
#ifdef DEBUG_ARPUPPET
    printf("COMMANDPARSER: appendCommand\n");
#endif
    SbPList *list=new SbPList;

    SbString *cmdString;
    for (int i=0;i<cmdList->size();i++){
        if ((*cmdList)[i].length()){
            SbStringList *subList=new SbStringList;
            cmdString=new SbString((*cmdList)[i].c_str());
            subList->append(cmdString);
            list->append(subList);
        }
        else break;
    }
    commandQueue.append(list);
}
