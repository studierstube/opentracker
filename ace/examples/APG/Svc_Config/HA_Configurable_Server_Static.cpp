// HA_Configurable_Server_Static.cpp,v 1.2 2004/06/10 22:43:04 shuston Exp

// Listing 1 code/ch19
#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/Reactor.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_STATIC_SVC_REGISTER (HA_Status_Descriptor);
  ACE_Service_Config::open (argc,
                            argv,
                            ACE_DEFAULT_LOGGER_KEY,
                            0);
  ACE_Reactor::instance ()->run_reactor_event_loop ();
  return 0;
}
// Listing 1
