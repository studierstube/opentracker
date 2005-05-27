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
/* SoLegoPuppet.cxx contains the implementation of class SoLegoPuppet,
 * a puppet for LEGO Mindstorms (R) robots
 *
 * @author Istvan Barakonyi
 *
 * The message coding and decoding parts of the RCX code are taken from the 
 * Direct Serial LEGO Mindstorms RCX control V1.0
 * (c) 7.4.2001 by Daniel Berger (berger@kyb.tuebingen.mpg.de),
 *
 * $Id: SoLegoPuppet.cxx 3986 2004-10-21 21:57:28Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <windows.h> // for USB port handling under Win
#endif
#include "SoLegoPuppet.h"
using namespace std;

SO_NODE_SOURCE(SoLegoPuppet);

SoLegoPuppet::SoLegoPuppet()
{
    SO_NODE_CONSTRUCTOR(SoLegoPuppet);

    SO_NODE_ADD_FIELD(alive,(FALSE));
    SO_NODE_ADD_FIELD(batteryPower,(0));
    SO_NODE_ADD_FIELD(engineA,(FALSE));
    SO_NODE_ADD_FIELD(engineB,(FALSE));
    SO_NODE_ADD_FIELD(engineC,(FALSE));
    SO_NODE_ADD_FIELD(engineDirA,(FALSE));
    SO_NODE_ADD_FIELD(engineDirB,(FALSE));
    SO_NODE_ADD_FIELD(engineDirC,(FALSE));
    SO_NODE_ADD_FIELD(enginePowerA,(0));
    SO_NODE_ADD_FIELD(enginePowerB,(0));
    SO_NODE_ADD_FIELD(enginePowerC,(0));
    SO_NODE_ADD_FIELD(sensor1,(0));
    SO_NODE_ADD_FIELD(sensor2,(0));
    SO_NODE_ADD_FIELD(sensor3,(0));
    SO_NODE_ADD_FIELD(sensorType1,(RAW));
    SO_NODE_ADD_FIELD(sensorType2,(RAW));
    SO_NODE_ADD_FIELD(sensorType3,(RAW));
    SO_NODE_ADD_FIELD(sensorMode1,(RAWVALUE));
    SO_NODE_ADD_FIELD(sensorMode2,(RAWVALUE));
    SO_NODE_ADD_FIELD(sensorMode3,(RAWVALUE));
    SO_NODE_ADD_FIELD(sensorSlope1,(0));
    SO_NODE_ADD_FIELD(sensorSlope2,(0));
    SO_NODE_ADD_FIELD(sensorSlope3,(0));

    SO_NODE_DEFINE_ENUM_VALUE(SensorType,RAW);
    SO_NODE_DEFINE_ENUM_VALUE(SensorType,TOUCH);
    SO_NODE_DEFINE_ENUM_VALUE(SensorType,TEMPERATURE);
    SO_NODE_DEFINE_ENUM_VALUE(SensorType,LIGHT);
    SO_NODE_DEFINE_ENUM_VALUE(SensorType,ROTATION);
    SO_NODE_SET_SF_ENUM_TYPE(sensorType1,SensorType);
    SO_NODE_SET_SF_ENUM_TYPE(sensorType2,SensorType);
    SO_NODE_SET_SF_ENUM_TYPE(sensorType3,SensorType);

    SO_NODE_DEFINE_ENUM_VALUE(SensorMode,RAWVALUE);
    SO_NODE_DEFINE_ENUM_VALUE(SensorMode,BOOLEANVALUE);
    SO_NODE_DEFINE_ENUM_VALUE(SensorMode,EDGECOUNT);
    SO_NODE_DEFINE_ENUM_VALUE(SensorMode,PULSECOUNT);
    SO_NODE_DEFINE_ENUM_VALUE(SensorMode,PERCENTAGE);
    SO_NODE_DEFINE_ENUM_VALUE(SensorMode,CELSIUS);
    SO_NODE_DEFINE_ENUM_VALUE(SensorMode,FAHRENHEIT);
    SO_NODE_DEFINE_ENUM_VALUE(SensorMode,ANGLE);
    SO_NODE_SET_SF_ENUM_TYPE(sensorMode1,SensorMode);
    SO_NODE_SET_SF_ENUM_TYPE(sensorMode2,SensorMode);
    SO_NODE_SET_SF_ENUM_TYPE(sensorMode3,SensorMode);

    initialized_USB=initialized_RCX=false;
    RCX_port=NULL;
    switchbit=0;
    state=DEAD;
    waitDuration=0;
    count=0;
    sensorQueryMode=0;

    timerSensor=new SoTimerSensor(timerSensorCallback,this); // start immediately
    timerSensor->setInterval(0.1); // fire every sec
    alarmSensor=new SoAlarmSensor(alarmSensorCallback,this); // time will be specified in the Wait calls later
    
    Init_USB();
    Init_RCX();
}

SoLegoPuppet::~SoLegoPuppet()
{
    RCX_motor_off(A|B|C);
    CloseHandle(RCX_port);

    if (timerSensor->isScheduled()) timerSensor->unschedule();
    if (alarmSensor->isScheduled()) alarmSensor->unschedule();
    delete timerSensor;
    delete alarmSensor;
}

void SoLegoPuppet::initClass()
{
    SO_NODE_INIT_CLASS(SoLegoPuppet, SoNode, "Node");
}

void SoLegoPuppet::timerSensorCallback(void *data, SoSensor *sensor)
{
    SoLegoPuppet *robot=(SoLegoPuppet*)data;
    list<RCX_Message>::iterator it=robot->messageQueue.begin();
    list<RCX_Message>::iterator prev;

    // higher priority task
    if (robot->count%5==0 && robot->alive.getValue()){
        if (robot->sensorQueryMode){
            for (int i=0;i<3;i++) {
                if (robot->sensorQueryMode & (int)(pow(2,i))) {
                    robot->RCX_sensor_value(i);
                }
            }
        }
    }

    // lower priority task
    if (robot->count%40==0){
        robot->count=1;
        if (!robot->alive.getValue()) robot->RCX_alive();
        else robot->RCX_battery_power();
    }

#ifdef DEBUG_ARPUPPET
    robot->dumpMessageQueue();
#endif

    while (it!=robot->messageQueue.end()){
        switch (robot->state) {
            case ALIVE:
                if (!(it->messageType)){ // our own, non-RCX wait message
                    robot->state=WAIT;
                    int sec=robot->waitDuration/1000;
                    robot->alarmSensor->setTimeFromNow(
                        SbTime(sec,1000*(robot->waitDuration-1000*sec))
                    ); // SbTime(seconds,microseconds)
                    robot->alarmSensor->schedule();
                    prev=it++;
                    robot->messageQueue.erase(prev); // we can remove the wait message
                    break;
                }
                if (!robot->RCX_send(*it)) {
                    robot->Dead(); // if the send fails, the robot is dead
                    robot->state=DEAD;
                }
                else {
                    Sleep(20);
                    if (!robot->RCX_receive(*it)) {
                        robot->Dead(); // if the receive fails, the robot is dead
                        robot->state=DEAD;
                    }
                    else robot->RCX_decode(*it); // the send and the receive were OK, the robot is still alive
                }
                if (robot->alive.getValue()) { // if the robot is still alive, remove the message from the queue
                    // command executed, update engine and sensor status fields
                    robot->UpdateState(*it);
                    // proceed to the next message and remove the already processed one
                    prev=it++;
                    robot->messageQueue.erase(prev);                    
                }
                else it++; // otherwise just proceed to the next message
                break;
            case DEAD:
            case WAIT:
                if (it->messageType==0x10 || it->messageType==0x18){ // process only "alive" messages
                    if (robot->RCX_send(*it) && robot->RCX_receive(*it)){ // if send and receive are OK
                        robot->RCX_decode(*it); // decode message
                        robot->Alive();         // message was received alright, there was a response, the robot is alive
                        if (robot->state==DEAD) robot->state=ALIVE;
                    }
                    else {
                        if (robot->state==WAIT) {
                            robot->Dead(); // message wasn't received correctly, the robot is dead
                            robot->state=DEAD;
                        }
                    }
                    prev=it++;
                    robot->messageQueue.erase(prev); // we can remove the alive message
                }
                // we only process alive messages when the robot is dead, so just proceed to the next message in the queue
                else it++;
                break;
        } // end of switch
    } // end of while
    robot->count++;
}


void SoLegoPuppet::alarmSensorCallback(void *data, SoSensor *sensor)
{
    SoLegoPuppet *robot=(SoLegoPuppet*)data;
    // the necessary amount of time has passed, so return from the WAIT state
    robot->state=(robot->alive.getValue()) ? ALIVE : DEAD;
}

void SoLegoPuppet::Init_USB()
{
    printf("LEGOPUPPET: initializing USB tower...");

    RCX_port = CreateFile( 
        "\\\\.\\legotower1", 
        GENERIC_READ | GENERIC_WRITE, 
        0,                  // comm devices must be opened w/exclusive-access
        0,                  // no security attributes
        OPEN_EXISTING,          // comm devices must use OPEN_EXISTING
        FILE_FLAG_WRITE_THROUGH,    
        0                   // hTemplate must be NULL for comm devices
        );

    if (RCX_port==INVALID_HANDLE_VALUE) {
        initialized_USB=false;
        printf("failed\n");
        return;
    }
    else {
        initialized_USB=true;
        printf("done\n");
        timerSensor->schedule();
    }
}

void SoLegoPuppet::Init_RCX()
{
    printf("LEGOPUPPET: initializing RCX...");
    // clear message buffer
    message[0]=0x90; 
    ScheduleMessage(1,7);

    // initialize: set transmitter range to 'large'
    message[0]=0x31; 
    message[1]=1;
    ScheduleMessage(2,7);

    // beep
    message[0]=0x51;
    message[1]=2;
    ScheduleMessage(2,7);

    initialized_RCX=true;
    printf("done\n");
}

void SoLegoPuppet::Dead()
{
    alive.setValue(FALSE);
    batteryPower.setValue(0);
    engineA.setValue(FALSE);
    engineB.setValue(FALSE);
    engineC.setValue(FALSE);
}

void SoLegoPuppet::Alive()
{
    alive.setValue(TRUE);
    
    // synchronize robot state: engine and sensor states
    RCX_motor_state(MotorType::A);
    RCX_motor_state(MotorType::B);
    RCX_motor_state(MotorType::C);
    RCX_sensor_state(0);
    RCX_sensor_state(1);
    RCX_sensor_state(2);

}

void SoLegoPuppet::UpdateState(RCX_Message &msg)
{
    unsigned char message1=msg.sendbuffer[5];
    unsigned char message2=msg.sendbuffer[7];
    unsigned char message3=msg.sendbuffer[9];

    switch (msg.messageType){
        // motor on/off/release
        case 0x21:
            if (message1 & A) engineA.setValue((message1 & 0x80)>0);
            if (message1 & B) engineB.setValue((message1 & 0x80)>0);
            if (message1 & C) engineC.setValue((message1 & 0x80)>0);
            break;
        // motor direction forward/backward
        case 0xe1:
            if (message1 & A) engineDirA.setValue((message1 & 0x80)==0);
            if (message1 & B) engineDirB.setValue((message1 & 0x80)==0);
            if (message1 & C) engineDirC.setValue((message1 & 0x80)==0);
            break;
        // motor power 0-7
        case 0x13:
            if (message1 & A) enginePowerA.setValue(message3);
            if (message1 & B) enginePowerB.setValue(message3);
            if (message1 & C) enginePowerC.setValue(message3);
            break;
        // sensor type RAW/TOUCH/TEMPERATURE/LIGHT/ROTATION
        case 0x32:
            if (message1==0) sensorType1.setValue(message3);
            if (message1==1) sensorType2.setValue(message3);
            if (message1==2) sensorType3.setValue(message3);
            break;
        // sensor mode RAWVALUE/BOOLEANVALUE/EDGECOUNT/PULSECOUNT/PERCENTAGE/CELSIUS/FAHRENHEIT/ANGLE + sensor slope
        case 0x42:
            if (message1==0) {
                sensorType1.setValue(message3 & 0xf0);
                sensorSlope1.setValue(message3 & 0x0f);
            }
            if (message1==1) {
                sensorType2.setValue(message3 & 0xf0);
                sensorSlope2.setValue(message3 & 0x0f);
            }
            if (message1==2) {
                sensorType3.setValue(message3 & 0xf0);
                sensorSlope3.setValue(message3 & 0x0f);
            }
            break;
    }
}

bool SoLegoPuppet::isAlive()
{
    return alive.getValue() ? true : false;
}

void SoLegoPuppet::ScheduleMessage(int sendlength,int receivelength)
{
    if (!initialized_USB) return;

    unsigned long ptr;
    int s=0;
    RCX_Message rcx_msg;

    rcx_msg.messageType=message[0];
    
    if (message[0]){ // it's not our wait message
        message[0]|=switchbit;
        switchbit^=0x08;

        // create message to send
        rcx_msg.sendbuffer[0]=0x55;
        rcx_msg.sendbuffer[1]=0xff;
        rcx_msg.sendbuffer[2]=0x00;
        ptr=3;
        int i;
        for (i=0; i<sendlength; i++){
            if (i==2 && (message[0]==0x12 || message[0]==0x1a)) {
                rcx_msg.num=message[i];
                rcx_msg.queryType=message[i-1];
            }
            rcx_msg.sendbuffer[ptr++]=message[i];
            rcx_msg.sendbuffer[ptr++]=message[i]^0xff;
            s+=message[i];
        }
        rcx_msg.sendbuffer[ptr++]=(unsigned char)s;
        rcx_msg.sendbuffer[ptr++]=((unsigned char)s)^0xff;
        
        rcx_msg.send_bytes=ptr;
        rcx_msg.rec_bytes=receivelength;
    }
    messageQueue.push_back(rcx_msg); // add message to the end of the queue
}

bool SoLegoPuppet::RCX_send(RCX_Message &msg)
{
    unsigned long res;
    res=0;
    if(!WriteFile(RCX_port,msg.sendbuffer,msg.send_bytes,&res,0)) return false;
    if (res!=msg.send_bytes) return false;
    return true;
}

bool SoLegoPuppet::RCX_receive(RCX_Message &msg)
{  
    unsigned long res=0;
    if (!ReadFile(RCX_port,msg.receivebuffer,msg.rec_bytes,&res,0)) {
        msg.dec_bytes=0;
        return false;
    }
    msg.dec_bytes=res;
    return (msg.rec_bytes==res);
}

bool SoLegoPuppet::RCX_decode(RCX_Message &msg){

    int pos=0;
    int result=-1;

    if (msg.dec_bytes<4 || msg.receivebuffer[0]!=0x55 || msg.receivebuffer[1]!=0xff || msg.receivebuffer[2]!=0x00) {
        Dead();
        return false;
    }

    int num,value;
    switch(msg.receivebuffer[4]){
        case 0x10:
        case 0x18: // alive
            alive.setValue(1);
            break;
        case 0x12:
        case 0x1a: // get sensor value
            num=msg.num;
            value=(((int)msg.receivebuffer[pos+7])<<8)+msg.receivebuffer[pos+5];
            switch (msg.queryType){
                case 3: // motor state: 0x00-0x07=power, 0x08=fwd, 0x40=off, 0x80=on, 0x00=float
                    switch (num){
                        case 0:
                            engineA.setValue((value & 0x80)>0);
                            engineDirA.setValue((value & 0x08)==0);
                            enginePowerA.setValue(value & 0x07);
                            break;
                        case 1:
                            engineB.setValue((value & 0x80)>0);
                            engineDirB.setValue((value & 0x08)==0);
                            enginePowerB.setValue(value & 0x07);
                            break;
                        case 2:
                            engineC.setValue((value & 0x80)>0);
                            engineDirC.setValue((value & 0x08)==0);
                            enginePowerC.setValue(value & 0x07);
                            break;
                        default:
                            break;
                    }
                    break;
                case 9: // sensor value: use value as it is
                    switch (num){
                        case 0:
                            sensor1.setValue(value);
                            break;
                        case 1:
                            sensor2.setValue(value);
                            break;
                        case 2:
                            sensor3.setValue(value);
                            break;
                        default:
                            break;
                    }
                    break;
                case 10: // sensor type: use value as it is
                    switch (num){
                        case 0:
                            sensorType1.setValue(value);
                            break;
                        case 1:
                            sensorType2.setValue(value);
                            break;
                        case 2:
                            sensorType3.setValue(value);
                            break;
                        default:
                            break;
                    }
                    break;
                case 11: // sensor mode: mode value | slope
                    switch (num){
                        case 0:
                            sensorMode1.setValue(value & 0xf0);
                            sensorSlope1.setValue(value & 0x0f);
                            break;
                        case 1:
                            sensorMode2.setValue(value & 0xf0);
                            sensorSlope2.setValue(value & 0x0f);
                            break;
                        case 2:
                            sensorMode3.setValue(value & 0xf0);
                            sensorSlope3.setValue(value & 0x0f);
                            break;
                        default:
                            break;
                    }
                    break;            
            }
            break;
        case 0x30:
        case 0x38: // get battery power
            batteryPower.setValue((((int)msg.receivebuffer[pos+7])<<8)+msg.receivebuffer[pos+5]);
            break;
    }

    return true;
}

// 1 = A, 2 = B, 4 = C
void SoLegoPuppet::RCX_motor_on(unsigned short motor)
{ 
    if (motor>=0x01 && motor<=0x07){
        message[0]=0x21;
        motor|=0x80;
        message[1]=motor;
        ScheduleMessage(2,7);
    }
    else printf("LEGOPUPPET: wrong motor index: %d\n",motor);
}

// 1 = A, 2 = B, 4 = C
void SoLegoPuppet::RCX_motor_off(unsigned short motor)
{
    if (motor>=0x01 && motor<=0x07){
        message[0]=0x21;
        motor|=0x40;
        message[1]=motor;
        ScheduleMessage(2,7);
    }
    else printf("LEGOPUPPET: wrong motor index: %d\n",motor);
}

// 1 = A, 2 = B, 4 = C
void SoLegoPuppet::RCX_motor_release(unsigned short motor)
{ 
    if (motor>=0x01 && motor<=0x07){
        message[0]=0x21;
        message[1]=motor;
        ScheduleMessage(2,7);
    }
    else printf("LEGOPUPPET: wrong motor index: %d\n",motor);
}

// 1 = A, 2 = B, 4 = C
void SoLegoPuppet::RCX_motor_direction(unsigned short motor,unsigned short direction)
{
    if (motor>=0x01 && motor<=0x07){
        if (direction == FORWARD || direction == BACKWARD){
            message[0]=0xe1;
            if (direction==BACKWARD) motor|=0x80;
            message[1]=motor;
            ScheduleMessage(2,7);
        }
        else printf("LEGOPUPPET: wrong direction index: %d\n",direction);
    }
    else printf("LEGOPUPPET: wrong motor index: %d\n",motor);
}

// 1 = A, 2 = B, 4 = C
void SoLegoPuppet::RCX_motor_power(unsigned short motor, unsigned short level)
{
    if (motor>=0x01 && motor<=0x07){
        if (level>=0 && level<=7){
            message[0]=0x13;
            message[1]=motor;
            message[2]=2;
            message[3]=level;
            ScheduleMessage(4,7);
        }
        else printf("LEGOPUPPET: wrong power level: %d\n",level);
    }
    else printf("LEGOPUPPET: wrong motor index: %d\n",motor);
}

void SoLegoPuppet::RCX_motor_state(unsigned short motor)
{
    if (motor>=0x01 && motor<=0x07){
        message[0]=0x12;
        message[1]=3; // source = motor
        message[2]=log((float)motor)/log(2.0f);
        ScheduleMessage(3,11);
    }
    else printf("LEGOPUPPET: wrong motor index: %d\n",motor);
}

void SoLegoPuppet::RCX_sensor_query(unsigned short queryMode)
{
    sensorQueryMode=queryMode;
}

void SoLegoPuppet::RCX_sensor_value(unsigned short sensor)
{
    if (sensor>=0 || sensor<3){
        message[0]=0x12;
        message[1]=9; // source = sensor
        message[2]=sensor;
        ScheduleMessage(3,11);
    }
    else printf("LEGOPUPPET: wrong sensor index: %d\n",sensor);
}

void SoLegoPuppet::RCX_sensor_type(unsigned short sensor,unsigned short type)
{
    if (sensor>=0 && sensor<3){
        if (type>=RAW && type<=ROTATION){
            message[0]=0x32;
            message[1]=sensor;
            message[2]=type;
            ScheduleMessage(3,7);
        }
        else printf("LEGOPUPPET: wrong sensor type: %d\n",type);
    }
    else printf("LEGOPUPPET: wrong sensor index: %d\n",sensor);
}

void SoLegoPuppet::RCX_sensor_mode(unsigned short sensor,unsigned short mode,unsigned short slope)
{
    if (sensor>=0 && sensor<3){
        if (mode==RAWVALUE|| mode==BOOLEANVALUE|| mode==EDGECOUNT || mode==PULSECOUNT || 
            mode==PERCENTAGE || mode==CELSIUS || mode==FAHRENHEIT || mode==ANGLE){
            if (slope<0 || slope>31) {
                slope=0;
                printf("LEGOPUPPET: wrong sensor slope value: %d\n",slope);
            }
            message[0]=0x42;
            message[1]=sensor;
            message[2]=mode | slope;
            ScheduleMessage(3,7);
        }
        else printf("LEGOPUPPET: wrong sensor mode: %d\n",mode);
    }
    else printf("LEGOPUPPET: wrong sensor index: %d\n",sensor);
}

void SoLegoPuppet::RCX_sensor_state(unsigned short sensor)
{
    if (sensor>=0 && sensor<3){
        message[0]=0x12;
        message[1]=10; // source = sensor type
        message[2]=sensor;
        ScheduleMessage(3,11);

        message[0]=0x12;
        message[1]=11; // source = sensor mode
        message[2]=sensor;
        ScheduleMessage(3,11);
    }
    else printf("LEGOPUPPET: wrong sensor index: %d\n",sensor);
}

void SoLegoPuppet::RCX_alive()
{
    message[0]=0x10;
    ScheduleMessage(1,7);
}

void SoLegoPuppet::RCX_battery_power()
{
    message[0]=0x30;
    ScheduleMessage(1,11);
}

// LegoRobot action interface for users

void SoLegoPuppet::Sound(unsigned short sound)
{
    if (sound>=0 && sound<=5){
        message[0]=0x51;
        message[1]=sound;
        ScheduleMessage(2,7);
    }
}

// wait for 'duration' milliseconds
void SoLegoPuppet::Wait(unsigned int duration)
{
    waitDuration=duration; // milliseconds
    message[0]=0; // wait message
    ScheduleMessage(0,0);
}

void SoLegoPuppet::PowerOff()
{
    message[0]=0x60;
    ScheduleMessage(1,7);
}

void SoLegoPuppet::dumpMessageQueue()
{
    list<RCX_Message>::iterator it=messageQueue.begin();
    
    printf("*** LEGOPUPPET message queue dump: ");
    while (it!=messageQueue.end()){
        printf("0x%x ",it->messageType);
        it++;
    }
    printf("***\n");
}
