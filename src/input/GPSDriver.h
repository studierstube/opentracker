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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSDriver.h,v 1.5 2003/06/16 13:17:01 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _GPSDRIVER_H
#define _GPSDRIVER_H

#include <map>
#include <string>
#include <vector>

#include "GPSParser.h"

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
    /** 
     * This is the callback method called by the GPSDriver class, whenever new data
     * arrives. Any subclasses of GPSListener must override it to receive the data.
     * @param point pointer to the GPResult object that represents the parsed data.
     * @param line the corresponding string received from the GPS receiver.
     * @param uderData a pointer to user data stored when registering the listener.
     */
	virtual void newData( const GPResult * point, const char * line, void * userData ) = 0;
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

    bool hasFix( void )
    {
        return fix;
    }

protected:

	void send_rtcm( const char * buffer, const int len );
    void new_line( const char * line );

	ACE_Reactor * reactor;
	GPS_Handler * receiver;
	DGPSIP_Handler * server;
    DGPSMirror_Acceptor * acceptor;

	bool debugOn;
    bool fix;

	std::map<GPSListener *, void *> listeners;

    std::vector<DGPSMirror_Handler *> clients;

	friend class GPS_Handler;
	friend class DGPSIP_Handler;
};

#endif // !defined(_GPSDRIVER_H)
