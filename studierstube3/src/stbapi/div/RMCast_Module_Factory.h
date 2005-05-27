/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Module_Factory.h,v 1.3 2000/10/10 00:06:30 coryan Exp

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Module_Factory.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_MODULE_FACTORY_H
#define ACE_RMCAST_MODULE_FACTORY_H

#include "RMCast.h"

class ACE_RMCast_Module;
class ACE_RMCast_IO_UDP;

//! Create Module stacks
/*!
 * Different application will probably require different
 * configurations in their Module stack, some will just want best
 * effort semantics.  Others will use Reliable communication with a
 * maximum retransmission time.  Furthermore, applications may want to
 * receive messages in send order, or just as soon as they are
 * received.
 * Obviously most applications will want to change want happens once a
 * message is completely received.
 *
 * To achieve all this flexibility the IO layer uses this factory to
 * create the full stack of Modules corresponding to a single
 * consumer.
 * To keep the complexity under control the intention is to create
 * helper Factories, such as Reliable_Module_Factory where
 * applications only need to customize a few features.
 */
class STBAPI_API ACE_RMCast_Module_Factory
{
public:
  //! Destructor
  virtual ~ACE_RMCast_Module_Factory (void);

  //! Create a new proxy
  virtual ACE_RMCast_Module *create (void) = 0;

  //! Destroy a proxy
  /*!
   * Some factories may allocate modules from a pool, or return the
   * same module for all proxies.  Consequently, only the factory
   * knows how to destroy them.
   */
  virtual void destroy (ACE_RMCast_Module *) = 0;
};

#endif /* ACE_RMCAST_MODULE_FACTORY_H */
