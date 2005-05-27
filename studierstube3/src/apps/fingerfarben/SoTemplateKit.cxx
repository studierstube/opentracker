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
  * Implements SoTemplateKit, a subclass of SoDropTarget that acts as a template to
  * replace the geometry of a SoDropItem.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoTemplateKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <memory>
#include <string>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SbBox.h>

#include <stbapi/event3d/Stb3DEventGenerator.h>


#include "SoTemplateKit.h"

using namespace std;

// OpenInventor macros

SO_KIT_SOURCE(SoTemplateKit);
 
// class initialization for OpenInventor

void SoTemplateKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoTemplateKit, SoDropTarget, "SoDropTarget");
}

// constructor

SoTemplateKit::SoTemplateKit()
{
    SO_KIT_CONSTRUCTOR(SoTemplateKit);
    SO_KIT_INIT_INSTANCE();
}

bool SoTemplateKit::handleItem(SoFullPath* pathToItem)
{
    SoDropItem* item = (SoDropItem*)pathToItem->getTail();
    string cmd = item->commands.getValue().getString();

    // are we responsible for this item
    if ((cmd == "*") || (cmd.find("TEMPLATE") > -1))
    {
        SoSearchAction sa;
        sa.setInterest(SoSearchAction::FIRST);
        sa.setName("TEMPLATE");
        sa.setSearchingAll(TRUE);
        sa.apply(this);

        if (sa.getPath() != NULL)
        {
            printf("SoTemplateKit: applying template...\n");
            SoNode* templ_geom = ((SoFullPath*)sa.getPath())->getTail();
            SoNode* new_geom = templ_geom->copy();
            new_geom->ref();
            if (item->getPart("geometry",true)->getName().getLength() > 0){
                new_geom->setName(item->getPart("geometry",true)->getName());
            }
            else {
                new_geom->setName("from_template");
            }

            item->setPart("geometry", new_geom);
            return true;
        }
        else 
            return false;
    }

    return false;

}

