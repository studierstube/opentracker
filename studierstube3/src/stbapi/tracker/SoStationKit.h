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
/** SoStationKit header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoStationKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOSTATIONKIT_H__
#define __SOSTATIONKIT_H__

#include <stbapi/event3d/Base3D.h>

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/SbPList.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class STBAPI_API SoStationKit;

/**
 * Type for event callback functions, that are called whenever a 3D event was
 * handled by the StationKit node.
 * @param userdata pointer to the user data registered with the callback
 * @param stationId station number the event was generated from
 * @param stationKit the StationKit calling the function
 * @param action the action traversing the scene graph
 */
typedef void StationEventCB( void *userData, int stationId, 
                             SoStationKit *stationKit, SoHandle3DEventAction *action);

/**
 * Type for timout callback functions, that are called whenever the  visibility
 * timeout of the StationKit node occured.
 * @param userdata pointer to the user data registered with the callback
 * @param stationKit the StationKit calling the function
 */
typedef void StationTimeoutCB( void *userData, SoStationKit *stationKit);

/**
 * does some miraculous things with tracking data. 
 *
 * The SoStationKit can be configured to accept move events from any number of 
 * tracker station (for example, ARToolKit markers). It has the following 
 * important features:
 * @li All behavior (except user defined callbacks) can be scripted in .iv 
 *   files without C++ coding.
 * @li Can process information from a set of stations.
 * @li In its simplest form, it processed information from a single station,
 *   which is used to move around a virtual object. The virtual object is an
 *   arbitrary scene graph that is specified by the user. This scene graph is
 *   optional, its automatic movement from the tracker information can be 
 *   turned off and it can also be set invisible at any time.
 * @li If a physical object is fitted with multiple stations (e. g., to avoid
 *   occlusion problems), the stations‘geometric relationships to each other 
 *   and the physical object can be conveniently expressed. The node computes 
 *   a final transformation of the physical object, which can again be used 
 *   to move a virtual object.
 * @li Using of multiple stations is particularly convenient for determining 
 *   the relative position of "large" objects such as a library or even 
 *   building. In these cases not the objects, but the user moves. However, 
 *   the problem remains the same, as a virtual model (e.g. for a library) 
 *   that augmented the physical environment must only be moved relative to the user. 
 *   We do <b>not</b> use the user (i. e., the virtual camera), but rather the 
 *   "world" (scene) that surrounds the user.
 * @li Station recording: The last event received from a particular 
 *   (where and when) is automatically recorded. Useful if one needs to 
 *   determine relative
 * @li Timeout: If no information is received from any configured station for 
 *   a specified time, a virtual object is made invisible and a user callback 
 *   is called.
 * @li User supplied callbacks can be executed every time a 3D event is 
 *   received. This opens up a wide range of interaction possibilities 
 *   described below.
 *
 * So what can be done with this node, in particular using the callbacks?
 * @li Slave movement of a virtual object, with single station 
 *  (mentioned above): Just configure one station and supply a scene graph
 * @li Slave movement with multiple stations: Just configure multiple 
 *  stations and supply a scene graph
 * @li User moves in a room or building: Same case as above, only the object
 *  is very big. (Note that we are also currently designing an extended 
 *  version of the node that can handle very large models partitioned into 
 *  rooms, where stations can be recycled in every room.)
 * @li Detection of movement of multiple stations, for example proximity 
 *  detector: For example, you can detect if station A is next to station B 
 *  by setting a callback where you look for events of station A. If any occurs, 
 *  check whether the recorded position of B is sufficiently close, and whether 
 *  the recorded timestamp of B is the same as for A, then A is next to B.
 * @li Constrained movement: Sometime you do not want direct slave movement 
 *  of a virtual object according to some station. For example, you may want 
 *  to move object_A with station_A, but only up to x <= 2m. When station_A 
 *  is moved to the right of the 2m boundary, object_A should remain where it was. 
 *  Then you disable the "transformUpdate" feature (i. e., slave movement) and 
 *  set a callback that manually computes the virtual objects position from 
 *  the station‘s event according to your algorithm.
 * 
 *  File format :
@verbatim SoStationKit {
    stations [-1]
  
    visible TRUE
    timeOut 0
    transformUpdate TRUE
}
@endverbatim
 *
 *  The kit has the following structure :
 *@verbatim base 
      |
    visibleSwitch
      |
      +----------------+ 
      |                |
    dummyGroup    contentSep
                       |
                       +-------------+
                       |             |
                   transform      content
@endverbatim
 * @ingroup tracking
 * @ingroup core
 */
class STBAPI_API SoStationKit : public SoBaseKit, public Base3D 
{
    SO_KIT_HEADER(SoStationKit);

     ///(hidden) root separator
    SO_KIT_CATALOG_ENTRY_HEADER(base);
    /// visibility switch
    SO_KIT_CATALOG_ENTRY_HEADER(visibleSwitch);
    /// empty group as invisible child of switch
    SO_KIT_CATALOG_ENTRY_HEADER(dummyGroup);
    /// separator for transformation and content
    SO_KIT_CATALOG_ENTRY_HEADER(contentSep);
    /// transformation to move the content around
    SO_KIT_CATALOG_ENTRY_HEADER(transform);      
    /// the actual user's content
    SO_KIT_CATALOG_ENTRY_HEADER(content);  

    BASE3D_HEADER;

public: // member-variables
    
    /** stores the position offsets of the individual stations. This field is
     * related to the field Base3D::stations inherited from Base3D. The i-th entry in
     * this field corresponds to the i-th station defined in Base3D::stations. The
     * blocked field from Base3D is set to FALSE per default to process only
     * the stations specified in Base3D::stations.*/
    SoMFVec3f stationPosition;

    /** stores the orientation offsets of the individual stations. See
     * stationPosition for details on the relationship to Base3D::stations.
     */
    SoMFRotation stationOrientation;
    
    /** stores the last seen position of the individual stations. See
     * stationPosition for details on the relationship to Base3D::stations.
     */
    SoMFVec3f currentPosition;
    /** stores the last seen orientations of the individual stations. See
     * stationPosition for details on the relationship to Base3D::stations.
     */
    SoMFRotation currentOrientation;

    /** stores the timestamps for the last seen event of each station. See
     * stationPosition for details on the relationship to Base3D::stations.
     */
    SoMFTime lastEvent;

    /** the current position of the SoStationKit. This position is computed to
     * place the SoStationKit on the offset position of the station
     * within the world coordinate system. It therefore incorporates all
     * transformations above the SoStationKit. */
    SoSFVec3f position;
    /** the current orientation of the SoStationKit. This orientation is computed to
     * place the SoStationKit in the offset orientation of the station
     * within the world coordinate system. It therefore incorporates all
     * transformations above the SoStationKit. */
    SoSFRotation orientation;

    /**
     * timeout value for setting the content invisible in seconds. If the
     * timeOut is 0 then no timeout occurs. */
    SoSFTime timeOut;

    /// whether the content of the SoStationKit is visible
    SoSFBool visible;
    /** controls whether the @ref position and @ref orientation of the content
     * is updated from the fields position and orientation or not. */
    SoSFBool transformUpdate;

public:// member-methods
                
    /// static method to initialize class for OpenInventor
    static void initClass();    

    /// constructor
    SoStationKit();
    /// destructor          
    virtual ~SoStationKit();

    /** checks whether the node is interested in a certain 3DEventAction.
     * The SoStationKit is always interested and therefore returns always
     * TRUE.
     * @param h3a pointer to the 3DEventAction.
     * @return always TRUE*/
    virtual SbBool isInterested(SoHandle3DEventAction *h3a);

    /** 
     * handles the 3D event. This method computes the new position and
     * orientation from the event's data and the station's offset as
     * defined in stationPosition and stationOrientation. It also updates
     * currentPosition, currentOrientation and lastEvent, 
     * reschedules the time out and calls the event callbacks.
     * @param h3a pointer to the 3DEventAction.
     */
    virtual void handle3DEvent(SoHandle3DEventAction *h3a);
                
    /**
     * adds an event callback to the SoStationKit.
     * @param cb pointer to the callback function
     * @param userData void pointer that is passed to the user function */
    void addEventCallback(StationEventCB *cb, void *userData=NULL);

    /**
     * adds a time out callback to the SoStationKit.
     * @param cb pointer to the callback function
     * @param userData void pointer that is passed to the user function */
    void addTimeoutCallback(StationTimeoutCB *cb, void *userData=NULL);

    /** 
     * removes an event callback from the SoStationKit.
     * @param cb pointer to the callback function
     * @return a flag to indicate, whether the function 
     * was a callback at all. */
    SbBool removeEventCallback(StationEventCB *cb);
    /** 
     * removes a time out callback from the SoStationKit.
     * @param cb pointer to the callback function
     * @return a flag to indicate, whether the function 
     * was a callback at all. */
    SbBool removeTimeoutCallback(StationTimeoutCB *cb);    

protected:
    /// lists to store the event callback functions and userdata
    SbPList eventCBs,
            timeoutCBs;

    /** a FieldSensor to observe the updateTransform field. It 
     * triggers the updateChanged callback with a pointer to the
     * SoStationKit that's attribute changed */
    SoFieldSensor updateSensor,

    /** a FieldSensor to observe the timeOut field. It 
     * triggers the timeOutChanged callback with a pointer to the
     * SoStationKit that's attribute changed */
                  timeOutSensor;
    
    /**
     * AlarmSensor to signal the time out. It triggers the
     * timerFired callback with a pointer to the SoStationKit. */
    SoAlarmSensor timer;

protected:

    /**
     * sets or deletes routes between the position and
     * orientation field and the Transform's fields depending
     * on the value of updateTransform.
     * @param data pointer to the SoStationKit involved
     * @param sensor the sensor that called the callback function */
    static void updateChanged( void *data, SoSensor *sensor );
    
    /**
     * reschedules or unschedules the SoStationKit's timer depending
     * on the value of the timeOut field.
     * @param data pointer to the SoStationKit involved
     * @param sensor the sensor that called the callback function */
    static void timeOutChanged( void *data, SoSensor *sensor );

    /**
     * called when the time out timer fires and sets the geometry
     * invisible.
     * @param data pointer to the SoStationKit involved
     * @param sensor the sensor that called the callback function */
    static void timerFired( void *data, SoSensor *sensor );

    /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

