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
 |	This file defines the SoVTKOpenGLRenderWindow class.
 |
 |   Author(s)		: Stefan Bruckner, Rudolf Seemann
 |
 _______________________________________________________________________
 */
#ifndef _SO_VTKOPENGLRENDERWINDOW_
#define _SO_VTKOPENGLRENDERWINDOW_

#ifdef WIN32
#include <vtkWin32OpenGLRenderWindow.h>
#else
#include <vtkXOpenGLRenderWindow.h>
#endif

class SoVTKOpenGLRenderWindow :
#ifdef WIN32
	public vtkWin32OpenGLRenderWindow
#else
	public vtkXOpenGLRenderWindow
#endif
{
public:
	SoVTKOpenGLRenderWindow(void);
	~SoVTKOpenGLRenderWindow(void);
	void Frame(void);
	void MakeCurrent(void);

};
#endif // _SO_VTKOPENGLRENDERWINDOW_
