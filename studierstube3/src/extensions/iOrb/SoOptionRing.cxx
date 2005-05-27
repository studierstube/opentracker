/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoOptionRing.cxx
 *
 * @author  EMC
 *
 * $Id: SoOptionRing.cxx,v 1.12 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoMaterial.h>

#include "SoOptionRing.h"
#include "SoMenuManager.h"

SO_KIT_SOURCE(SoOptionRing);

void SoOptionRing::initClass(void) 
{
	if (SoType::fromName("SoOptionRing").isBad())
	{
		SO_KIT_INIT_CLASS(SoOptionRing, SoRingWidget, "SoRingWidget");
	}
}

SoOptionRing::SoOptionRing(void)
{
	setTemplateGeometryStrings("items/radiobutton_checkbox.iv","items/radiobutton_checkbox_highlight.iv",
		"items/radiobutton_checkbox_selected.iv","items/radiobutton_checkbox_selected_highlight.iv","","");
	
	currentItem = -1;
	transform = NULL;
	geometrySep = NULL;
	currentAngle = 0.0f;

	SO_KIT_CONSTRUCTOR(SoOptionRing);

	SO_KIT_ADD_FIELD(items, ("Test"));
	SO_KIT_ADD_FIELD(option, (FALSE));
	SO_KIT_ADD_FIELD(multipleSelection, (TRUE));

	SO_KIT_INIT_INSTANCE();

	// set up field sensors
	itemsSensor.setData(this);
	itemsSensor.setFunction(SoOptionRing::itemsChanged);
	itemsSensor.attach(&items);

	setUpConnections(TRUE, TRUE);
}

SoOptionRing::~SoOptionRing(void)
{
}

SbBool SoOptionRing::setUpConnections(SbBool onOff, SbBool doItAlways)
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

void SoOptionRing::setRingRotation()
{
	transform->rotation.setValue(SbVec3f(0.0f, 0.0f, 1.0f), (float)((currentAngle*2*3.141592)/360.0f));
}


void SoOptionRing::createGeometry()
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
		itemText->string = items[i];
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
			if (!option[i]) { button = getTemplateGeometryHighLight(); }
				else { button = getTemplateGeometrySelectedHighLight(); }
		} else {
			if (!option[i]) { button = getTemplateGeometry(); }
				else { button = getTemplateGeometrySelected(); }
		}
		if (button != NULL) { itemSep->addChild(button); }
		
		geometrySep->addChild(itemSep);
	}
	getGeometryNode()->addChild(geometrySep);
}

void SoOptionRing::destroyGeometry()
{
	deleteNode((SoNode**)(&transform));
	deleteNode((SoNode**)(&geometrySep));
	getGeometryNode()->removeAllChildren();
}


void SoOptionRing::onClickSelect()
{
	// User clicks to select/deselect an item in the list
	if (currentItem > -1)
	{
		// If multiple selection is deactivated, disable all options before selecting one
		if (multipleSelection.getValue() == false)
		{
			for (int i=0;i<option.getNum();i++)
			{
				option.set1(i,"FALSE");
			}
		}

		if (option[currentItem]) { option.set1(currentItem,"FALSE"); } else { option.set1(currentItem,"TRUE"); }
		createGeometry();
	}
}

void SoOptionRing::onClickConfirm()
{
	// User clicks to confirm selection
	setStartWidget();
}

void SoOptionRing::onPrimaryRotationChanged(float angleIncrement)
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

void SoOptionRing::onSecondaryRotationChanged(float angleIncrement)
{
	// React on user input
}


void SoOptionRing::itemsChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoOptionRing::getClassTypeId()));

	SoOptionRing *self = (SoOptionRing*)data;

	if ((self->currentItem == -1) && (self->items.getNum() > 0)) { self->currentItem = 0; }
	self->option.setNum(self->items.getNum());
	self->createGeometry();
}
