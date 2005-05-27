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
/** SoApplicationKit node kit class definition
  *
  * @author Andreas Zajic
  *
  * $Id: SoApplicationKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file SoApplicationKit.h                                                */
 /* ======================================================================= */
#ifndef _SOAPPLICATIONKIT_H_
#define _SOAPPLICATIONKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/workspace/SoClassLoader.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/workspace/appdef.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


/** SoApplicationKit is a container class for all data associated with an application
 * The SoApplicationKit node kit class functions as a wrapper class that encapsulates all 
 * OIV nodes that construct the application. It is the node that is actually read from the
 * loader file and attached to the scene graph.
 * @ingroup workspace
 * @ingroup core
 */
class STBAPI_API SoApplicationKit
:
public SoBaseKit
{
    SO_KIT_HEADER(SoApplicationKit);
    
    SO_KIT_CATALOG_ENTRY_HEADER(classLoader);    
    SO_KIT_CATALOG_ENTRY_HEADER(contextKit);
    /** This part contains a sub tree that is searched for the first SoTexture2 node
     * to supply a texture to use for the task bar buttons. A default texture 
     * stored in the graphix directory is used, if no SoTexture2 node is supplied.
     */
    SO_KIT_CATALOG_ENTRY_HEADER(appGeom);
    SO_KIT_CATALOG_ENTRY_HEADER(info);
public:

    /** Flag that prevents the loader file from overwriting.*/
    SoSFBool readOnly;

    /** File name of the loader file.*/
    SoSFString loadFileName;

    static void initClass();
 
    SoApplicationKit();

	SoContextKit* getContext() {return (SoContextKit*)getPart("contextKit",FALSE);}
    SoSeparator* getAppGeom() {return (SoSeparator*)getPart("appGeom",FALSE);}

    void setClassLoader(SoClassLoader* loader) { setPart("classLoader",loader);}
    void setContextKit(SoContextKit* ctx) { setPart("contextKit",ctx);}
    void setAppGeom(SoSeparator* geom) { setPart("appGeom",geom);}
    void setInfo(SoInfo* inf) { info.setValue(inf);}

private:

    virtual ~SoApplicationKit();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOAPPLICATIONKIT_H_
