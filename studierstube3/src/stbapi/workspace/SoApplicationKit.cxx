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
/** SoApplicationKit node kit class implementation
  *
  * @author Andreas Zajic
  *
  * $Id: SoApplicationKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file SoApplicationKit.cxx                                              */
 /* ======================================================================= */
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>

#include <stbapi/workspace/SoApplicationKit.h>

SO_KIT_SOURCE(SoApplicationKit);

//initClass

void
SoApplicationKit::initClass()
{
    SO_KIT_INIT_CLASS(SoApplicationKit, SoBaseKit, "BaseKit");
}

// Constructor

SoApplicationKit::SoApplicationKit()
{
    SO_KIT_CONSTRUCTOR(SoApplicationKit);

    SO_KIT_ADD_FIELD(readOnly,(FALSE));
    SO_KIT_ADD_FIELD(loadFileName,(""));

    //       --------------------name---|---type-------|nuldef|-par |rite|public
    SO_KIT_ADD_CATALOG_ENTRY(classLoader, SoClassLoader, TRUE , this,\x0 ,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(contextKit,   SoContextKit, TRUE , this,\x0 ,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(appGeom,      SoSeparator , FALSE , this,\x0 ,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(info,              SoInfo , TRUE, this,\x0 ,TRUE);
    
    SO_KIT_INIT_INSTANCE();

    SoTexture2 * texture = new SoTexture2;
    texture->filename = "graphix/b_cube.jpg";
    ((SoSeparator *)appGeom.getValue())->addChild( texture );
}

// Destructor

SoApplicationKit::~SoApplicationKit()
{
    // printf("SoApplicationKit destructed!\n");
}




