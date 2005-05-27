/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoCommandRing.h contains the definition of SoCommandRing.
 * SoCommandRing is a ring type widget that supports a single select
 * list of commands.
 *
 * @author  EMC
 *
 * $Id: SoCommandRing.h,v 1.17 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_COMMAND_RING_H_
#define _SO_COMMAND_RING_H_

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/sensors/SoFieldSensor.h>


#include "SoRingWidget.h"

/** 
 * SoCommandRing is a ring type widget that supports a single select
 * list of commands.
 * 
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoCommandRing: public SoRingWidget
{
	SO_KIT_HEADER(SoCommandRing);

public:
	// construction
	SoCommandRing(void);
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
	virtual ~SoCommandRing(void);
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
	float	currentAngle;
};

#endif // _SO_COMMAND_RING_H_
