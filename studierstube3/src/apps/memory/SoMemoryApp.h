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
/** header file for SoMemoryApp kit
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoMemoryApp.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SoMemoryApp_H_
#define _SoMemoryApp_H_

#include "memory.h"
#include <stbapi/workspace/SoContextKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
 * a small demo application implementing a simple memory game.
 * The application itself only loads a specific geometry file that
 * actually defines the memory game. The implementation of the memory game
 * resides entirely in the SoMemoryCard kit. 
 * @author Gerhard Reitmayr
 * @ingroup memory
 */
class MEMORY_API SoMemoryApp : public SoContextKit  
{

	SO_KIT_HEADER(SoMemoryApp);

public:
	SoMemoryApp();
	virtual ~SoMemoryApp();
	
	///static class initialization
    static void initClass();
	
	/** override this to create new context
	 *  of a specific type */
    SoContextKit* factory();                             

private:
	/** create window and content
	 * index of window */
    SoWindowKit* createWindowGeometry( int index );  
    
    SoNode* createPipSheet( int uid );
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SoMemoryApp_H_
