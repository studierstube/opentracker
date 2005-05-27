  /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
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

#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#include <SoWinLeaveScope.h>
#endif

#include <Inventor/fields/SoMFString.h>

#include <stbapi/util/ivio.h>

#include "SoStateEngine.h"
#include "SoLightProjector.h"

#define DEBUG

SO_ENGINE_SOURCE(SoStateEngine);

void
SoStateEngine::initClass()
{
    SO_ENGINE_INIT_CLASS(SoStateEngine, SoEngine, "Engine");

    SoLightProjector::initClass();
}

SoStateEngine::SoStateEngine()
{
    SO_ENGINE_CONSTRUCTOR(SoStateEngine);

    SO_ENGINE_ADD_INPUT(specFile, (""));
    SO_ENGINE_ADD_INPUT(eventIn, (""));

    SO_ENGINE_ADD_OUTPUT(currentState, SoMFString);
    SO_ENGINE_ADD_OUTPUT(exitStates, SoMFString);
    SO_ENGINE_ADD_OUTPUT(availableTransitions, SoMFString);

    stateEngine = NULL;
}

SoStateEngine::~SoStateEngine()
{
	delete stateEngine;
}

void SoStateEngine::inputChanged(SoField * whichField)
{
	if (whichField == &eventIn)
	{
		stateEngine->event(eventIn.getValue().getString());
	}
	if (whichField == &specFile)
	{
        // FIXME: this introduces a dependency on stbapi.dll
        // if you don't want that, copy the code of checkFile
        // into a local utility function.
        // gerhard 03.09.2003
        SbString name = specFile.getValue();
        SbBool isValid;
        if( checkFile( name, isValid ))
        {
            stateEngine = new StateEngine();
	    	stateEngine->readFromFile( name.getString());
        }
        else
            printf("SoStateEngine: could not find story file %s !\n", name.getString());
	}
}

void SoStateEngine::evaluate()
{
    if (stateEngine == NULL) return;

    unsigned int i;
    vector<State*> states;
    
    states = stateEngine->getCurrentStates();
    
	SO_ENGINE_OUTPUT(currentState, SoMFString, setNum(states.size()));
	for (i=0; i<states.size(); i++) {
        SO_ENGINE_OUTPUT(currentState, SoMFString, set1Value(i,states[i]->name.c_str()));
    }

    states = stateEngine->getExitStates();
    
	SO_ENGINE_OUTPUT(exitStates, SoMFString, setNum(states.size()));
	for (i=0; i<states.size(); i++) {
        SO_ENGINE_OUTPUT(exitStates, SoMFString, set1Value(i,states[i]->name.c_str()));
    }

	vector<Transition*> trans = stateEngine->getAvailableTransitions();

	SO_ENGINE_OUTPUT(availableTransitions, SoMFString, setNum(trans.size()));
#ifdef DEBUG
    printf("SoStateEngine: available transitions#: %d\n", trans.size());
#endif
	for (i=0; i<trans.size(); i++) {
		SO_ENGINE_OUTPUT(availableTransitions, SoMFString, set1Value(i,trans[i]->event.c_str()));
	}
}



