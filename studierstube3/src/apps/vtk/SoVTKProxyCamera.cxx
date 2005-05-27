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
 |   $Revision: 1.3 $
 |
 |   Classes:
 |      SoVTKProxyCamera
 |
 |
 |   Author(s)          : Stefan Bruckner and Rudolf Seemann
 |
 _______________________________________________________________________
 */

#include <vtkRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLCamera.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <vtkObjectFactory.h>
#include <vtkOutputWindow.h>
#include <vtkgluPickMatrix.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkPerspectiveTransform.h>

#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include "SoVTKProxyCamera.h"

SO_NODE_SOURCE(SoVTKProxyCamera);

SoVTKProxyCamera::SoVTKProxyCamera(void)
{
    SO_NODE_CONSTRUCTOR(SoVTKProxyCamera);
	this->ParallelProjectionOff();
}

SoVTKProxyCamera::~SoVTKProxyCamera(void)
{
}

void SoVTKProxyCamera::initClass(void)
{
	SO_NODE_INIT_CLASS(SoVTKProxyCamera,SoNode,"SoNode");
}

/**
 * GLRender is called on every scenegraph traversal
 * It gets the viewing matrix of the camera
 */
void SoVTKProxyCamera::GLRender(SoGLRenderAction *action)
{
	SoState *state = action->getState();
	SbMatrix projectionMatrix = SoProjectionMatrixElement::get(state);
	SbMatrix viewingMatrix = SoViewingMatrixElement::get(state);

	vtkMatrix4x4 *v = vtkMatrix4x4::New();

	for(int i = 0; i<4; i++)
		for(int j = 0; j<4; j++)
		{
			v->Element[i][j] = (double) viewingMatrix[j][i];
		}

	this->ViewTransform->Identity();
	this->ViewTransform->SetMatrix(v);

	this->Position[0] = this->ViewTransform->GetMatrix()->Element[0][3] / 
						this->ViewTransform->GetMatrix()->Element[3][3];
	this->Position[1] = this->ViewTransform->GetMatrix()->Element[1][3] / 
						this->ViewTransform->GetMatrix()->Element[3][3];
	this->Position[2] = this->ViewTransform->GetMatrix()->Element[2][3] / 
						this->ViewTransform->GetMatrix()->Element[3][3];

	vtkMatrix4x4 *p = vtkMatrix4x4::New();

	for(int i = 0; i<4; i++)
		for(int j = 0; j<4; j++)
		{
			p->Element[j][i] = (double) projectionMatrix[i][j];
		}

	this->PerspectiveTransform->Identity();
	this->PerspectiveTransform->SetMatrix(p);

	//	this->PerspectiveTransform->Print(cout);
	v->Delete();
	p->Delete();
/*
	// calulate camera parameters from projection matrix
	glGetFloatv(GL_PROJECTION_MATRIX, matrix1);
	// transpone matrix (OpenGL uses t(M))
	for(int i = 0; i<4; i++)
		for(int j = 0; j<4; j++)
		{
			dmatrix[i][j] = (double) matrix1[j+i*4];
		}

	double t,b;
	b = m_pSoCamera->nearDistance.getValue()*(dmatrix[1][2] -1) / dmatrix[1][1];
	t = ((dmatrix[1][2] + 1) * b) / (dmatrix[1][2] -1);
	double angle = atan((t-b)/m_pSoCamera->nearDistance.getValue());
	angle = 360 * angle/(2*M_PI);
	//	this->SetViewAngle(angle);

 
//	glGetFloatv(GL_MODELVIEW_MATRIX, matrix1);
	// transpose matrix (OpenGL uses t(M))
	for(int i = 0; i<4; i++)
		for(int j = 0; j<4; j++)
		{
			this->ViewTransform->GetMatrix()->Element[j][i] = (double) matrix1[i*4+j];
		}
	this->Position[0] = this->ViewTransform->GetMatrix()->Element[0][3] / 
						this->ViewTransform->GetMatrix()->Element[3][3];
	this->Position[1] = this->ViewTransform->GetMatrix()->Element[1][3] / 
						this->ViewTransform->GetMatrix()->Element[3][3];
	this->Position[2] = this->ViewTransform->GetMatrix()->Element[2][3] / 
						this->ViewTransform->GetMatrix()->Element[3][3];

	m_pSoCamera->nearDistance.setValue(0.1);
	m_pSoCamera->farDistance.setValue(1000.0);
	//	this->SetClippingRange(m_pSoCamera->nearDistance.getValue(),
	//						   m_pSoCamera->farDistance.getValue());
*/
}
void SoVTKProxyCamera::Render(vtkRenderer *ren)
{
	float aspect[2];
	float *vport;
	int  lowerLeft[2];

	vtkMatrix4x4 *matrix = vtkMatrix4x4::New();
	vtkMatrix4x4 *invCameraView = vtkMatrix4x4::New();

	invCameraView->DeepCopy(this->GetViewTransformMatrix());
	invCameraView->Invert();
	std::list<SoVTKProxyProp3D *>::iterator it = m_lisProxies.begin();

	for(;it != m_lisProxies.end(); it++)
	{
		(*it)->Update(invCameraView);
	}


	//
	//vtkOpenGLCamera::Render(ren);
	//return;
	// ViewUp muss aus der Orientation berechnet werden, einfache Rotation
	//	this->SetViewUp(camera0->orientation.getValue().getValue());


	// find out if we should stereo render
	this->Stereo = (ren->GetRenderWindow())->GetStereoRender();
	vport = ren->GetViewport();

	float vpu, vpv;
	vpu = vport[0];
	vpv = vport[1];  
	ren->NormalizedDisplayToDisplay(vpu,vpv);
	lowerLeft[0] = (int)(vpu+0.5);
	lowerLeft[1] = (int)(vpv+0.5);
	float vpu2, vpv2;
	vpu2 = vport[2];
	vpv2 = vport[3];  
	ren->NormalizedDisplayToDisplay(vpu2,vpv2);
	int usize = (int)(vpu2 + 0.5) - lowerLeft[0];
	int vsize = (int)(vpv2 + 0.5) - lowerLeft[1];  

	// if were on a stereo renderer draw to special parts of screen
	/*  if (this->Stereo)
	{
	switch ((ren->GetRenderWindow())->GetStereoType())
		{
		case VTK_STEREO_CRYSTAL_EYES:
		if (this->LeftEye)
			{
			glDrawBuffer(GL_BACK_LEFT);
			}
		else
			{
			glDrawBuffer(GL_BACK_RIGHT);
			}
		break;
		case VTK_STEREO_LEFT:
		this->LeftEye = 1;
		break;
		case VTK_STEREO_RIGHT:
		this->LeftEye = 0;
		break;
		default:
		break;
		}
	}
	else
	{
	if (ren->GetRenderWindow()->GetDoubleBuffer())
		{
		glDrawBuffer(GL_BACK);
		}
	else
		{
		glDrawBuffer(GL_FRONT);
		}
	}
	*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(lowerLeft[0],lowerLeft[1], usize, vsize);

	ren->ComputeAspect();
	ren->GetAspect(aspect);
	glMatrixMode( GL_PROJECTION);
	matrix->DeepCopy(this->PerspectiveTransform->GetMatrix());//this->GetPerspectiveTransformMatrix(aspect[0]/aspect[1], -1,1));
	matrix->Transpose();
	if(ren->GetIsPicking())
	{
	int size[2]; size[0] = usize; size[1] = vsize;
	glLoadIdentity();
	vtkgluPickMatrix(ren->GetPickX(), ren->GetPickY(), 1, 1, lowerLeft, size);
	glMultMatrixd(matrix->Element[0]);
	}
	else
	{
	// insert camera view transformation 
	glLoadMatrixd(matrix->Element[0]);
	}
	
	// push the model view matrix onto the stack, make sure we 
	// adjust the mode first
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	matrix->DeepCopy(this->GetViewTransformMatrix());
	matrix->Transpose();

	// insert camera view transformation 
	glMultMatrixd(matrix->Element[0]);
	/*  

	if ((ren->GetRenderWindow())->GetErase())
	{
	ren->Clear();
	}
	*/


	// if we have a stereo renderer, draw other eye next time 
	if (this->Stereo)
	{
	if (this->LeftEye)
		{
		this->LeftEye = 0;
		}
	else
		{
		this->LeftEye = 1;
		}
	}
	matrix->Delete();
	
	//	glPopMatrix();
}

void SoVTKProxyCamera::AddProxyProp(SoVTKProxyProp3D *prop)
{
	this->m_lisProxies.push_back(prop);
}

vtkMatrix4x4 * SoVTKProxyCamera::GetPerspectiveTransformMatrix(double aspect, double nearZ, double farZ)
{
  //    this->ComputePerspectiveTransform(aspect,nearZ,farZ);
  //  this->PerspectiveTransform->Print(cout);
  return this->PerspectiveTransform->GetMatrix();
  
}






