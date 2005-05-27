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
  * Defines SoTemplateKit, a subclass of SoDropTarget that acts as a template to
  * replace the geometry of a SoDropItem.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoTemplateKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOTEMPLATEKIT_H_
#define _SOTEMPLATEKIT_H_

#include <Inventor/SoPath.h>

#include "fingerfarben.h"
#include "SoDropTarget.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
A subclass of SoDropTarget that acts as a template to replace the geometry
of a SoDropItem. The SoDropItem must accept the commands "TEMPLATE" or "*"
to be affected by this target. Any node that has a DEF name of "TEMPLATE"
within the geometry node of this node is used as template geometry to replace
the geometry node of the SoDropItem. Note that you can also use this node to 
delete geometry, by simply using (for example) an empty Group node as 
template. 

@ingroup fingerfarben
*/
class FINGERFARBEN_API SoTemplateKit : public SoDropTarget 
{
    SO_KIT_HEADER(SoTemplateKit);

public:
    /// constructor
    SoTemplateKit();

    /// static method to initialize class for OpenInventor
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

#endif//_SOTEMPLATEKIT_H_
