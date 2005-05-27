/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkVolumeShearWarpMapper.cxx,v $
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













#include "vtkVolumeShearWarpMapper.h"
#include "vtkVolume.h"
#include "vtkRenderer.h"
#include "vtkFiniteDifferenceGradientEstimator.h"
#include "vtkRenderWindow.h"
#include "vtkGraphicsFactory.h"
#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "vtkTimerLog.h"
#include "vtkPlaneCollection.h"
#include "vtkMapper.h"

#include "vtkOpenGLVolumeShearWarpMapper.h"
#include "vtkProperty.h"

#define vtkVSWMultiplyPointMacro( A, B, M ) \
  B[0] = A[0]*M[0]  + A[1]*M[1]  + A[2]*M[2]  + M[3]; \
  B[1] = A[0]*M[4]  + A[1]*M[5]  + A[2]*M[6]  + M[7]; \
  B[2] = A[0]*M[8]  + A[1]*M[9]  + A[2]*M[10] + M[11]; \
  B[3] = A[0]*M[12] + A[1]*M[13] + A[2]*M[14] + M[15]; \
  if ( B[3] != 1.0 ) { B[0] /= B[3]; B[1] /= B[3]; B[2] /= B[3]; }

// Factor the view matrix into shear and warp
void vtkVolumeShearWarpMapper::FactorViewMatrix()
{
  ComputeViewportMatrix();
  ComputeViewMatrix();

  if (this->ParallelProjection)
    ComputePrincipalAxisParallel();
  else
    ComputePrincipalAxisPerspective();
    
  ComputePermutationMatrix();
  
  if (this->ParallelProjection) 
    ComputeShearMatrixParallel();
  else
    ComputeShearMatrixPerspective();
    
  ComputeWarpMatrix();
}


// Compute the view matrix for parallel projection
void vtkVolumeShearWarpMapper::ComputeViewMatrix()
{
  vtkTransform *view = vtkTransform::New();
  view->SetMatrix(VoxelsToViewMatrix);
  view->Inverse();  

  WorldViewingDirection[0] = 0.0;
  WorldViewingDirection[1] = 0.0;
  WorldViewingDirection[2] = 1.0;
  WorldViewingDirection[3] = 0.0;

  WorldEyePosition[0] = 0.0;
  WorldEyePosition[1] = 0.0;
  WorldEyePosition[2] = -1.0;
  WorldEyePosition[3] = 0.0;

  // Compute viewing direction in object space (for parallel projection)
  view->MultiplyPoint(WorldViewingDirection,ObjectViewingDirection);

  // Compute eye position in object space (for perspective projection)
  view->MultiplyPoint(WorldEyePosition,ObjectEyePosition);
  
}

// Compute the viewport matrix
void vtkVolumeShearWarpMapper::ComputeViewportMatrix()
{
  ViewportMatrix->Identity();
  ViewportMatrix->Element[0][0] = 0.5 * (double) ImageViewportSize[0];
  ViewportMatrix->Element[0][3] = 0.5 * (double) ImageViewportSize[0];
  ViewportMatrix->Element[1][1] = 0.5 * (double) ImageViewportSize[1];
  ViewportMatrix->Element[1][3] = 0.5 * (double) ImageViewportSize[1];
}




// Compute the principal viewing axis for parallel projection
void vtkVolumeShearWarpMapper::ComputePrincipalAxisParallel()
{
  double x = fabs(ObjectViewingDirection[0]);
  double y = fabs(ObjectViewingDirection[1]);
  double z = fabs(ObjectViewingDirection[2]);
  
  if (x >= y)
  {
    if (x >= z)
      this->MajorAxis = VTK_X_AXIS;
    else
      this->MajorAxis = VTK_Z_AXIS;
  }
  else
  {
    if (y >= z)
      this->MajorAxis = VTK_Y_AXIS;
    else
      this->MajorAxis = VTK_Z_AXIS;

  }

  if (ObjectViewingDirection[this->MajorAxis] > 0)
    this->ReverseOrder = 0;
  else
    this->ReverseOrder = 1;
}

// Compute the principal viewing axis for perspective projection
void vtkVolumeShearWarpMapper::ComputePrincipalAxisPerspective()
{
  double vertex[3];
  double distance[3];
  double eye[3];
  double ax, ay, az;
  double maximumDistance;
  int maximumCount;
  int count[3];

  int order[3];
  int axis[8];
  int i;

  eye[0] = ObjectEyePosition[0] / ObjectEyePosition[3];
  eye[1] = ObjectEyePosition[1] / ObjectEyePosition[3];
  eye[2] = ObjectEyePosition[2] / ObjectEyePosition[3];
  
  // Find principal axes:
  for (i=0; i<8; i++)
  {
    // Generate volume corners:
    vertex[0] = -0.5 + (double)(i % 2);
    vertex[1] = -0.5 + (double)((i/2) % 2);
    vertex[2] = -0.5 + (double)((i/4) % 2);

    vertex[0] *= this->GetInput()->GetDimensions()[0];
    vertex[1] *= this->GetInput()->GetDimensions()[1];
    vertex[2] *= this->GetInput()->GetDimensions()[2];

    distance[0] = vertex[0] - eye[0];
    distance[1] = vertex[1] - eye[1];
    distance[2] = vertex[2] - eye[2];

    // Determine the principal viewing axis and the stacking order:
    ax = fabs(distance[0]);
    ay = fabs(distance[1]);
    az = fabs(distance[2]);

    maximumDistance = ax;

    if (ay > maximumDistance)
      maximumDistance = ay;

    if (az > maximumDistance)
      maximumDistance = az;

    if (maximumDistance == ax)
    {
      axis[i] = VTK_X_AXIS;
      order[0] = (distance[0] > 0.0) ? 0 : 1;
    }
    else if (maximumDistance == ay)
    {
      axis[i] = VTK_Y_AXIS;
      order[1] = (distance[1] > 0.0) ? 0 : 1;

    }
    else
    {
      axis[i] = VTK_Z_AXIS;
      order[2] = (distance[2] > 0.0) ? 0 : 1;
    }
  }

  // Find the dominating principal axis:
  for (i=0; i<3; i++)
	{
    count[i] = 0;
	}
    
  for (i=0; i<8; i++)
  {
    switch (axis[i])
    {
      case VTK_X_AXIS:
        count[0]++;
        break;

      case VTK_Y_AXIS:
        count[1]++;
        break;
        
      case VTK_Z_AXIS:
        count[2]++;
        break;

      default:
        break;
    }
  }

  // Assign the dominant axis for the principal axis (favor the Z axis for ties):
  maximumCount = count[2];

  if (count[1] > maximumCount)
    maximumCount = count[1];

  if (count[0] > maximumCount)
    maximumCount = count[0];

  if (maximumCount == count[2])
  {
    this->MajorAxis = VTK_Z_AXIS;
    this->ReverseOrder = order[2];
  }
  else if (maximumCount==count[1])
  {
    this->MajorAxis = VTK_Y_AXIS;
    this->ReverseOrder = order[1];
  }
  else
  {
    this->MajorAxis = VTK_X_AXIS;
    this->ReverseOrder = order[0];
  }
}


// Compute the permutation matrix (transformation from object space to standard object space)
void vtkVolumeShearWarpMapper::ComputePermutationMatrix()
{
  PermutationMatrix->Zero();

  int size[3];
  GetInput()->GetDimensions(size);

  switch (this->MajorAxis)
  {
    case VTK_X_AXIS:
      PermutationMatrix->Element[0][1] = 1.0f;
      PermutationMatrix->Element[1][2] = 1.0f;
      PermutationMatrix->Element[2][0] = 1.0f;
      PermutationMatrix->Element[3][3] = 1.0f;

      this->CountI = int(float(size[1]));// / this->ImageSampleDistance);
      this->CountJ = int(float(size[2]));// / this->ImageSampleDistance);
      this->CountK = int(float(size[0]));// / this->ImageSampleDistance);
      break;

    case VTK_Y_AXIS:
      PermutationMatrix->Element[0][2] = 1.0f;




      PermutationMatrix->Element[1][0] = 1.0f;
      PermutationMatrix->Element[2][1] = 1.0f;
      PermutationMatrix->Element[3][3] = 1.0f;

      this->CountI = int(float(size[2]));// / this->ImageSampleDistance);
      this->CountJ = int(float(size[0]));// / this->ImageSampleDistance);
      this->CountK = int(float(size[1]));// / this->ImageSampleDistance);
      break;

    case VTK_Z_AXIS:
    default:
      PermutationMatrix->Element[0][0] = 1.0f;
      PermutationMatrix->Element[1][1] = 1.0f;
      PermutationMatrix->Element[2][2] = 1.0f;

      PermutationMatrix->Element[3][3] = 1.0f;

      this->CountI = int(float(size[0]));// / this->ImageSampleDistance);
      this->CountJ = int(float(size[1]));// / this->ImageSampleDistance);
      this->CountK = int(float(size[2]));// / this->ImageSampleDistance);
      break;
  }

  this->MaximumIntermediateDimension = size[0];

  if (size[1] > this->MaximumIntermediateDimension)
    this->MaximumIntermediateDimension = size[1];


  if (size[2] > this->MaximumIntermediateDimension)
    this->MaximumIntermediateDimension = size[2];

  this->MaximumIntermediateDimension *= 2;

  // Compute the viewing direction in standard object space (for parallel projection)
  this->PermutationMatrix->MultiplyPoint(ObjectViewingDirection,StandardViewingDirection);

  // Compute the eye position in standard object space (for perspective projection)
  this->PermutationMatrix->MultiplyPoint(ObjectEyePosition,StandardEyePosition);



  // Compute the permuted view to voxel matrix
  vtkMatrix4x4::Multiply4x4(this->PermutationMatrix,this->ViewToVoxelsMatrix,this->PermutedViewToVoxelsMatrix);

  // Compute the permuted voxel to view matrix
  vtkMatrix4x4::Multiply4x4(this->PermutationMatrix,this->VoxelsToViewMatrix,this->PermutedVoxelsToViewMatrix);

  
  // Get depth cueing factors
  /*
  this->DepthI = this->PermutedVoxelsToViewMatrix->Element[2][0];
  this->DepthJ = this->PermutedVoxelsToViewMatrix->Element[2][1];
  this->DepthK = this->PermutedVoxelsToViewMatrix->Element[2][2];  
  this->Depth0 = this->PermutedVoxelsToViewMatrix->Element[2][3];
  */

}

// Compute the shear matrix (transformation from object to intermediate image space)
void vtkVolumeShearWarpMapper::ComputeShearMatrixParallel()
{
  vtkMatrix4x4 *conv = vtkMatrix4x4::New();  // conversion to intermediate image coordinate system
  vtkMatrix4x4 *shear = vtkMatrix4x4::New();  // shear standard object space to intermediate image space

  // Compute shear factors:
  this->ShearI = - StandardViewingDirection[0] / StandardViewingDirection[2];
  this->ShearJ = - StandardViewingDirection[1] / StandardViewingDirection[2];
  this->Scale  = 1.0f;
  
  /* compute the intermediate image size */
  this->IntermediateWidth = this->CountI + 1 + (int) ceil((this->CountK-1)*fabs(this->ShearI));
  this->IntermediateHeight = this->CountJ + 1 + (int) ceil((this->CountK-1)*fabs(this->ShearJ));

  /* compute the translation coefficients */
  if (this->ShearI >= 0.0)
    this->TranslationI = 1.0;
  else
    this->TranslationI = 1.0 - this->ShearI * (double) (this->CountK - 1);

  if (this->ShearJ >= 0.0)
    this->TranslationJ = 1.0;
  else
    this->TranslationJ = 1.0 - this->ShearJ * (double) (this->CountK - 1);

  // Assemble standard object space shear matrix from shear factors
  shear->Identity();
  shear->Element[0][2] = this->ShearI;
  shear->Element[1][2] = this->ShearJ;
	
  // Add scale factor depending on object size:
//  shear->Scale((double) size[0] / (double) this->CountI, (double) size[1] / (double)  this->CountJ, (double) size[2] / (double) this->CountK);

  // Create conversion matrix for intermediate image coordinates
  conv->Identity();
  conv->Element[0][3] = 0.5 * (double) this->IntermediateWidth;
  conv->Element[1][3] = 0.5 * (double) this->IntermediateHeight;

  vtkTransform *shearTransform = vtkTransform::New();
  shearTransform->SetMatrix(this->PermutationMatrix);
  shearTransform->PostMultiply();
  shearTransform->Concatenate(shear);
  shearTransform->Concatenate(conv);
  this->ShearMatrix->DeepCopy(shearTransform->GetMatrix());
  shearTransform->Delete();

  shear->Delete();
  conv->Delete();

}

// Compute the shear matrix (transformation from object to intermediate image space)
void vtkVolumeShearWarpMapper::ComputeShearMatrixPerspective()
{
	if (StandardEyePosition[2] == 0.0)
		StandardEyePosition[2] = -this->CountK;

  vtkMatrix4x4 *conv = vtkMatrix4x4::New();  // conversion to intermediate image coordinate system
  vtkMatrix4x4 *shear = vtkMatrix4x4::New();  // shear standard object space to intermediate image space
  vtkTransform *scale = vtkTransform::New();

  // Compute shear factors:
  this->ShearI =  - StandardEyePosition[0] / StandardEyePosition[2];
  this->ShearJ =  - StandardEyePosition[1] / StandardEyePosition[2];
  this->Scale  =  - StandardEyePosition[3] / StandardEyePosition[2];

//	this->Scale *= this->ImageSampleDistance;

	float factor = 1.0;
  if (this->ReverseOrder)
    factor = 1.0f + this->Scale * (this->CountK - 1);

  scale->Identity();
	scale->Scale(factor,factor,1.0);

  // Assemble standard object space shear matrix from shear factors
  shear->Identity();
	
	shear->Element[0][2] = this->ShearI;
  shear->Element[1][2] = this->ShearJ;
  shear->Element[3][2] = this->Scale;

	// Create conversion matrix for intermediate image coordinates
	/*
  conv->Identity();
  conv->Element[0][3] = 0.5 * (double) this->IntermediateWidth;
  conv->Element[1][3] = 0.5 * (double) this->IntermediateHeight;
	*/

  vtkTransform *shearTransform = vtkTransform::New();
	shearTransform->Identity();
  shearTransform->PostMultiply();
  shearTransform->Concatenate(shear);
  shearTransform->Concatenate(scale);
	this->ShearMatrix->DeepCopy(shearTransform->GetMatrix());

	float min[2];
	float max[2];

	this->ComputeIntermediateImageExtent(min,max);

	this->IntermediateWidth = (int)ceilf(max[0] - min[0]);
	this->IntermediateHeight = (int)ceilf(max[1] - min[1]);

	this->IntermediateWidth = this->IntermediateWidth  / this->ImageSampleDistance + 1;
	this->IntermediateHeight =this->IntermediateHeight / this->ImageSampleDistance + 1;

	this->TranslationI = -min[0];
	this->TranslationJ = -min[1];

  shearTransform->Delete();
  shear->Delete();
  scale->Delete();
  conv->Delete();
}

// Compute a the two-dimensional warp matrix
void vtkVolumeShearWarpMapper::ComputeWarpMatrix()
{
	vtkTransform *shear = vtkTransform::New();
	shear->SetMatrix(this->PermutationMatrix);
	shear->PostMultiply();
	shear->Concatenate(this->ShearMatrix);
	shear->Inverse();


  vtkTransform *warp = vtkTransform::New();
	warp->SetMatrix(shear->GetMatrix());
	warp->PostMultiply();
	warp->Concatenate(this->VoxelsToViewMatrix);

	this->WarpMatrix->DeepCopy(warp->GetMatrix());

	warp->Delete();
	shear->Delete();
}

/*
// Compute a lookup table used for depth cueing
void vtkVolumeShearWarpMapper::ComputeDepthTable(int first, int last)
{
  for (int i = first; i <= last; i++)
    this->DepthTable[i] = this->FrontDepth * exp(-this->DepthDensity*(1.0 - i*this->DeltaDepth));

  this->DepthTableSize = last - first;    
}


// Perfrom second step for fast depth cueing (multiply intermediate image by second depth factor)
void vtkVolumeShearWarpMapper::DepthCueImage (vtkShearWarpPixelData *im, int slice)
{

  double depthQuant = 1.0 / this->DeltaDepth;

  float pixelDepth;
  int pixelDepthInteger;

  float uSlice = this->ShearI * slice + this->TranslationI;
  float vSlice = this->ShearJ * slice + this->TranslationJ;
  float leftDepth = this->Depth0 + this->DepthK*slice - uSlice*this->DepthI - vSlice*this->DepthJ;

  if (this->DepthI > 0)
  {
    if (this->DepthJ > 0)
      max_depth = left_depth + this->DepthI * width + this->DepthJ * height;
    else
      max_depth = left_depth + this->DepthI * width;
  }
  else
  {
    if (depth_dj > 0)
      max_depth = left_depth + this->DepthJ * height;


    else

      max_depth = left_depth;
  }

  max_depth_int = max_depth * depth_quant;

  if (max_depth_int >= vpc->dc_table_len)
  {
    // Resize table
  }
    
  float di = this->DepthI * depthQuant;
  float dj = this->DepthJ * depthQuant;

  leftDepth *= depthQuant;

  for (int j = this->IntermediateHeight; j > 0; j--)
  {
    pixelDepth = leftDepth;
    leftDepth += dj;


    for (int i = this->IntermediateWidth; i > 0; i--)
    {

      pixelDepthInteger = pixelDepth;
      pixelDepth += di;

      if (pixelDepthInteger < 0)
        pixelDepthInteger = 0;
          
      if (pixelDepthInteger >= this->DepthTableSize)
      {
        // This shouldn't happen
        pixelDepthInteger = this->DepthTableSize - 1;
      }

      im->Red *= this->DepthTable[pixelDepthInteger];
      im->Green *= this->DepthTable[pixelDepthInteger];
      im->Blue *= this->DepthTable[pixelDepthInteger];
      im++;
    }
  }
}
*/

// Simple parallel projection shear-warp without runlength encoded volume using bilinear interpolation
template <class T>

static void CompositeIntermediateNearestSimple(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis)
{
  vtkShearWarpPixelData *pixels;

  float sampledRed,sampledGreen,sampledBlue,sampledOpacity;
  float sampledValue;
  float sampledGradientMagnitude;
  float oldRed,oldGreen,oldBlue,oldOpacity;
  float newRed,newGreen,newBlue,newOpacity;
  float redDiffuse,greenDiffuse,blueDiffuse;
  float redSpecular,greenSpecular,blueSpecular;
  float gradientOpacity;
  float isoRed = 0.0;
  float isoGreen = 0.0;
  float isoBlue = 0.0;

  int skipped;
  int i,j,k;
  int vi,vj,vk;

  float *SOTF;
  float *CTF;
  float *GTF;
  float *GOTF;
  float gradientOpacityConstant;
  int gradientOpacityIsConstant;
  unsigned short encodedNormal;
  unsigned char gradientMagnitude;

  float uSlice;
  float vSlice;

  int uSliceInteger;
  int vSliceInteger;

  T value;

  int kStart;
  int kEnd;
  int kIncrement;
  int vkStart;

  int viIncrement;
  int vjIncrement;
  int vkIncrement;

  T *dptr = (T*) myThis->GetInput()->GetScalarPointer();

  unsigned short *nptr = myThis->GradientEstimator->GetEncodedNormals();
  unsigned char *gptr = myThis->GradientEstimator->GetGradientMagnitudes();

  int *dimensions = myThis->GetInput()->GetDimensions();
  int location;
  int plane = dimensions[0]*dimensions[1];
  int halfDistance = myThis->ImageSampleDistance / 2;

  int functionType;


  if (myThis->ReverseOrder)
  {
    kStart = myThis->CountK - 1;
    kEnd = -1 + halfDistance;
    kIncrement = -1;
  }
  else
  {
    kStart = 0;
    kEnd = myThis->CountK - halfDistance;
    kIncrement = 1;
  }

  SOTF =  myThis->Volume->GetCorrectedScalarOpacityArray();
  CTF  =  myThis->Volume->GetRGBArray();
  GTF  =  myThis->Volume->GetGrayArray();
  GOTF =  myThis->Volume->GetGradientOpacityArray();
  gradientOpacityConstant = myThis->Volume->GetGradientOpacityConstant();


  if (gradientOpacityConstant > 0.0f)
    gradientOpacityIsConstant = 1;
  else
    gradientOpacityIsConstant = 0;


  if (myThis->FunctionType == VTK_SHEAR_WARP_ISOSURFACE_FUNCTION)
  {
    isoRed = CTF[int(myThis->IsoValue)*3 + 0];
    isoGreen = CTF[int(myThis->IsoValue)*3 + 1];
    isoBlue = CTF[int(myThis->IsoValue)*3 + 2];
  }

  switch (myThis->MajorAxis)
  {
    case VTK_X_AXIS:
      viIncrement = dimensions[0] * myThis->ImageSampleDistance;
      vjIncrement = plane * myThis->ImageSampleDistance;
      vkIncrement = kIncrement * myThis->ImageSampleDistance;
      vkStart = kStart * myThis->ImageSampleDistance;
      break;

    case VTK_Y_AXIS:
      viIncrement = plane * myThis->ImageSampleDistance;
      vjIncrement = myThis->ImageSampleDistance;
      vkIncrement = kIncrement * dimensions[0] * myThis->ImageSampleDistance;
      vkStart = kStart * dimensions[0] * myThis->ImageSampleDistance;
      break;

    case VTK_Z_AXIS:                       
      default:
      viIncrement = myThis->ImageSampleDistance;
      vjIncrement = dimensions[0] * myThis->ImageSampleDistance;
      vkIncrement = kIncrement * plane * myThis->ImageSampleDistance;
      vkStart = kStart * plane * myThis->ImageSampleDistance;

      break;
  }
 
  for (k = kStart, vk = vkStart; k != kEnd; k += kIncrement, vk += vkIncrement)
  {
    uSlice = k*myThis->ShearI + myThis->TranslationI;
    vSlice = k*myThis->ShearJ + myThis->TranslationJ;
   
    uSliceInteger = (int) ceil(uSlice) - 1;
    vSliceInteger = (int) ceil(vSlice) - 1;


    // Composite one slice into the intermediate image
    for (j=0, vj = halfDistance; j < myThis->CountJ-halfDistance; j++, vj += vjIncrement)
    {
      image->Position(pixels,uSliceInteger + (vSliceInteger+j)*myThis->IntermediateWidth);

      for (i=0, vi = halfDistance; i < myThis->CountI-halfDistance; )
      {
        // Skip opaque pixels in intermediate image
        skipped = image->Skip(pixels);

        // Update both runs if to be aligned with intermediate pixels
        if (skipped > 0)

        {
          i += skipped;
          vi += viIncrement * skipped;
        }
        else
        {
          if (myThis->IntermixIntersectingGeometry)
          {
            float depth = myThis->IntermediateZBuffer[myThis->ImageSampleDistance * (uSliceInteger + i) + myThis->ImageSampleDistance * (vSliceInteger + j) * myThis->IntermediateWidth * myThis->ImageSampleDistance];
             
            if (myThis->ReverseOrder)
            {
              if (k*myThis->ImageSampleDistance <= depth)
                pixels->Offset = 1;
            }
            else
            {
              if (k*myThis->ImageSampleDistance >= depth)
                pixels->Offset = 1;
            }

          }
          // Only process non-opaque pixels
          if (pixels->Offset == 0)
          {
//            if (myThis->IsVoxelClipped(i*myThis->ImageSampleDistance,j*myThis->ImageSampleDistance,k*myThis->ImageSampleDistance) == 1)
            if (myThis->IsVoxelClipped(myThis->ImageSampleDistance * (uSliceInteger + i), myThis->ImageSampleDistance * (vSliceInteger + j),k * myThis->ImageSampleDistance) == 1)
            {
              if (myThis->ClippingMode == VTK_SHEAR_WARP_MIP_FUNCTION)
              {
                functionType = VTK_SHEAR_WARP_MIP_FUNCTION;
              }
              else
              {
                image->Advance(pixels,1);
                i++;
                vi += viIncrement;
                continue;
              }
            }
            else
            {
              functionType = myThis->FunctionType;
            }

						oldOpacity = pixels->Opacity[pixels->Region];
            oldRed = pixels->Red[pixels->Region];
            oldGreen = pixels->Green[pixels->Region];
            oldBlue = pixels->Blue[pixels->Region];

            location = vi + vj + vk;

            if (functionType == VTK_SHEAR_WARP_COMPOSITE_FUNCTION)
            {
							sampledOpacity = 0.0f;
							sampledRed = 0.0f;
							sampledGreen = 0.0f;
							sampledBlue = 0.0f;

							value = dptr[location];
							sampledOpacity += SOTF[value];

							sampledRed += CTF[value*3 + 0];
							sampledGreen += CTF[value*3 + 1];
							sampledBlue += CTF[value*3 + 2];

							if (myThis->Shade)
							{
								redDiffuse = 0.0f;
								redSpecular = 0.0f;
								blueDiffuse = 0.0f;
								blueSpecular = 0.0f;
								greenDiffuse = 0.0f;
								greenSpecular = 0.0f;
								sampledGradientMagnitude = 0.0f;
								gradientOpacity = gradientOpacityConstant;

								encodedNormal = nptr[location];

								redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal];
								redSpecular += myThis->RedSpecularShadingTable[encodedNormal];
								greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal];
								greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal];
								blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal];
								blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal];

								if (!gradientOpacityIsConstant)
								{
									gradientMagnitude = gptr[location];
									sampledGradientMagnitude += float(gradientMagnitude);

									if (sampledGradientMagnitude > 255.0f)
										gradientOpacity = GOTF[255];
									else if (sampledGradientMagnitude < 0.0f)
										gradientOpacity = GOTF[0];
									else
										gradientOpacity = GOTF[(unsigned char) sampledGradientMagnitude];
								}

								sampledRed *= redDiffuse + redSpecular;
								sampledGreen *= greenDiffuse + greenSpecular;
								sampledBlue *= blueDiffuse + blueSpecular;
								sampledOpacity *= gradientOpacity;
							}

							// Alpha Compositing

							newRed = oldRed + sampledOpacity * sampledRed * (1.0f - oldOpacity);
							newGreen = oldGreen + sampledOpacity * sampledGreen * (1.0f - oldOpacity);
							newBlue = oldBlue + sampledOpacity * sampledBlue * (1.0f - oldOpacity);
							newOpacity = oldOpacity + sampledOpacity * (1.0f - oldOpacity);
            }
            else if (functionType == VTK_SHEAR_WARP_MIP_FUNCTION)
            {
              sampledValue = 0.0f;

              value = dptr[location];
              sampledValue = float(value);
              sampledOpacity = SOTF[int(sampledValue)];

              // Maximum Intensity Projection
              if (sampledValue > pixels->Value[pixels->Region] && sampledOpacity > oldOpacity)
              {
                newRed = CTF[int(sampledValue)*3+0];
                newGreen = CTF[int(sampledValue)*3+1];
                newBlue = CTF[int(sampledValue)*3+2];
                newOpacity = sampledOpacity;
                pixels->Value[pixels->Region] = sampledValue;
              }
              else
              {
                newRed = oldRed;
                newGreen = oldGreen;
                newBlue = oldBlue;
                newOpacity = oldOpacity;
              }
            }
            else
            {
              sampledValue = 0.0f;

              value = dptr[location];
              sampledValue += float(value);

              if (pixels->Value[pixels->Region] == 0 && sampledValue >= myThis->IsoValue)
              {               
                sampledRed = isoRed;
                sampledGreen = isoGreen;
                sampledBlue = isoBlue;
 
                if (myThis->Shade)
                {
                  redDiffuse = 0.0f;
                  redSpecular = 0.0f;
                  blueDiffuse = 0.0f;
                  blueSpecular = 0.0f;
                  greenDiffuse = 0.0f;
                  greenSpecular = 0.0f;

                  encodedNormal = nptr[location];

                  redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal];

                  redSpecular += myThis->RedSpecularShadingTable[encodedNormal];
                  greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal];
                  greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal];

                  blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal];
                  blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal];

                  sampledRed *= redDiffuse + redSpecular;
                  sampledGreen *= greenDiffuse + greenSpecular;
                  sampledBlue *= blueDiffuse + blueSpecular;
                }

                newRed = sampledRed;
                newGreen = sampledGreen;
                newBlue = sampledBlue;
                newOpacity = 1.0f;
              }
              else
              {
                newRed = oldRed;
                newGreen = oldGreen;
                newBlue = oldBlue;
                newOpacity = oldOpacity;
              }
            }

            pixels->Red[pixels->Region] = newRed;
            pixels->Green[pixels->Region] = newGreen;
            pixels->Blue[pixels->Region] = newBlue;
            pixels->Opacity[pixels->Region] = newOpacity;            

            if (newOpacity >= 1.0f)
            {
              // The current intermediate pixel is opaque, so exit
              // loop and skip opaque pixels
              pixels->Offset = 1;

            }
            else
            {
              image->Advance(pixels,1);
              i++;
              vi += viIncrement;
            }

          }
        }
      }
    }
  }
}


// Simple parallel projection shear-warp without runlength encoded volume using bilinear interpolation
template <class T>
static void CompositeIntermediateLinearSimple(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis)
{
  vtkShearWarpPixelData *pixels = 0;

  float sampledRed,sampledGreen,sampledBlue,sampledOpacity;
  float sampledValue;
  float sampledGradientMagnitude;
  float oldRed,oldGreen,oldBlue,oldOpacity;
  float newRed,newGreen,newBlue,newOpacity;
  float redDiffuse,greenDiffuse,blueDiffuse;
  float redSpecular,greenSpecular,blueSpecular;
  float gradientOpacity;

  int skipped;

  int i,j,k;
  int vi,vj,vk;

  float *SOTF[2];
  float *CTF[2];
  float *GOTF[2];
  float gradientOpacityConstant[2];
  int gradientOpacityIsConstant[2];
	int shade[2];
	float isoRed[2];
	float isoGreen[2];
	float isoBlue[2];

  unsigned short encodedNormal;
  unsigned char gradientMagnitude;

  float uSlice;
  float vSlice;

  int uSliceInteger;
  int vSliceInteger;

  float uSliceFractional;
  float vSliceFractional;


  float weightTopLeft;
  float weightBottomLeft;
  float weightTopRight;
  float weightBottomRight;

  float adjustedTL;
  float adjustedTR;
  float adjustedBL;
  float adjustedBR;

  T value;

  int kStart;
  int vkStart;
  int kEnd;
  int kIncrement;
  int viIncrement;  
  int vjIncrement;
  int vkIncrement;

  T *dptr = (T*) myThis->GetInput()->GetScalarPointer();

  unsigned short *nptr = myThis->GradientEstimator->GetEncodedNormals();
  unsigned char *gptr = myThis->GradientEstimator->GetGradientMagnitudes();

  int *dimensions = myThis->GetInput()->GetDimensions();
  int locationTL,locationTR,locationBL,locationBR;
  int plane = dimensions[0]*dimensions[1];

  int functionType;
	int clipped;


  if (myThis->ReverseOrder)
  {
    kStart = myThis->CountK - 1;
    kEnd = -1;
    kIncrement = -1;
  }
  else
  {
    kStart = 0;
    kEnd = myThis->CountK;
    kIncrement = 1;
  }

	SOTF[1] =  myThis->ClippingVolume->GetCorrectedScalarOpacityArray();
  CTF[1]  =  myThis->ClippingVolume->GetRGBArray();
	GOTF[1] =  myThis->ClippingVolume->GetGradientOpacityArray();
	gradientOpacityConstant[1] = myThis->ClippingVolume->GetGradientOpacityConstant();
	gradientOpacityIsConstant[1] = (gradientOpacityConstant[1] > 0.0f) ? 1 : 0;
	shade[1] = myThis->ClippingProperty->GetShade();

	SOTF[0] =  myThis->Volume->GetCorrectedScalarOpacityArray();
  CTF[0]  =  myThis->Volume->GetRGBArray();
  GOTF[0] =  myThis->Volume->GetGradientOpacityArray();
  gradientOpacityConstant[0] = myThis->Volume->GetGradientOpacityConstant();
	gradientOpacityIsConstant[0] = (gradientOpacityConstant[0] > 0.0f) ? 1 : 0;
	shade[0] = myThis->Shade;

  isoRed[1] = CTF[1][int(myThis->IsoValue)*3 + 0];
  isoGreen[1] = CTF[1][int(myThis->IsoValue)*3 + 1];
  isoBlue[1] = CTF[1][int(myThis->IsoValue)*3 + 2];

	isoRed[0] = CTF[0][int(myThis->IsoValue)*3 + 0];
	isoGreen[0] = CTF[0][int(myThis->IsoValue)*3 + 1];
	isoBlue[0] = CTF[0][int(myThis->IsoValue)*3 + 2];

	switch (myThis->MajorAxis)
  {
    case VTK_X_AXIS:
      viIncrement = dimensions[0] * myThis->ImageSampleDistance;

      vjIncrement = plane * myThis->ImageSampleDistance;


      vkIncrement = kIncrement * myThis->ImageSampleDistance;
      vkStart = kStart * myThis->ImageSampleDistance;
      break;

    case VTK_Y_AXIS:
      viIncrement = plane * myThis->ImageSampleDistance;
      vjIncrement = myThis->ImageSampleDistance;
      vkIncrement = kIncrement * dimensions[0] * myThis->ImageSampleDistance;
      vkStart = kStart * dimensions[0] * myThis->ImageSampleDistance;
      break;


    case VTK_Z_AXIS:
      default:

      viIncrement = myThis->ImageSampleDistance;
      vjIncrement = dimensions[0] * myThis->ImageSampleDistance;
      vkIncrement = kIncrement * plane * myThis->ImageSampleDistance;
      vkStart = kStart * plane * myThis->ImageSampleDistance;
      break;
  }
  


  for (k = kStart, vk = vkStart; k != kEnd; k += kIncrement, vk += vkIncrement)
  {
    uSlice = k*myThis->ShearI + myThis->TranslationI;
    vSlice = k*myThis->ShearJ + myThis->TranslationJ;

    uSliceInteger = (int)ceil(uSlice) - 1;
    vSliceInteger = (int)ceil(vSlice) - 1;


    uSliceFractional = uSlice - uSliceInteger;
    vSliceFractional = vSlice - vSliceInteger;

    weightTopLeft = uSliceFractional * vSliceFractional;
    weightBottomLeft = uSliceFractional * (1.0f - vSliceFractional);

    weightTopRight = (1.0f - uSliceFractional) * vSliceFractional;
    weightBottomRight = (1.0f - uSliceFractional) * (1.0f - vSliceFractional);

    // Composite one slice into the intermediate image
    for (j=0, vj = 0; j < myThis->CountJ; j++, vj += vjIncrement)
    {
      image->Position(pixels,uSliceInteger + (vSliceInteger+j)*myThis->IntermediateWidth);

      for (i=0, vi = 0; i < myThis->CountI; )
      {
        // Skip opaque pixels in intermediate image
        skipped = image->Skip(pixels);

        // Update both runs if to be aligned with intermediate pixels
        if (skipped > 0)
        {
          i += skipped;
          vi += viIncrement/*myThis->ImageSampleDistance*/ * skipped;
        }
        else
        {
          if (myThis->IntermixIntersectingGeometry && myThis->ImageSampleDistance == 1)
          {
            float depth = myThis->IntermediateZBuffer[myThis->ImageSampleDistance * (uSliceInteger + i) + myThis->ImageSampleDistance * (vSliceInteger + j) * myThis->IntermediateWidth * myThis->ImageSampleDistance];

            if (myThis->ReverseOrder)
            {
              if (k*myThis->ImageSampleDistance <= depth)
                pixels->Offset = 1;
            }
            else
            {
              if (k*myThis->ImageSampleDistance >= depth)
                pixels->Offset = 1;
            }
          }

          // Only process non-opaque pixels
          if (pixels->Offset == 0)
          {
						clipped = myThis->IsVoxelClipped(myThis->ImageSampleDistance * (uSliceInteger + i), myThis->ImageSampleDistance * (vSliceInteger + j),k * myThis->ImageSampleDistance);
//            if (myThis->IsVoxelClipped(i*myThis->ImageSampleDistance,j*myThis->ImageSampleDistance,k*myThis->ImageSampleDistance) == 1)

						if (pixels->State != clipped)
						{
							pixels->Region++;
							pixels->State = clipped;
						}

						if (clipped)
            {
							if (myThis->ClippingMode == VTK_SHEAR_WARP_TRANSPARENT_CLIPPING)
              {
                image->Advance(pixels,1);
                i++;
                vi += viIncrement;
                continue;
              }
							else
							{
								functionType = myThis->ClippingMode;
							}
            }
            else
            {
              functionType = myThis->FunctionType;
            }

            oldOpacity = pixels->Opacity[pixels->Region];
            oldRed = pixels->Red[pixels->Region];
            oldGreen = pixels->Green[pixels->Region];
            oldBlue = pixels->Blue[pixels->Region];

            locationTL = vi + vj + vk;
            locationTR = locationTL + viIncrement;
            locationBL = locationTL + vjIncrement;
            locationBR = locationBL + viIncrement;

            if (functionType == VTK_SHEAR_WARP_COMPOSITE_FUNCTION)
            {
              sampledOpacity = 0.0f;
              sampledRed = 0.0f;
              sampledGreen = 0.0f;
              sampledBlue = 0.0f;

              value = dptr[locationTL];
              sampledOpacity += SOTF[clipped][value] * weightTopLeft;
              sampledRed += CTF[clipped][value*3 + 0] * weightTopLeft;
              sampledGreen += CTF[clipped][value*3 + 1] * weightTopLeft;
              sampledBlue += CTF[clipped][value*3 + 2] * weightTopLeft;
              
              if (i + 1 < myThis->CountI)
              {
                value = dptr[locationTR];
                sampledOpacity += SOTF[clipped][value] * weightTopRight;
                sampledRed += CTF[clipped][value*3 + 0] * weightTopRight;
                sampledGreen += CTF[clipped][value*3 + 1] * weightTopRight;
                sampledBlue += CTF[clipped][value*3 + 2] * weightTopRight;
              }

              if (j + 1 < myThis->CountJ)
              {
                value = dptr[locationBL];
                sampledOpacity += SOTF[clipped][value] * weightBottomLeft;
                sampledRed += CTF[clipped][value*3 + 0] * weightBottomLeft;
                sampledGreen += CTF[clipped][value*3 + 1] * weightBottomLeft;
                sampledBlue += CTF[clipped][value*3 + 2] * weightBottomLeft;

                if (i + 1 < myThis->CountI)
                {
                  value = dptr[locationBR];
                  sampledOpacity += SOTF[clipped][value] * weightBottomRight;
                  sampledRed += CTF[clipped][value*3 + 0] * weightBottomRight;
                  sampledGreen += CTF[clipped][value*3 + 1] * weightBottomRight;
                  sampledBlue += CTF[clipped][value*3 + 2] * weightBottomRight;
                }
              }

              if (shade)
              {

                redDiffuse = 0.0f;
                redSpecular = 0.0f;
                blueDiffuse = 0.0f;
                blueSpecular = 0.0f;
                greenDiffuse = 0.0f;
                greenSpecular = 0.0f;
                sampledGradientMagnitude = 0.0f;

                gradientOpacity = gradientOpacityConstant[clipped];
                encodedNormal = nptr[locationTL];

                redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightTopLeft;
                redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightTopLeft;
                greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightTopLeft;
                greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightTopLeft;
                blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightTopLeft;
                blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightTopLeft;


                if (!gradientOpacityIsConstant[clipped])
                {
                  gradientMagnitude = gptr[locationTL];
                  sampledGradientMagnitude += float(gradientMagnitude) * weightTopLeft;
                }

                if (i + 1 < myThis->CountI)
                {
                  encodedNormal = nptr[locationTR];

                  redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightTopRight;
                  redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightTopRight;
                  greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightTopRight;
                  greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightTopRight;
                  blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightTopRight;
                  blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightTopRight;

                  if (!gradientOpacityIsConstant[clipped])
                  {
                    gradientMagnitude = gptr[locationTR];
                    sampledGradientMagnitude += float(gradientMagnitude) * weightTopRight;
                  }
                }

                if (j + 1 < myThis->CountJ)
                {
                  encodedNormal = nptr[locationBL];

                  redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightBottomLeft;
                  redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightBottomLeft;
                  greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightBottomLeft;
                  greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightBottomLeft;
                  blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightBottomLeft;
                  blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightBottomLeft;


                  if (!gradientOpacityIsConstant[clipped])
                  {
                    gradientMagnitude = gptr[locationBL];
                    sampledGradientMagnitude += float(gradientMagnitude) * weightBottomLeft;
                  }

                  if (i + 1 < myThis->CountI)
                  {
                    encodedNormal = nptr[locationBR];

                    redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightBottomRight;
                    redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightBottomRight;
                    greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightBottomRight;
                    greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightBottomRight;
                    blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightBottomRight;
                    blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightBottomRight;

                    if (!gradientOpacityIsConstant[clipped])
                    {
                      gradientMagnitude = gptr[locationBR];
                      sampledGradientMagnitude += float(gradientMagnitude) * weightBottomRight;
                    }
                  }
                }

                if (!gradientOpacityIsConstant[clipped])
                {
                  if (sampledGradientMagnitude > 255.0f)
                    gradientOpacity = GOTF[clipped][255];
                  else if (sampledGradientMagnitude < 0.0f)
                    gradientOpacity = GOTF[clipped][0];
                  else
                    gradientOpacity = GOTF[clipped][(unsigned char) sampledGradientMagnitude];
                }

                sampledRed *= redDiffuse + redSpecular;
                sampledGreen *= greenDiffuse + greenSpecular;
                sampledBlue *= blueDiffuse + blueSpecular;
                sampledOpacity *= gradientOpacity;
              }

              // Alpha Compositing
              newRed = oldRed + sampledOpacity * sampledRed * (1.0f - oldOpacity);
              newGreen = oldGreen + sampledOpacity * sampledGreen * (1.0f - oldOpacity);
              newBlue = oldBlue + sampledOpacity * sampledBlue * (1.0f - oldOpacity);
              newOpacity = oldOpacity + sampledOpacity * (1.0f - oldOpacity);
            }
            else if (functionType == VTK_SHEAR_WARP_MIP_FUNCTION)
            {
              sampledValue = 0.0f;

              value = dptr[locationTL];
              sampledValue += float(value) * weightTopLeft;

              if (i + 1 < myThis->CountI)
              {
                value = dptr[locationTR];
                sampledValue += float(value) * weightTopRight;
              }

              if (j + 1 < myThis->CountJ)
              {
                value = dptr[locationBL];
                sampledValue += float(value) * weightBottomLeft;

                if (i + 1 < myThis->CountI)
                {
                  value = dptr[locationBR];
                  sampledValue += float(value) * weightBottomRight;
                }
              }

              sampledOpacity = SOTF[clipped][int(sampledValue)];

              // Maximum Intensity Projection
              if (sampledValue > pixels->Value[pixels->Region])// && sampledOpacity > oldOpacity)
              {

                newRed = CTF[clipped][int(sampledValue)*3+0] * sampledOpacity;
                newGreen = CTF[clipped][int(sampledValue)*3+1] * sampledOpacity;
                newBlue = CTF[clipped][int(sampledValue)*3+2] * sampledOpacity;
                newOpacity = sampledOpacity;
                pixels->Value[pixels->Region] = sampledValue;
              }
              else
              {
                newRed = oldRed;
                newGreen = oldGreen;
                newBlue = oldBlue;
                newOpacity = oldOpacity;
              }
            }
            else
            {
              sampledRed = isoRed[clipped];
              sampledGreen = isoGreen[clipped];
              sampledBlue = isoBlue[clipped];

              sampledValue = 0.0f;

              value = dptr[locationTL];
              sampledValue += float(value) * weightTopLeft;

              if (i + 1 < myThis->CountI)
              {
                value = dptr[locationTR];
                sampledValue += float(value) * weightTopRight;
              }

              if (j + 1 < myThis->CountJ)

              {
                value = dptr[locationBL];
                sampledValue += float(value) * weightBottomLeft;

                if (i + 1 < myThis->CountI)

                {
                  value = dptr[locationBR];
                  sampledValue += float(value) * weightBottomRight;
                }
              }

              // Isosurface
              if (sampledValue >= myThis->IsoValue)
              {              
                if (shade)
                {
                  redDiffuse = 0.0f;
                  redSpecular = 0.0f;
                  blueDiffuse = 0.0f;
                  blueSpecular = 0.0f;
                  greenDiffuse = 0.0f;
                  greenSpecular = 0.0f;

                  adjustedTL = weightTopLeft;
                  adjustedBL = weightBottomLeft;
                  adjustedTR = weightTopRight;
                  adjustedBR = weightBottomRight;

                  if (i + 1 >= myThis->CountI)
                  {
                    adjustedTL += adjustedTR;
                    adjustedBL += adjustedBR;
                  }

                  if (j + 1 >= myThis->CountJ)
                  {
                    adjustedTL += adjustedBL;
                    adjustedTR += adjustedBR;
                  }

                  encodedNormal = nptr[locationTL];

                  redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedTL;

                  redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedTL;
                  greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedTL;
                  greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedTL;
                  blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedTL;
                  blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedTL;

                  if (i + 1 < myThis->CountI)
                  {
                    encodedNormal = nptr[locationTR];

                    redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedTR;
                    redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedTR;
                    greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedTR;
                    greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedTR;
                    blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedTR;
                    blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedTR;
                  }

                  if (j + 1 < myThis->CountJ)
                  {
                    encodedNormal = nptr[locationBL];

                    redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedBL;
                    redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedBL;

                    greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedBL;
                    greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedBL;
                    blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedBL;
                    blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedBL;

                    if (i + 1 < myThis->CountI)
                    {
                      encodedNormal = nptr[locationBR];

                      redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedBR;
                      redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedBR;
                      greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedBR;
                      greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedBR;
                      blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedBR;
                      blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedBR;

                    }
                  }

                  sampledRed *= redDiffuse + redSpecular;
                  sampledGreen *= greenDiffuse + greenSpecular;
                  sampledBlue *= blueDiffuse + blueSpecular;
                  
                }


                newRed = sampledRed;
                newGreen = sampledGreen;
                newBlue = sampledBlue;
                newOpacity = 1.0f;
              }

              else
              {
                newRed = oldRed;
                newGreen = oldGreen;
                newBlue = oldBlue;
                newOpacity = oldOpacity;
              }
            }


            pixels->Red[pixels->Region] = newRed;
            pixels->Green[pixels->Region] = newGreen;
            pixels->Blue[pixels->Region] = newBlue;
            pixels->Opacity[pixels->Region] = newOpacity;

            if (newOpacity >= 1.0f && functionType != VTK_SHEAR_WARP_MIP_FUNCTION)
            {
              // The current intermediate pixel is opaque, so exit

              // loop and skip opaque pixels
              pixels->Offset = 1;

            }
            else
            {
              image->Advance(pixels,1);
              i++;
              vi += viIncrement;
            }
          }
        }
      }
    }
  }

	image->Combine();
}

// Lacroute's parallel projection shear-warp algorithm with runlength encoded volume using nearest neighbour interpolation
template <class T>
static void CompositeIntermediateNearestRLE(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis)
{
  vtkShearWarpRLESlice<T> *slice;
  vtkShearWarpRLERun<T> *top;
  vtkShearWarpPixelData *pixels;

  float sampledRed,sampledGreen,sampledBlue,sampledOpacity;
  float sampledValue;
  float sampledGradientMagnitude;
  float oldRed,oldGreen,oldBlue,oldOpacity;

  float newRed,newGreen,newBlue,newOpacity;
  float redDiffuse,greenDiffuse,blueDiffuse;
  float redSpecular,greenSpecular,blueSpecular;
  float gradientOpacity;

  float isoRed = 0.0;

  float isoGreen = 0.0;
  float isoBlue = 0.0;

  int topIndex;

  int skipped;
  int runLength;

  int i,j,k;
  int vj,vk;

  float *SOTF;
  float *CTF;
  float *GTF;
  float *GOTF;
  float gradientOpacityConstant;
  int gradientOpacityIsConstant;
  unsigned short encodedNormal;
  unsigned char gradientMagnitude;

  float uSlice;
  float vSlice;

  int uSliceInteger;
  int vSliceInteger;

  T value;

  int kStart;
  int kEnd;
  int kIncrement;
  int vkStart;
  int vkIncrement;

  int halfDistance = myThis->ImageSampleDistance / 2;

  int functionType;

  vtkShearWarpRLEVolume<T> *encodedVolume = dynamic_cast< vtkShearWarpRLEVolume<T> * > (myThis->EncodedVolume);

  if (myThis->ReverseOrder)
  {
    kStart = myThis->CountK - 1;
    kEnd = -1 + halfDistance;

    kIncrement = -1;

    vkStart = (myThis->CountK - 1) * myThis->ImageSampleDistance - halfDistance;
    vkIncrement = -myThis->ImageSampleDistance;
  }

  else
  {
    kStart = 0;
    kEnd = myThis->CountK - halfDistance;
    kIncrement = 1;


    vkStart = halfDistance;
    vkIncrement = myThis->ImageSampleDistance;
  }

  SOTF =  myThis->Volume->GetCorrectedScalarOpacityArray();
  CTF  =  myThis->Volume->GetRGBArray();
  GTF  =  myThis->Volume->GetGrayArray();
  GOTF =  myThis->Volume->GetGradientOpacityArray();
  gradientOpacityConstant = myThis->Volume->GetGradientOpacityConstant();

  if (gradientOpacityConstant > 0.0f)
    gradientOpacityIsConstant = 1;
  else
    gradientOpacityIsConstant = 0;


  if (myThis->FunctionType == VTK_SHEAR_WARP_ISOSURFACE_FUNCTION)
  {
    isoRed = CTF[int(myThis->IsoValue)*3 + 0];
    isoGreen = CTF[int(myThis->IsoValue)*3 + 1];
    isoBlue = CTF[int(myThis->IsoValue)*3 + 2];
  }

  for (k = kStart, vk = vkStart; k != kEnd; k += kIncrement, vk += vkIncrement)
  {
    uSlice = k*myThis->ShearI + myThis->TranslationI;
    vSlice = k*myThis->ShearJ + myThis->TranslationJ;


    uSliceInteger = (int)ceil(uSlice) - 1;
    vSliceInteger = (int)ceil(vSlice) - 1;

    slice = encodedVolume->GetSlice(myThis->MajorAxis,vk);

    // Composite one slice into the intermediate image
    for (j=0, vj = halfDistance; j < myThis->CountJ-halfDistance; j++, vj += myThis->ImageSampleDistance)
    {
      top = slice->GetLineRuns(vj);
      topIndex = halfDistance;

      while (topIndex >= top->Length)

      {
        topIndex -= top->Length;
        top++;
      }
        

      image->Position(pixels,uSliceInteger + (vSliceInteger+j)*myThis->IntermediateWidth);


      for (i=0; i < myThis->CountI; )
      {
        // Update run
        while (topIndex >= top->Length)
        {

          topIndex -= top->Length;
          top++;
        }

        // Skip opaque pixels in intermediate image
        skipped = image->Skip(pixels);

        // Update both runs if to be aligned with intermediate pixels
        if (skipped > 0)
        {
          i += skipped;
          topIndex += skipped * myThis->ImageSampleDistance;
        }
        else
        {
          runLength = top->Length - topIndex;

          // Skip transparent voxels


          if (top->VoxelData == NULL)
          {
            while (topIndex < top->Length)
            {
              image->Advance(pixels,1);
              i++;
              topIndex += myThis->ImageSampleDistance;
            }

          }
          else
          {

            // This loops samples voxels, performs shading and performs
            // compositing into the intermediate image
            while (topIndex < top->Length)//h=0; h < runLength; h+=myThis->ImageSampleDistance)
            {
              if (myThis->IntermixIntersectingGeometry)

              {
                float depth = myThis->IntermediateZBuffer[myThis->ImageSampleDistance * (uSliceInteger + i) + myThis->ImageSampleDistance * (vSliceInteger + j) * myThis->IntermediateWidth * myThis->ImageSampleDistance];

                if (myThis->ReverseOrder)
                {

                  if (vk <= depth)
                    pixels->Offset = 1;
                }
                else
                {

                  if (vk >= depth)
                    pixels->Offset = 1;
                }
              }
              
              // Only process non-opaque pixels
              if (pixels->Offset == 0)
              {
                if (myThis->IsVoxelClipped(i*myThis->ImageSampleDistance,vj,vk) == 1)
                {
                  if (myThis->ClippingMode == VTK_SHEAR_WARP_MIP_FUNCTION)
                  {
                    functionType = VTK_SHEAR_WARP_MIP_FUNCTION;
                  }
                  else
                  {
                    image->Advance(pixels,1);
                    i++;
                    topIndex+=myThis->ImageSampleDistance;
                    continue;
                  }
                }
                else
                {
                  functionType = myThis->FunctionType;
                }
                
                oldOpacity = pixels->Opacity[pixels->Region];
                oldRed = pixels->Red[pixels->Region];
                oldGreen = pixels->Green[pixels->Region];
                oldBlue = pixels->Blue[pixels->Region];

                if (functionType == VTK_SHEAR_WARP_COMPOSITE_FUNCTION)
                {
                  if (pixels->Value[pixels->Region] == 0)
                  {
                    value = top->VoxelData[topIndex].Value;
                    sampledOpacity = SOTF[value];
                    sampledRed = CTF[value*3 + 0];
                    sampledGreen = CTF[value*3 + 1];
                    sampledBlue = CTF[value*3 + 2];

                    if (myThis->Shade)
                    {
                      encodedNormal = top->VoxelData[topIndex].EncodedNormal;

                      redDiffuse = myThis->RedDiffuseShadingTable[encodedNormal];
                      redSpecular = myThis->RedSpecularShadingTable[encodedNormal];
                      greenDiffuse = myThis->GreenDiffuseShadingTable[encodedNormal];
                      greenSpecular = myThis->GreenSpecularShadingTable[encodedNormal];
                      blueDiffuse = myThis->BlueDiffuseShadingTable[encodedNormal];
                      blueSpecular = myThis->BlueSpecularShadingTable[encodedNormal];

                      if (!gradientOpacityIsConstant)
                      {
                        gradientMagnitude = top->VoxelData[topIndex].GradientMagnitude;
                        sampledGradientMagnitude = float(gradientMagnitude);
                      }

                      if (!gradientOpacityIsConstant)
                      {
                        if (sampledGradientMagnitude > 255.0f)
                          gradientOpacity = GOTF[255];
                        else if (sampledGradientMagnitude < 0.0f)
                          gradientOpacity = GOTF[0];
                        else
                          gradientOpacity = GOTF[(unsigned char) sampledGradientMagnitude];
                      }
                      else
                        gradientOpacity = gradientOpacityConstant;

                      sampledRed *= redDiffuse + redSpecular;
                      sampledGreen *= greenDiffuse + greenSpecular;
                      sampledBlue *= blueDiffuse + blueSpecular;
                      sampledOpacity *= gradientOpacity;
                    }

                    // Alpha Compositing
                    newRed = oldRed + sampledOpacity * sampledRed * (1.0f - oldOpacity);
                    newGreen = oldGreen + sampledOpacity * sampledGreen * (1.0f - oldOpacity);
                    newBlue = oldBlue + sampledOpacity * sampledBlue * (1.0f - oldOpacity);
                    newOpacity = oldOpacity + sampledOpacity * (1.0f - oldOpacity);
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }
                else if (functionType == VTK_SHEAR_WARP_MIP_FUNCTION)
                {
                  value = top->VoxelData[topIndex].Value;
                  sampledValue = value;
                  sampledOpacity = SOTF[int(sampledValue)];

                  // Maximum Intensity Projection
                  if (sampledValue > pixels->Value[pixels->Region] && sampledOpacity > oldOpacity)
                  {
                    newRed = CTF[int(sampledValue)*3+0];
                    newGreen = CTF[int(sampledValue)*3+1];
                    newBlue = CTF[int(sampledValue)*3+2];
                    newOpacity = sampledOpacity;
                    pixels->Value[pixels->Region] = sampledValue;
                  }

                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }
                else
                {
                  if (pixels->Value[pixels->Region] == 0)
                  {
                    sampledRed = isoRed;
                    sampledGreen = isoGreen;
                    sampledBlue = isoBlue;
                    sampledOpacity = 1.0f;

                    if (myThis->Shade)
                    {
                      encodedNormal = top->VoxelData[topIndex].EncodedNormal;

                      redDiffuse = myThis->RedDiffuseShadingTable[encodedNormal];
                      redSpecular = myThis->RedSpecularShadingTable[encodedNormal];
                      greenDiffuse = myThis->GreenDiffuseShadingTable[encodedNormal];
                      greenSpecular = myThis->GreenSpecularShadingTable[encodedNormal];
                      blueDiffuse = myThis->BlueDiffuseShadingTable[encodedNormal];
                      blueSpecular = myThis->BlueSpecularShadingTable[encodedNormal];

                      sampledRed *= redDiffuse + redSpecular;
                      sampledGreen *= greenDiffuse + greenSpecular;

                      sampledBlue *= blueDiffuse + blueSpecular;
                    }

                    newRed = sampledRed;
                    newGreen = sampledGreen;
                    newBlue = sampledBlue;
                    newOpacity = 1.0f;
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }

                pixels->Red[pixels->Region] = newRed;
                pixels->Green[pixels->Region] = newGreen;
                pixels->Blue[pixels->Region] = newBlue;
                pixels->Opacity[pixels->Region] = newOpacity;

                if (newOpacity >= 1.0f)
                {
                  // The current intermediate pixel is opaque, so exit
                  // loop and skip opaque pixels
                  pixels->Offset = 1;
                  break;
                }

                image->Advance(pixels,1);
                i++;
                topIndex+=myThis->ImageSampleDistance;
              }
              else
              {
                // The current pixel has an offset greater than zero, so
                // we exit the loop and skip all opaque pixels
                break;
              }
            }
          }
        }
      }
    }
  }
}


// Lacroute's parallel projection shear-warp algorithm with runlength encoded volume using bilinear interpolation
template <class T>
static void CompositeIntermediateLinearRLE(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis)
{
  vtkShearWarpRLESlice<T> *slice;
  vtkShearWarpRLERun<T> *top;
  vtkShearWarpRLERun<T> *bottom;
  vtkShearWarpPixelData *pixels;


  float sampledRed,sampledGreen,sampledBlue,sampledOpacity;
  float sampledValue;
  float sampledGradientMagnitude;
  float oldRed,oldGreen,oldBlue,oldOpacity;
  float newRed,newGreen,newBlue,newOpacity;
  float redDiffuse,greenDiffuse,blueDiffuse;
  float redSpecular,greenSpecular,blueSpecular;
  float gradientOpacity;

  float isoRed = 0.0;
  float isoGreen = 0.0;
  float isoBlue = 0.0;

  int topIndex;
  int bottomIndex;
  int topStart;
  int bottomStart;

  int skipped;
  int runLength;

  int i,j,k,h;
  int vj,vk;

  float *SOTF;
  float *CTF;
  float *GTF;
  float *GOTF;

  float gradientOpacityConstant;
  int gradientOpacityIsConstant;
  unsigned short encodedNormal;
  unsigned char gradientMagnitude;

  float uSlice;
  float vSlice;

  int uSliceInteger;
  int vSliceInteger;


  float uSliceFractional;
  float vSliceFractional;

  float weightTopLeft;
  float weightBottomLeft;
  float weightTopRight;
  float weightBottomRight;


  float adjustedTL;
  float adjustedTR;
  float adjustedBL;
  float adjustedBR;

  T value;

  int kStart;
  int kEnd;
  int kIncrement;
  int vkIncrement;

  int functionType;

  vtkShearWarpRLEVolume<T> *encodedVolume = dynamic_cast< vtkShearWarpRLEVolume<T> * > (myThis->EncodedVolume);

  if (myThis->ReverseOrder)
  {
    kStart = myThis->CountK - 1;
    kEnd = -1;
    kIncrement = -1;
    vkIncrement = -myThis->ImageSampleDistance;
  }
  else
  {
    kStart = 0;
    kEnd = myThis->CountK;
    kIncrement = 1;
    vkIncrement = myThis->ImageSampleDistance;
  }

  SOTF =  myThis->Volume->GetCorrectedScalarOpacityArray();
  CTF  =  myThis->Volume->GetRGBArray();
  GTF  =  myThis->Volume->GetGrayArray();
  GOTF =  myThis->Volume->GetGradientOpacityArray();
  gradientOpacityConstant = myThis->Volume->GetGradientOpacityConstant();

  if (gradientOpacityConstant > 0.0f)
    gradientOpacityIsConstant = 1;
  else
    gradientOpacityIsConstant = 0;


  if (myThis->FunctionType == VTK_SHEAR_WARP_ISOSURFACE_FUNCTION)
  {
    isoRed = CTF[int(myThis->IsoValue)*3 + 0];
    isoGreen = CTF[int(myThis->IsoValue)*3 + 1];
    isoBlue = CTF[int(myThis->IsoValue)*3 + 2];
  }

  for (k = kStart, vk = kStart*myThis->ImageSampleDistance; k != kEnd; k += kIncrement, vk += vkIncrement)
  {
    topIndex = 0;
    bottomIndex = 0;
    topStart = 0;
    bottomStart = 0;

    uSlice = k*myThis->ShearI + myThis->TranslationI;
    vSlice = k*myThis->ShearJ + myThis->TranslationJ;

    uSliceInteger = (int)ceil(uSlice) - 1;
    vSliceInteger = (int)ceil(vSlice) - 1;

    uSliceFractional = uSlice - uSliceInteger;
    vSliceFractional = vSlice - vSliceInteger;

    weightTopLeft = uSliceFractional * vSliceFractional;

    weightBottomLeft = uSliceFractional * (1.0f - vSliceFractional);
    weightTopRight = (1.0f - uSliceFractional) * vSliceFractional;
    weightBottomRight = (1.0f - uSliceFractional) * (1.0f - vSliceFractional);

    slice = encodedVolume->GetSlice(myThis->MajorAxis,vk);

    // Composite one slice into the intermediate image
    for (j=0, vj = 0; j < myThis->CountJ; j++, vj += myThis->ImageSampleDistance)
    {
      top = slice->GetLineRuns(vj);

      if ((j + 1) < myThis->CountJ)
        bottom = slice->GetLineRuns(vj+myThis->ImageSampleDistance);

      else
        bottom = NULL;

      topStart = 0;
      bottomStart = 0;

      topIndex = 0;
      bottomIndex = 0;

      image->Position(pixels,uSliceInteger + (vSliceInteger+j)*myThis->IntermediateWidth);

      for (i=0; i < myThis->CountI; )
      {

        // Update runs
        while (topIndex >= top->Length)
        {
          topIndex -= top->Length;
          topStart += top->Length;
          top++;
        }

        if (bottom != NULL)
        {
          while (bottomIndex >= bottom->Length)
          {
            bottomIndex -= bottom->Length;
            bottomStart += bottom->Length;
            bottom++;
          }
        }
        
        // Skip opaque pixels in intermediate image
        skipped = image->Skip(pixels);

        // Update both runs if to be aligned with intermediate pixels
        if (skipped > 0)
        {
          i += skipped;

          topIndex += skipped * myThis->ImageSampleDistance;
          bottomIndex += skipped * myThis->ImageSampleDistance;
        }
        else
        {
          if (bottom != NULL)
          {
            if ( ((topStart + top->Length) - (topStart + topIndex)) < ((bottomStart + bottom->Length) - (bottomStart + bottomIndex)) )
              runLength = ((topStart + top->Length) - (topStart + topIndex));
            else
              runLength = ((bottomStart + bottom->Length) - (bottomStart + bottomIndex));
          }
          else

            runLength = top->Length - topIndex;

          // Skip transp1arent voxels in both runs

          if (top->VoxelData == NULL && (((bottom != NULL) ? bottom->VoxelData : NULL) == NULL))

          {
            for (h = 0; h < runLength; h+=myThis->ImageSampleDistance)

            {
              image->Advance(pixels,1);
              i++;
              topIndex += myThis->ImageSampleDistance;
              bottomIndex += myThis->ImageSampleDistance;
            }
          }
          else
          {

            // This loops samples voxels from both runs,
            // interpolates, performs shading and performs
            // compositing into the intermediate image
            for (h=0; h < runLength; h+= myThis->ImageSampleDistance)
            {
              if (myThis->IntermixIntersectingGeometry)
              {
                float depth = myThis->IntermediateZBuffer[myThis->ImageSampleDistance * (uSliceInteger + i) + myThis->ImageSampleDistance * (vSliceInteger + j) * myThis->IntermediateWidth * myThis->ImageSampleDistance];

                if (myThis->ReverseOrder)
                {
                  if (vk <= depth)
                    pixels->Offset = 1;
                }
                else
                {
                  if (vk >= depth)

                    pixels->Offset = 1;
                }
              }

              // Only process non-opaque pixels
              if (pixels->Offset == 0)
              {
                if (myThis->IsVoxelClipped(i*myThis->ImageSampleDistance,vj,vk) == 1)
                {
                  if (myThis->ClippingMode == VTK_SHEAR_WARP_MIP_FUNCTION)
                  {
                    functionType = VTK_SHEAR_WARP_MIP_FUNCTION;
                  }
                  else
                  {                  
                    image->Advance(pixels,1);
                    i++;
                    topIndex += myThis->ImageSampleDistance;
                    bottomIndex += myThis->ImageSampleDistance;
                    continue;
                  }
                }
                else
                {
                  functionType = myThis->FunctionType;
                }


                oldOpacity = pixels->Opacity[pixels->Region];
                oldRed = pixels->Red[pixels->Region];
                oldGreen = pixels->Green[pixels->Region];
                oldBlue = pixels->Blue[pixels->Region];

                if (functionType == VTK_SHEAR_WARP_COMPOSITE_FUNCTION)
                {
                  if (pixels->Value[pixels->Region] == 0)
                  {
                    sampledOpacity = 0.0f;
                    sampledRed = 0.0f;
                    sampledGreen = 0.0f;
                    sampledBlue = 0.0f;

                    if (top->VoxelData != NULL)
                    {
                      value = top->VoxelData[topIndex].Value;
                      sampledOpacity += SOTF[value] * weightTopLeft;
                      sampledRed += CTF[value*3 + 0] * weightTopLeft;
                      sampledGreen += CTF[value*3 + 1] * weightTopLeft;
                      sampledBlue += CTF[value*3 + 2] * weightTopLeft;

                      if (h + myThis->ImageSampleDistance < runLength)
                      {
                        value = top->VoxelData[topIndex+myThis->ImageSampleDistance].Value;
                        sampledOpacity += SOTF[value] * weightTopRight;
                        sampledRed += CTF[value*3 + 0] * weightTopRight;
                        sampledGreen += CTF[value*3 + 1] * weightTopRight;
                        sampledBlue += CTF[value*3 + 2] * weightTopRight;
                      }
                    }

                    if (bottom != NULL)
                    {
                      if (bottom->VoxelData != NULL)
                      {
                        value = bottom->VoxelData[bottomIndex].Value;
                        sampledOpacity += SOTF[value] * weightBottomLeft;
                        sampledRed += CTF[value*3 + 0] * weightBottomLeft;
                        sampledGreen += CTF[value*3 + 1] * weightBottomLeft;

                        sampledBlue += CTF[value*3 + 2] * weightBottomLeft;


                        if (h + myThis->ImageSampleDistance < runLength)
                        {
                          value = bottom->VoxelData[bottomIndex+myThis->ImageSampleDistance].Value;
                          sampledOpacity += SOTF[value] * weightBottomRight;
                          sampledRed += CTF[value*3 + 0] * weightBottomRight;
                          sampledGreen += CTF[value*3 + 1] * weightBottomRight;
                          sampledBlue += CTF[value*3 + 2] * weightBottomRight;
                        }
                      }
                    }

                    if (myThis->Shade)
                    {
                      redDiffuse = 0.0f;
                      redSpecular = 0.0f;
                      blueDiffuse = 0.0f;
                      blueSpecular = 0.0f;
                      greenDiffuse = 0.0f;
                      greenSpecular = 0.0f;
                      sampledGradientMagnitude = 0.0f;
                      gradientOpacity = gradientOpacityConstant;

                      if (top->VoxelData != NULL)
                      {
                        encodedNormal = top->VoxelData[topIndex].EncodedNormal;

                        redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightTopLeft;
                        redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightTopLeft;
                        greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightTopLeft;
                        greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightTopLeft;
                        blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightTopLeft;
                        blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightTopLeft;

                        if (!gradientOpacityIsConstant)
                        {
                          gradientMagnitude = top->VoxelData[topIndex].GradientMagnitude;
                          sampledGradientMagnitude += float(gradientMagnitude) * weightTopLeft;
                        }

                        if (h + myThis->ImageSampleDistance < runLength)
                        {
                          encodedNormal = top->VoxelData[topIndex+myThis->ImageSampleDistance].EncodedNormal;

                          redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightTopRight;
                          redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightTopRight;
                          greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightTopRight;
                          greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightTopRight;
                          blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightTopRight;
                          blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightTopRight;

                          if (!gradientOpacityIsConstant)
                          {
                            gradientMagnitude = top->VoxelData[topIndex+myThis->ImageSampleDistance].GradientMagnitude;
                            sampledGradientMagnitude += float(gradientMagnitude) * weightTopRight;
                          }
                        }
                      }

                      if (bottom != NULL)
                      {
                        if (bottom->VoxelData != NULL)
                        {
                          encodedNormal = bottom->VoxelData[bottomIndex].EncodedNormal;

                          redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightBottomLeft;

                          redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightBottomLeft;
                          greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightBottomLeft;
                          greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightBottomLeft;
                          blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightBottomLeft;
                          blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightBottomLeft;


                          if (!gradientOpacityIsConstant)
                          {
                            gradientMagnitude = bottom->VoxelData[bottomIndex].GradientMagnitude;
                            sampledGradientMagnitude += float(gradientMagnitude) * weightBottomLeft;
                          }

                          if (h + myThis->ImageSampleDistance < runLength)
                          {
                            encodedNormal = bottom->VoxelData[bottomIndex+myThis->ImageSampleDistance].EncodedNormal;

                            redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightBottomRight;

                            redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightBottomRight;
                            greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightBottomRight;
                            greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightBottomRight;
                            blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightBottomRight;
                            blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightBottomRight;


                            if (!gradientOpacityIsConstant)
                            {

                              gradientMagnitude = bottom->VoxelData[bottomIndex+myThis->ImageSampleDistance].GradientMagnitude;
                              sampledGradientMagnitude += float(gradientMagnitude) * weightBottomRight;
                            }
                          }
                        }
                      }


                      if (!gradientOpacityIsConstant)
                      {
                        if (sampledGradientMagnitude > 255.0f)
                          gradientOpacity = GOTF[255];
                        else if (sampledGradientMagnitude < 0.0f)
                          gradientOpacity = GOTF[0];
                        else
                          gradientOpacity = GOTF[(unsigned char) sampledGradientMagnitude];
                      }

                      sampledRed *= redDiffuse + redSpecular;

                      sampledGreen *= greenDiffuse + greenSpecular;
                      sampledBlue *= blueDiffuse + blueSpecular;
                      sampledOpacity *= gradientOpacity;
                    }
                    

                    // Alpha Compositing
                    newRed = oldRed + sampledOpacity * sampledRed * (1.0f - oldOpacity);
                    newGreen = oldGreen + sampledOpacity * sampledGreen * (1.0f - oldOpacity);
                    newBlue = oldBlue + sampledOpacity * sampledBlue * (1.0f - oldOpacity);
                    newOpacity = oldOpacity + sampledOpacity * (1.0f - oldOpacity);
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }
                else if (functionType == VTK_SHEAR_WARP_MIP_FUNCTION)
                {
                  sampledValue = 0.0f;

                  if (top->VoxelData != NULL)
                  {
                    value = top->VoxelData[topIndex].Value;
                    sampledValue += float(value) * weightTopLeft;

                    if (h + myThis->ImageSampleDistance < runLength)
                    {
                      value = top->VoxelData[topIndex+myThis->ImageSampleDistance].Value;
                      sampledValue += float(value) * weightTopRight;
                    }
                  }

                  if (bottom != NULL)
                  {
                    if (bottom->VoxelData != NULL)
                    {
                      value = bottom->VoxelData[bottomIndex].Value;
                      sampledValue += float(value) * weightBottomLeft;

                      if (h + myThis->ImageSampleDistance < runLength)
                      {

                        value = bottom->VoxelData[bottomIndex+myThis->ImageSampleDistance].Value;

                        sampledValue += float(value) * weightBottomRight;
                      }
                    }
                  }

                  sampledOpacity = SOTF[int(sampledValue)];

                  // Maximum Intensity Projection
                  if (sampledValue > pixels->Value[pixels->Region] && sampledOpacity > oldOpacity)
                  {
                    newRed = CTF[int(sampledValue)*3+0];
                    newGreen = CTF[int(sampledValue)*3+1];
                    newBlue = CTF[int(sampledValue)*3+2];
                    newOpacity = sampledOpacity;
                    pixels->Value[pixels->Region] = sampledValue;
                  }
                  else
                  {

                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }
                else
                {
                  if (pixels->Value[pixels->Region] == 0)
                  {
                    sampledRed = isoRed;
                    sampledGreen = isoGreen;
                    sampledBlue = isoBlue;

                    if (myThis->Shade)
                    {
                      redDiffuse = 0.0f;
                      redSpecular = 0.0f;
                      blueDiffuse = 0.0f;
                      blueSpecular = 0.0f;
                      greenDiffuse = 0.0f;
                      greenSpecular = 0.0f;

                      adjustedTL = weightTopLeft;
                      adjustedBL = weightBottomLeft;
                      adjustedTR = weightTopRight;
                      adjustedBR = weightBottomRight;

                      if (h + myThis->ImageSampleDistance >= runLength)
                      {
                        adjustedTL += adjustedTR;
                        adjustedBL += adjustedBR;
                      }

                      if (top->VoxelData == NULL)
                      {
                        adjustedBL += adjustedTL;
                        adjustedBR += adjustedTR;
                      }
                      else if ((((bottom != NULL) ? bottom->VoxelData : NULL) == NULL))
                      {
                        adjustedTL += adjustedBL;
                        adjustedTR += adjustedBR;
                      }

                      if (top->VoxelData != NULL)
                      {
                        encodedNormal = top->VoxelData[topIndex].EncodedNormal;

                        redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedTL;
                        redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedTL;
                        greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedTL;
                        greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedTL;
                        blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedTL;
                        blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedTL;

                        if (h + myThis->ImageSampleDistance < runLength)
                        {
                          encodedNormal = top->VoxelData[topIndex+myThis->ImageSampleDistance].EncodedNormal;

                          redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedTR;
                          redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedTR;
                          greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedTR;
                          greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedTR;
                          blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedTR;
                          blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedTR;
                        }
                      }

                      if (bottom != NULL)
                      {
                        if (bottom->VoxelData != NULL)
                        {
                          encodedNormal = bottom->VoxelData[bottomIndex].EncodedNormal;

                          redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedBL;
                          redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedBL;
                          greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedBL;
                          greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedBL;
                          blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedBL;
                          blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedBL;

                          if (h + myThis->ImageSampleDistance < runLength)
                          {
                            encodedNormal = bottom->VoxelData[bottomIndex+myThis->ImageSampleDistance].EncodedNormal;

                            redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedBR;
                            redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedBR;
                            greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedBR;
                            greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedBR;
                            blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedBR;
                            blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedBR;
                          }
                        }
                      }

                      sampledRed *= redDiffuse + redSpecular;
                      sampledGreen *= greenDiffuse + greenSpecular;
                      sampledBlue *= blueDiffuse + blueSpecular;
                    }

                    newRed = sampledRed;
                    newGreen = sampledGreen;
                    newBlue = sampledBlue;
                    newOpacity = 1.0f;

                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }

                pixels->Red[pixels->Region] = newRed;
                pixels->Green[pixels->Region] = newGreen;
                pixels->Blue[pixels->Region] = newBlue;
                pixels->Opacity[pixels->Region] = newOpacity;

                if (newOpacity >= 1.0f)
                {
                  // The current intermediate pixel is opaque, so exit
                  // loop and skip opaque pixels
                  pixels->Offset = 1;
                  break;
                }

                image->Advance(pixels,1);

                i++;
                topIndex += myThis->ImageSampleDistance;
                bottomIndex += myThis->ImageSampleDistance;
              }
              else
              {
                // The current pixel has an offset greater than zero, so

                // we exit the loop and skip all opaque pixels
                break;
              }
            }
          }
        }
      }
    }
  }
}

// Lacroute's perspective projection shear-warp algorithm with runlength encoded volume using bilinear interpolation
template <class T>
static void CompositeIntermediateLinearRLEPerspective(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis)
{
  vtkShearWarpRLERun<T> **line;

  float *lineIndex;
  vtkShearWarpRLERun<T> *currentLine;

  int currentLineIndex;
  vtkShearWarpRLESlice<T> *slice;
  vtkShearWarpPixelData *pixels;
  int voxels;
  int left;

  float sampledRed,sampledGreen,sampledBlue,sampledOpacity;
  float sampledValue;
  float sampledGradientMagnitude;
  float oldRed,oldGreen,oldBlue,oldOpacity;
  float newRed,newGreen,newBlue,newOpacity;

  float redDiffuse,greenDiffuse,blueDiffuse;
  float redSpecular,greenSpecular,blueSpecular;
  float gradientOpacity;

  int skipped;
  int runLength;

  int i,j,k,h,g;
  int vk;
	float vi;
  float vj;

  unsigned short encodedNormal;
  unsigned char gradientMagnitude;

  float uSlice;
  float vSlice;

  int uSliceInteger;
  int vSliceInteger;

  float scaleX;
	float scaleY;
  float weight;

	int footprintX;
	int footprintY;

  T value;
  int vkIncrement;
	int vkStart;

  vtkShearWarpRLEVolume<T> *encodedVolume = dynamic_cast< vtkShearWarpRLEVolume<T> * > (myThis->EncodedVolume);

  if (myThis->ReverseOrder)
  {
    vkStart = myThis->CountK - 1;
    vkIncrement = -myThis->ImageSampleDistance;

  }
  else
  {
    vkStart = 0;
    vkIncrement = myThis->ImageSampleDistance;
  }

  float *SOTF[2];
  float *CTF[2];
  float *GOTF[2];
  float gradientOpacityConstant[2];
  int gradientOpacityIsConstant[2];
	int shade[2];
	float isoRed[2];
	float isoGreen[2];
	float isoBlue[2];

	SOTF[1] =  myThis->ClippingVolume->GetCorrectedScalarOpacityArray();
  CTF[1]  =  myThis->ClippingVolume->GetRGBArray();
	GOTF[1] =  myThis->ClippingVolume->GetGradientOpacityArray();
	gradientOpacityConstant[1] = myThis->ClippingVolume->GetGradientOpacityConstant();
	gradientOpacityIsConstant[1] = (gradientOpacityConstant[1] > 0.0f) ? 1 : 0;
	shade[1] = myThis->ClippingProperty->GetShade();

	SOTF[0] =  myThis->Volume->GetCorrectedScalarOpacityArray();
  CTF[0]  =  myThis->Volume->GetRGBArray();
  GOTF[0] =  myThis->Volume->GetGradientOpacityArray();
  gradientOpacityConstant[0] = myThis->Volume->GetGradientOpacityConstant();
	gradientOpacityIsConstant[0] = (gradientOpacityConstant[0] > 0.0f) ? 1 : 0;
	shade[0] = myThis->Shade;

  isoRed[1] = CTF[1][int(myThis->IsoValue)*3 + 0];
  isoGreen[1] = CTF[1][int(myThis->IsoValue)*3 + 1];
  isoBlue[1] = CTF[1][int(myThis->IsoValue)*3 + 2];

	isoRed[0] = CTF[0][int(myThis->IsoValue)*3 + 0];
	isoGreen[0] = CTF[0][int(myThis->IsoValue)*3 + 1];
	isoBlue[0] = CTF[0][int(myThis->IsoValue)*3 + 2];

  float vjIncrement;
  float viIncrement;

	float start[4];
	float end[4];
	float tstart[4];
	float tend[4];

	start[0] = 0.0;
	start[1] = 0.0;
	start[3] = 1.0;

	end[0] = start[0] + myThis->CountI - 1.0;
	end[1] = start[1] + myThis->CountJ - 1.0;
	end[3] = 1.0;

	int clipped = 0;
	int functions[2] = {myThis->FunctionType,myThis->ClippingMode};
	int functionType = functions[0];

  for (k = 0, vk = vkStart; k < myThis->CountK; k += myThis->ImageSampleDistance, vk += vkIncrement)
  {
		start[2] = vk;
		end[2] = vk;
	
		myThis->ShearMatrix->MultiplyPoint(start,tstart);
		myThis->ShearMatrix->MultiplyPoint(end,tend);

		tstart[0] /= tstart[3];
		tstart[1] /= tstart[3];

		tend[0] /= tend[3];
		tend[1] /= tend[3];

		uSlice = (tstart[0] + myThis->TranslationI);
		vSlice = (tstart[1] + myThis->TranslationJ);

		uSlice /= float(myThis->ImageSampleDistance);
		vSlice /= float(myThis->ImageSampleDistance);

		scaleX = (tend[0] - tstart[0] + 1) / (end[0] - start[0] + 1);
		scaleY = (tend[1] - tstart[1] + 1) / (end[1] - start[1] + 1);

    footprintX = (int) (1.0 + ceil ( 1.0 / scaleX));
    footprintY = (int) (1.0 + ceil ( 1.0 / scaleY));

    line = new vtkShearWarpRLERun<T>*[footprintY];
    lineIndex = new float[footprintY];

    viIncrement = 1.0 / scaleX;
    vjIncrement = 1.0 / scaleY;

		viIncrement *= myThis->ImageSampleDistance;
		vjIncrement *= myThis->ImageSampleDistance;

		uSliceInteger = (int)(uSlice + 0.5);// + 1;
    vSliceInteger = (int)(vSlice + 0.5);// + 1;

    slice = encodedVolume->GetSlice(myThis->MajorAxis,vk);

    // Composite one slice into the intermediate image
    for (j=0, vj = 0.0; vj < myThis->CountJ; j++, vj += vjIncrement)
    {
      for (g = 0; g < footprintY; g++)
      {
        if (vj+g*myThis->ImageSampleDistance < (myThis->CountJ))
          line[g] = slice->GetLineRuns(int(vj+g*myThis->ImageSampleDistance));
        else
          line[g] = NULL;

        lineIndex[g] = 0;
      }

			int position = uSliceInteger + (vSliceInteger+j)*myThis->IntermediateWidth;
      image->Position(pixels,position);

      for (i=0, vi = 0.0 ; vi < myThis->CountI; )
      {
				for (g = 0; g < footprintY; g++)
				{
					if (line[g] == NULL)
						break;

					while (lineIndex[g] >= line[g]->Length)
					{
						lineIndex[g] -= line[g]->Length;
						line[g]++;
					}              
				}

				// Skip opaque pixels in intermediate image
        skipped = image->Skip(pixels);

        // Update both runs if to be aligned with intermediate pixels
        if (skipped > 0)
        {
          i += skipped;
					vi += skipped * viIncrement;
          
          for (g = 0; g < footprintY; g++)
          {
            if (line[g] == NULL)
              break;
              
            lineIndex[g] += skipped * viIncrement;

            while (lineIndex[g] >= line[g]->Length)
            {
              lineIndex[g] -= line[g]->Length;
              line[g]++;
            }
          }            
        }
        else
        {
					clipped = 0;

          if (myThis->IntermixIntersectingGeometry)
          {
            float depth = myThis->IntermediateZBuffer[pixels-image->GetPixelData()];

            if (myThis->ReverseOrder)
            {
              if (vk <= depth)
                pixels->Offset = 1;
            }
            else
            {
              if (vk >= depth)
                pixels->Offset = 1;
            }
          }

					if (pixels->Offset == 0)
					{
						if (myThis->ClippingRenderer != NULL)
						{
							float backDepth = myThis->ClippingBackBuffer[pixels-image->GetPixelData()];
							float frontDepth = myThis->ClippingFrontBuffer[pixels-image->GetPixelData()];

							if (myThis->ReverseOrder)
							{             
								if (vk >= backDepth && vk <= frontDepth)
									clipped = 1;
							}
							else
							{
						    
								if (vk >= frontDepth && vk <= backDepth)
									clipped = 1;
							}
						}
					}

					if (pixels->Offset == 0 && !(clipped == 0 && functionType == VTK_SHEAR_WARP_TRANSPARENT_CLIPPING))
					{
					  if (clipped != pixels->State)
						{
						  pixels->Region++;
						  pixels->State = clipped;
						}

						functionType = functions[clipped];

						if (functionType != VTK_SHEAR_WARP_TRANSPARENT_CLIPPING)
						{
							sampledOpacity = 0.0f;
							sampledRed = 0.0f;
							sampledGreen = 0.0f;
							sampledBlue = 0.0f;
							sampledValue = 0.0f;
							redDiffuse = 0.0f;
							redSpecular = 0.0f;
							blueDiffuse = 0.0f;
							blueSpecular = 0.0f;
							greenDiffuse = 0.0f;
							greenSpecular = 0.0f;
							sampledGradientMagnitude = 0.0f;
							gradientOpacity = gradientOpacityConstant[clipped];
				      
							oldOpacity = pixels->Opacity[pixels->Region];
							oldRed = pixels->Red[pixels->Region];
							oldGreen = pixels->Green[pixels->Region];
							oldBlue = pixels->Blue[pixels->Region];

							voxels = 0;

								// This loops samples voxels from both runs,
								// interpolates, performs shading and performs
								// compositing into the intermediate image
							for (g = 0; g < footprintY; g++)
							{		        
								if (line[g] == NULL)
									break;
				        
								left = footprintX * myThis->ImageSampleDistance;

								if (vi + left >= myThis->CountI)
									left = (int)(myThis->CountI - vi);

								currentLine = line[g];
								currentLineIndex = (int)lineIndex[g];

								while (left >= myThis->ImageSampleDistance)
								{
									while (currentLineIndex >= currentLine->Length)
									{
										currentLineIndex -= currentLine->Length;
										currentLine++;
									}
				          
									runLength = (currentLineIndex + left < currentLine->Length) ? left : (currentLine->Length - currentLineIndex);

									if (currentLine->VoxelData == NULL)
									{
										currentLineIndex += runLength;
										left -= runLength;
									}
									else
									{                
										for (h = 0; h < runLength && left >= myThis->ImageSampleDistance; h += myThis->ImageSampleDistance, left -= myThis->ImageSampleDistance)
										{                    
											voxels++;

											if (functionType == VTK_SHEAR_WARP_COMPOSITE_FUNCTION)
											{
												value = currentLine->VoxelData[currentLineIndex].Value;
												sampledOpacity += SOTF[clipped][value];
												sampledRed += CTF[clipped][value*3 + 0];
												sampledGreen += CTF[clipped][value*3 + 1];
												sampledBlue += CTF[clipped][value*3 + 2];

												if (shade[clipped])
												{
													encodedNormal = currentLine->VoxelData[currentLineIndex].EncodedNormal;
													redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal];
													redSpecular += myThis->RedSpecularShadingTable[encodedNormal];
													greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal];
													greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal];
													blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal];
													blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal];

													if (!gradientOpacityIsConstant[clipped])
													{
														gradientMagnitude = currentLine->VoxelData[currentLineIndex].GradientMagnitude;
														sampledGradientMagnitude += float(gradientMagnitude);
													}
												}
											}
											else if (functionType == VTK_SHEAR_WARP_MIP_FUNCTION)
											{
												value = currentLine->VoxelData[currentLineIndex].Value;
												sampledValue += float(value);
											}
											else
											{
												value = currentLine->VoxelData[currentLineIndex].Value;
												sampledValue += float(value);

												if (shade[clipped])
												{
													encodedNormal = currentLine->VoxelData[currentLineIndex].EncodedNormal;
													redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal];
													redSpecular += myThis->RedSpecularShadingTable[encodedNormal];
													greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal];
													greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal];
													blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal];
													blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal];
												}
											}
											
											currentLineIndex += myThis->ImageSampleDistance;

										}
									}
								}

								lineIndex[g] += viIncrement;
							}

							if (voxels > 0)
							{
								if (functionType == VTK_SHEAR_WARP_COMPOSITE_FUNCTION)
								{
									sampledRed /= (float)voxels;
									sampledGreen /= (float)voxels;
									sampledBlue /= (float)voxels;
									sampledOpacity /= (float)voxels;

									// Alpha Compositing
									if (shade[clipped])
									{                          
										sampledGradientMagnitude /= float(voxels);

										if (!gradientOpacityIsConstant[clipped])
										{
											if (sampledGradientMagnitude > 255.0f)
												gradientOpacity = GOTF[clipped][255];
											else if (sampledGradientMagnitude < 0.0f)
												gradientOpacity = GOTF[clipped][0];
											else
												gradientOpacity = GOTF[clipped][(unsigned char) sampledGradientMagnitude];
										}
										else
										{
											gradientOpacity = gradientOpacityConstant[clipped];
										}

										redDiffuse /= (float)voxels;
										redSpecular /= (float)voxels;
										greenDiffuse /= (float)voxels;
										greenSpecular /= (float)voxels;
										blueDiffuse /= (float)voxels;
										blueSpecular /= (float)voxels;
				          
										sampledRed *= redDiffuse + redSpecular;
										sampledGreen *= greenDiffuse + greenSpecular;
										sampledBlue *= blueDiffuse + blueSpecular;
										sampledOpacity *= gradientOpacity;

									}

									newRed = oldRed + sampledOpacity * sampledRed * (1.0f - oldOpacity);
									newGreen = oldGreen + sampledOpacity * sampledGreen * (1.0f - oldOpacity);
									newBlue = oldBlue + sampledOpacity * sampledBlue * (1.0f - oldOpacity);
									newOpacity = oldOpacity + sampledOpacity * (1.0f - oldOpacity);
				        
								}
								else if (functionType == VTK_SHEAR_WARP_MIP_FUNCTION)
								{
									sampledValue /= (float)voxels;
				          
									// Maximum Intensity Projection
									if (sampledValue > pixels->Value[pixels->Region])
									{
										newOpacity = SOTF[clipped][int(sampledValue)];
										newRed = CTF[clipped][int(sampledValue)*3+0] * newOpacity;
										newGreen = CTF[clipped][int(sampledValue)*3+1] * newOpacity;
										newBlue = CTF[clipped][int(sampledValue)*3+2] * newOpacity;
										pixels->Value[pixels->Region] = sampledValue;
									}
									else
									{
										newRed = oldRed;
										newGreen = oldGreen;
										newBlue = oldBlue;
										newOpacity = oldOpacity;
									}
								}
								else
								{
									sampledValue /= (float)voxels;

									// Isosurface              
									if (sampledValue > myThis->IsoValue)
									{
										sampledRed = isoRed[clipped];
										sampledGreen = isoGreen[clipped];
										sampledBlue = isoBlue[clipped];
				            
										if (shade[clipped])
										{
											redDiffuse /= (float)voxels;
											redSpecular /= (float)voxels;
											greenDiffuse /= (float)voxels;
											greenSpecular /= (float)voxels;
											blueDiffuse /= (float)voxels;
											blueSpecular /= (float)voxels;

											sampledRed *= redDiffuse + redSpecular;
											sampledGreen *= greenDiffuse + greenSpecular;
											sampledBlue *= blueDiffuse + blueSpecular;                    
										}

										newRed = sampledRed;
										newGreen = sampledGreen;
										newBlue = sampledBlue;
										newOpacity = 1.0f;
									}
									else
									{
										newRed = oldRed;
										newGreen = oldGreen;
										newBlue = oldBlue;
										newOpacity = oldOpacity;
									}
								}

								pixels->Red[pixels->Region] = newRed;
								pixels->Green[pixels->Region] = newGreen;
								pixels->Blue[pixels->Region] = newBlue;
								pixels->Opacity[pixels->Region] = newOpacity;

								if (newOpacity >= 1.0f && functionType != VTK_SHEAR_WARP_MIP_FUNCTION)
								{
									// The current intermediate pixel is opaque, so exit
									// loop and skip opaque pixels
									pixels->Offset = 1;
								}
							}
						}
					}
					else
					{
						for (g = 0; g < footprintY; g++)
						{
							if (line[g] == NULL)
								break;

							lineIndex[g] += viIncrement;
			      }
					}

          image->Advance(pixels,1);
          i++;
					vi += viIncrement;
        }
      }
    }

    delete[] lineIndex;
    delete[] line;
  }

	image->Combine();
}




// Parallel projection shear-warp fast classification algorithm using nearest neighbour interpolation
template <class T>
static void CompositeIntermediateNearestUnclassified(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis)
{
  vtkShearWarpPixelData *pixels = 0;
  vtkShearWarpOctreeRun *runs = new vtkShearWarpOctreeRun[myThis->CountJ];
  vtkShearWarpOctreeRun *top;

  int topIndex;
  float sampledRed,sampledGreen,sampledBlue,sampledOpacity;
  float sampledValue;
  float sampledGradientMagnitude;
  float oldRed,oldGreen,oldBlue,oldOpacity;
  float newRed,newGreen,newBlue,newOpacity;
  float redDiffuse,greenDiffuse,blueDiffuse;
  float redSpecular,greenSpecular,blueSpecular;
  float gradientOpacity;

  float isoRed = 0.0;
  float isoGreen = 0.0;
  float isoBlue = 0.0;

  int skipped;

  int i,j,k;
  int vi,vj,vk;

  float *SOTF;
  float *CTF;
  float *GTF;
  float *GOTF;
  float gradientOpacityConstant;
  int gradientOpacityIsConstant;
  unsigned short encodedNormal;
  unsigned char gradientMagnitude;

  float uSlice;
  float vSlice;

  int uSliceInteger;
  int vSliceInteger;

  T value;

  int kStart;
  int vkStart;
  int kEnd;
  int kIncrement;
  int viIncrement;
  int vjIncrement;
  int vkIncrement;
  int size;

  T *dptr = (T*) myThis->GetInput()->GetScalarPointer();

  unsigned short *nptr = myThis->GradientEstimator->GetEncodedNormals();
  unsigned char *gptr = myThis->GradientEstimator->GetGradientMagnitudes();

  int *dimensions = myThis->GetInput()->GetDimensions();
  int location;
  int plane = dimensions[0]*dimensions[1];

  int functionType;

  vtkShearWarpOctree<T> *octree = dynamic_cast< vtkShearWarpOctree<T> * > (myThis->Octree);
  
  if (myThis->ReverseOrder)
  {
    kStart = myThis->CountK - 1;
    kEnd = -1;
    kIncrement = -1;
  }
  else
  {
    kStart = 0;
    kEnd = myThis->CountK;
    kIncrement = 1;
  }

  SOTF =  myThis->Volume->GetCorrectedScalarOpacityArray();
  CTF  =  myThis->Volume->GetRGBArray();
  GTF  =  myThis->Volume->GetGrayArray();
  GOTF =  myThis->Volume->GetGradientOpacityArray();
  gradientOpacityConstant = myThis->Volume->GetGradientOpacityConstant();

  if (gradientOpacityConstant > 0.0f)
    gradientOpacityIsConstant = 1;
  else
    gradientOpacityIsConstant = 0;


  if (myThis->FunctionType == VTK_SHEAR_WARP_ISOSURFACE_FUNCTION)
  {
    isoRed = CTF[int(myThis->IsoValue)*3 + 0];
    isoGreen = CTF[int(myThis->IsoValue)*3 + 1];
    isoBlue = CTF[int(myThis->IsoValue)*3 + 2];
  }

  switch (myThis->MajorAxis)
  {
    case VTK_X_AXIS:
      viIncrement = dimensions[0] * myThis->ImageSampleDistance;
      vjIncrement = plane * myThis->ImageSampleDistance;
      vkIncrement = kIncrement * myThis->ImageSampleDistance;
      vkStart = kStart * myThis->ImageSampleDistance;
      break;

    case VTK_Y_AXIS:
      viIncrement = plane * myThis->ImageSampleDistance;
      vjIncrement = myThis->ImageSampleDistance;
      vkIncrement = kIncrement * dimensions[0] * myThis->ImageSampleDistance;
      vkStart = kStart * dimensions[0] * myThis->ImageSampleDistance;
      break;

    case VTK_Z_AXIS:
      default:
      viIncrement = myThis->ImageSampleDistance;
      vjIncrement = dimensions[0] * myThis->ImageSampleDistance;
      vkIncrement = kIncrement * plane * myThis->ImageSampleDistance;
      vkStart = kStart * plane * myThis->ImageSampleDistance;
      break;
  }


  for (k = kStart, vk = vkStart; k != kEnd; k += kIncrement, vk += vkIncrement)
  {
    uSlice = k*myThis->ShearI + myThis->TranslationI;

    vSlice = k*myThis->ShearJ + myThis->TranslationJ;



    uSliceInteger = (int)ceil(uSlice) - 1;
    vSliceInteger = (int)ceil(vSlice) - 1;


    size = 0;

    // Composite one slice into the intermediate image
    for (j=0, vj = 0; j < myThis->CountJ; j++, vj += vjIncrement)
    {
      size -= 2 * myThis->ImageSampleDistance;

      if (size <= 0)
        size = octree->GetLineRuns(runs,myThis->MajorAxis, k*myThis->ImageSampleDistance, j*myThis->ImageSampleDistance);

      top = runs;
      topIndex = 0;

      image->Position(pixels,uSliceInteger + (vSliceInteger+j)*myThis->IntermediateWidth);

      for (i=0, vi = 0; i < myThis->CountI; )
      {
        // Update runs
        while (topIndex >= top->Length)
        {
          topIndex -= top->Length;
          top++;
        }

        // Skip opaque pixels in intermediate image
        skipped = image->Skip(pixels);

        // Update both runs if to be aligned with intermediate pixels

        if (skipped > 0)
        {
          i += skipped;
          vi += skipped * viIncrement;
          topIndex += skipped * myThis->ImageSampleDistance;
        }

        else
        {
          // Skip transparent voxels

          if (top->Type == VTK_SHEAR_WARP_OCTREE_TRANSPARENT)
          {
            while (topIndex < top->Length)
            {
              image->Advance(pixels,1);
              i++;
              vi += viIncrement;
              topIndex += myThis->ImageSampleDistance;

            }
          }
          else
          {


            // This loops samples voxels, performs shading and performs
            // compositing into the intermediate image
            while (topIndex < top->Length)
            {
              if (myThis->IntermixIntersectingGeometry)
              {
                float depth = myThis->IntermediateZBuffer[myThis->ImageSampleDistance * (uSliceInteger + i) + myThis->ImageSampleDistance * (vSliceInteger + j) * myThis->IntermediateWidth * myThis->ImageSampleDistance];

                if (myThis->ReverseOrder)
                {
                  if (k*myThis->ImageSampleDistance <= depth)
                    pixels->Offset = 1;
                }
                else
                {
                  if (k*myThis->ImageSampleDistance >= depth)
                    pixels->Offset = 1;
                }
              }


              // Only process non-opaque pixels
              if (pixels->Offset == 0)
              {
                if (myThis->IsVoxelClipped(i*myThis->ImageSampleDistance,j*myThis->ImageSampleDistance,k*myThis->ImageSampleDistance) == 1)
                {

                  if (myThis->ClippingMode == VTK_SHEAR_WARP_MIP_FUNCTION)
                  {
                    functionType = VTK_SHEAR_WARP_MIP_FUNCTION;
                  }
                  else
                  {
                    image->Advance(pixels,1);
                    i++;
                    vi += viIncrement;
                    topIndex += myThis->ImageSampleDistance;
                    continue;
                  }
                }
                else
                {
                  functionType = myThis->FunctionType;
                }
                
                oldOpacity = pixels->Opacity[pixels->Region];
                oldRed = pixels->Red[pixels->Region];
                oldGreen = pixels->Green[pixels->Region];
                oldBlue = pixels->Blue[pixels->Region];

                location = vi + vj + vk;

                if (functionType == VTK_SHEAR_WARP_COMPOSITE_FUNCTION)
                {
                  if (pixels->Value[pixels->Region] == 0)
                  {
                    sampledOpacity = 0.0f;
                    sampledRed = 0.0f;
                    sampledGreen = 0.0f;
                    sampledBlue = 0.0f;

                    value = dptr[location];
                    sampledOpacity += SOTF[value];
                    sampledRed += CTF[value*3 + 0];

                    sampledGreen += CTF[value*3 + 1];
                    sampledBlue += CTF[value*3 + 2];

                    if (myThis->Shade)
                    {
                      redDiffuse = 0.0f;
                      redSpecular = 0.0f;
                      blueDiffuse = 0.0f;
                      blueSpecular = 0.0f;
                      greenDiffuse = 0.0f;
                      greenSpecular = 0.0f;
                      sampledGradientMagnitude = 0.0f;
                      gradientOpacity = gradientOpacityConstant;

                      encodedNormal = nptr[location];

                      redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal];
                      redSpecular += myThis->RedSpecularShadingTable[encodedNormal];
                      greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal];
                      greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal];
                      blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal];
                      blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal];

                      if (!gradientOpacityIsConstant)
                      {
                        gradientMagnitude = gptr[location];
                        sampledGradientMagnitude += float(gradientMagnitude);

                        if (sampledGradientMagnitude > 255.0f)
                          gradientOpacity = GOTF[255];
                        else if (sampledGradientMagnitude < 0.0f)
                          gradientOpacity = GOTF[0];
                        else
                          gradientOpacity = GOTF[(unsigned char) sampledGradientMagnitude];
                      }

                      sampledRed *= redDiffuse + redSpecular;
                      sampledGreen *= greenDiffuse + greenSpecular;
                      sampledBlue *= blueDiffuse + blueSpecular;
                      sampledOpacity *= gradientOpacity;
                    }


                    // Alpha Compositing
                    newRed = oldRed + sampledOpacity * sampledRed * (1.0f - oldOpacity);
                    newGreen = oldGreen + sampledOpacity * sampledGreen * (1.0f - oldOpacity);
                    newBlue = oldBlue + sampledOpacity * sampledBlue * (1.0f - oldOpacity);
                    newOpacity = oldOpacity + sampledOpacity * (1.0f - oldOpacity);
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }
                else if (functionType == VTK_SHEAR_WARP_MIP_FUNCTION)
                {
                  sampledValue = 0.0f;

                  value = dptr[location];
                  sampledValue = float(value);

                  sampledOpacity = SOTF[int(sampledValue)];

                  // Maximum Intensity Projection
                  if (sampledValue > pixels->Value[pixels->Region] && sampledOpacity > oldOpacity)
                  {
                    newRed = CTF[int(sampledValue)*3+0];
                    newGreen = CTF[int(sampledValue)*3+1];
                    newBlue = CTF[int(sampledValue)*3+2];
                    newOpacity = sampledOpacity;
                    pixels->Value[pixels->Region] = sampledValue;
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;


                  }
                }
                else
                {
                  
                  sampledValue = 0.0f;

                  value = dptr[location];
                  sampledValue += float(value);

                  if (pixels->Value[pixels->Region] == 0 && sampledValue >= myThis->IsoValue)
                  {
                    sampledRed = isoRed;

                    sampledGreen = isoGreen;
                    sampledBlue = isoBlue;

                    if (myThis->Shade)
                    {
                      redDiffuse = 0.0f;
                      redSpecular = 0.0f;
                      blueDiffuse = 0.0f;
                      blueSpecular = 0.0f;
                      greenDiffuse = 0.0f;
                      greenSpecular = 0.0f;

                      encodedNormal = nptr[location];

                      redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal];

                      redSpecular += myThis->RedSpecularShadingTable[encodedNormal];
                      greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal];
                      greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal];

                      blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal];
                      blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal];

                      sampledRed *= redDiffuse + redSpecular;
                      sampledGreen *= greenDiffuse + greenSpecular;
                      sampledBlue *= blueDiffuse + blueSpecular;
                    }

                    newRed = sampledRed;
                    newGreen = sampledGreen;
                    newBlue = sampledBlue;
                    newOpacity = 1.0f;
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }

                pixels->Red[pixels->Region] = newRed;
                pixels->Green[pixels->Region] = newGreen;
                pixels->Blue[pixels->Region] = newBlue;
                pixels->Opacity[pixels->Region] = newOpacity;

                if (newOpacity >= 1.0f)
                {
                  // The current intermediate pixel is opaque, so exit
                  // loop and skip opaque pixels
                  pixels->Offset = 1;
                  break;
                }

                image->Advance(pixels,1);
                i++;

                vi += viIncrement;
                topIndex += myThis->ImageSampleDistance;
              }
              else
              {

                // The current pixel has an offset greater than zero, so
                // we exit the loop and skip all opaque pixels
                break;
              }
            }
          }
        }
      }

    }
  }

  delete[] runs;
}



// Parallel projection shear-warp fast classification algorithm using bilinear interpolation

template <class T>
static void CompositeIntermediateLinearUnclassified(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis)
{
  vtkShearWarpPixelData *pixels = 0;
  vtkShearWarpOctreeRun *runs = new vtkShearWarpOctreeRun[myThis->CountJ];
  vtkShearWarpOctreeRun *top,*bottom;

  int topIndex, bottomIndex, topStart, bottomStart;

  float sampledRed,sampledGreen,sampledBlue,sampledOpacity;
  float sampledValue;
  float sampledGradientMagnitude;
  float oldRed,oldGreen,oldBlue,oldOpacity;
  float newRed,newGreen,newBlue,newOpacity;
  float redDiffuse,greenDiffuse,blueDiffuse;
  float redSpecular,greenSpecular,blueSpecular;
  float gradientOpacity;

  float isoRed = 0.0;
  float isoGreen = 0.0;

  float isoBlue = 0.0;

  int skipped;
  int runLength;

  int i,j,k,h;
  int vi,vj,vk;

  float *SOTF;
  float *CTF;
  float *GTF;
  float *GOTF;
  float gradientOpacityConstant;
  int gradientOpacityIsConstant;
  unsigned short encodedNormal;
  unsigned char gradientMagnitude;


  float uSlice;
  float vSlice;

  int uSliceInteger;
  int vSliceInteger;

  float uSliceFractional;
  float vSliceFractional;

  float weightTopLeft;
  float weightBottomLeft;
  float weightTopRight;
  float weightBottomRight;

  float adjustedTL;
  float adjustedTR;
  float adjustedBL;
  float adjustedBR;

  T value;

  int kStart;
  int vkStart;
  int kEnd;
  int kIncrement;
  int viIncrement;
  int vjIncrement;

  int vkIncrement;
  int size;


  T *dptr = (T*) myThis->GetInput()->GetScalarPointer();

  unsigned short *nptr = myThis->GradientEstimator->GetEncodedNormals();
  unsigned char *gptr = myThis->GradientEstimator->GetGradientMagnitudes();

  int *dimensions = myThis->GetInput()->GetDimensions();
  int locationTL,locationTR,locationBL,locationBR;
  int plane = dimensions[0]*dimensions[1];

  int functionType;

  vtkShearWarpOctree<T> *octree = dynamic_cast< vtkShearWarpOctree<T> * > (myThis->Octree);
  
  if (myThis->ReverseOrder)
  {
    kStart = myThis->CountK - 1;
    kEnd = -1;
    kIncrement = -1;
  }
  else
  {
    kStart = 0;
    kEnd = myThis->CountK;
    kIncrement = 1;
  }

  SOTF =  myThis->Volume->GetCorrectedScalarOpacityArray();
  CTF  =  myThis->Volume->GetRGBArray();
  GTF  =  myThis->Volume->GetGrayArray();
  GOTF =  myThis->Volume->GetGradientOpacityArray();
  gradientOpacityConstant = myThis->Volume->GetGradientOpacityConstant();

  if (gradientOpacityConstant > 0.0f)
    gradientOpacityIsConstant = 1;
  else
    gradientOpacityIsConstant = 0;


  if (myThis->FunctionType == VTK_SHEAR_WARP_ISOSURFACE_FUNCTION)
  {
    isoRed = CTF[int(myThis->IsoValue)*3 + 0];
    isoGreen = CTF[int(myThis->IsoValue)*3 + 1];
    isoBlue = CTF[int(myThis->IsoValue)*3 + 2];
  }

  switch (myThis->MajorAxis)
  {
    case VTK_X_AXIS:
      viIncrement = dimensions[0] * myThis->ImageSampleDistance;
      vjIncrement = plane * myThis->ImageSampleDistance;
      vkIncrement = kIncrement * myThis->ImageSampleDistance;
      vkStart = kStart * myThis->ImageSampleDistance;
      break;

    case VTK_Y_AXIS:
      viIncrement = plane * myThis->ImageSampleDistance;
      vjIncrement = myThis->ImageSampleDistance;
      vkIncrement = kIncrement * dimensions[0] * myThis->ImageSampleDistance;
      vkStart = kStart * dimensions[0] * myThis->ImageSampleDistance;
      break;

    case VTK_Z_AXIS:
      default:
      viIncrement = myThis->ImageSampleDistance;
      vjIncrement = dimensions[0] * myThis->ImageSampleDistance;

      vkIncrement = kIncrement * plane * myThis->ImageSampleDistance;
      vkStart = kStart * plane * myThis->ImageSampleDistance;
      break;
  }


  for (k = kStart, vk = vkStart; k != kEnd; k += kIncrement, vk += vkIncrement)
  {
    uSlice = k*myThis->ShearI + myThis->TranslationI;
    vSlice = k*myThis->ShearJ + myThis->TranslationJ;

    uSliceInteger = (int)ceil(uSlice) - 1;
    vSliceInteger = (int)ceil(vSlice) - 1;

    uSliceFractional = uSlice - uSliceInteger;
    vSliceFractional = vSlice - vSliceInteger;

    weightTopLeft = uSliceFractional * vSliceFractional;
    weightBottomLeft = uSliceFractional * (1.0f - vSliceFractional);
    weightTopRight = (1.0f - uSliceFractional) * vSliceFractional;
    weightBottomRight = (1.0f - uSliceFractional) * (1.0f - vSliceFractional);

    size = 0;

    // Composite one slice into the intermediate image
    for (j=0, vj = 0; j < myThis->CountJ; j++, vj += vjIncrement)
    {
      size -= myThis->ImageSampleDistance;

      if (size <= 0)
      {
        size = octree->GetLineRuns(runs,myThis->MajorAxis, k*myThis->ImageSampleDistance, j*myThis->ImageSampleDistance);
        size -= myThis->ImageSampleDistance;
      }

      top = runs;

      if ((j + 1) < myThis->CountJ)
      {
        size -= myThis->ImageSampleDistance;

        if (size <= 0)
        {
          size = octree->GetLineRuns(runs, myThis->MajorAxis, k*myThis->ImageSampleDistance, (j+1)*myThis->ImageSampleDistance);
          size -= myThis->ImageSampleDistance;
        }

        bottom = runs;
      }
      else
        bottom = NULL;

      topStart = 0;
      bottomStart = 0;

      topIndex = 0;
      bottomIndex = 0;

      image->Position(pixels,uSliceInteger + (vSliceInteger+j)*myThis->IntermediateWidth);

      for (i=0, vi = 0; i < myThis->CountI; )
      {
        // Update runs
        while (topIndex >= top->Length)
        {
          topIndex -= top->Length;
          topStart += top->Length;
          top++;
        }

        if (bottom != NULL)
        {
          while (bottomIndex >= bottom->Length)
          {
            bottomIndex -= bottom->Length;
            bottomStart += bottom->Length;
            bottom++;
          }
        }

        // Skip opaque pixels in intermediate image

        skipped = image->Skip(pixels);

        // Update both runs if to be aligned with intermediate pixels
        if (skipped > 0)
        {
          i += skipped;
          vi += skipped * viIncrement;
          topIndex += skipped * myThis->ImageSampleDistance;
          bottomIndex += skipped * myThis->ImageSampleDistance;
        }
        else
        {
          if (bottom != NULL)
          {
            if ( ((topStart + top->Length) - (topStart + topIndex)) < ((bottomStart + bottom->Length) - (bottomStart + bottomIndex)) )
              runLength = ((topStart + top->Length) - (topStart + topIndex));
            else
              runLength = ((bottomStart + bottom->Length) - (bottomStart + bottomIndex));
          }
          else
            runLength = top->Length - topIndex;

          // Skip transp1arent voxels in both runs

          if (top->Type == VTK_SHEAR_WARP_OCTREE_TRANSPARENT && (((bottom != NULL) ? bottom->Type : VTK_SHEAR_WARP_OCTREE_TRANSPARENT) == VTK_SHEAR_WARP_OCTREE_TRANSPARENT))
          {
            for (h = 0; h < runLength; h+=myThis->ImageSampleDistance)
            {
              image->Advance(pixels,1);
              i++;
              vi += viIncrement;

              topIndex += myThis->ImageSampleDistance;
              bottomIndex += myThis->ImageSampleDistance;
            }

          }
          else
          {
            // This loops samples voxels from both runs,
            // interpolates, performs shading and performs
            // compositing into the intermediate image
            for (h=0; h < runLength; h+= myThis->ImageSampleDistance)
            {
              if (myThis->IntermixIntersectingGeometry)
              {
                float depth = myThis->IntermediateZBuffer[myThis->ImageSampleDistance * (uSliceInteger + i) + myThis->ImageSampleDistance * (vSliceInteger + j) * myThis->IntermediateWidth * myThis->ImageSampleDistance];

                if (myThis->ReverseOrder)
                {
                  if (k*myThis->ImageSampleDistance <= depth)
                    pixels->Offset = 1;
                }
                else

                {
                  if (k*myThis->ImageSampleDistance >= depth)
                    pixels->Offset = 1;
                }

              }
              
              // Only process non-opaque pixels
              if (pixels->Offset == 0)
              {
                if (myThis->IsVoxelClipped(i*myThis->ImageSampleDistance,j*myThis->ImageSampleDistance,k*myThis->ImageSampleDistance) == 1)
                {
                  if (myThis->ClippingMode == VTK_SHEAR_WARP_MIP_FUNCTION)
                  {
                    functionType = VTK_SHEAR_WARP_MIP_FUNCTION;
                  }
                  else
                  {
                    image->Advance(pixels,1);
                    i++;
                    vi += viIncrement;

                    topIndex += myThis->ImageSampleDistance;
                    bottomIndex += myThis->ImageSampleDistance;
                    continue;
                  }
                }

                else
                {
                  functionType = myThis->FunctionType;

                }
                
                oldOpacity = pixels->Opacity[pixels->Region];
                oldRed = pixels->Red[pixels->Region];
                oldGreen = pixels->Green[pixels->Region];
                oldBlue = pixels->Blue[pixels->Region];

                locationTL = vi + vj + vk;
                locationTR = locationTL + viIncrement;
                locationBL = locationTL + vjIncrement;
                locationBR = locationBL + viIncrement;

                if (functionType == VTK_SHEAR_WARP_COMPOSITE_FUNCTION)
                {
                  if (pixels->Value[pixels->Region] == 0)
                  {
                    sampledOpacity = 0.0f;
                    sampledRed = 0.0f;
                    sampledGreen = 0.0f;
                    sampledBlue = 0.0f;

                    value = dptr[locationTL];
                    sampledOpacity += SOTF[value] * weightTopLeft;
                    sampledRed += CTF[value*3 + 0] * weightTopLeft;
                    sampledGreen += CTF[value*3 + 1] * weightTopLeft;
                    sampledBlue += CTF[value*3 + 2] * weightTopLeft;

                    if (i + 1 < myThis->CountI)
                    {
                      value = dptr[locationTR];
                      sampledOpacity += SOTF[value] * weightTopRight;
                      sampledRed += CTF[value*3 + 0] * weightTopRight;
                      sampledGreen += CTF[value*3 + 1] * weightTopRight;
                      sampledBlue += CTF[value*3 + 2] * weightTopRight;
                    }


                    if (j + 1 < myThis->CountJ)
                    {
                      value = dptr[locationBL];
                      sampledOpacity += SOTF[value] * weightBottomLeft;
                      sampledRed += CTF[value*3 + 0] * weightBottomLeft;
                      sampledGreen += CTF[value*3 + 1] * weightBottomLeft;
                      sampledBlue += CTF[value*3 + 2] * weightBottomLeft;

                      if (i + 1 < myThis->CountI)
                      {
                        value = dptr[locationBR];
                        sampledOpacity += SOTF[value] * weightBottomRight;
                        sampledRed += CTF[value*3 + 0] * weightBottomRight;
                        sampledGreen += CTF[value*3 + 1] * weightBottomRight;
                        sampledBlue += CTF[value*3 + 2] * weightBottomRight;
                      }
                    }

                    if (myThis->Shade)
                    {
                      redDiffuse = 0.0f;
                      redSpecular = 0.0f;
                      blueDiffuse = 0.0f;

                      blueSpecular = 0.0f;
                      greenDiffuse = 0.0f;
                      greenSpecular = 0.0f;
                      sampledGradientMagnitude = 0.0f;
                      gradientOpacity = gradientOpacityConstant;

                      encodedNormal = nptr[locationTL];

                      redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightTopLeft;
                      redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightTopLeft;
                      greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightTopLeft;

                      greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightTopLeft;
                      blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightTopLeft;
                      blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightTopLeft;

                      if (!gradientOpacityIsConstant)
                      {
                        gradientMagnitude = gptr[locationTL];
                        sampledGradientMagnitude += float(gradientMagnitude) * weightTopLeft;
                      }

                      if (i + 1 < myThis->CountI)
                      {
                        encodedNormal = nptr[locationTR];

                        redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightTopRight;
                        redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightTopRight;
                        greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightTopRight;
                        greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightTopRight;
                        blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightTopRight;
                        blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightTopRight;

                        if (!gradientOpacityIsConstant)
                        {
                          gradientMagnitude = gptr[locationTR];
                          sampledGradientMagnitude += float(gradientMagnitude) * weightTopRight;
                        }
                      }

                      if (j + 1 < myThis->CountJ)
                      {
                        encodedNormal = nptr[locationBL];

                        redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightBottomLeft;
                        redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightBottomLeft;
                        greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightBottomLeft;
                        greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightBottomLeft;
                        blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightBottomLeft;
                        blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightBottomLeft;

                        if (!gradientOpacityIsConstant)
                        {
                          gradientMagnitude = gptr[locationBL];
                          sampledGradientMagnitude += float(gradientMagnitude) * weightBottomLeft;
                        }

                        if (i + 1 < myThis->CountI)
                        {
                          encodedNormal = nptr[locationBR];

                          redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * weightBottomRight;
                          redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * weightBottomRight;
                          greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * weightBottomRight;
                          greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * weightBottomRight;
                          blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * weightBottomRight;
                          blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * weightBottomRight;

                          if (!gradientOpacityIsConstant)
                          {
                            gradientMagnitude = gptr[locationBR];
                            sampledGradientMagnitude += float(gradientMagnitude) * weightBottomRight;
                          }
                        }
                      }

                      if (!gradientOpacityIsConstant)
                      {
                        if (sampledGradientMagnitude > 255.0f)
                          gradientOpacity = GOTF[255];
                        else if (sampledGradientMagnitude < 0.0f)
                          gradientOpacity = GOTF[0];
                        else
                          gradientOpacity = GOTF[(unsigned char) sampledGradientMagnitude];
                      }

                      sampledRed *= redDiffuse + redSpecular;
                      sampledGreen *= greenDiffuse + greenSpecular;
                      sampledBlue *= blueDiffuse + blueSpecular;
                      sampledOpacity *= gradientOpacity;
                    }

                    // Alpha Compositing
                    newRed = oldRed + sampledOpacity * sampledRed * (1.0f - oldOpacity);
                    newGreen = oldGreen + sampledOpacity * sampledGreen * (1.0f - oldOpacity);
                    newBlue = oldBlue + sampledOpacity * sampledBlue * (1.0f - oldOpacity);
                    newOpacity = oldOpacity + sampledOpacity * (1.0f - oldOpacity);
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }
                else if (functionType == VTK_SHEAR_WARP_MIP_FUNCTION)
                {

                  sampledValue = 0.0f;

                  value = dptr[locationTL];
                  sampledValue += float(value) * weightTopLeft;

                  if (i + 1 < myThis->CountI)
                  {
                    value = dptr[locationTR];
                    sampledValue += float(value) * weightTopRight;
                  }

                  if (j + 1 < myThis->CountJ)
                  {
                    value = dptr[locationBL];
                    sampledValue += float(value) * weightBottomLeft;

                    if (i + 1 < myThis->CountI)
                    {
                      value = dptr[locationBR];
                      sampledValue += float(value) * weightBottomRight;

                    }
                  }

                  sampledOpacity = SOTF[int(sampledValue)];

                  // Maximum Intensity Projection
                  if (sampledValue > pixels->Value[pixels->Region] && sampledOpacity > oldOpacity)
                  {
                    newRed = CTF[int(sampledValue)*3+0];
                    newGreen = CTF[int(sampledValue)*3+1];
                    newBlue = CTF[int(sampledValue)*3+2];
                    newOpacity = sampledOpacity;
                    pixels->Value[pixels->Region] = sampledValue;
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;
                    newOpacity = oldOpacity;
                  }
                }
                else
                {

                  sampledRed = isoRed;
                  sampledGreen = isoGreen;
                  sampledBlue = isoBlue;

                  sampledValue = 0.0f;

                  value = dptr[locationTL];
                  sampledValue += float(value) * weightTopLeft;

                  if (i + 1 < myThis->CountI)
                  {
                    value = dptr[locationTR];
                    sampledValue += float(value) * weightTopRight;
                  }

                  if (j + 1 < myThis->CountJ)
                  {
                    value = dptr[locationBL];
                    sampledValue += float(value) * weightBottomLeft;


                    if (i + 1 < myThis->CountI)
                    {
                      value = dptr[locationBR];
                      sampledValue += float(value) * weightBottomRight;
                    }
                  }

                  // Isosurface
                  if (pixels->Value[pixels->Region] == 0 && sampledValue >= myThis->IsoValue)
                  {
                    if (myThis->Shade)
                    {
                      redDiffuse = 0.0f;
                      redSpecular = 0.0f;
                      blueDiffuse = 0.0f;
                      blueSpecular = 0.0f;
                      greenDiffuse = 0.0f;
                      greenSpecular = 0.0f;

                      adjustedTL = weightTopLeft;
                      adjustedBL = weightBottomLeft;
                      adjustedTR = weightTopRight;
                      adjustedBR = weightBottomRight;

                      if (i + 1 >= myThis->CountI)
                      {
                        adjustedTL += adjustedTR;
                        adjustedBL += adjustedBR;
                      }

                      if (j + 1 >= myThis->CountJ)
                      {
                        adjustedTL += adjustedBL;
                        adjustedTR += adjustedBR;

                      }

                      encodedNormal = nptr[locationTL];

                      redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedTL;
                      redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedTL;
                      greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedTL;
                      greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedTL;
                      blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedTL;
                      blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedTL;

                      if (i + 1 < myThis->CountI)
                      {
                        encodedNormal = nptr[locationTR];

                        redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedTR;
                        redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedTR;
                        greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedTR;
                        greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedTR;
                        blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedTR;
                        blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedTR;
                      }

                      if (j + 1 < myThis->CountJ)
                      {
                        encodedNormal = nptr[locationBL];

                        redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedBL;
                        redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedBL;
                        greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedBL;
                        greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedBL;
                        blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedBL;
                        blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedBL;


                        if (i + 1 < myThis->CountI)
                        {
                          encodedNormal = nptr[locationBR];

                          redDiffuse += myThis->RedDiffuseShadingTable[encodedNormal] * adjustedBR;
                          redSpecular += myThis->RedSpecularShadingTable[encodedNormal] * adjustedBR;
                          greenDiffuse += myThis->GreenDiffuseShadingTable[encodedNormal] * adjustedBR;
                          greenSpecular += myThis->GreenSpecularShadingTable[encodedNormal] * adjustedBR;
                          blueDiffuse += myThis->BlueDiffuseShadingTable[encodedNormal] * adjustedBR;
                          blueSpecular += myThis->BlueSpecularShadingTable[encodedNormal] * adjustedBR;

                        }
                      }

                      sampledRed *= redDiffuse + redSpecular;
                      sampledGreen *= greenDiffuse + greenSpecular;
                      sampledBlue *= blueDiffuse + blueSpecular;

                    }


                    newRed = sampledRed;
                    newGreen = sampledGreen;
                    newBlue = sampledBlue;
                    newOpacity = 1.0f;
                  }
                  else
                  {
                    newRed = oldRed;
                    newGreen = oldGreen;
                    newBlue = oldBlue;

                    newOpacity = oldOpacity;
                  }

                }


                pixels->Red[pixels->Region] = newRed;
                pixels->Green[pixels->Region] = newGreen;
                pixels->Blue[pixels->Region] = newBlue;
                pixels->Opacity[pixels->Region] = newOpacity;

                if (newOpacity >= 1.0f)
                {
                  // The current intermediate pixel is opaque, so exit
                  // loop and skip opaque pixels
                  pixels->Offset = 1;
                  break;

                }


                image->Advance(pixels,1);
                i++;
                vi += viIncrement;
                topIndex += myThis->ImageSampleDistance;
                bottomIndex += myThis->ImageSampleDistance;
              }
              else
              {
                // The current pixel has an offset greater than zero, so
                // we exit the loop and skip all opaque pixels
                break;
              }

            }
          }
        }
      }
    }
  }

  delete[] runs;  
}


vtkVolumeShearWarpMapper::vtkVolumeShearWarpMapper()
{
  this->ImageSampleDistance        =  1;
  this->MinimumImageSampleDistance =  1;
  this->MaximumImageSampleDistance =  4;
  this->AutoAdjustSampleDistances  =  1;

  this->PerspectiveMatrix      = vtkMatrix4x4::New();
  this->ViewToWorldMatrix      = vtkMatrix4x4::New();
  this->ViewToVoxelsMatrix     = vtkMatrix4x4::New();
  this->VoxelsToViewMatrix     = vtkMatrix4x4::New();
  this->WorldToVoxelsMatrix    = vtkMatrix4x4::New();


  this->VoxelsToWorldMatrix    = vtkMatrix4x4::New();
  this->VoxelTransformMatrix   = vtkMatrix4x4::New();
  this->ViewportMatrix         = vtkMatrix4x4::New();
  this->ShearMatrix            = vtkMatrix4x4::New();
  this->WarpMatrix             = vtkMatrix4x4::New();
  this->PermutationMatrix      = vtkMatrix4x4::New();
  this->PermutedViewToVoxelsMatrix = vtkMatrix4x4::New();
  this->PermutedVoxelsToViewMatrix = vtkMatrix4x4::New();
  this->PerspectiveTransform   = vtkTransform::New();

  this->EncodedVolume          = NULL;
  this->Octree                 = NULL;

  this->ImageData              = NULL;
  this->ImageWidth             = 0;
  this->ImageHeight            = 0;
  this->AllocatedSize          = 0;

  this->IntemediateImage       = NULL;
  this->IntermediateWidth      = 0;
  this->IntermediateHeight     = 0;
  this->ScalarOpacityMTime     = 0;

  this->RenderTimeTable        = NULL;
  this->RenderVolumeTable      = NULL;
  this->RenderRendererTable    = NULL;
  this->RenderTableSize        = 0;  
  this->RenderTableEntries     = 0;


  this->GradientEstimator      = vtkFiniteDifferenceGradientEstimator::New();
  this->GradientShader         = vtkEncodedGradientShader::New();

  this->IsoValue               = 0.0f;


  this->RunlengthEncoding      = 0;
  this->FastClassification     = 0;
  this->Shade                  = 0;
  this->ParallelProjection	   = 0;

	this->ZBuffer                = vtkFloatArray::New();
	this->IntermediateZArray		 = vtkFloatArray::New();
	this->ClippingFrontArray		 = vtkFloatArray::New();
	this->ClippingBackArray			 = vtkFloatArray::New();
	this->ZBufferSize[0]				 = 0;
	this->ZBufferSize[1]				 = 0;
  this->IntermediateZBuffer    = NULL;
	this->ClippingBackBuffer		 = NULL;
	this->ClippingFrontBuffer		 = NULL;
  this->IntermixIntersectingGeometry = 0;

  this->FunctionType           = VTK_SHEAR_WARP_COMPOSITE_FUNCTION;
  this->ClippingMode           = VTK_SHEAR_WARP_TRANSPARENT_CLIPPING;

	this->ClippingVolume				 = vtkVolume::New();
	this->ClippingProperty			 = this->ClippingVolume->GetProperty();

  /*
  this->FrontDepth = 1.0;
  this->DepthDensity = 2.0;
  this->DeltaDepth = 1.0 / 255.0;
  this->DepthTableSize = 0;
  
  ComputeDepthTable(0,511);
  */
  this->Debug = 0;
}

void vtkVolumeShearWarpMapper::SetGradientEstimator(
                                       vtkEncodedGradientEstimator *gradest )
{

  // If we are setting it to its current value, don't do anything
  if ( this->GradientEstimator == gradest )
    {
    return;
    }

  // If we already have a gradient estimator, unregister it.
  if ( this->GradientEstimator )
    {
    this->GradientEstimator->UnRegister(this);
    this->GradientEstimator = NULL;
    }

  // If we are passing in a non-NULL estimator, register it
  if ( gradest )
    {

    gradest->Register( this );
    }

  // Actually set the estimator, and consider the object Modified
  this->GradientEstimator = gradest;
  this->Modified();
}

vtkVolumeShearWarpMapper::~vtkVolumeShearWarpMapper()
{

  this->PerspectiveMatrix->Delete();
  this->ViewToWorldMatrix->Delete();
  this->ViewToVoxelsMatrix->Delete();
  this->VoxelsToViewMatrix->Delete();
  this->WorldToVoxelsMatrix->Delete();
  this->VoxelsToWorldMatrix->Delete();
  this->VoxelTransformMatrix->Delete();
  this->ViewportMatrix->Delete();
  this->ShearMatrix->Delete();
  this->WarpMatrix->Delete();

  this->PermutationMatrix->Delete();
  this->PermutedViewToVoxelsMatrix->Delete();
  this->PerspectiveTransform->Delete();

  this->SetGradientEstimator( NULL );
  this->GradientShader->Delete();


  if (this->EncodedVolume != NULL)
    delete this->EncodedVolume;

  if (this->IntemediateImage != NULL)
    delete this->IntemediateImage;

  if (this->Octree != NULL)
    delete this->Octree;

  if (this->ImageData != NULL)
    delete[] this->ImageData;

	this->ZBuffer->Delete();
	this->IntermediateZArray->Delete();
	this->ClippingFrontArray->Delete();
	this->ClippingBackArray->Delete();
	this->ClippingVolume->Delete();
}


void vtkVolumeShearWarpMapper::Update()
{
  if ( this->GetInput() )
    {
    this->GetInput()->UpdateInformation();

    this->GetInput()->SetUpdateExtentToWholeExtent();
    this->GetInput()->Update();
    }

}

void vtkVolumeShearWarpMapper::Render( vtkRenderer *ren,
                                       vtkVolume *vol )
{
	printf("\nRendering ...\n");

  this->Volume = vol;

  // make sure that we have scalar input and update the scalar input
  if ( this->GetInput() == NULL )
    {
    vtkErrorMacro(<< "No Input!");

    return;
    }
  else
    {
    this->GetInput()->UpdateInformation();
    this->GetInput()->SetUpdateExtentToWholeExtent();
    this->GetInput()->Update();
    }

  // Start timing now. We didn't want to capture the update of the

  // input data in the times
  this->Timer->StartTimer();

	// Nasty hack
	this->ClippingVolume->SetMapper(this);
	this->ClippingVolume->UpdateTransferFunctions(ren);

  vol->UpdateTransferFunctions( ren );

  ren->ComputeAspect();
  float *aspect = ren->GetAspect();
  vtkCamera *cam = ren->GetActiveCamera();

  // Keep track of the projection matrix - we'll need it in a couple of places
  // Get the projection matrix. The method is called perspective, but
  // the matrix is valid for perspective and parallel viewing transforms.
  // Don't replace this with the GetCompositePerspectiveTransformMatrix because that
  // turns off stereo rendering!!!
  this->PerspectiveTransform->SetMatrix(cam->GetPerspectiveTransformMatrix(aspect[0]/aspect[1],
                                                        0.0, 1.0 ));
  this->PerspectiveTransform->Concatenate(cam->GetViewTransformMatrix());
  this->PerspectiveMatrix->DeepCopy(this->PerspectiveTransform->GetMatrix());

 
  // Compute some matrices from voxels to view and vice versa based
  // on the whole input

  this->VoxelTransformMatrix->DeepCopy( vol->GetMatrix() );



  this->ComputeMatrices( this->GetInput(), vol );
  this->ParallelProjection = cam->GetParallelProjection();

    // How big is the viewport in pixels?
  float *viewport   =  ren->GetViewport();

  int *renWinSize   =  ren->GetRenderWindow()->GetSize();

  // Save this so that we can restore it if the image is cancelled
  int oldImageSampleDistance = this->ImageSampleDistance;

  // If we are automatically adjusting the size to achieve a desired frame
  // rate, then do that adjustment here. Base the new image sample distance
  // on the previous one and the previous render time. Don't let
  // the adjusted image sample distance be less than the minimum image sample
  // distance or more than the maximum image sample distance.
  if ( this->AutoAdjustSampleDistances )
    {
    float oldTime = this->RetrieveRenderTime( ren, vol );
    float newTime = vol->GetAllocatedRenderTime();
    this->ImageSampleDistance = (this->ImageSampleDistance * int(0.5+(sqrt(oldTime / newTime))));//int(rint(sqrt(oldTime / newTime))));
    this->ImageSampleDistance =
      (this->ImageSampleDistance>this->MaximumImageSampleDistance)?
      (this->MaximumImageSampleDistance):(this->ImageSampleDistance);
    this->ImageSampleDistance =
      (this->ImageSampleDistance<this->MinimumImageSampleDistance)?

      (this->MinimumImageSampleDistance):(this->ImageSampleDistance);
    }

	this->ClippingVolume->UpdateScalarOpacityforSampleSize( ren, this->ImageSampleDistance );
  vol->UpdateScalarOpacityforSampleSize( ren, this->ImageSampleDistance );

  // The full image fills the viewport. First, compute the actual viewport
  // size, then divide by the ImageSampleDistance to find the full image
  // size in pixels
  this->ImageViewportSize[0] = static_cast<int>
    (static_cast<float>(renWinSize[0]) * (viewport[2]-viewport[0]));
  this->ImageViewportSize[1] = static_cast<int>
    (static_cast<float>(renWinSize[1]) * (viewport[3]-viewport[1]));

  this->ImageViewportSize[0] =
    static_cast<int>( static_cast<float>(this->ImageViewportSize[0]) );
  this->ImageViewportSize[1] =
    static_cast<int>( static_cast<float>(this->ImageViewportSize[1]) );

/*    
  this->ImageViewportSize[0] =
    static_cast<int>( static_cast<float>(this->ImageViewportSize[0]) /
                      static_cast<float>(this->ImageSampleDistance) );
  this->ImageViewportSize[1] =
    static_cast<int>( static_cast<float>(this->ImageViewportSize[1])  /
                      static_cast<float>(this->ImageSampleDistance) );
*/
  vol->UpdateTransferFunctions( ren );

  vol->UpdateScalarOpacityforSampleSize( ren, this->ImageSampleDistance );

  this->Shade =  vol->GetProperty()->GetShade();

  this->GradientEstimator->SetInput( this->GetInput() );

  if ( this->Shade )
    {
    this->GradientShader->UpdateShadingTable( ren, vol,
                                              this->GradientEstimator );
    this->EncodedNormals =
      this->GradientEstimator->GetEncodedNormals();

    this->RedDiffuseShadingTable =
      this->GradientShader->GetRedDiffuseShadingTable(vol);

    this->GreenDiffuseShadingTable =
      this->GradientShader->GetGreenDiffuseShadingTable(vol);
    this->BlueDiffuseShadingTable =
      this->GradientShader->GetBlueDiffuseShadingTable(vol);

    this->RedSpecularShadingTable =
      this->GradientShader->GetRedSpecularShadingTable(vol);
    this->GreenSpecularShadingTable =
      this->GradientShader->GetGreenSpecularShadingTable(vol);
    this->BlueSpecularShadingTable =
      this->GradientShader->GetBlueSpecularShadingTable(vol);
    }

  else
    {

    this->EncodedNormals = NULL;
    this->RedDiffuseShadingTable = NULL;
    this->GreenDiffuseShadingTable = NULL;
    this->BlueDiffuseShadingTable = NULL;
    this->RedSpecularShadingTable = NULL;
    this->GreenSpecularShadingTable = NULL;
    this->BlueSpecularShadingTable = NULL;
    }

  // If we have non-constant opacity on the gradient magnitudes,
  // we need to use the gradient magnitudes to look up the opacity
  if ( vol->GetGradientOpacityConstant() == -1.0 )
    {
    this->GradientMagnitudes =
      this->GradientEstimator->GetGradientMagnitudes();
    }
  else
    {
    this->GradientMagnitudes = NULL;
    }

  float bounds[6];
  int dim[3];

  this->GetInput()->GetDimensions(dim);
  bounds[0] = bounds[2] = bounds[4] = 0.0;

  bounds[1] = static_cast<float>(dim[0]-1);

  bounds[3] = static_cast<float>(dim[1]-1);
  bounds[5] = static_cast<float>(dim[2]-1);

  float voxelPoint[3];
  float viewPoint[8][4];
  int i, j, k;
  float minX, minY, maxX, maxY, minZ, maxZ;

  minX =  1.0;
  minY =  1.0;
  maxX = -1.0;
  maxY = -1.0;
  minZ =  1.0;
  maxZ =  0.0;
    
  float camPos[3];
  float worldBounds[6];
  vol->GetBounds( worldBounds );
  int insideFlag = 0;
  ren->GetActiveCamera()->GetPosition( camPos );
  if ( camPos[0] >= worldBounds[0] &&
       camPos[0] <= worldBounds[1] &&
       camPos[1] >= worldBounds[2] &&
       camPos[1] <= worldBounds[3] &&
       camPos[2] >= worldBounds[4] &&
       camPos[2] <= worldBounds[5] )
    {
    insideFlag = 1;
    }



  // Copy the voxelsToView matrix to 16 floats
  float voxelsToViewMatrix[16];
  for ( j = 0; j < 4; j++ )
    {
    for ( i = 0; i < 4; i++ )
      {
      voxelsToViewMatrix[j*4+i] =
        static_cast<float>(this->VoxelsToViewMatrix->GetElement(j,i));
      }
    }

  // Convert the voxel bounds to view coordinates to find out the
  // size and location of the image we need to generate.
  int idx = 0;
  if ( insideFlag )
    {
    minX = -1.0;
    maxX =  1.0;
    minY = -1.0;
    maxY =  1.0;
    minZ =  0.001;
    maxZ =  0.001;
    }
  else

    {
    for ( k = 0; k < 2; k++ )
      {
      voxelPoint[2] = bounds[4+k];
      for ( j = 0; j < 2; j++ )
        {
        voxelPoint[1] = bounds[2+j];
        for ( i = 0; i < 2; i++ )
          {
          voxelPoint[0] = bounds[i];

          vtkVSWMultiplyPointMacro( voxelPoint, viewPoint[idx],
                                   voxelsToViewMatrix );

          minX = (viewPoint[idx][0]<minX)?(viewPoint[idx][0]):(minX);
          minY = (viewPoint[idx][1]<minY)?(viewPoint[idx][1]):(minY);
          maxX = (viewPoint[idx][0]>maxX)?(viewPoint[idx][0]):(maxX);
          maxY = (viewPoint[idx][1]>maxY)?(viewPoint[idx][1]):(maxY);

          minZ = (viewPoint[idx][2]<minZ)?(viewPoint[idx][2]):(minZ);
          maxZ = (viewPoint[idx][2]>maxZ)?(viewPoint[idx][2]):(maxZ);
          idx++;
          }
        }
      }
    }

  if ( minZ < 0.001 || maxZ > 0.9999 )
    {
    minX = -1.0;
    maxX =  1.0;
    minY = -1.0;
    maxY =  1.0;
    insideFlag = 1;
    }
    
  this->MinimumViewDistance =
    (minZ<0.001)?(0.001):((minZ>0.999)?(0.999):(minZ));    


  if ( !ren->GetRenderWindow()->GetAbortRender() )
    this->FactorViewMatrix();


  if ( this->ClippingPlanes )
    this->InitializeClippingPlanes( this->ClippingPlanes );
    
  if (this->IntermixIntersectingGeometry == 1)
  {
    if ( !ren->GetRenderWindow()->GetAbortRender() )
		{
      this->IntermediateZBuffer = this->Extract(this->IntermediateZArray,ren,vol);
		}
  }

  if ( !ren->GetRenderWindow()->GetAbortRender() && this->ClippingRenderer != NULL)
	{
		this->ClippingRenderer->GetRenderWindow()->MakeCurrent();

		vtkActorCollection *actors = this->ClippingRenderer->GetActors();
		vtkActor *actor = NULL;

		actors->InitTraversal();

		while ((actor = actors->GetNextActor()) != NULL)
		{
			actor->GetProperty()->BackfaceCullingOff();
			actor->GetProperty()->FrontfaceCullingOn();
		}

		this->ClippingRenderer->Clear();
		this->ClippingRenderer->Render();//GetRenderWindow()->Render();
		this->ClippingBackBuffer = Extract(this->ClippingBackArray,this->ClippingRenderer,vol);

		actors->InitTraversal();

		while ((actor = actors->GetNextActor()) != NULL)
		{
			actor->GetProperty()->BackfaceCullingOn();
			actor->GetProperty()->FrontfaceCullingOff();
		}

		this->ClippingRenderer->Clear();
		this->ClippingRenderer->Render();//GetRenderWindow()->Render();
		this->ClippingFrontBuffer = Extract(this->ClippingFrontArray,this->ClippingRenderer,vol);

		ren->GetRenderWindow()->MakeCurrent();
	}


//	ren->GetRenderWindow()->OffScreenRenderingOn();

//		this->ClippingActor->RenderOpaqueGeometry(ren);//this->ClippingRenderer);

//	ren->GetRenderWindow()->OffScreenRenderingOff();

	vtkTimerLog *timer = vtkTimerLog::New();
	timer->StartTimer();

  if ( !ren->GetRenderWindow()->GetAbortRender() )
    this->CompositeIntermediate();

	timer->StopTimer();
	printf(">> Composite: %f\n",timer->GetElapsedTime());

	ren->GetRenderWindow()->OffScreenRenderingOn();

	vol->RenderOpaqueGeometry(ren);

	ren->GetRenderWindow()->OffScreenRenderingOff();

  if ( !ren->GetRenderWindow()->GetAbortRender() )
    this->RenderTexture(ren,vol);

	ren->GetRenderWindow()->OffScreenRenderingOff();

	if ( !ren->GetRenderWindow()->GetAbortRender() )
      {
      this->Timer->StopTimer();

      this->TimeToDraw = this->Timer->GetElapsedTime();
      this->StoreRenderTime( ren, vol, this->TimeToDraw );
      }

    // Restore the image sample distance so that automatic adjustment
    // will work correctly
    else
      {
      this->ImageSampleDistance = oldImageSampleDistance;
      }

			timer->Delete();
}



vtkVolumeShearWarpMapper *vtkVolumeShearWarpMapper::New()
{

  // First try to create the object from the vtkObjectFactory
  vtkObject* ret =
    vtkGraphicsFactory::CreateInstance("vtkVolumeShearWarpMapper");

  if (ret != NULL)
    return (vtkVolumeShearWarpMapper*)ret;
	
  return vtkOpenGLVolumeShearWarpMapper::New();
}


void vtkVolumeShearWarpMapper::ComputeMatrices( vtkImageData *data,
                                                 vtkVolume *vol )
{
  // Get the data spacing. This scaling is not accounted for in
  // the volume's matrix, so we must add it in.
  float volumeSpacing[3];
  data->GetSpacing( volumeSpacing );

  // Get the origin of the data.  This translation is not accounted for in
  // the volume's matrix, so we must add it in.
  float volumeOrigin[3];
  data->GetOrigin( volumeOrigin );

  // Get the dimensions of the data.
  int volumeDimensions[3];
  data->GetDimensions( volumeDimensions );

  // Create some transform objects that we will need later
  vtkTransform *voxelsTransform = vtkTransform::New();
  vtkTransform *voxelsToViewTransform = vtkTransform::New();

  // Get the volume matrix. This is a volume to world matrix right now.
  // We'll need to invert it, translate by the origin and scale by the

  // spacing to change it to a world to voxels matrix.
  vtkMatrix4x4 *volMatrix = vtkMatrix4x4::New();
  volMatrix->DeepCopy( vol->GetMatrix() );

  voxelsToViewTransform->SetMatrix( volMatrix );

  // Create a transform that will account for the scaling and translation of
  // the scalar data. The is the volume to voxels matrix.
  voxelsTransform->Identity();
  voxelsTransform->Translate(volumeOrigin[0],
                             volumeOrigin[1],
                             volumeOrigin[2] );

  voxelsTransform->Scale( volumeSpacing[0],
                          volumeSpacing[1],
                          volumeSpacing[2] );

  // Now concatenate the volume's matrix with this scalar data matrix
  voxelsToViewTransform->PreMultiply();
  voxelsToViewTransform->Concatenate( voxelsTransform->GetMatrix() );

  // Now we actually have the world to voxels matrix - copy it out
  this->WorldToVoxelsMatrix->DeepCopy( voxelsToViewTransform->GetMatrix() );
  this->WorldToVoxelsMatrix->Invert();

  // We also want to invert this to get voxels to world
  this->VoxelsToWorldMatrix->DeepCopy( voxelsToViewTransform->GetMatrix() );

  // Compute the voxels to view transform by concatenating the
  // voxels to world matrix with the projection matrix (world to view)
  voxelsToViewTransform->PostMultiply();
  voxelsToViewTransform->Concatenate( this->PerspectiveMatrix );

  this->VoxelsToViewMatrix->DeepCopy( voxelsToViewTransform->GetMatrix() );
  this->ViewToVoxelsMatrix->DeepCopy( this->VoxelsToViewMatrix );
  this->ViewToVoxelsMatrix->Invert();

  voxelsToViewTransform->Delete();
  voxelsTransform->Delete();
  volMatrix->Delete();
}

float vtkVolumeShearWarpMapper::RetrieveRenderTime( vtkRenderer *ren,
                                                  vtkVolume   *vol )
{
  int i;

  for ( i = 0; i < this->RenderTableEntries; i++ )
    {
    if ( this->RenderVolumeTable[i] == vol &&
         this->RenderRendererTable[i] == ren )
      {
      return this->RenderTimeTable[i];
      }
    }

  return 0.0;
}

void vtkVolumeShearWarpMapper::StoreRenderTime( vtkRenderer *ren,
                                              vtkVolume   *vol,
                                              float       time )
{
  int i;
  for ( i = 0; i < this->RenderTableEntries; i++ )

    {

    if ( this->RenderVolumeTable[i] == vol &&
         this->RenderRendererTable[i] == ren )
      {
      this->RenderTimeTable[i] = time;
      return;
      }
    }


  // Need to increase size
  if ( this->RenderTableEntries >= this->RenderTableSize )
    {
    if ( this->RenderTableSize == 0 )
      {
      this->RenderTableSize = 10;
      }
    else
      {
      this->RenderTableSize *= 2;
      }

    float       *oldTimePtr     = this->RenderTimeTable;
    vtkVolume   **oldVolumePtr   = this->RenderVolumeTable;

    vtkRenderer **oldRendererPtr = this->RenderRendererTable;

    this->RenderTimeTable     = new float [this->RenderTableSize];
    this->RenderVolumeTable   = new vtkVolume *[this->RenderTableSize];
    this->RenderRendererTable = new vtkRenderer *[this->RenderTableSize];

    for (i = 0; i < this->RenderTableEntries; i++ )
      {
      this->RenderTimeTable[i] = oldTimePtr[i];
      this->RenderVolumeTable[i] = oldVolumePtr[i];
      this->RenderRendererTable[i] = oldRendererPtr[i];
      }

    delete [] oldTimePtr;
    delete [] oldVolumePtr;
    delete [] oldRendererPtr;
    }

  this->RenderTimeTable[this->RenderTableEntries] = time;
  this->RenderVolumeTable[this->RenderTableEntries] = vol;
  this->RenderRendererTable[this->RenderTableEntries] = ren;

  this->RenderTableEntries++;
}

void vtkVolumeShearWarpMapper::CompositeIntermediate()
{
  int scalarType = this->GetInput()->GetScalarType();
  int interpolationType = this->Volume->GetProperty()->GetInterpolationType();



  this->ImageWidth = 32;
  this->ImageHeight = 32;

  while (this->ImageWidth < this->MaximumIntermediateDimension)
    this->ImageWidth = this->ImageWidth << 1;

  while (this->ImageHeight < this->MaximumIntermediateDimension)
    this->ImageHeight = this->ImageHeight << 1;

  int imageSize = this->ImageWidth * this->ImageHeight;

  if (imageSize > this->AllocatedSize)
  {
    this->AllocatedSize = imageSize;

    if (this->ImageData != NULL)
      delete[] this->ImageData;

    this->ImageData = new unsigned char[imageSize*4];

    if (this->IntemediateImage != NULL)
      delete this->IntemediateImage;

    this->IntemediateImage = new vtkShearWarpRLEImage(imageSize);
  }
  else
    this->IntemediateImage->Clear();

  if (this->RunlengthEncoding == 1)
  {
    if (this->FunctionType == VTK_SHEAR_WARP_ISOSURFACE_FUNCTION)
    {
      if (this->EncodedVolume != NULL)
      {
        if (this->EncodedVolume->IsScalarEncoded())
        {
          if (this->EncodedVolume->GetIsoValue() != this->IsoValue)
          {
            delete this->EncodedVolume;
            this->EncodedVolume = NULL;
          }
        }
        else
        {
          delete this->EncodedVolume;
          this->EncodedVolume = NULL;
        }
      }


      if (this->EncodedVolume == NULL)
      {
        switch ( scalarType )
        {
          case VTK_UNSIGNED_CHAR:
          {
            vtkShearWarpRLEVolume<unsigned char> *encodedVolume = new vtkShearWarpRLEVolume<unsigned char>();

            encodedVolume->encodeScalar(this->GetInput(),this->Volume,this->GradientEstimator,this->IsoValue);
            this->EncodedVolume = encodedVolume;
            break;
          }
          case VTK_UNSIGNED_SHORT:
          {
            vtkShearWarpRLEVolume<unsigned short> *encodedVolume = new vtkShearWarpRLEVolume<unsigned short>();
            encodedVolume->encodeScalar(this->GetInput(),this->Volume,this->GradientEstimator,this->IsoValue);
            this->EncodedVolume = encodedVolume;            
            break;
          }
        }        
      }

      switch ( scalarType )
      {
        case VTK_UNSIGNED_CHAR:
          if ( interpolationType == VTK_NEAREST_INTERPOLATION )
            CompositeIntermediateNearestRLE<unsigned char>(this->IntemediateImage,this);
          else
            CompositeIntermediateLinearRLE<unsigned char>(this->IntemediateImage,this);
          break;

        case VTK_UNSIGNED_SHORT:
          if ( interpolationType == VTK_NEAREST_INTERPOLATION )

            CompositeIntermediateNearestRLE<unsigned short>(this->IntemediateImage,this);
          else
            CompositeIntermediateLinearRLE<unsigned short>(this->IntemediateImage,this);
          break;
      }      
    }
    else
    {
      // If the scalar opacity transfer function has been modified
      // the runlength encoding has to be redone
      unsigned long scalarOpacityMTime = this->Volume->GetProperty()->GetScalarOpacity()->GetMTime();


      if (this->EncodedVolume != NULL)
      {
        if ( scalarOpacityMTime > this->ScalarOpacityMTime)
        {
          delete this->EncodedVolume;
          this->EncodedVolume = NULL;
        }
      }

      if (this->EncodedVolume == NULL)
      {
        this->ScalarOpacityMTime =  scalarOpacityMTime;

        switch ( scalarType )
        {
          case VTK_UNSIGNED_CHAR:
          {
            vtkShearWarpRLEVolume<unsigned char> *encodedVolume = new vtkShearWarpRLEVolume<unsigned char>();
            encodedVolume->encodeOpacity(this->GetInput(),this->Volume,this->GradientEstimator,0.0f);
            this->EncodedVolume = encodedVolume;
            break;
          }

            
          case VTK_UNSIGNED_SHORT:
          {
            vtkShearWarpRLEVolume<unsigned short> *encodedVolume = new vtkShearWarpRLEVolume<unsigned short>();

            encodedVolume->encodeOpacity(this->GetInput(),this->Volume,this->GradientEstimator,0.0f);
            this->EncodedVolume = encodedVolume;
            break;
          }
        }
      }

      switch ( scalarType )
      {
        case VTK_UNSIGNED_CHAR:
          if ( interpolationType == VTK_NEAREST_INTERPOLATION )
            CompositeIntermediateNearestRLE<unsigned char>(this->IntemediateImage,this);
          else
//            CompositeIntermediateLinearRLE<unsigned char>(this->IntemediateImage,this);
            CompositeIntermediateLinearRLEPerspective<unsigned char>(this->IntemediateImage,this);
          break;

        case VTK_UNSIGNED_SHORT:
          if ( interpolationType == VTK_NEAREST_INTERPOLATION )
            CompositeIntermediateNearestRLE<unsigned short>(this->IntemediateImage,this);
          else
//            CompositeIntermediateLinearRLE<unsigned short>(this->IntemediateImage,this);
            CompositeIntermediateLinearRLEPerspective<unsigned short>(this->IntemediateImage,this);
          break;
      }      
    }
  }
  else if (this->FastClassification == 1)
  {
    if (this->EncodedVolume != NULL)
    {
      delete this->EncodedVolume;
      this->EncodedVolume = NULL;

    }    

    if (this->Octree == NULL)
    {
      switch ( scalarType )
      {
        case VTK_UNSIGNED_CHAR:
          this->Octree = new vtkShearWarpOctree<unsigned char>();
          dynamic_cast< vtkShearWarpOctree<unsigned char> *>(this->Octree)->build(this->GetInput());
          break;
        case VTK_UNSIGNED_SHORT:
          this->Octree = new vtkShearWarpOctree<unsigned short>();
          dynamic_cast< vtkShearWarpOctree<unsigned short> *>(this->Octree)->build(this->GetInput());
          break;
      }
    }

    if (this->FunctionType == VTK_SHEAR_WARP_ISOSURFACE_FUNCTION)
    {
      if (!this->Octree->IsScalarEncoded() || this->Octree->GetIsoValue() != this->IsoValue)
      {        
        switch ( scalarType )
        {
          case VTK_UNSIGNED_CHAR:
            dynamic_cast< vtkShearWarpOctree<unsigned char> *>(this->Octree)->classifyScalar((unsigned char)this->IsoValue);
            break;
          case VTK_UNSIGNED_SHORT:
            dynamic_cast< vtkShearWarpOctree<unsigned short> *>(this->Octree)->classifyScalar((unsigned short)this->IsoValue);
            break;
        }
      }        
    }
    else
    {      
      unsigned long scalarOpacityMTime = this->Volume->GetProperty()->GetScalarOpacity()->GetMTime();

      if ( scalarOpacityMTime > this->ScalarOpacityMTime)
      {
        this->ScalarOpacityMTime =  scalarOpacityMTime;

        switch ( scalarType )
        {
          case VTK_UNSIGNED_CHAR:
            dynamic_cast< vtkShearWarpOctree<unsigned char> *>(this->Octree)->classifyOpacity(this->Volume);
            break;
          case VTK_UNSIGNED_SHORT:

            dynamic_cast< vtkShearWarpOctree<unsigned short> *>(this->Octree)->classifyOpacity(this->Volume);
            break;
        }
      }
    }    
    
    if ( interpolationType == VTK_NEAREST_INTERPOLATION )
    {
      // Nearest neighbor
      switch ( scalarType )
      {
        case VTK_UNSIGNED_CHAR:
          CompositeIntermediateNearestUnclassified<unsigned char>(this->IntemediateImage,this);
          break;
        case VTK_UNSIGNED_SHORT:
          CompositeIntermediateNearestUnclassified<unsigned short>(this->IntemediateImage,this);
          break;
      }
    }
    else
    {
      // Linear interpolation
      switch ( scalarType )
      {

        case VTK_UNSIGNED_CHAR:
          CompositeIntermediateLinearUnclassified<unsigned char>(this->IntemediateImage,this);
          break;
        case VTK_UNSIGNED_SHORT:
          CompositeIntermediateLinearUnclassified<unsigned short>(this->IntemediateImage,this);
          break;
      }
    }
  }
  else
  {
    if ( interpolationType == VTK_NEAREST_INTERPOLATION )
    {
      // Nearest neighbor
      switch ( scalarType )
      {
        case VTK_UNSIGNED_CHAR:
          CompositeIntermediateNearestSimple<unsigned char>(this->IntemediateImage,this);
          break;
        case VTK_UNSIGNED_SHORT:
          CompositeIntermediateNearestSimple<unsigned short>(this->IntemediateImage,this);
          break;
      }
    }
    else
    {
      // Linear interpolation
      switch ( scalarType )
      {
        case VTK_UNSIGNED_CHAR:
          CompositeIntermediateLinearSimple<unsigned char>(this->IntemediateImage,this);
          break;
        case VTK_UNSIGNED_SHORT:

          CompositeIntermediateLinearSimple<unsigned short>(this->IntemediateImage,this);
          break;
      }
    }
  }
  
  BuildImage(this->ImageData,this->IntemediateImage->GetPixelData());
}



void vtkVolumeShearWarpMapper::BuildImage(unsigned char *id, vtkShearWarpPixelData *im)
{
  int i,j;
  float red,green,blue,opacity;

//  int startX = int(float(this->ImageWidth) * 0.5f - float(this->IntermediateWidth) * 0.5f);
//  int startY = int(float(this->ImageHeight) * 0.5f - float(this->IntermediateHeight) * 0.5f);

	int startX = 0;
	int startY = 0;
  
  for (j = 0; j < this->ImageHeight; j++)

  {
    for (i = 0; i < this->ImageWidth; i++)
    {
      red = 0.0f;
      green = 0.0f;
      blue = 0.0f;
      opacity = 0.0f;

      if (i >= startX && j >= startY && i < startX+this->IntermediateWidth && j < startY+this->IntermediateHeight)
      {
        red = im->Red[im->Region];
        green = im->Green[im->Region];
        blue = im->Blue[im->Region];
        opacity = im->Opacity[im->Region];

        if (red > 1.0f)
          red = 1.0f;
        else if (red < 0.0f)
          red = 0.0f;

        if (green > 1.0f)

          green = 1.0f;
        else if (green < 0.0f)
          green = 0.0f;


        if (blue > 1.0f)
          blue = 1.0f;
        else if (blue < 0.0f)
          blue = 0.0f;

        if (opacity > 1.0f)
          opacity = 1.0f;
        else if (opacity < 0.0f)
          opacity = 0.0f;

        im++;
      }
      /*
      else
      {
        red = 1.0f;
        green = 0.0f;
        blue = 0.0f;
        opacity = 1.0f;
      }
      */
      id[0] = (unsigned char) (255.0f * red);
      id[1] = (unsigned char) (255.0f * green);
      id[2] = (unsigned char) (255.0f * blue);
      id[3] = (unsigned char) (255.0f * opacity);

      id += 4;
    }
  }
}

void vtkVolumeShearWarpMapper::InitializeClippingPlanes( vtkPlaneCollection *planes )
{
  vtkPlane *onePlane;
  float planePoint[4];
  float normalPoint[4];
  
  float *clippingPlane;
  float d;
  int i;  

  this->ClippingPlaneCount = planes->GetNumberOfItems();

  if (this->ClippingPlaneCount == 0)
    return;
 
  // loop through all clipping planes
  for ( i = 0; i < this->ClippingPlaneCount; i++ )
  {    
    onePlane = (vtkPlane *)planes->GetItemAsObject(i);

    onePlane->GetOrigin(planePoint);
    onePlane->GetNormal(normalPoint);
    normalPoint[0] += planePoint[0];
    normalPoint[1] += planePoint[1];
    normalPoint[2] += planePoint[2];
    planePoint[3] = 1.0;
    normalPoint[3] = 1.0;

    this->WorldToVoxelsMatrix->MultiplyPoint(planePoint,planePoint);
    this->WorldToVoxelsMatrix->MultiplyPoint(normalPoint,normalPoint);

    this->PermutationMatrix->MultiplyPoint(planePoint,planePoint);
    this->PermutationMatrix->MultiplyPoint(normalPoint,normalPoint);

    clippingPlane = this->ClippingPlane + 4*i;
    clippingPlane[0] = normalPoint[0] - planePoint[0]; 
    clippingPlane[1] = normalPoint[1] - planePoint[1];
    clippingPlane[2] = normalPoint[2] - planePoint[2];

    d = sqrt(clippingPlane[0] * clippingPlane[0] + clippingPlane[1] * clippingPlane[1] + clippingPlane[2] * clippingPlane[2]);
    clippingPlane[0] /= d;
    clippingPlane[1] /= d;
    clippingPlane[2] /= d;


    clippingPlane[3] = clippingPlane[0] * planePoint[0] + clippingPlane[1] * planePoint[1] + clippingPlane[2] * planePoint[2];    
  }
}


int vtkVolumeShearWarpMapper::IsVoxelClipped(int x, int y, int z)
{
	if (this->ImageSampleDistance != 1)
		return 0;


  float backDepth = this->ClippingBackBuffer[x + y*this->IntermediateWidth*this->ImageSampleDistance];
  float frontDepth = this->ClippingFrontBuffer[x + y*this->IntermediateWidth*this->ImageSampleDistance];

  if (this->ReverseOrder)
  {             
    if (z >= backDepth && z <= frontDepth)
    {
      return 1;
    }
  }
  else
  {
    
    if (z >= frontDepth && z <= backDepth)
    {
      return 1;
   }
  }

  return 0;
/*
  float *clippingPlane;

  for (int i=0; i<this->ClippingPlaneCount; i++)
  {
    clippingPlane = this->ClippingPlane + 4*i;
  
    if (clippingPlane[0] * (float)x +
        clippingPlane[1] * (float)y +
        clippingPlane[2] * (float)z < clippingPlane[3])
      return 1;
  }

  return 0;
*/  
}

void vtkVolumeShearWarpMapper::ComputeIntermediateImageExtent(float min[2], float max[2])
{
  float corner[4][4];  // corners of first and last voxel slice on intermediate image
  int i;

	corner[0][0] = 0.0;
	corner[0][1] = 0.0;
	corner[0][2] = 0.0;
	corner[0][3] = 1.0f;
	
	corner[1][0] = 0.0;
	corner[1][1] = 0.0;
	corner[1][2] = this->CountK - 1;
	corner[1][3] = 1.0f;

	corner[2][0] = this->CountI - 1;
	corner[2][1] = this->CountJ - 1;
	corner[2][2] = 0.0;
	corner[2][3] = 1.0f;
	
	corner[3][0] = this->CountI - 1;
	corner[3][1] = this->CountJ - 1;
	corner[3][2] = this->CountK - 1;
	corner[3][3] = 1.0f;

	this->ShearMatrix->MultiplyPoint(corner[0],corner[0]);
	this->ShearMatrix->MultiplyPoint(corner[1],corner[1]);
	this->ShearMatrix->MultiplyPoint(corner[2],corner[2]);
	this->ShearMatrix->MultiplyPoint(corner[3],corner[3]);

	for (i=0;i<4;i++)
	{
		corner[i][0] /= corner[i][3];
		corner[i][1] /= corner[i][3];
		corner[i][2] /= corner[i][3];
		corner[i][3] /= corner[i][3];
	}

  min[0] = corner[0][0];
  max[0] = corner[0][0];
  min[1] = corner[0][1];
  max[1] = corner[0][1];

  for (i=1; i<4; ++i)
	{
		min[0] = (corner[i][0] < min[0]) ? corner[i][0] : min[0];
    max[0] = (corner[i][0] > max[0]) ? corner[i][0] : max[0];
		min[1] = (corner[i][1] < min[1]) ? corner[i][1] : min[1];
    max[1] = (corner[i][1] > max[1]) ? corner[i][1] : max[1];
  }

}

// Print the vtkVolumeShearWarpMapper
void vtkVolumeShearWarpMapper::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkVolumeMapper::PrintSelf(os,indent);
}
