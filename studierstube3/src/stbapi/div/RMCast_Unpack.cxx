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
 * $Id: RMCast_Unpack.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#define ACE_NLOGGING

#include "RMCast_Unpack.h"
#include <ace/Message_Block.h>

ACE_RMCast_Unpack::ACE_RMCast_Unpack()
: pack_number_generator_(0)
{

}

ACE_RMCast_Unpack::~ACE_RMCast_Unpack()
{

}

int ACE_RMCast_Unpack::data (ACE_RMCast::Data & data)
{
    if( data.fragment_offset == 0  )
    {
        pack_number_generator_++;        
    }
    data.sequence_number = pack_number_generator_;
    return ACE_RMCast_Module::data(data);
}
