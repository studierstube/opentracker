 /* ========================================================================
  * Copyright (C) 2003  Vienna University of Technology
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
  * PROJECT: PUCServer
  * ======================================================================== */
/** header file for PUCServer.
  *
  * @author Thomas Psik
  *
  * $Id: PUCInformationServer.h 3846 2004-10-07 08:41:32Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _PUCINFORMATIONSERVER_H
#define _PUCINFORMATIONSERVER_H

#include <ace/Svc_Handler.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/Thread.h>
#include <ace/Synch.h>

#include <vector>

class PUCMessage;
class PUCServer;
class PUCServerInfo_Acceptor;
class PUCServerInfo_Handler;
class PUCDeviceServer;
class PUCInformationServer;

/**
 * The PUCBasicLib specifies the interface to create applications and libaries that want to use the PUC system. This library is at the time only
 * used by PUC for Studierstube.
 * @defgroup pucbasicLib PUCBasicLib
 * @ingroup puc
*/

class PUCServerInfo_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	PUCServerInfo_Handler();
	virtual ~PUCServerInfo_Handler();
	void setAcceptor(PUCServerInfo_Acceptor * acceptor_) 
	{
		acceptor = acceptor_;
	}

    PUCServerInfo_Acceptor * getAcceptor() const
	{
		return acceptor;
	}

    virtual int open( void * factory );
    virtual int handle_input(ACE_HANDLE fd);    
    
protected:
	PUCServerInfo_Acceptor * acceptor;

};

/* ***
*/

class PUCServerInfo_Acceptor : public ACE_Acceptor<PUCServerInfo_Handler, ACE_SOCK_ACCEPTOR> 
{
public:
	PUCServerInfo_Acceptor(PUCInformationServer *server_) 
	{
		server = server_;
	}

	virtual ~PUCServerInfo_Acceptor();
	void addClient( PUCServerInfo_Handler * client );
    void removeClient( PUCServerInfo_Handler * client );    

	void sendToAllClients(const char * buffer, const int len );

	PUCInformationServer *getServer() { return server; }
protected:
   std::vector<PUCServerInfo_Handler *> clients;
   PUCInformationServer *server;
};


typedef	std::vector<PUCDeviceServer *> PUCDevicePtrArray;

class PUCInformationServer
{
public:
	static const int SERVER_INFO_PORT;

	PUCInformationServer(ACE_Reactor * reactor_ = NULL ) ;

	virtual ~PUCInformationServer();

	/* thread start function */
	void run();

	/* internal functions */
	int startServer();

	ACE_Reactor * getReactor() const
	{
		return reactor;
	};
	
	void handleMsg(PUCMessage *msg, PUCServerInfo_Handler *client); 
	void sendMessageToClient(std::string msg, PUCServerInfo_Handler *client);
	
	void setServerName(std::string serverName_)
	{
		serverName = serverName_;
	}
	
	void addDevice (PUCDeviceServer * deviceServer_) 
	{
		deviceServers.push_back(deviceServer_);
	}
	
	bool isRunning() const { return running; }

	void close();	
protected:
	bool running;
	std::string serverName;
	ACE_Reactor * reactor;
    PUCServerInfo_Acceptor * infoAcceptor;

	PUCDevicePtrArray deviceServers;

	void *serverThread;
};


#endif // !defined(_PUCINFORMATIONSERVER_H)
