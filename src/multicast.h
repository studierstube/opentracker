// ===========================================================================
//  (C) 1999 Vienna University of Technology
// ===========================================================================
//  NAME:       multicast
//  TYPE:       c++ code
//  PROJECT:    Studierstube: Zero Problem Tools
//  CONTENT:    wrappers for multicasting stuff
//  VERSION:    1.2
// ===========================================================================
//  AUTHORS:    gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 16:59:18  gh      last modification
// ===========================================================================

#ifndef _MULTICAST_H_
#define _MULTICAST_H_

extern "C" {

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

enum castMode {
     MULTICAST,
     UNICAST
};

//initNetwork: needs to be called in a win32 environment BEFORE
//the first access to a socket function is made.
void initNetwork();

//createMulticastSocket: creates a socket and connects it to the
//specified address and port. If multicastGroup and/or portName is NULL then
//the socket is created but not connected. If mode is UNICAST then no
//additional socket options are set. (default: multicast)
//return: <0: error, socket otherwise
int createMulticastSocket(const char *multicastGroup, const char *portName,
              char mode=MULTICAST);

//closeMulticastSocket: closes the specified socket (unicast or multicast)
//return: 0: success, <0: error
int closeMulticastSocket(int sock);

//createMulticastGroup: creates a multicastGroup (only useful as a sender)
//return: <0: error, socket otherwise
int createMulticastGroup(const char *multicastGroup, const char *portName);

//joinMulticastGroup: (guess...)
//return: TRUE: success, -1 otherwise
int joinMulticastGroup(const int sock, const char *multicastGroup, const char *portName);

//joinMulticastGroup: (guess...)
//return: TRUE: success, -1 otherwise
int leaveMulticastGroup(const int sock, const char *multicastGroup, const char *portName);

/* Take a service name, and a service type, and return a port number.  If the
   service name is not found, it tries it as a decimal number.  The number
   returned is byte ordered for the network. */
int atoport(const char *service, const char *proto);

/* Converts ascii text to in_addr struct.  NULL is returned if the address
   can not be found. */
struct in_addr *atoaddr(const char *address);

//sendMulticastData: send data via the specified socket. multicast or unicast
int sendMulticastData(const int sock, const char *data, const int length);

//receiveMulticastData: receive data via the specified socket.
//multicast or unicast
int receiveMulticastData(const int sock, char *data, const int length);

} 

#endif
//eof
