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
/** source file for DGPSIP_Handler
 *
 * @author Gerhard Reitmayr
 * 
 * $Id$
 *
 * @file                                                                   */
/* ======================================================================= */

#define ACE_NLOGGING

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <stdlib.h>
#include "../tool/FixWinCE.h"
#include "DGPSIP_Handler.h"
#include "rtcm.h"


namespace ot {

DGPSIP_Handler::DGPSIP_Handler( GPSDriver * parent_ ) :
    isReconnecting( false ),
    parent( parent_ ),
    counter( 0 )
{
    
}

DGPSIP_Handler::~DGPSIP_Handler()
{
    parent->removeListener(this);
}

int DGPSIP_Handler::open( void * factory )
{
    int result = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open( factory );
    if( result == 0)
    {
        // send initialization string here 
        char hn[1024];
        char buf[4*1024];
        ACE_OS::hostname( hn, sizeof(hn));
        // the last "\r\nR" part tells the server to send real time updates 
        // alternatively use "\r\nB" for bulk service
        ACE_OS::snprintf(buf, sizeof(buf), "HELO %s %s %s%s\r\n", hn, "GPSDriver", "0.1", "\r\nR");
        result = peer().send_n( buf, ACE_OS::strlen(buf));
        peer().get_remote_addr( remoteAddr  );
        parent->addListener( this );
        ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::open succeeded\n"));
    }
    return result;
}

int DGPSIP_Handler::handle_input(ACE_HANDLE fd)
{
    // handle rtcm input and send to receiver
    char buf[4*1024];
    int cnt;
    if ((cnt = peer().recv( buf, sizeof(buf))) > 0) {
        ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input received %i bytes\n", cnt));
        // this is the rtcm decoder, use for debug purposes		
        if ( parent->getDebug() ) {
            int i;
            for (i = 0; i < cnt; i++) {
                new_byte(buf[i]);
            }
        }
        ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input send to rtcm\n"));
        // send data to GPS receiver
        parent->send_rtcm( buf, cnt );
    }
    ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input received %i bytes and error %i != %i\n", cnt, errno, EWOULDBLOCK));
    if( cnt <= 0 )  // try a reconnect !
    {
        isReconnecting = true;
        ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input trying to reconnect...\n"));
        // remove ourselves from the reactor
        reactor()->remove_handler(fd, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL );
        peer().close();
        // try to reconnect
        const size_t MAX_RETRIES = 5;
        ACE_Time_Value timeout(1);
        size_t i;
        DGPSIP_Handler * This = (DGPSIP_Handler *) this;
        for( i = 0; i < MAX_RETRIES; ++i )
        {
            ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, timeout);
            DGPSIP_Connector ipconnect( reactor() );
            if( i > 0 ) 
                ACE_OS::sleep(timeout);
            ACE_DEBUG((LM_DEBUG, "%i...\n",i));
            if( ipconnect.connect(This, remoteAddr, options ) == 0 )
                break;
            timeout *= 2;                
        }
        isReconnecting = false;
        ACE_DEBUG((LM_DEBUG, "done %i\n",i));
        return i == MAX_RETRIES ? -1 : 0;
    }
    ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input returned\n"));
    return 0;
}

void DGPSIP_Handler::newData( const GPResult * res, const char *, void * userData )
{
    char rptbuf[1024];
    
    if( res->type == GPResult::GPGGA )
    {
        ++counter;
        if( counter % 10 == 0)
        {
            ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::newData send data to server\n"));
            GPGGA * point = (GPGGA *) res;
            // only send 2 digit on the wire to anonymize our position 
            ACE_OS::snprintf(rptbuf, 1024,
                "R %0.2f %0.2f %0.2f -- %d %0.2e %0.2e %0.2e %0.1f %0.1f\n",
                point->lat, point->lon, point->height, (double)1.0, (double)1.0, 
                (double)1.0, (double)1.0,(double)point->hdop,(double)point->numsats );
            peer().send_n( rptbuf, ACE_OS::strnlen( rptbuf, 1024 ));
            counter = 0;
        }	
    }
}

int DGPSIP_Handler::handle_close( ACE_HANDLE fd, ACE_Reactor_Mask mask )
{
    ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_close with isReconnecting %i\n", isReconnecting));
    if( !isReconnecting )
        return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::handle_close( fd, mask);
    return 0;
}

}  // namespace ot
