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
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodekits/SoSubKit.h>

#include <stbapi/event3d/SoHandle3DEventAction.h>

// this macro register the behaviorKit's static callHandle3DEvent method in the
// SoHandle3DEventAction class

#define SO_BEHAVIOR_INIT_CLASS(nodeClass, parentClass, parentName) \
	SO_KIT_INIT_CLASS(nodeClass, parentClass, parentName); \
	SoHandle3DEventAction::addMethod(nodeClass::getClassTypeId(), nodeClass::handle3DEventS)

#define SO_BEHAVIOR_HEADER(nodeClass) \
	SO_KIT_HEADER(nodeClass); \
	private: static void handle3DEventS(SoAction* action, SoNode* node)

#define SO_BEHAVIOR_SOURCE(nodeClass) \
	SO_KIT_SOURCE(nodeClass); \
	void nodeClass::handle3DEventS(SoAction *action, SoNode* node) \
	{ \
		SoHandle3DEventAction *h3a = (SoHandle3DEventAction *) action; \
		nodeClass *behavior = (nodeClass *) node;\
        \
        behavior->getChildren()->traverse(h3a);\
        \
        if (!h3a->isHandled())\
        {\
		    behavior->examineEvent( (SoHandle3DEventAction *) h3a );\
															    \
		    if (behavior->isInterested())\
		    {\
			    behavior->handle3DEvent(h3a);\
/*			    h3a->setHandled();*/\
		    }\
        }\
	}

#define SO_BEHAVIOR_CONSTRUCTOR(nodeClass) \
	SO_KIT_CONSTRUCTOR(nodeClass)

#define	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(entry) \
	SO_KIT_CATALOG_ENTRY_HEADER(entry)

#define	SO_BEHAVIOR_ADD_CATALOG_ENTRY(a, b, c, d, e, f) \
	SO_KIT_ADD_CATALOG_ENTRY(a, b, c, d, e, f)

#define SO_BEHAVIOR_INIT_INSTANCE() \
	SO_KIT_INIT_INSTANCE()

#define SO_BEHAVIOR_ADD_FIELD(a,b) \
	SO_KIT_ADD_FIELD(a,b)

