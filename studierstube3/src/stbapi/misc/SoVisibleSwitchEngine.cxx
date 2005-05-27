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
/** This file contains an engine to connect a switch to a field.
  * The field uses nice enum values (not SO_SWITCH_ALL, ...).
  * The engine can be used to switch something on or off by writing to a
  * field.
  *
  * @author Benjamin A. Schmit
  *
  * $Id: SoVisibleSwitchEngine.cxx 4030 2004-11-22 10:47:09Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "SoVisibleSwitchEngine.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

SO_ENGINE_SOURCE(SoVisibleSwitchEngine);


/// Static class initialization.

void SoVisibleSwitchEngine::initClass()
{
    SO_ENGINE_INIT_CLASS(SoVisibleSwitchEngine, SoEngine, "Engine");
}


/// Default constructor.

SoVisibleSwitchEngine::SoVisibleSwitchEngine()
{
    SO_ENGINE_CONSTRUCTOR(SoVisibleSwitchEngine);
    SO_ENGINE_DEFINE_ENUM_VALUE(Visible, HIDDEN);
    SO_ENGINE_DEFINE_ENUM_VALUE(Visible, VISIBLE);
    SO_ENGINE_SET_SF_ENUM_TYPE(in, Visible);
    SO_ENGINE_ADD_INPUT(in, (VISIBLE));
    SO_ENGINE_ADD_OUTPUT(out, SoSFInt32);
}


/// Destructor.

SoVisibleSwitchEngine::~SoVisibleSwitchEngine() {}


/// Trigger fuction.

void SoVisibleSwitchEngine::evaluate()
{
    if (in.getValue() == HIDDEN)
        SO_ENGINE_OUTPUT(out, SoSFInt32, setValue(-1));
    if (in.getValue() == VISIBLE)
        SO_ENGINE_OUTPUT(out, SoSFInt32, setValue(-3));
}
