/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
//
// RMCast_UDP_Event_Handler.cpp,v 1.3 2000/10/10 00:06:30 coryan Exp
//

#include "RMCast_UDP_Event_Handler.h"
#include "RMCast_IO_UDP.h"

ACE_RMCast_UDP_Event_Handler::~ACE_RMCast_UDP_Event_Handler (void)
{
}

ACE_HANDLE ACE_RMCast_UDP_Event_Handler::get_handle (void) const
{
    if( udp_ == true )
        return this->io_udp_->get_handle_udp ();
    else
        return this->io_udp_->get_handle_mcast ();
}

int ACE_RMCast_UDP_Event_Handler::handle_input (ACE_HANDLE h)
{
    return this->io_udp_->handle_input (h);
}

int ACE_RMCast_UDP_Event_Handler::handle_timeout (const ACE_Time_Value &,
                                              const void *)
{
    // @@ return this->io_udp_->handle_timeout ();
    return 0;
}
