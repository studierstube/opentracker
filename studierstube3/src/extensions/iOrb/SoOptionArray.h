/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoOptionArray.h contains the definition of SoOptionArray.
 * SoOptionArray is an array type (list type) UI widget that supports a
 * single or multi select list of bool values.
 *
 * @author  EMC
 *
 * $Id: SoOptionArray.h,v 1.7 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _SO_OPTION_ARRAY_H_
#define _SO_OPTION_ARRAY_H_

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "SoArrayWidget.h"

/** 
 * SoOptionArray is an array type (list type) UI widget that supports a
 * single or multi select list of bool values.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoOptionArray: public SoArrayWidget
{
	SO_KIT_HEADER(SoOptionArray);

public:
	// construction
	SoOptionArray(void);
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
	virtual ~SoOptionArray(void);
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

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

	float accumRotation;

};

#endif // _SO_OPTION_ARRAY_H_
