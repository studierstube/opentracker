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
/** class definition for SoFitInKit
  *
  * base class for scene objects the user can interact with
  *
  * @author  Thomas Psik
  *
  * $Id: SoFitInKit.h 3552 2004-07-26 13:58:41Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SO_FIT_IN_KIT_
#define _SO_FIT_IN_KIT_

#include <Inventor/SbViewportRegion.h>

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <stbapi/interaction/SoLayoutKit.h>

#include <stbapi/stbapi.h>
#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
   This kit will scale and move any geometry so that it fits within the specified boundary box. Width, height and depth can be specifed, 
   the center of the box will be placed at 0/0/0. If keepAspect is set to FALSE the geometry will fill the box, destroying the aspect ratio 
   of object else the object will be scaled so that it fits within the box, but generally does not fill the box. 
   
   The rayPickGeometry flag can be set to TRUE to apply the rayPickAction to the geometry instead of returning this kit as the hit object.
 
   File Format / defaults:

@verbatim

SoFitInKit {

	SoSFBool	keepAspect				 TRUE
	SoSFBool	rayPickGeometry			 FALSE
  
	# from SoLayoutKit
  	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0
	
	  }
  
@endverbatim

Examples


 * @ingroup widgets
 */

class SoRayPickAction;
class STBAPI_API SoFitInKit : public SoLayoutKit {

	SO_KIT_HEADER(SoFitInKit);
	

	/**
	 * SoTranslation - used to move the geometry (to place it in the middle of the box)
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(translation);


public:

	/**
	* output extracted translation (relative to center of geometry)
	* transforming an object with this will place it in the middle of the geometry
	*/
	SoSFVec3f translationVec;

	/**
	* output extracted scale 
	* transforming an object with this will size it according to the object
	*/
	SoSFVec3f scaleVec;

	/**
	 * flag if uniform scale should be performed
	 */
	SoSFBool keepAspect;


	/**
	 * flag if the rayPickAction should be applied to the geometry or return this kit as object
	 */
	SoSFBool rayPickGeometry;

	/*
	 * constructor
	 */
	SoFitInKit();

	/**
	 * destructor
	 */
	virtual ~SoFitInKit();

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);


SoINTERNAL public:

	static void initClass();

protected:
	SbXfBox3f storedBox;

	/** sensor to react on changes of the geometry
	*/
	SoFieldSensor geometrySensor;
	SoNodeSensor geometryNodeSensor;
	static void geometryChangedCB(void *, SoSensor*); ///<

	virtual void resizeCB();						///< each sub class should behave correctly on dimension changes

	/** function that evaluates the bounding box of the geometry and will adjust the
	    transformations to fit the geometry within the specified box
	*/
	void adaptToChanges(); 

	SoGetBoundingBoxAction bba;
	static SbViewportRegion vp;

private:


};

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#endif //_SO_FIT_IN_KIT_

