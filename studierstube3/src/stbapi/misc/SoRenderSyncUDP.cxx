// ===========================================================================
//  (C) 2003 Vienna University of Technology
// ===========================================================================
//  NAME:       SoRenderSyncUDP.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    class for synchronizing path traveral in Open Inventor
//  VERSION:    1.0
// ===========================================================================
//  AUTHORS:    ge             Gottfried Eibner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

// maximum elapsing time until a blocking reveive returns unsuccessfully
#define SYNC_TIMEOUT_USEC 1000000

#ifdef _DEBUG
    // suppress server and client error output
    //#define SYNC_NODES_SILENT
#endif

#ifdef SYNC_NODES_SILENT
    #define serverOut(_x)
    #define clientOut(_x)
#else
    #define serverOut(_x) cerr << outClassName << "server:" << _x
    #define clientOut(_x) cerr << outClassName << "client:" << _x
    #define paco "<client " << packet.clientNumber << ":frame "<< packet.frameNumber << ">"
    static char const * const outClassName = "SoRenderSyncUDP ";
#endif

#include "UDPServerClient.h"
#include "SoRenderSyncUDP.h"

#include <iostream>
using namespace std;

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/SbLinear.h>
#include <Inventor/fields/SoFieldContainer.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


SO_NODE_SOURCE(SoRenderSyncUDP);

void
SoRenderSyncUDP::initClass(void)
{
    SO_NODE_INIT_CLASS(SoRenderSyncUDP, SoRenderSync, "SoRenderSync");
}

SoRenderSyncUDP::SoRenderSyncUDP(void) :
    server(NULL), sender(NULL), client(NULL), receiver(NULL)
{
    SO_NODE_CONSTRUCTOR(SoRenderSyncUDP);

//-- default values of fields
    SO_NODE_ADD_FIELD(multicastAddress,("224.0.0.1"));
    SO_NODE_ADD_FIELD(multicastPort   ,(2001));

    SO_NODE_ADD_FIELD(interfaceAddress,("127.0.0.1"));

    SO_NODE_ADD_FIELD(serverAddress   ,("127.0.0.1"));
    SO_NODE_ADD_FIELD(serverPort      ,(2010));

//-- set up sensors
    multicastAddrSensor = new SoFieldSensor(serverAndClientChangedCB, (void *)this);
    multicastPortSensor = new SoFieldSensor(serverAndClientChangedCB, (void *)this);
    serverAddrSensor    = new SoFieldSensor(serverAndClientChangedCB, (void *)this);
    serverPortSensor    = new SoFieldSensor(serverAndClientChangedCB, (void *)this);

    multicastAddrSensor->attach(&multicastAddress);
    multicastPortSensor->attach(&multicastPort);
    serverAddrSensor   ->attach(&serverAddress);
    serverPortSensor   ->attach(&serverPort);
}

SoRenderSyncUDP::~SoRenderSyncUDP(void)
{
    if (client  ) { delete client  ; }
    if (sender  ) { delete sender  ; }
    if (server  ) { delete server  ; }
    if (receiver) { delete receiver; }
}

void
SoRenderSyncUDP::setupServerAndClients(void)
{
    if (client  ) { delete client  ; client   = NULL; }
    if (sender  ) { delete sender  ; sender   = NULL; }
    if (server  ) { delete server  ; server   = NULL; }
    if (receiver) { delete receiver; receiver = NULL; }

    if (-1 == clientNum.getValue())
    {
        server = new UDPServer(serverPort.getValue());
        sender = new MulticastSender(multicastAddress.getValue().getString(),multicastPort.getValue());
        serverOut("\nnew server created " << serverAddress.getValue().getString() << ":" << serverPort.getValue() << endl);
    }
    client   = new UDPClient(serverAddress.getValue().getString(),serverPort.getValue(),interfaceAddress.getValue().getString());

    receiver = new MulticastReceiver(multicastAddress.getValue().getString(),multicastPort.getValue());
    clientOut("new client joined  " << multicastAddress.getValue().getString() << ":" << multicastPort.getValue() << endl);

    SoRenderSync::setupServerAndClients();
}

void
SoRenderSyncUDP::serverAndClientChangedCB(void *data, SoSensor *)
{
    ((SoRenderSyncUDP *)data)->hasChanged = true;
}

void
SoRenderSyncUDP::GLRender(SoGLRenderAction *action)
{
    // @todo refine processing of changed states
    if (hasChanged)
    {
        setupServerAndClients();
        hasChanged = false;
    }
    // node renders asynchronous (server's client node only).
    bool serverClientAsynchron = (
        -1 == clientNum.getValue() &&   // server's node rendering
        !synchronized.getValue() );     // asynchronous rendering choosen

    Packet packet;
    int srb;//, count;
    bool gotnotification = false;

    // setup everything for rendering
    while (!gotnotification)
    {
        packet.frameNumber  = frameNumber;
        packet.clientNumber = (unsigned short)(clientNum.getValue()+1);
        packet.retransmit   = 0;

	    // write out UDP-packet with client identification and requested frame number
        // clientOut("client -> <" << packet.clientNumber << "," << packet.frameNumber << "," << packet.retransmit << ">\n");
        srb = client->send((char*)&packet,sizeof(packet));
        if(srb<0) { clientOut("client " << packet.clientNumber  << ": send error:" << srb << endl); }

        if (serverClientAsynchron)
        {
            // non blocking receiving
            // just wait 1 msec for servers reply (non blocking).
            // immediately switch to next frame.
            receiver->recv((char *)&packet,sizeof(packet),1000);

            packet.frameNumber = frameNumber;   // ignore sync frame number
            gotnotification = true;
        }
        else
        {
            // blocking receive, if server does not respond within SYNC_TIMEOUT_USEC
            // the client resets its actual frame number and tries to send its number
            // to the server. (this is important in conjunction with div, since some
            // packages might be lost...)
           if ((srb = receiver->recv((char *)&packet,sizeof(packet),SYNC_TIMEOUT_USEC)) < 0)
            {
                frameNumber = 1;
                clientOut("server lost\n");
            }

            int diff = packet.frameNumber - frameNumber;
            if (diff)
            {
                clientOut("recv differing packet " << paco <<
                    "\n\tserver frame number "<< packet.frameNumber <<
                    "\n\tclient frame number "<< frameNumber << endl);

                // try to catch up, if we are not just starting off
                if (diff > 0)
                {
                    if (frameNumber > 1)
                    {
                        clientOut("missed some sync-packets ... try to catch up\n");
                    }
                    gotnotification = true;
                }
            }

            if (packet.retransmit == 0)
            {
                gotnotification = true;
            } //otherwise loop continues sending request packet

            if (!gotnotification)
            {
                clientOut("rendering too fast, waiting for other clients to catch up " << frameNumber << endl);
            }
        }
    }
    // set next frame number to be requested
    frameNumber = packet.frameNumber+1;

    SoCacheElement::invalidate(action->getState());
}


/** Method which contains server semantics. */
void SoRenderSyncUDP::serverCode(void)
{
    serverOut("server thread starting up\n");
    static int numOfClients = 1;
    static ClientStatistic *clients = new ClientStatistic[1];
    memset(clients,0,sizeof(ClientStatistic));

    Packet packet;
    unsigned long syncFrameNumber = 1;
    int i, srb;     // i, SendReceivedBytes
    int start;      // start index of first synchronized client (0 or 1).
    clock_t oldtick = 0, newtick = 0;

    // if server's client is synchronized set start to 0, else to 1.
    if (synchronized.getValue()) { start = 0; }
    else  { start = 1; }

    while (true)
    {
        // no notification from any client within a given time leads to an error recovery.
       if ((srb = server->recv((char*)&packet,sizeof(packet),SYNC_TIMEOUT_USEC)) > 0)
        {
            do
            {
                // if client number out of range expand client statistic array.
                if (packet.clientNumber >= numOfClients)
                {
                    int num = packet.clientNumber+1;
                    ClientStatistic *erase = clients;
                    clients = new ClientStatistic[num];

                    memset(clients,0,num*sizeof(ClientStatistic));
                    memcpy(clients,erase,numOfClients*sizeof(ClientStatistic));

                    delete [] erase;

                    numOfClients = num;
                }
                // set client's statistic.
                clients[packet.clientNumber].frameNumber = packet.frameNumber;

                // new client is joining synchronization server
                if (!clients[packet.clientNumber].active)
                {
                    clients[packet.clientNumber].joinedAtFrame = syncFrameNumber;
                    clients[packet.clientNumber].frameNumber   = syncFrameNumber;
                    clients[packet.clientNumber].active = true;
                    serverOut("server received :: " << paco << endl);
                    serverOut("new client " << packet.clientNumber << " joined at " << syncFrameNumber << endl);
                }

                // test if all clients have reached the syncFrameNumber
                newtick = clock();
                clock_t difftick = newtick-oldtick;
                bool ready = true;
                for (i = start; i < numOfClients; i++)
                {
                    if (clients[i].active && clients[i].frameNumber != syncFrameNumber)
                    {
                        ready = false;
                        if (difftick > 5000)
                        {
                            clients[i].active = false;
                            serverOut("client " << i << " did not respond within 5 seconds. client killed\n");
                        }
                    }
                }

                // 
                if (ready)
                {
                    packet.frameNumber  = syncFrameNumber;  // actual sync frame
                    packet.clientNumber = (unsigned short)-1;               // message from server
                    packet.retransmit   = 0;
                    if ((srb = sender->send((char *)&packet,sizeof(packet))) < 0)
                    {
                        serverOut("sync-packet send error: " << srb << endl);
                    }
                    // in the case we have a asynchronous server, start will be one.
                    // if numOfClients is also one, we do not increment the
                    // synch frame number (only the server's asynchronous client is active).
                    if (start != numOfClients)
                    {
                        //ACE_OS::sleep(1);   //@TESTING ONLY
                        syncFrameNumber++;
                        oldtick = clock();
                    }
                }
            }
            while ((srb = server->recv((char*)&packet,sizeof(packet),1000)) > 0);
            // empty server's message queue.

        }
        else
        {
            serverOut("timeout occurred -> retransmitting frame request " << syncFrameNumber <<  "..." << endl);
            packet.frameNumber  = syncFrameNumber;   // new sync frame number for all clients
            packet.clientNumber = (unsigned short)-1;        // message from server
            packet.retransmit   =  1;        // retransmit demanded

            // reset client statistic
            for (i=0; i<numOfClients; i++)
            {
                clients[i].active = 0;
            }

            // send multicast to clients requesting retransmit of request
            if ((srb = sender->send((char *)&packet,sizeof(packet))) < 0)
            {
                serverOut("retransmit-request-packet send error: " << srb << endl);
            }
        }
    } // while(1)

    delete [] clients;
}
