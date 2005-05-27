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
/** SoStationKit implementation file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoStationKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoStationKit.h"

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/workspace/StbWorkspace.h>

// OpenInventor macros

SO_KIT_SOURCE(SoStationKit);
BASE3D_SOURCE(SoStationKit);

// class initialisation for OpenInventor

void SoStationKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoStationKit, SoBaseKit, "BaseKit");
    BASE3D_INIT_CLASS(SoStationKit);    
}

// constructor

SoStationKit::SoStationKit()
{
    SO_KIT_CONSTRUCTOR(SoStationKit);
    //-----------------------name-----|-type-------|nuldef|parent|right sibling|public
    SO_KIT_ADD_CATALOG_ENTRY(base,          SoSeparator, FALSE, this,          \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(visibleSwitch, SoSwitch,    FALSE, base,          \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(dummyGroup,    SoGroup,     FALSE, visibleSwitch, \x0, FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(contentSep,    SoSeparator, FALSE, visibleSwitch, \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(transform,     SoTransform, FALSE, contentSep,    \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(content,       SoGroup,     FALSE, contentSep,    \x0, TRUE );
        
    SO_KIT_ADD_FIELD( stationPosition, (0,0,0));
    SO_KIT_ADD_FIELD( stationOrientation, (0,0,0,1));
    SO_KIT_ADD_FIELD( currentPosition, (0,0,0));
    SO_KIT_ADD_FIELD( currentOrientation, (0,0,0,1));
    SO_KIT_ADD_FIELD( lastEvent, (0.0));
    SO_KIT_ADD_FIELD( position, (0,0,0));
    SO_KIT_ADD_FIELD( orientation, (0,0,0,1));
    SO_KIT_ADD_FIELD( timeOut, (0.0));
    SO_KIT_ADD_FIELD( visible, (TRUE));
    SO_KIT_ADD_FIELD( transformUpdate, (TRUE));  

    SO_KIT_INIT_INSTANCE();
    
    BASE3D_CONSTRUCTOR;

    blocked.setValue(FALSE);

    SoSwitch * sw = (SoSwitch*)visibleSwitch.getValue();        
    sw->whichChild.setValue( 0 );    
    
    updateSensor.setFunction( updateChanged );
    updateSensor.setData( this );
    updateSensor.setPriority( 0 );   

    timeOutSensor.setFunction( timeOutChanged );
    timeOutSensor.setData( this );
    timeOutSensor.setPriority( 0 );   

    timer.setFunction( timerFired );
    timer.setData( this );
    setUpConnections( TRUE, TRUE );

}

SbBool SoStationKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);

        // do your own special setup here !
        SoTransform * xf = (SoTransform*)transform.getValue();
        xf->translation.connectFrom( &position );
        xf->rotation.connectFrom( &orientation );
        SoSwitch * sw = (SoSwitch*)visibleSwitch.getValue();
        sw->whichChild.connectFrom( &visible );
        updateSensor.attach( &transformUpdate );
        timeOutSensor.attach( &timeOut );
		// reset timeOut timer
		if( timeOut.getValue().getValue() > 0.0 )
		{
            if( timer.isScheduled())
			    timer.unschedule();
			timer.setTimeFromNow( timeOut.getValue());
			timer.schedule();
		}
    }
    else {
        // We disconnect BEFORE base class.
        // Disconnect the field sensors.
        SoTransform * xf = (SoTransform*)transform.getValue();
        xf->translation.disconnect();
        xf->rotation.disconnect();
        SoSwitch * sw = (SoSwitch*)visibleSwitch.getValue();
        sw->whichChild.disconnect();
        updateSensor.detach();
        timeOutSensor.detach();
        if( timer.isScheduled())
		    timer.unschedule();

        SoBaseKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
}

// destructor, clears all lists and removes any listeners etc.

SoStationKit::~SoStationKit()
{
    updateSensor.detach();
    timeOutSensor.detach();
    eventCBs.truncate(0);
    timeoutCBs.truncate(0);
    if( timer.isScheduled())
        timer.unschedule();
}

// we are interested in all 3D events by our stations

SbBool SoStationKit::isInterested(SoHandle3DEventAction *h3a)
{    
    return TRUE;
}

// compute the new pose based on the tracked station

void SoStationKit::handle3DEvent(SoHandle3DEventAction *h3a)
{
	DISABLEDIVNODE(this);

    So3DEvent *ev = (So3DEvent*)h3a->getEvent();

    int station = ev->getStation();
    int index = stations.find( station );
    if( index == -1 )
        return;
    if( stationPosition.getNum() <= index )
        stationPosition.set1Value( index, SbVec3f( 0, 0, 0 ));
    if( stationOrientation.getNum() <= index )
        stationOrientation.set1Value( index, SbRotation( SbVec3f(0, 0, 1), 0 ));

    // get the transformation from the root 
    SbViewportRegion vp;
    SoGetMatrixAction ma(vp);
    h3a->getPathFromRoot()->ref();
    ma.apply((SoFullPath*)h3a->getPathFromRoot());
	h3a->getPathFromRoot()->unrefNoDelete();
    SbMatrix globalToLocal = ma.getInverse();

    // transform the rotation part of the 3D event
    SbMatrix rotMatrix;
    ev->getRotation().getValue(rotMatrix);
    SbMatrix resMatrix;
    resMatrix = rotMatrix * globalToLocal;
    SbVec3f t,s; SbRotation rot, so;
    resMatrix.getTransform( t, rot, s, so );

    // transfrom the position part of the 3D event
    SbVec3f pos;
    globalToLocal.multVecMatrix( ev->getTranslation(), pos);    

    // store values
    currentPosition.set1Value( index, pos );
    currentOrientation.set1Value( index, rot );
    lastEvent.set1Value( index, ev->getTime());
   
    // compute the new pose based on the transformed event data
	SbVec3f posOffset;
	SbRotation rotOffset = stationOrientation[ index ].inverse() * rot;
	rotOffset.multVec( stationPosition[ index ], posOffset );
    position.setValue( pos - posOffset );
    orientation.setValue( rotOffset );

    // do some visibility stuff
    if( visible.getValue() == TRUE )
        visible.setValue( TRUE );

    // reset timeOut timer
    if( timeOut.getValue().getValue() > 0.0 )
    {
        if( timer.isScheduled())
            timer.unschedule();
        timer.setTimeFromNow( timeOut.getValue());
        timer.schedule();
    }

	ENABLEDIVNODE(this);

	if( getBase3DMode() != Base3D::BASE3D_SLAVEMODE )
	{
		// call event callbacks
		for( int i = 0; i < eventCBs.getLength(); i+=2 )
			(*((StationEventCB *)eventCBs[i]))(eventCBs[i+1], station, this, h3a );
	}
}
				
// adds a callback function to the event list

void SoStationKit::addEventCallback(StationEventCB *cb, void *userData )
{
   	eventCBs.append((void*)cb);
	eventCBs.append(userData);
}

// adds a callback function to the timeout list

void SoStationKit::addTimeoutCallback(StationTimeoutCB *cb, void *userData )
{
    timeoutCBs.append((void*)cb);
	timeoutCBs.append(userData);
}

// removes a callback function from the timeout list

SbBool SoStationKit::removeEventCallback(StationEventCB *cb)
{
    int i;	
    // try to find the pointer in the list
	i=eventCBs.find((void*)cb);
	// if not found return FALSE
	if (i==-1) 
		return FALSE;
	// remove pointer and user data and return TRUE
	eventCBs.remove(i);
	eventCBs.remove(i);
	return TRUE;
}

// removes a callback function from the timeout list

SbBool SoStationKit::removeTimeoutCallback(StationTimeoutCB *cb)
{
    int i;	
    // try to find the pointer in the list
	i=eventCBs.find((void*)cb);
	// if not found return FALSE
	if (i==-1) 
		return FALSE;
	// remove pointer and user data and return TRUE
	timeoutCBs.remove(i);
	timeoutCBs.remove(i);
	return TRUE;
}

// setup or clear internal routes depending on updateTransfrom flag

void SoStationKit::updateChanged(void *data, SoSensor *sensor)
{
    SoStationKit * self = (SoStationKit *)data;
    if( self->transformUpdate.getValue() == TRUE )
    {
        SoTransform * xf = (SoTransform*)self->transform.getValue();
        xf->translation.connectFrom( &(self->position));
        xf->rotation.connectFrom( &(self->orientation));
    } else 
    {
        SoTransform * xf = (SoTransform*)self->transform.getValue();
        xf->translation.disconnect();
        xf->rotation.disconnect();
    }
}

// reset timeOut sensor when timeout value changes

void SoStationKit::timeOutChanged(void *data, SoSensor *sensor)
{
	SoStationKit * self = (SoStationKit *)data;
	DISABLEDIVNODE(self);
    if( self->timer.isScheduled())
        self->timer.unschedule();    
    if( self->timeOut.getValue().getValue() > 0.0 )
    {
        self->timer.setTimeFromNow( self->timeOut.getValue());
        self->timer.schedule();
    }
	ENABLEDIVNODE(self);
}

// make children invisible, when timeout timer fired

void SoStationKit::timerFired(void *data, SoSensor *sensor)
{
	SoStationKit * self = (SoStationKit *)data;
	DISABLEDIVNODE(self);    
    // self->visible.setValue( FALSE );
    SoSwitch * sw = (SoSwitch*) self->visibleSwitch.getValue();
    sw->whichChild.setValue( 0 );
	ENABLEDIVNODE(self);

	if( self->getBase3DMode() != Base3D::BASE3D_SLAVEMODE )
	{
		for( int i = 0; i < self->timeoutCBs.getLength(); i+=2 )
			(*((StationTimeoutCB *)self->timeoutCBs[i]))(self->timeoutCBs[i+1], self );
	}
}

