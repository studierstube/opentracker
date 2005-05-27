 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
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
  * ======================================================================== */
//  NAME:       SoBillboardKit
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    SoBillboardKit - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ep      Erich Pohn
//              ds      Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
//  29-may-02   ep      last modification
// ===========================================================================
#ifndef _SOBILLBOARDKIT_H_
#define _SOBILLBOARDKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoCallback.h> 
#include <Inventor/actions/SoAction.h> 
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#include <stbapi/resource/viewer/SoStereoCameraKit.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/SoUserManagerKit.h>

#include <stbapi/stbapi.h>
#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoBillboardKit;

/**
 * @ingroup util
 */
class STBAPI_API SoBillboardKit : public SoBaseKit {

    SO_KIT_HEADER(SoBillboardKit);

	SO_KIT_CATALOG_ENTRY_HEADER(root);
	SO_KIT_CATALOG_ENTRY_HEADER(cbNode);
	// transform node should be used to set the translation of a Billboard-object
	SO_KIT_CATALOG_ENTRY_HEADER(transform);
	SO_KIT_CATALOG_ENTRY_HEADER(content);

public:

    SoBillboardKit();
	static void initClass();

protected:

	~SoBillboardKit();

private:

    static SbVec3f getActiveEyepoint(int userId);
    static void billboardCB(void *userData, SoAction *action);
    void performRotation(SoState * state) const;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
