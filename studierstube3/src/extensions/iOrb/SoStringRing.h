/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoStringRing.h contains the definition of SoStringRing.
 * SoStringRing is a ring type widget that supports a single or multi
 * select list of strings.
 *
 * @author  EMC
 *
 * $Id: SoStringRing.h,v 1.9 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_STRING_RING_H_
#define _SO_STRING_RING_H_

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "SoRingWidget.h"

/** 
 * SoStringRing is a ring type widget that supports a single or multi
 * select list of strings.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoStringRing: public SoRingWidget
{
	SO_KIT_HEADER(SoStringRing);

public:
	// construction
	SoStringRing(void);
	static void initClass(void);

	// fields
	SoMFString	items;
	SoMFString	selected;
	SoMFBool	option;
	SoSFBool	multipleSelection;

	// user input methods
	virtual void onClickSelect();
	virtual void onClickConfirm();
	virtual void onPrimaryRotationChanged(float angleIncrement);
	virtual void onSecondaryRotationChanged(float angleIncrement);

protected:
	// methods
	virtual ~SoStringRing(void);
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	void setRingRotation();

	void createGeometry();
	void destroyGeometry();
	
	void setSelected();

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

#endif // _SO_STRING_RING_H_
