/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoMenuManager.cxx
 *
 * @author  EMC
 *
 * $Id: SoMenuManager.cxx,v 1.27 2005/01/26 23:16:52 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#include <stbapi/tracker/SoTrakEngine.h>

#include "globals.h"

#include "SoRotationTracker.h"
#include "SoMenuManager.h"
#include "SoMenuWidget.h"
#include "SoRingWidget.h"
#include "SoArrayWidget.h"

SO_NODE_SOURCE(SoMenuManager);

void SoMenuManager::initClass(void) 
{
	if (SoType::fromName("SoMenuManager").isBad())
	{
		SO_NODE_INIT_CLASS(SoMenuManager, SoSwitch, "Switch");
	}
}

SoMenuManager::SoMenuManager(void): 
	current(NULL), rotationTracker(NULL), trakEngine(NULL), lastButtonClick(false)
{
	SO_NODE_CONSTRUCTOR(SoMenuManager);

	SO_NODE_DEFINE_ENUM_VALUE(MenuMode, MENU_TIMEOUT);
	SO_NODE_DEFINE_ENUM_VALUE(MenuMode, MENU_SECONDARY_ROTATION);
	SO_NODE_SET_SF_ENUM_TYPE(mode, MenuMode);

	SO_NODE_DEFINE_ENUM_VALUE(ConfirmMode, EXIT_TO_START);
	SO_NODE_DEFINE_ENUM_VALUE(ConfirmMode, EXIT_ONE_LEVEL);
	SO_NODE_SET_SF_ENUM_TYPE(menuConfirmMode, ConfirmMode);

	SO_NODE_ADD_FIELD(startWidget,	(FALSE));
	SO_NODE_ADD_FIELD(mode,			(MENU_TIMEOUT));
	SO_NODE_ADD_FIELD(active,		(FALSE));
	SO_NODE_ADD_FIELD(currentWidget,(FALSE));
	SO_NODE_ADD_FIELD(history,		(FALSE));
	SO_NODE_ADD_FIELD(station,		(0));
	SO_NODE_ADD_FIELD(buttonPressTimeOut,	(1000));
	SO_NODE_ADD_FIELD(menuConfirmAngle,		(DEG2RAD(75)));
	SO_NODE_ADD_FIELD(menuConfirmMode, (EXIT_TO_START));

	// set up field sensors
	SET_SENSOR(currentWidgetSensor, currentWidget, SoMenuManager::currentWidgetChanged);
	SET_SENSOR(startWidgetSensor, startWidget, SoMenuManager::startWidgetChanged);
	SET_SENSOR(activeSensor, active, SoMenuManager::activeChanged);

	// Create the SoRotationTracker and TrakEngine Node
	createTrakEngine();
	createRotationTracker();

	currentWidget.setValue(startWidget.getValue().getString());
	whichChild = SO_SWITCH_NONE;
}

SoMenuManager::~SoMenuManager(void)
{
	// decrease reference count
	if (trakEngine != NULL)
	{
		trakEngine->unref();
		trakEngine = NULL;
	}
	if (rotationTracker != NULL)
	{
		rotationTracker->unref();
		rotationTracker = NULL;
	}
}

void SoMenuManager::addChild(SoNode*  node)
{
	SoGroup::addChild(node);

	if (node->isOfType(SoMenuWidget::getClassTypeId()))
	{
		SoMenuWidget* widget = static_cast<SoMenuWidget*>(node);
		widget->setParent(this);
	}
}

void SoMenuManager::createTrakEngine()
{
	trakEngine = new SoTrakEngine();
	trakEngine->ref();
}

void SoMenuManager::createRotationTracker()
{
	rotationTracker = new SoRotationTracker();
	addChild(rotationTracker);
	rotationTracker->ref();

	// connect our own station field to the SoTrakEngine station field
	trakEngine->station.connectFrom(&station);
	trakEngine->station.enableConnection(TRUE);
	trakEngine->station.setValue(station.getValue());
	
	// connect button0 to field sensor
	SET_SENSOR(buttonSensor, (trakEngine->buttonIn0), SoMenuManager::buttonChanged);
	
	SoRotationTracker* rotTracker = rotationTracker;

	// connect SoRotationTracker's primaryAngle field to a sensor
	SET_SENSOR(primaryAngleSensor, (rotTracker->primaryAngleIncrement), SoMenuManager::primaryAngleChanged);

	// connect SoRotationTracker's secondaryAngle field to a sensor
	SET_SENSOR(secondaryAngleSensor, (rotTracker->secondaryAngle), SoMenuManager::secondaryAngleChanged);

	// connect SoRotationTracker's rotationIn field to a sensor
	SET_SENSOR(rotationInSensor, (rotTracker->rotationIn), SoMenuManager::rotationInChanged);

	// connect SoTrakEngine's rotation to SoRotationTracker's rotationIn field
	rotTracker->rotationIn.connectFrom(&trakEngine->rotationIn);
	rotTracker->rotationIn.enableConnection(TRUE);
}

void SoMenuManager::executeCommand(SbString command)
{
	// Check if the command is the name of a widget
	if (getWidget(command) != NULL)
	{
		currentWidget.setValue(command);
	}
}


SoMenuWidget* SoMenuManager::getWidget(SbString name)
{
	int children = getNumChildren();
	for(int i = 0; i < children; i++)
	{
		SoNode* childnode = getChild(i);
		if ( (childnode->isOfType(SoMenuWidget::getClassTypeId())) ||
			(childnode->getClassTypeId().isDerivedFrom(SoMenuWidget::getClassTypeId())) ||
			(childnode->getClassTypeId().isDerivedFrom(SoRingWidget::getClassTypeId())) ||
			(childnode->getClassTypeId().isDerivedFrom(SoArrayWidget::getClassTypeId())))
		{
			SoMenuWidget* child = (SoMenuWidget*)childnode;
			if (child->getName() == name) { return child; }
		}
	}
	
	return NULL; // Widget not found
}

SoMenuWidget* SoMenuManager::getWidget(int index)
{
	if ((index < 0) || (index >= getNumChildren())) return NULL;

	return (SoMenuWidget*)getChild(index);
}

int SoMenuManager::getWidgetIndex(SbString name)
{
	int children = getNumChildren();
	for(int i = 0; i < children; i++)
	{
		SoNode* childnode = getChild(i);
		if ( (childnode->isOfType(SoMenuWidget::getClassTypeId())) ||
			(childnode->getClassTypeId().isDerivedFrom(SoMenuWidget::getClassTypeId())) ||
			(childnode->getClassTypeId().isDerivedFrom(SoRingWidget::getClassTypeId())) ||
			(childnode->getClassTypeId().isDerivedFrom(SoArrayWidget::getClassTypeId())))
		{
			SoMenuWidget* child = (SoMenuWidget*)childnode;
			if (child->getName() == name) { return i; }
		}
	}
	
	return -1; // Widget not found
}

SbBool SoMenuManager::setCurrentWidget(SbString name)
{
	if (active.getValue() == FALSE)
	{ 
		whichChild.setValue(SO_SWITCH_NONE); 
		return false; 
	}
	if (name == "") return false;

	int childIndex = getWidgetIndex(name);
	if (childIndex != -1)
	{
		// Found the desired widget, store it
		current = getWidget(childIndex);

		whichChild = childIndex;
		return true;
	}

	return false; // No widget found
}


void SoMenuManager::checkCurrentWidget()
{
	if (current == NULL)
		setCurrentWidget(currentWidget.getValue().getString());
}


void SoMenuManager::processClick()
{
	// React on user input. Forward this to the currently active widget
	if (active.getValue() == FALSE)
	{
		active.setValue(TRUE);
		return;
	}

	if (current != NULL)
	{
		SbTime timeOutInterval;
		timeOutInterval.setMsecValue(buttonPressTimeOut.getValue());

		if ((mode.getValue() == MENU_TIMEOUT) &&
			((SbTime::getTimeOfDay() - buttonPressTime) > timeOutInterval))
		{
			// only process this on SO_MENU_MODE_TIMEOUT mode
			current->onClickConfirm();

			if (current->name.getValue().getString() == startWidget.getValue())
			{
				whichChild.setValue(SO_SWITCH_NONE);
				active.setValue(FALSE);
				return;
			}
		}
		else
		{
			current->onClickSelect();
		}
	}
}


void SoMenuManager::currentWidgetChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoMenuManager::getClassTypeId()));

	SoMenuManager *self = (SoMenuManager*)data;

	// update history
	self->pushCurrentWidget(self->currentWidget.getValue());

	self->setCurrentWidget(self->currentWidget.getValue());
}

void SoMenuManager::startWidgetChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoMenuManager::getClassTypeId()));

	SoMenuManager *self = (SoMenuManager*)data;

	self->currentWidget = self->startWidget;
}

void SoMenuManager::buttonChanged(void *data, SoSensor *sensor)
{
	// Take button input from trak engine and use it for menu selection
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoMenuManager::getClassTypeId()));

	SoMenuManager *self = (SoMenuManager*)data;
	if (self->current == NULL) 
		self->checkCurrentWidget();

	if (self->trakEngine->buttonIn0.getValue())
	{
		// Button is pressed now, set current timestamp
		if (!self->lastButtonClick)
			self->buttonPressTime = SbTime::getTimeOfDay();

		self->lastButtonClick = true; // Button stays clicked
	}
	else 
	{
		// Releasing the button now
		if (self->lastButtonClick)
			self->processClick();

		self->lastButtonClick = false; // Button is released
	}
}

void SoMenuManager::primaryAngleChanged(void *data, SoSensor *sensor)
{
	// Take input from rotation tracker and forward it to the currently active widget
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoMenuManager::getClassTypeId()));

	SoMenuManager *self = (SoMenuManager*)data;
	if (self->current == NULL) 
		self->checkCurrentWidget();

	if (self->current != NULL)
	{
		float angleincrement = self->rotationTracker->primaryAngleIncrement.getValue();
		self->current->onPrimaryRotationChanged(angleincrement);
	}
}

void SoMenuManager::secondaryAngleChanged(void *data, SoSensor *sensor)
{
	// Take input from rotation tracker and forward it to the currently active widget
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoMenuManager::getClassTypeId()));

	SoMenuManager *self = (SoMenuManager*)data;
	if (self->current == NULL) 
		self->checkCurrentWidget();

	if (self->current != NULL)
	{
		float angle = self->rotationTracker->secondaryAngle.getValue();

		if ( (self->mode.getValue() == MENU_SECONDARY_ROTATION) &&
			 (angle > self->menuConfirmAngle.getValue()))
		{
			// only process this on SO_MENU_MODE_SECONDARY_ROTATION mode
			self->current->onClickConfirm();

			if (self->current->name.getValue().getString() == self->startWidget.getValue())
			{
				self->whichChild.setValue(SO_SWITCH_NONE);
				self->active.setValue(FALSE);
				return;
			}
		}

		self->current->onSecondaryRotationChanged(angle);
	}
}

void SoMenuManager::rotationInChanged(void *data, SoSensor *sensor)
{
	// Take input from rotation tracker and forward it to the currently active widget
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoMenuManager::getClassTypeId()));

	SoMenuManager *self = (SoMenuManager*)data;
	if (self->current == NULL) { self->checkCurrentWidget(); }

	if (self->current != NULL)
	{
		// direct rotation directly to widget, not as increment
		self->current->onRotationInChanged(self->rotationTracker->rotationIn.getValue());
	}
}

void SoMenuManager::activeChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoMenuManager::getClassTypeId()));

	SoMenuManager *self = (SoMenuManager*)data;
	if (self->active.getValue() == TRUE)
	{
		self->setCurrentWidget(self->startWidget.getValue());
		self->rotationTracker->startRotation.setValue(self->trakEngine->rotationIn.getValue());
	} 
	else 
	{
		self->whichChild.setValue(SO_SWITCH_NONE);
	}
}

void SoMenuManager::exitCurrentWidget()
{
	// either jump back once in history or go back completely to the start widget, depending
	// on the menuConfirmMode field
	if (menuConfirmMode.getValue() == EXIT_TO_START)
	{		
		currentWidget.setValue(startWidget.getValue().getString());
	}
	else
	{
		currentWidget.setValue(popLastWidget());
	}
}

void SoMenuManager::pushCurrentWidget(SbString newEntry)
{
	// only add if last entry differs
	if (history[history.getNum()-1] != newEntry)
	{
		// a little bit tricky due to the nature of SoMFString...

        history.insertSpace(history.getNum(), 1);
		history.set1(history.getNum()-1, newEntry.getString());
	}
}

SbString SoMenuManager::popLastWidget()
{
	// delete last history entry
	history.deleteValues(history.getNum() - 1, 1);
	return history[history.getNum() - 1];
}




