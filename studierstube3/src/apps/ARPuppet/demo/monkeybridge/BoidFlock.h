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
/* BoidFlock.cxx contains the implementation of class BoidFlock
 *
 * @author Istvan Barakonyi
 *
 * $Id: BoidFlock.h 3426 2004-07-14 12:11:27Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _BOIDFLOCK_H_
#define _BOIDFLOCK_H_

// Inventor classes
#include "Inventor/fields/SoSFVec3f.h"
#include "Inventor/fields/SoSFInt32.h"
#include "Inventor/fields/SoSFFloat.h"
#include "Inventor/fields/SoSFString.h"
#include "Inventor/fields/SoMFVec3f.h"
#include <Inventor/sensors/SoFieldSensor.h>

// own classes
#include "../../characters/SoPuppeteerKit.h"

// for DLL export
#include "monkeybridge.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// Data structure for boid behavior parameters
typedef struct t_boid {
	SbVec3f loc;
	SbVec3f dir;        // cur direction
	SbVec3f delta_pos;	// change in position from flock centering
	SbVec3f delta_dir;	// change in direction
	int delta_cnt;	    // number of boids that influence this delta_dir
	float speed;
	float yaw,pitch,roll,dyaw;
} Boid;

/// The BoidFlock class is an implementation of a flock of birdoids, which are added to the puppeteer as puppets
class MONKEYBRIDGE_API BoidFlock : public SoPuppeteerKit
{
    SO_KIT_HEADER(BoidFlock);

public:

    /// Boid appearance parameters
    SoSFString filename;
    SoSFInt32 number;

    /// Boid behavior parameters
    SoSFFloat influenceRadiusSquared;
    SoSFFloat collisionFraction;
    SoSFFloat angleTweak;
    SoSFFloat pitchToSpeedRatio;
    SoSFFloat normalSpeed;

    /// Movement goal and obstacle information
    SoSFVec3f goal;
    SoMFVec3f obstaclePosition;
    SoMFFloat obstacleRadius;

    BoidFlock();
    ~BoidFlock();    

SoINTERNAL public:

    static void initClass();

protected:

    /// Field sensors
    SoFieldSensor *numberSensor;
    SoFieldSensor *filenameSensor;

    /// Quicklist to access the position and orientation information of boids
    SbPList transformList;

    /// Array of boid distances --> number X number
    SbPList distanceList;

    /// Storing the boids
    SbPList boidList;

    /// Setting up field connections etc.
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    // Static callback functions
    static void generateFlockCB(void *data,SoSensor *sensor);

    /// Timer sensor callback function
    static void timerSensorCB(void *data,SoSensor *sensor);

    /// Get boid distance
    float getBoidDistance(int num1,int num2);

    /// Set boid distance
    void setBoidDistance(int num1,int num2,float dist);

    /// Get boid
    Boid* getBoid(int num);

    /// Generate random number
    float rnd(float max=1.0f);

    /// Animate boids
    void animate();

    SbMatrix MatrixMult(const SbMatrix & a, const SbMatrix & b);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _BOIDFLOCK_H_

