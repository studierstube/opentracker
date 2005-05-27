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
/** SoTrackedArtifactKit.h contains the implementation of class SoTrackedArtifactKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoTrackedArtifactKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <Inventor/nodes/SoTransform.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/workspace/StbWorkspace.h>

#include "SoTrackedArtifactKit.h"

SO_KIT_SOURCE(SoTrackedArtifactKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoTrackedArtifactKit::initClass()
{
    SO_KIT_INIT_CLASS(SoTrackedArtifactKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
/** The constructor of the kit inits the fields and connects the tracker. */

SoTrackedArtifactKit::SoTrackedArtifactKit()
{
    SO_KIT_CONSTRUCTOR(SoTrackedArtifactKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        artifactRoot,    So3DSeparator,FALSE,this,     \0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        mover,    SoTransform,         FALSE,artifactRoot,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        offset,    SoTransform,         TRUE,artifactRoot,    \0,TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(
        onOffSwitch,    SoSwitch,         FALSE,artifactRoot,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(
        geometry,    SoNode, SoSeparator,FALSE, onOffSwitch, \0 ,TRUE );

    // add field
    SO_KIT_ADD_FIELD(station, (-1)); 
	SO_KIT_ADD_FIELD(timeOut, (0.0));
    SO_KIT_ADD_FIELD(isVisible, (TRUE));

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);
    
	moveSensor.setFunction( moved );
    moveSensor.setData( this );
    moveSensor.setPriority( 1 );   

    timer.setFunction( timerFired );
    timer.setData( this );
	
	tracker = NULL;

    visibleSensor.setFunction( visibleChanged );
    visibleSensor.setData( this );
    visibleSensor.setPriority( 1 );
    visibleSensor.attach( &isVisible );

    setUpConnections( TRUE, TRUE );

    showGeometry(TRUE);
    setUpConnections( TRUE, TRUE );

    setEventMask( So3DSeparator::BM_NO_EVENT );
}


//----------------------------------------------------------------------------
/** The destructor of the kit does nothing. */

SoTrackedArtifactKit::~SoTrackedArtifactKit()
{
	// tracker is an Engine an will be deleted if all Outputs are disconnected !!
//	if (tracker!= NULL) delete tracker;
    // nil
}

SbBool SoTrackedArtifactKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);

        // do your own special setup here !
        
		tracker = new SoTrakEngine;
		tracker->station.connectFrom(&station);
		SoTransform *mover = getMover();   
		mover->translation.connectFrom(&tracker->translation);
		mover->rotation.connectFrom(&tracker->rotation);

		// if a SoTrackedArtifactKit is shared, it should probably not
        // move it's slave copies around, as they are connected to tracking
        // input as well
        CDivMain::globalDisableNode( getMover());
		CDivMain::globalDisableNode( SO_GET_ANY_PART(this,"onOffSwitch",SoSwitch));
		moveSensor.attach( &getMover()->translation );        
        
        // if a timeout is set, the timer sensor should be scheduled here already.
        // otherwise the geometry will not become invisible, if no movement happens.
        /*
        if( timeOut.getValue().getValue() > 0.0 )
        {
            if( timer.isScheduled())
                timer.unschedule();
            timer.setTimeFromNow( timeOut.getValue());
            timer.schedule();
        }*/
    }
    else {
        // We disconnect BEFORE base class.
        // Disconnect the field sensors.
		CDivMain::globalEnableNode( SO_GET_ANY_PART(this,"onOffSwitch",SoSwitch));
        CDivMain::globalEnableNode( getMover());
		moveSensor.detach();

		SoTransform *mover = getMover();   
		mover->translation.disconnect();
		mover->rotation.disconnect();

		//tracker->station.disconnect();

        SoBaseKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
}

//----------------------------------------------------------------------------
/** Returns the SoTransform node which moves the object. */

SoTransform*
SoTrackedArtifactKit::getMover()
{
    return SO_GET_ANY_PART(this,"mover",SoTransform);
}

//----------------------------------------------------------------------------
/** Returns the offset of this object. */

SoTransform*
SoTrackedArtifactKit::getOffset()
{
    return SO_GET_PART(this,"offset",SoTransform);
}

//----------------------------------------------------------------------------
/** Returns the root of the geometry-graph of this object. */

SoNode*
SoTrackedArtifactKit::getGeometry()
{
    return SO_GET_PART(this,"geometry",SoNode);
}

//----------------------------------------------------------------------------
/** Set the geometry-graph of this object. */

void 
SoTrackedArtifactKit::setGeometry(SoNode *newGeometry)
{
    setPart("geometry", newGeometry);
}

//----------------------------------------------------------------------------
/** Use this method to switch the object's geometry on or off. */

void
SoTrackedArtifactKit::showGeometry(SbBool onOff)
{
    SO_GET_ANY_PART(this,"onOffSwitch",SoSwitch)->whichChild = onOff ? -3 : -1;
    visibleSensor.detach();
    isVisible.setValue( onOff );
    visibleSensor.attach( &isVisible );
}

//----------------------------------------------------------------------------
/** Set an event mask for this nodeKit. */

void
SoTrackedArtifactKit::setEventMask(So3DSeparator::EventTypeMask eventMask)
{
    SO_GET_ANY_PART(this,"artifactRoot",So3DSeparator)->eventMask = eventMask;
}

//----------------------------------------------------------------------------
/** Returns the state of the geometry switch. */

SbBool
SoTrackedArtifactKit::geometryIsVisible()
{
    int switchStatus = SO_GET_ANY_PART(this,"onOffSwitch",SoSwitch)->whichChild.getValue();
    if (switchStatus == -3)
        return TRUE;
    else
        return FALSE;
}

//----------------------------------------------------------------------------

void SoTrackedArtifactKit::movedCB() 
{
	// to be used in subclasses
}

//----------------------------------------------------------------------------

void SoTrackedArtifactKit::moved( void *data, SoSensor *sensor )
{
	// reset timeOut timer
	SoTrackedArtifactKit * self = (SoTrackedArtifactKit *)data;
    if( self->timeOut.getValue().getValue() > 0.0 )
    {
        if( self->timer.isScheduled())
            self->timer.unschedule();
        self->timer.setTimeFromNow( self->timeOut.getValue());
        self->timer.schedule();
    }
	if( FALSE == self->geometryIsVisible())
	{
		DISABLEDIVNODE(self);
		self->showGeometry( TRUE );
		ENABLEDIVNODE(self);
	}
	self->movedCB();
}
    
void SoTrackedArtifactKit::timerFired( void *data, SoSensor *sensor )
{
	SoTrackedArtifactKit * self = (SoTrackedArtifactKit *)data;
	DISABLEDIVNODE(self);    
    self->showGeometry( FALSE );
	ENABLEDIVNODE(self);
}

void SoTrackedArtifactKit::visibleChanged( void * data, SoSensor * sensor )
{
    SoTrackedArtifactKit * self = (SoTrackedArtifactKit *)data;
	DISABLEDIVNODE(self);
    self->showGeometry(self->isVisible.getValue());
    ENABLEDIVNODE(self);
}


//eof

