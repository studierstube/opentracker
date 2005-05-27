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
  * ========================================================================
  * PROJECT: Studierstube - StubeRenA: Studierstube Render Array
  * ======================================================================== */
/** SoAlphaMaskKit.h  - definition of class SoAlphaMaskKit.
  * SoAlphaMaskKit keeps an alpha texture for blending overlapping render
  * areas as happened when multiple projectors are used which are allowed to
  * overlap.
  *
  * @author Gottfried Eibner
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOALPHAMASKKIT_H_
#define _SOALPHAMASKKIT_H_

#include <stbapi/stbapi.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * This class renders an alpha mask for blending multiple overlapping render
 * areas (i.e. using multiple projectors which are allowed to overlap their
 * projection areas). This class is used during tiled projection in the
 * StubeRenA project.
 * The Kit contains one public part - texture, a SoTexture2 node - and several
 * private parts to render the texture as overlay to cover the whole scene.
 *
 * File format :
<pre>SoAlphaMaskKit {
  SoTexture2 texture
}</pre>
 *
 * @author Gottfried Eibner
 * @ingroup misc
 */
class STBAPI_API SoAlphaMaskKit : public SoBaseKit
{
    SO_KIT_HEADER(SoAppearanceKit);

    SO_KIT_CATALOG_ENTRY_HEADER(texQuality  );  // SoComplextiy,     private
    SO_KIT_CATALOG_ENTRY_HEADER(texture     );  // SoTexture2,       public
    SO_KIT_CATALOG_ENTRY_HEADER(stateChange );  // SoCallback,       private
    SO_KIT_CATALOG_ENTRY_HEADER(rectangle   );  // SoQuadMesh,       public

public:
    SoAlphaMaskKit(void);

SoINTERNAL public:
    static void initClass();

private:
    virtual ~SoAlphaMaskKit(void);

 // reset modelview and projection matrix elements
 // disable depth test and ligthing -> lazy element
    static void stateChangeCB(void *node, SoAction *action);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
