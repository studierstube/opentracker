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
//  CONTENT:    SoBase3DDial - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw              Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:28  gh      last modification
// ===========================================================================
#ifndef __SOBASE3DDIAL_H__
#define __SOBASE3DDIAL_H__

#include <Inventor/SbPList.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>

#include <Inventor/engines/SoCalculator.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFString.h>

#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#include <stbapi/event3d/Base3D.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoHandle3DEventAction;
class SoBase3DDial;

typedef void SoBase3DDialCB(void *userData, SoBase3DDial *button);

////////////////////////////////////////////////////////////////////
//
//  Class: SoBase3DDial
//
//  Superclass for all Dials. It builds up the geometry and
//  manages highlighting and selection
//
////////////////////////////////////////////////////////////////////

/**
 * SoBase3DDial. Documented at
 * http://www.studierstube.org/stbapi/SoBase3DDial.html
 * @ingroup oldwidgets
 * @ingroup deprecated
 */

class STBAPI_API SoBase3DDial : public SoBaseKit, public Base3D {

	SO_KIT_HEADER(SoBase3DDial);

	BASE3D_HEADER;

	SO_KIT_CATALOG_ENTRY_HEADER(kitSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(additionalGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(rotation);
	SO_KIT_CATALOG_ENTRY_HEADER(colorSwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(geoSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(surroundingDrawStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(surrSeparator);

  public:
	enum CBPolicy
	{
		ON_RELEASE=0, ALWAYS
	};

		// member-variables
	SoSFColor dialColor;
	SoSFColor positionColor;
	SoSFColor highlightColor;
	SoSFColor dragColor;

	SoSFFloat startAngle;
	SoSFFloat endAngle;

	SoSFFloat radius;
	SoSFFloat height;

	SoSFString nodeId;
	SoSFEnum   callback;

	SoSFInt32	highlighted;
	SoSFInt32	triggerButton;

	// member-methods
	static void initClass();	
	void		initFields();

				SoBase3DDial();				

	virtual void	setPosition(float newValue);
	virtual float	getPosition();
	virtual void    setPositionUpdate(float newValue);
    virtual void    updateFromNetwork(float newValue);
	
	void		setGeometry(SoNode *geometry, SoSensorCB *callback);
	SoNode	   *getGeometry();
	void		setSurrounding(SoNode *geometry);
	SoNode	   *getSurrounding();
	void		setAdditionalGeometry(SoNode *geometry);
	SoNode	   *getAdditionalGeometry();

	void		disableHighlighting(SbBool disHigh);
	void		setHighlighted(SbBool highlightB);
	SbBool		isHighlighted();

	virtual SbBool	isInterested(SoHandle3DEventAction *h3a);
	virtual void	handle3DEvent(SoHandle3DEventAction *h3a);

	virtual void getBoundingBox(SoGetBoundingBoxAction *action);

  protected:
		// member-variables
	SoFieldSensor *heightSensor, *radiusSensor;
    SoAlarmSensor *snapBackSensor;
		 
	SoSFFloat	position;

	SbPList    *funcList;		// list of callback-function *

	SoSFBool	disableHL;

	SbBool		dragging;
	int		dragStation;
	int 		highlightCount;

	int			inside;
	float		oldAngle;
	float		oldDistance;
	
	static int  baseInitClassAt;

		// member-methods
	static void setupDialMesh(void *data, SoSensor *);
	virtual	   ~SoBase3DDial();	

  private:
	int firstTime;
		// member-methods
	void		setupDialGeometry(float height, float radius);

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

