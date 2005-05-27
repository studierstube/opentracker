/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkVolumeShearWarpMapper.h,v $
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
// .NAME vtkVolumeShearWarpMapper - Abstract class for a volume mapper

// .SECTION Description
// vtkVolumeShearWarpMapper is the abstract definition of a volume mapper
// that uses a shear-warp factorization approach.

// .SECTION see also
// vtkVolumeMapper

#ifndef __vtkVolumeShearWarpMapper_h
#define __vtkVolumeShearWarpMapper_h

#include "vtkVolumeMapper.h"
#include "vtkEncodedGradientShader.h"
#include "vtkEncodedGradientEstimator.h"

#include "vtkStructuredPoints.h"
#include "vtkCamera.h"
#include "vtkVolume.h"
#include "vtkImageData.h"
#include "vtkVolumeProperty.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkTransform.h"
#include "vtkFloatArray.h"
#include "vtkOpenGLActor.h"


class vtkShearWarpClippingActor;


class vtkVolume;
class vtkRenderer;
class vtkRenderWindow;

#define VTK_X_AXIS  0
#define VTK_Y_AXIS  1
#define VTK_Z_AXIS  2

#define VTK_SHEAR_WARP_COMPOSITE_FUNCTION      0
#define VTK_SHEAR_WARP_MIP_FUNCTION            1
#define VTK_SHEAR_WARP_ISOSURFACE_FUNCTION     2

#define VTK_SHEAR_WARP_TRANSPARENT_CLIPPING    3

#define VTK_SHEAR_WARP_OCTREE_TRANSPARENT      0
#define VTK_SHEAR_WARP_OCTREE_NONTRANSPARENT   1
#define VTK_SHEAR_WARP_OCTREE_COMBINATION      2
#define VTK_SHEAR_WARP_OCTREE_MINIMUM_SIZE     16

#define VTK_SHEAR_WARP_MAXIMUM_REGIONS				 4
                                               

// Intermediate image pixel data for early ray termination
class vtkShearWarpPixelData
{
public:
  float Red[VTK_SHEAR_WARP_MAXIMUM_REGIONS];
  float Green[VTK_SHEAR_WARP_MAXIMUM_REGIONS];
  float Blue[VTK_SHEAR_WARP_MAXIMUM_REGIONS];
  float Opacity[VTK_SHEAR_WARP_MAXIMUM_REGIONS];
  float Value[VTK_SHEAR_WARP_MAXIMUM_REGIONS];

	int State;
	int Region;
  int Offset;
};

// Runlength encoded intermediate image
class vtkShearWarpRLEImage
{
public:
  inline vtkShearWarpRLEImage(int size)
  {
    this->PixelData = new vtkShearWarpPixelData[size];
    this->ImageSize = size;
    this->Clear();      
  };

  inline ~vtkShearWarpRLEImage()
  {
    if (this->PixelData != NULL)
      delete[] this->PixelData;
  };

  // Reset all pixels to default values
  inline void Clear()
  {
    for (int i = 0; i < this->ImageSize; i++)
    {
			for (int j=0; j < VTK_SHEAR_WARP_MAXIMUM_REGIONS; j++)
			{
				this->PixelData[i].Red[j] = 0.0f;
				this->PixelData[i].Green[j] = 0.0f;
				this->PixelData[i].Blue[j] = 0.0f;
				this->PixelData[i].Opacity[j] = 0.0f;
				this->PixelData[i].Value[j] = 0.0f;
			}

			this->PixelData[i].Region = -1;
			this->PixelData[i].State = -1;

      this->PixelData[i].Offset = 0;
    }
  };

  // Reset the current pixel pointer to the first pixel
  inline void First(vtkShearWarpPixelData * &ptr)
  {
    ptr = this->PixelData;
  };

  // Sets the current pixel pointer to the specified position
  inline void Position (vtkShearWarpPixelData * &ptr, int position)
  {
    ptr = this->PixelData + position;
  }; 

  // Advances the current pixel pointer by the specified increment
  inline void Advance(vtkShearWarpPixelData * &ptr, int count)
  {
    ptr = ptr + count;
  };

  // Skip over transparent voxels and returns the count of skipped voxels
  inline int Skip(vtkShearWarpPixelData * &ptr)
  {
    vtkShearWarpPixelData *data = ptr;
    int runLength = 0;
    int pathLength = 0;
    int offset = 0;
    
    while (ptr->Offset > 0)
    {
      runLength += ptr->Offset;
      ptr += ptr->Offset;
    }

    data->Offset = runLength;

    while  (data->Offset > 0)
    {
      offset = data->Offset;
      data->Offset = runLength - pathLength;
      pathLength += offset;
      data += offset;
    }
      
    return runLength;
  };
        
  // Retrieves a pointer to the first pixel
  inline vtkShearWarpPixelData * GetPixelData()
  {
    return this->PixelData;
  };

  // Retrieves the allocated image size
  inline int GetSize()
  {
    return this->ImageSize;
  };

	inline void Combine()
	{
		vtkShearWarpPixelData *data = this->PixelData;

		for (int i=0;i<GetSize();i++)
		{
			for (int j=0;j<data[i].Region;j++)
			{
				float oldRed = data[i].Red[j];
				float oldGreen = data[i].Green[j];
				float oldBlue = data[i].Blue[j];
				float oldOpacity = data[i].Opacity[j];

				float red = data[i].Red[j+1];
				float green = data[i].Green[j+1];
				float blue = data[i].Blue[j+1];
				float opacity = data[i].Opacity[j+1];

				float newRed = oldRed + opacity * red * (1.0f - oldOpacity);
				float newGreen = oldGreen + opacity * green * (1.0f - oldOpacity);
				float newBlue = oldBlue + opacity * blue * (1.0f - oldOpacity);
				float newOpacity = oldOpacity + opacity * (1.0f - oldOpacity);

				data[i].Red[j+1] = newRed;
				data[i].Green[j+1] = newGreen;
				data[i].Blue[j+1] = newBlue;
				data[i].Opacity[j+1] = newOpacity;
			}
		}
	};

private:
  // the pixel data
  vtkShearWarpPixelData *PixelData;

  // the allocated image size
  int ImageSize;

};


// Voxel data for runlength encoding, contains the scalar value and shading information
template <class T>
struct vtkShearWarpVoxelData
{
public:
  T Value;
  unsigned short EncodedNormal;
  unsigned char GradientMagnitude;  
};



// An RLE run. It has a length and a pointer to the first voxel.
template <class T>
class vtkShearWarpRLERun
{
public:
  unsigned char Length;
  vtkShearWarpVoxelData<T> *VoxelData;
};


// A runlength encoded voxel slice which provides scanline-wise access to the data
template <class T>
class vtkShearWarpRLESlice
{
public:
  inline vtkShearWarpRLESlice()
  {
    this->SliceRuns = NULL;
    this->VoxelData = NULL;
    this->LineRuns = NULL;
  };

  inline ~vtkShearWarpRLESlice()
  {
    if (this->LineRuns != NULL)
      delete[] this->LineRuns;

    if (this->SliceRuns != NULL)
      delete[] this->SliceRuns;

    if (this->VoxelData != NULL)
      delete[] this->VoxelData;
  };
  
  
  // Encodes the data by opacity (for alpha compositing)
  inline void encodeOpacity (vtkImageData* data, vtkVolume *volume, vtkEncodedGradientEstimator *gradest, int axis, int k, float opacityThreshold)
  {
    if (data == NULL || volume == NULL)
      return;

    T voxelScalar = 0;
    float voxelOpacity = 0.0f;
    unsigned char currentIndex = 0;
    int currentRun = 0;
    int location = 0;
    bool transparentRun = false;

    float *SOTF = volume->GetCorrectedScalarOpacityArray();

    T *dptr = (T*) data->GetScalarPointer();
    unsigned short *nptr = gradest->GetEncodedNormals();
    unsigned char *gptr = gradest->GetGradientMagnitudes();

    int i,j;
    
    int jCount = 0;
    int iCount = 0;
    int voxelIndex = 0;

    int *dimensions = data->GetDimensions();
    int plane = dimensions[0] * dimensions[1];
    int iIncrement,jIncrement;
    int vi,vj,vk;

    switch (axis)
    {
      case VTK_X_AXIS:
        iCount = dimensions[1];
        jCount = dimensions[2];
        vk = k;
        iIncrement = dimensions[0];
        jIncrement = plane;
        break;

      case VTK_Y_AXIS:
        iCount = dimensions[2];
        jCount = dimensions[0];
        vk = k*dimensions[0];
        iIncrement = plane;
        jIncrement = 1;
        break;

      case VTK_Z_AXIS:
      default:
        iCount = dimensions[0];
        jCount = dimensions[1];
        vk = k*plane;
        iIncrement = 1;
        jIncrement = dimensions[0];
        break;
    }
 
    // First we determine the number of runs in the slice
    for (j=0,vj=0; j<jCount;j++, vj += jIncrement)
    {
      for (i=0,vi=0; i<iCount;i++, vi += iIncrement)
      {
        location = vi + vj + vk;
        voxelScalar = dptr[location];        
        voxelOpacity = SOTF[voxelScalar];

        if (voxelOpacity > opacityThreshold)
        {
          if (!transparentRun && i > 0 && currentIndex < 254)
          {
            currentIndex++;
          }
          else
          {
            currentIndex = 0;
            currentRun++;
            transparentRun = false;
          }

          voxelIndex++;
          
        }
        else
        {
          if (transparentRun && i > 0 && currentIndex < 254)
          {
            currentIndex++;
          }
          else
          {
            currentIndex = 0;
            currentRun++;
            transparentRun = true;
          }
        }
      }
    }

    this->LineRuns = new vtkShearWarpRLERun<T>*[jCount];
    this->SliceRuns = new vtkShearWarpRLERun<T>[currentRun];
    this->VoxelData = new vtkShearWarpVoxelData<T>[voxelIndex];

    vtkShearWarpRLERun<T> *activeRun = &this->SliceRuns[0];

    voxelIndex = 0;
    currentRun = 0;

    // Now we run-length-encode the slice
    for (j=0,vj=0; j<jCount;j++, vj += jIncrement)
    {
      this->LineRuns[j] = activeRun;
      currentIndex = 0;

      for (i=0,vi=0; i<iCount;i++, vi += iIncrement)
      {
        location = vi + vj + vk;        
        voxelScalar = dptr[location];        
        voxelOpacity = SOTF[voxelScalar];

        if (voxelOpacity > opacityThreshold)
        {
         
          if (!transparentRun && i > 0 && currentIndex < 254)
          {
            currentIndex++;
          }
          else
          {
            if (i > 0)
            {
              activeRun->Length = currentIndex + 1;
              activeRun++;
              currentRun++;
              currentIndex = 0;
            }
                                                 
            activeRun->VoxelData = &this->VoxelData[voxelIndex];
            transparentRun = false;
          }

          // Set voxel data
          this->VoxelData[voxelIndex].Value = voxelScalar;
          this->VoxelData[voxelIndex].EncodedNormal = nptr[location];
          this->VoxelData[voxelIndex].GradientMagnitude = gptr[location];
          
          voxelIndex++;
        }
        else
        {
          if (transparentRun && i > 0 && currentIndex < 254)
          {
            currentIndex++;
          }
          else
          {
            if (i > 0)
            {
              activeRun->Length = currentIndex + 1;
              activeRun++;
              currentRun++;
              currentIndex = 0;
            }
              
            activeRun->VoxelData = NULL;
            transparentRun = true;
          }
        }
      }

      activeRun->Length = currentIndex + 1;
      activeRun++;
      currentRun++;

    }                                                                                                  
  };

  // Encodes the data by scalar value (for isosurface display)
  inline void encodeScalar (vtkImageData* data, vtkVolume *volume, vtkEncodedGradientEstimator *gradest, int axis, int k, float isoValue)
  {
    if (data == NULL || volume == NULL)
      return;
    
    T voxelScalar = 0;
    unsigned char currentIndex = 0;
    int currentRun = 0;
    int location = 0;
    bool transparentRun = false;

    T *dptr = (T*) data->GetScalarPointer();
    unsigned short *nptr = gradest->GetEncodedNormals();
    unsigned char *gptr = gradest->GetGradientMagnitudes();

    int i,j;

    int jCount = 0;
    int iCount = 0;
    int voxelIndex = 0;

    int *dimensions = data->GetDimensions();
    int plane = dimensions[0] * dimensions[1];
    int iIncrement,jIncrement;
    int vi,vj,vk;

    switch (axis)
    {
      case VTK_X_AXIS:
        iCount = dimensions[1];
        jCount = dimensions[2];
        vk = k;
        iIncrement = dimensions[0];
        jIncrement = plane;
        break;

      case VTK_Y_AXIS:
        iCount = dimensions[2];
        jCount = dimensions[0];
        vk = k*dimensions[0];
        iIncrement = plane;
        jIncrement = 1;
        break;

      case VTK_Z_AXIS:
      default:
        iCount = dimensions[0];
        jCount = dimensions[1];
        vk = k*plane;
        iIncrement = 1;
        jIncrement = dimensions[0];        
        break;
    }

    // First we determine the number of runs in the slice
    for (j=0,vj=0; j<jCount;j++, vj += jIncrement)
    {
      for (i=0,vi=0; i<iCount;i++, vi += iIncrement)
      {
        location = vi + vj + vk;
        voxelScalar = dptr[location];

        if (voxelScalar >= isoValue)
        {
          if (!transparentRun && i > 0 && currentIndex < 254)
          {
            currentIndex++;
          }
          else
          {
            currentIndex = 0;
            currentRun++;
            transparentRun = false;
          }

          voxelIndex++;

        }
        else
        {
          if (transparentRun && i > 0 && currentIndex < 254)
          {
            currentIndex++;
          }
          else
          {
            currentIndex = 0;
            currentRun++;
            transparentRun = true;
          }
        }
      }
    }

    this->LineRuns = new vtkShearWarpRLERun<T>*[jCount];
    this->SliceRuns = new vtkShearWarpRLERun<T>[currentRun];
    this->VoxelData = new vtkShearWarpVoxelData<T>[voxelIndex];

    vtkShearWarpRLERun<T> *activeRun = this->SliceRuns;

    voxelIndex = 0;
    currentRun = 0;

    // Now we run-length-encode the slice
    for (j=0,vj=0; j<jCount;j++, vj += jIncrement)
    {
      this->LineRuns[j] = activeRun;
      currentIndex = 0;

      for (i=0,vi=0; i<iCount;i++, vi += iIncrement)
      {
        location = vi + vj + vk;
        voxelScalar = dptr[location];

        if (voxelScalar >= isoValue)
        {
          if (!transparentRun && i > 0 && currentIndex < 254)
          {
            currentIndex++;
          }
          else
          {
            if (i > 0)
            {
              activeRun->Length = currentIndex + 1;
              activeRun++;
              currentRun++;
              currentIndex = 0;
            }

            activeRun->VoxelData = &this->VoxelData[voxelIndex];
            transparentRun = false;
          }

          // Set voxel data
          this->VoxelData[voxelIndex].Value = voxelScalar;
          this->VoxelData[voxelIndex].EncodedNormal = nptr[location];
          this->VoxelData[voxelIndex].GradientMagnitude = gptr[location];

          voxelIndex++;
        }
        else
        {
          if (transparentRun && i > 0 && currentIndex < 254)
          {
            currentIndex++;
          }
          else
          {
            if (i > 0)
            {
              activeRun->Length = currentIndex + 1;
              activeRun++;
              currentRun++;
              currentIndex = 0;
            }

            activeRun->VoxelData = NULL;
            transparentRun = true;
          }
        }
      }

      activeRun->Length = currentIndex + 1;
      activeRun++;
      currentRun++;
    }
  };

  // Returns a pointer to the first run of a specified scanline
  inline vtkShearWarpRLERun<T> * GetLineRuns(int line)
  {
    return this->LineRuns[line];
  }
  
private:
  // pointers to the first run for every scanline
  vtkShearWarpRLERun<T> **LineRuns;

  // all runs of the slice
  vtkShearWarpRLERun<T> *SliceRuns;

  // the voxel data of the slice
  vtkShearWarpVoxelData<T> *VoxelData;
          
};

// Base class for encoded volume
class vtkShearWarpBase
{
public:
  inline vtkShearWarpBase()
  {
    this->VolumeDimensions[0] = 0;
    this->VolumeDimensions[1] = 0;
    this->VolumeDimensions[2] = 0;
  };
  
  inline virtual ~vtkShearWarpBase()
  {
  };
  
  // Returns the volume dimensions
  inline int * GetDimensions()
  {
    return this->VolumeDimensions;
  };

  // Returns the encoded isovalue, if the volume is scalar encoded
  inline float GetIsoValue()
  {
    return this->IsoValue;
  };

  // Returns true if the volume is opacity encoded
  inline bool IsOpacityEncoded()
  {
    return (this->OpacityEncoded == 1);
  };
  
  // Returns true if the volume is scalar encoded
  inline bool IsScalarEncoded()
  {
    return (!this->OpacityEncoded && this->IsoValue >= 0.0);
  }          

protected:
  // the volume dimensions
  int VolumeDimensions[3];

  // the encoded isovalue
  float IsoValue;

  // encoding type flag
  int OpacityEncoded;    
};


// A runlength encoded volume. It contains voxel data encoded for each major viewing direction.
template <class T>
class vtkShearWarpRLEVolume : public vtkShearWarpBase
{
public:
  inline vtkShearWarpRLEVolume()
  {
    for (int l=0;l<3;l++)
      this->EncodedSlices[l] = NULL;

    this->OpacityEncoded = 0;
    this->IsoValue = -1.0f;
  };

  inline virtual ~vtkShearWarpRLEVolume()
  {
    for (int l=0;l<3;l++)
      if (this->EncodedSlices[l] != NULL)
        delete[] this->EncodedSlices[l];
  };

  // Encodes the volume by opacity (for alpha-compositing)
  inline void encodeOpacity(vtkImageData *data, vtkVolume *volume, vtkEncodedGradientEstimator* gradest, float opacityThreshold)
  {
		int l;

    this->IsoValue = -1.0f;
    this->OpacityEncoded = 1;
    
    for (l=0;l<3;l++)
      if (this->EncodedSlices[l] != NULL)
        delete[] this->EncodedSlices[l];
    
    int *dimensions = data->GetDimensions();    
    this->Volume = volume;
    this->VolumeDimensions[0] = dimensions[0];
    this->VolumeDimensions[1] = dimensions[1];
    this->VolumeDimensions[2] = dimensions[2];

    for (l=0; l<3; l++)
    {
      this->EncodedSlices[l] = new vtkShearWarpRLESlice<T>[dimensions[l]]();
      
      for (int k = 0; k < dimensions[l]; k++)
          this->EncodedSlices[l][k].encodeOpacity(data,volume,gradest,l,k,opacityThreshold);
    }
  };

  // Encodes the volume by scalar (for isosurface display)
  inline void encodeScalar(vtkImageData *data, vtkVolume *volume, vtkEncodedGradientEstimator* gradest, float isoValue)
  {
		int l;

    this->IsoValue = isoValue;
    this->OpacityEncoded = 0;    
    
    for (l=0;l<3;l++)
      if (this->EncodedSlices[l] != NULL)
        delete[] this->EncodedSlices[l];

    int *dimensions = data->GetDimensions();
    this->Volume = volume;
    this->VolumeDimensions[0] = dimensions[0];
    this->VolumeDimensions[1] = dimensions[1];
    this->VolumeDimensions[2] = dimensions[2];

    for (l=0; l<3; l++)
    {
      this->EncodedSlices[l] = new vtkShearWarpRLESlice<T>[dimensions[l]]();

      for (int k = 0; k < dimensions[l]; k++)
        this->EncodedSlices[l][k].encodeScalar(data,volume,gradest,l,k,isoValue);
    }
  };

  // Returns the slice
  inline vtkShearWarpRLESlice<T> * GetSlice(int axis, int slice)
  {
    return &this->EncodedSlices[axis][slice];
  };

  // Returns a pointer to the source volume
  inline vtkVolume * GetVolume()
  {
    return this->Volume;
  };


private:
  // the encoded slices for all three principal axes
  vtkShearWarpRLESlice<T> *EncodedSlices[3];

  // the source volume
  vtkVolume *Volume;

};

template <class T>
class vtkShearWarpSummedAreaTable
{
public:
  inline vtkShearWarpSummedAreaTable()
  {
    this->Table = new float[2 << ((sizeof(T)*8)-1)];
    this->Opacity = NULL;
  };

  inline ~vtkShearWarpSummedAreaTable()
  {
    if (this->Table)
      delete[] this->Table;
  };

  inline void build(float *SOTF, T upper)
  {
    this->Table[0] = SOTF[0];
    this->Opacity = SOTF;

    for (int i=1;i<=upper;i++)
    {
      this->Table[i] = this->Table[i-1] + SOTF[i];
    }    
  };

  inline float integrate(T min, T max)
  {
    if (min != max)
      return this->Table[max] - this->Table[min];
    else
      return this->Opacity[min];
    
  };

private:
  float *Table;
  float *Opacity;
};

struct vtkShearWarpOctreeRun
{
  unsigned short Length;
  unsigned char Type;
};

template <class T>
class vtkShearWarpOctreeNode
{
public:
  inline vtkShearWarpOctreeNode()
  {
    this->Children = NULL;
  };
  
  inline ~vtkShearWarpOctreeNode()
  {
    if (this->Children != NULL)
      delete[] this->Children;
  };

  inline T GetMinimum()
  {
    return this->Minimum;
  };

  inline T GetMaximum()
  {
    return this->Maximum;
  };

  inline void build(vtkImageData *data, int min[3], int max[3], int level)
  {
//    cout << "minX: " << min[0] << " - minY: " << min[1] << " - minZ: " << min[2] << "\n";
//    cout << "maxX: " << max[0] << " - maxY: " << max[1] << " - maxZ: " << max[2] << "\n\n";
    
    if (this->Children != NULL)
    {
      delete[] this->Children;
      this->Children = NULL;
    }
    
    if (max[0] <= min[0] && max[1] <= min[1] && max[2] <= min[2])
    {
      this->Minimum = *((T*) data->GetScalarPointer(max[0],max[1],max[2]));
      this->Maximum = this->Minimum;
    }
    else
    {
      int center[3] = {(max[0]+min[0]) / 2, (max[1]+min[1]) / 2, (max[2]+min[2]) / 2};
      int newMin[3];
      int newMax[3];
      this->Children = new vtkShearWarpOctreeNode<T>[8];

      newMin[0] = min[0];
      newMin[1] = min[1];
      newMin[2] = min[2];
      newMax[0] = center[0];
      newMax[1] = center[1];
      newMax[2] = center[2];
      this->Children[0].build(data,newMin,newMax,level+1);
      
      newMin[0] = center[0]+1;
      newMin[1] = min[1];
      newMin[2] = min[2];
      newMax[0] = max[0];
      newMax[1] = center[1];
      newMax[2] = center[2];
      this->Children[1].build(data,newMin,newMax,level+1);

      newMin[0] = min[0];
      newMin[1] = center[1]+1;
      newMin[2] = min[2];
      newMax[0] = center[0];
      newMax[1] = max[1];
      newMax[2] = center[2];
      this->Children[2].build(data,newMin,newMax,level+1);
      
      newMin[0] = center[0]+1;
      newMin[1] = center[1]+1;
      newMin[2] = min[2];
      newMax[0] = max[0];
      newMax[1] = max[1];
      newMax[2] = center[2];
      this->Children[3].build(data,newMin,newMax,level+1);
    
      newMin[0] = min[0];
      newMin[1] = min[1];
      newMin[2] = center[2]+1;
      newMax[0] = center[0];
      newMax[1] = center[1];
      newMax[2] = max[2];
      this->Children[4].build(data,newMin,newMax,level+1);

      newMin[0] = center[0]+1;
      newMin[1] = min[1];
      newMin[2] = center[2]+1;
      newMax[0] = max[0];
      newMax[1] = center[1];
      newMax[2] = max[2];
      this->Children[5].build(data,newMin,newMax,level+1);

      newMin[0] = min[0];
      newMin[1] = center[1]+1;
      newMin[2] = center[2]+1;
      newMax[0] = center[0];
      newMax[1] = max[1];
      newMax[2] = max[2];
      this->Children[6].build(data,newMin,newMax,level+1);

      newMin[0] = center[0]+1;
      newMin[1] = center[1]+1;
      newMin[2] = center[2]+1;
      newMax[0] = max[0];
      newMax[1] = max[1];
      newMax[2] = max[2];
      this->Children[7].build(data,newMin,newMax,level+1);

      this->Minimum = this->Children[0].Minimum;
      this->Maximum = this->Children[0].Maximum;

      bool equalMinimum = true;
      bool equalMaximum = true;
      
      for (int i=1; i < 8; i++)
      {
        if (this->Minimum != this->Children[i].Minimum)
        {        
          if (this->Children[i].Minimum < this->Minimum)
            this->Minimum = this->Children[i].Minimum;

          equalMinimum = false;
        }

        if (this->Maximum != this->Children[i].Maximum)
        {
          if (this->Children[i].Maximum > this->Maximum)
            this->Maximum = this->Children[i].Maximum;

          equalMaximum = false;
        }
      }

      // If minimum and maximum of all children are equal, we can remove them
      if (equalMinimum && equalMaximum)
      {
        delete[] this->Children;
        this->Children = NULL;
      }
      else
      {
        // Remove children if node already is at the lowest level
/*        if ((max[0] - min[0] + 1) <= VTK_SHEAR_WARP_OCTREE_MINIMUM_SIZE &&
            (max[1] - min[1] + 1) <= VTK_SHEAR_WARP_OCTREE_MINIMUM_SIZE &&
            (max[2] - min[2] + 1) <= VTK_SHEAR_WARP_OCTREE_MINIMUM_SIZE)*/
        if (level >= 4)
        {
          delete[] this->Children;
          this->Children = NULL;
        }
      }
    }
  };

  inline void classifyOpacity(vtkShearWarpSummedAreaTable<T> *table)
  {
    float integral = table->integrate(this->Minimum,this->Maximum);

    if (integral == 0.0f)
    {
      this->Status = VTK_SHEAR_WARP_OCTREE_TRANSPARENT;
    }
    else if (this->Children == NULL)
    {
      this->Status = VTK_SHEAR_WARP_OCTREE_NONTRANSPARENT;
    }
    else
    {
      this->Status = VTK_SHEAR_WARP_OCTREE_COMBINATION;

      for (int i = 0; i < 8; i++)
        this->Children[i].classifyOpacity(table);
    }
  };  

  inline void classifyScalar(T value)
  {
    if (this->Minimum >= value || this->Maximum >= value)
    {
      if (this->Children == NULL)
      {
        this->Status = VTK_SHEAR_WARP_OCTREE_NONTRANSPARENT;      
      }
      else
      {
        this->Status = VTK_SHEAR_WARP_OCTREE_COMBINATION;

        for (int i = 0; i < 8; i++)
          this->Children[i].classifyScalar(value);
      }
    }
    else
    {
      this->Status = VTK_SHEAR_WARP_OCTREE_TRANSPARENT;
    }
  };

  inline int computeRuns(vtkShearWarpOctreeRun *& runs, int axis, int slices, int lines, int voxels, int slice, int line)
  {
    static const int increments[3][3] = {{2,4,1},{4,1,2},{1,2,4}};
            
    if (this->Status == VTK_SHEAR_WARP_OCTREE_COMBINATION)
    {
      int child = 0;
//      int half = size / 2;
      int halfSlices = slices / 2;
      int halfLines = lines / 2;
      int halfVoxels = voxels / 2;

      if (slice > halfSlices)
      {
        child += increments[axis][2];
        slice -= halfSlices;

        halfSlices = slices - halfSlices;
      }
        
      if (line > halfLines)
      {
        child += increments[axis][1];
        line -= halfLines;

        halfLines = lines - halfLines;
      }

      int a = this->Children[child].computeRuns(runs, axis, halfSlices, halfLines, halfVoxels, slice,line);
      int b = this->Children[child + increments[axis][0]].computeRuns(runs, axis, halfSlices, halfLines, voxels-halfVoxels, slice,line);

      if (a < b)
        return a;
      else
        return b;
    }
    else
    {
      if (runs->Type == this->Status)
      {
        runs->Length += voxels;//size;
      }
      else
      {
        if (runs[0].Type != 255)          
          runs++;

        runs->Type = this->Status;
        runs->Length = voxels;//size;        
      }

      return voxels;//size;
      
    }
  }

private:  
  vtkShearWarpOctreeNode<T> *Children;
  unsigned char Status;
  T Minimum;
  T Maximum;
};

template <class T>
class vtkShearWarpOctree : public vtkShearWarpBase
{
public:
  inline vtkShearWarpOctree()
  {
  };

  inline virtual ~vtkShearWarpOctree()
  {
    
  };

  inline void build(vtkImageData* data)
  {
    int min[3],max[3];
    data->GetDimensions(this->Dimensions);
    data->GetExtent(min[0],max[0],min[1],max[1],min[2],max[2]);
    this->Root.build(data,min,max,0);
  };

  inline void classifyOpacity(vtkVolume* volume)
  {
    this->Table.build(volume->GetCorrectedScalarOpacityArray(),this->Root.GetMaximum());
    this->Root.classifyOpacity(&this->Table);
    this->OpacityEncoded = 1;
  };

  inline void classifyScalar(T value)
  {
    this->Root.classifyScalar(value);
    this->OpacityEncoded = 0;
  };

  inline int GetLineRuns(vtkShearWarpOctreeRun *runs, int axis, int slice, int line)
  {
    static const int sizes[3][3] = {{this->Dimensions[1],this->Dimensions[2],this->Dimensions[0]},
                                    {this->Dimensions[2],this->Dimensions[0],this->Dimensions[1]},
                                    {this->Dimensions[0],this->Dimensions[1],this->Dimensions[2]}};
        
    runs[0].Type = 255;
    runs[0].Length = 0;
    return this->Root.computeRuns(runs,axis,/*this->Dimensions[axis],*/sizes[axis][2],sizes[axis][1],sizes[axis][0],slice,line);
  };
  
private:
  vtkShearWarpOctreeNode<T> Root;
  vtkShearWarpSummedAreaTable<T> Table;
  int Dimensions[3];
  
};

class /*VTK_RENDERING_EXPORT*/ vtkVolumeShearWarpMapper : public vtkVolumeMapper
{
  friend class vtkShearWarpClippingActor;
//BTX
  template <class T>
  friend void CompositeIntermediateNearestSimple(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis);

  template <class T>
  friend void CompositeIntermediateLinearSimple(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis);
  
  template <class T>
  friend void CompositeIntermediateNearestRLE(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis);

  template <class T>
  friend void CompositeIntermediateLinearRLE(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis);

  template <class T>
  friend void CompositeIntermediateNearestUnclassified(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis);

  template <class T>
  friend void CompositeIntermediateLinearUnclassified(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis);

  template <class T>
  friend void CompositeIntermediateLinearRLEPerspective(vtkShearWarpRLEImage *image, vtkVolumeShearWarpMapper *myThis);
//ETX
  
public:
  vtkTypeMacro(vtkVolumeShearWarpMapper,vtkVolumeMapper);
  void PrintSelf( ostream& os, vtkIndent index );

  static vtkVolumeShearWarpMapper *New();  

  // Description:
  // Update the volume rendering pipeline by updating the scalar input
  virtual void Update();

  // Description:
  // Set / Get the gradient estimator used to estimate normals
  void SetGradientEstimator( vtkEncodedGradientEstimator *gradest );
  vtkGetObjectMacro( GradientEstimator, vtkEncodedGradientEstimator );

  // Description:
  // Get the gradient shader.
  vtkGetObjectMacro( GradientShader, vtkEncodedGradientShader );
  
  // Description:
  // Set/Get the value of IsoValue.
  vtkSetMacro( IsoValue, float );
  vtkGetMacro( IsoValue, float );

  // Description:
  // Enable/Disable runlength encoding
  vtkSetMacro(RunlengthEncoding,int);
  vtkGetMacro(RunlengthEncoding,int);
  vtkBooleanMacro(RunlengthEncoding,int);

  // Description:
  // Enable/Disable classification optimaziation (fast classification)
  vtkSetMacro(FastClassification,int);
  vtkGetMacro(FastClassification,int);
  vtkBooleanMacro(FastClassification,int);

  // Description:
  // Get/Set the compositing function type.
  vtkSetClampMacro( FunctionType, int,
        VTK_SHEAR_WARP_COMPOSITE_FUNCTION, VTK_SHEAR_WARP_ISOSURFACE_FUNCTION);
  vtkGetMacro(FunctionType,int);
  void SetFunctionTypeToComposite()
        {this->SetFunctionType(VTK_SHEAR_WARP_COMPOSITE_FUNCTION);};
  void SetFunctionTypeToMIP()
        {this->SetFunctionType(VTK_SHEAR_WARP_MIP_FUNCTION);};
  void SetFunctionTypeToIsosurface()
        {this->SetFunctionType(VTK_SHEAR_WARP_ISOSURFACE_FUNCTION);};

  // Description:
  // Get/Set the clipping mode.
  vtkSetClampMacro( ClippingMode, int,
        VTK_SHEAR_WARP_COMPOSITE_FUNCTION,VTK_SHEAR_WARP_TRANSPARENT_CLIPPING);
  vtkGetMacro(ClippingMode,int);
	void SetClippingModeToComposite()
				{this->SetClippingMode(VTK_SHEAR_WARP_COMPOSITE_FUNCTION);};
	void SetClippingModeToMIP()
				{this->SetClippingMode(VTK_SHEAR_WARP_MIP_FUNCTION);};
	void SetClippingModeToIsosurface()
				{this->SetClippingMode(VTK_SHEAR_WARP_ISOSURFACE_FUNCTION);};
	void SetClippingModeToTransparent()
				{this->SetClippingMode(VTK_SHEAR_WARP_TRANSPARENT_CLIPPING);};

	vtkVolumeProperty * GetClippingProperty()
	{
		return this->ClippingProperty;
	};

  int Debug;

  // Description:
  // Sampling distance. Default value of 1 meaning that every voxel is being
  // processed, 2 causes every second voxel to be processed, etc.
  vtkSetClampMacro( ImageSampleDistance, int, 1, 32 );
  vtkGetMacro( ImageSampleDistance, int );

  // Description:
  // This is the minimum image sample distance allow when the image
  // sample distance is being automatically adjusted
  vtkSetClampMacro( MinimumImageSampleDistance, int, 1, 32 );
  vtkGetMacro( MinimumImageSampleDistance, int );

  // Description:
  // This is the maximum image sample distance allow when the image
  // sample distance is being automatically adjusted
  vtkSetClampMacro( MaximumImageSampleDistance, int, 1, 32 );
  vtkGetMacro( MaximumImageSampleDistance, int );

  // Description:
  // If AutoAdjustSampleDistances is on, the the ImageSampleDistance
  // will be varied to achieve the allocated render time of this
  // prop (controlled by the desired update rate and any culling in
  // use).
  vtkSetClampMacro( AutoAdjustSampleDistances, int, 0, 1 );
  vtkGetMacro( AutoAdjustSampleDistances, int );
  vtkBooleanMacro( AutoAdjustSampleDistances, int );

  // Description:
  // If IntermixIntersectingGeometry is turned on, the zbuffer will be
  // captured and used to limit the traversal of the rays.
  vtkSetClampMacro( IntermixIntersectingGeometry, int, 0, 1 );
  vtkGetMacro( IntermixIntersectingGeometry, int );
  vtkBooleanMacro( IntermixIntersectingGeometry, int );
                              
	void SetClippingRenderer (vtkRenderer *renderer)
	{
		this->ClippingRenderer = renderer;
	};

	//BTX
  
  // Description:
  // WARNING: INTERNAL METHOD - NOT INTENDED FOR GENERAL USE
  // DO NOT USE THIS METHOD OUTSIDE OF THE RENDERING PROCESS
  // Render the volume
  virtual void Render(vtkRenderer *ren, vtkVolume *vol);

//ETX

protected:
  vtkVolumeShearWarpMapper();
  ~vtkVolumeShearWarpMapper();

  // Objects / variables  needed for shading / gradient magnitude opacity
  vtkEncodedGradientEstimator  *GradientEstimator;
  vtkEncodedGradientShader     *GradientShader;
  int                          Shade;

  float          *RedDiffuseShadingTable;
  float          *GreenDiffuseShadingTable;
  float          *BlueDiffuseShadingTable;
  float          *RedSpecularShadingTable;
  float          *GreenSpecularShadingTable;
  float          *BlueSpecularShadingTable;

  unsigned short *EncodedNormals;
  unsigned char  *GradientMagnitudes;

  vtkTransform *PerspectiveTransform;
  vtkMatrix4x4 *PerspectiveMatrix;
  vtkMatrix4x4 *ViewToWorldMatrix;
  vtkMatrix4x4 *ViewToVoxelsMatrix;
  vtkMatrix4x4 *VoxelsToViewMatrix;
  vtkMatrix4x4 *WorldToVoxelsMatrix;
  vtkMatrix4x4 *VoxelsToWorldMatrix;
  vtkMatrix4x4 *VoxelTransformMatrix;
  vtkMatrix4x4 *ViewportMatrix;
  vtkMatrix4x4 *ShearMatrix;
  vtkMatrix4x4 *WarpMatrix;
  vtkMatrix4x4 *PermutationMatrix;
  vtkMatrix4x4 *PermutedViewToVoxelsMatrix;
  vtkMatrix4x4 *PermutedVoxelsToViewMatrix;

  int           IntermixIntersectingGeometry;
  int           ZBufferSize[2];
  float         MinimumViewDistance;
  vtkFloatArray	*ZBuffer;

	vtkFloatArray *IntermediateZArray;
	vtkFloatArray *ClippingBackArray;
	vtkFloatArray *ClippingFrontArray;

	float					*IntermediateZBuffer;  
  float         *ClippingBackBuffer;
  float         *ClippingFrontBuffer;


  vtkVolume *Volume;
  vtkShearWarpBase *EncodedVolume;
  vtkShearWarpBase *Octree;
  vtkShearWarpRLEImage *IntemediateImage;
  unsigned char *ImageData;

  int ImageWidth;
  int ImageHeight;
  int AllocatedSize;
  
  unsigned long ScalarOpacityMTime;
  int FunctionType;
  int ClippingMode;
  float IsoValue;
  int RunlengthEncoding;
  int FastClassification;

  int CountI;
  int CountJ;
  int CountK;
    
  int ReverseOrder;
  int MajorAxis;
  int ParallelProjection;

  int IntermediateWidth;
  int IntermediateHeight;
  int MaximumIntermediateDimension;

  float ShearI;
  float ShearJ;
  float TranslationI;
  float TranslationJ;
  float Scale;

  // Depth cueing stuff
  /*
  float DepthI;
  float DepthJ;
  float DepthK;
  float Depth0;
  float DeltaDepth;
  float FrontDepth;
  float DepthDensity;
  float DepthTable[512];
  int DepthTableSize;
  */
  float ClippingPlane[4*6];
  int ClippingPlaneCount;

  // This is how big the image would be if it covered the entire viewport
  int ImageViewportSize[2];
    
  double WorldViewingDirection[4];
  double ObjectViewingDirection[4];
  double StandardViewingDirection[4];

  double WorldEyePosition[4];
  double ObjectEyePosition[4];
  double StandardEyePosition[4];

  double ClippingRange[2];

  // The distance between sample points along the ray
  int                          ImageSampleDistance;
  int                          MinimumImageSampleDistance;
  int                          MaximumImageSampleDistance;
  int                          AutoAdjustSampleDistances;

  float        *RenderTimeTable;
  vtkVolume   **RenderVolumeTable;
  vtkRenderer **RenderRendererTable;
  int           RenderTableSize;
  int           RenderTableEntries;

  void StoreRenderTime( vtkRenderer *ren, vtkVolume *vol, float t );
  float RetrieveRenderTime( vtkRenderer *ren, vtkVolume *vol );

  void ComputeMatrices( vtkImageData *data, vtkVolume *vol );
  void FactorViewMatrix();
  void ComputeViewMatrix();
  void ComputeViewportMatrix();
  void ComputePrincipalAxisParallel();
  void ComputePrincipalAxisPerspective();
  void ComputePermutationMatrix();
  void ComputeShearMatrixParallel();
  void ComputeShearMatrixPerspective();
  void ComputeWarpMatrix();
  void CompositeIntermediate();
  void BuildImage(unsigned char *id, vtkShearWarpPixelData *im);

	void ComputeSlicePosition(int slice, float *start, float *end);
	void ComputeIntermediateImageExtent(float min[2], float max[2]);

  void InitializeClippingPlanes( vtkPlaneCollection *planes );
  int IsVoxelClipped(int x, int y, int z);

  // void ComputeDepthTable(int first, int last);
  //void DepthCueImage (vtkShearWarpPixelData *im, int slice);

  virtual void RenderTexture(vtkRenderer *ren, vtkVolume *vol) = 0;
	virtual float * Extract(vtkFloatArray *array, vtkRenderer *ren, vtkVolume *vol) = 0;

	vtkRenderer *ClippingRenderer;
	vtkVolumeProperty *ClippingProperty;
	vtkVolume *ClippingVolume;
	float ClippingOpacity;
   
private:
  vtkVolumeShearWarpMapper(const vtkVolumeShearWarpMapper&);  // Not implemented.
  void operator=(const vtkVolumeShearWarpMapper&);  // Not implemented.
};

#endif


