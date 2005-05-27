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
  * $Id: SoVisibleSwitchEngine.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOVISIBLESWITCHENGINE_H_
#define _SOVISIBLESWITCHENGINE_H_

#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFInt32.h>
#include <stbapi/stbapi.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** This engine connects a switch to a field.
  * It is used to switch a part of the scene graph on or off by writing to a
  * field.
  * @ingroup util
  */
class STBAPI_API SoVisibleSwitchEngine : public SoEngine
{

    SO_ENGINE_HEADER(SoVisibleSwitchEngine);

public:

    /** These are the values the field uses.
      * When creating your field, don't forget to add this to your
      * constructor:
      * SO_NODE_DEFINE_ENUM_VALUE(Visible, HIDDEN);
      * SO_NODE_DEFINE_ENUM_VALUE(Visible, VISIBLE);
      * SO_NODE_SET_SF_ENUM_TYPE(myField, Visible);
      */
    enum Visible
    {
        HIDDEN= 0,  /// Triggers SO_SWITCH_NONE.
        VISIBLE     /// Triggers SO_SWITCH_ALL.
    };

public:

    /** Static class initialization.
      * Call this before instantiating an engine.
      */
    static void initClass();

    /** Default constructor.
      */
    SoVisibleSwitchEngine();

    /** Destructor.
      */
    ~SoVisibleSwitchEngine();

    /** Engine input.
      * Comes from the field holding the enum value.
      */
    SoSFEnum in;

    /** Engine output.
      * Goes to the SoSwitch.
      */
    SoEngineOutput out;

protected:

    /** Trigger function.
      */
    virtual void evaluate();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif


#endif
