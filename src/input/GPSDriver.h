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
/** header file for GPSDriver
  *
  * @author Gerhard Reitmayr
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSDriver.h,v 1.2 2003/04/08 18:59:59 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _GPSDRIVER_H
#define _GPSDRIVER_H

#include "nmea.h"
#include <map>
#include <string>
#include <vector>

class GPS_Handler;
class DGPSIP_Handler;
class DGPSMirror_Handler;
class DGPSMirror_Acceptor;
class ACE_Reactor;

/**
 * This is an abstract interface for the clients of the GPSDriver class.
 * They have to register with the GPSDriver class and will be
 * called back, if new data arrives.
 * @ingroup input
 * @author Gerhard Reitmayr
 */
class GPSListener {
public:
	typedef struct llpoint GPSPoint;
	virtual void newPoint( const GPSPoint & point, void * userData ) = 0;
};

/**
 * The GPSDriver class encapsulates the whole GPS and ACE framework code
 * for reading data from the GPS receiver and also supplying it with
 * correction data from a DGPSIP host.
 *
 * @ingroup input
 * @author Gerhard Reitmayr
 * @todo fix and test for unix ! ( use handle_input because of different handling there,
 *       this will require a compile time switch in open, maybe )
 * @todo make tpc connection robust by reconnecting upon failure 
 * @todo remove static verbose variable for debugging output in GPS parser code.
 */
class GPSDriver  
{
public:
	GPSDriver( ACE_Reactor * reactor_ = NULL );
	virtual ~GPSDriver();

	int open( const std::string & device, int baud, const std::string & serveraddr = "", int serverport = 2101, int dgpsmirror = -1 );
	void close();

	void setDebug( bool debug );
	bool getDebug()
	{
		return debugOn;
	};

	ACE_Reactor * getReactor()
	{
		return reactor;
	};

	void addListener( GPSListener * listener, void * userData = NULL );
	void removeListener( GPSListener * listener, void * userData = NULL );

    void addClient( DGPSMirror_Handler * client );
    void removeClient( DGPSMirror_Handler * client );    

protected:

	void new_point( const GPSListener::GPSPoint & point );
	void send_dgpsip( const char * buffer, const int len );
	void send_rtcm( const char * buffer, const int len );

	ACE_Reactor * reactor;
	GPS_Handler * receiver;
	DGPSIP_Handler * server;
    DGPSMirror_Acceptor * acceptor;

	bool debugOn;

	std::map<GPSListener *, void *> listeners;

    std::vector<DGPSMirror_Handler *> clients;

	friend class GPS_Handler;
	friend class DGPSIP_Handler;
};

#endif // !defined(_GPSDRIVER_H)
