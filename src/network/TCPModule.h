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
/** header file for TCPModule module for the virtual show case viewer.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/TCPModule.h,v 1.1 2001/11/09 22:07:45 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section tcpmodule TCPModule
 * The TCPModule is a simple network transmit module that implements a TCP/IP
 * server for data streams of several @ref tcpsink nodes. It is configured with a port
 * to listen to for new connections. All clients will get the same stream of
 * data that consists of a station number for the next station and position and
 * orientation information. Clients can connect and disconnect at will. The configuration 
 * element is @c TCPConfig and has the following attributes :
 * @li @c port port number to listen for new connections
 *
 * An example configuration element looks like this :
 * @verbatim
<TCPConfig port="12345"/>@endverbatim
 * 
 * This module was implemented specifically for the virtual show case viewer of
 * Oliver Bimber <obimber@crcg.edu>. 
 */

#ifndef _TCPMODULE_H
#define _TCPMODULE_H

#include "TCPSink.h"
#include "../OpenTracker.h"

// forward declaration of the ACE stream socket class
class ACE_SOCK_Stream;

/** TCPModule implements a multithreaded module to serve a list of clients
 * with a stream of data captured by TCPSink nodes via TCP. The thread is 
 * to establish new connections, in the main thread the sinks are checked for
 * new data and their data is send over the connections. There is no 
 * guaranteed order in which the connection data is send. 
 *
 * The protocol in detail is :
 * @li int - station number
 * @li unsigned char - protocol header
 * @li 3 floats - position in x, y, z 
 * @li 4 floats - orientation as x, y, z, w
 * @li short unsigned - button states
 * @li double - timestamp
 * All data is send in network byte order.
 *
 * @author Gerhard Reitmayr
 * @ingroup network
 */
class OPENTRACKER_API TCPModule  : public ThreadModule, public NodeFactory {

protected:
    /// list of sink nodes 
	NodeVector sinks;
    /// list of sockets that connect to clients
    std::vector<ACE_SOCK_Stream *> connections;
    /// port number to listen to
	int port;
    /// flag to stop the listening thread 
    int running;

    void run();

public:

    TCPModule() : running(0)
	{};

	~TCPModule();

    Node * createNode( const std::string& name,  StringTable& attributes);
 
	void pullState();
	
	void close();

	void init(StringTable& attributes,  ConfigNode * localTree);
    
	void start();
};

#endif