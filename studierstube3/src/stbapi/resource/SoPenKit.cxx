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
/** SoPenKit.cxx contains the implementation of class SoPenKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoPenKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/util/ivio.h>

#include "SoPenKit.h"

SO_KIT_SOURCE(SoPenKit);

//----------------------------------------------------------------------------

/** Inventor class initialization */

void
SoPenKit::initClass()
{
    SO_KIT_INIT_CLASS(SoPenKit, SoTrackedArtifactKit, "SoTrackedArtifactKit");
}

//----------------------------------------------------------------------------
/** The constructor inits all fields. */

SoPenKit::SoPenKit()
{
    SO_KIT_CONSTRUCTOR(SoPenKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(
        dragGeometry,    SoNode, SoSeparator,TRUE, artifactRoot, \0 ,FALSE );

    // add fields
    SO_KIT_ADD_FIELD(primaryButton, (0)); 
    SO_KIT_ADD_FIELD(secondaryButton, (1)); 
    SO_KIT_ADD_FIELD(focusPolicy, (CLICK_TO_FOCUS)); 
    
    SO_NODE_DEFINE_ENUM_VALUE(FocusPolicy, POINT_TO_FOCUS);
    SO_NODE_DEFINE_ENUM_VALUE(FocusPolicy, CLICK_TO_FOCUS);
    SO_NODE_DEFINE_ENUM_VALUE(FocusPolicy, CLICK_TO_MASTER);

    SO_NODE_SET_SF_ENUM_TYPE(focusPolicy, FocusPolicy);

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

}


//----------------------------------------------------------------------------
/** The destructor does nothing */

SoPenKit::~SoPenKit()
{
    // nil
}

//----------------------------------------------------------------------------
/** Enables the pen to trigger 3D events in the given graph ("root") */

void
SoPenKit::setEventRoot(SoNode *root) 
{
    Stb3DEventGenerator::setRoot(station.getValue(),root);
}

//----------------------------------------------------------------------------
/** Attaches the geometry of the current Drag&Drop to the pen. */

void
SoPenKit::setDragGeometry(SoNode *dragGeometry) 
{
    setAnyPart("dragGeometry",dragGeometry);
}

//----------------------------------------------------------------------------
/** Use this method to clear the dragGeometry field */

void
SoPenKit::removeDragGeometry() 
{
    setAnyPart("dragGeometry",NULL);
}

//----------------------------------------------------------------------------

//eof

