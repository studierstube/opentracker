/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoStringArray.cxx
 *
 * @author  EMC
 *
 * $Id: SoStringArray.cxx,v 1.9 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoMaterial.h>

#include "SoStringArray.h"
#include "SoMenuManager.h"

SO_KIT_SOURCE(SoStringArray);

void SoStringArray::initClass(void) 
{
	if (SoType::fromName("SoStringArray").isBad())
	{
		SO_KIT_INIT_CLASS(SoStringArray, SoArrayWidget, "SoArrayWidget");
	}
}

SoStringArray::SoStringArray(void)
{
	setTemplateGeometryStrings("items/radiobutton_checkbox.iv","items/radiobutton_checkbox_highlight.iv",
		"items/radiobutton_checkbox_selected.iv","items/radiobutton_checkbox_selected_highlight.iv",
		"items/radiobutton_arrowup.iv","items/radiobutton_arrowdown.iv");

	currentItem = -1;
	transform = NULL;
	geometrySep = NULL;
	accumRotation = 0.0f;

	SO_KIT_CONSTRUCTOR(SoStringArray);

	SO_KIT_ADD_FIELD(items, ("Test"));
	SO_KIT_ADD_FIELD(selected, (""));
	SO_KIT_ADD_FIELD(multipleSelection, (TRUE));

	SO_KIT_INIT_INSTANCE();

	// set up field sensors
	itemsSensor.setData(this);
	itemsSensor.setFunction(SoStringArray::itemsChanged);
	itemsSensor.attach(&items);

	setUpConnections(TRUE, TRUE);
}

SoStringArray::~SoStringArray(void)
{
}

SbBool SoStringArray::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;

	if (onOff)
	{
		// We connect AFTER base class.
		SoArrayWidget::setUpConnections(onOff, doItAlways);
	}
	else
	{
		// We disconnect BEFORE base class.
		SoArrayWidget::setUpConnections(onOff, doItAlways);
	}
	return !(connectionsSetUp = onOff);
}

void SoStringArray::setRingRotation()
{
	transform->setMatrix(SbMatrix::identity());
}


void SoStringArray::createGeometry()
{
	destroyGeometry();

	transform = new SoTransform();
	transform->ref();
	transform->scaleFactor.setValue(0.006f, 0.006f, 0.006f);
	getGeometryNode()->addChild(transform);
	
	geometrySep = new SoSeparator();
	geometrySep->ref();

	// Set font node
	createFontNode(geometrySep);

	bool useLabels = false;		// SoStringArray has no labels, items are the text that is displayed

	float lineheight = 7.0f * itemSize.getValue() * (fontSize.getValue()/20.0f);

	// Display all visible items. If current item is out of visible range, adjust the visible range
	if (currentItem<firstVisibleLine) { firstVisibleLine = currentItem; }
	if (currentItem>=firstVisibleLine+displayLines.getValue()) { firstVisibleLine = currentItem-displayLines.getValue()+1; }
	
	if (firstVisibleLine>items.getNum()-displayLines.getValue()) { firstVisibleLine = items.getNum()-displayLines.getValue(); }
	if (firstVisibleLine<0) { firstVisibleLine = 0; }
	
	for (int line = 0; line < displayLines.getValue(); line++)
	{
		int i = line + firstVisibleLine;
		if (i >= items.getNum()) { continue; }

		SoSeparator* itemSep = new SoSeparator();
        SoTransform* itemTrans = new SoTransform();
		itemTrans->translation.setValue(0, (float)-line * lineheight, 0);

		SoText2* itemText = new SoText2();
		itemText->string = items[i];
		itemText->justification = 1;

		itemSep->addChild(itemTrans);

		if (i == currentItem)
		{
            SoMaterial* itemColor = new SoMaterial();
			itemColor->diffuseColor.setValue(0, 1, 0);
			itemSep->addChild(itemColor);
		}

		itemSep->addChild(itemText);

		SoTransform* buttonTransform = new SoTransform();
		buttonTransform->ref();
		buttonTransform->scaleFactor.setValue(SbVec3f(0.1f,0.1f,0.1f) * lineheight);
		buttonTransform->rotation.setValue(SbVec3f(0,0,1),1.5708f);
		buttonTransform->translation.setValue(-4.0f,1.0f,0.0f);
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

	// Add arrows to indicate possible list scrolling
	if (firstVisibleLine > 0)
	{
		createGeometryArrowUp(lineheight,geometrySep);
	}
	if (firstVisibleLine < items.getNum()-displayLines.getValue())
	{
		createGeometryArrowDown(lineheight,geometrySep);
	}

	getGeometryNode()->addChild(geometrySep);
}

void SoStringArray::destroyGeometry()
{
	deleteNode((SoNode**)(&transform));
	deleteNode((SoNode**)(&geometrySep));
	getGeometryNode()->removeAllChildren();
}


void SoStringArray::setSelected()
{
	int count = 0;
	for(int i=0;i<option.getNum();i++)
	{
		if (option[i]) { count++; }
	}
	selected.setNum(count);

	count = -1;
	for(int i=0;i<option.getNum();i++)
	{
		if (option[i])
		{
			count++;
			SbString str = items[i].getString();

			selected.set1Value(count, str.getString());
		}
	}
}


void SoStringArray::onClickSelect()
{
	// User clicks to select an item in the list
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
	setSelected();
}

void SoStringArray::onClickConfirm()
{
	// User clicks to confirm selection
	setSelected();
	setStartWidget();
}

void SoStringArray::onPrimaryRotationChanged(float angleIncrement)
{
	// React on user input

	accumRotation -= (float)(angleIncrement * 100);

	createGeometry();

	if ((accumRotation > 10) ||
		(accumRotation < -10))
	{
		if (accumRotation < 0) currentItem--;
		else if (accumRotation > 0) currentItem++;

		if (currentItem < 0) currentItem = 0;
		else if (currentItem >= items.getNum()) currentItem = items.getNum() - 1;

		accumRotation = 0.0f;
	}
}

void SoStringArray::onSecondaryRotationChanged(float angleIncrement)
{
	// React on user input
}


void SoStringArray::itemsChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoStringArray::getClassTypeId()));

	SoStringArray *self = (SoStringArray*)data;

	if ((self->currentItem == -1) && (self->items.getNum() > 0)) { self->currentItem = 0; }
	self->option.setNum(self->items.getNum());
	self->createGeometry();
}
