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
/**
 *
 * @author Thomas Psik
 *
 * $Id: SoAddUserWidgetConnectionToFanIn.h 3964 2004-10-19 13:43:28Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SoAddUserWidgetConnectionToFanIn_H_
#define _SoAddUserWidgetConnectionToFanIn_H_

#include <Inventor/nodes/SoSubNode.h>

#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFString.h>

#include <Inventor/sensors/SoFieldSensor.h>


#include "construct3d.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoEngine;
class SoNode;
class SoNodeEngine;

/**
@ingroup construct3d

This node allows to create a very specific connection between widgets and a SoFanIn node.

File Format / defaults:

@verbatim

SoAddUserWidgetConnectionToFanIn {
	SoSFNode widget  # node must be derived from SoBehaviorKit
	SoSFEngine fanIn # Engine must be derived from SoFanIn
  }
  
@endverbatim

Example

  lineCommand = DEF FAN_IN SoFanIn { }.out
  DEF LINE_COMMAND SoPushButton {}	

  SoAddUserWidgetConnectionToFanIn { 
  widget USE LINE_COMMAND 
  fanIn USE FAN_IN 
  }

*/
class SoPushButton;
class SoFanIn;

class CONSTRUCT3D_API SoAddUserWidgetConnectionToFanIn : public SoNode {

   SO_NODE_HEADER(SoAddUserWidgetConnectionToFanIn);

 public:
   // Fields:
   	
   /**  */
   SoSFString widget;				    

   /**  */
   SoSFString fanIn;				 
  	

   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   SoAddUserWidgetConnectionToFanIn();

   void setup(int uid, SoNode *base );

 protected:
	/** helper */
	SoSFTrigger trigger;
    SoSFString  value;
	SoField *nextIn;
	SoFieldSensor triggerSensor;
	static void triggerCB(void *data, SoSensor *sensor);

	void setupPushButton(SoPushButton *pushButPtr, SoFanIn *fanInPtr, int uid); 

 private:
	
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SoAddUserWidgetConnectionToFanIn_H_
