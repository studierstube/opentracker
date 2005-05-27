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
/* SoLegoPuppeteer.h contains the definition of class SoLegoPuppeteer
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoLegoPuppeteer.h 3871 2004-10-10 10:14:33Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _SOLEGOPUPPETEER_H_
#define _SOLEGOPUPPETEER_H_

// for DLL export
#include "legoPuppet.h"

// Inventor specific classes
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>

// own classes
#include "../SoPuppeteerKit.h"
#include "SoLegoPuppet.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class LEGOPUPPET_API SoLegoPuppeteer : public SoPuppeteerKit
{
    SO_KIT_HEADER(SoLegoPuppeteer);

public:

    /// Determines whether to always synchronize fields with the physical legorobot (if available)
    SoSFBool synchRCX;

    /// Fields identical to those of the SoLegoPuppet
    /*@{*/
    
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
    SoSFInt32 legoSoundNum;

    /*@}*/

    SoLegoPuppeteer();
    ~SoLegoPuppeteer();

SoINTERNAL public:

    static void initClass();

protected:

    /// Field sensor for <em>synchRCX</em>
    SoFieldSensor *synchRCXSensor;

    /// Storing a pointer to the physical LEGO robot
    SoLegoPuppet *legoRobot;

    // Static callback for field sensor
    static void synchRCXCB(void *data,SoSensor *sensor);

    /// Setting up field connections etc.
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Command implementation
    virtual bool implementCommand(SoNode *puppet);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOLEGOPUPPETEER_H_
