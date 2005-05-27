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
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** SoAlphaMaskKit source file
  *
  * @author Gottfried Eibner
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <string.h>

#include "SoAlphaMaskKit.h"
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoQuadMesh.h>

#include <Inventor/elements/SoElements.h>

#include <string.h>

SO_KIT_SOURCE(SoAlphaMaskKit);

void SoAlphaMaskKit::initClass()
{
    SO_KIT_INIT_CLASS(SoAlphaMaskKit, SoBaseKit, "BaseKit");
}

// constructor

#include <Inventor/actions/SoWriteAction.h>
SoAlphaMaskKit::SoAlphaMaskKit()
{
    // The part 'rectangle' must be public in order to be rendered by OpenInventor.

    SO_KIT_CONSTRUCTOR(SoAlphaMaskKit);

    SO_KIT_ADD_CATALOG_ENTRY(texQuality  ,SoComplexity,FALSE,this,\0,FALSE);

    SO_KIT_ADD_CATALOG_ENTRY(texture     ,SoTexture2  ,TRUE ,this,\0,TRUE );

    SO_KIT_ADD_CATALOG_ENTRY(stateChange ,SoCallback  ,FALSE,this,\0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(rectangle   ,SoQuadMesh  ,FALSE,this,\0,TRUE );

    SO_KIT_INIT_INSTANCE();

    SO_GET_ANY_PART(this,"stateChange",SoCallback)->setCallback(stateChangeCB,(void *)this);
    SO_GET_ANY_PART(this,"texQuality" ,SoComplexity)->textureQuality = 1.0;

    char *vertexProperty =
    "VertexProperty {\n"
    "    vertex   [ -1  1 -0.1 ,  1  1 -0.1 , -1 -1 -0.1 ,  1 -1 -0.1 ]\n"
    "    texCoord [     0 1 ,     1 1 ,     0 0 ,     1 0 ]\n"
    "    materialBinding OVERALL\n"
    "    normalBinding   OVERALL\n"
    "}\n";

    // build unit quad mesh (a quad which covers the whole projection area of one tile).
    SoNode *result;
    SoInput in;

    SoVertexProperty *property;
    in.setBuffer(vertexProperty, strlen(vertexProperty));
    SoDB::read(&in, result);
    property = (SoVertexProperty *)result;

    set("rectangle" ,"verticesPerColumn 2 verticesPerRow 2");
    SO_GET_ANY_PART(this,"rectangle" ,SoQuadMesh)->vertexProperty = property;
}

SoAlphaMaskKit::~SoAlphaMaskKit()
{
}

void SoAlphaMaskKit::stateChangeCB(void *node, SoAction *action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId()))
    {
        SoGLRenderAction *ra = (SoGLRenderAction *)action;
        ra->enableElement(SoLazyElement::getClassTypeId(),FALSE);

        static const SbMatrix m(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

     // Reset all transformation matrices
        SoProjectionMatrixElement::set(ra->getState(),(SoNode *)node,m);
        SoModelMatrixElement::set(ra->getState(),(SoNode *)node,m);
        SoViewingMatrixElement::set(ra->getState(),(SoNode *)node,m);
    }
}
