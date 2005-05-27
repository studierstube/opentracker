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
/** SoTrackedArtifactKit.h contains the definition of class SoTrackedArtifactKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoTrackedArtifactKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOTRACKEDARTIFACTKIT_H_
#define _SOTRACKEDARTIFACTKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#include <stbapi/interaction/So3DSeparator.h> 

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------

/** SoTrackedArtifactKit is a nodekit representing a tracked object.

This class can be used as base-class for any tracked object. 
The object will be automatically moved by (connected to) a SoTrakEngine with
the station specified in the kits field.
The class offers some simple access-methods for setting/getting the geometry of
the object and getting an offset. The geometry of the object can be
turned on and off by a calling the showGeometry() method. 

It also includes an So3DSeparator node to be able to limit the 3DEvents passed to
the children. To control this behaviour it exposes the Base3D interface using the
eventMask, stations and blocked fields. These will set the corresponding fields on
the internal So3DSeparator node.

In a distributed environment, the SoTrackedArtifactKit will only influence the local
copy of scene graph. the changes to the underlying SoTransform that are driven by
a SoTrakEngine are not distributed, because tracking events are only handled locally.
Slave clients should be supplied with the right events directly. All other fields and
properties of the node are shared as usuall.

@author Rainer Splechtna
@ingroup tracking
 */

class SoTrakEngine;

class STBAPI_API SoTrackedArtifactKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoTrackedArtifactKit);
    SO_KIT_CATALOG_ENTRY_HEADER(artifactRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(mover);
    SO_KIT_CATALOG_ENTRY_HEADER(offset);
    SO_KIT_CATALOG_ENTRY_HEADER(onOffSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);

public:

    /** This field specifys the number of the tracker station, which "moves"
        the object.
      */
    SoSFInt32 station; 

    /** The constructor of the kit inits the fields and connects the tracker
        to the object.
      */
    SoTrackedArtifactKit(               
        );
    /** The destructor of the kit does nothing. */
    ~SoTrackedArtifactKit(              
        );
    /** Inventor class initialization */
    static void initClass(             
        );

    /** Returns the offset of this object.

        @return the SoTransform-node representing the offset of the object
      */
    SoTransform* getOffset();
    /** Returns the root of the geometry-graph of this object.

        @return the SoNode representing the object's geometry root
      */
    SoNode*      getGeometry();
    /** Set the geometry-graph of this object.

        @param newGeometry the object's new geometry 
      */
    void         setGeometry(SoNode *newGeometry);
    /** Use this method to switch the object's geometry on or off.

        @param onOff TRUE: the geometry is switched on, 
                     FALSE: the geometry is switched off
      */
    void         showGeometry(SbBool onOff);
    /** Returns the state of the geometry switch

        @return TRUE: if the geometry is switched on
                FALSE: if the geometry is switched off
    */
    SbBool       geometryIsVisible();
    /** Set an event mask for this nodeKit.

        Per default the nodekit won't be traversed by the
        SoHandle3DEventAction. If the geometry should be aware of certain
        3Devents set the mask accordingly.

        @param eventMask determines, in which 3Devents the nodekit is interested
      */
    void setEventMask(So3DSeparator::EventTypeMask eventMask);
    /** Returns the SoTransform node which moves the object (i.e. which is
        updated by a SoTrakEngine) 

        @return the SoTransform-node, which moves the object
      */
    SoTransform* getMover();

    /** defines a timeout to set the geometry invisible, if it didn't move. 
      * 0 is no timeout. */
	SoSFTime timeOut;

    /** denotes if the geometry is currently visible */
    SoSFBool isVisible;

protected:
    /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// sensor to check on movements to reset timer for timeout
	SoFieldSensor moveSensor;
    /// sensor to trigger a time out
	SoAlarmSensor timer;
    /// sensor to check on isVisible
    SoFieldSensor visibleSensor;

    /** sensor callback method for movement sensor. it rescedules
     * the timeout sensor and sets the geometry to visible if 
     * necessary.
     */
	static void moved( void *data, SoSensor *sensor );

	/** callback from within moved, that can be used to do additional processing if the
	* TrackedAritfact has been moved.
	*/
	virtual void movedCB();

	/** sensor callback method for timeout. it set the
     * geometry to invisible.
     */
	static void timerFired( void *data, SoSensor *sensor );

	SoTrakEngine *tracker;
    static void visibleChanged( void * data, SoSensor * sensor );

};

//----------------------------------------------------------------------------
#endif//_SOPENKIT_H_

