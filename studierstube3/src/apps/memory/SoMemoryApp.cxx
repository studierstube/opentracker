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
 * PROJECT: memory
 * ======================================================================== */
/**
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoMemoryApp.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <assert.h>

#include "SoMemoryApp.h"

#include <stbapi/util/ivio.h>
#include <stbapi/workspace/SoContextKitSub.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include "SoMemoryCard.h"

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoMemoryApp);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(SoMemoryApp);

SoMemoryApp::SoMemoryApp()
{
	SO_KIT_CONSTRUCTOR(SoMemoryApp);
    SO_KIT_INIT_INSTANCE();
}

SoMemoryApp::~SoMemoryApp()
{
	destructor();
}

//static class initialization

void SoMemoryApp::initClass(){
	SO_KIT_INIT_CLASS(SoMemoryApp, SoContextKit, "SoContextKit");
	SoMemoryCard::initClass();
}

SoContextKit * SoMemoryApp::factory()
{
	return new SoMemoryApp;
}

SoWindowKit * SoMemoryApp::createWindowGeometry( int index )
{
	// create a default window
    WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    wc.size = SbVec3f(1,1,1);
    wc.position = SbVec3f(0,0,0);
    wc.orientation = SbRotation( 0,0,0,1);
    SoWindowKit* windowKit = comm->createWindow(NULL, &wc, NULL, NULL);

    // build scene graph composed of a material and a sphere node
    // and place it window's client area
    SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();
    SoSeparator* node = readFile("memory.iv",comm->workingDirectory);
    assert(node);
    clientVolume->addChild(node);
	return windowKit;
}

SoNode * SoMemoryApp::createPipSheet( int uid )
{
	comm->setContextAliasName(getName(),"Memory Game");
    SoSeparator * sheet = readFile("sheet.iv",comm->workingDirectory);
    return sheet;
}