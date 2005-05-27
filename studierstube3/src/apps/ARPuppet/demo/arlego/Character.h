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
/* Character.h contains the definition of class Character
 *
 * @author Istvan Barakonyi
 *
 * $Id: Character.h 3745 2004-09-22 02:31:22Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

// for DLL export
#include "arlego.h"

// Cal3D classes
#include <cal3d/skeleton.h>
#include <cal3d/vector.h>

// Inventor classes
#include "Inventor/fields/SoMFInt32.h"
#include "Inventor/fields/SoMFVec3f.h"

// own classes
#include "../../characters/cal3DPuppet/SoCal3DPuppeteer.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/// This class is derived from SoCal3DPuppeteer and implements application-specific commands
class ARLEGO_API Character : public SoCal3DPuppeteer
{
    SO_KIT_HEADER(Character);

public:

    enum IdleMode { DEFAULT=0, TILE_IN_1HAND, TILE_IN_2HANDS };

    SoMFInt32 hotspotBoneID;

    SoMFVec3f hsOffset;

    Character();
    ~Character();

SoINTERNAL public:

    static void initClass();

protected:

    /// Internal status variable effecting the idle behavior
    int idleMode;

    /// Internal pointer for associated Cal3D puppet
    SoCal3DPuppet *cal3dPuppet;

    /// Quick pointer to the Cal3D puppets skeleton
    CalSkeleton *skeleton;

    /// overriden virtual functions
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);
    virtual void commandLoopFunction();
    virtual bool implementCommand(SoNode *puppet);
    virtual void idleCommand(SoNode *puppet);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _CHARACTER_H_
