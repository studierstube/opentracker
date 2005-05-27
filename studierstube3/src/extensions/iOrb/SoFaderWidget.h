/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoFaderWidget.h contains the definition of SoFaderWidget.
 * SoFaderWidget is a fader-like widget type where you can dial a value.
 *
 * @author  EMC
 *
 * $Id: SoFaderWidget.h,v 1.13 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_FADER_WIDGET_H_
#define _SO_FADER_WIDGET_H_

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoText2.h>

#include "SoMenuWidget.h"

/**
 * SoFaderWidget is a fader-like widget type where you can dial a value.
@verbatim
SoFaderWidget {
    SoSFInt32 minValue
    SoSFInt32 maxValue
    SoSFInt32 currentValue
}
@endverbatim
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoFaderWidget: public SoMenuWidget
{
	SO_KIT_HEADER(SoFaderWidget);

public:
	// construction
	SoFaderWidget(void);
	static void initClass(void);

	// fields

	SoSFBool  isMoving;		// determines whether the fader is tracking the rotation or not
							// default is FALSE
	SoSFInt32 minValue;
	SoSFInt32 maxValue;

	SoSFFloat scaleRotation;
	SoSFFloat minAngle;
	SoSFFloat maxAngle;
	SoSFFloat currentAngle;

	SoSFInt32 currentValue;

	SoSFString label;

	// user input methods
	virtual void onClickSelect();
	virtual void onClickConfirm();
	virtual void onPrimaryRotationChanged(float angleIncrement);
	virtual void onSecondaryRotationChanged(float angleIncrement);

protected:
	// methods

	virtual ~SoFaderWidget(void);

	void createGeometry();
	void destroyGeometry();

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	// member attributes
	SoTransform* transform;
	SoText2* textLabel;
	SoText2* textValue;
};

#endif // _SO_FADER_WIDGET_H_

