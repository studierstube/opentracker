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
//  PROJECT:    Studierstube
//  CONTENT:    SoExtCubeKit - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    mk      Markus Krutz
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:36  gh      last modification
// ===========================================================================
#ifndef _SoExtCubeKit_H_
#define _SoExtCubeKit_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/stbapi.h>
#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoExtCubeKit;

/**
 * @ingroup workspace
 */
class STBAPI_API SoExtCubeKit : public SoBaseKit {
	SO_KIT_HEADER(SoExtCubeKit);

	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(shapeHints);
	SO_KIT_CATALOG_ENTRY_HEADER(boxSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(boxMaterial);
	SO_KIT_CATALOG_ENTRY_HEADER(boxCoordinates);
	SO_KIT_CATALOG_ENTRY_HEADER(boxNormals);
	SO_KIT_CATALOG_ENTRY_HEADER(boxNormalBinding);
	SO_KIT_CATALOG_ENTRY_HEADER(boxIndexedFaceSet);
public:
	SoExtCubeKit();
	static void initClass();

	SoSFFloat width;
	SoSFFloat height;
	SoSFFloat depth;
protected:
	virtual void setDefaultOnNonWritingFields();
	~SoExtCubeKit();
private:
	SoFieldSensor *widthSensor, *heightSensor, *depthSensor;
	static void sensorCB(void *, SoSensor *);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
