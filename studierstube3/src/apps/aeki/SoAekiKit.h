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
/** c++ header for SoAekiKit - Aeki application class
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: SoAekiKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOAEKIKIT_H_
#define _SOAEKIKIT_H_

#include "aeki.h"
#include <stbapi/workspace/SoContextKit.h>

class SoEventCallback;
class So3DButton;

/**
 * 
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class AEKI_API SoAekiKit : public SoContextKit {
	SO_KIT_HEADER(SoAekiKit);

public:
	static void initClass();                                     // Constructors etc.
	SoAekiKit();
	SoContextKit* factory();
	~SoAekiKit();

	// Callback functions
	/** Callback function called when prev or next was pressed */
	static void stepButtonCB(void* data, So3DButton* button);    
	/** Callback function called when rev or fwd was pressed */
	static void resetButtonCB(void* data, So3DButton* button);
	/** callback function called for keyboard events */
	static void eventCB(void * data, SoEventCallback * node );

protected:   
    virtual SbBool checkWindowGeometry();

    virtual void checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode );
};

#endif

/* ===========================================================================
    End of SoAekiKit.h
=========================================================================== */
