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

/** class definition for SoLabelObjectKit
  *
  * class to add geometry to a defined geometry, mostly used to label geometry.
  *
  * @author  Thomas Psik
  *
  * $Id: SoLabelObjectKit.h 3505 2004-07-20 11:39:36Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_LABELOBJECT_KIT_
#define _SO_LABELOBJECT_KIT_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include <Inventor/nodekits/SoWrapperKit.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


/**
 * Represents a label (text) as part of an object. 
 * 
 *
 * @ingroup widgets
 */

class STBAPI_API SoLabelObjectKit : public SoWrapperKit {

	SO_KIT_HEADER(SoLabelObjectKit);	

public:

	/// node ptr to object that should be labeled
	SoSFNode      geometry;

	/// enum of placements of the label
	enum LabelPlacing
	{
		DEFAULT = -1,			  ///< use default
		POS_X = 0,		  ///< label will be placed in the positive X axis of the geometry
		NEG_X  = 1,		  ///< label will be placed in the negative X axis of the geometry
		POS_Y = 2,		  ///< label will be placed in the positive Y axis of the geometry
		NEG_Y  = 3,		  ///< label will be placed in the negative Y axis of the geometry
		POS_Z = 4,		  ///< label will be placed in the positive Z axis of the geometry
		NEG_Z  = 5,		  ///< label will be placed in the negative Z axis of the geometry
		NONE = 6,		  ///< label will be placed in the (bounding box) center of the geometry
	};

	/// specify position of the label relative to the object
	SoSFEnum      placement;	 
	
	/// offset of the label, in percent of the geometry size eg .5 means half the size of the geometry
	SoSFVec3f     offset;

	/// text if used as a text label
    SoMFString    text;

	/// flag wether the label should be resized to the object
	SoSFBool     resizeToObject;
	
	/// flag wether the scale should be uniform 
	SoSFBool     uniformScale;

	/// flag wether the label should react on changes of the geometry
	SoSFBool     dynamicUpdate;
	/**
	 * constructor
	 */
	SoLabelObjectKit();

	/**
	 * destructor
	 */
	virtual ~SoLabelObjectKit();

	///
	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

	///
	void resizeToObjectCB(void *data, SoSensor *senor);

	///
	void adaptToTemplate(SoLabelObjectKit *labelTempl);

SoINTERNAL public:
	
	///
	static void initClass();


protected:
	/** sensor to react on changes of the geometry
	*/
	SoFieldSensor geometrySensor;
//	SoNodeSensor geometryNodeSensor;
	static void geometryChangedCB(void *, SoSensor*); ///<

	SoFieldSensor offsetSensor;
	SoFieldSensor placementSensor;
	SoFieldSensor resizeToObjectSensor;
	SoFieldSensor uniformScaleSensor;
	static void paramChangedCB(void *, SoSensor*); ///<
	
	void adaptToGeometry(); ///< change the label according to geometry and parameters

	SoGetBoundingBoxAction bba;
	static SbViewportRegion vp;
};

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#endif //_SO_LABELOBJECT_KIT_

