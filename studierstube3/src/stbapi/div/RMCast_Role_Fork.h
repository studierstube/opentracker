/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Fork.h,v 1.1 2000/10/11 00:57:08 coryan Exp

#ifndef ACE_RMCAST_ROLE_FORK_H
#define ACE_RMCAST_ROLE_FORK_H

#include "RMCast_Module.h"

/**
 * Fork messages to multiple destinations according to role of destination.
 * In some instances the messages must be sent to multiple
 * destinations, where the messages are routed depending on the type of receiver.
 */
class STBAPI_API ACE_RMCast_Role_Fork : public ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Role_Fork (void);

  //! Destructor
  virtual ~ACE_RMCast_Role_Fork (void);

  /// Set the control module, all incoming control messages go to it
  void receiver (ACE_RMCast_Module *module);

  /// Return the current control module
  ACE_RMCast_Module *receiver (void) const;

  virtual int open (void);
  virtual int close (void);
  virtual int data (ACE_RMCast::Data &);
  virtual int poll (ACE_RMCast::Poll &);
  virtual int ack_join (ACE_RMCast::Ack_Join &);
  virtual int ack_leave (ACE_RMCast::Ack_Leave &);

private:
  /// The receiver module
  ACE_RMCast_Module *receiver_;
};

#endif /* ACE_RMCAST_FORK_H */
