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
  * Defines SoDropItem, a simple wrapper for geometry on which actions of a SoDropTarget can
  * be applied.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoDropItem.h 3426 2004-07-14 12:11:27Z tamer $
  * @file
  */
 /* ======================================================================= */

#ifndef _SODROPITEM_H_
#define _SODROPITEM_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#include "fingerfarben.h"
#include "SoDropTarget.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
A simple wrapper for geometry on which actions of a SoDropTarget can
be applied.

File Format / defaults:
@verbatim

SoDropItem {
    SoSFString  commands    "*"         # actions that are allowed on this item. Use "*" to accept all actions.
    SoNode      geometry    Group {}    # geometry that the action should be applied to
}
@endverbatim

@ingroup fingerfarben
*/
class FINGERFARBEN_API SoDropItem : public SoBaseKit
{
    SO_KIT_HEADER(SoDropItem);

     /// geometry separator
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);

public: // member-variables

    /// A String containing the commands that this item accepts. "*" accepts all commands.
    SoSFString commands;

public:// member-methods
				
    /// Static method to initialize class for OpenInventor
    static void initClass();
    
    /// Constructor.
    SoDropItem();

	/// Destructor.			
    virtual ~SoDropItem();

    /// SoDropTarget will alter and read our state variable.
    friend class SoDropTarget;

private:

    /// State values used by SoDropTarget to keep track of the state of this item.
    enum state_t {
        OUTSIDE = 0,    ///< The item is outside the bounds.
        HOVER,          ///< The item is inside the bounds, before an action is triggered.
        ACTION          ///< The item is inside the bounds, after the action has been triggered.
    };

    /// State field to keep track of state. Used by SoDropTarget.
    state_t state;

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SODROPITEM_H_
