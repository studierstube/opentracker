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
/** source file for GPS_Handler
  *
  * @author Gerhard Reitmayr
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPS_Handler.cxx,v 1.2 2003/04/03 14:45:57 tamer Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "GPS_Handler.h"
#include "GPSDriver.h"

GPS_Handler::GPS_Handler(GPSDriver * parent_) :
parent( parent_ ),
nmeaind( 0 )
{
	
}

GPS_Handler::~GPS_Handler()
{
	
}

int GPS_Handler::open( void * factory )
{
#ifdef WIN32
	return reactor()->register_handler(this);
#else
	return reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
#endif
}

int	GPS_Handler::handle_input(ACE_HANDLE fd)
{
  return handle_signal(0, NULL, NULL);
}

int GPS_Handler::handle_signal( int, siginfo_t *, ucontext_t * )
{
    int ncnt, rd;
	GPSListener::GPSPoint point;
	char rptbuf[NMEABUFSZ];

    ncnt = peer().recv( nmeabuf + nmeaind, NMEABUFSZ - nmeaind);
    if (ncnt > 0) {
		nmeaind += ncnt;
		while ((rd = nmeaRead(nmeabuf, nmeaind, &point)) > 0) {
			if (point.fix > 0) {
				if( parent->getDebug() )
				{
					printf("GPS R %0.8f %0.8f %0.2f - - - %0.1f %0.1f\n",
						point.lat, point.lon, point.height,
						point.hdop, point.numsats);
				}
				// copy data to opentracker sink here !
				parent->new_point( point );

				/* only send 2 digit on the wire */
				ACE_OS::snprintf(rptbuf, NMEABUFSZ,
					"R %0.2f %0.2f %0.2f -- %d %0.2e %0.2e %0.2e %0.1f %0.1f\n",
					point.lat, point.lon, point.height, (double)1,
					(double)1.0, (double)1.0, (double)1.0,
					(double)point.hdop,
					(double)point.numsats );				
				// write to DGPSIP_Handler
				parent->send_dgpsip( rptbuf, strlen(rptbuf));
			}
			else
			{
				if( parent->getDebug() )
				{					
					printf("GPS no fix\n");
				}
			}
			nmeaind -= rd;
			memmove(nmeabuf, nmeabuf + rd, nmeaind);
		}
	}
	return 0;
}
