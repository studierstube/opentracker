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
/* SoMD2Puppeteer.cxx contains the implementation of class SoMD2Puppeteer
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoMD2Puppeteer.h 3426 2004-07-14 12:11:27Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _SOMD2PUPPETEER_H_
#define _SOMD2PUPPETEER_H_

// for DLL export
#include "md2Puppet.h"

// Inventor classes
#include "Inventor/fields/SoMFString.h"

// own classes
#include "../SoPuppeteerKit.h"

// Stb classes
#include <stbapi/misc/SoMD2NodeKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class MD2PUPPET_API SoMD2Puppeteer : public SoPuppeteerKit
{
    SO_KIT_HEADER(SoMD2Puppeteer);

public:

    SoMFString animationMap;

    SoMD2Puppeteer();
    ~SoMD2Puppeteer();    

SoINTERNAL public:

    static void initClass();

protected:

    bool idle;

    virtual bool implementCommand(SoNode *puppet);
    virtual void idleCommand(SoNode *puppet);

    void getAnimationName(const char *inputName,std::string *outputName,float *outputTime);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOMD2PUPPETEER_H_

