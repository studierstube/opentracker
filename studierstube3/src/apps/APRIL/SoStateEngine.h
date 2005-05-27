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
  * ======================================================================== */

#ifndef _SOSTATEENGINE_H_
#define _SOSTATEENGINE_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFString.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "StateEngine.h"
#include "april.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
@ingroup april

A state engine node. The state engine operates on string tokens and can be read from an XML specification.
The state engine is operating as a black box, reding events from its input and outputting the current state and
available transitions to its outputs.

For the syntax of the specifiaction file and other details, see the StateEngine class. This is just an OIV
wrapper for the generic state engine implemented there.

Together with a set of ConditionalTrigger nodes for converting in and out tokens to field values, quite complex stateful
applications can be built by simple OIV-scripting.
*/
class APRIL_API SoStateEngine : public SoEngine {

   SO_ENGINE_HEADER(SoStateEngine);

 public:
   // Fields:
	SoSFString specFile;        /// path to the spec file the engine will be read from.
	SoSFString eventIn;         /// A string, that will be interpreted as an event going into the state engine.

	SoEngineOutput currentState;    /// The name of the current state of the state engine.
	SoEngineOutput availableTransitions;    /// Names of the available transitions that lead from the current state to another.
	SoEngineOutput exitStates;    
	
   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void initClass();

   // Constructor
   SoStateEngine();

 private:
	 StateEngine * stateEngine;

   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoStateEngine();

   virtual void inputChanged(SoField * whichField);
   virtual void evaluate();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _STATEENGINE_H_
