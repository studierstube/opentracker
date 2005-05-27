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
  * PROJECT: PUCInformationServer
  * ======================================================================== */
/** source file for PUCInformationServer.
  *
  * @author Thomas Psik
  *
  * $Id: PUCInformationServer.cpp 3823 2004-10-05 12:27:08Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <string>
#include <iostream>
#include <algorithm>
#include <xercesc/util/PlatformUtils.hpp>

#include "PUCBasicLib/PUCInformationServer.h"
#include "PUCBasicLib/PUCDeviceServer.h"
#include "PUCBasicLib/PUCMessages.h"

using namespace std;
XERCES_CPP_NAMESPACE_USE


#if defined(_DEBUG_PUC) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

// ----------------------------------------------------------------------------------------------

PUCServerInfo_Handler::PUCServerInfo_Handler()
{
    acceptor = NULL;
}

// ----------------------------------------------------------------------------------------------

PUCServerInfo_Handler::~PUCServerInfo_Handler()
{
    if( acceptor != NULL )
        acceptor->removeClient( this );
}

// ----------------------------------------------------------------------------------------------

// on new connection add client to list in acceptor
int PUCServerInfo_Handler::open( void * factory )
{
    int result = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open( factory );
    if( result == 0)
    {
        PUCServerInfo_Acceptor * acceptor_ = (PUCServerInfo_Acceptor *)factory;
		setAcceptor(acceptor_);
        acceptor_->addClient( this );
    }
    return result;
}

// ----------------------------------------------------------------------------------------------

// on new message   
int PUCServerInfo_Handler::handle_input(ACE_HANDLE fd)
{
	/*
 char buf[2048];
    int cnt;	
    cnt = peer().recv( buf, sizeof(long));
	if (cnt < 0 || cnt > sizeof(long))
	{
		acceptor->removeClient(this);
		return -1;
	}
	long length = (long) *(long*)buf;
	long lengthL = ntohl(length);
		DEBUG_CODE( cout << "length " <<lengthL <<endl;);
	if (lengthL < 0 || lengthL > 2000) return 1;
	
	// since version PUC/2.0 msgLen and Xml-Length may differ ?? Probably due to binary data.
	// so just recieve 4 more bytes ....
	cnt = peer().recv( buf, sizeof(long));
	if (cnt < 0 || cnt > sizeof(long))
	{
		acceptor->removeClient(this);
		return -1;
	}

	cnt = peer().recv_n( buf, lengthL);
	if (cnt < 0 || cnt > lengthL)
	{
		acceptor->removeClient(this);
		return -1;
	}
	
	// add trailing '\0' for end of string
	buf[cnt] = 0;

	DEBUG_CODE( cout << "PUCServerInfo_Handler received [" << cnt << "] bytes" << endl << buf << endl;);

	if (cnt > 8) // no xml message -> dump it
	{
		PUCMessage *msg = PUCMessage::Decode(string(buf));
		
		if (msg) 
		{
			acceptor->getServer()->handleMsg(msg, this);
			delete msg;
		}
	}
	else
	{
		DEBUG_CODE( cout << "PUCServerInfo_Handler message dumped due to small < 10 size " << endl;);
	}
	return 0;

*/

	char buf[2048];
    int cnt;	
    cnt = peer().recv( buf, sizeof(long));
	if (cnt < 0 || cnt > sizeof(long))
	{
		acceptor->removeClient(this);
		return -1;
	}
	long length = (long) *(long*)buf;
	long lengthL = ntohl(length);
		DEBUG_CODE( cout << "length of info packet " <<lengthL <<endl;);
	if (lengthL < 0 || lengthL > 2000) return 1;
	
	// since version PUC/2.0 msgLen and Xml-Length may differ ?? Probably due to binary data.
	// so just recieve 4 more bytes ....
	cnt = 0;
	int wait = 0;
	while (cnt < 4&& wait < 300) {
		int ret = peer().recv( buf, sizeof(long));
		if (ret > 0)
			cnt +=ret;
	}

	if (cnt < 0 || cnt > sizeof(long))
	{
		acceptor->removeClient(this);
		return -1;
	}

	cnt  = 0;
	wait = 0;
	while (cnt < lengthL && wait < 300)
	{
		int ret = peer().recv_n( buf, lengthL);
		if (ret > 0) 
			cnt += ret;
		wait ++;
	}	
	if (cnt < 0 || cnt > lengthL)
	{
		printf(" problem reading all information %d/%d/%d \n", cnt, lengthL, wait);
		acceptor->removeClient(this);
		return -1;
	}

	// add trailing '\0' for end of string
	buf[cnt] = 0;

	DEBUG_CODE( cout << "PUCServerInfo_Handler received [" << cnt << "] bytes" << endl << buf << endl;);

	if (cnt > 8) // no xml message -> dump it
	{
		PUCMessage *msg = PUCMessage::Decode(string(buf));
		
		if (msg) 
		{
			acceptor->getServer()->handleMsg(msg, this);
			delete msg;
		}
	}
	else
	{
		DEBUG_CODE( cout << "PUCServerInfo_Handler message dumped due to small < 10 size " << endl;);
	}
	return 0;

}    

// ----------------------------------------------------------------------------------------------

PUCServerInfo_Acceptor::~PUCServerInfo_Acceptor()
{
	vector<PUCServerInfo_Handler *>::reverse_iterator it = clients.rbegin();
    while( it != clients.rend())
    {
        (*it)->destroy();
        it++;
    }
    clients.clear();
}

// ----------------------------------------------------------------------------------------------

void PUCServerInfo_Acceptor::addClient( PUCServerInfo_Handler * client )
{
    if( find( clients.begin(), clients.end(), client) == clients.end())
	{ 
		clients.push_back( client );
		DEBUG_CODE(cout << "client added [" << client <<"]" << endl);
	}
}

// ----------------------------------------------------------------------------------------------

void PUCServerInfo_Acceptor::removeClient( PUCServerInfo_Handler * client )
{
    vector<PUCServerInfo_Handler *>::iterator it = find( clients.begin(), clients.end(), client);
    if( it != clients.end())
	{
        clients.erase( it );
		DEBUG_CODE(cout << "client removed [" << *it <<"]" << endl);
	}
}

// ----------------------------------------------------------------------------------------------

void PUCServerInfo_Acceptor::sendToAllClients( const char * buffer, const int len )
{
	vector<PUCServerInfo_Handler *>::iterator it;
    for( it = clients.begin(); it != clients.end(); it ++ )
    {
        if( (*it)->peer().send_n( buffer, len ) < 0 )
        {
            (*it)->destroy();
            it--;
        }
    }
}

// ----------------------------------------------------------------------------------------------

const int PUCInformationServer::SERVER_INFO_PORT = 5149;

// ----------------------------------------------------------------------------------------------

PUCInformationServer::PUCInformationServer(ACE_Reactor * reactor_):
reactor( reactor_ )
{
	running = false;
	if( NULL == reactor )
		reactor = ACE_Reactor::instance();
	serverThread = NULL;
}

// ----------------------------------------------------------------------------------------------


PUCInformationServer::~PUCInformationServer() 
{
	close();
}  


// ----------------------------------------------------------------------------------------------

void PUCInformationServer::run()
{
	if (running) return;
	
	if( startServer(  ) != 0 )
	{
		cout << "PUCInformationServer could not start listen info port [" << SERVER_INFO_PORT <<"] !\n";
		return;
	}

	running = true;
	
}

// ----------------------------------------------------------------------------------------------

int PUCInformationServer::startServer() 
{
 	cout << "[PUC] PUCInformationServer starts.\n";
    int result = 0;

    infoAcceptor = new PUCServerInfo_Acceptor(this);
    result = infoAcceptor->open( ACE_INET_Addr( SERVER_INFO_PORT) , reactor ) ;
	if(result != 0 )
    {
        delete infoAcceptor;
        infoAcceptor = NULL;
        cerr << "[PUC] PUCInformationServer could not open PUCInformationServerInfo server on port " << SERVER_INFO_PORT << " !\n";
    }

 	cout << "[PUC] PUCInformationServer listening on port [" << SERVER_INFO_PORT << "]\n";
	return result;

}

// ----------------------------------------------------------------------------------------------

void PUCInformationServer::close()
{
	running = false;

	if( infoAcceptor != NULL )
    {
        delete infoAcceptor;
        infoAcceptor = NULL;
    }
}

// ----------------------------------------------------------------------------------------------

void PUCInformationServer::sendMessageToClient(string msg, PUCServerInfo_Handler *client)
{
	char buffer[4];
	// special PUC -> no network conversion in byte ordering !!
	//*(int*)(&buffer) = htonl(retMsg.size());
	*(long*)(&buffer) = htonl(msg.size());
	int ret = client->peer().send_n( buffer, sizeof(long) ) ;
	if (ret < 0)
	{
		client->getAcceptor()->removeClient(client);
		return;
	}
	// since version PUC/2.0 have to send size 2 times
    ret = client->peer().send_n( buffer, sizeof(long) ) ;
	if (ret == 4)
	{
		ret = client->peer().send_n( msg.c_str(), msg.size() ) ;
		if (ret < 0)
		{
			client->getAcceptor()->removeClient(client);
		}
	}
}

// ----------------------------------------------------------------------------------------------

void PUCInformationServer::handleMsg(PUCMessage *msg, PUCServerInfo_Handler *client) 
{
	std::string retMsg;
	switch (msg->getType())
	{
		case PUCMessage::SERVER_INFORMATION_REQUEST:
		{
			PUCDeviceArray devices;
			vector<PUCDeviceServer *>::iterator it;
			for( it = deviceServers.begin(); it != deviceServers.end(); it ++ )
			{
				devices.push_back(PUCDeviceInfo((*it)->getDeviceName(), (*it)->getPort()));
			}

			DEBUG_CODE(cout << "[PUC] react on [SERVER_INFORMATION_REQUEST] sending server information" <<endl;)

			PUCServerInformation info = PUCServerInformation(serverName.c_str(), devices );
			retMsg = info.getXML();
			DEBUG_CODE(cout << "[PUC] " << retMsg <<endl;)

			sendMessageToClient(retMsg, client);
			break;
		}
	}

}

// ----------------------------------------------------------------------------------------------

