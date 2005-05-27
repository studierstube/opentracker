/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Reliable_Factory.h,v 1.1 2000/10/10 00:06:30 coryan Exp

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Reliable_Factory.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_RELIABLE_FACTORY_H
#define ACE_RMCAST_RELIABLE_FACTORY_H

#include "RMCast_Module_Factory.h"

/// Implement an ACE_RMCast_Module_Factory that "creates" a single
/// object.
/**
 * Many applications (and even some internal components), will use a
 * single ACE_RMCast_Module to process all the events, for example, a
 * receiver may decide to use the same ACE_RMCast_Module to process
 * all incoming events, instead of using one per remote sender.
 */
class STBAPI_API ACE_RMCast_Reliable_Factory : public ACE_RMCast_Module_Factory
{
public:
  /// Constructor
  /**
   * The create() method will return always.
   * \param factory <?>
   */
  ACE_RMCast_Reliable_Factory (ACE_RMCast_Module_Factory *factory);

  //! Destructor
  virtual ~ACE_RMCast_Reliable_Factory (void);

  virtual ACE_RMCast_Module *create (void);
  virtual void destroy (ACE_RMCast_Module *);

private:
  /// Delegate on another factory to create the user module
  ACE_RMCast_Module_Factory *factory_;
};

inline ACE_RMCast_Reliable_Factory::
    ACE_RMCast_Reliable_Factory (ACE_RMCast_Module_Factory *factory)
      : factory_ (factory)
{
}

#endif /* ACE_RMCAST_RELIABLE_FACTORY_H */
