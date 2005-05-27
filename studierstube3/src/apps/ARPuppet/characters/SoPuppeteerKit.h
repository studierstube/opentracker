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
/* SoPuppeteerKit.h contains the definition of SoPuppeteerKit, 
 * the general Augmented Reality Agent class
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoPuppeteerKit.h 4007 2004-10-27 07:50:43Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _SOPUPPETEERKIT_H_
#define _SOPUPPETEERKIT_H_

// general C++ classes
#include <string>

// Inventor specific classes
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFBitMask.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoInterpolate.h>
#include <Inventor/engines/SoOneShot.h>
#include <Inventor/engines/SoComputeBoundingBox.h>
#include <Inventor/engines/SoConcatenate.h>
#include <Inventor/engines/SoCompose.h>

// Stb specific classes
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/misc/SoFanIn.h>
#include <stbapi/misc/SoMultiSwitch.h>
#include <stbapi/misc/SoRotateVector.h>
#include <stbapi/interaction/SoDragKit.h>
#include <stbapi/resource/viewer/MultRotRot.h>

// own classes
#include "ARPuppet.h" // for the DLL export
#include "SoCommandParserKit.h"
#include "SoTransformCapture.h"

using namespace std;

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoPuppeteerKit is a nodekit representing a general Augmented Reality Agent. 

All AR Agents possessing any special behavior or requiring additional functionalities need to derive from this class.

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class ARPUPPET_API SoPuppeteerKit : public SoCommandParserKit
{
    SO_KIT_HEADER(SoPuppeteerKit);

    // node kit parts -> scene graphs to attach
    SO_KIT_CATALOG_ENTRY_HEADER(rootSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(puppetRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(auxRoot);

    SO_KIT_CATALOG_ENTRY_HEADER(transformPosBase);
    SO_KIT_CATALOG_ENTRY_HEADER(transformPosOffset);
    SO_KIT_CATALOG_ENTRY_HEADER(transformRotOffset);
    SO_KIT_CATALOG_ENTRY_HEADER(transformRotBase);
    SO_KIT_CATALOG_ENTRY_HEADER(transformScale);

    SO_KIT_CATALOG_ENTRY_HEADER(puppets);
    SO_KIT_CATALOG_ENTRY_HEADER(dragTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(attributes);

    SO_KIT_CATALOG_ENTRY_HEADER(vfSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(vfRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(vfMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(vfDrawStyle);    
    SO_KIT_CATALOG_ENTRY_HEADER(viewingFrustums);

    SO_KIT_CATALOG_ENTRY_HEADER(velSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(velRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(velTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(velMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(velDrawStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(velocityVis);
    
    SO_KIT_CATALOG_ENTRY_HEADER(bbSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(bbRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(bbMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(bbDrawStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(bbTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(bbCube);


public:

    /** Enum values for the field <em>controlMode</em> 
        --> SCRIPT is intentionally 0, it's not used as a bit anyways!
      */
    enum ControlMode { INVALID=1, SCRIPT=2, STATION=4, DRAG=8, EXTERNAL=16 };

    /// Modes for hotspot queries
    enum HotspotMode { LOCAL_HS, ABSOLUTE_HS, OFFSET_HS };

    /// Enum value for the field <em>boundingBoxMode</em>
    enum BoundingBoxMode { NONE, AABB, OBB };
    
    /** Defines the indexes of attached puppet scenegraphs to be displayed (starting from 0),
        e.g. the value [0,2,3] displays all the 4 attached puppets but the 2nd one (having index #1).
      */
    SoMFInt32 whichPuppets;

    /** Defines the indexes of attached attribute scenegraphs to be displayed (starting from 0),
        e.g. the value [0,2,3] displays all the 4 attached attributes but the 2nd one (having index #1).
      */
    SoMFInt32 whichAttributes;

    /** Set control mode to control the puppeteer's pose (separately adjustable for position and orientation): 
          SCRIPT: default mode, positionOffset/rotationOffset controlled by script commands
        EXTERNAL: external mode, positionBase/rotationBase/positionOffset/rotationOffset can be connected to an external object
                  (e.g. when the puppeteer is picked up)
         STATION: positionBase/rotationBase controlled by OpenTracker station specified in <em>station</em>          
            DRAG: enable dragging by OpenTracker stations specified in <em>dragButton</em>
      */
    SoMFBitMask controlMode;

    /// Specifies the name of the hotspots listed in the field <em>hotspotCoord</em> in order
    SoMFString hotspotName;

    /** Specifies hotspots i.e. 3D locations relative to the center of the puppeteer scenegraph
        e.g. hotspot (0.1,0,0) would always refer to a point being 10cm from the center along the object's local +X axis.
      */
    SoMFVec3f hotspotCoord;

    /// Specifies optional orientation for relative to the orientation of the puppeteer
    SoMFRotation hotspotRot;

    /// The calculated absolute coordinates of the hotspots
    SoMFVec3f hotspotAbsCoord;

    /// The calculated absolute rotations of the hotspots
    SoMFRotation hotspotAbsRot;

    /// The calculated offset coordinates of the hotspots (without station transformations)
    SoMFVec3f hotspotOffsetCoord;

    /// The calculated absolute rotations of the hotspots (without station transformations)
    SoMFRotation hotspotOffsetRot;

    /// The 8 vertices of the puppeteer's bounding box (already contains orientation information for OBBs!)
    SoMFVec3f boundingBox;

    /// @name Fields for auxiliary visualization
    /*@{*/

    /// Sets the bounding box mode: Axis-aligned bounding box (AABB) or oriented bounding box (OBB)
    SoSFEnum boundingBoxMode;

    /// Turns the display of the bounding box on/off
    SoSFBool boundingBoxOn;

    /** Turns the bounding box on if the puppeteer receives a button press event or 
        is being dragged and <em>boundingBoxOn</em> is TRUE
      */
    SoSFBool highlightDragOn;

    /// Turns the eyesight visualization i.e. the viewing frustum(s) on/off
    SoSFBool viewingFrustumOn;

    /// Turns the velocity vector visualization on/off
    SoSFBool velocityVisOn;

    /// Sets the scale factor of the velocity visualization vector
    SoSFFloat velocityScale;

    /*@}*/

    /// Field of view and pickup related fields
    /*@{*/

    /** Defines which hotspots represent the "head" or the "eyes" of the puppeteer
        i.e. which hotspots determine the transformation of the viewport cones ("valid_hotspot_name" --> 
        appropriate hotspot pose, "" or invalid hotspot name --> puppeteer pose)
      */
    SoMFString fovHotspot;

    /** Specifies which hotspots can be used for picking up objects (specified in order of importance --> if not specified in
        a choreographer command, the 1st hotspot number is used ("valid_hotspot_name" --> appropriate hotspot pose,
        "" or invalid hotspot name --> puppeteer pose)
      */
    SoMFString pickupHotspot;

    /*@}*/

    /** @name Animation fields
        Data related to the animation of the puppeteer
     */
    /*@{*/

    /// Shows/hides the whole puppeteer scenegraph (including all the puppet and attribute scenegraphs).
    SoSFBool showPuppeteer;

    /// Shows/hides all attribute scenegraphs.
    SoSFBool showAttributes;
    
    /*@}*/

    /** @name Pose and scale fields
        Data storing the position, orientation and scale of the puppeteer
     */
    /*@{*/

    /// OpenTracker station number to use for position and orientation tracking
    SoMFInt32 station;

    /** Fields to control the base and offset of the position and orientation and the scale offset
        This fields are either internally part of a control structure specified in <em>controlMode</em> or are connected 
        and controlled by external objects (e.g. when being picked up)
      */
    SoSFVec3f positionBase;
    SoSFVec3f positionOffset;
    SoSFRotation orientationBase;
    SoSFRotation orientationOffset;
    SoSFVec3f scaleSet;

    /** Stores the current absolute position of the puppeteer i.e. the scenegraph representing all the attached puppets 
        and attributes. Setting this attribute is ignored.
      */
    SoSFVec3f position;

    /** Stores the current absolute orientation of the puppeteer i.e. the scenegraph representing all the attached puppets 
        and attributes. Setting this attribute is ignored.
      */
    SoSFRotation orientation;

    /** Stores the scale of the puppeteer i.e. the scenegraph representing all the attached puppets and attributes.
        Setting this attribute is ignored.
      */
    SoSFVec3f scale;

    /** @name Fields related to velocity
     */
    /*@{*/
    
    /// Velocity = (current_position - last_position) / elapsed_time
    SoSFFloat velocity;
    
    /// Normalized direction vector of the velocity
    SoSFVec3f velocityDir;

    /*@}*/

    /// TRUE if puppeteer is being dragged
    SoSFBool dragging;

    /** If triggered, the puppeteer's relative position and orientation to the marker will be zeroed and the puppeteer 
       "jumps" to the tracked station
      */
    SoSFTrigger snapToStation;

    /// Button to drag with
    SoSFInt32 dragButton;

    /// If being dragged, it is set to the number of the station that currently drags the puppeteer, otherwise it's -1
    SoSFInt32 draggingStation;

    /// Speech string -> the user can include a TTS object in the scene graph and connects its text input to this field
    SoSFString speechText;

    /// Indicates whether or not to play sound
    SoSFBool soundPlay;

    /// Indicates whether or not to loop sound
    SoSFBool soundLoop;

    /// Name of the sound file to play
    SoSFString soundFileName;
    
    /// Trigger to notify external speech synthesizer(s) to stop the audio output
    SoSFTrigger stopSpeech;

    /*@}*/

    /// Enables/disables automatic triggering of idle behavior
    SoSFBool disableIdleBehavior;

    /// Public getter functions for position and orientation
    bool getHotspotCoord(const char *name,SbVec3f &coord,int mode=ABSOLUTE_HS);
    bool getHotspotRot(const char *name,SbRotation &rot,int mode=ABSOLUTE_HS);
    bool getHotspotNum(const char *name,int &index);

    /// Public getter functions for puppets
    SoNode* getPuppet(int num);
    SoNode* getActivePuppet(int num);
    int getPuppetNum();
    int getActivePuppetNum();

    /// Public setter functions for puppets
    void addPuppet(SoNode *puppet);
    void replacePuppet(int num,SoNode *puppet);
    void removePuppet(int num);

    /// Constructor
    SoPuppeteerKit();

    /// Destructor
    ~SoPuppeteerKit();

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:

    /** @name Helper objects and variables
     */
    /*@{*/

    /// SoMultiSwitch node to which the puppets are attached
    SoMultiSwitch *puppetSwitch;

    /// Variable supporting the monitoring of the commands
    SbPList finishedSubCommandSensorList;

    /// Previous position value for calculating velocity and velocity direction
    SbVec3f positionOld;

    /// Previous time value for calculating velocity
    SbTime timeOld;

    /// Previous velocity samples to calculate velocity
    float velocityOld[4];

    /// Previous velocity direction samples to calculate velocity direction
    SbVec3f velocityDirOld[4];

    /// Bounding box calculation helper objects
    /*@{*/ 

    SoComputeBoundingBox *computeBoundingBox;
    SoCalculator *bbCalculator;
    SoRotateVector *bbRotator;

    /*@}*/

    /// Tracker engine
    SoTrakEngine *trackerPosition;
    SoTrakEngine *trackerOrientation;

    /// Helpers to calculate the absolute coordinates and rotations of the puppeteer hotspots + dragging conditions
    SoCalculator *hotspotPosCalculator;
    SoRotateVector *hotSpotRotatorAbs;
    SoRotateVector *hotSpotRotatorOffset;
    MultRotRot *hotSpotRotMultAbs;
    MultRotRot *hotSpotRotMultOffset;

    SoInterpolateVec3f *positionInterpolator;
    SoOneShot *positionTimer;
    SoRotateVector *positionOffsetRotator;
    SoTransformCapture *transformCapture;

    SoInterpolateRotation *orientationInterpolator;
    SoOneShot *orientationTimer;

    SoInterpolateVec3f *scaleInterpolator;
    SoOneShot *scaleTimer;

    /*@}*/

    /// @name Field sensors
    /*@{*/

    SoFieldSensor *whichPuppetsSensor;
    SoFieldSensor *whichAttributesSensor;
    SoFieldSensor *controlModeSensor;
    SoFieldSensor *showPuppeteerSensor;
    SoFieldSensor *showAttributesSensor;
    SoFieldSensor *dataValidSensor;
    SoFieldSensor *positionSensor;
    SoFieldSensor *orientationSensor;
    SoFieldSensor *scaleSensor;
    SoFieldSensor *snapToStationSensor;
    SoFieldSensor *stationSensor;
    SoFieldSensor *draggingSensor;
    SoFieldSensor *boundingBoxModeSensor;
    SoFieldSensor *boundingBoxOnSensor;
    SoFieldSensor *viewingFrustumOnSensor;
    SoFieldSensor *velocityVisOnSensor;

    /*@}*/

    /// @name Attribute change static callbacks
    /*@{*/

    static void finishedSubCommandCB(void *data,SoSensor *sensor);
    static void whichPuppetsCB(void *data,SoSensor *sensor);
    static void whichAttributesCB(void *data,SoSensor *sensor);
    static void controlModeCB(void *data,SoSensor *sensor);
    static void showPuppeteerCB(void *data,SoSensor *sensor);
    static void showAttributesCB(void *data,SoSensor *sensor);
    static void positionCB(void *data,SoSensor *sensor);
    static void orientationCB(void *data,SoSensor *sensor);
    static void scaleCB(void *data,SoSensor *sensor);
    static void snapToStationCB(void *data,SoSensor *sensor);
    static void stationCB(void *data,SoSensor *sensor);
    static void draggingCB(void *data,SoSensor *sensor);
    static void boundingBoxModeCB(void *data,SoSensor *sensor);
    static void boundingBoxOnCB(void *data,SoSensor *sensor);
    static void viewingFrustumOnCB(void *data,SoSensor *sensor);
    static void velocityVisOnCB(void *data,SoSensor *sensor);

    /*@}*/

    /// @name 3D event handler static callbacks
    /*@{*/

    static void pressCB(void* data, SoDragKit* dragKit);
    static void releaseCB(void* data, SoDragKit* dragKit);
    static void moveCB(void* data, SoDragKit* dragKit);

    /*@}*/

    /// @name Common routines
    /*@{*/
    
    /// Setup the transformation structure
    void setupTransformation();

    /// Detach sensor elements of triggerSensorList and empty the list
    void resetTriggerList();

    /*@}*/

    //------------------------------------------------------------------------------------------
    /// Virtual functions

    /// Virtual functions inherited from SoCommandParserKit
    /*@{*/

    /// Setting up field connections etc.
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Contains routines that are needed to be executed in each command loop
    virtual void commandLoopFunction();

    /// Executes the current command.
    virtual bool executeCommand();

    /// Defines idle behavior
    virtual void idle();

    /** Constructs the triggering structure for the finishedCommand notification. 
        Here it calls the overloadable virtual function getFinishedSubCommandTrigger(puppet) && getFinishedSubCommandData(puppet)
      */
    virtual void constructTrigger();

    /*@}*/

    /// @name Attribute change virtual callback functions
    /*@{*/

    virtual void whichPuppetsChanged(SoNode *node);
    virtual void whichAttributesChanged(SoNode *node);
    virtual void controlModeChanged(SoNode *node);
    virtual void showPuppeteerChanged(SoNode *puppet);
    virtual void showAttributesChanged(SoNode *node);
    virtual void positionChanged(SoNode *node);
    virtual void orientationChanged(SoNode *node);
    virtual void scaleChanged(SoNode *node);
    virtual void snapToStationChanged(SoNode *node);
    virtual void stationChanged(SoNode *node);
    virtual void draggingChanged(SoNode *node);
    virtual void boundingBoxOnModeChanged(SoNode *node);
    virtual void boundingBoxOnChanged(SoNode *node);
    virtual void viewingFrustumOnChanged(SoNode *node);
    virtual void velocityVisOnChanged(SoNode *node);

    /*@}*/

    /// @name 3D event handler static callbacks
    /*@{*/

    virtual void pressed(SoNode *node);
    virtual void released(SoNode *node);
    virtual void moved(SoNode *node);

    /*@}*/

    /// @name Virtual functions that may be overloaded when creating a new type of puppeteer    
    /*@{*/

    /// Calculate the maximum of the puppets' bounding boxes and sets default material and draw style if necessary.
    virtual void setupBoundingBox();

    /** Check the cameras representing the viewports of the SoPuppeteerKit
        If no cameras have been defined externally, define a default camera.
        Sets default material and draw style if necessary.
      */
    virtual void setupViewingFrustums();

    /** Set up velocity vector visualization scene graph structure and sets default material, 
        draw style and visualization scene graph if necessary.
      */
    virtual void setupVelocityVisualization();

    /** Functions called by <em>constructTrigger()</em> to construct the triggering structure for the 
        finishedCommand notification.
      */
    /*@{*/

    virtual SoField* getFinishedSubCommandTrigger(SoNode *puppet);
    virtual const char* getFinishedSubCommandData(SoNode *puppet);

    /*@}*/

    /** Puppet-specific command implementation
        Returns 'false' if the default implementation doesn't need to be called after the puppet-specific implementation
        Returns 'true' if the default implementation NEEDS to be called after the puppet-specific implementation
      */
    virtual bool implementCommand(SoNode *puppet);

    /// Executes puppet-specific commands when idle
    virtual void idleCommand(SoNode *puppet);

    /// Default implementation of animation commands
    virtual bool defaultImplementation(SoNode *puppet);

    /*@}*/

    /// @name Common commands
    /*@{*/

    virtual void setPosition(float x,float y,float z,int duration);
    virtual void setOrientationQ(float qx,float qy,float qz,float w,int duration);
    virtual void setOrientationV(float vx,float vy,float vz,float a,int duration);
    virtual void setScale(float x,float y,float z,int duration);
    virtual void turnTowards(float x,float y,float z,int duration);
    virtual void lookAt(float x,float y,float z,int duration);    

    /*@}*/

    /*@}*/
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_SOPUPPETEERKIT_H_
