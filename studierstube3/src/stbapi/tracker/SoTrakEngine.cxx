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
/** Implementation of class SoTrakEngine
  *
  * @author Anton Fuhrmann, Dieter Schmalstieg, Andreas Zajic, Gerd Hesina
  *
  * $Id: SoTrakEngine.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>
#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include "SoTrakEngine.h"
#include <stbapi/event3d/Stb3DEventGenerator.h>

SO_ENGINE_SOURCE(SoTrakEngine);

//-----------------------------------------------------------------------------
// Constructor. initializes all the fields and internal values.

SoTrakEngine::SoTrakEngine() 
{
    SO_ENGINE_CONSTRUCTOR(SoTrakEngine);
    SO_ENGINE_ADD_INPUT(station, (0));
    SO_ENGINE_ADD_INPUT(translationIn, (0,0,0));
    SO_ENGINE_ADD_INPUT(rotationIn, (0,0,0,1));
    SO_ENGINE_ADD_INPUT(buttonIn0, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn1, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn2, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn3, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn4, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn5, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn6, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn7, (FALSE));
    //SO_ENGINE_ADD_INPUT(timeIn, (SbTime::zero()));
	
    SO_ENGINE_ADD_OUTPUT(translation, SoSFVec3f);
    SO_ENGINE_ADD_OUTPUT(rotation, SoSFRotation);
    SO_ENGINE_ADD_OUTPUT(button0, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button1, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button2, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button3, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button4, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button5, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button6, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button7, SoSFBool);

    //timeIn.connectFrom(SoDB::getGlobalField("realTime"));
    connectFields(0);
}

//-----------------------------------------------------------------------------

void
SoTrakEngine::connectFields(int i)
{
    translationIn.disconnect();
    rotationIn.disconnect();
    buttonIn0.disconnect();
    buttonIn1.disconnect();
    buttonIn2.disconnect();
    buttonIn3.disconnect();
    buttonIn4.disconnect();
    buttonIn5.disconnect();
    buttonIn6.disconnect();
    buttonIn7.disconnect();

    translationIn.connectFrom(Stb3DEventGenerator::getTranslationField(i));
    rotationIn.connectFrom(Stb3DEventGenerator::getRotationField(i));
    buttonIn0.connectFrom(Stb3DEventGenerator::getButtonField(i,0));
    buttonIn1.connectFrom(Stb3DEventGenerator::getButtonField(i,1));
    buttonIn2.connectFrom(Stb3DEventGenerator::getButtonField(i,2));
    buttonIn3.connectFrom(Stb3DEventGenerator::getButtonField(i,3));
    buttonIn4.connectFrom(Stb3DEventGenerator::getButtonField(i,4));
    buttonIn5.connectFrom(Stb3DEventGenerator::getButtonField(i,5));
    buttonIn6.connectFrom(Stb3DEventGenerator::getButtonField(i,6));
    buttonIn7.connectFrom(Stb3DEventGenerator::getButtonField(i,7));
}

//-----------------------------------------------------------------------------
// destructor. does nothing

SoTrakEngine::~SoTrakEngine() 
{
    // nil
}

//-----------------------------------------------------------------------------
// initClass. call before first usage

void 
SoTrakEngine::initClass() 
{
    SO_ENGINE_INIT_CLASS(SoTrakEngine, SoEngine, "Engine");
}

//-----------------------------------------------------------------------------
// evaluate. standard engine evaluation

void
SoTrakEngine::evaluate() 
{
//printf("evaluating engine %d\n",station.getValue());

    SO_ENGINE_OUTPUT(translation, SoSFVec3f, setValue(
        translationIn.getValue()));
    SO_ENGINE_OUTPUT(rotation, SoSFRotation, setValue(
        rotationIn.getValue()));
    SO_ENGINE_OUTPUT(button0,SoSFBool, setValue(
        buttonIn0.getValue()));
    SO_ENGINE_OUTPUT(button1,SoSFBool, setValue(
        buttonIn1.getValue()));
    SO_ENGINE_OUTPUT(button2,SoSFBool, setValue(
        buttonIn2.getValue()));
    SO_ENGINE_OUTPUT(button3,SoSFBool, setValue(
        buttonIn3.getValue()));
    SO_ENGINE_OUTPUT(button4,SoSFBool, setValue(
        buttonIn4.getValue()));
    SO_ENGINE_OUTPUT(button5,SoSFBool, setValue(
        buttonIn5.getValue()));
    SO_ENGINE_OUTPUT(button6,SoSFBool, setValue(
        buttonIn6.getValue()));
    SO_ENGINE_OUTPUT(button7,SoSFBool, setValue(
        buttonIn7.getValue()));
}

//-----------------------------------------------------------------------------

void
SoTrakEngine::inputChanged(SoField* whichField)
{
    if(whichField == &station)
    {
        connectFields(station.getValue());
    }
}

void SoTrakEngine::writeInstance( SoOutput * out )
{
    translationIn.disconnect();
    rotationIn.disconnect();
    buttonIn0.disconnect();
    buttonIn1.disconnect();
    buttonIn2.disconnect();
    buttonIn3.disconnect();
    buttonIn4.disconnect();
    buttonIn5.disconnect();
    buttonIn6.disconnect();
    buttonIn7.disconnect();

    SoEngine::writeInstance( out );

    connectFields(station.getValue());
}

//-----------------------------------------------------------------------------
//eof

