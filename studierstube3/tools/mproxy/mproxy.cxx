/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       MProxy
    TYPE:       c++ code
    PROJECT:    Studierstube Tools
    CONTENT:    Multicast proxy
    VERSION:    2.0
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
                gh      Gerd Hesina
   ===========================================================================
    HISTORY:

    22-Feb-01 14:00:00  gh      last modification (cleanup, restructuring)
    14-Dec-00 14:35:48  jp      Linux port
    12-Dec-90 10:54:36  gh      created
=========================================================================== */

#include "mproxy.h"
#include <assert.h>
#include <iostream>

MProxy::MProxy()
:
acceptedConnectorSock(-1),
connectorInterface(NULL),
multicastSock(-1),
biggestSockNumber(-1)
{
    memset((char*) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(0);
}

MProxy::~MProxy()
{
    // nil
}

void MProxy::createNewMulticastGroup(SockInterface *net, CBuffer *buffer, int32 messageType)
{
	McastInterface *newInterface = new McastInterface;
	newInterface->active = FALSE;
	strcpy(newInterface->multicastGroup,buffer->getString());
	strcpy(newInterface->portName,buffer->getString());
    newInterface->multicastSocket = 
		createMulticastGroup(newInterface->multicastGroup, 
		                     newInterface->portName);
    if(newInterface->multicastSocket<0)
	{
        fprintf(stderr,"cannot create multicastgroup at address: %s, port: "
                  "%s\n",newInterface->multicastGroup, newInterface->portName);
        exit(-1);
	}
    fprintf(stderr,"create new multicastgroup at address: %s, port: "
                  "%s\n",newInterface->multicastGroup, newInterface->portName);
    mcastSockets[newInterface->multicastSocket] =  newInterface; // put a new mcast iface into the map
}

void MProxy::deleteMulticastGroup(SockInterface *net, CBuffer *buffer, int32 messageType)
{
}

McastInterface* MProxy::getMcastInterface(char *mGroup, char *mPort)
{
	mcastIterator mCastIterator;
	McastInterface *mcastInterface = NULL;
    for( mCastIterator=mcastSockets.begin();
	     mCastIterator !=mcastSockets.end(); mCastIterator++ )        
	{
        mcastInterface = mCastIterator->second;
	    if ((strcmp(mcastInterface->multicastGroup, mGroup) == 0) &&
            (strcmp(mcastInterface->portName, mPort) == 0) )
			break;
	}
	return mcastInterface;
}

void MProxy::processMulticastData(SockInterface *net, CBuffer *buffer, int32 messageType)
{
	void *sendBuf;
	ulong sendBufLen;
	char *mGroup = buffer->getString();
	char *mPort = buffer->getString();
	buffer->getRemainingBytes(sendBuf, sendBufLen);
	McastInterface *mcastInterface = getMcastInterface(mGroup, mPort);
	if(!mcastInterface)
		fprintf(stderr,"mcastInterface for group %s and port %s not found!\n",
		                mGroup, mPort);
	sendMulticastData(mcastInterface->multicastSocket, (char*)sendBuf, sendBufLen);
}

void MProxy::processReceivedMulticastData(McastInterface *mcastInterface, char *buffer, int bufferSize)
{
    connectorInterface->initOutput();
	connectorInterface->addInt32ToOutput(MPROXY_MULTICAST_DATA);
    connectorInterface->addStringToOutput(mcastInterface->multicastGroup);
    connectorInterface->addStringToOutput(mcastInterface->portName);
	connectorInterface->addBytesToOutput(bufferSize, buffer);
    if(!connectorInterface->sendOutput())
	{
	    delete connectorInterface;
		connectorInterface = NULL;
		acceptedConnectorSock = -1;
	}
}

void MProxy::processSAGData(char *msg, long msgLength)
{
    if(msgLength==0) return;
    CBuffer buffer((void*)msg, msgLength, CBuffer::CBUFFER_STATIC);
    int32 messageType=buffer.getInt32();

	switch(messageType)
	{
	    case MPROXY_SAG_NEW_GROUP:
		{
            char *mGroup = buffer.getString();
			char *mPort = buffer.getString();
            connectorInterface->initOutput();
	        connectorInterface->addInt32ToOutput(MPROXY_CREATE_MULTICAST_GROUP);
	        connectorInterface->addStringToOutput(mGroup);
	        connectorInterface->addStringToOutput(mPort);
            if(!connectorInterface->sendOutput())
			{
	            delete connectorInterface;
		        connectorInterface = NULL;
		        acceptedConnectorSock = -1;
			}
            McastInterface *newInterface = new McastInterface;
			newInterface->active = TRUE;
	        strcpy(newInterface->multicastGroup,mGroup);
	        strcpy(newInterface->portName,mPort);
	        newInterface->multicastSocket = 
				createMulticastSocket(NULL,NULL);

            if(newInterface->multicastSocket<0)
			{
                fprintf(stderr,"cannot create multicastgroup at address: %s, port: "
                      "%s\n",newInterface->multicastGroup, newInterface->portName);
                exit(-1);
			}
			
            if(joinMulticastGroup(newInterface->multicastSocket, 
		                          newInterface->multicastGroup, 
		                          newInterface->portName)<0)
			{
                fprintf(stderr,"can't join multicast group...exiting!\n");
                exit(-1);
			}
			// put a new mcast iface into the map
            mcastSockets[newInterface->multicastSocket] =  newInterface; 
            if(biggestSockNumber < newInterface->multicastSocket)
			    biggestSockNumber = newInterface->multicastSocket;
			break;
		}
	    case MPROXY_SAG_DELETE_GROUP:
		{
            char *mGroup = buffer.getString();
			char *mPort = buffer.getString();
            connectorInterface->initOutput();
	        connectorInterface->addInt32ToOutput(MPROXY_DELETE_MULTICAST_GROUP);
	        connectorInterface->addStringToOutput(mGroup);
	        connectorInterface->addStringToOutput(mPort);
            if(!connectorInterface->sendOutput())
			{
	            delete connectorInterface;
		        connectorInterface = NULL;
		        acceptedConnectorSock = -1;
			}
			break;
		}
		default:
            fprintf(stderr,"MProxy: unknown SAG message received (%d)\n",
               messageType);
	}
}

void MProxy::processMessage(char *msg, long msgLength)
{
    if(msgLength==0) return;
    CBuffer buffer((void*)msg, msgLength, CBuffer::CBUFFER_STATIC);
    SockInterface *net=connectorInterface;
    int32 messageType=buffer.getInt32();

    switch(messageType)
    {
	    case MPROXY_CREATE_MULTICAST_GROUP:
        {
            fprintf(stderr,
				"MProxy: MPROXY_CREATE_MULTICAST_GROUP received\n");
            createNewMulticastGroup(net, &buffer, messageType);
			break;
		}
        case MPROXY_DELETE_MULTICAST_GROUP:
        {
            fprintf(stderr,
				"MProxy: MPROXY_DELETE_MULTICAST_GROUP received\n");
            deleteMulticastGroup(net, &buffer, messageType);
			break;
		}
        case MPROXY_MULTICAST_DATA:
        {
            processMulticastData(net, &buffer, messageType);
			break;
		}

        default:
            fprintf(stderr,"MProxy: unknown message received (%d)\n",
               messageType);
    }    
}

void MProxy::createSAG() // SAG=Session Announcment Group
{
    strcpy(multicastGroup, "224.100.200.220");
    strcpy(portName, "12843");

    multicastSock = createMulticastSocket(NULL,NULL,MULTICAST);
    if(joinMulticastGroup(multicastSock, multicastGroup, portName) <0)
	{ 
        perror("MProxy: can't join SAG group");
        exit(-1);
	}
}


int MProxy::createRegistrationSocket(int port)
{
    // Create a registration socket for new clients
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // bind it to a port on the local host
    struct sockaddr_in srvr_addr = {
                AF_INET,
                htons(port),
                INADDR_ANY};

    int on=1;
#ifdef WIN32
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
#else
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
#endif

    if( bind(sock, (const struct sockaddr *)&srvr_addr, sizeof(srvr_addr)) < 0 )
    {
        perror("MProxy: bind");
        exit(-1);
    }

    // make it passive
    if( listen(sock, 5) < 0 )
    {
        perror("MProxy: listen");
        exit(-1);
    }
	return sock;
}

 
#define MPROXY_USAGE fprintf(stderr,"usage: mproxy [listen port] | [-connect address:connection_port]\n");

void MProxy::run(int argc, char **argv)
{
    using namespace std;
    
	// default values for port
    int port = 9000;
    int mode = MProxy::LISTENER;
    char *connectorAddress = NULL;
	static char buffer[65536];
    // command line: check port
    if(argc == 1)
	{
        MPROXY_USAGE
        fprintf(stderr,"using default port %d for connections\n", port);
	}
    else if(argc == 3)
	{
        int okFlag = FALSE;
		if(strcmp(argv[1],"-connect")==0)
		{
            connectorAddress=strdup(argv[2]);
			char *cPort=strchr(connectorAddress,':');
            if(cPort)
			{
				*cPort++='\0';
				okFlag = TRUE;
				port = atoi(cPort);
				mode = MProxy::CONNECTOR;
			}
		}
		if(!okFlag)
		{
			MPROXY_USAGE
		    exit(EXIT_FAILURE);
		}
	}
	else if(argc == 2)
	{
        port = atoi(argv[1]);
        fprintf(stderr,"using port %d for client connections\n", port);
	}
	else
	{
		MPROXY_USAGE
		exit(EXIT_FAILURE);
	}

#ifdef WIN32
    initNetwork();
#endif
    int sock = -1;
	if(mode == MProxy::LISTENER)
	{
        sock = createRegistrationSocket(port);
	    biggestSockNumber = sock;
		createSAG();
		if(biggestSockNumber < multicastSock)
			biggestSockNumber = multicastSock;
    }
	else
	{
		if(!connectorAddress)
		{
			fprintf(stderr,"No connection Address specified!\n");
            MPROXY_USAGE
		    exit(EXIT_FAILURE);
		}
        connectorInterface=new SockInterface;
        // open connection
        if(connectorInterface->openConnection(connectorAddress, port))
	    cerr<<"INFO: MProxy @"<<connectorAddress<<":"<<
		                      port<<" contacted"<<endl;
        else
		{
		    cerr<<"ERROR: could not establish connection to "
		           <<connectorAddress<<":"<<
		           port<<endl;
            exit(EXIT_FAILURE);
		}
        connectorInterface->setVerbose(0);
		acceptedConnectorSock = connectorInterface->getSocket();
		biggestSockNumber = acceptedConnectorSock;
		createSAG();
		if(biggestSockNumber < multicastSock)
			biggestSockNumber = multicastSock;
	}
    // main loop
    while(1)
    {
        // listen to incoming messages
        // init select: all sockets
        fd_set readfds;
		mcastIterator mCastIterator;
        FD_ZERO(&readfds);
        if(sock>0) FD_SET(sock, &readfds);
		if(multicastSock>0) FD_SET(multicastSock, &readfds);
        if(acceptedConnectorSock>0)
            FD_SET(acceptedConnectorSock, &readfds);
	    for( mCastIterator=mcastSockets.begin();
		     mCastIterator != mcastSockets.end(); ++mCastIterator )
		{
			McastInterface *mcastInterface = mCastIterator->second;
            if(mcastInterface->active) FD_SET(mCastIterator->first, &readfds);
		}
        // check if some sockets have been hit
        int s = select(biggestSockNumber+1,&readfds, NULL, NULL, 0);
	    if( s < 0 )
	        perror("MProxy: select");


            for( mCastIterator=mcastSockets.begin();
			     mCastIterator !=mcastSockets.end(); mCastIterator++ )        
			{
	            if ( FD_ISSET(mCastIterator->first, &readfds))
				{
	                McastInterface* mcastInterface = mCastIterator->second;
					int receivedBytes = -1;
                    if((receivedBytes=receiveMulticastData(mCastIterator->first, buffer, 
                                 sizeof(buffer)))<0)
					{
                        perror("MProxy: receiveMulticast");
	                    exit(-1);
					}
                    processReceivedMulticastData(mcastInterface, buffer, receivedBytes);
					FD_CLR(mCastIterator->first, &readfds);
				} //if(FD_ISSET)
			} //for





        // connector registration socket hit ?
        if ((sock > 0) && (FD_ISSET(sock, &readfds)))
        {
            struct sockaddr_in addr;
#ifdef _LINUX_SOURCE
            socklen_t len;
#else
            int len;
#endif
            int s;

            len = sizeof(addr);

            // accept new connector
            if( (s = accept(sock, (struct sockaddr *)&addr, &len)) < 0 )
            {
	            fprintf(stderr, "MProxy: monitor accept returned %d", s);
		        perror("MProxy: accept");
		        continue;
			}

	        // prepare to accept connection
            fprintf(stderr, "MProxy: Accepting CONNECTOR connection from %s:%d\n",
                    inet_ntoa(addr.sin_addr),
                    ntohs(addr.sin_port));

			if(connectorInterface)
			{
				char buf[1024];
                sprintf(buf,"closing this connection. reason: current active CONNECTOR connection from %s:%d\n",
                   inet_ntoa(connectorInterface->getAddress()->sin_addr),
                   ntohs(connectorInterface->getAddress()->sin_port));
				SockInterface *tempInterface = new SockInterface;
				tempInterface->bind(s, &addr);
				tempInterface->setVerbose(0);
//				tempInterface->initOutput();
//				tempInterface->addBytesToOutput(strlen(buf), buf);
//				tempInterface->addStringToOutput("\r");
//				tempInterface->sendAsciiOutput();
                delete tempInterface;
				fprintf(stderr,"%s", buf);
                FD_CLR(sock, &readfds);
			}
            else
			{
                // create new socket interface and bind it 
                connectorInterface = new SockInterface;  // store connector interface
                connectorInterface->bind(s, &addr);     // bind the soket
                connectorInterface->setVerbose(0);      // no debug

                // biggest sock number is required by function select
                if( biggestSockNumber < s ) biggestSockNumber = s;
	            FD_CLR(sock, &readfds);
			    acceptedConnectorSock = s;

			}
        }
        if ((multicastSock > 0) && (FD_ISSET(multicastSock, &readfds)))
		{
		   int bufferSize = sizeof(buffer);
           int structlength = sizeof(sockaddr_in);
		   int bytes = receiveMulticastData(multicastSock, buffer, bufferSize);
           if(bytes<0) 
           {
               perror("MProxy: receive");
               exit(-1);
           }
		   processSAGData(buffer,bytes);
		}

        if ((acceptedConnectorSock > 0) && (FD_ISSET(acceptedConnectorSock, &readfds)))
		{
            long msgLength=0;
            char* msg = connectorInterface->getMessage(0, &msgLength);
			if( ((int)msg) == SockInterface::SOCKET_CLOSED)
			{
                cerr<<"MProxy: Partner disappeared"<<endl;
				exit(EXIT_FAILURE);
			}
            else
			{
                // process message
	            processMessage(msg, msgLength);
                delete msg;
                FD_CLR(acceptedConnectorSock, &readfds);
			} 
		} //if(FD_ISSET)
    } //while
}

/* ===========================================================================
   End of MProxy.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   eval: (setq indent-tabs-mode nil)
   End:
=========================================================================== */
