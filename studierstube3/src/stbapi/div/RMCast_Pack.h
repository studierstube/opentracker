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
/** header for ACE_RMCast_Pack data packing module
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: RMCast_Pack.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef ACE_RMCAST_PACK_H
#define ACE_RMCAST_PACK_H

#include "RMCast_Module.h"

/**
 * This module packs a data message inside another data message. This is
 * used to have the fragment modules output be sequenced and subjected to
 * retransmission, instead of only being able to use it after the sequencing
 */
class STBAPI_API ACE_RMCast_Pack : public ACE_RMCast_Module  
{
public:
    ACE_RMCast_Pack();
    virtual ~ACE_RMCast_Pack();

    //! Push data through the stack
    virtual int data (ACE_RMCast::Data &);

protected:
  /// Create the sequence numbers
  ACE_UINT32 pack_number_generator_; 
};

#endif // ACE_RMCAST_PACK_H
