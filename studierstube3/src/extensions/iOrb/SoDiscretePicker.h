/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoDiscretePicker.h contains the definition of SoDiscretePicker.
 * SoDiscretePicker is a node which lets you specify a set of coordinates.
 * It then performs a ray pick on it based on a starting position and the
 * rotation it gets fed.
 *
 * @author  EMC
 *
 * $Id: SoDiscretePicker.h,v 1.8 2005/01/22 20:10:26 chris Exp $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _SO_DISCRETE_PICKER_H_
#define _SO_DISCRETE_PICKER_H_

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "extension.h"

/** 
 * SoDiscretePicker is a node which lets you specify a set of coordinates.
 * It then performs a ray pick on it based on a starting position and the
 * rotation it gets fed.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoDiscretePicker: public SoNode
{
	SO_NODE_HEADER(SoDiscretePicker);

public:
	// construction
	SoDiscretePicker(void);
	static void initClass(void);

	// configuration fields
	SoMFVec3f		point;
	SoMFString		value;
	SoSFFloat		separation;

	// input fields
	SoSFRotation	rotation;
	SoSFVec3f		position;

	// output fields
	SoSFString		selected;		// empty if not selected
	SoSFInt32		selectedIndex;	// -1 if not selected
	SoSFVec3f		selectedPoint;	// 0, 0, 0 is not selected
	SoSFInt32		switchControl;	// output field to control a SoSwitch's whichChild field

protected:
	// methods
	virtual ~SoDiscretePicker(void);

	void updatePicking();
	
	float getAngleBetween(SbVec3f& v1, SbVec3f& v2);

    // field sensor stuff
    static void pointChanged(void* data, SoSensor* sensor);
    SoFieldSensor pointSensor;

	static void rotationChanged(void* data, SoSensor* sensor);
	SoFieldSensor rotationSensor;

	static void positionChanged(void* data, SoSensor* sensor);
	SoFieldSensor positionSensor;

	// member attributes
};

#endif // _SO_DISCRETE_PICKER_H_
