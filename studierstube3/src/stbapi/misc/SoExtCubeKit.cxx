/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoExtCubeKit - implementation
  *
  * @author  Markus Krutz, Gerd Hesina
  *
  * $Id: SoExtCubeKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoExtCubeKit.h"

SO_KIT_SOURCE(SoExtCubeKit);

/***********************************************************************************************
Function  * void initClass()
Purpose   * Class initializer.
Arguments * -
Return    * -
Notes     * -
***********************************************************************************************/
void SoExtCubeKit::initClass(void) {
	SO_KIT_INIT_CLASS(SoExtCubeKit, SoBaseKit, "BaseKit");
}

/***********************************************************************************************
Function  * SoExtCubeKit()
Purpose   * Constructor. Standard procedere.
Arguments * -
Return    * -
Notes     * There's a html-help file for this class.
***********************************************************************************************/
SoExtCubeKit::SoExtCubeKit() {
	SO_KIT_CONSTRUCTOR(SoExtCubeKit);

	SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, this, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(shapeHints, SoShapeHints, FALSE, topSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(boxSeparator, SoSeparator, FALSE, topSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(boxMaterial, SoMaterial, FALSE, boxSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(boxCoordinates, SoCoordinate3, FALSE, boxSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(boxNormals, SoNormal, FALSE, boxSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(boxNormalBinding, SoNormalBinding, FALSE, boxSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(boxIndexedFaceSet, SoIndexedFaceSet, FALSE, boxSeparator, \x0, TRUE);

	SO_KIT_ADD_FIELD(width, (2.0));
	SO_KIT_ADD_FIELD(height, (2.0));
	SO_KIT_ADD_FIELD(depth, (2.0));

	SO_KIT_INIT_INSTANCE();

	SoShapeHints *sh = SO_GET_ANY_PART(this, "shapeHints", SoShapeHints);
	sh->shapeType = SoShapeHints::SOLID;
	sh->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	sh->faceType = SoShapeHints::CONVEX;

	SoMaterial *m = SO_GET_ANY_PART(this, "boxMaterial", SoMaterial);
	m->ambientColor.setValue(0.31, 0.424, 0.565);
	m->diffuseColor.setValue(0.31, 0.424, 0.565);
	m->specularColor.setValue(0.0, 0.0, 0.0);
	m->shininess = 0.4;
	m->transparency = 0.0;

	SoCoordinate3 *coord = SO_GET_ANY_PART(this, "boxCoordinates", SoCoordinate3);
	const float coords[8][3] = {
		{ -1.0, -1.0, 1.0 },
		{ 1.0, -1.0, 1.0 },
		{ -1.0, -1.0, -1.0 },
		{ 1.0, -1.0, -1.0 },
		{ -1.0, 1.0, 1.0 },
		{ 1.0, 1.0, 1.0 },
		{ -1.0, 1.0, -1.0 },
		{ 1.0, 1.0, -1.0 }
	};
	coord->point.setValues(0, 8, coords);

	SoNormal *normal = SO_GET_ANY_PART(this, "boxNormals", SoNormal);
	const float normals[6][3] = {
		{ 1.0, 0.0, 0.0 },
		{ -1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, 0.0, -1.0 }
	};

	normal->vector.setValues(0, 6, normals);

	SoNormalBinding *nb = SO_GET_ANY_PART(this, "boxNormalBinding", SoNormalBinding);
	nb->value = SoNormalBinding::PER_VERTEX_INDEXED;

	SoIndexedFaceSet *ifs = SO_GET_ANY_PART(this, "boxIndexedFaceSet", SoIndexedFaceSet);
	const int coordIndex[4*12] = {
		2, 0, 3, -1,
		1, 3, 0, -1,
		5, 4, 7, -1,
		6, 7, 4, -1, 
		1, 0, 5, -1, 
		4, 5, 0, -1, 
		3, 1, 7, -1, 
		5, 7, 1, -1, 
		2, 3, 6, -1, 
		7, 6, 3, -1, 
		0, 2, 4, -1, 
		6, 4, 2, -1
	};
	const int normalIndex[4*12] = {
	    2, 2, 2, -1,
		2, 2, 2, -1,
		4, 4, 4, -1,
		4, 4, 4, -1,
		5, 5, 5, -1,
		5, 5, 5, -1,
		1, 1, 1, -1,
		1, 1, 1, -1,
		3, 3, 3, -1,
		3, 3, 3, -1,
		0, 0, 0, -1,
		0, 0, 0, -1
	};
	ifs->coordIndex.setValues(0, 4*12, coordIndex);
	ifs->normalIndex.setValues(0, 4*12, normalIndex);

	widthSensor = new SoFieldSensor(sensorCB, (void *)this);
	heightSensor = new SoFieldSensor(sensorCB, (void *)this);
	depthSensor = new SoFieldSensor(sensorCB, (void *)this);
	widthSensor->attach(&width);
	heightSensor->attach(&height);
	depthSensor->attach(&depth);
    setSearchingChildren(TRUE);
}

/***********************************************************************************************
Function  * ~SoExtCubeKit()
Purpose   * Destructor. Detach sensors and delete them.
Arguments * -
Return    * -
Notes     * -
***********************************************************************************************/
SoExtCubeKit::~SoExtCubeKit() {
	widthSensor->detach();
	heightSensor->detach();
	depthSensor->detach();
	if (widthSensor)
		delete widthSensor;
	if (heightSensor)
		delete heightSensor;
	if (depthSensor)
		delete depthSensor;
}

#define SGN(x) ((x) < 0 ? -1 : +1)
/***********************************************************************************************
Function  * void sensorCB(void *userData, SoSensor *sensor)
Purpose   * Called if one of the following fields changes: width, height, depth. The geometry gets
					* adapted to the new size.
Arguments * userData... this
					* sensor... pointer to sensor, which invoked this callback
Return    * -
Notes     * static.
***********************************************************************************************/
void SoExtCubeKit::sensorCB(void *userData, SoSensor *sensor) {
	SoExtCubeKit *self = (SoExtCubeKit *)userData;
	SoCoordinate3 *c = SO_GET_ANY_PART(self, "boxCoordinates", SoCoordinate3);
	SbVec3f v;
	
	const int sign1[8] = { -1, 1, -1, 1, -1, 1, -1, 1 };
	const int sign2[8] = { -1, -1, -1, -1, 1, 1, 1, 1 };
	const int sign3[8] = { 1, 1, -1, -1, 1, 1, -1, -1 };

	if (sensor == self->widthSensor) {
		float x = self->width.getValue()/2;
		for (int i=0; i<8; i++) {
			v = c->point[i];
			c->point.set1Value(i, x*sign1[i], v[1], v[2]); // was SGN(v[0])
		}
	}
	if (sensor == self->heightSensor) {
		float y = self->height.getValue()/2;
		for (int i=0; i<8; i++) {
			v = c->point[i];
			c->point.set1Value(i, v[0], y*sign2[i], v[2]);
		}
	}
	if (sensor == self->depthSensor) {
		float z = self->depth.getValue()/2;
		for (int i=0; i<8; i++) {
			v = c->point[i];
			c->point.set1Value(i, v[0], v[1], z*sign3[i]);
		}
	}
}

void SoExtCubeKit::setDefaultOnNonWritingFields() {
	SoBaseKit::setDefaultOnNonWritingFields();
//	clipPlaneGroup.setDefault(TRUE);
}

