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
/* SoFacePuppeteer.h contains the defintion of class SoFacePuppeteer
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoFacePuppeteer.h 3426 2004-07-14 12:11:27Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _SOFACEPUPPETEER_H_
#define _SOFACEPUPPETEER_H_

#include "facepuppet.h"
#include "../SoPuppeteerKit.h"
#include "SoFacePuppet.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class FACEPUPPET_API SoFacePuppeteer : public SoPuppeteerKit
{
    SO_KIT_HEADER(SoFacePuppeteer);

public:

    SoFacePuppeteer();
    ~SoFacePuppeteer();

SoINTERNAL public:

    static void initClass();

protected:

    virtual void whichPuppetsChanged(SoNode *puppet);
    virtual void showPuppeteerChanged(SoNode *puppet);

    virtual bool implementCommand(SoNode *puppet);
    virtual SoField* getFinishedSubCommandTrigger(SoNode *puppet);
    virtual const char* getFinishedSubCommandData(SoNode *puppet);
    virtual bool translateFinishedCommand(const char *current,const char *finished);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOFACEPUPPETEER_H_

