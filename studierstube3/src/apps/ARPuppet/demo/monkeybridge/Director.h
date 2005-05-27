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
/* Director.h contains the director component
 *
 * @author Istvan Barakonyi
 *
 * $Id: Director.h 3767 2004-09-27 00:37:26Z bara $ 
 * @file
 */
/* ======================================================================== */

#ifndef _DIRECTOR_H_
#define _DIRECTOR_H_

// general C++ classes
#include <string>

// Inventor classes
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>

// for the DLL export
#include "monkeybridge.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** Director is a nodekit responsible for the following tasks:
    ...

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class MONKEYBRIDGE_API Director : public SoBaseKit
{
    SO_KIT_HEADER(Director);

public:

    /// Enum values for the state engine
    enum State { INITIALIZATION=0, GAMESTARTED, HS_START, CHECK_HS, DECIDE, HS_CENTER, HS_END, GAMEFINISHED };

    /// Enum values for state engine events
    enum Event { HOTSPOTSETEVENT=0, FINISHEDCOMMANDEVENT };

    Director();
    ~Director();

    /// Trigger to tell the director to initialize itself upon game start or restart
    SoSFTrigger init;

    /// Number of characters representing the user --> connected from the <em>characterNum</em> field of the choreographer
    SoSFInt32 characterNum;

    /// Commands for the choreographer --> this field is connected to the choreographer's <em>command</em> field
    SoMFString command;

    /// Internal status of each character's state machine
    SoMFInt32 state;

    /// The finishedCommandData field connected from the choreographer
    SoMFString finishedCommandData;

    /* Indicates that the hotspot information for user#(hotspotSet.getValue()) has been set and ready to be used in the motion
       This field is connected from the scene manager.
     */
    SoMFInt32 hotspotSet;

    /* The user ID of the player that has just started to move from the center hotspot to the end hotspot of the tile
       This field is connected to the scene manager's <em>playerCompletedTile</em> field.
     */
    SoSFInt32 playerCompletedTile;

    /* The user ID of the player that has just started to move from the start hotspot to the center hotspot of the tile,
       therefore needs to know where the next tile to know the direction to move into after reaching the center.
       This field is connected to the scene manager's <em>seekNextTile</em> field.
     */
    SoSFInt32 seekNextTile;

    /// Indicates whether user #i has reached his end tile. This field is connected from the scene manager.
    SoMFBool targetReached;

    /* Indicates whether the value in the scene manager's <em>nextTileCoordinate</em> value of user #i represents 
       a valid tile. This field is connected from the scene manager.
     */
    SoMFBool nextTileValid;

    /// Indicates whether the characters are alive or dead. It is connected to the game manager.
    SoMFBool alive;

    /// Indicates whether a character has won the game or not. It is connected to the game manager.
    SoMFBool win;

    /* Indicates the ID of the character that has just fallen down because of a large height difference
       It is connected from the choreographer.
     */
    SoSFInt32 fallenDown;

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:

    bool initialized;

    /// Field sensors
    SoFieldSensor *initSensor;
    SoFieldSensor *hotspotSetSensor;
    SoFieldSensor *finishedCommandDataSensor;
    SoFieldSensor *fallenDownSensor;

    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Static callback functions for field sensors
    static void initCB(void *data,SoSensor *sensor);
    static void hotspotSetCB(void *data,SoSensor *sensor);
    static void finishedCommandDataCB(void *data,SoSensor *sensor);
    static void fallenDownCB(void *data,SoSensor *sensor);

    /// State engine and event function: go to next state in the state engine of the userID'th character
    void stateEngine(int userID,int eventID);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _DIRECTOR_H_
