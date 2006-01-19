 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

//using namespace std;
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
