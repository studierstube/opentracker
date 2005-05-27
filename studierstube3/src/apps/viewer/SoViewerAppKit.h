/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
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
 * PROJECT: viewer
 * ======================================================================== */
/**
 * header file for StbViewerApp, a simple test application
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoViewerAppKit.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOVIEWERAPPKIT_H_
#define _SOVIEWERAPPKIT_H_

#include <inventor/nodes/SoGroup.h>
#include <stbapi/workspace/SoContextKit.h>
#include "viewer.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class VIEWER_API SoViewerAppKit : public SoContextKit
{
    SO_KIT_HEADER(SoViewerAppKit);
    SO_KIT_CATALOG_ENTRY_HEADER(toAuxGeom);

public:
	//static class initialization
    static void initClass();
	//constructor
    SoViewerAppKit();

    ~SoViewerAppKit();  

protected:
	virtual SbBool checkPipGeometry();

	virtual SbBool checkWindowGeometry();

    static void startUp( void * data, SoSensor * sensor );
}; //class SoViewerAppKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOVIEWERAPPKIT_H_

