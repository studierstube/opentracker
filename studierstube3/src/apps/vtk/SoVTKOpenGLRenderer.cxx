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
 |   $Revision: 1.1 $
 |
 |   Classes:
 |      SoVTKOpenGLRenderer
 |
 |
 |   Author(s)          : Stefan Bruckner and Rudolf Seemann
 |
 _______________________________________________________________________
 */
#include "SoVTKOpenGLRenderer.h"

SoVTKOpenGLRenderer::SoVTKOpenGLRenderer(void)
{
}

SoVTKOpenGLRenderer::~SoVTKOpenGLRenderer(void)
{
}

// Concrete open gl render method.
void SoVTKOpenGLRenderer::DeviceRender(void)
{
/*  // Do not remove this MakeCurrent! Due to Start / End methods on
  // some objects which get executed during a pipeline update, 
  // other windows might get rendered since the last time
  // a MakeCurrent was called.
  */
  this->RenderWindow->MakeCurrent();
/*
  // standard render method 
*/
  this->ClearLights();
  this->UpdateCamera();
  this->UpdateLightGeometry();
  this->UpdateLights();

  // set matrix mode for actors 
  glMatrixMode(GL_MODELVIEW);

  this->UpdateGeometry();

  // clean up the model view matrix set up by the camera 
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  
}
