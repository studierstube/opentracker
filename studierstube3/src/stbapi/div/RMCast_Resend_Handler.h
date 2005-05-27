/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
// RMCast_Resend_Handler.h,v 1.1 2000/10/10 00:06:30 coryan Exp

#ifndef ACE_RMCAST_RESEND_HANDLER_H
#define ACE_RMCAST_RESEND_HANDLER_H

#include <ace/Event_Handler.h>
#include <stbapi/stbapi.h>

class ACE_RMCast_Retransmission;

/**Implement an adapter to resend messages in the
 * ACE_RMCast_Retransmission layer, but based on Reactor based
 * timeouts.*/
class STBAPI_API ACE_RMCast_Resend_Handler : public ACE_Event_Handler
{
public:
    /// Constructor, save io_udp as the Adaptee in the Adapter pattern.
    ACE_RMCast_Resend_Handler (ACE_RMCast_Retransmission *retransmission);
    
    /// Destructor
    ~ACE_RMCast_Resend_Handler (void);
    
    //@{
    /// Documented in ACE_Event_Handler class
    virtual int handle_timeout (const ACE_Time_Value &current_time,
        const void *act = 0);
    //@}
    
private:
    /// The adaptee
    ACE_RMCast_Retransmission *retransmission_;
};

inline ACE_RMCast_Resend_Handler::ACE_RMCast_Resend_Handler (ACE_RMCast_Retransmission *r)
:  retransmission_ (r)
{
}

#endif /* ACE_RMCAST_RESEND_HANDLER_H */
