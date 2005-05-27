/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Ack_Worker.h,v 1.1 2000/10/11 00:57:08 coryan Exp

#ifndef ACE_RMCAST_ACK_WORKER_H
#define ACE_RMCAST_ACK_WORKER_H

#include "RMCast_Retransmission.h"

/**
 * Process an Ack message in the ACE_RMCast_Retransmission module.
 * The retransmission module uses internal iterators, thus it needs to
 * create helper Worker classes to process some messages.
 */
class ACE_RMCast_Ack_Worker
  : public ACE_RMCast_Worker<ACE_UINT32,ACE_RMCast::Data>
{
public:
  ACE_RMCast_Ack_Worker (ACE_RMCast::Ack &ack,
                         ACE_RMCast_Retransmission::Messages::Write_Guard &g,
                         ACE_RMCast_Retransmission::Messages *messages);

  virtual int work (ACE_UINT32 const & key,
                    ACE_RMCast::Data const &item);

private:
  ACE_RMCast_Ack_Worker (const ACE_RMCast_Ack_Worker&);
  ACE_RMCast_Ack_Worker& operator= (const ACE_RMCast_Ack_Worker&);

private:
  ACE_RMCast::Ack &ack_;

  ACE_RMCast_Retransmission::Messages::Write_Guard &ace_mon_;

  ACE_RMCast_Retransmission::Messages *messages_;
};

inline ACE_RMCast_Ack_Worker::ACE_RMCast_Ack_Worker (ACE_RMCast::Ack &ack,
                       ACE_RMCast_Retransmission::Messages::Write_Guard &g,
                       ACE_RMCast_Retransmission::Messages *messages)
  : ack_ (ack)
  , ace_mon_ (g)
  , messages_ (messages)
{
}

#endif /* ACE_RMCAST_ACK_WORKER_H */
