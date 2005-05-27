 /* ========================================================================
  * PROJECT: DirectShow Video Processing Library
  * File:    Video Deinterlacing Effect
  * Version: 0.0.6 (08/16/2003)
  * ========================================================================
  * Author:  Thomas Pintaric, Vienna University of Technology
  * Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
  * =======================================================================
  *
  * Copyright (C) 2003  Vienna University of Technology
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

#ifndef __VFX_FLIP__
#define __VFX_FLIP__

#include "VFxBase.h"

const unsigned int default_FLIP_PRIORITY = (default_VFX_PRIORITY + 0x06);
static const char *FLIP_internal_name= "VFxFlip";


// ----------------------------------------------------------------------------
class CVFxFlipParameter : public CVFxBaseParameter
{
public:
	CVFxFlipParameter();
	bool flip_h;
	bool flip_v;
};

// ----------------------------------------------------------------------------
class CVFxFlip : public CVFxBase
{
public:
	CVFxFlip(CVFxFlipParameter* _parameter = NULL);
	~CVFxFlip();

	const char* GetInternalName() { return(FLIP_internal_name); }
	HRESULT Initialize();
	HRESULT CleanUp();

	HRESULT EffectProc(CVFxMediaSample sample);
	CVFxFlipParameter* parameter;
};

// ----------------------------------------------------------------------------


#endif
