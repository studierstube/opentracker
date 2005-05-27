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
/* SoCal3DPuppeteer.h contains the definition of class SoCal3DPuppeteer
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoCal3DPuppeteer.h 3426 2004-07-14 12:11:27Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _SOCAL3DPUPPETEER_H_
#define _SOCAL3DPUPPETEER_H_


// for DLL export
#include "cal3DPuppet.h"

// Inventor classes
#include "Inventor/fields/SoSFString.h"
#include "Inventor/fields/SoMFString.h"

// own classes
#include "../SoPuppeteerKit.h"
#include "SoCal3DPuppet.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class CAL3DPUPPET_API SoCal3DPuppeteer : public SoPuppeteerKit
{
    SO_KIT_HEADER(SoCal3DPuppeteer);

public:

    SoMFString animationMap;
    SoSFString animationPrefix;
    
    SoCal3DPuppeteer();
    ~SoCal3DPuppeteer();

SoINTERNAL public:

    static void initClass();

    const char* getAnimationName(const char *anim);

protected:

    virtual bool implementCommand(SoNode *puppet);
    virtual void idleCommand(SoNode *puppet);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOCAL3DPUPPETEER_H_
