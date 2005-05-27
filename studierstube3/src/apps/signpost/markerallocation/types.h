/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
/** Marker Allocator
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: types.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef __types_h__
#define __types_h__

#include <string>
#include "bauml.h"

struct markerinfo
{
    int num;
//    std::string room;
    ARRoom *room;
    ARRoom::ARMarker *marker;
    bool assigned;

    bool operator < (const markerinfo &mi) const
    {
        return (marker->number < mi.marker->number);
    }
};

struct usedmarkerinfo
{
    int num;
    std::string name;
    bool used;
    int count;

    bool operator == (const usedmarkerinfo &umi) const
    {
        return (name == umi.name);
    }

};

#endif