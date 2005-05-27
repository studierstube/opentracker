/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoCommandArray.h contains the definition of SoCommandArray.
 * SoCommandArray is an array type (list type) UI widget that supports a
 * single select list which specify commands.
 *
 * @author  EMC
 *
 * $Id: SoCommandArray.h,v 1.7 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_COMMAND_ARRAY_H_
#define _SO_COMMAND_ARRAY_H_

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>


#include "SoArrayWidget.h"
/** 
 * SoCommandArray is an array type (list type) UI widget that supports a
 * single select list which specify commands.
 * 
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoCommandArray: public SoArrayWidget
{
	SO_KIT_HEADER(SoCommandArray);

public:
	// construction
	SoCommandArray(void);
	static void initClass(void);

	// fields
	SoMFString items;
	SoMFString labels;
	SoSFString command;

	// user input methods
	virtual void onClickSelect();
	virtual void onClickConfirm();
	virtual void onPrimaryRotationChanged(float angleIncrement);
	virtual void onSecondaryRotationChanged(float angleIncrement);

protected:
	// methods
	virtual ~SoCommandArray(void);
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	void setRingRotation();
	
	void createGeometry();
	void destroyGeometry();

	void setCommand();

	// sensors
	SoFieldSensor itemsSensor;
	SoFieldSensor labelsSensor;
	
	// static sensor methods
	static void itemsChanged(void *data, SoSensor *sensor);
	static void labelsChanged(void *data, SoSensor *sensor);
	
	// nodes
	SoTransform*	transform;
	SoSeparator*	geometrySep;

	// members
	int		currentItem;

	float accumRotation;

};

#endif // _SO_COMMAND_ARRAY_H_
