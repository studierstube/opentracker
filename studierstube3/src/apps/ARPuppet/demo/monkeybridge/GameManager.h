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
/* GameManager.h contains the GameManager component
 *
 * @author Istvan Barakonyi
 *
 * $Id: GameManager.h 3736 2004-09-20 17:44:47Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

// for the DLL export
#include "monkeybridge.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** GameManager is a nodekit responsible for starting and stopping the game as well as the following tasks:
        - countdown
        - enabling/disabling placing tiles
        - announcing winner

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class MONKEYBRIDGE_API GameManager : public SoBaseKit
{
    SO_KIT_HEADER(GameManager);

    SO_KIT_CATALOG_ENTRY_HEADER(root);
    SO_KIT_CATALOG_ENTRY_HEADER(texture);
    SO_KIT_CATALOG_ENTRY_HEADER(material);
    SO_KIT_CATALOG_ENTRY_HEADER(font);
    SO_KIT_CATALOG_ENTRY_HEADER(text);

public:

    /// Fields storing game parameters
    /*@{*/

    /// Start/stop the game manually, sets the <em>started</em> field
    SoSFTrigger init;

    /// Trigger to inform other components that the countdown has finished already after starting the game
    SoSFBool countdownFinished;

    /// Field to inform other components that the game has started/stopped
    SoSFBool started;

    /// Number to start the countdown from upon startup
    SoSFInt32 countdownFrom;

    /// Speed of countdown
    SoSFFloat speed;

    /// Auxiliary field connected to the time counter component
    SoSFInt32 countdown;

    /// Number of seconds to display messages except countdown info
    SoSFFloat displayTime;

    /*@}*/

    /// Fields storing various user information
    /*@{*/

    /* Indicates user status (alive or dead). 
       If alive[i] == TRUE, then user#i is alive --> end of game, user#i won.
       If == FALSE, then he is dead --> end of game, user#i lost.
     */
    SoMFInt32 alive;

    /* Indicates that a user has reached the target tile --> end of game, user won
       Connected from the <em>wayCompleted</em> field of the scene manager
     */
    SoMFBool win;

    /* Output field connected to scene manager's <em>enabled</em> field to enable/disable the placement of tiles.
       When the game is stopped or it has started but the countdown hasn't finished yet, it's FALSE. When the countdown
       has finished, it's TRUE. While playing the game it does not send any updates, so the scene manager's <em>enabled</em>
       field can be externally manipulated despite the field connection.
     */
    SoMFBool enabled;

    /*@}*/

    GameManager();

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:

    /// Field sensor
    SoFieldSensor *initSensor;
    SoFieldSensor *countdownSensor;
    SoFieldSensor *aliveSensor;
    SoFieldSensor *winSensor;

    /// Timer sensor
    SoTimeCounter *counter;

    /// Alarm sensor
    SoAlarmSensor *displayTimer;
    
    virtual ~GameManager();

    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Static callback for field sensors
    static void initCB(void *data,SoSensor *sensor);
    static void countdownCB(void *data, SoSensor *sensor);
    static void aliveCB(void *data,SoSensor *sensor);
    static void winCB(void *data,SoSensor *sensor);
    static void displayTimerCB(void *data,SoSensor *sensor);

    // Things to do when game is finished
    void finish(const char *str);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _GAMEMANAGER_H_
