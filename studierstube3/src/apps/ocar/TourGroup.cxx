 /* ========================================================================
* Copyright (C) 2000,2001,2002  Vienna University of Technology
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
* Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
* Austria.
* ========================================================================
* PROJECT: ocar
* ======================================================================== */
/**
* implementation of distributed functionality in ocar
*
* @author Gerhard Reitmayr
*
* $Id: TourGroup.cxx 3801 2004-09-30 14:02:02Z reitmayr $
* @file                                                                   */
/* ======================================================================= */

#include <cassert>
#include <vector>

#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/lists/SbStringList.h>

#include "TourGroup.h"
#include "Navigator.h"


// #define OCAR_DEBUG

#ifdef OCAR_DEBUG
    #define DEBUG(a) a
#else
    #define DEBUG(a) ;
#endif


SO_NODE_SOURCE(TourGroup);

void TourGroup::initClass(void)
{
    static SbBool first = TRUE;
    if (first) {
        SO_NODE_INIT_CLASS(TourGroup, SoNode, "Node" );
        first = FALSE;
    } 
}

TourGroup::TourGroup()
{
    SO_NODE_CONSTRUCTOR(TourGroup);

    isBuiltIn = TRUE;

	SO_NODE_ADD_FIELD( name, (""));
    SO_NODE_ADD_FIELD( timeOut, (10.0));
	SO_NODE_ADD_FIELD( neighbor, (""));
    SO_NODE_ADD_FIELD( neighborStation, (0));
    SO_NODE_ADD_FIELD( neighborWaypoint, (""));    
    SO_NODE_ADD_FIELD( neighborDestination, (""));
    SO_NODE_ADD_FIELD( neighborStamp, (0.0));
	SO_NODE_ADD_FIELD( activeNeighbor, (""));
    SO_NODE_ADD_FIELD( activePosition, (0,0,0));
    SO_NODE_ADD_FIELD( activeStation, (-1));
	SO_NODE_ADD_FIELD( userDestination, (""));
    SO_NODE_ADD_FIELD( userPosition, (0,0,0));
    SO_NODE_ADD_FIELD( userWaypoint, (""));
    SO_NODE_ADD_FIELD( userStation, (0));
    SO_NODE_ADD_FIELD( currentPath, (""));
    SO_NODE_ADD_FIELD( listboxFeedback, (""));
    SO_NODE_ADD_FIELD( mode, (OFF));
    SO_NODE_ADD_FIELD( navigator, (NULL));
    SO_NODE_ADD_FIELD( waitingForPath, (FALSE));
    SO_NODE_ADD_FIELD( send, (""));
    SO_NODE_ADD_FIELD( receive, (""));

    neighbor.deleteValues(0);
    neighbor.setDefault( TRUE );

	activeNeighborSensor.setFunction( TourGroup::activeNeighborChanged );
	activeNeighborSensor.setData( this );
    activeNeighborSensor.attach( &activeNeighbor );
    modeSensor.setFunction( TourGroup::modeChanged);
    modeSensor.setData( this );
    modeSensor.attach( &mode );
    pathSensor.setFunction( TourGroup::pathChanged );
    pathSensor.setData( this );
    listboxFeedbackSensor.setFunction( TourGroup::listboxFeedbackChanged );
    listboxFeedbackSensor.setData( this );
    listboxFeedbackSensor.attach( & listboxFeedback );
    receiveSensor.setFunction( TourGroup::receiveCB );
    receiveSensor.setData( this );
    receiveSensor.attach( &receive );
    pingTimer.setInterval( timeOut.getValue() / 2 );
    pingTimer.setFunction( TourGroup::pingCB );
    pingTimer.setData( this );
    pingTimer.schedule();

    activePosition.enableConnection( FALSE );
}

TourGroup::~TourGroup()
{
}

void TourGroup::activeNeighborChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
	TourGroup * self = (TourGroup *)data;

    if( self->activeNeighbor.getNum() > 1 )
        self->activeNeighbor.deleteValues(1);
    else if( self->activeNeighbor.getNum() == 0)
        self->activeNeighbor.setValue("");

    DEBUG(printf( "TourGroup %s::activeNeighborChanged %s\n",self->name.getValue().getString(), self->activeNeighbor[0].getString()));

    self->update();
}

void TourGroup::modeChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    TourGroup * self = (TourGroup *)data;

    DEBUG(printf( "TourGroup %s::modeChanged %i\n",self->name.getValue().getString(), self->mode.getValue()));

    self->update();
}

void TourGroup::pathChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    TourGroup * self = (TourGroup *)data;
    
    DEBUG(printf( "TourGroup %s::pathChanged %i\n",self->name.getValue().getString(), self->waitingForPath.getValue()));

    if( self->waitingForPath.getValue() && self->currentPath.getNum() > 0)
    {
        self->waitingForPath.setValue( FALSE );
        self->pathSensor.detach();
        int index = self->currentPath.getNum() / 2;
        self->userDestination.setValue( self->currentPath[index] );
        ((Navigator*)self->navigator.getValue())->destination.setValue( self->currentPath[index] );

        std::vector<SbString> msg(4);
        msg[0] = self->name.getValue();
        msg[1] = "DESTINATION";
        msg[2] = self->activeNeighbor[0];
        msg[3] = self->currentPath[index];
        self->sendData( msg );

        DEBUG(printf("TourGroup %s::pathChanged to %i, %s\n", self->name.getValue().getString(), index,
                      self->currentPath[index].getString()));
    }
}

void TourGroup::listboxFeedbackChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    TourGroup * self = (TourGroup *)data;
    
    DEBUG(printf( "TourGroup %s::listbox...Changed %i\n",self->name.getValue().getString(), self->listboxFeedback.getValue()));

    if( self->mode.getValue() == MEET )
    {
        std::vector<SbString> msg(4);
        msg[0] = self->name.getValue();
        msg[1] = "DESTINATION";
        msg[2] = self->activeNeighbor[0];
        msg[3] = self->listboxFeedback.getValue();
        self->sendData( msg );
        ((Navigator*)self->navigator.getValue())->destination.setValue( self->listboxFeedback.getValue());
    }
    else
        self->update();
}

void TourGroup::update(void)
{
    activePosition.enableConnection( FALSE );
    int activeInd = neighbor.find( activeNeighbor[0]);
    if( activeInd == -1 )
    {
        activeStation.setValue(-1);
        return;    
    }
    activeStation.setValue( neighborStation[activeInd] );

    // do whatever is necessary 
    switch( mode.getValue())
    {
    case TourGroup::OFF:
        break;
    case TourGroup::FOLLOW:
        activePosition.enableConnection( TRUE );
        activePosition.touch();
        break;
    case TourGroup::MEET:
        waitingForPath.setValue( TRUE );
        pathSensor.attach( &currentPath );
        ((Navigator*)navigator.getValue())->destination.setValue( neighborWaypoint[activeInd] );
        break;
    case TourGroup::GUIDE:
        {
            std::vector<SbString> msg(4);
            msg[0] = name.getValue();
            msg[1] = "DESTINATION";
            msg[2] = activeNeighbor[0];
            msg[3] = listboxFeedback.getValue();
            sendData( msg );
        }
        break;
    default:
        // can happen because of the way the engines work to compute the mode value !
        break;
    }
}

void TourGroup::receiveCB( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    TourGroup * self = (TourGroup *)data;
    self->receiveData();
}

void TourGroup::receiveData(void)
{
    if( receive.getNum() > 3 )
    {
        SbString senderName = receive[0];
        SbString command = receive[1];

        // ignore our own messages
        if ( senderName == name.getValue())
            return;

        int senderNameInd = neighbor.find( senderName );
        if( senderNameInd == -1 )
        {
            if( command == "PING" )
            {
                // add new collaborator
                senderNameInd = neighbor.getNum();
                neighbor.set1Value( senderNameInd, senderName);
                neighborStamp.set1Value( senderNameInd, SbTime::getTimeOfDay());
                // workaround for bug in SoMField::set1 not expanding array
                neighborStation.setNum( senderNameInd + 1 ); 
                neighborStation.set1( senderNameInd, receive[2].getString() );
                neighborWaypoint.set1Value( senderNameInd, receive[3] );
                neighborDestination.set1Value( senderNameInd, receive[4] );                
            }
            else
                DEBUG(printf("TourGroup::receiveData command %s from unknown user %s\n", command.getString(), senderName.getString()));
        } 
        else if( command == "PING" )
        {
            // update data
            neighborStamp.set1Value( senderNameInd, SbTime::getTimeOfDay());
            neighborStation.set1( senderNameInd, receive[2].getString() );
            neighborWaypoint.set1Value( senderNameInd, receive[3] );
            neighborDestination.set1Value( senderNameInd, receive[4] );
            // test if == active user and act accordingly
            if( activeNeighbor.getNum() > 0 && 
                senderName == activeNeighbor[0] && 
                neighborStation[senderNameInd] != activeStation.getValue())
            {
                activeStation.setValue( neighborStation[senderNameInd] );
            }
        }
        else if( command == "DESTINATION" && receive[2] == name.getValue() )
        {
            ((Navigator*)navigator.getValue())->destination.setValue( receive[3]);
        }
    }
}

void TourGroup::pingCB( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    TourGroup * self = (TourGroup *)data;    

    if( self->timeOut.getValue() != self->pingTimer.getInterval() )
    {
        self->pingTimer.unschedule();
        self->pingTimer.setInterval( self->timeOut.getValue() / 2);
        self->pingTimer.schedule();
    }
    
    std::vector<SbString> msg(5);
    msg[0] = self->name.getValue();
    msg[1] = "PING";
    self->userStation.get( msg[2]);
    msg[3] = self->userWaypoint.getValue();
    msg[4] = self->userDestination.getValue();
    self->sendData( msg );

    // check for timed out neighbors
    for( int i = 0 ; i < self->neighbor.getNum(); i++ )
    {
        if( self->neighborStamp[i] < (SbTime::getTimeOfDay() - self->timeOut.getValue()))
        {
            if( self->activeNeighbor.getNum() > 0 && self->neighbor[i] == self->activeNeighbor[0])
                self->activeNeighbor.setValue("");
            self->neighbor.deleteValues(i,1);
            self->neighborStamp.deleteValues(i,1);
            self->neighborStation.deleteValues(i,1);
            self->neighborDestination.deleteValues(i,1);
            self->neighborWaypoint.deleteValues(i,1);
        }
    }
}

void TourGroup::sendData(std::vector<SbString> & msg)
{    
    send.enableNotify( FALSE );
    send.setNum( msg.size() );
    send.setValues( 0, msg.size(), &(*msg.begin()) );
    send.enableNotify( TRUE );
    send.touch();
}
