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

#ifndef _SOLIGHTPROJECTOR_H_
#define _SOLIGHTPROJECTOR_H_

#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/stbapi.h>
#include "april.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
@ingroup april

  Node to affect rendering in a way that allows us to render Projector based illumination effects.

  The SoLightProjector node affect rendering so that it renders a scene graph defined in the worldProxy field
  as if lit by a light source at lightPosition, occluded by objects defined by the content field. Therefore,
  it is possible to illuminate a real object (that is represented by the geometry in worldProxy) with a projector 
  matching the camera model, and cast shados from virtual objects (defined in the content field) onto that object.

  For more information about this concept visit http://www.studierstube.org/virtualshowcase


*/
class APRIL_API SoLightProjector : public SoNode {

   SO_NODE_HEADER(SoLightProjector);

 public:
   // Fields:
   /// The geometry of the real object to be illuminated by a projector matching the camera model.
   SoSFNode       worldProxy;       
   /// The virtual geometry that should cast shadows onto the real object.
   SoSFNode       content;          

   /// Color of the light to use.
   SoSFColor		lightColor;     
   /// Position of the virtual light source(s).
   SoMFVec3f		lightPosition;  

	
   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   SoLightProjector();

   virtual SoChildList* getChildren() const;

   static void SoLightProjector::fieldChangedCB(void* data, SoSensor* which);

 protected:

	 SoChildList* children;

   // These implement supported actions. We will inherit
   // all other action methods from SoNode.
   virtual void   GLRender(SoGLRenderAction *action);
   virtual void   callback(SoCallbackAction *action);

   // This implements generic traversal of GLTweak node, used in
   // both of the above methods.
   virtual void   doAction(SoAction *action);

   	SoFieldSensor worldProxySensor;
	SoFieldSensor contentSensor;

 private:
   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~SoLightProjector();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOLIGHTPROJECTOR_H_
