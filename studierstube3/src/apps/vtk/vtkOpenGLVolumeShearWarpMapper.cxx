/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkOpenGLVolumeShearWarpMapper.cxx,v $
  Language:  C++
  Date:      $Date: 2003/04/08 17:21:16 $
  Version:   $Revision: 1.2 $


Copyright (c) 1993-2001 Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names
   of any contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkOpenGLVolumeShearWarpMapper.h"
#include "vtkMatrix4x4.h"
#include "vtkVolume.h"
#ifndef VTK_IMPLEMENT_MESA_CXX
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif
#include "vtkObjectFactory.h"
#include "vtkTimerLog.h"
#include <float.h>

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#ifndef VTK_IMPLEMENT_MESA_CXX
//------------------------------------------------------------------------------
vtkOpenGLVolumeShearWarpMapper* vtkOpenGLVolumeShearWarpMapper::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkOpenGLVolumeShearWarpMapper");
  if(ret)
    {
    return (vtkOpenGLVolumeShearWarpMapper*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkOpenGLVolumeShearWarpMapper;
}
#endif



vtkOpenGLVolumeShearWarpMapper::vtkOpenGLVolumeShearWarpMapper()
{
}

vtkOpenGLVolumeShearWarpMapper::~vtkOpenGLVolumeShearWarpMapper()
{
}

void vtkOpenGLVolumeShearWarpMapper::RenderTexture(vtkRenderer *ren, vtkVolume *vol)
{
  if (this->vtkVolumeShearWarpMapper::IntermixIntersectingGeometry)
    glDisable(GL_DEPTH_TEST);

	glMatrixMode( GL_PROJECTION );
  glPushMatrix();
	glLoadIdentity();

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
	glLoadIdentity();

  float warp[16];
  int row, col, i=0;

  for (col=0; col<4; ++col)
    for (row=0; row<4; ++row)
      warp[i++] = this->WarpMatrix->Element[row][col];

  glLoadMatrixf(warp);

  glDisable( GL_LIGHTING );
  glEnable( GL_TEXTURE_2D );

  GLuint tempIndex;
  glGenTextures(1, &tempIndex);
  glBindTexture(GL_TEXTURE_2D, tempIndex);

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, this->ImageWidth, this->ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->ImageData );
  glColor3f( 1.0, 1.0, 1.0 );

  float position[4][3];

  position[0][0] = -this->TranslationI;
  position[0][1] = -this->TranslationJ;
	position[0][2] = 0.0;
  
  position[1][0] = position[0][0] + this->IntermediateWidth * this->ImageSampleDistance;
  position[1][1] = position[0][1];
	position[1][2] = position[0][2];

  position[2][0] = position[0][0] + this->IntermediateWidth * this->ImageSampleDistance;
  position[2][1] = position[0][1] + this->IntermediateHeight * this->ImageSampleDistance;
	position[2][2] = position[0][2];

  position[3][0] = position[0][0];           
  position[3][1] = position[0][1] + this->IntermediateHeight * this->ImageSampleDistance;
  position[3][2] = position[0][2];

	float px = float(this->vtkVolumeShearWarpMapper::IntermediateWidth)/ float(this->vtkVolumeShearWarpMapper::ImageWidth);
  float py = float(this->vtkVolumeShearWarpMapper::IntermediateHeight) / float(this->vtkVolumeShearWarpMapper::ImageHeight);  

  glDepthMask(0);
  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 0.0f);
  glVertex3fv(position[0]);    // bottom left

  glTexCoord2f(px, 0.0f);
  glVertex3fv(position[1]);    // bottom left
      
  glTexCoord2f(px, py);
  glVertex3fv(position[2]);    // bottom left
      
  glTexCoord2f(0.0f, py);
  glVertex3fv(position[3]);    // bottom left
      
  glEnd();
  glDepthMask(1);

  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();

  glMatrixMode( GL_PROJECTION );
  glPopMatrix();

  glDisable( GL_TEXTURE_2D );
  glEnable( GL_LIGHTING );

  if (this->vtkVolumeShearWarpMapper::IntermixIntersectingGeometry)
    glEnable(GL_DEPTH_TEST);
	
//	glDrawPixels(this->ZBufferSize[0],this->ZBufferSize[1],GL_RED,GL_FLOAT,this->ZBuffer->GetPointer(0));
//	if (this->ImageSampleDistance == 1)
//		glDrawPixels(this->IntermediateWidth,this->IntermediateHeight,GL_LUMINANCE,GL_FLOAT,this->IntermediateZBuffer);
//		glDrawPixels(this->IntermediateWidth,this->IntermediateHeight,GL_LUMINANCE,GL_FLOAT,this->ClippingFrontBuffer);

}

float * vtkOpenGLVolumeShearWarpMapper::Extract(vtkFloatArray *array, vtkRenderer *ren, vtkVolume *vol)
{
	vtkTimerLog *timer = vtkTimerLog::New();
	int *renWinSize   =  ren->GetRenderWindow()->GetSize();
  int row, col, i=0,j=0;

	const double projection[16] = { 1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0 };
  double modelview[16];

	i = 0;
	for (col=0; col<4; ++col)
    for (row=0; row<4; ++row)
      modelview[i++] = this->WarpMatrix->Element[row][col];

	int viewport[4];
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = renWinSize[0]-1;
	viewport[3] = renWinSize[1]-1;

  double position[4][3];
  position[0][0] = -this->TranslationI;
  position[0][1] = -this->TranslationJ;
	position[0][2] = 0.0;
  
  position[1][0] = position[0][0] + this->IntermediateWidth * this->ImageSampleDistance;
  position[1][1] = position[0][1];
	position[1][2] = position[0][2];

  position[2][0] = position[0][0] + this->IntermediateWidth * this->ImageSampleDistance;
  position[2][1] = position[0][1] + this->IntermediateHeight * this->ImageSampleDistance;
	position[2][2] = position[0][2];

  position[3][0] = position[0][0];           
  position[3][1] = position[0][1] + this->IntermediateHeight * this->ImageSampleDistance;
  position[3][2] = position[0][2];

	gluProject(position[0][0],position[0][1],position[0][2],modelview,projection,viewport,&position[0][0],&position[0][1],&position[0][2]);
	gluProject(position[1][0],position[1][1],position[1][2],modelview,projection,viewport,&position[1][0],&position[1][1],&position[1][2]);
	gluProject(position[2][0],position[2][1],position[2][2],modelview,projection,viewport,&position[2][0],&position[2][1],&position[2][2]);
	gluProject(position[3][0],position[3][1],position[3][2],modelview,projection,viewport,&position[3][0],&position[3][1],&position[3][2]);

	double min[3],max[3];

	min[0] = position[0][0];
	min[1] = position[0][1];
	min[2] = position[0][2];

	max[0] = position[0][0];
	max[1] = position[0][1];
	max[2] = position[0][2];

	for (i=1; i < 4; i++)
	{
		min[0] = (position[i][0] < min[0]) ? position[i][0] : min[0];
		min[1] = (position[i][1] < min[1]) ? position[i][1] : min[1];
		min[2] = (position[i][2] < min[2]) ? position[i][2] : min[2];

		max[0] = (position[i][0] > max[0]) ? position[i][0] : max[0];
		max[1] = (position[i][1] > max[1]) ? position[i][1] : max[1];
		max[2] = (position[i][2] > max[2]) ? position[i][2] : max[2];
	}

	int zx1 = int((min[0] < 0) ? 0 : min[0]);
  int zy1 = int((min[1] < 0) ? 0 : min[1]);
  int zx2 = int((max[0] > renWinSize[0]-1) ? renWinSize[0] - 1 : ceilf(max[0]));
  int zy2 = int((max[1] > renWinSize[1]-1) ? renWinSize[1] - 1 : ceilf(max[1]));

	timer->StartTimer();
	ren->GetRenderWindow()->GetZbufferData(zx1,zy1,zx2,zy2,this->ZBuffer);
	timer->StopTimer();

	this->ZBufferSize[0] = zx2 - zx1 + 1;
	this->ZBufferSize[1] = zy2 - zy1 + 1;
	printf(">> Read (%d,%d): %f s\n",this->ZBufferSize[0],this->ZBufferSize[1],timer->GetElapsedTime());

	timer->StartTimer();
	array->Resize(this->IntermediateWidth * this->IntermediateHeight);

	const int dWidth = this->IntermediateWidth;
	const int dHeight = this->IntermediateHeight;

	const int sWidth = this->ZBufferSize[0];
	const int sHeight = this->ZBufferSize[1];

	float *source = this->ZBuffer->GetPointer(0);
	float *destination = array->GetPointer(0);

  int xs,ys;
  double xd,yd,zd;
  double warped[4];

	const float scaleX = float(dWidth) / (max[0] - min[0] + 1);
  const float scaleY = float(dHeight) / (max[1] - min[1] + 1);

  int kernelX = int(ceil(2.0 * scaleX));
  int kernelY = int(ceil(2.0 * scaleY));

  const int startX = -kernelX / 2;  
  const int endX = kernelX + startX - 1;

  const int startY = -kernelY / 2;
  const int endY = kernelY + startY - 1;

  if (this->ReverseOrder == 0)
  {
    for (j=0; j < dHeight; j++)
    {
      for (i=0; i < dWidth; i++)
      {
        destination[i + j*dWidth] = FLT_MAX;
      }
    }
  }
  else
  {
    for (j=0; j < dHeight; j++)
    {
      for (i=0; i < dWidth; i++)
      {
        destination[i + j*dWidth] = -FLT_MAX;
      }
    }
  }

	float isd = 1.0 / this->ImageSampleDistance;
	float iv0 = 1.0 / viewport[2];
	float iv1 = 1.0 / viewport[3];

	double origin[3];
	gluUnProject(position[0][0],position[0][1],position[0][2],modelview,projection,viewport,&origin[0],&origin[1],&origin[2]);

	vtkMatrix4x4 *warp = vtkMatrix4x4::New();
	warp->DeepCopy(this->WarpMatrix);
	warp->Invert();

	const double *m = &warp->Element[0][0];

  for (ys=0; ys < sHeight; ys++)
  {
    for (xs=0; xs < sWidth; xs++)
    {
			// gluUnproject
			/*
													(  2(winX - view[0])      )
													|  ----------------- - 1  |
													|       view[2]           |
				(      )          |                         |
				| objX |          |  2(winY - view[1])      |
				| objY | = INV(PM)|  ----------------- - 1  |
				| objZ |          |       view[3]           |
				(  W   )          |                         |
													|       2(winZ) - 1       |
													|                         |
													(            1            )
			*/
			//gluUnProject(xs + zx1,ys + zy1,0.5*source[xs + sWidth*ys] + 0.5,modelview,projection,viewport,&warped[0],&warped[1],&warped[2]);

			// we do extactly the same as gluUnproject does, but this is way faster
			const double vector[4] = {2.0 * (xs + zx1 - viewport[0]) / viewport[2] - 1.0, 2.0 * (ys + zy1 - viewport[1]) / viewport[3] - 1.0, source[xs + sWidth*ys], 1.0};

			warped[0] = vector[0]*m[0]  + vector[1]*m[1]  + vector[2]*m[2]  + vector[3]*m[3];
			warped[1] = vector[0]*m[4]  + vector[1]*m[5]  + vector[2]*m[6]  + vector[3]*m[7];
			warped[2] = vector[0]*m[8]  + vector[1]*m[9]  + vector[2]*m[10] + vector[3]*m[11];
			warped[3] = vector[0]*m[12] + vector[1]*m[13] + vector[2]*m[14] + vector[3]*m[15];

			warped[0] /= warped[3];
			warped[1] /= warped[3];
			warped[2] /= warped[3];

			xd = warped[0] - origin[0];
      yd = warped[1] - origin[1];
			zd = warped[2];

			xd /= this->ImageSampleDistance;
			yd /= this->ImageSampleDistance;


			const int iXd0 = (int) xd;
      const int iYd0 = (int) yd;
      
      if ( iXd0 >= 0 && iYd0 >= 0 && iXd0 < dWidth && iYd0 < dHeight )
      {
        const int iXd1 = (iXd0+1 < dWidth)  ? iXd0+1 : 0;
        const int iYd1 = (iYd0+1 < dHeight) ? iYd0+1 : 0;
       
        if (this->ReverseOrder == 0)
        {
          for (j=startY; j <= endY; j++)
          {
            for (i=startX; i <= endX; i++)
            {
              if ( (iXd0+i) >= 0 && (iYd0+j) >= 0 && (iXd0 + i) < dWidth && (iYd0 + j) < dHeight )
              {              
                float & value = destination[(iXd0 + i) + (iYd0 + j)*dWidth];
                value = (zd < value) ? zd : value;
              }
            }
          }
        }
        else
        {
          for (j=startY; j <= endY; j++)
          {
            for (i=startX; i <= endX; i++)
            {
							if ( (iXd0+i) >= 0 && (iYd0+j) >= 0 && (iXd0 + i) < dWidth && (iYd0 + j) < dHeight )
              {
                float & value = destination[(iXd0 + i) + (iYd0 + j)*dWidth];
                value = (zd > value) ? zd : value;
              }
            }
          }
        }
      }      
    }
  }

	timer->StopTimer();
	printf(">> Warp (%d,%d): %f s\n",this->ZBufferSize[0],this->ZBufferSize[1],timer->GetElapsedTime());
	timer->Delete();
	warp->Delete();

  return array->GetPointer(0); 
}


// Print the vtkOpenGLVolumeShearWarpMapper
void vtkOpenGLVolumeShearWarpMapper::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkVolumeShearWarpMapper::PrintSelf(os,indent);
}

