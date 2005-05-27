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
/** c++ source for SoAekiFurnitureKit
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: SoAekiFurnitureKit.cxx 3835 2004-10-06 10:08:47Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <iostream>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoComputeBoundingBox.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLException.hpp>
#include <stbapi/tracker/SoStationKit.h>
#include <stbapi/resource/SoTrackedArtifactKit.h>
#include "FurnitureFactory.h"
#include "SoAekiFurnitureKit.h"
#include "XmlHandler.h"

SO_KIT_SOURCE(SoAekiFurnitureKit);

//----------------------------------------------------------------------------
// Static class initialization
//----------------------------------------------------------------------------

void SoAekiFurnitureKit::initClass() {
	SO_KIT_INIT_CLASS(SoAekiFurnitureKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

SoAekiFurnitureKit::SoAekiFurnitureKit() {
	SO_KIT_CONSTRUCTOR(SoAekiFurnitureKit);
	SO_NODE_ADD_FIELD(filename, (SbString()));
	SO_NODE_ADD_FIELD(position, (0));
	SO_NODE_ADD_FIELD(modelStation, (-1));
	SO_KIT_ADD_CATALOG_LIST_ENTRY(componentInstanceList, SoGroup, FALSE, this,\0x, SoSwitch, FALSE);
	SO_KIT_ADD_CATALOG_LIST_ENTRY(centerList, SoGroup, FALSE, this, componentInstanceList, SoNode, FALSE);
	SO_KIT_ADD_CATALOG_LIST_ENTRY(rotationList, SoGroup, FALSE, this, centerList, SoSwitch, FALSE);
	SO_KIT_ADD_CATALOG_LIST_ENTRY(setupList, SoGroup, FALSE, this, rotationList, SoNode, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(modelTransform, SoTransform, FALSE, this, setupList, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(modelTracker, SoTrackedArtifactKit, FALSE, this, modelTransform, FALSE );
	SO_KIT_ADD_CATALOG_LIST_ENTRY(trackerList, SoGroup, FALSE, this, modelTracker, SoStationKit, FALSE);
	SO_KIT_INIT_INSTANCE();
	createInitialFurniture();
	setUpConnections(TRUE, TRUE);

	SoTrackedArtifactKit * tracker = reinterpret_cast<SoTrackedArtifactKit*>(modelTracker.getValue());
	tracker->station.connectFrom( &modelStation );
	SoTransform * trans = reinterpret_cast<SoTransform*>(modelTransform.getValue());
	trans->translation.connectFrom( &tracker->getMover()->translation );
	trans->rotation.connectFrom( &tracker->getMover()->rotation );
	trans->scaleFactor.setValue(0.7,0.7,0.7);
}

//----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------

SoAekiFurnitureKit::~SoAekiFurnitureKit() { }

//----------------------------------------------------------------------------
// Set up parts for the default configuration
//----------------------------------------------------------------------------

void SoAekiFurnitureKit::createInitialFurniture() {

	// Texture node
	SoTexture2* texture = new SoTexture2;
	texture->filename.setValue("../apps/aeki/textures/wood_5.gif");
	texture->setName("top_texture");

	// Add texture and scaling to scene graph
	SoGroup* setupList = reinterpret_cast<SoGroup*>(this->setupList.getValue());
	if (setupList != NULL) {
		setupList->addChild(texture);
		//setupList->addChild(scaling);
	}

	// Center content
	SoGroup* componentInstanceList = reinterpret_cast<SoGroup*>(this->componentInstanceList.getValue());
	if (componentInstanceList != NULL) {
		SoComputeBoundingBox *bbox = new SoComputeBoundingBox;
		bbox->node = componentInstanceList;
		SoCalculator *calc = new SoCalculator;
		calc->A.connectFrom(&bbox->boxCenter);
		calc->expression.set1Value(0, "oA[0] = -A[0];oA[1] = -A[1];"); // oA[2] = -A[2];
		SoTranslation* center = new SoTranslation;
		center->setName("center");
		center->translation.connectFrom(&calc->oA);
		SoGroup* centerList = reinterpret_cast<SoGroup*>(this->centerList.getValue());
		if (centerList != NULL) {
			centerList->addChild(center);
		}
	}

	// Set field sensors
	this->filenameSensor = new SoFieldSensor(SoAekiFurnitureKit::filenameCB, this);
	this->positionSensor = new SoFieldSensor(SoAekiFurnitureKit::positionCB, this);

	XMLPlatformUtils::Initialize();   // Perform per-process parser-initialization
}

//----------------------------------------------------------------------------
// Connect/deconnect field sensors
//----------------------------------------------------------------------------

SbBool SoAekiFurnitureKit::setUpConnections(SbBool onOff, SbBool doItAlways) {
	if (!doItAlways && connectionsSetUp == onOff) {
		return onOff;
	}

	if (onOff) {
		if (this->filenameSensor->getAttachedField() != &this->filename) {
			this->filenameSensor->attach(&this->filename);
		}
		if (this->positionSensor->getAttachedField() != &this->position) {
			this->positionSensor->attach(&this->position);
		}
	} else {
		if (this->filenameSensor->getAttachedField() != NULL) {
			this->filenameSensor->detach();
		}
		if (this->positionSensor->getAttachedField() != NULL) {
			this->positionSensor->detach();
		}
	}

	return !(connectionsSetUp = onOff);
}

//----------------------------------------------------------------------------
// Called when the value of the filename field changes
//----------------------------------------------------------------------------

void SoAekiFurnitureKit::filenameCB(void* data, SoSensor* sensor) {
	SoAekiFurnitureKit* self = reinterpret_cast<SoAekiFurnitureKit*> (data);

	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

	try {
		XmlHandler* handler = new XmlHandler;
		parser->setContentHandler(handler);
		parser->setErrorHandler(handler);
		parser->parse(self->filename.getValue().getString());

		self->componentInstances = handler->getComponentInstances();
		self->assemblySteps = handler->getAssemblySteps();
		self->position = 0;

		delete handler;

		std::vector<SoSwitch*>* switches = FurnitureFactory::buildComponentInstanceList(*self->componentInstances, *self->assemblySteps);
		SoGroup* helper = reinterpret_cast<SoGroup*>(self->componentInstanceList.getValue());
		for (unsigned int i=0;i<switches->size();i++) {
			helper->addChild((*switches)[i]);
		}

		std::vector<SoSwitch*>* switches2 = FurnitureFactory::buildRotationList(*self->assemblySteps);
		SoGroup* helper2 = reinterpret_cast<SoGroup*>(self->rotationList.getValue());
		for (unsigned i=0;i<switches2->size();i++) {
			helper2->addChild((*switches2)[i]);
		}

		std::vector<SoStationKit*>* stationKits = FurnitureFactory::buildTrackerList(*self->componentInstances, *self->assemblySteps);
		SoGroup* helper3 = reinterpret_cast<SoGroup*>(self->trackerList.getValue());
		for (unsigned i=0;i<stationKits->size();i++) {
			helper3->addChild((*stationKits)[i]);
		}
	} catch (const XMLException &e) {
		std::cout << "Error: " << e.getMessage()  << std::endl;
	} catch (const SAXParseException &e) {
		std::cout << e.getMessage() << std::endl;
	}

	delete parser;
}

//----------------------------------------------------------------------------
// Called when the value of the filename field changes
//----------------------------------------------------------------------------

void SoAekiFurnitureKit::positionCB(void* data, SoSensor* sensor) {
	SoAekiFurnitureKit* self = reinterpret_cast<SoAekiFurnitureKit*> (data);

    //////////////////////////
	// Show/hide components //
	//////////////////////////

	SoGroup* componentInstanceList = reinterpret_cast<SoGroup*>(self->componentInstanceList.getValue());

	for (int i=0;i<componentInstanceList->getNumChildren();i++) {
		SoSwitch* child = reinterpret_cast<SoSwitch*> (componentInstanceList->getChild(i));
		if (child == NULL) continue;

		char *endptr;
		unsigned long step = strtoul(child->getName().getString()+4, &endptr, 10);
		if (*endptr != '\0') continue;

		if (step <= self->position.getValue()) {
			child->whichChild = SO_SWITCH_ALL;

			SoSeparator* temp1 = reinterpret_cast<SoSeparator*>(child->getChild(0));
			SoSwitch* temp2 = reinterpret_cast<SoSwitch*>(temp1->getChild(2));

			if (step == self->position.getValue()) {
				temp2->whichChild = SO_SWITCH_ALL;
			} else {
				temp2->whichChild = SO_SWITCH_NONE;
			}
		} else {
			child->whichChild = SO_SWITCH_NONE;
		}



	}

	/////////////////////////
	// Show/hide rotations //
	/////////////////////////

	SoGroup* rotationList = reinterpret_cast<SoGroup*>(self->rotationList.getValue());

	for (i=0;i<rotationList->getNumChildren();i++) {

		SoSwitch* child = reinterpret_cast<SoSwitch*> (rotationList->getChild(i));
		if (child == NULL) continue;

		char *endptr;
		unsigned long step = strtoul(child->getName().getString()+4, &endptr, 10);
		if (*endptr != '\0') continue;

		if (step <= self->position.getValue()) {
			child->whichChild = SO_SWITCH_ALL;
		} else {
			child->whichChild = SO_SWITCH_NONE;
		}
	}

	////////////////////////
	// Show/hide trackers //
	////////////////////////

	SoGroup* trackerList = reinterpret_cast<SoGroup*>(self->trackerList.getValue());

	for (i=0;i<trackerList->getNumChildren();i++) {

		SoStationKit* child = reinterpret_cast<SoStationKit*> (trackerList->getChild(i));
		if (child == NULL) continue;

		char *endptr;
		long step = strtol(child->getName().getString()+4, &endptr, 10);
		if (*endptr != '\0') continue;

		SoGroup* content = reinterpret_cast<SoGroup*>(child->getPart("content", FALSE));
		if (content == NULL) continue;
		SoSwitch* contentSwitch = reinterpret_cast<SoSwitch*>(content->getChild(0));
		if (contentSwitch == NULL) continue;

		if (step == self->position.getValue()) {
			contentSwitch->whichChild = SO_SWITCH_ALL;
		} else {
			contentSwitch->whichChild = SO_SWITCH_NONE;
		}
	}
}

//----------------------------------------------------------------------------
// Returns the index of the last assembly step
//----------------------------------------------------------------------------

unsigned int SoAekiFurnitureKit::getMaxPosition() {
	if (this->assemblySteps != NULL) return (this->assemblySteps->size());
	else return (0);
}

/* ===========================================================================
    End of SoAekiFurnitureKit.cxx
=========================================================================== */
