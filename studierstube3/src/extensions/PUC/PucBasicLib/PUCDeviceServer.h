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
  * $Id: PUCDeviceServer.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _PUCDEVICESERVER_H
#define _PUCDEVICESERVER_H

#include <ace/Svc_Handler.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>

#include <vector>
#include "PUCBasicLib/PUCMessages.h"
#include "PUCBasicLib/PUCInterface.h"

class PUCServerDevice_Acceptor;
class PUCServerDevice_Handler;
class PUCDeviceServer;

/**
 * 
 * @ingroup pucbasicLib
 */
class PUCServerDevice_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	PUCServerDevice_Handler();
	virtual ~PUCServerDevice_Handler();

	void setAcceptor(PUCServerDevice_Acceptor * acceptor_) 
	{
		acceptor = acceptor_;
	}

    PUCServerDevice_Acceptor * getAcceptor() const
	{
		return acceptor;
	}

    virtual int open( void * factory );
    virtual int handle_input(ACE_HANDLE fd);    
    
protected:
	PUCServerDevice_Acceptor * acceptor;

};

/* ***
*/

class PUCServerDevice_Acceptor : public ACE_Acceptor<PUCServerDevice_Handler, ACE_SOCK_ACCEPTOR> 
{
public:
	PUCServerDevice_Acceptor(PUCDeviceServer *server_) 
	{
		server = server_;
	}

	virtual ~PUCServerDevice_Acceptor();
	void addClient( PUCServerDevice_Handler * client );
    void removeClient( PUCServerDevice_Handler * client );    

	void sendToAllClients(const char * buffer, const int len );

	PUCDeviceServer  *getServer() { return server; }
protected:
   std::vector<PUCServerDevice_Handler *> clients;
   PUCDeviceServer *server;
};

/* ***
*/

//typedef std::string (*stringCallBackMethod)(void*);
//typedef void (*valueChangeReqMethod)(void *, std::string, std::string);
//typedef void (*fullStateMethod)(void *);

/* ***
*/

class PUCDeviceServer
{
public:

	PUCDeviceServer(ACE_Reactor * reactor_ = NULL ) ;

	virtual ~PUCDeviceServer();

	/* thread start function */
//	void run();

	int startServer( );

	// send message functions
	void sendMessageStateChangeNotification(std::string stateName, std::string stateValue);
	void sendCommandInvoke(std::string commandName);
	
	void setPort (int port_) { port = port_;}
	int  getPort () const { return port;}

	void setInterface(PUCInterface *interface_)
	{
		interfacePtr = interface_;
	}
	
	
	void setDeviceName(std::string name_) { deviceName = name_; }
	std::string getDeviceName() const { return deviceName; }
	
	static std::string getQuotedString(const std::string inStr);
	
	void close();

protected:
	/* internal functions */

	ACE_Reactor * getReactor() const
	{
		return reactor;
	};
	
	void handleMsg(PUCMessage *msg, PUCServerDevice_Handler *client); 
	void sendMessageToClient(std::string msg, PUCServerDevice_Handler *client);

	int port;
	std::string deviceName;

	PUCInterface *interfacePtr;
	ACE_Reactor * reactor;
    PUCServerDevice_Acceptor * appAcceptor;

	friend PUCServerDevice_Handler;
};


#endif // !defined(_PUCDEVICESERVER_H)
