/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: stbapi
 * ======================================================================== */
/**
 * A class collection to create UDP server and client and multicast sender
 * and receiver instances via ACE (Adaptive Communication Environment).
 * The following classes are only wrapper classes to create an unified
 * interface to send and receive data.
 *
 * class UDPServer
 *  usage:
 *     UDPServer server(serverPort);
 *     server.recv(data,size);
 *
 * class UDPClient
 *  usage :
 *     UDPClient client(serverName,serverPort);
 *     client.send(data,size);
 *
 * class MulticastReceiver
 *  usage :
 *     MulticastReceiver m(mcastAddr,mcastport);
 *     m.recv(data,size);
 *
 * class MulticastSender
 *   usage:
 *      MulticastSender m(mcastAddr,mcastport);
 *      m.send(data,size)
 *
 *
 * @author Gottfried Eibner
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef _UDP_SERVER_CLIENT_
#define _UDP_SERVER_CLIENT_

#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Synch.h"

// --- udp server using ACE ---
// UDPServer class. Provides a server which receives UDP messages from its clients.
class UDPServer
{
  public:
    // Constructor sets up a server to receive UDP messages.
    // @param port - number of port to listen to
    UDPServer(int port) : localAddr(port),dgram(localAddr) { }

    // Receives data with given size.
    // @param data - pointer to data block
    // @param size - size of data block in bytes
    // @param timeout_usec - max. time to wait for data (micro seconds).
    int recv(char *data, int size, int timeout_usec = 20000)
    {
        ACE_Time_Value tv(0,timeout_usec);
        return dgram.recv(data,size,clientAddr,0,&tv);
    }

  private:
    ACE_INET_Addr clientAddr;
    ACE_INET_Addr localAddr;
    ACE_SOCK_Dgram dgram;
};


// UDPClient class. Provides a client which sends UDP messages to its server.
class UDPClient
{
  public:
    // Constructor sets up a client to send UDP messages.
    // @param host - name or address of server host
    // @param port - number of server port
    UDPClient(const char *host, int port, const char *interfaceAddr=NULL) :
        serverAddr(port,host)
    {
        if (interfaceAddr && strcmp("",interfaceAddr))
            { localAddr = ACE_INET_Addr(interfaceAddr); }
        else
            { localAddr = ACE_INET_Addr((u_short)0); }
        dgram = ACE_SOCK_Dgram(localAddr);
    }

    // Sends data to given server.
    // @param data - pointer to data block
    // @param size - size of data in bytes
    int send(const char *data,int size)
    {
        return dgram.send(data,size,serverAddr);
    }

  private:
    ACE_INET_Addr serverAddr;
    ACE_INET_Addr localAddr;
    ACE_SOCK_Dgram dgram;
};


// MulticastReceiver class. Provides a multicast receveiver which receives multicast
// messages from any sender. An arbitrary number of receivers can join the
// same group (given by same host and port).
class MulticastReceiver
{
  public:
    // Constructor joins multicast group.
    // @param host - name or address of multicast group host
    // @param port - number of multicast group port
    MulticastReceiver(const char *host, int port) : mcastAddr(port,host),localAddr((u_short)0)
    {
        // Subscribe to multicast address.
        assert(mcdgram.subscribe (mcastAddr) == -1);
    }

    // Destructor leaves multicast group.
    ~MulticastReceiver()
    {
        if(mcdgram.unsubscribe()==-1)
        {
            ACE_DEBUG((LM_ERROR,"Error in unsubscribing from Multicast group\n"));
        }
    }

    // Receives data that was sent to its multicast group by a multicast sender.
    // @param data - pointer to data block
    // @param size - size of data block in bytes
    int recv(char *data, int size)
    {
        //get ready to receive data from the sender.
        return mcdgram.recv(data,size,localAddr);
    }

    // Receives data that was sent to its multicast group by a multicast sender.
    // @param data - pointer to data block
    // @param size - size of data block in bytes
    // @param timeout_usec - max. time to wait for data (micro seconds).
    int recv(char *data, int size, int timeout_usec)
    {
        //get ready to receive data from the sender.
        ACE_Time_Value tv(0,timeout_usec);
        return mcdgram.recv(data,size,localAddr,0,&tv);
    }

  private:
    ACE_INET_Addr mcastAddr;
    ACE_INET_Addr localAddr;
    ACE_SOCK_Dgram_Mcast mcdgram;
};


// MulticastSender class. Provides a multicast sender which sends multicast
// messages to given multicast group.
class MulticastSender
{
  public:
    // Constructor sets up a multicast sender which can send messages to given
    // mulicast group.
    // @param host - name or address of multicast group host
    // @param port - number of multicast group port
    MulticastSender(const char *host, int port)
       : mcastAddr(port,host),localAddr((u_short)0),dgram(localAddr) { }

    // Sends data to its multicast group.
    // @param data - pointer to data block
    // @param size - size of data block in bytes
    int send(const char *data, int size)
    {
        return dgram.send (data,size,mcastAddr);
    }

  private:
    ACE_INET_Addr mcastAddr;
    ACE_INET_Addr localAddr;
    ACE_SOCK_Dgram dgram;
};

#endif //_UDP_SERVER_CLIENT_
