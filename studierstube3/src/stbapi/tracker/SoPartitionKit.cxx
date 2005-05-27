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
/** SoPartitionKit implementation file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoPartitionKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPartitionKit.h"

#include <cassert>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/sensors/SoOneShotSensor.h>

// OpenInventor macros

SO_KIT_SOURCE(SoPartitionKit);

// class initialisation for OpenInventor

void SoPartitionKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoPartitionKit, SoBaseKit, "BaseKit");   
}

// constructor

SoPartitionKit::SoPartitionKit()
{
    SO_KIT_CONSTRUCTOR(SoPartitionKit);
    //-----------------------name-----   |type         |nuldef |parent|right sibling |public
    SO_KIT_ADD_CATALOG_ENTRY(stationKits, SoMultiSwitch,  FALSE, this,     \x0, TRUE);

    SO_KIT_ADD_FIELD( activeChild, (-1));
    SO_KIT_ADD_FIELD( numPortals, (-1));
    SO_KIT_ADD_FIELD( portals, (-1));

    SO_KIT_INIT_INSTANCE();

    /*
    SoMultiSwitch * multiSwitch = (SoMultiSwitch *) stationKits.getValue();
    assert( multiSwitch );

    // set a node sensor on the multiswitch to get at new StationKit nodes
    childSensor.setFunction( childCB );
    childSensor.setData( this );
    childSensor.attach( multiSwitch );
    childSensor.setPriority( 0 );
    */

    // set a one shot sensor to initialize the multiswitch's children once they are loaded
    SoOneShotSensor * one = new SoOneShotSensor( initChildren, this );
    one->schedule();	

    /*    // set the field sensor on activeChild
    activeSensor.setFunction( activeChanged );
    activeSensor.setData( this );
    activeSensor.attach( &activeChild );
    activeSensor.setPriority( 0 );
    */
}

// destructor, clears all lists and removes any listeners etc.

SoPartitionKit::~SoPartitionKit()
{
    childSensor.unschedule();
    childSensor.detach();
    kitList.truncate(0);
}

// callback triggered after the construction to set up callbacks in the children

void SoPartitionKit::initChildren( void * data, SoSensor * sensor )
{   
    SoPartitionKit * self = (SoPartitionKit *) data;
    assert( self );

    // set the field sensor on activeChild
    self->activeSensor.setFunction( activeChanged );
    self->activeSensor.setData( self );
    self->activeSensor.attach( &self->activeChild );
    self->activeSensor.setPriority( 0 );

    SoMultiSwitch * multiSwitch = (SoMultiSwitch *) self->stationKits.getValue();
    assert( multiSwitch );

    // for each child, find the underlying SoStationKit, register the eventCB
    // and store in stationKits list
    SoSearchAction mySearch;
    mySearch.setType( SoStationKit::getClassTypeId());
    mySearch.setInterest( SoSearchAction::FIRST );
    for( int i = 0; i < multiSwitch->getNumChildren(); i++ )
    {
        SoNode * node = multiSwitch->getChild( i );
        mySearch.apply( node );
        SoPath * path = mySearch.getPath();
        if( path != NULL )
        {
            path->ref();        
            assert((path->getTail()->isOfType( SoStationKit::getClassTypeId())));
            SoStationKit * stationKit = (SoStationKit *) path->getTail();
            stationKit->addEventCallback( eventCB, self );
            stationKit->visible.setValue( FALSE );
            self->kitList.append( stationKit );
            path->unref();
        }
        else {
            multiSwitch->removeChild( i-- );
        }
    }      
    self->setActiveChild( self->activeChild.getValue());
}

// callback triggered by SoStationKit nodes to change the active node, if necessary

void SoPartitionKit::eventCB( void *data, int station, SoStationKit *kit, 
                         SoHandle3DEventAction *h3a)
{
    SoPartitionKit * self = (SoPartitionKit *) data;
    int index = self->kitList.find( kit );
    assert( index != -1 );
    if( index == self->activeChild.getValue())
        return;
    self->activeChild.setValue( index );
}

void SoPartitionKit::childCB( void *data, SoSensor * sensor )
{
    SoPartitionKit * self = (SoPartitionKit *) data;
    SoMultiSwitch * multiSwitch = (SoMultiSwitch *) self->stationKits.getValue();
    assert( multiSwitch );  
}

// changes the currently active node

void SoPartitionKit::setActiveChild( int index )
{
    if( index >= numPortals.getNum() || index < 0 )
        return;
    
    // compute the indices into the portal field etc.
    int neighbors = 0;
    int num = numPortals[index];

    int i;
    for( i = 0; i < index; i++ )
        neighbors += numPortals[i];

    // set the indices in the multiSwitch field and set the visibility stuff correctly
    SoMultiSwitch * multiSwitch = (SoMultiSwitch *) stationKits.getValue();
    multiSwitch->whichChildren.set1Value( 0, index );
    
    SoStationKit * station = (SoStationKit *) kitList[index];
    station->visible.setValue( TRUE );

    for( i = 0; i < num; i++ )
    {
        multiSwitch->whichChildren.set1Value( i+1, portals[neighbors+i]);
        station = (SoStationKit *) kitList[portals[neighbors+i]];
        station->visible.setValue( FALSE );
    }
    multiSwitch->whichChildren.setNum( num + 1 );    
}

// check if someone changed the activeChild field

void SoPartitionKit::activeChanged( void * data, SoSensor * sensor )
{
    SoPartitionKit * self = (SoPartitionKit *) data;
    assert( self );
    self->setActiveChild( self->activeChild.getValue()); 
}
