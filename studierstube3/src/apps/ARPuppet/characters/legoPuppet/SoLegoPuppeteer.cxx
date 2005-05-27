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
/* SoLegoPuppeteer.cxx contains the implementation of class SoPuppeteerKit
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoLegoPuppeteer.cxx 3986 2004-10-21 21:57:28Z bara $
 * @file
 */
/* ======================================================================== */

// Inventor classes
#include <Inventor/actions/SoSearchAction.h>

// own classes
#include "SoLegoPuppeteer.h"

SO_KIT_SOURCE(SoLegoPuppeteer);

void SoLegoPuppeteer::initClass()
{
    SO_KIT_INIT_CLASS(SoLegoPuppeteer, SoPuppeteerKit, "SoPuppeteerKit");
    SoLegoPuppet::initClass();
#ifdef DEBUG_ARPUPPET
    printf("LEGOPUPPETEER: initialized\n");
#endif
}

SoLegoPuppeteer::SoLegoPuppeteer()
{
#ifdef DEBUG_ARPUPPET
    printf("LEGOPUPPETEER: constructor\n");
#endif

    SO_KIT_CONSTRUCTOR(SoLegoPuppeteer);

    SO_KIT_ADD_FIELD(synchRCX, (FALSE));
    SO_KIT_ADD_FIELD(alive,(FALSE));
    SO_KIT_ADD_FIELD(batteryPower,(0));
    SO_KIT_ADD_FIELD(engineA,(FALSE));
    SO_KIT_ADD_FIELD(engineB,(FALSE));
    SO_KIT_ADD_FIELD(engineC,(FALSE));
    SO_KIT_ADD_FIELD(engineDirA,(FALSE));
    SO_KIT_ADD_FIELD(engineDirB,(FALSE));
    SO_KIT_ADD_FIELD(engineDirC,(FALSE));
    SO_KIT_ADD_FIELD(enginePowerA,(0));
    SO_KIT_ADD_FIELD(enginePowerB,(0));
    SO_KIT_ADD_FIELD(enginePowerC,(0));
    SO_KIT_ADD_FIELD(sensor1,(0));
    SO_KIT_ADD_FIELD(sensor2,(0));
    SO_KIT_ADD_FIELD(sensor3,(0));
    SO_KIT_ADD_FIELD(sensorType1,(SoLegoPuppet::RAW));
    SO_KIT_ADD_FIELD(sensorType2,(SoLegoPuppet::RAW));
    SO_KIT_ADD_FIELD(sensorType3,(SoLegoPuppet::RAW));
    SO_KIT_ADD_FIELD(sensorMode1,(SoLegoPuppet::RAWVALUE));
    SO_KIT_ADD_FIELD(sensorMode2,(SoLegoPuppet::RAWVALUE));
    SO_KIT_ADD_FIELD(sensorMode3,(SoLegoPuppet::RAWVALUE));
    SO_KIT_ADD_FIELD(sensorSlope1,(0));
    SO_KIT_ADD_FIELD(sensorSlope2,(0));
    SO_KIT_ADD_FIELD(sensorSlope3,(0));
    SO_KIT_ADD_FIELD(legoSoundNum,(0));

    SO_KIT_DEFINE_ENUM_VALUE(SensorType,SoLegoPuppet::RAW);
    SO_KIT_DEFINE_ENUM_VALUE(SensorType,SoLegoPuppet::TOUCH);
    SO_KIT_DEFINE_ENUM_VALUE(SensorType,SoLegoPuppet::TEMPERATURE);
    SO_KIT_DEFINE_ENUM_VALUE(SensorType,SoLegoPuppet::LIGHT);
    SO_KIT_DEFINE_ENUM_VALUE(SensorType,SoLegoPuppet::ROTATION);
    SO_KIT_SET_SF_ENUM_TYPE(sensorType1,SensorType);
    SO_KIT_SET_SF_ENUM_TYPE(sensorType2,SensorType);
    SO_KIT_SET_SF_ENUM_TYPE(sensorType3,SensorType);

    SO_KIT_DEFINE_ENUM_VALUE(SensorMode,SoLegoPuppet::RAWVALUE);
    SO_KIT_DEFINE_ENUM_VALUE(SensorMode,SoLegoPuppet::BOOLEANVALUE);
    SO_KIT_DEFINE_ENUM_VALUE(SensorMode,SoLegoPuppet::EDGECOUNT);
    SO_KIT_DEFINE_ENUM_VALUE(SensorMode,SoLegoPuppet::PULSECOUNT);
    SO_KIT_DEFINE_ENUM_VALUE(SensorMode,SoLegoPuppet::PERCENTAGE);
    SO_KIT_DEFINE_ENUM_VALUE(SensorMode,SoLegoPuppet::CELSIUS);
    SO_KIT_DEFINE_ENUM_VALUE(SensorMode,SoLegoPuppet::FAHRENHEIT);
    SO_KIT_DEFINE_ENUM_VALUE(SensorMode,SoLegoPuppet::ANGLE);
    SO_KIT_SET_SF_ENUM_TYPE(sensorMode1,SensorMode);
    SO_KIT_SET_SF_ENUM_TYPE(sensorMode2,SensorMode);
    SO_KIT_SET_SF_ENUM_TYPE(sensorMode3,SensorMode);
    
    SO_KIT_INIT_INSTANCE();

    // field sensors
    synchRCXSensor=new SoFieldSensor(synchRCXCB,this);

    legoRobot=NULL;

    commandString=
        "motorState %s \
         motorDir %s \
         motorPower %s \
         setSensor %s \
         sensorType %d %d \
         sensorMode %d %d %d(0)\
         sensorState %s \
         wait %d \
         sound %d \
         off";

    setUpConnections(TRUE,TRUE);
}

SoLegoPuppeteer::~SoLegoPuppeteer()
{
    delete synchRCXSensor;
}

SbBool SoLegoPuppeteer::setUpConnections(SbBool onOff, SbBool doItAlways)
{
#ifdef DEBUG_ARPUPPET
    printf("LEGOPUPPETEER: setUpConnections %s %s for puppeteer %s\n",onOff ? "TRUE" : "FALSE",doItAlways ? "TRUE" : "FALSE",id.getValue().getString());
#endif

    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        synchRCXSensor->attach(&synchRCX);

        // Search for an SoLegoPuppet node representing the physical LEGO robot
        SoSearchAction searchAction;
        searchAction.setInterest(SoSearchAction::FIRST);
        searchAction.setSearchingAll(TRUE); // search every node in the scene graph regardless of switch settings
        SoType type=SoType::fromName("SoLegoPuppet");
        searchAction.setType(type,TRUE);
        this->ref();
        searchAction.apply(this);
        SoPath *path=searchAction.getPath();
        if (path!=NULL) legoRobot=(SoLegoPuppet*)(((SoFullPath*)path)->getTail());
        else legoRobot=NULL;

        synchRCXCB(this,NULL);
    }
    else {
        synchRCXSensor->detach();
    }
    
    // set up connections in parent class SoPuppeteerKit
    SoPuppeteerKit::setUpConnections(onOff,doItAlways);

    return !(connectionsSetUp = onOff);
}

void SoLegoPuppeteer::synchRCXCB(void *data,SoSensor *sensor)
{
    SoLegoPuppeteer *puppeteerKit=(SoLegoPuppeteer*)data;

    if (puppeteerKit->synchRCX.getValue() && puppeteerKit->legoRobot!=NULL){
        puppeteerKit->engineA.connectFrom(&puppeteerKit->legoRobot->engineA);
        puppeteerKit->engineB.connectFrom(&puppeteerKit->legoRobot->engineB);
        puppeteerKit->engineC.connectFrom(&puppeteerKit->legoRobot->engineC);
        puppeteerKit->engineDirA.connectFrom(&puppeteerKit->legoRobot->engineDirA);
        puppeteerKit->engineDirB.connectFrom(&puppeteerKit->legoRobot->engineDirB);
        puppeteerKit->engineDirC.connectFrom(&puppeteerKit->legoRobot->engineDirC);
        puppeteerKit->enginePowerA.connectFrom(&puppeteerKit->legoRobot->enginePowerA);
        puppeteerKit->enginePowerB.connectFrom(&puppeteerKit->legoRobot->enginePowerB);
        puppeteerKit->enginePowerC.connectFrom(&puppeteerKit->legoRobot->enginePowerC);
        puppeteerKit->enginePowerA.connectFrom(&puppeteerKit->legoRobot->enginePowerA);
        puppeteerKit->sensor1.connectFrom(&puppeteerKit->legoRobot->sensor1);
        puppeteerKit->sensor2.connectFrom(&puppeteerKit->legoRobot->sensor2);
        puppeteerKit->sensor3.connectFrom(&puppeteerKit->legoRobot->sensor3);
        puppeteerKit->sensorType1.connectFrom(&puppeteerKit->legoRobot->sensorType1);
        puppeteerKit->sensorType2.connectFrom(&puppeteerKit->legoRobot->sensorType2);
        puppeteerKit->sensorType3.connectFrom(&puppeteerKit->legoRobot->sensorType3);
        puppeteerKit->sensorMode1.connectFrom(&puppeteerKit->legoRobot->sensorMode1);
        puppeteerKit->sensorMode2.connectFrom(&puppeteerKit->legoRobot->sensorMode2);
        puppeteerKit->sensorMode3.connectFrom(&puppeteerKit->legoRobot->sensorMode3);
        puppeteerKit->sensorSlope1.connectFrom(&puppeteerKit->legoRobot->sensorSlope1);
        puppeteerKit->sensorSlope2.connectFrom(&puppeteerKit->legoRobot->sensorSlope2);
        puppeteerKit->sensorSlope3.connectFrom(&puppeteerKit->legoRobot->sensorSlope3);
        puppeteerKit->batteryPower.connectFrom(&puppeteerKit->legoRobot->batteryPower);
        puppeteerKit->alive.connectFrom(&puppeteerKit->legoRobot->alive);
    }
    else {
        puppeteerKit->engineA.disconnect();
        puppeteerKit->engineB.disconnect();
        puppeteerKit->engineC.disconnect();
        puppeteerKit->engineDirA.disconnect();
        puppeteerKit->engineDirB.disconnect();
        puppeteerKit->engineDirC.disconnect();
        puppeteerKit->enginePowerA.disconnect();
        puppeteerKit->enginePowerB.disconnect();
        puppeteerKit->enginePowerC.disconnect();
        puppeteerKit->enginePowerA.disconnect();
        puppeteerKit->sensor1.disconnect();
        puppeteerKit->sensor2.disconnect();
        puppeteerKit->sensor3.disconnect();
        puppeteerKit->sensorType1.disconnect();
        puppeteerKit->sensorType2.disconnect();
        puppeteerKit->sensorType3.disconnect();
        puppeteerKit->sensorMode1.disconnect();
        puppeteerKit->sensorMode2.disconnect();
        puppeteerKit->sensorMode3.disconnect();
        puppeteerKit->sensorSlope1.disconnect();
        puppeteerKit->sensorSlope2.disconnect();
        puppeteerKit->sensorSlope3.disconnect();
        puppeteerKit->batteryPower.disconnect();
        if (puppeteerKit->legoRobot!=NULL) puppeteerKit->alive.disconnect();
        // if the status is not synchronized, set alive to TRUE so that visual feedback can be still given even if the commands are not necessarily physically executed
        puppeteerKit->alive=TRUE;
    }
}

bool SoLegoPuppeteer::implementCommand(SoNode *puppet)
{
#ifdef DEBUG_ARPUPPET
    printf("LEGOPUPPETEER: implement command\n");
#endif
    if (isType(puppet,"SoLegoPuppet")){
        SoLegoPuppet *legoPuppet=(SoLegoPuppet*)foundNode;
        if (isCommandType("motorState")){
            unsigned int onMotors=0;
            unsigned int offMotors=0;
            unsigned int releaseMotors=0;
            unsigned int motorCode=0;
            for (int i=0;i<3;i++){
                switch (i) {
                    case 0:
                        motorCode=SoLegoPuppet::A;
                        break;
                    case 1:
                        motorCode=SoLegoPuppet::B;
                        break;
                    case 2:
                        motorCode=SoLegoPuppet::C;
                        break;
                    default:
                        break;
                }
                switch (getStringParam(0)[i]) {
                    case '+':
                        onMotors|=motorCode;
                        break;
                    case '-':
                        offMotors|=motorCode;
                        break;
                    case '0':
                        releaseMotors|=motorCode;
                        break;
                    case '*':
                        break;
                    default:
                        printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                        break;
                }
            }
            if (onMotors>0) legoPuppet->RCX_motor_on(onMotors);
            if (offMotors>0) legoPuppet->RCX_motor_off(offMotors);
            if (releaseMotors>0) legoPuppet->RCX_motor_release(releaseMotors);
            if (onMotors>0 || offMotors>0 || releaseMotors>0) noTrigger();
        } // end of command implementation "motorState"
        else if (isCommandType("motorDir")){
            unsigned int forwardMotors=0;
            unsigned int backwardMotors=0;
            unsigned int motorCode=0;
            for (int i=0;i<3;i++){
                switch (i) {
                    case 0:
                        motorCode=SoLegoPuppet::A;
                        break;
                    case 1:
                        motorCode=SoLegoPuppet::B;
                        break;
                    case 2:
                        motorCode=SoLegoPuppet::C;
                        break;
                    default:
                        break;
                }
                switch (getStringParam(0)[i]) {
                    case 'f':
                    case 'F':
                        forwardMotors|=motorCode;
                        break;
                    case 'b':
                    case 'B':
                        backwardMotors|=motorCode;
                        break;
                    case '*':
                        break;
                    default:
                        printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                        break;
                }
            }
            if (forwardMotors>0) {
                legoPuppet->RCX_motor_direction(forwardMotors,SoLegoPuppet::FORWARD);
            }
            if (backwardMotors>0) {
                legoPuppet->RCX_motor_direction(backwardMotors,SoLegoPuppet::BACKWARD);
            }
            if (forwardMotors>0 || backwardMotors>0) noTrigger();
        } // end of command implementation "motorDir"
        else if (isCommandType("motorPower")){
            unsigned int powerMotors[8];
            int i;
            for (i=0;i<8;i++) powerMotors[i]=0;
            for (i=0;i<3;i++){
                if (getStringParam(0)[i]>='0' && getStringParam(0)[i]<='7') {
                    unsigned int power=getStringParam(0)[i]-'0';
                    switch (i) {
                        case 0:
                            powerMotors[power]|=SoLegoPuppet::A;
                            break;
                        case 1:
                            powerMotors[power]|=SoLegoPuppet::B;
                        break;
                        case 2:
                            powerMotors[power]|=SoLegoPuppet::C;
                            break;
                        default:
                            break;
                    }
                }
                else if (getStringParam(0)[i]!='*') {
                    printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                }
            }
            for (i=0;i<8;i++) {
                if (powerMotors[i]) {
                    legoPuppet->RCX_motor_power(powerMotors[i],i);
                }
            }
            noTrigger();
        }
        else if (isCommandType("setSensor")){
            std::vector<std::string> cmdlist;
            for (int i=0;i<3;i++){
                char cmd1[255]="";
                char cmd2[255]="";
                switch (getStringParam(0)[i]){
                    case 'r':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::RAW);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::RAWVALUE);
                        break;
                    case 't':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::TOUCH);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::BOOLEANVALUE);
                        break;
                    case 'c':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::TEMPERATURE);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::CELSIUS);
                        break;
                    case 'l':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::LIGHT);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::PERCENTAGE);
                        break;
                    case 'o':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::ROTATION);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::ANGLE);
                        break;
                    case '*':
                        break;
                    default:
                        printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                }
                if (strlen(cmd1)) {
                    cmdlist.push_back(cmd1);
                    cmdlist.push_back(cmd2);
                }
            }
            addCompoundCommand(&cmdlist);
        }
        else if (isCommandType("sensorType")){
            int num=getIntParam(0);
            int type=getIntParam(1);
            if (num>0 && num<4){
                if (type>=SoLegoPuppet::RAW && type<=SoLegoPuppet::ROTATION){
                    legoPuppet->RCX_sensor_type(num-1,type);
                    noTrigger();
                }
                else printf("LEGOPUPPETEER: wrong sensor type: %d\n",type);
            }
            else printf("LEGOPUPPETEER: error in command: wrong sensor index '%c' \n",num);
        }
        else if (isCommandType("sensorMode")){
            int num=getIntParam(0);
            int mode=getIntParam(1);
            int slope=getIntParam(2);
            if (num>0 && num<4){
                if (mode>=SoLegoPuppet::RAWVALUE && mode<=SoLegoPuppet::ANGLE){
                    if (slope<0 || slope>31) slope=0;
                    legoPuppet->RCX_sensor_mode(num-1,getIntParam(1),slope);
                    noTrigger();
                }
                else printf("LEGOPUPPETEER: wrong sensor mode: %d\n",mode);
            }
            else printf("LEGOPUPPETEER: error in command: wrong sensor index '%c' \n",num);
        }
        else if (isCommandType("sensorState")){

            unsigned int onSensors=0;
            for (int i=0;i<3;i++){
                switch (getStringParam(0)[i]) {
                    case '+':
                        onSensors|=(int)(pow(2,i));
                        break;
                    case '-':
                        onSensors&=~(int)(pow(2,i));
                        break;
                    case '*':
                        break;
                    default:
                        printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                        break;
                }
            }
            if (onSensors>0) {
                legoPuppet->RCX_sensor_query(onSensors);
                noTrigger();
            }
        }
        else if (isCommandType("wait")){
            legoPuppet->Wait(getIntParam(0));
            addDurationMonitor(getIntParam(0));
        }
        else if (isCommandType("sound")){
            legoPuppet->Sound(getIntParam(0));
            noTrigger();
        }
        else if (isCommandType("off")){
            legoPuppet->PowerOff();
            noTrigger();
        }
        else return false; // command not found and implemented here, default implementation needed
        return true; // command found and implemented here, no default implementation needed
    } // end of SoLegoPuppet command implementation
    else if (!synchRCX.getValue() || legoRobot==NULL){
        if (isCommandType("motorState")){
            bool trigger=true;
            for (int i=0;i<3;i++){
                switch (getStringParam(0)[i]){
                    case '+':
                    case '-':
                    case '0':
                        switch (i) {
                            case 0:
                                engineA.setValue(getStringParam(0)[i]=='+');
                                break;
                            case 1:
                                engineB.setValue(getStringParam(0)[i]=='+');
                                break;
                            case 2:
                                engineC.setValue(getStringParam(0)[i]=='+');
                                break;
                            default:
                                break;
                        }
                        break;
                    case '*':
                        break;
                    default:
                        trigger=false;
                        printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                        break;
                }
            }
            if (trigger) noTrigger();
        } // end of command implementation "motorState"
        else if (isCommandType("motorDir")){
            bool trigger=true;
            for (int i=0;i<3;i++){
                switch (getStringParam(0)[i]){
                    case 'f':
                    case 'F':
                    case 'b':
                    case 'B':
                        switch (i) {
                            case 0:
                                engineDirA.setValue(getStringParam(0)[i]=='f' || getStringParam(0)[i]=='F');
                                break;
                            case 1:
                                engineDirB.setValue(getStringParam(0)[i]=='f' || getStringParam(0)[i]=='F');
                                break;
                            case 2:
                                engineDirC.setValue(getStringParam(0)[i]=='f' || getStringParam(0)[i]=='F');
                                break;
                            default:
                                break;
                        }
                        break;
                    case '*':
                        break;
                    default:
                        trigger=false;
                        printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                        break;
                }
            }
            if (trigger) noTrigger();
        } // end of command implementation "motorDir"
        else if (isCommandType("motorPower")){
            bool trigger=true;
            unsigned int powerMotors[8];
            int i;
            for (i=0;i<8;i++) powerMotors[i]=0;
            for (i=0;i<3;i++){
                if (getStringParam(0)[i]>='0' && getStringParam(0)[i]<='7') {
                    switch (i) {
                        case 0:
                            enginePowerA.setValue(getStringParam(0)[i]-'0');
                            break;
                        case 1:
                            enginePowerB.setValue(getStringParam(0)[i]-'0');
                            break;
                        case 2:
                            enginePowerC.setValue(getStringParam(0)[i]-'0');
                            break;
                        default:
                            break;
                    }
                }
                else if (getStringParam(0)[i]!='*') {
                    trigger=false;
                    printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                }
            }
            if (trigger) noTrigger();
        }
        else if (isCommandType("setSensor")){
            std::vector<std::string> cmdlist;
            for (int i=0;i<3;i++){
                char cmd1[255]="";
                char cmd2[255]="";
                switch (getStringParam(0)[i]){
                    case 'r':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::RAW);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::RAWVALUE);
                        break;
                    case 't':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::TOUCH);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::BOOLEANVALUE);
                        break;
                    case 'c':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::TEMPERATURE);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::CELSIUS);
                        break;
                    case 'l':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::LIGHT);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::PERCENTAGE);
                        break;
                    case 'o':
                        sprintf(cmd1,"sensorType %d %d",i+1,SoLegoPuppet::ROTATION);
                        sprintf(cmd2,"sensorMode %d %d",i+1,SoLegoPuppet::ANGLE);
                        break;
                    case '*':
                        break;
                    default:
                        printf("LEGOPUPPETEER: error in command: unknown symbol '%c' \n",getStringParam(0)[i]);
                }
                if (strlen(cmd1)) {
                    cmdlist.push_back(cmd1);
                    cmdlist.push_back(cmd2);
                }
            }
            addCompoundCommand(&cmdlist);
        }
        else if (isCommandType("sensorType")){
            int num=getIntParam(0);
            int type=getIntParam(1);
            if (num>0 && num<4){
                if (type>=SoLegoPuppet::RAW && type<=SoLegoPuppet::ROTATION){
                    switch (num) {
                        case 1:
                            sensorType1.setValue(type);
                            break;
                        case 2:
                            sensorType2.setValue(type);
                            break;
                        case 3:
                            sensorType3.setValue(type);
                            break;
                        default:
                            break;
                    }
                    noTrigger();
                }
                else printf("LEGOPUPPETEER: wrong sensor type: %d\n",type);
            }
            else printf("LEGOPUPPETEER: error in command: wrong sensor index '%c' \n",num);
        }
        else if (isCommandType("sensorMode")){
            int num=getIntParam(0);
            int mode=getIntParam(1);
            int slope=getIntParam(2);
            if (num>0 && num<4){
                if (mode>=SoLegoPuppet::RAWVALUE && mode<=SoLegoPuppet::ANGLE){
                    if (slope<0 || slope>31) slope=0;
                    switch (num) {
                        case 1:
                            sensorMode1.setValue(mode);
                            break;
                        case 2:
                            sensorMode2.setValue(mode);
                            break;
                        case 3:
                            sensorMode3.setValue(mode);
                            break;
                        default:
                            break;
                    }
                    noTrigger();
                }
                else printf("LEGOPUPPETEER: wrong sensor mode: %d\n",mode);
            }
            else printf("LEGOPUPPETEER: error in command: wrong sensor index '%c' \n",num);
        }
        else if (isCommandType("querySensor") || isCommandType("getSensorValue") || isCommandType("sensorValue")){
            // no implementation
            noTrigger();
        }
        else if (isCommandType("sound")){
            legoSoundNum.setValue(getIntParam(0));
            noTrigger();
        }
        else if (isCommandType("off")){
            engineA.setValue(FALSE);
            engineB.setValue(FALSE);
            engineC.setValue(FALSE);
            noTrigger();
        }
        else return false; // command not found and implemented here, default implementation needed
        return true;
    }

    return false;
}
