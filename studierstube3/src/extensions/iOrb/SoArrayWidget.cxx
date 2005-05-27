/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoArrayWidget.cxx
 *
 * @author  EMC
 *
 * $Id: SoArrayWidget.cxx,v 1.10 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoTransform.h>

#include "SoArrayWidget.h"
#include "SoMenuManager.h"

SO_KIT_SOURCE(SoArrayWidget);

void SoArrayWidget::initClass(void) 
{
	if (SoType::fromName("SoArrayWidget").isBad())
	{
		SO_KIT_INIT_CLASS(SoArrayWidget, SoMenuWidget, "SoMenuWidget");
	}
}

SoArrayWidget::SoArrayWidget(void)
{
	firstVisibleLine = 0;

	SO_KIT_CONSTRUCTOR(SoArrayWidget);

	SO_KIT_ADD_FIELD(itemSize, (1));
	SO_KIT_ADD_FIELD(displayLines, (7));

	SO_KIT_INIT_INSTANCE();

	setUpConnections(TRUE, TRUE);
}

SoArrayWidget::~SoArrayWidget(void)
{
}

SbBool SoArrayWidget::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;

	if (onOff)
	{
		// We connect AFTER base class.
		SoMenuWidget::setUpConnections(onOff, doItAlways);

	}
	else
	{
		// We disconnect BEFORE base class.
		SoMenuWidget::setUpConnections(onOff, doItAlways);
	}
	return !(connectionsSetUp = onOff);
}

void SoArrayWidget::createGeometryArrowUp(float lineheight, SoGroup* parent)
{
	SoSeparator* itemSep = new SoSeparator();
    SoTransform* itemTrans = new SoTransform();
	itemTrans->translation.setValue(0, lineheight, 0);

	SoTransform* buttonTransform = new SoTransform();
	buttonTransform->ref();
	buttonTransform->scaleFactor.setValue(SbVec3f(0.1f,0.1f,0.1f) * lineheight);
	buttonTransform->rotation.setValue(SbVec3f(0,0,1),1.57079f);
	buttonTransform->translation.setValue(-4.0f,1.0f,0.0f);
	SoNode* button = getTemplateGeometryUp();
	itemSep->addChild(itemTrans);
	itemSep->addChild(buttonTransform);
	itemSep->addChild(button);

	parent->addChild(itemSep);
}

void SoArrayWidget::createGeometryArrowDown(float lineheight, SoGroup* parent)
{
	SoSeparator* itemSep = new SoSeparator();
    SoTransform* itemTrans = new SoTransform();
	itemTrans->translation.setValue(0, -lineheight * displayLines.getValue(), 0);

	SoTransform* buttonTransform = new SoTransform();
	buttonTransform->ref();
	buttonTransform->scaleFactor.setValue(SbVec3f(0.1f,0.1f,0.1f) * lineheight);
	buttonTransform->rotation.setValue(SbVec3f(0,0,1),1.57079f);
	buttonTransform->translation.setValue(-4.0f,1.0f,0.0f);
	SoNode* button = getTemplateGeometryDown();
	itemSep->addChild(itemTrans);
	itemSep->addChild(buttonTransform);
	itemSep->addChild(button);

	parent->addChild(itemSep);
}
