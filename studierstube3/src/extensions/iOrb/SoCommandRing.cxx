/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoCommandRing.cxx
 *
 * @author  EMC
 *
 * $Id: SoCommandRing.cxx,v 1.24 2005/01/26 23:16:52 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoMaterial.h>

#include "SoCommandRing.h"
#include "SoMenuManager.h"

SO_KIT_SOURCE(SoCommandRing);

void SoCommandRing::initClass(void) 
{
	if (SoType::fromName("SoCommandRing").isBad())
	{
		SO_KIT_INIT_CLASS(SoCommandRing, SoRingWidget, "SoRingWidget");
	}
}

SoCommandRing::SoCommandRing(void)
{
	setTemplateGeometryStrings("items/radiobutton.iv","items/radiobutton_highlight.iv","","","","");
	currentItem = -1;
	transform = NULL;
	geometrySep = NULL;
	currentAngle = 0.0f;

	SO_KIT_CONSTRUCTOR(SoCommandRing);

	SO_KIT_ADD_FIELD(items, ("Test"));
	SO_KIT_ADD_FIELD(labels, ("Test"));
	SO_KIT_ADD_FIELD(command, (""));

	SO_KIT_INIT_INSTANCE();

	// set up field sensors
	itemsSensor.setData(this);
	itemsSensor.setFunction(SoCommandRing::itemsChanged);
	itemsSensor.attach(&items);
	labelsSensor.setData(this);
	labelsSensor.setFunction(SoCommandRing::labelsChanged);
	labelsSensor.attach(&labels);

	setUpConnections(TRUE, TRUE);
}

SoCommandRing::~SoCommandRing(void)
{
}

SbBool SoCommandRing::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;

	if (onOff)
	{
		// We connect AFTER base class.
		SoRingWidget::setUpConnections(onOff, doItAlways);
	}
	else
	{
		// We disconnect BEFORE base class.
		SoRingWidget::setUpConnections(onOff, doItAlways);
	}
	return !(connectionsSetUp = onOff);
}

void SoCommandRing::setRingRotation()
{
	transform->rotation.setValue(SbVec3f(0.0f, 0.0f, 1.0f), (float)((currentAngle*2*3.141592)/360.0f));
}


void SoCommandRing::createGeometry()
{
	destroyGeometry();

	transform = new SoTransform();
	transform->ref();
	transform->scaleFactor.setValue(0.003f, 0.003f, 0.003f);
	getGeometryNode()->addChild(transform);
	
	setRingRotation();

	geometrySep = new SoSeparator();
	geometrySep->ref();

	// Set font node
	createFontNode(geometrySep);

	bool useLabels = false;
	if (labels.getNum()==items.getNum()) { useLabels = true; }

	float multiplysize = items.getNum()*9.0f*radius.getValue()*itemSize.getValue();
	SoTransform* itemTrans = new SoTransform();
	itemTrans->translation.setValue(0.0f, multiplysize, 0.0f);
	float itemAngle = 360.0f/items.getNum();
	float currentItemAngle = 0;

	for (int i=0; i<items.getNum(); i++)
	{
		SoSeparator* itemSep = new SoSeparator();
		SoTransform* itemRot = new SoTransform();
		SoTransform* itemRotBack = new SoTransform();
		double currentRotAngle = (currentItemAngle*2*3.141592)/360.0f;
		itemRot->rotation.setValue(SbVec3f(0.0f, 0.0f, 1.0f), (float)-currentRotAngle);
		itemRotBack->rotation.setValue(SbVec3f(0.0f, 0.0f, 1.0f), (float)currentRotAngle-(currentAngle*3.141592f/180.0f));
		currentItemAngle += itemAngle;
		SoText2* itemText = new SoText2();
		if (useLabels) { itemText->string = labels[i]; } else { itemText->string = items[i]; }
		itemText->justification = 3;

		if (i == currentItem)
		{
            SoMaterial* itemColor = new SoMaterial();
			itemColor->diffuseColor.setValue(0, 1, 0);
			itemSep->addChild(itemColor);
		}

		itemSep->addChild(itemRot);
		itemSep->addChild(itemTrans);
		itemSep->addChild(itemText);
		itemSep->addChild(itemRotBack);
		
		SoTransform* buttonTransform = new SoTransform();
		buttonTransform->ref();
		buttonTransform->scaleFactor.setValue(SbVec3f(5.0f,5.0f,5.0f)*itemSize.getValue());
		buttonTransform->rotation.setValue(SbVec3f(0,0,1),1.5708f);
		itemSep->addChild(buttonTransform);
		SoNode* button = NULL;
		if (currentItem == i)
		{
			button = this->getTemplateGeometryHighLight();
		} else {
			button = this->getTemplateGeometry();
		}
		if (button != NULL) { itemSep->addChild(button); }
		
		geometrySep->addChild(itemSep);
	}
	getGeometryNode()->addChild(geometrySep);
}

void SoCommandRing::destroyGeometry()
{
	deleteNode((SoNode**)(&transform));
	deleteNode((SoNode**)(&geometrySep));
	getGeometryNode()->removeAllChildren();
}


void SoCommandRing::setCommand()
{
	if (currentItem > -1)
	{
		command.setValue(items[currentItem]);
		if (parent != NULL)
		{
			parent->executeCommand(command.getValue());
		}
	}
}


void SoCommandRing::onClickSelect()
{
	// User clicks to select an item in the list
	setCommand();
}

void SoCommandRing::onClickConfirm()
{
	// User clicks to confirm selection
	setStartWidget();
}

void SoCommandRing::onPrimaryRotationChanged(float angleIncrement)
{
	// React on user input
	currentAngle -= (float)(angleIncrement*180/3.141592);
	if (currentAngle<0.0f) { currentAngle += 360.0f; }
	if (currentAngle>360.0f) { currentAngle -= 360.0f; }
	
	createGeometry();

	float currentItemAngle = (float)(360.0f/items.getNum());
	currentItem = (int)((currentAngle+currentItemAngle/2)/currentItemAngle);
	if (currentItem<0) { currentItem = items.getNum()-1; }
	if (currentItem>=items.getNum()) { currentItem = 0; }
}

void SoCommandRing::onSecondaryRotationChanged(float angleIncrement)
{
	// React on user input
}


void SoCommandRing::itemsChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoCommandRing::getClassTypeId()));

	SoCommandRing *self = (SoCommandRing*)data;

	if ((self->currentItem == -1) && (self->items.getNum() > 0)) { self->currentItem = 0; }
	self->createGeometry();
}

void SoCommandRing::labelsChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoCommandRing::getClassTypeId()));

	SoCommandRing *self = (SoCommandRing*)data;

	self->createGeometry();
}
