/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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

 /* ======================================================================= */
/** SignPost Augmentation
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: SoArchKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoArchKit_H_
#define _SoArchKit_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/widgets/SoButtonKit.h>

#include "signpost.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The Augmentation Application (AUG)
 * @ingroup signpost
 */
class SIGNPOST_API SoArchKit : public SoContextKit
{
    SO_KIT_HEADER(SoArchKit);

	SO_KIT_CATALOG_ENTRY_HEADER(architecture);


public:
    static void initClass();
    SoArchKit();
    ~SoArchKit();

    SoContextKit *factory();

    void checkPipMasterMode(SoNode *pipSheet, SbBool masterMode);

protected:
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

