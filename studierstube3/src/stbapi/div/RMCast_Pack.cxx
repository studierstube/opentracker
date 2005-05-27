/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** 
 * pack module for multicast 
 *
 * @author Gerhard Reitmayr
 *
 * $Id: RMCast_Pack.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#define ACE_NLOGGING

#include "RMCast_Pack.h"
#include <ace/Message_Block.h>
#include <ace/Log_Msg.h>

ACE_RMCast_Pack::ACE_RMCast_Pack() :
pack_number_generator_(1)
{

}

ACE_RMCast_Pack::~ACE_RMCast_Pack()
{

}

int ACE_RMCast_Pack::data (ACE_RMCast::Data & data)
{   
    // to avoid any side effects
    ACE_UINT32 help = data.sequence_number;
    // different implementation just uses a message counter :)...
    ACE_DEBUG((LM_DEBUG,"Pack changed number from %u to %u\n",data.sequence_number, pack_number_generator_ ));
    data.sequence_number = pack_number_generator_;
    pack_number_generator_++;        
    int r = ACE_RMCast_Module::data( data );
    data.sequence_number = help;
    return r;
}
