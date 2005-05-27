#ifndef _SMAN_H_INCLUDED_
#define _SMAN_H_INCLUDED_

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
#include <list>
#include <stbapi/util/sockets.h>
#include <stbapi/util/multicast.h>
#include <stbapi/util/os.h>
#include <stbapi/workspace/distrManager/StbDistrManager.h>
#include <stbapi/workspace/ContextParameters.h>
#include "cprocess.h"
#ifdef WIN32
using namespace std ;
#endif

class Sman
{
    public:
		Sman();
		~Sman();
		void run(
			int argc,
			char **argv
			);

	protected:
		int createRegistrationSocket(          // create and bind a registration socket
			int port                           // to this local port
            );

        void removeClient(                     // remove a client
			SockInterface *net                 // with this SockInterface
			);

        void loadBalance(                      // do load balancing
			SockInterface *net
			);

		void setNewAddresses(                  // set new div specific addresses
			ContextParameters *buffer,
			SockInterface *master
			);

		void advanceAddresses(                 // advance div specific addresses
			);

		void migrate(                          // migrate a context
			SockInterface *source,             // from source
			SockInterface *destination,        // to destination
			ContextParameters *ctxParams       // using ctxParams
			);

        void processMessage(                   // process a message
			char *msg,                         // pointer to message data
			long msgLength,                    // length of message data
			int s                              // receive socket
			);

		void monitorUsage(                     // send help text to a monitor
			);

        void processMonitorMessage(            // process a monitor message
			char *msg,                         // pointer to message data
			long msgLength                     // length of message data
			);

        SockInterface* getClientWithNumber(
			int clientNumber
			);

		CProcess* getMasterProcessWithNumber(
			SockInterface *net,
			int masterProcessNumber
			);

		void createSlaveContext(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void slaveModeSet(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void masterModeSet(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void slaveContextMigrated(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void slaveContextCreated(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void slaveContextDeleted(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void masterContextDeleted(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void loadApplication(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void systemBarChanged(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void setFocus(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void queryApps(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void contextStreamed(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void slaveContextStreamed(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void getMasterContext(
			SockInterface *net,
			CBuffer *buffer,
			int32 messageType
			);

		void broadcastMessage(
			void *sendBuf,
			ulong sendBufLen,
			SockInterface *net,
			int32 messageType
			);

		void getOwnAddress(
			);

		void resetAddresses(
			);

	/// create a map <socket number, socket interface> for registered clients
        std::map<int,SockInterface*> clientSockets;
	/// map from the SockInterface objects to lists of master processes
	std::map<SockInterface*, std::list<CProcess*> > masterProcesses;
	/// map from the SockInterface objects to lists of slave processes
	std::map<SockInterface*, std::list<CProcess*> > slaveProcesses;

	// some iterator typedefs
        typedef std::map<int,SockInterface*>::iterator clientIterator;
        typedef std::list<CProcess*>::iterator processIterator;

		SockInterface *monitorInterface;
		int acceptedMonitorSock;
        int divAddress;
        int divPort;
        int serverPort;
		double lastLoadApplicationTime;
		int lastLoadApplicationUserId;
		char lastLoadApplicationName[512];
		SystemBarMessage lastSystemBar;
		FocusSetMessage lastSetFocus;
		double lastSystemBarChangedTime;
		double lastSetFocusTime;
		char *ownAddress;
};

#endif
// eof

