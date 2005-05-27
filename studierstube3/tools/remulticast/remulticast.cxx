/* ===========================================================================
    Copyright (c) 1996,1997,1998,1999,2000
    Vienna University of Technology
    
    This software is provided AS IS, without any express or implied
    warranty.  In no event will the authors or the Vienna University of
    Technology be held liable for any damages or loss of profit arising
    from the use or non-fitness for a particular purpose of this software.
   ===========================================================================
    NAME:       remulticast
    TYPE:       c code
    PROJECT:    Studierstube - General unicast to multicast forwader
    CONTENT:    Unicast to multicast forwarder code
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    22-Jan-01 23:45:57  jp      last modification
    25-Aug-00 14:39:56  jp      created
=========================================================================== */

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "util/multicast.h"
 
#define BUFFER_SIZE	4096

static struct sockaddr_in Server;

/* 
   TODO: Look at the socket.cxx in src/stbapi/util, this function will
   be probably defined there ... 
*/

int ReadData(
	int            sock,
	char         * buffer,
	unsigned int * bytes
	)
{
    int             structlength = sizeof(sockaddr_in);
    fd_set          rfds;
    struct timeval  tv;
    int             retval;

    /* Read data from socket */

    do 
    {
        /* Watch socket to see when it has input */

        FD_ZERO(&rfds);
        FD_SET(sock, &rfds);

        /* Wait up to 1 second */

        tv.tv_sec  = 1;
        tv.tv_usec = 0;

        retval = select(sock+1, &rfds, NULL, NULL, &tv);

        if (retval>0)
        {
	    /* Message ready */
#ifdef _LINUX_SOURCE
	    *bytes = recvfrom(sock, buffer, BUFFER_SIZE, 0,
			      (struct sockaddr *)&Server, 
			      (socklen_t *)&structlength);
#else
#ifdef _SGI_SOURCE
	    *bytes = recvfrom(sock, buffer, BUFFER_SIZE, 0,
			      (struct sockaddr *)&Server, 
			      &structlength);
#else
	    *bytes = recvfrom(sock, (char FAR*)buffer, BUFFER_SIZE, 0, 
			      (struct sockaddr FAR*)&Server,
			      &structlength);
#endif
#endif
	    if (*bytes < 0) 
	    {
		perror("ReadData() - recvfrom");
		exit(-1);
	    }
        }
        else 
#if defined(_SGI_SOURCE) || defined (_LINUX_SOURCE)
        if (retval == -1)
        {
            if (errno != EINTR)
	    {
#else
        if (retval == SOCKET_ERROR) 
        { 
            int err = WSAGetLastError();
            if (err != EINTR) 
            {
#endif
                perror("ReadData() - select");
                exit(-1);
            }
        }
    } while (retval <= 0);

    return retval;
}

   
int main(
	unsigned int    argc,
	char         ** argv
	)
{
    char          buffer[BUFFER_SIZE];
    unsigned int  bytes;
    int           sockIn;
    int           sockOut;
    
    if (argc != 5)
    {
	printf("Usage: %s ucast_in port_in mcast_out port_out\n", argv[0]);
	exit(1);
    }

#ifdef WIN32
    initNetwork();
#endif

    /* UDP socket for reading the data from Ultratrack */

    sockIn = createMulticastSocket(argv[1], argv[2], UNICAST);
    if (sockIn < 0)
    {
       perror("Can't create unicast socket");
       exit(-1);
    }

    printf("Listening to UDP unicast from %s:%s\n", argv[1], argv[2]);

    /* Multicast group that the data will be sent to */

    sockOut = createMulticastGroup(argv[3], argv[4]);
    if (sockOut < 0)
    {
	perror("Can't create multicast group");
	exit(-1);
    }
    
    printf("Sending to mluticast group %s:%s\n", argv[3], argv[4]);

    memset((char*) &Server, 0, sizeof(Server));
    Server.sin_family = AF_INET;
    Server.sin_addr.s_addr = htonl(INADDR_ANY);
    Server.sin_port = htons(0);

    /* Loop forever */

    while(1)
    {
        if (ReadData(sockIn, buffer, &bytes))
        {
	    sendMulticastData(sockOut, buffer, bytes);
	}
    }

}

/* ===========================================================================
    End of remulticast.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    End:
=========================================================================== */
