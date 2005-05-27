/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoMenuManager.h contains the definition of SoMenuManager.
 * SoMenuManager is the manager for the main menu framework of the iOrb
 * user interface. It manages the widget list as well as the navigation
 * between widgets.
 *
 * @author  EMC
 *
 * $Id: SoMenuManager.h,v 1.21 2005/01/26 23:16:52 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_MENU_MANAGER_H_
#define _SO_MENU_MANAGER_H_

#include <Inventor/nodes/SoSwitch.h>

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>

#include <Inventor/SbTime.h> 

#include <Inventor/sensors/SoFieldSensor.h>

#include "extension.h"

class SoTrakEngine;
class SoRotationTracker;
class SoMenuWidget;

/**
 * SoMenuManager is the manager for the main menu framework of the iOrb
 * user interface. It manages the widget list as well as the navigation
 * between widgets.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoMenuManager: public SoSwitch
{
	SO_NODE_HEADER(SoMenuManager);

public:
	// construction
	SoMenuManager(void);
	static void initClass(void);

	// public methods
	void executeCommand(SbString command);
	void exitCurrentWidget();

	// public types

	enum MenuMode {
		MENU_TIMEOUT,
		MENU_SECONDARY_ROTATION
	};

	enum ConfirmMode {
		EXIT_TO_START,
		EXIT_ONE_LEVEL
	};

	// fields
	SoSFString	startWidget;		// the widget that represents the main menu
	SoSFEnum	mode;				// one-level-back mode: 
									// long button press or secondary rotation?
									// SO_MENU_MODE_TIMEOUT (default) || SO_MENU_MODE_SECONDARY_ROTATION

	SoSFBool	active;				// is the main menu active?
	SoSFString	currentWidget;		// currently displayed widget
    SoMFString	history;			// history of active widgets
	SoSFInt32	station;			// tracker station
	SoSFInt32	buttonPressTimeOut;	// button time-out for going back one level in msecs
	SoSFFloat	menuConfirmAngle;	// angle at which the "menu back" command is issued (in radiants)
	SoSFEnum	menuConfirmMode;	// EXIT_TO_START or EXIT_ONE_LEVEL (go back to startWidget immediately, 
									// or just jump back one step in the history)


protected:
	// destructor
	virtual ~SoMenuManager(void);
	
	// methods
	virtual void addChild(SoNode*  node);

	void createTrakEngine();
	void createRotationTracker();

	/**
	 * push the current widget into history
	 */
	void pushCurrentWidget(SbString newEntry);

	/**
	 * get's the last current widget
	 */
	SbString popLastWidget();

	SoMenuWidget*	getWidget(SbString name);
	int				getWidgetIndex(SbString name);
	SoMenuWidget*	getWidget(int index);
	SbBool			setCurrentWidget(SbString name);
	
	void checkCurrentWidget();
	void processClick();

	// sensors
	SoFieldSensor startWidgetSensor;
	SoFieldSensor currentWidgetSensor;
	SoFieldSensor buttonSensor;
	SoFieldSensor primaryAngleSensor;
	SoFieldSensor secondaryAngleSensor;
	SoFieldSensor rotationInSensor;
	SoFieldSensor activeSensor;


	// static sensor methods
	static void currentWidgetChanged(void *data, SoSensor *sensor);
	static void startWidgetChanged(void *data, SoSensor *sensor);
	static void buttonChanged(void *data, SoSensor *sensor);
	static void primaryAngleChanged(void *data, SoSensor *sensor);
	static void secondaryAngleChanged(void *data, SoSensor *sensor);
	static void rotationInChanged(void *data, SoSensor *sensor);
	static void activeChanged(void *data, SoSensor *sensor);

	// members
	SoMenuWidget*		current;
	SoRotationTracker*	rotationTracker;
	SoTrakEngine*		trakEngine;

	SbBool	lastButtonClick;
	SbTime	buttonPressTime;

};

#endif // _SO_MENU_MANAGER_H_
