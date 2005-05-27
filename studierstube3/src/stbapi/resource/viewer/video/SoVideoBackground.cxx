/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/**
 * implementation of the base SoVideoBackground node.
 *
 * @author Tamer Fahmy
 *
 * $Id: SoVideoBackground.cxx 4190 2005-05-06 06:47:24Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include "SoVideoBackground.h"
#include "SoVideoBackgroundSGI.h"
#include "SoVideoBackgroundUSB.h"
#include "SoVideoBackgroundIEEE1394.h"
#include "SoVideoBackgroundOT.h"
#include "SoVideoBackgroundH323.h"

#ifdef USE_DC1394
#include "SoVideoBackgroundDC1394.h"
#endif


SO_NODE_SOURCE(SoVideoBackground);

void
SoVideoBackground::initClass()
{ 
   SO_NODE_INIT_CLASS(SoVideoBackground, SoNode, "Node");
   SoVideoBackgroundSGI::initClass();
   SoVideoBackgroundUSB::initClass();
   SoVideoBackgroundIEEE1394::initClass();
   SoVideoBackgroundOT::initClass();
   SoVideoBackgroundH323::initClass();
#ifdef USE_DC1394
   SoVideoBackgroundDC1394::initClass();
#endif
}

SoVideoBackground::SoVideoBackground()
{
   SO_NODE_CONSTRUCTOR(SoVideoBackground);
   SO_NODE_ADD_FIELD(mirrorMode, (FALSE));
}
