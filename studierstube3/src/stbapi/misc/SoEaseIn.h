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

#ifndef _SOEASEIN_H_
#define _SOEASEIN_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFFloat.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
@ingroup util

A helper class for smooth animations. Converts float inputs in the range from 0..1 (e.g. from a
SoOneShot engine) to float values in the range form 0..1, applying a filter that smoothens the start
and end velocity.
*/
class STBAPI_API SoEaseIn : public SoEngine {

   SO_ENGINE_HEADER(SoEaseIn);

 public:
    /// ease value for configuring. determines how big the effect will be.
    SoSFFloat ease;     

    /// value input
    SoSFFloat in;       
	/// value output
	SoEngineOutput out; 
	
   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void initClass();

   // Constructor
   SoEaseIn();

 private:

   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoEaseIn();

   virtual void evaluate();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOEASEIN_H_
