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
// ===========================================================================
//  NAME:       GLTweak
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Definition of GLTweak node
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    jp      Jan Prikryl
// ===========================================================================
//  HISTORY:
//
//  05-Sep-00 17:46:22  jp      last modification
//  04-Sep-00 10:59:15  jp      created
// ===========================================================================
//
//  PURPOSE:
//
//  This node makes it possible to set different OpenGL state parameters
//  while traversing the Inventor scene graph.
//
//  SYNTAX:
//
//  GLTweak {
//    depthTest  TRUE/FALSE
//    writeColor TRUE/FALSE
//    writeAlpha TRUE/FALSE
//    writeDepth TRUE/FALSE
//  }
//
//  REMARKS:
//
//  Fields not mentioned in the node definition will be ignored when setting
//  OpenGL parameters. This makes it possible to change all paramaters
//  independently.

#ifndef _SOGLTWEAK_H_
#define _SOGLTWEAK_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoSubNode.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * This node should not be used anymore. Its functionality is superseeded by
 * the following nodes found either in Coin or the Studierstube :
 * @li SoPolygonOffset 
 * @li DepthBuffer (in SmallChange)
 * @li @ref SoFrameBuffer
 * @li @ref SoStencilBuffer
 *
 * @ingroup camera
 * @ingroup deprecated
 */
class STBAPI_API GLTweak : public SoNode {

   SO_NODE_HEADER(GLTweak);

 public:
   // Fields:
   SoSFBool       depthTest;
   SoSFBool       writeColor;
   SoSFBool       writeAlpha;
   SoSFBool       writeDepth;
   SoSFFloat      offsetFactor;
   SoSFFloat      offsetUnits;
   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   GLTweak();

 protected:
   // These implement supported actions. The only actions that
   // deal with materials are the callback and GL render
   // actions. We will inherit all other action methods from
   // SoNode.
   virtual void   GLRender(SoGLRenderAction *action);
   virtual void   callback(SoCallbackAction *action);

   // This implements generic traversal of GLTweak node, used in
   // both of the above methods.
   virtual void   doAction(SoAction *action);

 private:
   // Destructor. Private to keep people from trying to delete
   // nodes, rather than using the reference count mechanism.
   // Makes newer GCC complaining about destructor not being
   // avaliable as public function.
   virtual ~GLTweak();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOGLTWEAK_H_
