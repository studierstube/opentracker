/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Sequencer.h,v 1.1 2000/10/11 00:57:08 coryan Exp

// ============================================================================
//
// = LIBRARY
//    ace/RMCast
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_SEQUENCER_H
#define ACE_RMCAST_SEQUENCER_H

#include "RMCast_Module.h"
#include <ace/Synch.h>

/**
 * Assign sequence numbers to outgoing messages.
 * On the sender side we must assign sequence numbers to the messages
 * <B>before</B> they are put in the retransmission queue.
 */
class STBAPI_API ACE_RMCast_Sequencer : public ACE_RMCast_Module
{
public:
  /// Constructor
  ACE_RMCast_Sequencer (void);

  /// Destructor
  virtual ~ACE_RMCast_Sequencer (void);

  virtual int data (ACE_RMCast::Data &);

  /// returns next sequence number
  unsigned int next_sequence_number()
  {
	  return sequence_number_generator_;
  }

protected:
  /// Create the sequence numbers
  ACE_UINT32 sequence_number_generator_;

  /// Synchronization
  ACE_SYNCH_MUTEX mutex_;
};

inline ACE_RMCast_Sequencer::ACE_RMCast_Sequencer (void)
  :  sequence_number_generator_ (1)
{
}
#endif /* ACE_RMCAST_SEQUENCER_H */
