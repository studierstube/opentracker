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
//  CONTENT:    SoSizeShapeKit - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    mk      Markus Krutz
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:36  gh      last modification
// ===========================================================================
#ifndef __SOSIZESHAPEKIT_H__
#define __SOSIZESHAPEKIT_H__

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/stbapi.h>
#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class STBAPI_API SoSizeShapeKit : public SoBaseKit {
	SO_KIT_HEADER(SoSizeShapeKit);
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(scale);
	SO_KIT_CATALOG_ENTRY_HEADER(geomSeparator);
public:
	SoSizeShapeKit();
	SoSFVec3f size;

	static void initClass();
	virtual SbBool setPart(const SbName &partName, SoNode *newPart);
protected:
	~SoSizeShapeKit();
	SoFieldSensor *sizeSensor;
private:
	static void sizeSensorCB(void *, SoSensor *);
	void adjustScale(const SbVec3f& newSize);
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
