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
/** SoSwitchSelect source file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoSwitchSelect.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoSwitchSelect.h"
#include "SoGroupSwitchElement.h"

#include <stbapi/event3d/SoHandle3DEventAction.h>

SO_NODE_SOURCE(SoSwitchSelect);

void SoSwitchSelect::initClass()
{
    SO_NODE_INIT_CLASS(SoSwitchSelect, SoNode, "Node");

    SO_ENABLE(SoCallbackAction, SoGroupSwitchElement);
    SO_ENABLE(SoGLRenderAction, SoGroupSwitchElement);
    SO_ENABLE(SoGetBoundingBoxAction, SoGroupSwitchElement);
    SO_ENABLE(SoGetMatrixAction, SoGroupSwitchElement);
    SO_ENABLE(SoHandleEventAction, SoGroupSwitchElement);
    SO_ENABLE(SoPickAction, SoGroupSwitchElement);
    SO_ENABLE(SoSearchAction, SoGroupSwitchElement);
    SO_ENABLE(SoHandle3DEventAction, SoGroupSwitchElement);

    // explicitly add to the SoHandle3DEventAction method table
    SoHandle3DEventAction::addMethod(SoSwitchSelect::getClassTypeId(), SoSwitchSelect::callDoAction);
}

// constructor

SoSwitchSelect::SoSwitchSelect()
{
    SO_NODE_CONSTRUCTOR(SoSwitchSelect);
    SO_NODE_ADD_FIELD(groups, (-1));
    isBuiltIn = TRUE;   
}

SoSwitchSelect::~SoSwitchSelect()
{}

void SoSwitchSelect::GLRender(SoGLRenderAction *action)
{
    doAction(action);
}

void SoSwitchSelect::callback(SoCallbackAction *action)
{
    doAction(action);
}

void SoSwitchSelect::getBoundingBox(SoGetBoundingBoxAction *action)
{
    doAction(action);
}

void SoSwitchSelect::handleEvent(SoHandleEventAction *action)
{
    doAction(action);
}

void SoSwitchSelect::pick(SoPickAction *action)
{
    doAction(action);
}

void SoSwitchSelect::getMatrix(SoGetMatrixAction *action)
{
    doAction(action);
}

void SoSwitchSelect::search(SoSearchAction *action)
{
    doAction(action);
    SoNode::search(action);
}

void SoSwitchSelect::doAction(SoAction *action)
{
    SoGroupSwitchElement::set(action->getState(), this, groups.getValues(0), groups.getNum());
}

void SoSwitchSelect::callDoAction(SoAction * action, SoNode * node)
{
    node->doAction( action );
}
