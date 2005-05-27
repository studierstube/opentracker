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
  * Implements SoDropItem, a simple wrapper for geometry on which actions of a SoDropTarget can
  * be applied.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoDropItem.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file
  */
 /* ======================================================================= */

#include <memory>
#include <string>

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoCube.h>

#include "SoDropItem.h"

using namespace std;

// OpenInventor macros

SO_KIT_SOURCE(SoDropItem);
 
// class initialization for OpenInventor

void SoDropItem::initClass(void)
{
    SO_KIT_INIT_CLASS(SoDropItem, SoBaseKit, "BaseKit");
}

// constructor

SoDropItem::SoDropItem()
{
    SO_KIT_CONSTRUCTOR(SoDropItem);

    //--------------------------------name--------------|-type----  |-deftype|nuldef|-parent---------|right sib|public
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(geometry,          SoNode,     SoGroup, FALSE, this,            \x0,      TRUE  );
        
    SO_KIT_ADD_FIELD(commands, ("*"));

    SO_KIT_INIT_INSTANCE();
    
}

/*
SbBool SoDropItem::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (onOff) {
        printf("SoDropItem: setUpConnections(on)\n");
    }
}
*/

// destructor, clears all lists and removes any listeners etc.

SoDropItem::~SoDropItem()
{
}


