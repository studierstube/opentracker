 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for VRPNSource node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include "VRPNSource.h"

#ifdef USE_VRPN

#include <assert.h>

// #define ACE_NTRACE 0

#include <ace/Log_Msg.h>

using namespace std;
using namespace ot;

#include <vrpn_Tracker.h>
#include <vrpn_Button.h>

void trackerPosOriCallback( void * userdata, const vrpn_TRACKERCB info )
{
    assert( userdata != NULL );
    VRPNSource * self = (VRPNSource *)userdata;
    if( info.sensor != self->station )
        return;

    self->state.position[0] = info.pos[0];
    self->state.position[1] = info.pos[1];
    self->state.position[2] = info.pos[2];
    
    self->state.orientation[0] = info.quat[0];
    self->state.orientation[1] = info.quat[1];
    self->state.orientation[2] = info.quat[2];
    self->state.orientation[3] = info.quat[3];
    
    self->state.time = info.msg_time.tv_sec * 1000.0 + info.msg_time.tv_usec / 1000.0;

    self->updateObservers( self->state );
}

void buttonChangeCallback( void * userdata, const vrpn_BUTTONCB info )
{
    assert( userdata != NULL );
    VRPNSource * self = (VRPNSource *)userdata;

    if( info.button > 7 ) // we only use the first 8 buttons
        return;

    if( info.state == VRPN_BUTTON_ON )
        self->state.button |= 1 << info.button;
    else
        self->state.button &= ~(1 << info.button);

    // appearantly VRPN doesn't send any timestamp with the buttons !
    // self->state.time = info.msg_time.tv_sec * 1000.0 + info.msg_time.tv_usec / 1000.0;
    self->state.timeStamp();
    
    self->updateObservers( self->state );
}

VRPNSource::VRPNSource():
    Node(), 
    name( ""),
    type( TRACKER ),
    station( 0 ),
    trackerObj( NULL )
{
}

VRPNSource::~VRPNSource()
{
    if( trackerObj != NULL )
    {
        delete trackerObj;
        trackerObj = NULL;
    }
}

void VRPNSource::start()
{
    ACE_TRACE(ACE_TEXT("VRPNSource::start"));
    vrpn_Tracker_Remote * tracker = NULL;
    vrpn_Button_Remote * button = NULL;

    switch(type){
    case TRACKER:
        tracker = new vrpn_Tracker_Remote( name.c_str());
        tracker->register_change_handler( this, trackerPosOriCallback );
        trackerObj = tracker;
        break;
    case BUTTON:
        button = new vrpn_Button_Remote( name.c_str());
        button->register_change_handler( this, buttonChangeCallback );
        trackerObj = button;
        break;
    default:
        assert(FALSE && "VRPNSource undefined type!");
    }
}

void VRPNSource::mainloop()
{
    trackerObj->mainloop();
}

#endif
