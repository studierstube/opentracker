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
/** Definition of SoAppNode node
  *
  * @author  Rainer Splechtna
  *
  * $Id: SoAppNode.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOAPPNODE_H_
#define _SOAPPNODE_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**  This node loads an application into the workspace

    SYNTAX:

    SoAppNode {
        appName         ""
        workingDir      ""
        isContext       TRUE
    }
*/
class STBAPI_API SoAppNode : public SoGroup
{
   SO_NODE_HEADER(SoAppNode);
 public:
   // Fields:
   /** name of the application */
   SoSFString     appName;
   /** working directory of the application */
   SoSFString     workingDir;
   /** specifys, whether the application is derived from SoContextKit or a legacy application */
   SoSFBool       isContext;
   /** Initializes this class for use in scene graphs. This
       should be called after database initialization and before
       any instance of this node is constructed.
	   */
   static void    initClass();

   /** Constructor */
   SoAppNode();

    /** Ignore GLRender actions (needed when precaching the scene graph
        where the first GLRender action would strat the application). */
    static void setIgnoreGLRenderAction(SbBool truefalse);
   /** ID of the application (set after it was loaded) */
   int appID;
   
 protected:

   /** The only action that is overridden is the GL render
      action. We will inherit all other action methods from
      SoGroup.
	  */
   virtual void   GLRender(SoGLRenderAction *action);

 private:

   /** Destructor. Private to keep people from trying to delete
      nodes, rather than using the reference count mechanism.
      Makes newer GCC complaining about destructor not being
      avaliable as public function.
	  */
   virtual ~SoAppNode();
   /** state of application */
   int      appRunning;
   static SbBool ignoreGLRenderAction;
};



#ifdef WIN32
#include <SoWinEnterScope.h>
#endif
#endif // _SOAPPNODE_H_

