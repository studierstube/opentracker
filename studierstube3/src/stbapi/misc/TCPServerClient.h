#ifndef _SOCKETUTILS_H
#define _SOCKETUTILS_H

// ACE includes
#include <ace/OS.h>
#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"

// STL includes
#include <vector>
#include <string>

using namespace std;

class TCPAcceptor;
class TCPServer;

class TCPHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{

public:

    TCPHandler() {};
    virtual ~TCPHandler();

    int open(void *acceptor);
    int handle_input(ACE_HANDLE handle);

protected:

    TCPAcceptor *acceptor;
};

//------------------------------------------------------------------------------

class TCPAcceptor : public ACE_Acceptor<TCPHandler, ACE_SOCK_ACCEPTOR> 
{
public:

    TCPAcceptor(TCPServer *server_) { server = server_;	}
    virtual ~TCPAcceptor();
	
	void addClient(TCPHandler *client);
    void removeClient(TCPHandler *client);
	void sendToAllClients(string buffer);
    void receivedFromClient(string buffer);

protected:

   vector<TCPHandler*> clients;
   TCPServer *server;
};

//------------------------------------------------------------------------------

typedef void TCPReceiveCB(string msg,void *data);

class TCPServer
{

public:

    TCPServer();
    virtual ~TCPServer();

    void eventLoop();
    int open(int port);
    void send(string msg);
    void receive(string msg);
    void setReceiveCB(TCPReceiveCB *cb);
    void setUserData(void *data);

protected:

    ACE_Reactor *reactor;
    TCPAcceptor *acceptor;
    TCPReceiveCB *callback;
    void *userData;
};

//------------------------------------------------------------------------------

class TCPClient : public ACE_SOCK_Stream
{

public:
    
    TCPClient();
    TCPClient(const char *server,int port);
    virtual ~TCPClient();
    void setReceiveCB(TCPReceiveCB *cb);
    void setUserData(void *data);

    int open();
    int send(string msg);
    int receive();

protected:

    ACE_SOCK_Connector *connector;
    ACE_INET_Addr addr;
    TCPReceiveCB *callback;
    void *userData;

    bool initialized_;
};

#endif
