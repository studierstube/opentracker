/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
// RMCast_Reliable_Factory.cpp,v 1.2 2000/10/11 00:57:08 coryan Exp

#include "RMCast_Transceiver_Factory.h"
#include "RMCast_Role_Fork.h"
#include "RMCast_Receiver_Module.h"
#include "RMCast_Reassembly.h"
#include "RMCast_Reordering.h"
#include "RMCast_Unpack.h"

ACE_RMCast_Transceiver_Factory::
ACE_RMCast_Transceiver_Factory (ACE_RMCast_Module_Factory *sender_factory, 
                                ACE_RMCast_Module_Factory *user_factory)
                                : sender_factory_ (sender_factory), user_factory_ (user_factory)
{
}

ACE_RMCast_Transceiver_Factory::~ACE_RMCast_Transceiver_Factory (void)
{
}

ACE_RMCast_Module*
ACE_RMCast_Transceiver_Factory::create (void)
{
    ACE_RMCast_Role_Fork *fork;
    ACE_NEW_RETURN (fork, ACE_RMCast_Role_Fork, 0);
    
    ACE_RMCast_Module *receiver;
    ACE_NEW_RETURN (receiver, ACE_RMCast_Receiver_Module, 0);
    
    ACE_RMCast_Module *reassembly;
    ACE_NEW_RETURN (reassembly, ACE_RMCast_Reassembly, 0);
    
    ACE_RMCast_Module *reordering;
    ACE_NEW_RETURN (reordering, ACE_RMCast_Reordering, 0);
    
    ACE_RMCast_Module *unpack;
    ACE_NEW_RETURN( unpack, ACE_RMCast_Unpack, 0);

    ACE_RMCast_Module *sender = this->sender_factory_->create();  
    ACE_RMCast_Module *user = this->user_factory_->create ();
    
    if (user == 0 || sender == 0)
    {
        delete fork;
        delete receiver;
        delete reordering;
        delete reassembly;
        delete unpack;
        return 0;
    }
    fork->next (sender);
    fork->receiver (receiver);
    receiver->next (reordering);
    reordering->next (unpack);
    unpack->next (reassembly);
    reassembly->next (user);
    return fork;
}

void
ACE_RMCast_Transceiver_Factory::destroy (ACE_RMCast_Module *module)
{
    ACE_RMCast_Role_Fork *fork = (ACE_RMCast_Role_Fork *)module;
    ACE_RMCast_Module *sender = fork->next ();
    ACE_RMCast_Module *receiver = fork->receiver ();
    ACE_RMCast_Module *reordering = receiver->next ();
    ACE_RMCast_Module *unpack = reordering->next();
    ACE_RMCast_Module *reassembly = unpack->next ();
    ACE_RMCast_Module *user = reassembly->next ();
    this->user_factory_->destroy (user);
    this->sender_factory_->destroy (sender);  
    delete reordering;
    delete unpack;
    delete reassembly;
    delete receiver;
    delete fork;
}
