#ifndef COIN_SOGLPOLYGONOFFSETELEMENT_H
#define COIN_SOGLPOLYGONOFFSETELEMENT_H

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

#include <Inventor/elements/SoPolygonOffsetElement.h>

class COIN_DLL_API SoGLPolygonOffsetElement : public SoPolygonOffsetElement {
  typedef SoPolygonOffsetElement inherited;

  SO_ELEMENT_HEADER(SoGLPolygonOffsetElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLPolygonOffsetElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

protected:
  virtual void setElt(float factor, float units, Style styles, SbBool on);

private:

  SoState * state; // needed to test for OpenGL extension
  void updategl(void);
};

#endif // !COIN_SOGLPOLYGONOFFSETELEMENT_H
