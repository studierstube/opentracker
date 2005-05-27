/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoStringArray.h contains the definition of SoStringArray.
 * SoStringArray is an array type (list type) UI widget that supports a
 * single or multi select string list.
 *
 * @author  EMC
 *
 * $Id: SoStringArray.h,v 1.6 2005/01/22 20:10:27 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_STRING_ARRAY_H_
#define _SO_STRING_ARRAY_H_

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "SoArrayWidget.h"

/** 
 * SoStringArray is an array type (list type) UI widget that supports a
 * single or multi select string list.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoStringArray: public SoArrayWidget
{
	SO_KIT_HEADER(SoStringArray);

public:
	// construction
	SoStringArray(void);
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
	virtual ~SoStringArray(void);
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
	float accumRotation;
};

#endif // _SO_STRING_ARRAY_H_
