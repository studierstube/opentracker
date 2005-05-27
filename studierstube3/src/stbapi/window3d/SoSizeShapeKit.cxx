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
/** SoSizeShapeKit - implementation
  *
  * @author Markus Krutz, Gerd Hesina
  *
  * $Id: SoSizeShapeKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "SoSizeShapeKit.h"
#include "misc.h"

SO_KIT_SOURCE(SoSizeShapeKit);

/***********************************************************************************************
Function  * void initClass()
Purpose   * Class initializer.
Arguments * -
Return    * -
Notes     * static.
***********************************************************************************************/
void SoSizeShapeKit::initClass() {
	SO_KIT_INIT_CLASS(SoSizeShapeKit, SoBaseKit, "BaseKit");
}

/***********************************************************************************************
Function  * SoSizeShapeKit()
Purpose   * Constructor.
Arguments * -
Return    * -
Notes     * see html-help.
***********************************************************************************************/
SoSizeShapeKit::SoSizeShapeKit() {
	SO_KIT_CONSTRUCTOR(SoSizeShapeKit);
	
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, this, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(scale, SoScale, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(geomSeparator, SoSeparator, TRUE, topSeparator, \x0, TRUE);

	SO_KIT_ADD_FIELD(size, (1.0, 1.0, 1.0));

	SO_KIT_INIT_INSTANCE();

	sizeSensor = new SoFieldSensor(SoSizeShapeKit::sizeSensorCB, (void *)this);
	sizeSensor->attach(&size);

	sizeSensor->schedule();
    setSearchingChildren(TRUE);
}

/***********************************************************************************************
Function  * void adjustScale(const SbVec3f& newSize)
Purpose   * Adjust scale-part to fit geometry to new size.
Arguments * newSize... new size
Return    * -
Notes     * -
***********************************************************************************************/
void SoSizeShapeKit::adjustScale(const SbVec3f& newSize) {
	SoSeparator *geom = (SoSeparator *)getPart("geomSeparator", FALSE);
	if (geom == NULL) return;

	static SbViewportRegion vp;
	SoGetBoundingBoxAction bba(vp);
	bba.apply(geom);
	SbBox3f bbox = bba.getBoundingBox();

	float x, y, z;
	bbox.getSize(x, y, z);


	SoScale *sc = SO_GET_ANY_PART(this, "scale", SoScale);
	float sx, sy, sz;
	if (x == 0) sx = 0; else sx = newSize[0]/x;
	if (y == 0) sy = 0; else sy = newSize[1]/y;
	if (z == 0) sz = 0; else sz = newSize[2]/z;
	sc->scaleFactor.setValue(sx, sy, sz);
}

/***********************************************************************************************
Function  * void sizeSensorCB(void *userData, SoSensor *sensor)
Purpose   * React to changes to the size field.
Arguments * userData... this
					* sensor... sensor, which invoked this function
Return    * -
Notes     * static.
***********************************************************************************************/
void SoSizeShapeKit::sizeSensorCB(void *userData, SoSensor *) {
	SoSizeShapeKit *self = (SoSizeShapeKit *)userData;
	SbVec3f newSize = self->size.getValue();
	self->adjustScale(newSize);
}

/***********************************************************************************************
Function  * SbBool setPart(const SbName& partName, SoNode *newPart)
Purpose   * Overrides standard setPart-method. Since this function is mainly (only) used to set
					* a new geometry, the scale node is adapted in a way, that the geometry fits the actual
					* size.
Arguments * partName... name of part
					* newPart.. pointer to new part
Return    * see SoBaseKit::setPart()
Notes     * -
***********************************************************************************************/
SbBool SoSizeShapeKit::setPart(const SbName &partName, SoNode *newPart) {
	SbBool rc = SoBaseKit::setPart(partName, newPart);
	const SbVec3f newSize = size.getValue();
	adjustScale(newSize);
	return rc;
}

/***********************************************************************************************
Function  * ~SoSizeShapeKit()
Purpose   * Destructor. Delete sensor.
Arguments * -
Return    * -
Notes     * -
***********************************************************************************************/
SoSizeShapeKit::~SoSizeShapeKit() {
	if (sizeSensor)
		delete sizeSensor;
}
