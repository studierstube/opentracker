/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */

#include "RMCast_UDP_Reliable_Transceiver.h"
#include "RMCast_UDP_Event_Handler.h"
#include "RMCast_Resend_Handler.h"
#include <ace/Reactor.h>

ACE_RMCast_UDP_Reliable_Transceiver::ACE_RMCast_UDP_Reliable_Transceiver (ACE_RMCast_Module *user_control)
: user_control_ (user_control)

// We use a singleton factory, all proxys send their messages to the
// retransmission module.  There should be only control messages
// coming back, so this is OK.
, membership_factory_ (&membership_)
, user_factory_(user_control)
, io_udp_ (&transceiver_factory_)
, transceiver_factory_ (&membership_factory_, &user_factory_)
, resend_handler_(&retransmission_)
, udpHandler_(NULL)
, mcHandler_(NULL)
{
    // Control messages are received by the membership module and passed
    // up to the both the retransmission and user modules, we use a fork
    // module to do that
    this->membership_.next (&this->fork_);
    
    this->fork_.next (&this->retransmission_);
    this->fork_.secondary (user_control);
    
    // Messages are passed down to the sequencer module
    this->next (&this->sequencer_);
    
    // then they are fragmented
    this->sequencer_.next (&this->fragment_);

    // packed into new data blocks
    this->fragment_.next (&this->pack_);
    
    // numbered again
    this->pack_.next (&this->retransmission_);
    
    // and finally are send
    this->retransmission_.next (&this->io_udp_);
}

ACE_RMCast_UDP_Reliable_Transceiver::~ACE_RMCast_UDP_Reliable_Transceiver (void)
{
}

void
ACE_RMCast_UDP_Reliable_Transceiver::reactive_incoming_messages (ACE_Reactor *reactor)
{
    ACE_NEW (udpHandler_, ACE_RMCast_UDP_Event_Handler (&this->io_udp_, false));    
    (void) reactor->register_handler (udpHandler_, ACE_Event_Handler::READ_MASK);
    
    ACE_NEW (mcHandler_, ACE_RMCast_UDP_Event_Handler (&this->io_udp_, true));  
    (void) reactor->register_handler (mcHandler_, ACE_Event_Handler::READ_MASK);
}

void
ACE_RMCast_UDP_Reliable_Transceiver::reactive_resends (ACE_Reactor *reactor,
                                                       const ACE_Time_Value &period)
{  
    reactor->cancel_timer( &resend_handler_ );
    (void) reactor->schedule_timer (&resend_handler_, 0, period, period);
}

int
ACE_RMCast_UDP_Reliable_Transceiver::init (const ACE_INET_Addr &mcast_group, 
										   const ACE_TCHAR *net_if)
{
    return this->io_udp_.init (mcast_group, ACE_Addr::sap_any, net_if);
}

int
ACE_RMCast_UDP_Reliable_Transceiver::has_data (void)
{
    return this->retransmission_.has_data ();
}

int
ACE_RMCast_UDP_Reliable_Transceiver::has_members (void)
{
    return this->membership_.has_members ();
}

int
ACE_RMCast_UDP_Reliable_Transceiver::close (void)
{
    if( resend_handler_.reactor() != NULL )
        resend_handler_.reactor()->cancel_timer( &resend_handler_ );
    if( udpHandler_ != NULL )
        udpHandler_->reactor()->remove_handler( udpHandler_, ACE_Event_Handler::READ_MASK);
    if( mcHandler_ != NULL )
        mcHandler_->reactor()->remove_handler( mcHandler_, ACE_Event_Handler::READ_MASK);
    membership_.close();
    return ACE_RMCast_Module::close();
}
