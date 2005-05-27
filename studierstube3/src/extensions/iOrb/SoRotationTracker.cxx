/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoRotationTracker.cxx
 *
 * @author  EMC
 *
 * $Id: SoRotationTracker.cxx,v 1.1 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#include <stbapi/misc/SoFactorRotation.h>

#include "SoRotationTracker.h"

SO_NODE_SOURCE(SoRotationTracker);

void SoRotationTracker::initClass(void)
{
	if (SoType::fromName("SoRotationTracker").isBad())
	{
		SO_NODE_INIT_CLASS(SoRotationTracker, SoNode, "Node");
	}
}

SoRotationTracker::SoRotationTracker(void): axisHistory(NULL), angleHistory(NULL), historyCounter(0)
{
	SO_NODE_CONSTRUCTOR(SoRotationTracker);

	// field macros
	SO_NODE_ADD_FIELD(rotationIn, (SbVec3f(0, 0, 1), 0));
	SO_NODE_ADD_FIELD(startRotation, (SbVec3f(0, 0, 1), 0));
	SO_NODE_ADD_FIELD(history, (10));

	SO_NODE_ADD_FIELD(primary, (SbVec3f(0, 0, 1), 0));
	SO_NODE_ADD_FIELD(primaryAxis, (0, 0, 1));
	SO_NODE_ADD_FIELD(primaryAngle, (0.0f));

	SO_NODE_ADD_FIELD(secondary, (SbVec3f(0, 0, 1), 0));
	SO_NODE_ADD_FIELD(secondaryAxis, (0, 0, 1));
	SO_NODE_ADD_FIELD(secondaryAngle, (0.0f));

	SO_NODE_ADD_FIELD(primaryAngleIncrement, (0.0f));
	SO_NODE_ADD_FIELD(secondaryAngleIncrement, (0.0f));

	// set up field sensors
	rotationInSensor.setData(this);
	rotationInSensor.setFunction(SoRotationTracker::rotationInChanged);
	rotationInSensor.attach(&rotationIn);
	rotationInSensor.setPriority(0);

	startRotationSensor.setData(this);
	startRotationSensor.setFunction(SoRotationTracker::startRotationChanged);
	startRotationSensor.attach(&startRotation);
	startRotationSensor.setPriority(0);

	allocateHistory();
}

SoRotationTracker::~SoRotationTracker()
{
	if (axisHistory != NULL)
	{
		delete[] axisHistory;
		axisHistory = NULL;
	}
	if (angleHistory != NULL)
	{
		delete[] angleHistory;
		angleHistory = NULL;
	}
}

void SoRotationTracker::rotationInChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoRotationTracker::getClassTypeId()));

	SoRotationTracker *self = (SoRotationTracker*)data;

	SbVec3f axis; float angle;
	self->rotationIn.getValue(axis, angle);

	// calculate r_diff = r_m * r_start^-1 where r_m is the current orientation and r_start
	// is the start rotation. store the result at the back of the history list.
	SbRotation invStartRotation = self->startRotation.getValue();
	invStartRotation.invert();

	SbRotation diffRotation = self->rotationIn.getValue() * invStartRotation;

	diffRotation.getValue( axis, angle );

	if( fabs(angle) < 0.1)
		return;

	// store in next position in history, unless history is full
	if (self->historyCounter < self->history.getValue())
	{
		diffRotation.getValue(axis, angle);

		self->axisHistory[self->historyCounter] = axis;
		self->angleHistory[self->historyCounter] = angle;

		self->historyCounter++;

		// since history changed, calculate new primary axis
		self->updatePrimaryAngle();

		// calculate angle
		self->computeAngle();
	}
	else
	{
		// primary axis is fixed because we have historyCount elements in the history list.
		// now we just calculate the primary angle and the secondary rotation

		diffRotation.getValue(axis, angle);

		// since history is full until new startRotation specified or history newly allocated,
		// we can just safely overwrite the last position in the array, because computeAngle()
		// takes that value
		self->axisHistory[self->historyCounter - 1] = axis;
		self->angleHistory[self->historyCounter - 1] = angle;

		// calculate angle
		self->computeAngle();
	}
}

void SoRotationTracker::updatePrimaryAngle()
{
	// Track axis - first, make sure all the axis are in the same half-space by comparing the 
	// dot product of the last one with each other and reversing each axis where the result is < 0.
	// (see wiki for details)
	SbVec3f reference_axis = axisHistory[0];
	float reference_angle = angleHistory[0];
	
	// Debug output
	for (int i = historyCounter - 2; i >= 0; i--)
	{
		if (axisHistory[i] != SbVec3f(0,0,0))
		{
			if (axisHistory[i].dot(reference_axis) < 0)
			{
				axisHistory[i] = -axisHistory[i];
				angleHistory[i] = -angleHistory[i];
			}
		}
	}

	// average the axis using simple arithmetic average
	// (see wiki for details)
	SbVec3f result;
	int numHistory = 0;
	for (int i = 0; i < historyCounter; i++)
	{
		if ((axisHistory[i] != SbVec3f(0,0,0)) || (angleHistory[i] != 0))
		{
			result += axisHistory[i];
			numHistory++;
		}
	}

	result.normalize();

    // set primary axis
	primaryAxis = result;
}

void SoRotationTracker::computeAngle()
{
	// compute the angle (see wiki for details) based on the primary axis
	SbVec3f result = primaryAxis.getValue(); // result from updatePrimaryAxis()
	SbRotation r_canonical = SbRotation(result, SbVec3f(1, 0, 0));
	SbRotation r_diff = SbRotation(axisHistory[historyCounter - 1], angleHistory[historyCounter - 1]);
	r_diff = r_canonical * r_diff * r_canonical.inverse();

	SbRotation r_main;
	SbRotation r_secondary;
	SoFactorRotation::computeFactorization(r_diff, r_main, r_secondary, SoFactorRotation::X, SoFactorRotation::PRE);

	// r_main = r_canonical.inverse() * r_main * r_canonical;

	SbVec3f primary_axis; float primary_angle;
	SbVec3f secondary_axis; float secondary_angle;
	r_main.getValue(primary_axis, primary_angle);
	r_secondary.getValue(secondary_axis, secondary_angle);

	// now set the output fields
	primary			= r_main;
	float oldPrimaryAngle = primaryAngle.getValue();
	primaryAngle	= primary_angle;
	float newPrimaryIncrement = primaryAngle.getValue() - oldPrimaryAngle;

	if (newPrimaryIncrement < (-PI)) newPrimaryIncrement = 2*PI + newPrimaryIncrement;
	if (newPrimaryIncrement > (PI)) newPrimaryIncrement = newPrimaryIncrement - 2*PI;

	primaryAngleIncrement = newPrimaryIncrement;



	secondary		= r_secondary;
	secondaryAxis	= secondary_axis;
	secondaryAngle	= secondary_angle;
}

void SoRotationTracker::startRotationChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoRotationTracker::getClassTypeId()));

	SoRotationTracker *self = (SoRotationTracker*)data;

	// startRotation (the reference rotation) changed, need to clear history and start
	// trackin' again
	self->allocateHistory();

	SbVec3f temp;
	float angle;
	self->startRotation.getValue(temp, angle);
}

void SoRotationTracker::historyChanged(void *data, SoSensor *sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoRotationTracker::getClassTypeId()));

	SoRotationTracker *self = (SoRotationTracker*)data;

	self->allocateHistory();
}

void SoRotationTracker::allocateHistory()
{
	// clear old history, and allocate the history using the value of the "history" field
	if (axisHistory != NULL) delete[] axisHistory;
	if (angleHistory != NULL) delete[] angleHistory;

	axisHistory = new SbVec3f[history.getValue()];
	angleHistory = new float[history.getValue()];

	for (int i = 0; i < history.getValue(); i++)
	{
		axisHistory[i] = SbVec3f(0, 0, 0);
		angleHistory[i] = 0;
	}

	historyCounter = 0;
}

