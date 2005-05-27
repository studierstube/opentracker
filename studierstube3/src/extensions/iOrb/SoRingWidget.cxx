/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoRingWidget.cxx
 *
 * @author  EMC
 *
 * $Id: SoRingWidget.cxx,v 1.10 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#include "SoRingWidget.h"

SO_KIT_SOURCE(SoRingWidget);

void SoRingWidget::initClass(void) 
{
	if (SoType::fromName("SoRingWidget").isBad())
	{
		SO_KIT_INIT_CLASS(SoRingWidget, SoMenuWidget, "SoMenuWidget");
	}
}

SoRingWidget::SoRingWidget(void)
{
	SO_KIT_CONSTRUCTOR(SoRingWidget);

	SO_KIT_ADD_FIELD(radius, (1));
	SO_KIT_ADD_FIELD(itemSize, (1));

	SO_KIT_INIT_INSTANCE();

	setUpConnections(TRUE, TRUE);
}

SoRingWidget::~SoRingWidget(void)
{
}

SbBool SoRingWidget::setUpConnections(SbBool onOff, SbBool doItAlways)
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

