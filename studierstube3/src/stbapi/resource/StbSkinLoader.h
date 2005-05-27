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
/** StbSkinLoader.h contains the definition of class StbSkinLoader and
  * macros for getting nodes by name.
  *
  * @author Rainer Splechtna
  *
  * $Id: StbSkinLoader.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _STBSKINLOADER_H_
#define _STBSKINLOADER_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/SbString.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#define MAX_SKINS 8

#define SETUNIQUENAME(name,uPointer) \
{ \
    if (SoNode::getByName(name)) {\
        char buffer[100];\
        sprintf(buffer,"%s_%p",name,(void*)uPointer);\
        SoNode::getByName(name)->setName(buffer); \
    }\
}

#define UAGETNODE(var,type,name,uPointer) \
    type* var = NULL; \
    { \
        char buffer[100];\
        sprintf(buffer,"%s_%p",name,(void*)uPointer);\
        var=(type*)SoNode::getByName(buffer); \
    } \
    assert(var)

#define UAVGETNODE(var,type,name,uPointer) \
{ \
    char buffer[100];\
    sprintf(buffer,"%s_%p",name,(void*)uPointer);\
    var=(type*)SoNode::getByName(buffer); \
    assert(var); \
}
 
#define PUAGETNODE(var,type,prefix,name,uPointer) \
    type* var = NULL; \
    { \
        char buffer[100];\
        sprintf(buffer,"%s%s_%p",prefix,name,(void*)uPointer);\
        var=(type*)SoNode::getByName(buffer); \
    } \
    assert(var)

#define PUAVGETNODE(var,type,prefix,name,uPointer) \
{ \
    char buffer[100];\
    sprintf(buffer,"%s%s_%p",prefix,name,(void*)uPointer);\
    var=(type*)SoNode::getByName(buffer); \
    assert(var); \
}



//----------------------------------------------------------------------------

/** This class is used to manage skin-files.
    A skin-file is a simple IV-file containing a collection of nodes, which all
    have an arbitrary name (e.g.: nodes describing properties of an object,
    texture-nodes).
    The only public method is getSkin(), which returns a pointer to the
    skinLoader, which represents the skin, specified by a filename. Each skin 
    is only loaded once (better texture-handling).
    The skinLoader makes all node-names unique, when the skin is loaded.
    Therefore multiple skins containing nodes with the same name can be loaded
    without creating name-conflicts and identified by the skinLoader-pointer.
    Use the provided macros defined in this file to access the nodes of
    a skin-file.
@author Rainer Splechtna
@ingroup workspace
 */
class STBAPI_API StbSkinLoader
{

public:
    /** Returns a pointer to the skinLoader representing the skin
        with the specified fileName.
        If a skinLoader representing the skin with the specified fileName
        already exists, this skinLoader will be returned, otherwise a new
        skinLoader is created and returned.
        @param fileName name of the skin-file
        @return pointer to the skinLoader representing the skin
                with the specified fileName.
     */
    static StbSkinLoader *getSkin(const char *fileName);
private:
    /** Constructor, loads file and makes names of contained nodes unique. 
        @param fileName name of the skin-file
      */
    StbSkinLoader(const char *fileName               
        );
     /** Destructor, unrefs the skin. */
    ~StbSkinLoader(               
        );
    /** Name of the skin-file this instance represents. */
    SbString fileName;
    /** A pointer to the loaded skin-file. */
    SoSeparator *skin;
};

//----------------------------------------------------------------------------
#endif//_STBSKINLOADER_H_

