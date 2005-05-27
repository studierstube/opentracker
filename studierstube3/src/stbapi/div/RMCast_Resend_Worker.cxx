/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
//
// RMCast_Resend_Worker.cpp,v 1.1 2000/10/11 00:57:08 coryan Exp
//

#include "RMCast_Resend_Worker.h"
#include "RMCast_Module.h"

int ACE_RMCast_Resend_Worker::work (ACE_UINT32 const & key, ACE_RMCast::Data const &item)
{
    if (key >= this->max_sequence_number_)
        return 0;
    
    ACE_RMCast::Data data = item;
    int r = this->next_->data (data);
    if (r != 0)
        return r;
    this->n++;
    
    return 0; // @@ Stop after the first message...
}
