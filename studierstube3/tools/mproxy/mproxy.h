#ifndef _MProxy_H_INCLUDED_
#define _MProxy_H_INCLUDED_

#include <stdlib.h>
#ifdef WIN32
#pragma warning(disable:4786)
#include <windows.h>
#include <winsock.h>
#include <iostream>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <arpa/inet.h>
#endif
#include <map>
#include <stbapi/util/sockets.h>
#include <stbapi/util/multicast.h>
#include <stbapi/util/os.h>
#include <stbapi/workspace/distrManager/StbDistrManager.h>
#include <stbapi/workspace/ContextParameters.h>
#ifdef WIN32
using namespace std ;
#endif


struct McastInterface
{
	char multicastGroup[16];
	char portName[6];
    struct sockaddr_in sockAddr;
	int multicastSocket;
	int active;
};

class MProxy
{
    public:
		MProxy();
		~MProxy();
		void run(
			int argc,
			char **argv
			);

		enum MPROXY_CONNECTION_MODE
		{
			LISTENER,
			CONNECTOR
		};
	protected:
		int createRegistrationSocket(          // create and bind a registration socket
			int port                           // to this local port
            );

        void removeClient(                     // remove a client
			SockInterface *net                 // with this SockInterface
			);


        void processMessage(                   // process a message
			char *msg,                         // pointer to message data
			long msgLength                     // length of message data
			);

		void createNewMulticastGroup(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void deleteMulticastGroup(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void processMulticastData(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);
       void processReceivedMulticastData(
           McastInterface *mcastInterface,
		   char *buffer, 
		   int bufferSize
		   );

       void processSAGData(
		   char *msg, 
		   long msgLength
		   );

	   McastInterface* getMcastInterface(
		   char *mGroup,
		   char *mPort
		   );

	   void createSAG(     //create session annoncment group
		   );
		SockInterface *connectorInterface;
		int acceptedConnectorSock;
        // create a map <socket number, multicast interface>
		// for registered multicast groups
        std::map<int,McastInterface*> mcastSockets;
        typedef std::map<int,McastInterface*>::iterator mcastIterator;
        char multicastGroup[16];
	    char portName[6];
        int multicastSock;
        struct sockaddr_in server;         // Address information server
		int biggestSockNumber;
};

#endif
// eof

