/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoOptionRing.h contains the definition of SoOptionRing.
 * SoOptionRing is a ring type widget that supports a single or multi
 * select list of bool values.
 *
 * @author  EMC
 *
 * $Id: SoOptionRing.h,v 1.8 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_OPTION_RING_H_
#define _SO_OPTION_RING_H_

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "SoRingWidget.h"

/** 
 * SoOptionRing is a ring type widget that supports a single or multi
 * select list of bool values.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoOptionRing: public SoRingWidget
{
	SO_KIT_HEADER(SoOptionRing);

public:
	// construction
	SoOptionRing(void);
	static void initClass(void);

	// fields
	SoMFString	items;
	SoMFBool	option;
	SoSFBool	multipleSelection;

	// user input methods
	virtual void onClickSelect();
	virtual void onClickConfirm();
	virtual void onPrimaryRotationChanged(float angleIncrement);
	virtual void onSecondaryRotationChanged(float angleIncrement);

protected:
	// methods
	virtual ~SoOptionRing(void);
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);
	
	void setRingRotation();
	
	void createGeometry();
	void destroyGeometry();

	// sensors
	SoFieldSensor itemsSensor;
	
	// static sensor methods
	static void itemsChanged(void *data, SoSensor *sensor);
	
	// nodes
	SoTransform*	transform;
	SoSeparator*	geometrySep;

	// members
	int		currentItem;
	float	currentAngle;
};

#endif // _SO_OPTION_RING_H_
