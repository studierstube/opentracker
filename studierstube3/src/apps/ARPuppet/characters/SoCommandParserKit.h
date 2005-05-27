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
/* SoCommandParserKit.h contains the definition of SoCommandParserKit, 
 * the general command parser class for puppeteers and utilities
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoCommandParserKit.h 3677 2004-09-08 22:51:14Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _SOCOMMANDPARSERKIT_H_
#define _SOCOMMANDPARSERKIT_H_

// general C++ classes
#include <string>
#include <vector>

// Inventor specific classes
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/SbPList.h>

// own classes
#include "ARPuppet.h" // for the DLL export

using namespace std;

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoCommandParserKit is a nodekit representing a general command parser class for puppeteers and utilities

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class ARPUPPET_API SoCommandParserKit : public SoBaseKit
{
    SO_KIT_HEADER(SoCommandParserKit);

public:

    /** @name Input fields
     */
    /*@{*/

    /** Puppeteer string ID
        NO dot character ('.') is allowed in the id.
      */
    SoSFString id;

    /// List of the commands
    SoMFString commandList;

    /// List of the command formats
    SoMFString commandFormatList;

    /// Commands to be processed at the same time. The derived classes will implement the behavior.
    SoMFString command;

    /// Response from the module.
    SoMFString response;

    /// Indices telling which command element generated which response element
    SoMFInt32 responseIndex;

    /** If TRUE, the value written in the field <em>animation</em> will be assigned when the trigger 
        <em>dataValid</em> is fired.
     */
    SoSFBool dataNeedsValidation;

    /** If the value of field <em>dataNeedsValidation</em> is TRUE, the value written in the field 
        <em>animation</em> will be assigned when this trigger is fired.
      */
    SoSFTrigger dataValid;

    /** All the commands remaining in the animation queue will be removed upon calling this trigger.
        The currently executed commands will be finished.
      */
    SoSFTrigger purgeQueue;

    /* Determines whether we should wait with the execution of the next command sequence until it's finished 
       (TRUE --> synchronous mode) or not (FALSE --> asynchronous mode)
     */
    SoSFBool synchronize;

    /// Time to pass between commandLoop() calls
    SoSFTime timerInterval;

    /*@}*/

    /** @name Public output fields
        The values of these fields are written by the SoCommandParserKit node, modifying the values of these fields has no effect.
        These fields are public, they can be used by other classes or applications.
     */
    /*@{*/

    /// This trigger gets fired when a subcommand has finished
    SoSFTrigger finishedSubCommand;

    /// This field stores the command monitor's last finished subcommand
    SoSFString finishedSubCommandData;

    /** This trigger is fired when a certain command is finished. The field <em>finishedCommandData</em> will store
        the latest finished command string.
      */
    SoSFTrigger finishedCommand;

    /// This field stores the latest finished command string.
    SoMFString finishedCommandData;

    /// This trigger is fired when all the commands issued at once in the command field have been finished
    SoSFTrigger finishedAll;

    /** @name Private output fields
        The values of these fields are used by the SoCommandParserKit node, modifying the values of these fields has no effect.
        These fields are private, users should not use them.
     */
    /*@{*/
    
    /// Stores the global time
    SoSFTime globalTime;

    /*@}*/

    /// Constructor
    SoCommandParserKit();

    /// Destructor
    ~SoCommandParserKit();

    /// Public functions to append commands to the command queue
    /*@{*/

    void appendCommand(const char *cmd,...);
    void appendCommand(vector<string> *cmdList);

    /*@}*/

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

public:

    /// Utility function to search for nodes with a given type in subgraph specified
    bool isType(SoNode *node,SbName name);

    /// Returns last found node after the <em>isType</em> function's search
    SoNode* getFoundNode() { return foundNode; }

protected:

    /** Flag indicating whether the commands and command formats are specified externally (the commandList field 
        has non-empty elements) or internally ((the commandList field has no or just 1 empty element)
     */
    bool setInternalCmds;

    /** @name Command parsing helpers
        These fields are related to the animation command parser.
     */
    /*@{*/

    /// Lists storing the parsed parameters
    SbStringList strParamList;
    SbIntList intParamList;
    SbPList floatParamList;

    /// A single string containing the syntax of all commands
    string commandString;

    /// Currently parsed command string
    string currentCommand;

    /// Type of currently parsed command
    string currentType;

    /// Syntax of the currently parsed command in use
    string currentFormat;    

    /// List of calculated result strings
    SbStringList responseList;

    /// List of the command indices of the calculated result strings
    SbIntList responseIndexList;

    /// Currently calculated result strings stored in a list
    SbStringList subResponseList;

    /// The index of the command in the command queue to which the calculated subResponseList belongs
    int currentQueueIndex;

    /// Stores the result of a searched typed node (used when executing commands)
    SoNode *foundNode;

    /*@}*/

    /** @name Command queue helpers
        These fields are related to the command queue.
     */
    /*@{*/

    /// Command queue: a list to store "sub-command-lists" i.e. animation sequences that need to be executed sequentially
    SbPList commandQueue;

    /// Pointer to a list to store commands that need to be executed in parallel
    SbPList *subCommandQueue;

    /// Indicates that we have just entered the idle state
    bool idleEntered;

    /** Attribute set by the executed commands to indicate whether we should go into the idle state after the execution of 
        this command in case there are no commands left. It's reset before the execution of each command to 'true'.
      */
    bool goIdle;

    /*@}*/

    /// @name Command monitor
    /*@{*/

    /// Number of animation durations to monitor
    int durationNum;

    /// Number of animation triggers to monitor
    int triggerNum;

    /// List to store the time of the pending duration commands
    SbPList durationTimeList;

    /// List to store the pending duration command strings
    SbStringList durationCommandList;

    /*@}*/
    
    /// @name Field sensors + timer sensor
    /*@{*/

    SoFieldSensor *idSensor;
    SoFieldSensor *commandSensor;
    SoFieldSensor *dataValidSensor;
    SoFieldSensor *purgeQueueSensor;
    SoFieldSensor *timerIntervalSensor;
    SoTimerSensor *timerSensor;

    /*@}*/

    /// @name Field sensors
    /*@{*/

    static void idCB(void *data,SoSensor *sensor);
    static void commandCB(void *data,SoSensor *sensor);
    static void dataValidCB(void *data,SoSensor *sensor);
    static void purgeQueueCB(void *data,SoSensor *sensor);
    static void timerIntervalCB(void *data,SoSensor *sensor);
    static void timerSensorCB(void *data, SoSensor *);

    /*@}*/

    /** @name Common routines
     */
    /*@{*/
    
    /// Getter functions
    const char* getStringParam(int i);
    int getIntParam(int i);
    float getFloatParam(int i);
    bool isCommandType(const char* cmd);

    void removeAllListItems(SbPList *list);
    void setCommandList();

    /// This function converts the time <em>dur</em> given in milliseconds to the Inventor type SbTime(nt32_t sec,long usec)
    SbTime getTimeValue(int dur);
    
    /// Add trigger feedback request for "command finished" feedback
    void addDurationMonitor(int duration=0);
    void addTriggerMonitor();
    void noTrigger();

    /// Add string to current sub-response list
    void addResponse(const char *str);
    
    /*@}*/

    /// @name Common routines that don't need to be overloaded
    /*@{*/

    /// Command loop function called by the timer
    void commandLoop();

    /// Process finished subcommand
    void subCommandFinished(const char *cmd);

    /// Gets called when a trigger command has finished
    void commandTriggered(const char *cmd);

    /// Dump the message queue for debugging purposes
    void dumpMessageQueue();

    /// @name Commands to construct a compound command
    /*@{*/
    
    void addCompoundCommand(const char *cmd,...);
    void addCompoundCommand(vector<string> *cmdList);

    /*@}*/

    /// Parses and stores the type and the parameters of the current command
    bool parseCommand();

    /** Parses and searches for the type and format of the current command in the given 
        SoCommandParserKit's command list and command format list
      */
    bool parseCommandType(SoCommandParserKit *commandParserKit);

    /*@}*/

    //------------------------------------------------------------------------------------------
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

    /// Processes the last finished command to manage the compound command queues
    virtual bool processFinishedCommand(const char *current,const char *finished,int currentIndex);

    /// Contains routines that are needed to be executed in each command loop
    virtual void commandLoopFunction();

    /// Executes the current command.
    virtual bool executeCommand();

    /// Determines what to do in the idle state
    virtual void idle();

    /// Evaluates the response to the current command
    virtual void evaluateResponse();

    /// Constructs the triggering structure for the finishedCommand notification.
    virtual void constructTrigger();

    /** Provides customized comparison with the currently executed command if object
        returns specific finishedCommand messages
      */
    virtual bool translateFinishedCommand(const char *current,const char *finished);

    /// @name Attribute change virtual callback function that can be overloaded in derived classes
    /*@{*/

    /// ID of the object has changed
    virtual void idChanged();

    /*@}*/

    /** Add the values of the command field to the command queue depending on the <em>dataNeedsValidation</em> setting:
        - if (<em>dataValid</em> has been triggered && <em>dataNeedsValidation</em>==true) 
          then add commands from field <em>command</em> to the command queue
        - if (there is new value in field <em>command</em> && <em>dataNeedsValidation</em>==false) 
          then add commands from field <em>command</em>
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

#endif //_SOCOMMANDPARSERKIT_H_
