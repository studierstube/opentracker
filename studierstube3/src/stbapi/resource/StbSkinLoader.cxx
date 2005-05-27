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
/** StbSkinLoader.h contains the implementation of class StbSkinLoader. 
  *
  * @author Rainer Splechtna
  *
  * $Id: StbSkinLoader.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cstdlib>
#include <stbapi/util/ivio.h>
#include "StbSkinLoader.h"

//----------------------------------------------------------------------------
/** Constructor, loads file and makes names of contained nodes unique. */

StbSkinLoader::StbSkinLoader(const char *fileName)
    : fileName(fileName), skin(NULL)
{
    // load the skin file
    printf("loading skin %s ...\n",fileName);
    skin = readFile(fileName, "");
    if(!skin) {
        printf("error: could not load file \"%s\"\n",
                fileName);
        exit(-1);
    }
    skin->ref();

    for(int i=0; i<((SoSeparator *)skin)->getNumChildren(); i++)
    {
        SoNode* n = skin->getChild(i);
        if (n->getName() != "")
            SETUNIQUENAME(n->getName().getString(),this);
    }

}

//----------------------------------------------------------------------------
/** Destructor, unrefs the skin. */

StbSkinLoader::~StbSkinLoader() 
{
    if (skin)
        skin->unref();
}

//----------------------------------------------------------------------------
/** Returns skinLoader representing the skin with the specified fileName. */

StbSkinLoader *
StbSkinLoader::getSkin(const char *fileName)
{
    static int count = 0;
    static StbSkinLoader *skins[MAX_SKINS];
    for (int i = 0; i<count; i++)
    {
        if (fileName == skins[i]->fileName) return skins[i];
    }
    if (count < MAX_SKINS) 
        return skins[count++] = new StbSkinLoader(fileName);
    return NULL;
}

/* ===========================================================================
    End of StbSkinLoader.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */

