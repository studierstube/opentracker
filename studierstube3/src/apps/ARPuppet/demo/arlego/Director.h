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
 * $Id: Director.h 4127 2005-01-20 14:03:28Z bara $ 
 * @file
 */
/* ======================================================================== */

#ifndef _DIRECTOR_H_
#define _DIRECTOR_H_

// Inventor classes
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SbStringList.h>

// STL classes
#include <string>

// for the DLL export
#include "arlego.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** Director is a nodekit responsible for the following tasks:
    ...

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class ARLEGO_API Director : public SoBaseKit
{
    SO_KIT_HEADER(Director);

public:

    /// Constants for the LEGO robot model display modes
    enum DisplayMode { HIDE_ALL=0, SHOW_ALL, SHOW_CURRENT_ONLY, SHOW_UP_TO_CURRENT };

    /// Constants for minifig behavior
    enum State { PUC_DEFAULT=0, PUC_NOINTRO_FOR_RCX, RCX_DEFAULT, RCX_NOINTRO, REMOTE };

    /// Constants for tile placement animation mode
    enum TilePlaceAnimMode { INTRO_ONLY=0, PLACE_ONLY, INTRO_AND_PLACE };

    Director();
    ~Director();

    /// Fields effecting the overall construction
    /*@{*/

    /* Name of the configuration file storing all the construction step descriptions in the following format:
         0th line: serves as a separator, always starts with a # character, after it any text can be written, it will be handled as a comment
         1st line: model parts to show (space separated list of index numbers starting from 0)
         2nd line: augmentation parts to show (space separated list of index numbers starting from 0)
         3rd line: animation hint, which is string storing a textual hint for the agent about the type of behavior it should perform while executing this step
                   possible values: demonstrate, explain, place1, place2, turnaround
         4th-...th line: explanation text consisting of several lines (the lines together make an SoMFString value, 
                         each line should be max. 26 characters long to fit the HUD)
     */
    SoSFString constructionFile;

    /// Number of the current construction step
    SoSFInt32 constructionStep;

    /// Maximum construction step index (set only after having parsed the config file specified in <em>constructionFile</em>)
    SoSFInt32 maxConstructionStepIndex;

    /*@}*/

    /// LEGO model-related fields
    /*@{*/

    /// Output parameter connected to the <em>whichPuppets</em> field of the LEGO puppeteer. Its values are effected by the current display mode.
    SoMFInt32 showModelPart;

    /// Output parameter connected to the <em>whichAttributes</em> field of the LEGO puppeteer. Its values are effected by the current display mode.
    SoMFInt32 showAugmentation;

    /* Influences the way the construction information is presented: if false ( = no presenter ) simply shows the steps in a linear way without a virtual presenter,
       while true ( = use presenter ) uses a virtual character to present how to carry out the construction steps
     */
    SoSFBool usePresenter;

    /// Show all model parts up to the specified part number
    SoSFEnum displayMode;

    /* Number of puppets that are NOT parts of the construction but are permanently there (e.g. the RCX puppet). They are not turned on/off
       while constructing the LEGO model. These permanent puppets have to be configured to be the first <em>permanentPuppetNum</em> children
       of the SoMultiSwitch node in the scenegraph attached to the <em>puppets</em> field
     */
    SoSFInt32 permanentPuppetNum;

    /* Number of attributes that are NOT parts of the construction but are permanently there (e.g. battery level). They are not turned on/off
       while constructing the LEGO model (but can be turned on/off upon user request). These permanent attrbiutes have to be configured to be 
       the first <em>permanentAttributeNum</em> children of the SoMultiSwitch node in the scenegraph attached to the <em>attributes</em> field
     */
    SoSFInt32 permanentAttributeNum;

    /// Bounding box size threshold: if any of the BB sides of the new tile are larger than this number, hold the tile with two hands, otherwise with one hand
    SoSFFloat sizeThreshold;

    /*@}*/

    /// Minifig behavior-related fields
    /*@{*/

    /* Minifig behavior states:
         - PUC_DEFAULT: TODO: description
         - PUC_NOINTRO_FOR_RCX: TODO: description
         - RCX_DEFAULT: TODO: description
         - RCX_NOINTRO: TODO: description
         - REMOTE: TODO: description
     */
    SoSFInt32 state;

    /* This field receives events about the current position of the LEGO minifig character. These events are defined in the Event constant enum
       of the choreographer module. This field is connected from the <em>minifigPositionEvent</em> field of the choreographer.
     */
    SoSFInt32 minifigEvent;

    /// Commands for the choreographer --> this field is connected to the choreographer's <em>command</em> field
    SoMFString command;

    /// The <em>finishedCommandData</em> field connected from the choreographer
    SoMFString finishedCommandData;

    /// If triggered, resets the "tile already demonstrated" flag of the character
    SoSFTrigger showAgainStep;

    /// Enable/disable minifig event processing
    SoSFBool enableEvents;

    /// Number of seconds to elapse before fixing the tile's position
    SoSFTime eventTimeOut;

    /* Explanation text associated with the current construction step. It can be used for display on a HUD and speech synthesis 
       (both of them are optional and connected to external modules)
     */
    SoMFString explanation;

    /*@}*/

    /// LEGO RCX related properties
    /*@{*/

    SoSFBool RCX_engineA;
    SoSFBool RCX_engineB;
    SoSFBool RCX_engineC;
    SoSFBool RCX_engineDirA;
    SoSFBool RCX_engineDirB;
    SoSFBool RCX_engineDirC;
    SoSFInt32 RCX_enginePowerA;
    SoSFInt32 RCX_enginePowerB;
    SoSFInt32 RCX_enginePowerC;

    /*@}*/


SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:

    /// Storing information for the choreographer on which puppet in SoLegoPuppeteer to attach to the tileToPlace puppeteer
    int tilePuppetNum;

    /// Stores current animation hint string
    SbString *currentAnimationHint;

    /// Timer to re-enable events
    SoAlarmSensor *eventEnableTimer;

    /// Lists
    SbPList modelPartList;
    SbPList augmentationList;
    SbPList explanationList;
    SbStringList animationHintList;

    /// Field sensors
    SoFieldSensor *constructionFileSensor;
    SoFieldSensor *constructionStepSensor;
    SoFieldSensor *showAgainStepSensor;
    SoFieldSensor *usePresenterSensor;
    SoFieldSensor *displayModeSensor;
    SoFieldSensor *minifigEventSensor;
    SoFieldSensor *finishedCommandDataSensor;
    SoFieldSensor *RCX_engineASensor;
    SoFieldSensor *RCX_engineBSensor;
    SoFieldSensor *RCX_engineCSensor;
    SoFieldSensor *RCX_engineDirASensor;
    SoFieldSensor *RCX_engineDirBSensor;
    SoFieldSensor *RCX_engineDirCSensor;
    SoFieldSensor *RCX_enginePowerASensor;
    SoFieldSensor *RCX_enginePowerBSensor;
    SoFieldSensor *RCX_enginePowerCSensor;
    
    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Static callback functions for field sensors
    static void constructionFileCB(void *data,SoSensor *sensor);
    static void showCurrentStepCB(void *data,SoSensor *sensor);
    static void minifigEventCB(void *data,SoSensor *sensor);
    static void finishedCommandDataCB(void *data,SoSensor *sensor);
    static void eventEnableCB(void *data,SoSensor *sensor);
    static void RCXcommandCB(void *data,SoSensor *sensor);

    /// Checks whether the current construction step is in valid range or not
    bool isConstructionStepValid();

    /// Parses give string for space-separated integers and adds them to the given integer list
    void parseIntList(std::string &parseString,SbIntList *indexList,int lineNumber);

    /// Disable new events until a specific time has passed
    void disableEvents();

    /// Output explanation text belonging to the current construction step
    void displayExplanation();

    /// Display model parts and augmentation associated with the current construction step based on the display mode settings
    void displayModelPartAndAugmentation(bool isPlacingTile=false);

    /// Send animation command to choreographer via the <em>command</em> field, which is connected to the Choreographer component
    void sendAnimationCommand(int puppetNum,int animMode,std::string cmd);

    /// State engine and event function: go to next state in the state engine of the minifig character based on the ID of the most recent
    void stateEngine(int eventID);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _DIRECTOR_H_
