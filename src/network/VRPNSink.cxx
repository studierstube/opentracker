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
/** source file for VRPNSink node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include "VRPNSink.h"

#ifdef USE_VRPN

#include <assert.h>

using namespace std;

#include <vrpn_Tracker.h>
#include <vrpn_Button.h>


VRPNSink::VRPNSink():
    Node(), 
    name( ""),
    type( TRACKER ),
    station( 0 ),
    server( NULL )
{
}

VRPNSink::~VRPNSink()
{
    if( server != NULL )
    {
        delete server;
        server = NULL;
    }        
}

void VRPNSink::start(vrpn_Connection * connection)
{
    switch(type){
    case TRACKER:
        server = new vrpn_Tracker_Server( name.c_str(), connection, station + 1);
        break;
    case BUTTON:
        server = new vrpn_Button_Server( name.c_str(), connection, 8 );
        break;
    default:
        assert(FALSE && "VRPNSink undefined type!");
    }
}

void VRPNSink::mainloop()
{
    server->mainloop();
}

void VRPNSink::onEventGenerated( State& event, Node& generator)
{
    switch(type){
    case TRACKER:
        {
            vrpn_float64 position[3] = 
            {
                event.position[0],
                event.position[1],
                event.position[2]
            };
            vrpn_float64 orientation[4] = 
            {
                event.orientation[0],
                event.orientation[1],
                event.orientation[2],
                event.orientation[3]
            };

            timeval time;
            time.tv_sec = event.time / 1000.0;
            time.tv_usec = ( event.time - time.tv_sec * 1000.0 ) * 1000;

            ((vrpn_Tracker_Server *)server)->report_pose( station, time, position, orientation );
        }
        break;
    case BUTTON:
        {
            int i;
            for( i = 0; i < 8; i++ )
            {
                ((vrpn_Button_Server *)server)->set_button(i, (event.button & (1 << i)));
            }
        }
        break;
    default:
        assert(FALSE && "VRPNSink undefined type!");
    }
    updateObservers( event );
}

#endif
