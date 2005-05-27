/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
// RMCast_Role_Fork.cpp,v 1.1 2000/10/11 00:57:08 coryan Exp

#include "RMCast_Role_Fork.h"

ACE_RMCast_Role_Fork::~ACE_RMCast_Role_Fork (void)
{
}

int
ACE_RMCast_Role_Fork::open (void)
{
    int r = this->ACE_RMCast_Module::open ();
    if (r != 0)
        return r;
    if (this->receiver () != 0)
        return this->receiver ()->open ();
    return 0;
}

int
ACE_RMCast_Role_Fork::close (void)
{
    int r = this->ACE_RMCast_Module::close ();
    if (r != 0)
        return r;
    if (this->receiver () != 0)
        return this->receiver ()->close ();
    return 0;
}

int
ACE_RMCast_Role_Fork::data (ACE_RMCast::Data &data)
{
    if (this->receiver () != 0)
        return this->receiver ()->data (data);
    return 0;
}

int
ACE_RMCast_Role_Fork::poll (ACE_RMCast::Poll &poll)
{
    if (this->receiver () != 0)
        return this->receiver ()->poll (poll);
    return 0;
}

int
ACE_RMCast_Role_Fork::ack_join (ACE_RMCast::Ack_Join &ack_join)
{
    if (this->receiver () != 0)
        return this->receiver ()->ack_join (ack_join);
    return 0;
}

int
ACE_RMCast_Role_Fork::ack_leave (ACE_RMCast::Ack_Leave &ack_leave)
{
    if (this->receiver () != 0)
        return this->receiver ()->ack_leave (ack_leave);
    return 0;
}

ACE_RMCast_Role_Fork::ACE_RMCast_Role_Fork (void)
: ACE_RMCast_Module ()
, receiver_ (0)
{
}

void
ACE_RMCast_Role_Fork::receiver (ACE_RMCast_Module *module)
{
    this->receiver_ = module;
}


ACE_INLINE ACE_RMCast_Module *
ACE_RMCast_Role_Fork::receiver (void) const
{
    return this->receiver_;
}
