 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: SignPost
  * ======================================================================== */
/** header file for SoInfoKit
  *
  * @author Gerhard Reitmayr, Michael Knapp
  * 
  * $Id: SoInfoKit.h 3426 2004-07-14 12:11:27Z tamer $
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoInfoKit_H_
#define _SoInfoKit_H_

#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/workspace/SoContextKit.h>

#include "signpost.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SIGNPOST_API SoInfoKit : public SoContextKit
{

    SO_KIT_HEADER(SoInfoKit);

     /// some part
    // SO_KIT_CATALOG_ENTRY_HEADER(  );

public:
    /** static method to initialize class for OpenInventor.
     */
    static void initClass();

    /// constructor
    SoInfoKit();

    SoMFNode pickedPath;

    SoSFNode currentIcon;

    SoMFString info;

    SoSFString destination;

    SoSFInt32 index;
    
protected:
    
    /// destructor
    virtual ~SoInfoKit();

    static void pathChanged( void * data, SoSensor * sensor );
    SoFieldSensor pathSensor;

    void setPickedIcon( SoNode * icon, int childIndex );
};

#endif // _SoInfoKit_H_
