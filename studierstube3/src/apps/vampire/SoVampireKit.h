 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
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
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */
/**
 * header file of the SoVampireKit
 *
 * @author Tamer Fahmy
 *
 * $Id: SoVampireKit.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SoVampireKit_H_
#define _SoVampireKit_H_

#include <Inventor/SbBasic.h>
#include <Inventor/SoPath.h>

#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/workspace/SoContextKit.h>


class SoVampireKit : public SoContextKit
{
   SO_KIT_HEADER(SoVampireKit);

  public:
   static void initClass();

   SoVampireKit();
   ~SoVampireKit() {}

   SoContextKit *factory();

  protected:
   SbBool windowEventCB(void *data, int message, SoWindowKit *window,
                        int userID, So3DEvent *event, SoPath *path);
};

#endif //_SoVampireKit_H_
