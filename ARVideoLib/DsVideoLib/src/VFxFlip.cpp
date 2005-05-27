 /* ========================================================================
  * PROJECT: DirectShow Video Processing Library
  * File:    Video Deinterlacing Effect
  * Version: 0.0.6 (08/16/2003)
  * ========================================================================
  * Author:  Thomas Pintaric, Vienna University of Technology
  *          assembly code (c) 1998-2001 by Avery Lee (VirtualDub)
  * Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
  * =======================================================================
  *
  * Copyright (C) 2003  Vienna University of Technology
  * Portions Copyright (C) 1998-2001 Avery Lee (http://www.virtualdub.org)
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
  * For further information please contact Thomas Pintaric under
  * <pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================*/

#include "streams.h"
#include "VFxFlip.h"

void FlipImageRGB32(BYTE* pBuf, int width, int height, bool flipImageH, bool flipImageV);

// ----------------------------------------------------------------------------
CVFxFlipParameter::CVFxFlipParameter() :
	CVFxBaseParameter(),
	flip_h(false),
	flip_v(false)
{
	priority = default_FLIP_PRIORITY;
}

// ----------------------------------------------------------------------------
CVFxFlip::CVFxFlip(CVFxFlipParameter* _parameter) :
	parameter(_parameter),
	CVFxBase(_parameter)
{}

// ----------------------------------------------------------------------------
CVFxFlip::~CVFxFlip()
{}

// ----------------------------------------------------------------------------
HRESULT CVFxFlip::Initialize() { return(S_OK); }
HRESULT CVFxFlip::CleanUp()    { return(S_OK); }

// ----------------------------------------------------------------------------
HRESULT CVFxFlip::EffectProc(CVFxMediaSample sample)
{

	if(sample.pSample == NULL || sample.pType == NULL) return(E_INVALIDARG);

	BYTE* pixelBuffer = NULL;
	sample.pSample->GetPointer(&pixelBuffer);
    VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER*) sample.pType->pbFormat;
		// VIDEOINFOHEADER2 is backwards compatible with VIDEOINFOHEADER, so it's safe to typecast
    DWORD width    = pvi->bmiHeader.biWidth;
    DWORD height   = pvi->bmiHeader.biHeight;

	FlipImageRGB32(pixelBuffer, width, height, parameter->flip_h, parameter->flip_v);
	return(S_OK);
}

void FlipImageRGB32(BYTE* pBuf, int width, int height, 
					bool flipImageH, bool flipImageV)
{
	DWORD *ptr = (DWORD*)pBuf;
	int pixelCount = width * height;

	if (!pBuf)
		return;

    // Added code for more image manipulations
    // Gerhard Reitmayr <reitmayr@ims.tuwien.ac.at

    if( flipImageV )
    {
        if( flipImageH )
        {
            // both flips set -> rotation about 180 degree
	        for (int index = 0; index < pixelCount/2; index++)
	        {
		        ptr[index] = ptr[index] ^ ptr[pixelCount - index - 1];
		        ptr[pixelCount - index - 1] = ptr[index] ^ ptr[pixelCount - index - 1];
		        ptr[index] = ptr[index] ^ ptr[pixelCount - index - 1];
	        }
        } 
        else  
        {
            // only vertical flip 
            for( int line = 0; line < height/2; line++ )
                for( int pixel = 0; pixel < width; pixel ++ )
                {
                    ptr[line*width+pixel] = ptr[line*width+pixel] ^ ptr[pixelCount - line*width - (width - pixel )];
                    ptr[pixelCount - line*width - (width - pixel )] = ptr[line*width+pixel] ^ ptr[pixelCount - line*width - (width - pixel )];
                    ptr[line*width+pixel] = ptr[line*width+pixel] ^ ptr[pixelCount - line*width - (width - pixel )];
                }
        }
    }
    else 
    {
        if( flipImageH )
        {
            // only horizontal flip
            for( int line = 0; line < height; line++ )
                for( int pixel = 0; pixel < width/2; pixel ++ )
                {
                    ptr[line*width+pixel] = ptr[line*width+pixel] ^ ptr[line*width + (width - pixel )];
                    ptr[line*width + (width - pixel )] = ptr[line*width+pixel] ^ ptr[line*width + (width - pixel )];
                    ptr[line*width+pixel] = ptr[line*width+pixel] ^ ptr[line*width + (width - pixel )];
                }
        }
    }
}
