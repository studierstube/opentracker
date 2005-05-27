#ifndef COIN_SOMATERIALBUNDLE_H
#define COIN_SOMATERIALBUNDLE_H

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

#include <Inventor/bundles/SoBundle.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>

class SoGLLazyElement;

class COIN_DLL_API SoMaterialBundle : public SoBundle {
  typedef SoBundle inherited;
public:
  SoMaterialBundle(SoAction *action);
  ~SoMaterialBundle();

  void setUpMultiple(void);
  void sendFirst(void);
  void forceSend(const int index);
  SbBool isColorOnly(void) const;
  
  void send(const int index, const SbBool betweenbeginend);
  
private:

  void reallySend(const int index);
  void setupElements(const SbBool isbetweenbeginend);

  SbBool firsttime;
  SbBool coloronly;
  int currindex;
  const SoGLLazyElement * lazyelem;
};

#endif // !COIN_SOMATERIALBUNDLE_H
