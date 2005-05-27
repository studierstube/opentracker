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
//  CONTENT:    SoBase3DSlider - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw              Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:28  gh      last modification
// ===========================================================================
#ifndef __SOBASE3DSLIDER_H__
#define __SOBASE3DSLIDER_H__

#include <Inventor/SbPList.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <Inventor/engines/SoCalculator.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFString.h>

#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#include <stbapi/event3d/Base3D.h>

#include <set>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoHandle3DEventAction;
class SoBase3DSlider;

////////////////////////////////////////////////////////////////////
//
//  Class: SoBase3DSlider
//
//  This is the Superclass for all Slider-Classes. It builds up
//  the node-kit and initializes the geometry 
//
////////////////////////////////////////////////////////////////////

/**
 * SoBase3DSlider. Documented at
 * http://www.studierstube.org/stbapi/SoBase3DSlider.html
 * @ingroup oldwidgets
 * @ingroup deprecated
 */

class STBAPI_API SoBase3DSlider : public SoBaseKit, public Base3D {

	SO_KIT_HEADER(SoBase3DSlider);

	BASE3D_HEADER;

	SO_KIT_CATALOG_ENTRY_HEADER(kitSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(additionalGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(bodySeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(bodySwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(bodyGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(bodyDrawStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(bodySurrounding);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderTranslation);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderSwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderDrawStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderSurrounding);

  public:
	
	enum CBPolicy
	{
		ON_RELEASE=0, ALWAYS
	};

		// member-variables  
	SoSFColor	bodyColor;		// color of slider-body
	SoSFColor	sliderColor;	// color of slider
	SoSFColor	bodyHLColor;	// highlight-color of slider-body
	SoSFColor	sliderHLColor;	// highlight-color of slider
	SoSFColor	sliderDragColor;// dragging-color of slider
	SoSFVec3f	startPoint;		// start-point of slider
	SoSFVec3f	endPoint;		// end-point of slider


	SoSFFloat	radius;
	SoSFFloat	width;

	SoSFString	nodeId;

	SoSFEnum	callback;

	SoSFInt32	bodyHL;			// 0 for no body-HL, 1 for body-HL		
	SoSFInt32	sliderHL;		// 0 for no slider-HL, 1 for slider-HL
								// 2 for dragging
	SoSFInt32	triggerButton;	// number of pen-button to manipulate

	SoSFFloat	position;		// position of slider (btw. 0 and 1)
								// 0 -> slider is at start-point
								// 1 -> slider is at end-point
	SoSFBool	disableHL;		// TRUE for disabling highlighting
								// FALSE for enabling it

		// member-methods
	static void	initClass();	
	SoBase3DSlider();		

	virtual void setPosition(float newValue);
	virtual void setPositionUpdate(float newValue);
    virtual void        updateFromNetwork(float newValue);

	void		setBodyGeometry(SoNode *newBody, SoSensorCB *callback);
	void		setSliderGeometry(SoNode *newSlider);
	SoNode	   *getBodyGeometry();
	SoNode	   *getSliderGeometry();

	void		setBodySurrounding(SoNode *newBody);
	void		setSliderSurrounding(SoNode *newSlider);
	SoNode     *getBodySurrounding();
	SoNode     *getSliderSurrounding();

	void		setAdditionalGeometry(SoNode *geometry);
	SoNode	   *getAdditionalGeometry();

	void		setSliderHighlighted(SbBool highlightB, int station);
	SbBool		isSliderHighlighted(int station);
	SbBool		isSliderHighlighted();
	void		setBodyHighlighted(SbBool highlightB, int station);
	SbBool		isBodyHighlighted(int station);
	SbBool		isBodyHighlighted();


	void		disableHighlighting(SbBool disHigh);


	virtual float getNormalizedIncrement();

	virtual SbBool isInterested(SoHandle3DEventAction *h3a);
	virtual void handle3DEvent(SoHandle3DEventAction *h3a);

	virtual void getBoundingBox(SoGetBoundingBoxAction *action);

protected:
		// member-variabled 
	SoFieldSensor *widthSensor, *radiusSensor;
	  
	SbPList    *funcList;		// list of callback-function *
								// when slider changes
		
	SbBool		sliderSel;
	SbBool		dragging;		// TRUE when mousebutton is held down
	int			insideBody, insideSlider;
	float		dragDistance;	// distance from position to position
								// of picked-point during dragging
    int dragStation;
	static int  baseInitClassAttempts;
	
    /// to keep track which station
    std::set<int> bodyHLArray;
    /// highlights which part of the slider
    std::set<int> sliderHLArray;
	int firstTime;
    SoAlarmSensor *snapBackSensor;
	// member-functions
	static void	setupSliderMesh(void *data, SoSensor *);
	virtual	   ~SoBase3DSlider();	

  private:
	// member-functions
	void		createSliderGraph();
	void		initFields(float width, float radius, float pos);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
