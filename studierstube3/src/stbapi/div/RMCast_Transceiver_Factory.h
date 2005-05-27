/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */

#ifndef ACE_RMCAST_TRANSCEIVER_FACTORY_H
#define ACE_RMCAST_TRANSCEIVER_FACTORY_H

#include "RMCast_Module_Factory.h"

class STBAPI_API ACE_RMCast_Transceiver_Factory : public ACE_RMCast_Module_Factory
{
public:
  /// Constructor
  ACE_RMCast_Transceiver_Factory (ACE_RMCast_Module_Factory *sender_factory, 
	  ACE_RMCast_Module_Factory *user_factory);

  //! Destructor
  virtual ~ACE_RMCast_Transceiver_Factory (void);

  virtual ACE_RMCast_Module *create (void);
  virtual void destroy (ACE_RMCast_Module *);

private:
  /// Delegate on another factory to create the sender module
  ACE_RMCast_Module_Factory *sender_factory_;
  /// Delegate on another factory to create the user module
  ACE_RMCast_Module_Factory *user_factory_;
};

#endif /* ACE_RMCAST_TRANSCEIVER_FACTORY_H */
