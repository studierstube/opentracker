/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// 

#ifndef ACE_RMCAST_UDP_RELIABLE_TRANSCEIVER_H
#define ACE_RMCAST_UDP_RELIABLE_TRANSCEIVER_H

#include "RMCast_Singleton_Factory.h"
#include "RMCast_IO_UDP.h"
#include "RMCast_Sequencer.h"
#include "RMCast_Retransmission.h"
#include "RMCast_Membership.h"
#include "RMCast_Fragment.h"
#include "RMCast_Fork.h"
#include "RMCast_Transceiver_Factory.h"
#include "RMCast_Resend_Handler.h"
#include "RMCast_Pack.h"

class ACE_Reactor;
class ACE_RMCast_UDP_Event_Handler;

class STBAPI_API ACE_RMCast_UDP_Reliable_Transceiver : public ACE_RMCast_Module
{
public:
  /// Constructor
  ACE_RMCast_UDP_Reliable_Transceiver (ACE_RMCast_Module *user_control);

  /// Destructor
  virtual ~ACE_RMCast_UDP_Reliable_Transceiver (void);

  /// Open the UDP I/O module, by using specified nic
  int init (const ACE_INET_Addr &mcast_group, const ACE_TCHAR *net_if = NULL);

  /// Close the module.
  virtual int close (void);

  /// Use the reactor to handle incoming messages
  void reactive_incoming_messages (ACE_Reactor *reactor);

  /// Use the reactor to periodically resend messages
  void reactive_resends (ACE_Reactor *reactor,
                         const ACE_Time_Value &period);

  /// Check if there is still some messages to send, return 0 if not.
  int has_data (void);

  /// Check if there are any members still connected
  int has_members (void);

  /// returns number of blocks still in retransmission
  int count_blocks(void)
  {
      return retransmission_.count_blocks();
  }

  /// returns next sequence number
  unsigned int next_sequence_number()
  {
	  return sequencer_.next_sequence_number();
  }

public:
  /// The application-level control module
  ACE_RMCast_Module *user_control_;

  /// Assign sequence numbers
  ACE_RMCast_Sequencer sequencer_;

  /// The retransmission module
  ACE_RMCast_Retransmission retransmission_;

  /// All the proxys give their messages to the membership module
  ACE_RMCast_Singleton_Factory membership_factory_;

   /// and All the proxys give their messages to the user module
  ACE_RMCast_Singleton_Factory user_factory_;

  /// Handle all the UDP I/O
  ACE_RMCast_IO_UDP io_udp_;

  /// The membership module
  ACE_RMCast_Membership membership_;

  /// The fragmentation module
  ACE_RMCast_Fragment fragment_;

  /// Redirect control messages to the user supplied module
  ACE_RMCast_Fork fork_;

  /// pack messages
  ACE_RMCast_Pack pack_;

  /// This factory creates the per-proxy stack
  ACE_RMCast_Transceiver_Factory transceiver_factory_;

  /// timer handler for polling requests
  ACE_RMCast_Resend_Handler resend_handler_;

  /// handles input from the udp socket
  ACE_RMCast_UDP_Event_Handler * udpHandler_;

  /// handles input from the multicast socket
  ACE_RMCast_UDP_Event_Handler * mcHandler_;
};

#endif /* ACE_RMCAST_UDP_RELIABLE_TRANSCEIVER_H */
