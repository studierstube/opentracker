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
/** SignPost BAU
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: SoBAUStyleKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSwitch.h>

#include "SoBAUStyleKit.h"

SO_KIT_SOURCE(SoBAUStyleKit);

//----------------------------------------------------------------------------

void
SoBAUStyleKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoBAUStyleKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoBAUStyleKit::SoBAUStyleKit()
{
    SO_KIT_CONSTRUCTOR(SoBAUStyleKit);

    SO_KIT_ADD_CATALOG_ENTRY(styleSwitch,  SoSwitch,   TRUE, this, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(allStyle,     SoGroup,    TRUE, styleSwitch, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(wallStyle,    SoGroup,    TRUE, styleSwitch, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(ceilingStyle, SoGroup,    TRUE, styleSwitch, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(floorStyle,   SoGroup,    TRUE, styleSwitch, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(portalStyle,  SoGroup,    TRUE, styleSwitch, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(specialStyle, SoGroup,    TRUE, styleSwitch, \x0, TRUE);

    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoBAUStyleKit::~SoBAUStyleKit()
{
}

//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

