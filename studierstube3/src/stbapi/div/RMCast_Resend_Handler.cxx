/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
// RMCast_Resend_Handler.cpp,v 1.2 2000/10/11 00:57:08 coryan Exp

#include "RMCast_Resend_Handler.h"
#include "RMCast_Retransmission.h"

ACE_RMCast_Resend_Handler::~ACE_RMCast_Resend_Handler (void)
{
}

int
ACE_RMCast_Resend_Handler::handle_timeout (const ACE_Time_Value &,
                                           const void *)
{
     ACE_RMCast::Poll poll;
	(void) this->retransmission_->poll( poll );
  return 0;
}
