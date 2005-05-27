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
/* ChoreographerKit.h contains the definition of class Choreographer
 *
 * @author Istvan Barakonyi
 *
 * $Id: Choreographer.h 4209 2005-05-19 09:06:39Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _CHOREOGRAPHER_H_
#define _CHOREOGRAPHER_H_

// for the DLL export
#include "monkeybridge.h"

// Inventor classes
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>

// own classes
#include "../../characters/SoChoreographerKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** Choreographer is derived SoChoreographerKit and contains the monkeybridge game-specific
    choreographer commands 

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class MONKEYBRIDGE_API Choreographer : public SoChoreographerKit
{
    SO_KIT_HEADER(Choreographer);

public:

    /// Useful enum values for easier configuration and better readability
    enum MotionType { WALK=0, WALK_UP, WALK_DOWN, LOOKAROUND, DUNNO, WIN, LOSE, JUMP_UP, JUMP_DOWN, FALL_BOARD, FALL_WATER };

    // Initializes / resets the the choreographer --> move all characters to the start tile
    SoSFTrigger init;

    /// Number of characters representing the user --> automatically extracted from puppeteers by searching for node with type 'Character' 
    SoSFInt32 characterNum;

    /* Indicates the ID of the character that has just fallen down because of a large height difference
       It is connected to the director.
     */
    SoSFInt32 fallenDown;

    /// Character motion parameters
    /*@{*/

    /// Scaling motion parameters (connected to global game scale parameter)
    SoSFFloat globalScale;

    /// Height difference interval determining whether the character should walk or jump over to the next tile, or fall down and die
    SoSFFloat heightThreshold;

    /// Threshold indicating the distance between the end and start hotspots on the [x,y] plane. It helps decide the animation to trigger.
    SoSFFloat tileGapThreshold;
          
    /// Attribute to control and tune the walking speed of the characters
    SoMFInt32 speed;

    /*@}*/

    /// Constructor
    Choreographer();

    /// Destructor
    ~Choreographer();

    /// Public, static utility function to parse user ID from string containing text "playerN"
    static bool getUserIdFromString(const char *cmd,int &id);

    /// Constant prefix string for the id value of the character puppeteers
    static const SbString characterIDString;

SoINTERNAL public:

    static void initClass();

protected:

    /// Field sensors
    SoFieldSensor *initSensor;

    /// Static callback functions for field sensors
    static void initCB(void *data,SoSensor *sensor);

    /// Setting up field connections etc.
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Game-specific command implementation
    virtual bool implementCommand();

    /// Game-specific command choreography
    virtual bool choreograph();

    /// Get speed parameter for given motion
    int getSpeed(int animationID);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_CHOREOGRAPHER_H_

