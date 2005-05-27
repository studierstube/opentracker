/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoRotationTracker.h contains the definition of SoRotationTracker.
 * SoRotationTracker translates a given rotation (usually fed from the 
 * iOrb) into primary and secondary rotations.
 *
 * @author  EMC
 *
 * $Id: SoRotationTracker.h,v 1.8 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_ROTATION_TRACKER_H_
#define _SO_ROTATION_TRACKER_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "extension.h"
#include "globals.h"

/** 
 * SoRotationTracker translates a given rotation (usually fed from the 
 * iOrb) into primary and secondary rotations.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoRotationTracker : public SoNode 
{
	SO_NODE_HEADER(SoRotationTracker);

public:
	/**
	 * Constructor.
	 */
	SoRotationTracker(void);

	/**
	 * Required static method.
	 */
	static void initClass(void);

	// fields
	SoSFRotation	rotationIn;			// the measurement rotations, this input drives the node
	SoSFRotation	startRotation;		// this is the startRotation used. history is reset when it changes
	SoSFInt32		history;			// number of rotations to store and use for axis computations

	// output fields, these are overwritten with the results of the computations
	SoSFRotation	primary;
	SoSFVec3f		primaryAxis;
	SoSFFloat		primaryAngle;
    SoSFFloat		primaryAngleIncrement;

	SoSFRotation	secondary;
	SoSFVec3f		secondaryAxis;
	SoSFFloat		secondaryAngle;
    SoSFFloat		secondaryAngleIncrement;

protected:
	// destructor
	virtual ~SoRotationTracker();

    // sensors
	SoFieldSensor rotationInSensor;
	SoFieldSensor startRotationSensor;
	SoFieldSensor historySensor;

	// static sensor methods
	static void rotationInChanged(void *data, SoSensor *sensor);
	static void startRotationChanged(void *data, SoSensor *sensor);
	static void historyChanged(void *data, SoSensor *sensor);

    /// number of history list elements that have been added already, must be <= history.getValue().
	int historyCounter;
    // stores the rotation axis history. allocated with allocateHistory()
    SbVec3f *axisHistory;
	// stores the rotation angle history. allocated with allocateHistory()
	float	*angleHistory;

	// methods

	/**
	 * Updates the primary axis information.
	 * This averages all the axis of all the rotations in the rotationHistory array that
	 * don't have a zero rotation (i.e. that are not identity quaternions).
	 */
	void updatePrimaryAngle();

	/**
	 * Calculates the final primary angle (and also the secondary rotation.
	 */
	void computeAngle();
	
	/**
	 * Allocates memory for the rotation history.
	 * The size of the rotationHistory array is determined by the history field.
	 */
	void allocateHistory();

};

#endif // _SO_ROTATION_TRACKER_H_
