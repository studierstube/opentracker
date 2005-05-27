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
/* Director.cxx contains the implementation of Director
 *
 * @author Istvan Barakonyi
 *
 * $Id: Director.cxx 4127 2005-01-20 14:03:28Z bara $ 
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// own classes
#include "Choreographer.h"
#include "Director.h"

//#define ARLEGO_DEBUG

SO_KIT_SOURCE(Director);

void Director::initClass()
{
    SO_KIT_INIT_CLASS(Director, SoBaseKit, "BaseKit");
}

Director::Director()
{
    SO_KIT_CONSTRUCTOR(Director);

    // fields effecting the overall construction
    SO_KIT_ADD_FIELD(constructionFile, (""));
    SO_KIT_ADD_FIELD(constructionStep, (0));
    SO_KIT_ADD_FIELD(maxConstructionStepIndex, (0));

    // LEGO model-related fields
    SO_KIT_ADD_FIELD(showModelPart, (-1));
    SO_KIT_ADD_FIELD(showAugmentation, (-1));
    SO_KIT_ADD_FIELD(usePresenter, (FALSE));
    SO_KIT_ADD_FIELD(displayMode, (SHOW_UP_TO_CURRENT));
    SO_KIT_ADD_FIELD(permanentPuppetNum, (0));
    SO_KIT_ADD_FIELD(permanentAttributeNum, (0));

    // minifig behavior-related fields
    SO_KIT_ADD_FIELD(state, (PUC_DEFAULT));
    SO_KIT_ADD_FIELD(minifigEvent, (Choreographer::NONE));
    SO_KIT_ADD_FIELD(command, (""));
    SO_KIT_ADD_FIELD(finishedCommandData, (""));
    SO_KIT_ADD_FIELD(showAgainStep, ());
    SO_KIT_ADD_FIELD(enableEvents, (TRUE));
    SO_KIT_ADD_FIELD(eventTimeOut, (SbTime(3.0f)));
    SO_KIT_ADD_FIELD(explanation, (""));

    // RCX properties
    SO_KIT_ADD_FIELD(RCX_engineA,(FALSE));
    SO_KIT_ADD_FIELD(RCX_engineB,(FALSE));
    SO_KIT_ADD_FIELD(RCX_engineC,(FALSE));
    SO_KIT_ADD_FIELD(RCX_engineDirA,(FALSE));
    SO_KIT_ADD_FIELD(RCX_engineDirB,(FALSE));
    SO_KIT_ADD_FIELD(RCX_engineDirC,(FALSE));
    SO_KIT_ADD_FIELD(RCX_enginePowerA,(0));
    SO_KIT_ADD_FIELD(RCX_enginePowerB,(0));
    SO_KIT_ADD_FIELD(RCX_enginePowerC,(0));

    SO_KIT_DEFINE_ENUM_VALUE(DisplayMode,HIDE_ALL);
    SO_KIT_DEFINE_ENUM_VALUE(DisplayMode,SHOW_ALL);
    SO_KIT_DEFINE_ENUM_VALUE(DisplayMode,SHOW_CURRENT_ONLY);
    SO_KIT_DEFINE_ENUM_VALUE(DisplayMode,SHOW_UP_TO_CURRENT);
    SO_KIT_SET_SF_ENUM_TYPE(displayMode, DisplayMode);

    SO_KIT_INIT_INSTANCE();

    // init auxiliary variables
    tilePuppetNum=-1;
    currentAnimationHint=NULL;

    // create field sensors
    constructionFileSensor=new SoFieldSensor(constructionFileCB,this);
    constructionStepSensor=new SoFieldSensor(showCurrentStepCB,this);
    showAgainStepSensor=new SoFieldSensor(showCurrentStepCB,this);
    usePresenterSensor=new SoFieldSensor(showCurrentStepCB,this); // we use showAgainStepCB as the usePresenter callback function
    displayModeSensor=new SoFieldSensor(showCurrentStepCB,this);
    minifigEventSensor=new SoFieldSensor(minifigEventCB,this);
    finishedCommandDataSensor=new SoFieldSensor(finishedCommandDataCB,this);
    RCX_engineASensor=new SoFieldSensor(RCXcommandCB,this);
    RCX_engineBSensor=new SoFieldSensor(RCXcommandCB,this);
    RCX_engineCSensor=new SoFieldSensor(RCXcommandCB,this);
    RCX_engineDirASensor=new SoFieldSensor(RCXcommandCB,this);
    RCX_engineDirBSensor=new SoFieldSensor(RCXcommandCB,this);
    RCX_engineDirCSensor=new SoFieldSensor(RCXcommandCB,this);
    RCX_enginePowerASensor=new SoFieldSensor(RCXcommandCB,this);
    RCX_enginePowerBSensor=new SoFieldSensor(RCXcommandCB,this);
    RCX_enginePowerCSensor=new SoFieldSensor(RCXcommandCB,this);

    eventEnableTimer=new SoAlarmSensor(eventEnableCB,this);;

    setUpConnections(TRUE,TRUE);
}

Director::~Director()
{
    delete constructionFileSensor;
    delete constructionStepSensor;
    delete showAgainStepSensor;
    delete usePresenterSensor;
    delete displayModeSensor;
    delete minifigEventSensor;
    delete finishedCommandDataSensor;
    delete RCX_engineASensor;
    delete RCX_engineBSensor;
    delete RCX_engineCSensor;
    delete RCX_engineDirASensor;
    delete RCX_engineDirBSensor;
    delete RCX_engineDirCSensor;
    delete RCX_enginePowerASensor;
    delete RCX_enginePowerBSensor;
    delete RCX_enginePowerCSensor;
}

SbBool Director::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        constructionFileSensor->attach(&constructionFile);
        constructionStepSensor->attach(&constructionStep);
        showAgainStepSensor->attach(&showAgainStep);
        usePresenterSensor->attach(&usePresenter);
		displayModeSensor->attach(&displayMode);
        minifigEventSensor->attach(&minifigEvent);
        finishedCommandDataSensor->attach(&finishedCommandData);
        RCX_engineASensor->attach(&RCX_engineA);
        RCX_engineBSensor->attach(&RCX_engineB);
        RCX_engineCSensor->attach(&RCX_engineC);
        RCX_engineDirASensor->attach(&RCX_engineDirA);
        RCX_engineDirBSensor->attach(&RCX_engineDirB);
        RCX_engineDirCSensor->attach(&RCX_engineDirC);
        RCX_enginePowerASensor->attach(&RCX_enginePowerA);
        RCX_enginePowerBSensor->attach(&RCX_enginePowerB);
        RCX_enginePowerCSensor->attach(&RCX_enginePowerC);

        constructionFileCB(this,NULL);
        showCurrentStepCB(this,NULL);
    }
    else {
        constructionFileSensor->detach();
        constructionStepSensor->detach();
        showAgainStepSensor->detach();
        usePresenterSensor->detach();
        displayModeSensor->detach();
        minifigEventSensor->detach();
        finishedCommandDataSensor->detach();
        RCX_engineASensor->detach();
        RCX_engineBSensor->detach();
        RCX_engineCSensor->detach();
        RCX_engineDirASensor->detach();
        RCX_engineDirBSensor->detach();
        RCX_engineDirCSensor->detach();
        RCX_enginePowerASensor->detach();
        RCX_enginePowerBSensor->detach();
        RCX_enginePowerCSensor->detach();
    }

    return !(connectionsSetUp = onOff);
}

void Director::showCurrentStepCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;

    // if the user wants to show the step again and we've already showed the current tile, let's go back to the default state of the 
    // physical location where we were, otherwise don't do anything
    if (sensor==director->showAgainStepSensor){        
        switch (director->state.getValue()) {
            case Director::PUC_NOINTRO_FOR_RCX:
                director->state=Director::PUC_DEFAULT;
    	        break;
            case Director::RCX_NOINTRO:
                director->state=Director::RCX_DEFAULT;
    	        break;
        }
    }

    if (director->isConstructionStepValid()) {
        if (director->constructionStep.getValue()<director->animationHintList.getLength()){
            director->currentAnimationHint=director->animationHintList[director->constructionStep.getValue()];
        }
        else director->currentAnimationHint=NULL;
        director->displayExplanation();
        director->stateEngine(Choreographer::COMMAND);
    }
}

void Director::constructionFileCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;

    // reset lists
    while (director->modelPartList.getLength()) director->modelPartList.remove(0);
    while (director->augmentationList.getLength()) director->augmentationList.remove(0);
    while (director->explanationList.getLength()) director->explanationList.remove(0);

    // open construction file
    const char *filename=director->constructionFile.getValue().getString();
    if (!strlen(filename)) {
        printf("DIRECTOR: name of construction file is empty\n");
        return;
    }
    FILE *stream=fopen(filename,"r");
    if (!stream) {
        printf("DIRECTOR: couldn't open file '%s'\n",director->constructionFile.getValue().getString());
        director->maxConstructionStepIndex=-1;
        return;
    }

    // debug message
    printf("DIRECTOR: parsing construction file '%s' ... \n",director->constructionFile.getValue().getString());

    // read in data
    int i=0;
    char line[255];
    int stepNumber=0;
    int lineNumber=-1;
    SbIntList *modelPIndexList=NULL,*augIndexList=NULL;
    SbStringList *textList=NULL;
    SbString *animHintString=NULL;
    std::string parseString;
    while (fgets(line,256,stream)){      
        lineNumber++;        
        if (line[0]!='#'){
#ifdef ARLEGO_DEBUG
            printf("\t%d: %s",lineNumber,line);
#endif
            switch (i) {
                case 0: // model parts to show
                    modelPIndexList=new SbIntList;
                    parseString=line;
                    director->parseIntList(parseString,modelPIndexList,lineNumber);
                    break;
                case 1: // augmentation parts to show
                    augIndexList=new SbIntList;
                    parseString=line;
                    director->parseIntList(parseString,augIndexList,lineNumber);
                    break;
                case 2: // animation hint string
                    animHintString=new SbString(line);
                    break;
                case 3:
                    textList=new SbStringList;
                    textList->append(new SbString(line));
                    break;
                default: // 4,5 ... --> lines of the explanation text
                    textList->append(new SbString(line));
                    break;
            }
            i++;
        }
        else {
#ifdef ARLEGO_DEBUG
            printf("-----------------------\n");
#endif
            if (stepNumber){
                director->modelPartList.append(modelPIndexList);
                director->augmentationList.append(augIndexList);
                director->animationHintList.append(animHintString);
                director->explanationList.append(textList);
            }
            stepNumber++;
            i=0;
        }
    }
    director->modelPartList.append(modelPIndexList);
    director->augmentationList.append(augIndexList);
    director->animationHintList.append(animHintString);
    director->explanationList.append(textList);

    // close file
    fclose(stream);

    // set maximum model part index
    director->maxConstructionStepIndex=stepNumber-1;
}

void Director::minifigEventCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;

#ifdef ARLEGO_DEBUG
    printf("DIRECTOR: event %d arrived\n",director->minifigEvent.getValue());
#endif

    if (!director->enableEvents.getValue()) return;
    else {
        // disable event handling temporarily to avoid bouncing back and forth between states
        if (director->minifigEvent.getValue()!=Choreographer::NONE) director->disableEvents();
        // construction step index check for security
        if (director->isConstructionStepValid()) director->stateEngine(director->minifigEvent.getValue());
    }
}

void Director::finishedCommandDataCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;

    printf("DIRECTOR: finishedCommandData: ");
    if (director!=NULL && director->finishedCommandData.getNum()){
         for (int j=0;j<director->finishedCommandData.getNum();j++){
            if (j) printf("\t");
            printf("---> %s\n",director->finishedCommandData[j].getString());
         }
    }
    else printf("\n");

    if (director->isConstructionStepValid()) director->stateEngine(Choreographer::FINISHED_CMD);
}

void Director::eventEnableCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;
    director->enableEvents=TRUE;
}

void Director::RCXcommandCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;

    char cmd[255];

    if (sensor==director->RCX_engineASensor){
        sprintf(cmd,"rcx: motorState `%s**`",director->RCX_engineA.getValue() ? "+" : "-");
    }
    else if (sensor==director->RCX_engineBSensor){
        sprintf(cmd,"rcx: motorState `*%s*`",director->RCX_engineB.getValue() ? "+" : "-");
    }
    else if (sensor==director->RCX_engineCSensor){
        sprintf(cmd,"rcx: motorState `**%s`",director->RCX_engineC.getValue() ? "+" : "-");
    }
    else if (sensor==director->RCX_enginePowerASensor){
        sprintf(cmd,"rcx: motorPower `%d**`",director->RCX_enginePowerA.getValue());
    }
    else if (sensor==director->RCX_enginePowerBSensor){
        sprintf(cmd,"rcx: motorPower `*%d*`",director->RCX_enginePowerB.getValue());
    }
    else if (sensor==director->RCX_enginePowerCSensor){
        sprintf(cmd,"rcx: motorPower `**%d`",director->RCX_enginePowerC.getValue());
    }
    else if (sensor==director->RCX_engineDirASensor){
        sprintf(cmd,"rcx: motorDir `%s**`",director->RCX_engineDirA.getValue() ? "f" : "b");
    }
    else if (sensor==director->RCX_engineDirBSensor){
        sprintf(cmd,"rcx: motorDir `*%s*`",director->RCX_engineDirB.getValue() ? "f" : "b");
    }
    else if (sensor==director->RCX_engineDirCSensor){
        sprintf(cmd,"rcx: motorDir `**%s`",director->RCX_engineDirC.getValue() ? "f" : "b");
    }

    director->command.setValue(cmd);
}

bool Director::isConstructionStepValid()
{
    if (constructionStep.getValue()<0 || constructionStep.getValue()>maxConstructionStepIndex.getValue()) {
        printf("DIRECTOR: construction step (%d) should be 0<= <=%d\n",constructionStep.getValue(),maxConstructionStepIndex.getValue());
        return false;
    }
    else return true;
}

void Director::parseIntList(std::string &parseString,SbIntList *indexList,int lineNumber)
{
    int marker1=0,marker2=0;
    bool dataFound=false;
    int num;
    while ((marker1=parseString.find_first_not_of(" ",marker2))!=parseString.npos){
        dataFound=true;
        if ((marker2=parseString.find(" ",marker1))==parseString.npos) marker2=parseString.length();
        if (sscanf(parseString.substr(marker1,marker2-marker1).c_str(),"%d",&num)) indexList->append(num);
        else {
            if (parseString.substr(marker1,marker2-marker1).find("nil",0)==parseString.npos){
                printf("DIRECTOR: error: '%s' cannot be parsed as an integer in line %d\n",
                    parseString.substr(marker1,(marker2==parseString.length() ? marker2-marker1-1 : marker2-marker1)).c_str(),lineNumber
                );
            }
        }
    }
    if (!dataFound) printf("DIRECTOR: warning: line %d is empty or contains only white spaces\n",lineNumber);
}

void Director::disableEvents()
{
    enableEvents=FALSE;
    eventEnableTimer->setTimeFromNow(eventTimeOut.getValue());
    eventEnableTimer->schedule();
}

void Director::displayExplanation()
{
    int i;
    switch (displayMode.getValue()){
        case HIDE_ALL:
        case SHOW_ALL:
        default:
            explanation.setValue("");
            break;
        case SHOW_CURRENT_ONLY:
        case SHOW_UP_TO_CURRENT:
            // show construction step-specific parts            
            SbStringList *expList=(SbStringList*)(explanationList[constructionStep.getValue()]);

            if (!expList) {
                printf("DIRECTOR: construction step %d contains erroneous data\n",constructionStep.getValue());
                break;
            }
            // set explanation text field
            for (i=0;i<expList->getLength();i++){
                explanation.set1Value(i,*((*expList)[i]));
            }
            explanation.setNum(expList->getLength());
            break;
    }
}

void Director::displayModelPartAndAugmentation(bool isPlacingTile)
{
    SbIntList *modelPList=NULL;
    SbIntList *augList=NULL;

    modelPList=(SbIntList*)(modelPartList[constructionStep.getValue()]);
    if (modelPList!=NULL && modelPList->getLength()) tilePuppetNum=permanentPuppetNum.getValue()+(*modelPList)[0];
    else tilePuppetNum=-1;

    int i;
    switch (displayMode.getValue()){
        case HIDE_ALL:
        default:
            showModelPart.setValue(-1);
            showAugmentation.setValue(-1);
            break;
        case SHOW_ALL:
            showModelPart.setValue(-3);
            // show permanent augmentation parts
            for (i=0;i<permanentAttributeNum.getValue();i++) showAugmentation.set1Value(i,i);
            break;
        case SHOW_CURRENT_ONLY:
        case SHOW_UP_TO_CURRENT:
            // show permanent model and augmentation parts
            for (i=0;i<permanentPuppetNum.getValue();i++) showModelPart.set1Value(i,i);
            for (i=0;i<permanentAttributeNum.getValue();i++) showAugmentation.set1Value(i,i);

            // show construction step-specific parts
            if (tilePuppetNum<0) { // >=0 indicates success at the beginning of this routine
                showModelPart.setNum(permanentPuppetNum.getValue());
                showAugmentation.setNum(permanentAttributeNum.getValue());
            }
            else {
                augList=(SbIntList*)(augmentationList[constructionStep.getValue()]);
                if (!modelPList || !augList) {
                    printf("DIRECTOR: construction step %d contains erroneous data\n",constructionStep.getValue());
                    break;
                }

                // set visible LEGO model parts
                if (displayMode.getValue()==SHOW_UP_TO_CURRENT){
                    if (modelPList->getLength()){
                        // if the virtual presenter is currently placing a new tile, display it up to the current but one part (don't add 1 to the index)
                        // in this mode only consider the first modelPList element, it doesn't make sense to display "up to" multiple elements ********* TODO: is this correct?
                        int maxIndex=(*modelPList)[0]+permanentPuppetNum.getValue()+(isPlacingTile ? 0 : 1);
                        for (i=permanentPuppetNum.getValue();i<maxIndex;i++) showModelPart.set1Value(i,i);
	                    showModelPart.setNum(maxIndex);
                    }
                }
                else {
                    // don't show the tile currently being placed in this mode
                    if (!isPlacingTile) {
                        for (i=0;i<modelPList->getLength();i++) {
                            showModelPart.set1Value(
                                permanentPuppetNum.getValue()+i,
                                permanentPuppetNum.getValue()+(*modelPList)[i]
                            );
                        }
                        showModelPart.setNum(permanentPuppetNum.getValue()+modelPList->getLength());
                    }
                    else showModelPart.setNum(permanentPuppetNum.getValue());
                }

                // set visible LEGO model augmentation (regardless whether the virtual presenter is placing a tile or not)
                for (i=0;i<augList->getLength();i++){
                    showAugmentation.set1Value(
                        permanentAttributeNum.getValue()+i,
                        permanentAttributeNum.getValue()+(*augList)[i]
                    );
                }
                showAugmentation.setNum(permanentAttributeNum.getValue()+augList->getLength());
            }
            break;
    }
}

void Director::sendAnimationCommand(int puppetNum,int animMode,std::string cmd)
{
    if (!usePresenter.getValue()) return;

    if (currentAnimationHint!=NULL){
        int marker;
        char str[255];
        if ((marker=cmd.find("\n"))!=cmd.npos) cmd=cmd.substr(0,marker);
        if (cmd.find("place")!=cmd.npos) sprintf(str,"%s: %s %d %d",Choreographer::minifigIDString.getString(),cmd.c_str(),puppetNum,animMode);
        else if (cmd.find("explain")!=cmd.npos) sprintf(str,"%s: explain %d",Choreographer::minifigIDString.getString(),puppetNum);
        else if (cmd.find("motor")!=cmd.npos || cmd.find("sensor")!=cmd.npos) sprintf(str,"%s: %s",Choreographer::rcxIDString.getString(),currentAnimationHint->getString());
        else if (cmd.find("nil")!=cmd.npos) strcpy(str,"");
        else sprintf(str,"%s: %s",Choreographer::minifigIDString.getString(),cmd.c_str());
        command.setValue(str);
#ifdef ARLEGO_DEBUG
        printf("DIRECTOR: animation command '%s' sent\n",str);
#endif
    }
#ifdef ARLEGO_DEBUG
    else printf("DIRECTOR: no animation hint defined for construction step %d\n",constructionStep.getValue());
#endif
}

void Director::stateEngine(int eventID)
{
#ifdef ARLEGO_DEBUG
    printf("ARLEGO: stateEngine: current state: %d, event ID %d\n",state.getValue(),eventID);
#endif

    if (!usePresenter.getValue()) {
        displayModelPartAndAugmentation();
        return;
    }

    std::string cmd=currentAnimationHint->getString();
    std::string finishedCmdData=finishedCommandData.getNum() ? finishedCommandData[0].getString() : "";

    switch (state.getValue()) {
        case PUC_DEFAULT:
        case PUC_NOINTRO_FOR_RCX:
            switch(eventID) {
                case Choreographer::COMMAND:
                    displayModelPartAndAugmentation();
                    sendAnimationCommand(tilePuppetNum,INTRO_ONLY,currentAnimationHint->getString());
                    state=PUC_NOINTRO_FOR_RCX;
                    break;
                case Choreographer::NEAR_RCX:
                    // send minifig to RCX
                    // position & orientation change
                    command.setValue("sendTo `RCX`");
                    if (state.getValue()==PUC_DEFAULT) state=RCX_DEFAULT;
                    else state=RCX_NOINTRO;
            	    break;
                case Choreographer::NEAR_DISPLAY:
                    // send minifig to remote user
                    // ...
                    state=REMOTE;
            	    break;
                default:
                    // idle behavior: instructs user to press "Next" on PUC after a random amount of time has passed while waving hands towards (!!!) the user
                    // ...
                    break;
            }
            break;
        case RCX_DEFAULT:
        case RCX_NOINTRO:
            switch(eventID) {
                case Choreographer::COMMAND:
                    displayModelPartAndAugmentation(cmd.find("place")!=cmd.npos);
                    // execute command --> introduce new tile if we haven't introduced it before and then mount tile on robot
                    sendAnimationCommand(tilePuppetNum,state.getValue()==RCX_NOINTRO ? PLACE_ONLY : INTRO_AND_PLACE,currentAnimationHint->getString());
                    state=RCX_DEFAULT;
                    break;
                case Choreographer::FINISHED_CMD:
                    if (finishedCmdData.find("place")!=finishedCmdData.npos) {
                        displayModelPartAndAugmentation();
                        sendAnimationCommand(tilePuppetNum,state.getValue()==RCX_NOINTRO ? PLACE_ONLY : INTRO_AND_PLACE,"moveBack");
                    }
                    // no state change here!
                    break;
                case Choreographer::NEAR_RCX:
                    // pick up minifig by PUC
                    // position & orientation change
                    command.setValue("sendTo `PUC`");
                    if (state.getValue()==RCX_DEFAULT) state=PUC_DEFAULT;
                    else state=PUC_NOINTRO_FOR_RCX;
            	    break;
                case Choreographer::NEAR_DISPLAY:
                    // send minifig to remote user
                    // ...
                    state=REMOTE;
            	    break;
                default:
                    // idle behavior: instructs user to press "Next" on PUC after a random amount of time has passed while waving hands towards (!!!) the user
                    // ...
                    break;
            }
    	    break;
        case REMOTE:
            switch(eventID) {
                case Choreographer::COMMAND:
                    // execute command --> show tile to remote user
                    // ...
                    break;
                case Choreographer::NEAR_DISPLAY:
                    // pick up minifig by PUC --> send minifig back to local user
                    // ...
                    state=PUC_DEFAULT;
            	    break;
                default:
                    // idle behavior: instructs user to press "Next" on PUC after a random amount of time has passed while waving hands towards (!!!) the user
                    // ...
                    break;
            }
            break;
        default:
            printf("DIRECTOR: invalid minifig state (%d)\n",state.getValue());
            return;        
    }
}

