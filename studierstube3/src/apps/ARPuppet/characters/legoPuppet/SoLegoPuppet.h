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
/* SoLegoPuppet.h contains the definition of class SoLegoPuppet,
 * a puppet for LEGO Mindstorms (R) robots
 *
 * @author Istvan Barakonyi
 *
 * The message coding and decoding parts of the RCX code are taken from the 
 * Direct Serial LEGO Mindstorms RCX control V1.0
 * (c) 7.4.2001 by Daniel Berger (berger@kyb.tuebingen.mpg.de),
 *
 * $Id: SoLegoPuppet.h 3986 2004-10-21 21:57:28Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _SOLEGOPUPPET_H_
#define _SOLEGOPUPPET_H_

#include "legoPuppet.h"
#include <list>

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/SbPList.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class RCX_Message {

public:

    RCX_Message() {};
    ~RCX_Message() {};

    unsigned short messageType; // if messageType = 0x00, it's our own, _non-RCX_ wait message
    unsigned char sendbuffer[256];
    unsigned char receivebuffer[256];
    unsigned long send_bytes;
    unsigned long rec_bytes;
    unsigned long dec_bytes;
    unsigned short num;
    unsigned short queryType;
};

class LEGOPUPPET_API SoLegoPuppet : public SoNode {

    SO_NODE_HEADER(SoLegoPuppet);

public:

    typedef enum MotorType  { A=1, B=2, C=4 };
    typedef enum Direction  { FORWARD, BACKWARD };
    typedef enum SensorType { RAW=0, TOUCH, TEMPERATURE, LIGHT, ROTATION };
    typedef enum SensorMode { 
        RAWVALUE=0, BOOLEANVALUE=0x20, EDGECOUNT=0x40, PULSECOUNT=0x60, 
        PERCENTAGE=0x80, CELSIUS=0xa0, FAHRENHEIT=0xc0, ANGLE=0xf0 
    };

    SoSFBool alive;
    SoSFInt32 batteryPower;
    SoSFBool engineA;
    SoSFBool engineB;
    SoSFBool engineC;
    SoSFBool engineDirA;
    SoSFBool engineDirB;
    SoSFBool engineDirC;
    SoSFInt32 enginePowerA;
    SoSFInt32 enginePowerB;
    SoSFInt32 enginePowerC;
    SoSFInt32 sensor1;
    SoSFInt32 sensor2;
    SoSFInt32 sensor3;
    SoSFEnum sensorType1;
    SoSFEnum sensorType2;
    SoSFEnum sensorType3;
    SoSFEnum sensorMode1;
    SoSFEnum sensorMode2;
    SoSFEnum sensorMode3;
    SoSFInt32 sensorSlope1;
    SoSFInt32 sensorSlope2;
    SoSFInt32 sensorSlope3;

    SoLegoPuppet();
    ~SoLegoPuppet();

/*  
    SENSOR TYPES                        |  SENSOR MODES
    Id Name        Default mode         |   Mode Name              Description
    0  Raw         Raw                  |   0    Raw               Value in 0..1023.
    1  Touch       Boolean              |   1    Boolean           Either 0 or 1.
    2  Temperature Temperature in °C    |   2    Edge count        Number of boolean transitions.
    3  Light       Percentage           |   3    Pulse count       Number of boolean transitions divided by two.
    4  Rotation    Angle                |   4    Percentage        Raw value scaled to 0..100.
                                        |   5    Temperature in °C 1/10ths of a degree, -19.8..69.5.
                                        |   6    Temperature in °F 1/10ths of a degree, -3.6..157.1.
                                        |   7    Angle             1/16ths of a rotation, represented as a signed short
*/
    void RCX_motor_on(unsigned short motor);
    void RCX_motor_off(unsigned short motor);
    void RCX_motor_release(unsigned short motor);
    void RCX_motor_direction(unsigned short motor,unsigned short direction);
    void RCX_motor_power(unsigned short motor, unsigned short level);
    void RCX_motor_state(unsigned short motor);
    void RCX_sensor_query(unsigned short queryMode);
    void RCX_sensor_value(unsigned short sensor);
    void RCX_sensor_type(unsigned short sensor,unsigned short type);
    void RCX_sensor_mode(unsigned short sensor,unsigned short mode,unsigned short slope=0);
    void RCX_sensor_state(unsigned short sensor);

    void Sound(unsigned short sound=0);
    void Wait(unsigned int duration);
    void PowerOff();

    bool isAlive();
    void dumpMessageQueue();

SoINTERNAL public:

    static void initClass();

protected:

    typedef enum State { ALIVE, DEAD, WAIT };

    SoTimerSensor *timerSensor;
    SoAlarmSensor *alarmSensor;

    void *RCX_port;
    unsigned char message[256];
    bool acknowledgable;
    // the switchbit is necessary for sequence bit problems:
    // if the opcode we want to send is the same as the last opcode acknowledged by the RCX, 
    // we have to toggle the 0x08 bit for the opcode to be accepted again
    unsigned char switchbit;
    bool initialized_USB;
    bool initialized_RCX;
    std::list<RCX_Message> messageQueue;
    State state;
    int waitDuration;
    unsigned int count;
    unsigned int sensorQueryMode;

    static void timerSensorCallback(void *data, SoSensor *);
    static void alarmSensorCallback(void *data, SoSensor *);

    void Init_USB();
    void Init_RCX();
    void ScheduleMessage(int sendlength,int receivelength);
    void Dead();
    void Alive();
    void UpdateState(RCX_Message &msg);

    bool RCX_send(RCX_Message &msg);
    bool RCX_receive(RCX_Message &msg);
    bool RCX_decode(RCX_Message &msg);
    void RCX_alive();
    void RCX_battery_power();
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOLEGOPUPPET_H_
