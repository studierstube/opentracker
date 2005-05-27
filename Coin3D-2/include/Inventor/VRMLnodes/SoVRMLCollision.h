/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLCOLLISION_H
#define COIN_SOVRMLCOLLISION_H

#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFTime.h>

class SoVRMLCollisionP;

class COIN_DLL_API SoVRMLCollision : public SoVRMLGroup
{
  typedef SoVRMLGroup inherited;
  SO_NODE_HEADER(SoVRMLCollision);

public:
  static void initClass(void);
  SoVRMLCollision(void);
  SoVRMLCollision(int numchildren);

  SoSFBool collide;
  SoSFNode proxy;
  SoSFTime collideTime;

  virtual void GLRender(SoGLRenderAction * action);

  virtual void notify(SoNotList * list);

protected:
  virtual ~SoVRMLCollision();

private:
  void commonConstructor(void);
  SoVRMLCollisionP * pimpl;
};

#endif // ! COIN_SOVRMLCOLLISION_H
