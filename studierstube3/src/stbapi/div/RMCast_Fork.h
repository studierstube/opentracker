/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Fork.h,v 1.1 2000/10/11 00:57:08 coryan Exp

#ifndef ACE_RMCAST_FORK_H
#define ACE_RMCAST_FORK_H

#include "RMCast_Module.h"

/// Fork messages to multiple destinations
/**
 * In some instances the messages must be sent to multiple
 * destinations, this module is a generic component to duplicate such
 * messages.
 */
class STBAPI_API ACE_RMCast_Fork : public ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Fork (void);

  //! Destructor
  virtual ~ACE_RMCast_Fork (void);

  /// Set the control module, all incoming control messages go to it
  void secondary (ACE_RMCast_Module *module);

  /// Return the current control module
  ACE_RMCast_Module *secondary (void) const;

  virtual int open (void);
  virtual int close (void);
  virtual int data (ACE_RMCast::Data &);
  virtual int poll (ACE_RMCast::Poll &);
  virtual int ack_join (ACE_RMCast::Ack_Join &);
  virtual int ack_leave (ACE_RMCast::Ack_Leave &);
  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);
  virtual int leave (ACE_RMCast::Leave &);

private:
  /// The control module
  ACE_RMCast_Module *secondary_;
};

inline ACE_RMCast_Fork::ACE_RMCast_Fork (void)
  : ACE_RMCast_Module ()
  , secondary_ (0)
{
}

inline void ACE_RMCast_Fork::secondary (ACE_RMCast_Module *module)
{
  this->secondary_ = module;
}

inline ACE_RMCast_Module * ACE_RMCast_Fork::secondary (void) const
{
  return this->secondary_;
}

#endif /* ACE_RMCAST_FORK_H */
