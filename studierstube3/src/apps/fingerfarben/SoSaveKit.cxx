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
 /** 
  * Implements SoSaveKit, a subclass of SoDropTarget to save geometry.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoSaveKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <memory>
#include <string>

#include <Inventor/actions/SoWriteAction.h>

#include "SoSaveKit.h"

using namespace std;

// OpenInventor macros

SO_KIT_SOURCE(SoSaveKit);
 
int SoSaveKit::numSaved = 0;

// class initialization for OpenInventor

void SoSaveKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoSaveKit, SoDropTarget, "SoDropTarget");
}

// constructor

SoSaveKit::SoSaveKit()
{
    SO_KIT_CONSTRUCTOR(SoSaveKit);

    SO_KIT_ADD_FIELD(filename, ("autosave"));

    SO_KIT_INIT_INSTANCE();
}

bool SoSaveKit::handleItem(SoFullPath* pathToItem)
{
    SoDropItem* item = (SoDropItem*)pathToItem->getTail();
    string cmd = item->commands.getValue().getString();

    // are we responsible for this item
    if ((cmd == "*") || (cmd.find("SAVE") > -1))
    {

        SoWriteAction wa;

        char fname[256];
        sprintf(fname,"%s%d.iv",filename.getValue().getString(),SoSaveKit::numSaved);

        wa.getOutput()->openFile(fname);
        wa.apply(item->getPart("geometry",FALSE));
        wa.getOutput()->closeFile();

        printf("SoSaveKit: saved file to %s", fname);

        SoSaveKit::numSaved++;

        return true;
    }

    return false;

}

