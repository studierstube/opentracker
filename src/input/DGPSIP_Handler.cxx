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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/DGPSIP_Handler.cxx,v 1.1 2003/03/27 18:26:02 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "DGPSIP_Handler.h"
#include "GPSDriver.h"
#include "rtcm.h"

DGPSIP_Handler::DGPSIP_Handler( GPSDriver * parent_ ) :
parent( parent_ )
{

}

DGPSIP_Handler::~DGPSIP_Handler()
{

}

int DGPSIP_Handler::open( void * factory )
{
	int result = ACE_Svc_Handler<ACE_SOCK_Stream, ACE_Null_Mutex, ACE_NULL_SYNCH>::open( factory );
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
	}
	return result;
}

int DGPSIP_Handler::handle_input(ACE_HANDLE fd)
{
	// handle rtcm input and send to receiver
	char buf[4*1024];
	int cnt;	
	while ((cnt = peer().recv( buf, sizeof(buf))) > 0) {
		// this is the rtcm decoder, use for debug purposes		
		if ( parent->getDebug() ) {
			int i;
			for (i = 0; i < cnt; i++) {
				new_byte(buf[i]);
			}
		}
		// send data to GPS receiver
		parent->send_rtcm( buf, cnt );
	}
	return 0;
}
