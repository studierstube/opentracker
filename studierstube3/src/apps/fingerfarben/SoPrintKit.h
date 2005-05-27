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
 /** 
  * Defines the class SoPrintKit, a subclass of SoDropTarget to print geometry.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoPrintKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOPRINTKIT_H_
#define _SOPRINTKIT_H_

#include <Inventor/SoPath.h>

#include "fingerfarben.h"
#include "SoDropTarget.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
Subclass of SoDropTarget to print geometry. Any content of a SoDropItem that is moved inside
the bounds of this node is printed (from the perspective of the users camera).

See documentation of SoDropTarget for documentation of file format and parameters.

@ingroup fingerfarben
*/
class FINGERFARBEN_API SoPrintKit : public SoDropTarget 
{
    SO_KIT_HEADER(SoPrintKit);

public:
    /// Default constructor.
    SoPrintKit();

    /// Static method to initialize class for OpenInventor.
    static void initClass();	

    /**
    This method is called if an action on an SoDropTarget is triggered within
    the bounds of this node.
    @see SoDropTarget
    @see SoDropItem
    */
    virtual bool handleItem(SoFullPath* pathToItem);

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOPRINTKIT_H_
