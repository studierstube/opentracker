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
/** source file for PUCServer.
*
* @author Thomas Psik
*
* $Id: PUCDeviceServer.cpp 3823 2004-10-05 12:27:08Z tomp $
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

#include "PUCBasicLib/PUCDeviceServer.h"
//#include "PUCBasicLib/PUCServerAcceptor.h"
#include "PUCBasicLib/PUCMessages.h"

using namespace std;
XERCES_CPP_NAMESPACE_USE

#if defined(_DEBUG_PUC) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif


// ----------------------------------------------------------------------------------------------

PUCServerDevice_Handler::PUCServerDevice_Handler()
{
    acceptor = NULL;
}

// ----------------------------------------------------------------------------------------------

PUCServerDevice_Handler::~PUCServerDevice_Handler()
{
    if( acceptor != NULL )
        acceptor->removeClient( this );
}

// ----------------------------------------------------------------------------------------------

// on new connection add client to list in acceptor
int PUCServerDevice_Handler::open( void * factory )
{
    int result = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open( factory );
    if( result == 0)
    {
        PUCServerDevice_Acceptor * acceptor_ = (PUCServerDevice_Acceptor *)factory;
		setAcceptor(acceptor_);
        acceptor_->addClient( this );
    }
    return result;
}

// ----------------------------------------------------------------------------------------------

// on new message   
int PUCServerDevice_Handler::handle_input(ACE_HANDLE fd)
{
	char buf[2048];
    int cnt;	
	int wait = 0;
	cnt  = -1;
	while (cnt == -1 && wait < 300)
	{
		cnt = peer().recv( buf, sizeof(long));
		wait++;
	}
	if (cnt < 0 || cnt > sizeof(long))
	{
		printf("error recv length [%d/%d]\n",cnt, wait);
		acceptor->removeClient(this);
		return -1;
	}
	
	long length = (long) *(long*)buf;
	long lengthL = ntohl(length);
	DEBUG_CODE( cout << "length of msg " <<lengthL <<endl;);
	if (lengthL < 0 || lengthL > 2000)	
	{
		acceptor->removeClient(this);
		return -1;
	}
	
	
	// since version PUC/2.0 msgLen and Xml-Length may differ ?? Probably due to binary data.
	// so just recieve 4 more bytes ....
	cnt  = -1;
	wait = 0;
	while (cnt == -1 && wait < 100)
	{
		cnt = peer().recv( buf, sizeof(long));
		wait ++;
		Sleep(100);
	}
	if (cnt < 0 || cnt > sizeof(long))
	{
		printf("error recv length 2 [%d/%d]\n",cnt, wait);
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
		acceptor->removeClient(this);
		return -1;
	}
	// add trailing '\0' for end of string
	buf[cnt] = 0;
	
	DEBUG_CODE( cout << "PUCServerDevice_Handler received [" << cnt << "] bytes" << endl << buf << endl;);
	
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
		DEBUG_CODE( cout << "PUCServerDevice_Handler message dumped due to small < 10 size " << endl;);
	}
	return 0;
}    

// ----------------------------------------------------------------------------------------------

PUCServerDevice_Acceptor::~PUCServerDevice_Acceptor()
{
	vector<PUCServerDevice_Handler *>::reverse_iterator it = clients.rbegin();
    while( it != clients.rend())
    {
        (*it)->destroy();
        it++;
    }
    clients.clear();
}

// ----------------------------------------------------------------------------------------------

void PUCServerDevice_Acceptor::addClient( PUCServerDevice_Handler * client )
{
    if( find( clients.begin(), clients.end(), client) == clients.end())
	{ 
		clients.push_back( client );
		DEBUG_CODE(cout << "client added [" << client <<"]" << endl);
	}
}

// ----------------------------------------------------------------------------------------------

void PUCServerDevice_Acceptor::removeClient( PUCServerDevice_Handler * client )
{
    vector<PUCServerDevice_Handler *>::iterator it = find( clients.begin(), clients.end(), client);
    if( it != clients.end())
	{
        clients.erase( it );
		DEBUG_CODE(cout << "client removed [" << *it <<"]" << endl);
	}
}

// ----------------------------------------------------------------------------------------------

void PUCServerDevice_Acceptor::sendToAllClients( const char * buffer, const int len )
{
	char bufferNum[4];
	// special PUC -> no network conversion in byte ordering !!
	//*(int*)(&buffer) = htonl(retMsg.size());
	//	*(int*)(&bufferNum) = (long) len;
	
	*(long*)(&bufferNum) = htonl(len);
	
	vector<PUCServerDevice_Handler *>::iterator it;
    for( it = clients.begin(); it != clients.end(); it ++ )
    {
		if  ( 
			((*it)->peer().send_n( bufferNum, sizeof(long))  < 0 )
			|| 
			((*it)->peer().send_n( bufferNum, sizeof(long))  < 0 )
			|| 
			((*it)->peer().send_n( buffer, len ) < 0 ) 
			)
        {
            (*it)->destroy();
            it--;
        }
    }
}


// ----------------------------------------------------------------------------------------------

PUCDeviceServer::PUCDeviceServer(ACE_Reactor * reactor_):
reactor( reactor_ )
{
	if( NULL == reactor )
		reactor = ACE_Reactor::instance();
	
	//	devices.push_back(PUCDeviceInfo("testDevice"));
}

// ----------------------------------------------------------------------------------------------

PUCDeviceServer::~PUCDeviceServer() 
{
	close();
}  

// ----------------------------------------------------------------------------------------------
/*  

  void PUCDeviceServer::run()
  {
 	getReactor()->owner(ACE_Thread::self());
	if( startServer( ) != 0 )
	{
	cout << "[PUC] PUCDeviceServer could not start port [" << port <<"] !\n";
	return;
	}
	getReactor()->run_reactor_event_loop();
	close();
	}
*/


// ----------------------------------------------------------------------------------------------


int PUCDeviceServer::startServer() 
{
	cout << "[PUC] PUCDeviceServer starts.\n";
	
	int result = 0;
	if(port != -1)
    {
        appAcceptor = new PUCServerDevice_Acceptor(this);
        result = appAcceptor->open( ACE_INET_Addr( port) , reactor ) ;
		if(result != 0 )
        {
            delete appAcceptor;
            appAcceptor = NULL;
            cerr << "[PUC] PUCDeviceServer could not open PUCDeviceServer server on port " << port << " !\n";
        }
		
    }
	cout << "[PUC] PUCDeviceServer listening on port [" << port << "]\n";
	
	return result;
}

// ----------------------------------------------------------------------------------------------

void PUCDeviceServer::close()
{
	if( appAcceptor != NULL )
    {
        delete appAcceptor;
        appAcceptor = NULL;
    }
}

// ----------------------------------------------------------------------------------------------

void PUCDeviceServer::sendMessageToClient(string msg, PUCServerDevice_Handler *client)
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

void PUCDeviceServer::handleMsg(PUCMessage *msg, PUCServerDevice_Handler  *client) 
{
	std::string retMsg;
	switch (msg->getType())
	{
	case PUCMessage::SPEC_REQUEST:
		{
			DEBUG_CODE(cout << "[PUC] react on [SPEC_REQUEST] sending device information" <<endl;)
				
				std::string devSpecStr;
			
			if (interfacePtr != NULL)
			{
				devSpecStr = interfacePtr->getSpecificationMethod();
			}
			std::string quoted =  getQuotedString(devSpecStr);
			PUCDeviceSpec devSpec = PUCDeviceSpec(quoted);
			retMsg = devSpec.getXML();
			DEBUG_CODE(cout << retMsg <<endl;)
				
				sendMessageToClient(retMsg, client);
			break;
		}
	case PUCMessage::STATE_CHANGE_REQ:
		{
			if (!msg->isValid()) 
			{
				DEBUG_CODE(cout << "[PUC] illigal [STATE_CHANGE_REQ] detected !!" <<endl;)
					break;
			}
			PUCStateChangeRequest *msgStateReq = (PUCStateChangeRequest *)msg;
			DEBUG_CODE(cout << "[PUC] react on [STATE_CHANGE_REQ] calling function interfacePtr->stateChangeReqMethod() "<<endl;)
				if (interfacePtr != NULL)
				{
					interfacePtr->stateChangeReqMethod(msgStateReq->getStateName(), msgStateReq->getStateValue());
				}
				else
					DEBUG_CODE(cout << "[PUC] react on [FULL_STATE_REQ] interface == NULL !! "<<endl;)
					break;
		}
	case PUCMessage::FULL_STATE_REQ:
		{
			DEBUG_CODE(cout << "[PUC] react on [FULL_STATE_REQ] calling function interfacePtr->fullStateMethod() "<<endl;)
				if (interfacePtr != NULL)
				{
					interfacePtr->fullStateMethod();
				}
				else
					DEBUG_CODE(cout << "[PUC] react on [FULL_STATE_REQ] interface == NULL !! "<<endl;)
					
					break;
		}
	case PUCMessage::COMMAND_INVOKE_REQ:
		{
			// todo !!
			PUCCommandInvokeRequest *msgCommand = (PUCCommandInvokeRequest *)msg;
			DEBUG_CODE(cout << "[PUC] react on [COMMAND_INVOKE_REQ] calling function " << 0 <<endl;)
				
				if (interfacePtr != NULL)
				{
					interfacePtr->invokeCommandMethod(msgCommand->getCommandName());
				}
				break;
		}
		// case PUCMessage::STATE_CHANGE_NOT -> will be send
	}
}

// ----------------------------------------------------------------------------------------------

void PUCDeviceServer::sendMessageStateChangeNotification(std::string stateName, std::string stateValue)
{
	std::string retMsg;
	PUCStateChangeNotification changeNot = PUCStateChangeNotification(stateName, stateValue);
	retMsg = changeNot.getXML();
	//	DEBUG_CODE(cout << retMsg <<endl;)
	// send message		
	if (appAcceptor!=NULL)
		appAcceptor->sendToAllClients(retMsg.c_str(), retMsg.size());
}

// ----------------------------------------------------------------------------------------------

std::string PUCDeviceServer::getQuotedString(const std::string inStr)
{
	std::string ret;
	for (int i = 0 ; i< inStr.length(); i++)
	{
		if (inStr[i] == '<')
			ret += "&lt;";
		else if (inStr[i] == '>')
			ret += "&gt;";
		else 
			ret += inStr[i];
	}
	return ret;
}

// ----------------------------------------------------------------------------------------------
