/*
 *
 *  Copyright (C) 2003
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 * 
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * Copyright (C) 2003
 *
 _______________________________________________________________________
 |
 |   $Revision: 1.2 $
 |
 |   Classes:
 |      SoVTKProxyProp3D
 |
 |
 |   Author(s)          : Stefan Bruckner and Rudolf Seemann
 |
 _______________________________________________________________________
 */

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <Inventor/SbBox.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/misc/SoState.h>

#include "SoVTKProxyProp3D.h"

SO_NODE_SOURCE(SoVTKProxyProp3D);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoVTKProxyProp3D::SoVTKProxyProp3D()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoVTKProxyProp3D);
    isBuiltIn = TRUE;
	this->m_pvtkProp3D = 0;
	this->m_pModelviewMatrix = 0;
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    sets SoCamera
//
// Use: private
void SoVTKProxyProp3D::Update(vtkMatrix4x4 *matrix)
//
////////////////////////////////////////////////////////////////////////
{
	if(m_pvtkProp3D != 0 && this->m_pModelviewMatrix != 0)
	{
		vtkMatrix4x4 *tempMatrix = vtkMatrix4x4::New();
		vtkMatrix4x4::Multiply4x4(matrix, this->m_pModelviewMatrix, tempMatrix);
		m_pvtkProp3D->SetUserMatrix(tempMatrix);
		//		m_pvtkProp3D->ComputeMatrix();
		tempMatrix->Delete();
	}
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoVTKProxyProp3D::~SoVTKProxyProp3D()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    sets a vtkProp3D object.
//
// Use: public

void SoVTKProxyProp3D::SetVTKProp3D(vtkProp3D *prop)
//
////////////////////////////////////////////////////////////////////////
{
	m_pvtkProp3D = prop;
}

void SoVTKProxyProp3D::initClass(void)
{
	SO_NODE_INIT_CLASS(SoVTKProxyProp3D, SoNode, "SoNode");
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    reads the OpenGL modelview matrix of the object
//
// Use: extender

void
SoVTKProxyProp3D::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
	float matrix1[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix1);
	// transpose matrix (OpenGL uses t(M))
	m_pModelviewMatrix = vtkMatrix4x4::New();
	for(int i = 0; i<4; i++)
		for(int j = 0; j<4; j++)
		{
			m_pModelviewMatrix->Element[j][i] = (double) matrix1[i*4+j];
		}
}
