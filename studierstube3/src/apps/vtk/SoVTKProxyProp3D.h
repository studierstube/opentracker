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

//  -*- C++ -*-

/*
 * Copyright (C) 2003
 *
 _______________________________________________________________________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file defines the SoVTKProxyProp3D node class.
 |
 |   Author(s)		: Stefan Bruckner, Rudolf Seemann
 |
 _______________________________________________________________________
 */

#ifndef  _SO_VTKPROXYPROP3D_
#define  _SO_VTKPROXYPROP3D_

// Open Inventor includes
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

// VTK includes
#include <vtkProp3D.h>
#include <vtkMatrix4x4.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoVTKProxyProp3D
//
//  integrates a vtkVolume into an OpenInventor scenegraph.
//
//////////////////////////////////////////////////////////////////////////////

class SoVTKProxyProp3D : public SoNode
{
    SO_NODE_HEADER(SoVTKProxyProp3D);

public:

	vtkMatrix4x4 *m_pModelviewMatrix;
	// member variable holds vtkProp3D
	vtkProp3D *m_pvtkProp3D;

	// Constructor
	SoVTKProxyProp3D(void);

	void SetVTKProp3D(vtkProp3D *prop);
	void Update(vtkMatrix4x4 *matrix);
  SoEXTENDER public:
    virtual void	GLRender(SoGLRenderAction *action);

  SoINTERNAL public:
    static void		initClass();

  protected:
    virtual ~SoVTKProxyProp3D(void);

};

#endif /* _SO_VTKPROXYPROP3D_ */
