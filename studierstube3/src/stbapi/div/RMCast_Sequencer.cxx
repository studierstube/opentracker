/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
//
// RMCast_Sequencer.cpp,v 1.1 2000/10/11 00:57:08 coryan Exp
//

#include "RMCast_Sequencer.h"
#include <ace/Message_Block.h>

ACE_RMCast_Sequencer::~ACE_RMCast_Sequencer (void)
{
}

int ACE_RMCast_Sequencer::data (ACE_RMCast::Data &data)
{
    {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
        data.sequence_number = this->sequence_number_generator_++;
        data.total_size = data.payload->total_length ();
        data.fragment_offset = 0;
    }
    return this->ACE_RMCast_Module::data (data);
}
