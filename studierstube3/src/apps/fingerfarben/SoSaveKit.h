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
  * Defines SoSaveKit, a subclass of SoDropTarget to save geometry.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoSaveKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOSAVEKIT_H_
#define _SOSAVEKIT_H_

#include <Inventor/SoPath.h>

#include "fingerfarben.h"
#include "SoDropTarget.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
A subclass of SoDropTarget to save geometry. Every SoDropItem that triggers
the action of this node (must accept the "SAVE" or "*" action) is automatically
saved to disk. The filename is taken from the filename field, plus a serial
number that is automatically appended and increased with every save action.

@see SoDropTarget
@see SoDropItem

@ingroup fingerfarben
*/
class FINGERFARBEN_API SoSaveKit : public SoDropTarget 
{
    SO_KIT_HEADER(SoSaveKit);

public:
    /// constructor
    SoSaveKit();

    /// static method to initialize class for OpenInventor
    static void initClass();
    
    /**
    This method is called if an action on an SoDropTarget is triggered within
    the bounds of this node.
    @see SoDropTarget
    @see SoDropItem
    */
    virtual bool handleItem(SoFullPath* pathToItem);

    /**
    The path+filename to use for saving. A serial number and the '.iv'
    extension are added automatically.
    */
    SoSFString filename;

protected:

    /// The serial number to append to saved files.
    static int numSaved;
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOSAVEKIT_H_
