/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoOptionBallWidget.h contains the definition of ChessManKit.
 * SoOptionBallWidget is a single-selection Option List arranged in a sphere.
 * Up to 6 options are supported.
 *
 * @author  EMC
 *
 * $Id: SoOptionBallWidget.h,v 1.12 2005/01/26 23:16:52 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_OPTION_BALL_WIDGET_H_
#define _SO_OPTION_BALL_WIDGET_H_

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>

#include "SoMenuWidget.h"

/**
 * SoOptionBallWidget is a single-selection Option List arranged in a sphere.
 * Up to 6 options are supported.
 *
@verbatim
SoOptionBall {
    # configuration fields
    SoSFEnum type 4 | 6 | 8 | 10 | 12 | 20   # number of faces -> defines geometry
    SoMFString value                         # string values 
    SoMFNode   valueNode                     # or whole geometries. can be scaled to fixed size...

    # input fields
    SoSFRotation rotation                    # the input rotation, relative to a start rotation given at active = TRUE ?
    SoSFBool     active                      # active or inactive ? 

    # output fields
    SoSFString selected                      # selected string value
    SoSFInt32  selectedIndex                 # selected index
}

STRUCTURE OF SoOptionBall SUB-SCENEGRAPH:

SoOptionBallWidget
|
geometry (getGeometry)
|                  |                         |
indicatorPart   ballPart (visualization)   pickerPart (ray picking root)
|                  |                         |
cube              transform                transform (clone from the other)
                  |        |                 |
                sphere   labels            cube
@endverbatim
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoOptionBallWidget: public SoMenuWidget
{
	SO_KIT_HEADER(SoOptionBallWidget);

public:
	// construction
	SoOptionBallWidget(void);
	static void initClass(void);

	enum TypeMode {
		OPTION_4 = 0,
		OPTION_6 = 1,
		OPTION_8 = 2,
		OPTION_10 = 3,
		OPTION_12 = 4,
		OPTION_20 = 5
	};

	// fields
	SoSFBool  isMoving;

	SoSFEnum type;							// number of faces -> defines geometry (currently only 6 is defined)
	SoMFString value;						// string values to put at appropiate places on ball surface

	// input fields
	SoSFRotation rotation;					// the input rotation, relative to a start rotation given at active = TRUE ?

	// output fields
	SoSFString selected;					// selected string value
	SoSFInt32  selectedIndex;				// selected index

	SoSFFloat		ballRadius;
	SoSFFloat		labelOffset;			// how far the labels are to be positioned off the sphere

	// user input methods
	virtual void onClickSelect();
	virtual void onClickConfirm();
	virtual void onPrimaryRotationChanged(float angleIncrement);
	virtual void onSecondaryRotationChanged(float angleIncrement);
	virtual void onRotationInChanged(const SbRotation& rotation);

protected:
	// methods

	virtual ~SoOptionBallWidget(void);

	void createGeometry();
	void destroyGeometry();

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	void createBallPart();
	void createPickerPart();

	void setLabelPos();

	// field sensors
	static void typeChanged(void *data, SoSensor *sensor);
	SoFieldSensor typeSensor;

	static void ballRadiusChanged(void *data, SoSensor *sensor);
	SoFieldSensor ballRadiusSensor;

	static void labelOffsetChanged(void *data, SoSensor *sensor);
	SoFieldSensor labelOffsetSensor;

	// member attributes
    SbRotation		startRotation;

	SoSwitch*		pickerSwitch;
	SoSeparator*	pickerPart;
	SoTransform*	pickerTransform;
	
	SoTransform*	ballTransform;

	SbVec3f*		labelPos;
	SoMaterial**	labelMat;
	SoFont**		labelFont;

};

#endif // _SO_OPTION_BALL_WIDGET_H_
