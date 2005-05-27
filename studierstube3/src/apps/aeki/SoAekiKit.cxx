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
/** c++ source for SoAekiKit
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: SoAekiKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <iostream>
#else
#include <iostream.h>
#endif

#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/util/ivio.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/resource/SoTrackedArtifactKit.h>
#include <stbapi/window3d/SoWindowKit.h>
#include "SoAekiKit.h"
#include "SoAekiFurnitureKit.h"

SO_KIT_SOURCE(SoAekiKit);

//----------------------------------------------------------------------------
// Static class initialization
//----------------------------------------------------------------------------

void SoAekiKit::initClass(void) {
	SO_KIT_INIT_CLASS(SoAekiKit, SoContextKit, "SoContextKit");
    SoAekiFurnitureKit::initClass();
}

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

SoAekiKit::SoAekiKit() {
	SO_KIT_CONSTRUCTOR(SoAekiKit);
	SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// Desctructor
//----------------------------------------------------------------------------

SoAekiKit::~SoAekiKit() {
}

//----------------------------------------------------------------------------
// Create window geometry and attach scene graph to it
//----------------------------------------------------------------------------

SbBool SoAekiKit::checkWindowGeometry() {
    SoWindowKit * window = NULL;
    if( getNumWindows() == 0 )
    {
        window = new SoWindowKit;
        window->position.setValue(0,0,0);
        window->state.setValue( SoWindowKit::MAXIMIZED );
    }
    else
    {
        window = getWindow(0);
    }
    
    window->ref();

    SoSearchAction sa;
    sa.setType( SoInfo::getClassTypeId());
    sa.setSearchingAll( TRUE );
    sa.setInterest( SoSearchAction::FIRST );
    sa.apply( window );

    SoFullPath * path = (SoFullPath *) sa.getPath();
    if( path != NULL )
    {
        SoInfo * info = (SoInfo *) path->getTail();        
	    SoAekiFurnitureKit* furniture = new SoAekiFurnitureKit;
	    furniture->filename = info->string;
	    furniture->setName("top_furniture");	
	    SoSeparator* clientVolume = window->getClientVolume();
	    clientVolume->addChild( furniture );
	    furniture->modelStation.setValue(10);
    }
    else
    {
        printf("ERROR: Aeki: no Info node with information about model file found !\n");
    }

    window->unrefNoDelete();	
	return TRUE;
}

//----------------------------------------------------------------------------
// Sets/Removes button callbacks
//----------------------------------------------------------------------------

void SoAekiKit::checkPipMasterMode(SoNode* pipSheet, SbBool masterMode) {
	So3DButton* bRev = (So3DButton*)findNode(pipSheet, "REV_BUTTON");
	So3DButton* bPrev = (So3DButton*)findNode(pipSheet, "PREV_BUTTON");
	So3DButton* bNext = (So3DButton*)findNode(pipSheet, "NEXT_BUTTON");
	So3DButton* bFwd = (So3DButton*)findNode(pipSheet, "FWD_BUTTON");
	SoEventCallback * event = (SoEventCallback*)findNode(pipSheet, "EVENT" );
	if (masterMode) {
		bRev->addReleaseCallback(resetButtonCB, this);
		bPrev->addReleaseCallback(stepButtonCB, this);
		bNext->addReleaseCallback(stepButtonCB, this);
		bFwd->addReleaseCallback(resetButtonCB, this);
		event->addEventCallback(SoKeyboardEvent::getClassTypeId(),eventCB, this);
	} else {
		bRev->removeReleaseCallback(resetButtonCB);
		bPrev->removeReleaseCallback(stepButtonCB);
        bNext->removeReleaseCallback(stepButtonCB);
		bFwd->removeReleaseCallback(resetButtonCB);
		event->removeEventCallback(SoKeyboardEvent::getClassTypeId(),eventCB, this);
	}
}

//----------------------------------------------------------------------------
// Callback function called when prev or next was pressed
//----------------------------------------------------------------------------

void SoAekiKit::stepButtonCB(void* data, So3DButton* button) {
	SoAekiKit* self = reinterpret_cast<SoAekiKit*> (data);
	SoAekiFurnitureKit* furniture = reinterpret_cast<SoAekiFurnitureKit*> (self->getWindow(0)->getClientVolume()->getByName("top_furniture"));

	if (button == (So3DButton*)findNode(self->getPipSheet(self->getUserId()), "PREV_BUTTON")) {   // Previous
		if (furniture->position.getValue() > 0) {
			furniture->position.setValue(furniture->position.getValue() - 1);
		}
	}

	if (button == (So3DButton*)findNode(self->getPipSheet(self->getUserId()), "NEXT_BUTTON")) {   // Next
		if (furniture->position.getValue() < furniture->getMaxPosition()) {
			furniture->position.setValue(furniture->position.getValue() + 1);
		}
	}
}

//----------------------------------------------------------------------------
// Callback function called when rev or fwd was pressed
//----------------------------------------------------------------------------

void SoAekiKit::resetButtonCB(void* data, So3DButton* button) {
	SoAekiKit* self = reinterpret_cast<SoAekiKit*> (data);
	SoAekiFurnitureKit* furniture = reinterpret_cast<SoAekiFurnitureKit*> (self->getWindow(0)->getClientVolume()->getByName("top_furniture"));

	if (button == (So3DButton*)findNode(self->getPipSheet(self->getUserId()), "REV_BUTTON")) {   // Reverse
		if (furniture->position.getValue() > 0) {
			furniture->position.setValue(0);
		}
	}

	if (button == (So3DButton*)findNode(self->getPipSheet(self->getUserId()), "FWD_BUTTON")) {   // Forward
		if (furniture->position.getValue() < furniture->getMaxPosition()) {
			furniture->position.setValue(furniture->getMaxPosition());
		}
	}
}

// handles keyboard events to control application using the keyboard

void SoAekiKit::eventCB(void * data, SoEventCallback * node ) {
	SoAekiKit* self = reinterpret_cast<SoAekiKit*> (data);
	// std::cout << "Got event " << ((SoKeyboardEvent *)node->getEvent())->getKey() << std::endl;
	if( SoKeyboardEvent::isKeyReleaseEvent(node->getEvent(),SoKeyboardEvent::K))
	{
		SoAekiKit::stepButtonCB( data, (So3DButton*)findNode(self->getPipSheet(self->getUserId()), "NEXT_BUTTON"));
		node->setHandled();
	}
	else if( SoKeyboardEvent::isKeyReleaseEvent(node->getEvent(),SoKeyboardEvent::J))
	{
		SoAekiKit::stepButtonCB( data, (So3DButton*)findNode(self->getPipSheet(self->getUserId()), "PREV_BUTTON"));
		node->setHandled();
	}
	else if( SoKeyboardEvent::isKeyReleaseEvent(node->getEvent(),SoKeyboardEvent::L))
	{
		SoAekiKit::resetButtonCB( data, (So3DButton*)findNode(self->getPipSheet(self->getUserId()), "FWD_BUTTON"));
		node->setHandled();
	}
	else if( SoKeyboardEvent::isKeyReleaseEvent(node->getEvent(),SoKeyboardEvent::H))
	{
		SoAekiKit::resetButtonCB( data, (So3DButton*)findNode(self->getPipSheet(self->getUserId()), "REV_BUTTON"));
		node->setHandled();
	}
}

/* ===========================================================================
    End of SoAekiKit.cxx
=========================================================================== */
