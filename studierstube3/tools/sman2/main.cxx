#include <ace/OS.h>
#include <ace/Get_Opt.h>

#ifdef WIN32
#pragma warning(disable:4284)
#endif

#include "SessionManager.h"

int main( int argc, char **argv )
{
    // default values for port and monitorPort
    ACE_INET_Addr sessionAddress(1234);
    ACE_INET_Addr monitorAddress(1235);

    // options to parse (see help text below)
    ACE_Get_Opt options(argc, argv, "i:p:m:n:");

    int option = EOF;
    while((option = options()) != EOF)
    {
        switch(option) {
        case 'i':
            sessionAddress.set(sessionAddress.get_port_number(), options.opt_arg());
        	break;
        case 'p': 
            {
                int port = ACE_OS::atoi(options.opt_arg());
                sessionAddress.set_port_number(port);
            }            
            break;
        case 'm':
            monitorAddress.set(monitorAddress.get_port_number(),options.opt_arg());
        	break;
        case 'n': 
            {
                int mport = ACE_OS::atoi(options.opt_arg());
                monitorAddress.set_port_number(mport);
            }        	
            break;
        default:
    		ACE_DEBUG((LM_ERROR,"Usage : sman2 [-i ipaddress] [-p port] [-m monitoraddress ] [-n monitor port]\n"
                                "where default values are localhost:1234 and localhost:1235 for the monitor port\n"));
            exit(1);
        }
    }

    char buffer1[1024], buffer2[1024];
    sessionAddress.addr_to_string( buffer1, 1024 );
    monitorAddress.addr_to_string( buffer2, 1024 );
    ACE_DEBUG((LM_INFO, "SessionManager2 listening on %s and monitoring on %s\n",
               buffer1, buffer2));

    SessionManager sman;
    sman.open( sessionAddress, monitorAddress );
    sman.run();

    return 0;
}
