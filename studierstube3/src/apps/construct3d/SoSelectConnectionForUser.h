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
 * $Id: SoSelectConnectionForUser.h 3964 2004-10-19 13:43:28Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SoSelectConnectionForUser_H_
#define _SoSelectConnectionForUser_H_

#include <Inventor/nodes/SoSubNode.h>

#include <Inventor/fields/SoMFString.h>
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

SoSelectConnectionForUser {
	SoSFNode widget  # node must be derived from SoBehaviorKit
	SoSFEngine fanIn # Engine must be derived from SoFanIn
  }
  
@endverbatim

Example

  lineCommand = DEF FAN_IN SoFanIn { }.out
  DEF LINE_COMMAND SoPushButton {}	

  SoSelectConnectionForUser { 
  widget USE LINE_COMMAND 
  fanIn USE FAN_IN 
  }

*/
/* class SoPushButton; class SoFanIn;*/
class SoLabelListBox ;
class SoPucStateEnumerated;
class SoPucCommand;
class SoPushButton;
class SoToggleButton;
class SoPucStateBool;
class SoIncrementalSlider;
class SoPucStateFixed;

class CONSTRUCT3D_API SoSelectConnectionForUser : public SoNode {

   SO_NODE_HEADER(SoSelectConnectionForUser);

 public:
   // Fields:
   	
   /**  */
   SoMFString inputs;				    

   /**  */
   SoSFString widget;				 
  	

   // Initializes this class for use in scene graphs. This
   // should be called after database initialization and before
   // any instance of this node is constructed.
   static void    initClass();

   // Constructor
   SoSelectConnectionForUser();

   void setup(int uid, SoNode *base );

 protected:
	/** helper */
	void setupEnumToListBox(SoLabelListBox *listBox, SoPucStateEnumerated *stateEnum); 
	void setupCommandToPushButton(SoPushButton *button,SoPucCommand* command);
	void setupBoolToToggleButton(SoToggleButton *button, SoPucStateBool *pucBool);
	void setupFixedToIncSlider(SoIncrementalSlider *slider, SoPucStateFixed *pucFixed);
 private:
	
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SoSelectConnectionForUser_H_
