/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Resend_Worker.h,v 1.1 2000/10/11 00:57:08 coryan Exp

#ifndef ACE_RMCAST_RESEND_WORKER_H
#define ACE_RMCAST_RESEND_WORKER_H

#include "RMCast.h"
#include "RMCast_Copy_On_Write.h"

class ACE_RMCast_Module;

/**
* Process an Ack message in the ACE_RMCast_Retransmission module.
* The retransmission module uses internal iterators, thus it needs to
* create helper Worker classes to process some messages.
*/
class ACE_RMCast_Resend_Worker : public ACE_RMCast_Worker<ACE_UINT32,ACE_RMCast::Data>
{
public:
    ACE_RMCast_Resend_Worker (ACE_RMCast_Module *next, ACE_UINT32 max_sequence_number);
    
    virtual int work (ACE_UINT32 const & key, ACE_RMCast::Data const &item);
    
    int n;
    
private:
    ACE_RMCast_Module *next_;
    
    ACE_UINT32 max_sequence_number_;
    
private:
    ACE_RMCast_Resend_Worker (const ACE_RMCast_Resend_Worker&);
    ACE_RMCast_Resend_Worker& operator= (const ACE_RMCast_Resend_Worker&);
};

inline ACE_RMCast_Resend_Worker::ACE_RMCast_Resend_Worker (ACE_RMCast_Module *next,
                                                    ACE_UINT32 max_sequence_number)
                                                    : n (0)
                                                    , next_ (next)
                                                    , max_sequence_number_ (max_sequence_number)
{
}

#endif /* ACE_RMCAST_RESEND_WORKER_H */
