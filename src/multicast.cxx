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

#include "multicast.h"

//---------------------------------------------------------------------------



void 
initNetwork()
{
#ifdef WIN32
   INT err;
   WSADATA WsaData;

   //
   // Initialize Windows Sockets and request version 1.1
   //
   err = WSAStartup (0x0101, &WsaData);
   if (err == SOCKET_ERROR)
   {
      fprintf (stderr, "WSAStartup() failed: %ld\n", GetLastError ());
      exit(-1);
   }    
#endif
}

//---------------------------------------------------------------------------

int 
createMulticastSocket(const char *multicastGroup, const char *portName,
                          char mode)
{
   int sock;
   int one = 1;
   struct sockaddr_in server;
   int port;

   // generate socket
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0)
   {
      perror("createMulticastSocket: socket creation failed");
      return(-1);
   }

   if(mode==MULTICAST)
   {
#ifdef _SGI_SOURCE
//*********************** SGI code
   // set socket options to REUSEPORT
   if( setsockopt( sock, SOL_SOCKET, SO_REUSEPORT,  &one, sizeof(one)) == -1 )
	     perror( "createMulticastSocket: can't set REUSEPORT" );
#else
  #ifndef WIN32
//*********************** LINUX code
   // set socket options to REUSEPORT
   if( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR,  &one, sizeof(one)) == -1 )
	     perror( "createMulticastSocket: can't set REUSEADDR" );
  #else 
//*********************** WIN32 code
  // set socket options to REUSEADDR !!
  // WIN32 port: winsock does not support REUSEPORT. It combines REUSEPORT and REUSEADDR (UNIX)
  //             in REUSEADDR

   if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one)) == SOCKET_ERROR)
		     perror( "createMulticastSocket: can't set REUSEADDR" );
  #endif
#endif
   }
   if((multicastGroup!=NULL)||(portName!=NULL))
   {
      struct in_addr *addr;
      addr = atoaddr(multicastGroup);
      if (addr == NULL)
      {
         fprintf(stderr,"createMulticastSocket: Invalid network address:"
                        " %s\n", multicastGroup);
         return(-1);
      }

      port = atoport(portName,"udp");
      if (port < 0)
      {
         fprintf(stderr,"createMulticastSocket: unable to find port %s.\n",
                        portName);
         return(-1);
      }

   struct sockaddr_in client;
   memset((char *) &client, 0, sizeof(client));
   client.sin_family = AF_INET;
   client.sin_addr.s_addr = htonl(INADDR_ANY);
//   client.sin_addr.s_addr = inet_addr(multicastGroup);//DS
   client.sin_port = port;
   // bind socket to multicast group & port
   if (bind(sock, (struct sockaddr *) &client, sizeof(client)) < 0)
   {
      perror("joinMulticastGroup: bind");
      return(-1);
   }

      /* Set up the server info */
      memset((char *) &server, 0, sizeof(server));
      server.sin_family = AF_INET;
      server.sin_addr.s_addr = addr->s_addr;
      server.sin_port = port;

      if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0)
      {
         perror("createMulticastSocket: connect failed");
         return(-1);
      }
   }

   return(sock);
}

//---------------------------------------------------------------------------

int 
closeMulticastSocket(int sock)
{
#ifdef WIN32
   return(closesocket(sock));
#else
   return(close(sock));
#endif
}

//---------------------------------------------------------------------------

int 
createMulticastGroup(const char *multicastGroup, const char *portName)
{
   int sock;
   struct sockaddr_in client, server;
   int port;
   unsigned char ttl=1;
   int one=1;
   struct in_addr *addr;

   // generate socket
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0)
   {
      perror("createMulticastGroup: socket creation failed");
      return(-1);
   }

#ifdef _SGI_SOURCE
//*********************** SGI code
   // set socket options to REUSEPORT
   if( setsockopt( sock, SOL_SOCKET, SO_REUSEPORT,  &one, sizeof(one)) == -1 )
	     perror( "createMulticastSocket: can't set REUSEPORT" );
#else
  #ifndef WIN32
//*********************** LINUX code
   // set socket options to REUSEPORT
   if( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR,  &one, sizeof(one)) == -1 )
	     perror( "createMulticastSocket: can't set REUSEADDR" );
  #else 
//*********************** WIN32 code
  // set socket options to REUSEADDR !!
  // WIN32 port: winsock does not support REUSEPORT. It combines REUSEPORT and REUSEADDR (UNIX)
  //             in REUSEADDR

   if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one)) == SOCKET_ERROR)
		     perror( "createMulticastSocket: can't set REUSEADDR" );
  #endif
#endif
   port = atoport(portName,"udp");
   if (port < 0)
   {
      fprintf(stderr,"createMulticastGroup: unable to find port %s.\n",
                      portName);
      return(-1);
   }
   addr = atoaddr(multicastGroup);
   if (addr == NULL)
   {
      fprintf(stderr,"createMulticastSocket: Invalid network address:"
                     " %s\n", multicastGroup);
      return(-1);
   }

   memset((char*)&client, 0, sizeof(client));
   client.sin_family=AF_INET;
   client.sin_addr.s_addr=htonl(INADDR_ANY);
   client.sin_port=port;

   // bind socket to multicast group & port
   if (bind(sock, (struct sockaddr *) &client, sizeof(client)) < 0)
   {
      perror("createMulticastGroup: bind");
      return(-1);
   }

   /* Set up the server info */
   memset((char *) &server, 0, sizeof(server));
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = addr->s_addr;
   server.sin_port = port;

   if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0)
   {
      perror("createMulticastSocket: connect failed");
      return(-1);
   }

   if(setsockopt(sock,IPPROTO_IP,IP_MULTICAST_TTL,(char*)&ttl,sizeof(ttl))==-1)
      perror("createMulticastGroup: set ttl value");

   return(sock);
}

//---------------------------------------------------------------------------

int 
joinMulticastGroup(const int sock, const char *multicastGroup, const char *portName)
{
   int port;
   char charBuf[80];
   struct hostent *host;
   struct ip_mreq imr;
   struct in_addr *inp, mc_address;

   port = atoport(portName,"udp");
   if (port < 0)
   {
      fprintf(stderr,"joinMulticastGroup: unable to find port %s.\n", portName);
      return(-1);
   }


   struct sockaddr_in client;
   memset((char *) &client, 0, sizeof(client));
   client.sin_family = AF_INET;
   client.sin_addr.s_addr = htonl(INADDR_ANY);
//   client.sin_addr.s_addr = inet_addr(multicastGroup);//DS
   client.sin_port = port;
   // bind socket to multicast group & port
   if (bind(sock, (struct sockaddr *) &client, sizeof(client)) < 0)
   {
      perror("joinMulticastGroup: bind");
      return(-1);
   }

   if (gethostname(charBuf, sizeof(charBuf))<0)
   {
      perror("joinMulticastGroup: can't get own hostname");
      return(-1);
   }

   if ((host=gethostbyname(charBuf))==NULL)
   {
      perror("joinMulticastGroup: can't get own internet address");
      return(-1);
   }

   memcpy(&mc_address, host->h_addr_list[0], sizeof(mc_address));
   printf("INFO: joinMulticastGroup: using interface %s\n", inet_ntoa(mc_address));
   
   if ((inp=atoaddr(multicastGroup))==NULL)
   {
      fprintf(stderr,"joinMulticastGroup: Invalid network address: %s\n", multicastGroup);
      return(-1);
   }

   imr.imr_multiaddr = *inp;
   imr.imr_interface = mc_address;
#ifndef WIN32
   if( setsockopt( sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
	               &imr, sizeof(struct ip_mreq) ) == -1 )
   {
      perror( "joinMulticastGroup: can't join group" );
	  return(-1);
   }
//		else printf( "joinMulticastGroup: multicast group %s joined\n", inet_ntoa(*inp));
#else
   INT err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   (char*)&imr, sizeof(imr));
   if(err)
   {
//      MessageBox( NULL, "joinMulticastGroup: can't join group! exiting...", "setsockopt error", MB_OK|MB_ICONINFORMATION );
      return(-1);
   }
#endif

   return(1);   
}

//---------------------------------------------------------------------------

int
leaveMulticastGroup(const int sock, const char *multicastGroup, const char *portName)
{
   int port;
   char charBuf[80];
   struct hostent *host;
   struct ip_mreq imr;
   struct in_addr *inp, mc_address;

   port = atoport(portName,"udp");
   if (port < 0)
   {
      fprintf(stderr,"leaveMulticastGroup: unable to find port %s.\n", portName);
      return(-1);
   }

   if (gethostname(charBuf, sizeof(charBuf))<0)
   {
      perror("leaveMulticastGroup: can't get own hostname");
      return(-1);
   }

   if ((host=gethostbyname(charBuf))==NULL)
   {
      perror("leaveMulticastGroup: can't get own internet address");
      return(-1);
   }

   memcpy(&mc_address, host->h_addr_list[0], sizeof(mc_address));
   
   if ((inp=atoaddr(multicastGroup))==NULL)
   {
      fprintf(stderr,"leaveMulticastGroup: Invalid network address: %s\n", multicastGroup);
      return(-1);
   }

   imr.imr_multiaddr = *inp;
   imr.imr_interface = mc_address;
#ifndef WIN32
   if( setsockopt( sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
	               &imr, sizeof(struct ip_mreq) ) == -1 )
   {
      perror( "leaveMulticastGroup: can't leave group" );
	  return(-1);
   }
//		else printf( "leaveMulticastGroup: left multicast group %s\n", inet_ntoa(*inp));
#else
   INT err = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                   (char*)&imr, sizeof(imr));
   if(err)
   {
//      MessageBox( NULL, "leaveMulticastGroup: can't leave group! exiting...", "setsockopt error", MB_OK|MB_ICONINFORMATION );
      return(-1);
   }
#endif

   return(1);      
}

//---------------------------------------------------------------------------

/* Take a service name, and a service type, and return a port number.  If the
   service name is not found, it tries it as a decimal number.  The number
   returned is byte ordered for the network. */

int 
atoport(const char *service, const char *proto)
{
  int port;
  long int lport;
  struct servent *serv;
  char *errpos;

  /* First try to read it from /etc/services */
  serv = getservbyname(service, proto);
  if (serv != NULL)
    port = serv->s_port;
  else { /* Not in services, maybe a number? */
    lport = strtol(service,&errpos,0);
    if ( (errpos[0] != 0) || (lport < 1) || (lport > 65535) )
      return -1; /* Invalid port address */
    port = htons((unsigned short)lport);
  }
  return port;
}

/* Converts ascii text to in_addr struct.  NULL is returned if the address
   can not be found. */
struct in_addr *atoaddr(const char *address)
{
  struct hostent *host;
  static struct in_addr saddr;

  /* First try it as aaa.bbb.ccc.ddd. */
  saddr.s_addr = inet_addr(address);
  if (saddr.s_addr != -1) {
    return &saddr;
  }
  host = gethostbyname(address);
  if (host != NULL) {
    return (struct in_addr *) *host->h_addr_list;
  }
  return NULL;
}

//---------------------------------------------------------------------------

int sendMulticastData(const int sock, const char *data, const int length)
{
  return (send(sock, data, length, 0));
}

//---------------------------------------------------------------------------

int receiveMulticastData(const int sock, char *data, const int length)
{
  return (recv(sock, data, length, 0));
}

//---------------------------------------------------------------------------
//eof
