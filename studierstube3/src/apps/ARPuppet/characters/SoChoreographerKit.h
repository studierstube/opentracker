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
/* SoChoreographerKit.h contains the definition of class SoChoreographerKit
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoChoreographerKit.h 3792 2004-09-29 22:39:35Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _SOCHOREOGRAPHERKIT_H_
#define _SOCHOREOGRAPHERKIT_H_

// for the DLL export
#include "ARPuppet.h"

// general C++ classes
#include <string>

// Inventor specific classes
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/SbPList.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>

// own classes
#include "SoCommandParserKit.h"
#include "SoPuppeteerKit.h"

using namespace std;

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoChoreographerKit is a nodekit representing a the choreographer component in the ARPuppet framework.

The parsing, choreographing and execution of high-level commands are implemented here.

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class ARPUPPET_API SoChoreographerKit : public SoCommandParserKit
{
    SO_KIT_HEADER(SoChoreographerKit);

    // node kit parts -> scene graphs to attach
    SO_KIT_CATALOG_ENTRY_HEADER(puppeteers);

public:

    /** Validate the data loaded into the field <em>puppeteers</em>
        This field got introduced only because Coin treats field sensors like node sensors if the field contains a
        node: whenever there is a change in the contained node (e.g. position), it's field sensor triggers unwantedly,
        therefore we need to validate the data of the field <em>puppeteers</em>.
     */
    SoSFTrigger loadPuppeteers;

    /** @name Constructing commands from fields
     */
    /*@{*/
    
    /// Name of the commands
    SoMFString commandType;

    /// Target list (puppeteers' name)
    SoMFString targetList;

    /// Parameter list
    SoMFString parameterList;

    /// List of currently loaded puppeteers
    SoMFString puppeteerName;

    /// List of events to register for event generation
    SoMFString registerEvent;

    /// List of events to deregister
    SoMFString deregisterEvent;

    /*@}*/

    /// The constructor initializes all the fields, creates sensors and calls setUpConnection to set up field connections
    SoChoreographerKit();

    /// Destructor
    ~SoChoreographerKit();

    /** @name Common public utilities
     */
    /*@{*/

    /// Add new puppeteer to puppeteer list
    void addPuppeteer(SoPuppeteerKit *puppeteer);

    /// Remove puppeteer from the repository, returns true if succeeded, otherwise false
    bool removePuppeteer(SoPuppeteerKit *puppeteer);

    /// Remove puppeteer from the repository, returns true if succeeded, otherwise false
    bool removePuppeteer(int num);

    /// Getter function to retrieve puppeteer by its name
    SoPuppeteerKit *getPuppeteer(const char *name);

    /// Getter function to retrieve puppeteer by its child number
    SoPuppeteerKit *getPuppeteer(int num);

    /// Get number of puppeteers
    int getPuppeteerNum();

    /*@}*/

SoINTERNAL public:

    static void initClass();

protected:

    /// Storing the attached puppeteers
    SbPList puppeteerInventory;

    /** @name Helper objects
     */
    /*@{*/
    
    SbPList finishedSubCommandSensorList;
    SbIntList targetNumberQueue;
    SbPList hotspotSelectorList;
    SbStringList hotspotInfoList;
    SbIntList controlModeList;

    /*@}*/

    /** @name Helper variables
     */
    /*@{*/

    /// Macro parsing tokens
    enum { PUPPETEER=0, STATION, USER, PEN, PIP, NODE, HOTSPOT, HOTSPOTABSOLUTE, HOTSPOTOFFSET };
    string parseToken[NODE+1];

    /// List of command targets
    SbStringList targetNameList;
    
    /*@}*/

    /** @name Field sensors
     */
    /*@{*/

    SoFieldSensor *loadPuppeteersSensor;
    SoFieldSensor *registerEventSensor;
    SoFieldSensor *deregisterEventSensor;

    /*@}*/

    /** @name Attribute change static callbacks
     */
    /*@{*/

    static void loadPuppeteersCB(void *data,SoSensor *sensor);
    static void registerEventCB(void *data,SoSensor *sensor);
    static void deregisterEventCB(void *data,SoSensor *sensor);
    static void finishedSubCommandCB(void *data,SoSensor *sensor);
    
    /*@}*/

    /** @name Attribute change overloadable callback functions
     */
    /*@{*/

    virtual void loadPuppeteersChanged();

    /*@}*/
    
    /// Parsing functions
    /*@{*/

    bool convertCommand();
    void replaceMacros();
    bool getMacroObject(string &macroString,void*& result,int &typeOut,int &startPos,int &endPos,int searchPos=0,int typeIn=-1);
    bool getAttributeString(string &result,int &endPos,string &attrStr,void *object,void *object2,int type);
    bool getVectorString(string &result,int &endPos,string &attrStr,SbVec3f vec);
    bool getRotationString(string &result,int &endPos,string &attrStr,SbRotation rot);
    bool getFieldValueString(string &result,SoField *field,int index=-1);
    bool parseTargetList(const char *cmd);

    /*@}*/

    /// Utility functions
    /*@{*/

    bool collision_aabb(SbVec3f min0,SbVec3f max0,SbVec3f min1,SbVec3f max1);
    bool collision_obb(SbVec3f min0,SbVec3f max0,SbRotation rot0,SbVec3f min1,SbVec3f max1,SbRotation rot1);

    /*@}*/

    /// Detach sensor elements of triggerSensorList and empty the list
    void resetTriggerList();

    /// @name Virtual functions to be overloaded
    /*@{*/

    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Reset variables for command parsing and execution
    virtual void resetCommandProcess();

    /// Reset variables before processing next subcommand
    virtual void resetSubCommandProcess();

    /// Processes the current command: parsing, executing, setting the response
    virtual void processCommand(int index);

    virtual bool processFinishedCommand(const char *current,const char *finished,int currentIndex);

    /// Executes the current command.
    virtual bool executeCommand();

    /// Implement own command
    virtual bool implementCommand();

    /* Compose the mid-level choreography of high-level commands for all the puppeteers
       This function can change, "choreograph" all command components (target list --> targetNameList, command string -> currentCommand) 
       or turn the command into a compound command and issue them to the command queue
       The function returns TRUE  if it's not a compound command and can be immediately sent to puppeteers
                            FALSE if it's a compound command that consists of one or several other commands to be reissued
     */
    virtual bool choreograph();

    /** Constructs the triggering structure for the finishedCommand notification. 
        Here it calls the overloadable virtual function getPuppetTrigger()
      */
    virtual void constructTrigger();

    /** Add the values of the command field to the command queue depending on the <em>dataNeedsValidation</em> setting:
        - if (<em>dataValid</em> has been triggered && <em>dataNeedsValidation</em>==true) 
          then combine the value of fields <em>targetList</em>, <em>commandType</em> and <em>patameterList</em> into 
          commands and add them to the command queue
        - if (there is new value in field <em>command</em> && <em>dataNeedsValidation</em>==false) 
          then add commands from field <em>command</em> to the command queue
      */    
    /*@{*/

    virtual void commandChanged();
    virtual void dataValidChanged();

    /*@}*/

    /*@}*/
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_SOCHOREOGRAPHERKIT_H_

