/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoDiscretePicker.cxx
 *
 * @author  EMC
 *
 * $Id: SoDiscretePicker.cxx,v 1.9 2005/01/24 19:26:36 emcgames Exp $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>

#include "globals.h"
#include "SoDiscretePicker.h"

SO_NODE_SOURCE(SoDiscretePicker);

void SoDiscretePicker::initClass(void)
{
	if (SoType::fromName("SoDiscretePicker").isBad())
	{
		SO_NODE_INIT_CLASS(SoDiscretePicker, SoNode, "Node");
	}
}

SoDiscretePicker::SoDiscretePicker(void)
{
	SO_NODE_CONSTRUCTOR(SoDiscretePicker);

	// fields
	SO_NODE_ADD_FIELD(point, (NULL));
	SO_NODE_ADD_FIELD(value, (NULL));
	SO_NODE_ADD_FIELD(separation, (DEG2RAD(15)));

	SO_NODE_ADD_FIELD(rotation, (SbVec3f(0, 0, 1), 0));
	SO_NODE_ADD_FIELD(position, (SbVec3f(0, 0, 0)));

	SO_NODE_ADD_FIELD(selected, (""));
	SO_NODE_ADD_FIELD(selectedIndex, (-1));
	SO_NODE_ADD_FIELD(selectedPoint, (SbVec3f(0, 0, 0)));
	SO_NODE_ADD_FIELD(switchControl, (SO_SWITCH_NONE));

	// sensors
	SET_SENSOR(pointSensor, point, SoDiscretePicker::pointChanged);
	SET_SENSOR(rotationSensor, rotation, SoDiscretePicker::rotationChanged);
	SET_SENSOR(positionSensor, position, SoDiscretePicker::positionChanged);
}

SoDiscretePicker::~SoDiscretePicker(void)
{
}

void SoDiscretePicker::pointChanged(void* data, SoSensor* sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoDiscretePicker::getClassTypeId()));
	SoDiscretePicker* self = (SoDiscretePicker*)data;

	// update the coordinates node
	self->updatePicking();
}

void SoDiscretePicker::rotationChanged(void* data, SoSensor* sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoDiscretePicker::getClassTypeId()));
	SoDiscretePicker* self = (SoDiscretePicker*)data;

	self->updatePicking();
}

void SoDiscretePicker::positionChanged(void* data, SoSensor* sensor)
{
	assert(data != NULL);
	assert(((SoNode*)data)->isOfType(SoDiscretePicker::getClassTypeId()));
	SoDiscretePicker* self = (SoDiscretePicker*)data;

	self->updatePicking();
}

void SoDiscretePicker::updatePicking()
{
	int num = point.getNum();
	float* angles = new float[num];

	const SbRotation& rot = rotation.getValue();
	const SbVec3f& source_pt = position.getValue();

	// default ray vectorgoes from position along z axis -> rotate according to rotation
	SbVec3f ray_vec = SbVec3f(0, 0, -1);
	rot.multVec(ray_vec, ray_vec);
	ray_vec.normalize();

	// calculate angles
	for (int i = 0; i < num; i++)
	{
		const SbVec3f& p = point[i];
        
		SbVec3f point_vec = p - source_pt;
		point_vec.normalize();

        // calculate angle between point_vec and ray_vec
		angles[i] = getAngleBetween(point_vec, ray_vec);
	}

	// find the minimum
	float	min_angle = 1000000;
	int		min_index = -1;

	for (int i = 0; i < num; i++)
	{
		if (angles[i] < min_angle)
		{
			min_angle = angles[i];
			min_index = i;
		}
	}

	if ((i != -1) && (min_angle <= separation.getValue()))
	{
		// a minimum angle was found, and it is within the minimum angle that is required
		// so turn on the switch, and set the appropiate coordinate

		// only set selected if the value array field is same size or larger than the point array field
		if (value.getNum() >= point.getNum())
			selected = value[min_index];

		selectedIndex = min_index;
		selectedPoint = point[min_index];
		switchControl.setValue(SO_SWITCH_ALL);
	}
	else
	{
		// either none found, or the one found wasnt within the minimum angle
		// so hide the marker sphere
		selected = "";
		selectedIndex = -1;
		selectedPoint = SbVec3f(0, 0, 0);
		switchControl.setValue(SO_SWITCH_NONE);
	}

	delete[] angles;
}

float SoDiscretePicker::getAngleBetween(SbVec3f& v1, SbVec3f& v2)
{
	SbVec3f n1 = v1;
	SbVec3f n2 = v2;
	n1.normalize();
	n2.normalize();

	float angle = acos(n1.dot(n2));

	return angle;
}
