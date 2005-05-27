/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoFaderWidget.cxx
 *
 * @author  EMC
 *
 * $Id: SoFaderWidget.cxx,v 1.1 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#include "globals.h"

#include <Inventor/nodes/SoSeparator.h>

#include "SoFaderWidget.h"

SO_KIT_SOURCE(SoFaderWidget);

void SoFaderWidget::initClass(void) 
{
	if (SoType::fromName("SoFaderWidget").isBad())
	{
		SO_KIT_INIT_CLASS(SoFaderWidget, SoMenuWidget, "SoMenuWidget");
	}
}

SoFaderWidget::SoFaderWidget(void):  
	transform(NULL),
	textLabel(NULL),
	textValue(NULL)
{
	setTemplateGeometryStrings("items/faderwidget_base.iv","items/faderwidget_base_highlight.iv",
		"items/faderwidget.iv","items/faderwidget_highlight.iv","","");
	
	SO_KIT_CONSTRUCTOR(SoFaderWidget);

	SO_KIT_ADD_FIELD(isMoving, (FALSE));
	SO_KIT_ADD_FIELD(minValue, (0));
	SO_KIT_ADD_FIELD(maxValue, (10));
	SO_KIT_ADD_FIELD(currentValue, (0));
	SO_KIT_ADD_FIELD(scaleRotation, (1.0f));
	SO_KIT_ADD_FIELD(minAngle, (-2.25f));
	SO_KIT_ADD_FIELD(maxAngle, (2.25f));
	SO_KIT_ADD_FIELD(currentAngle, (0.0f));
	SO_KIT_ADD_FIELD(label, ("Value"));

	SO_KIT_INIT_INSTANCE();

	setUpConnections(TRUE, TRUE);

	onPrimaryRotationChanged(0);
}

SoFaderWidget::~SoFaderWidget(void)
{
	destroyGeometry();
}

void SoFaderWidget::createGeometry()
{
	destroyGeometry();

	// Set font node
	createFontNode(getGeometryNode());

	SoTransform* baseTransform = new SoTransform();
	baseTransform->rotation.setValue(SbVec3f(1, 0, 0), (float)(PI / 2));
	getGeometryNode()->addChild(baseTransform);

	// The text

	SoSeparator* textSep = new SoSeparator();

	SoTransform* textTransform1 = new SoTransform();
	textTransform1->translation.setValue(0.0f,0.0f,0.01f);
	textSep->addChild(textTransform1);

	textValue = new SoText2();
	textValue->ref();
	textValue->justification=3;
	textValue->string.setValue(SbString(currentValue.getValue()));
	textSep->addChild(textValue);

	SoTransform* textTransform2 = new SoTransform();
	textTransform2->translation.setValue(0.0f,0.0f,0.11f);
	textSep->addChild(textTransform2);

	textLabel = new SoText2();
	textLabel->ref();
	textLabel->string.setValue(label.getValue());
	textLabel->justification=3;
	textSep->addChild(textLabel);

	getGeometryNode()->addChild(textSep);

	// Widget Base

	SoSeparator* geomSep = new SoSeparator();
	getGeometryNode()->addChild(geomSep);

	SoTransform* geomTransform = new SoTransform();
	geomTransform->scaleFactor.setValue(0.02f,0.02f,0.02f);
	geomTransform->rotation.setValue(SbVec3f(1,0,0),(float)(PI/2));
	geomSep->addChild(geomTransform);

	SoTransform* geomTransform2 = new SoTransform();
	geomTransform2->rotation.setValue(SbVec3f(0,0,1),(float)(-PI/2));
	geomSep->addChild(geomTransform2);

	SoNode* widgetbase = this->getTemplateGeometry();
	if (isMoving.getValue() == TRUE) { widgetbase = this->getTemplateGeometryHighLight(); }
	geomSep->addChild(widgetbase);

	// Widget Top

	transform = new SoTransform();
	transform->rotation.setValue(SbVec3f(0,0,1),(currentAngle.getValue()));
	geomSep->addChild(transform);

	SoNode* widget = this->getTemplateGeometrySelected();
	if (isMoving.getValue() == TRUE) { widget = this->getTemplateGeometrySelectedHighLight(); }
	geomSep->addChild(widget);
}

void SoFaderWidget::destroyGeometry()
{
	// decrease reference count

	UNREF(transform);
	UNREF(textLabel);
	UNREF(textValue);
	getGeometryNode()->removeAllChildren();
}

SbBool SoFaderWidget::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;

	if (onOff)
	{
		// We connect AFTER base class.
		SoMenuWidget::setUpConnections(onOff, doItAlways);

		createGeometry();
	}
	else
	{
		destroyGeometry();

		// We disconnect BEFORE base class.
		SoMenuWidget::setUpConnections(onOff, doItAlways);
	}
	return !(connectionsSetUp = onOff);
}


void SoFaderWidget::onClickSelect()
{
	if (isMoving.getValue() == FALSE)
		isMoving.setValue(TRUE);
	else 
		isMoving.setValue(FALSE);
	createGeometry();
}

void SoFaderWidget::onClickConfirm()
{
	// User clicks to confirm selection
	setStartWidget();
}

void SoFaderWidget::onPrimaryRotationChanged(float angleIncrement)
{
	if (isMoving.getValue() == TRUE)
	{
		currentAngle = currentAngle.getValue() + angleIncrement * scaleRotation.getValue();
    
		if (currentAngle.getValue() > maxAngle.getValue())
			currentAngle = maxAngle.getValue();
		if (currentAngle.getValue() < minAngle.getValue())
			currentAngle = minAngle.getValue();

		transform->rotation.setValue(SbVec3f(0,0,1),(currentAngle.getValue()));
	}

	double relativePosition = (currentAngle.getValue()-minAngle.getValue())/(maxAngle.getValue()-minAngle.getValue());
	int diff1 = (maxValue.getValue()-minValue.getValue());
	int newValue = (int)(minValue.getValue()+(diff1*relativePosition));
	currentValue.setValue(newValue);
		
	textLabel->string.setValue(label.getValue());
	textValue->string.setValue(SbString(currentValue.getValue()));
}


void SoFaderWidget::onSecondaryRotationChanged(float angleIncrement)
{
	// React on user input
}
