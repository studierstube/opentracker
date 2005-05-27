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
/** class definition for SoLayoutKit
  *
  * base class for scene objects the user can interact with
  *
  * @author  Thomas Psik
  *
  * $Id: SoLayoutKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_LAYOUT_KIT_
#define _SO_LAYOUT_KIT_

#include <Inventor/fields/SoSFFloat.h>

#include <Inventor/engines/SoCompose.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
  Used to have width, height and depth available for all geometry that should be layouted.

CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
 </table>

   File Format / defaults:

@verbatim

SoLayoutKit {
  	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0
  }
  
@endverbatim

Examples

  DO NOT INSTATIATE THIS CLASS !!

 * @ingroup widgets
 */
class STBAPI_API SoLayoutKit : public SoBaseKit {

	SO_KIT_HEADER(SoLayoutKit);
	
    /**
	 * root separator
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(base);

	/**
	 * SoScale - used to scale the dragger part
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(scale);

	/**
	 * SoSeparator - place geometry inside this separator
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);

public:

	/**
	 * the widget's width
	 */
	SoSFFloat width;

	/**
	 * the widget's height
	 */
   	SoSFFloat height;

	/**
	 * the widget's depth
	 */
	SoSFFloat depth;

	/*
	 * constructor
	 */
	SoLayoutKit();

	/**
	 * destructor
	 */
	virtual ~SoLayoutKit();

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

	virtual void doStyleAction(SoAction *action);

SoINTERNAL public:

	static void initClass();

protected:
	
	SoFieldSensor heightSensor;						///< used to track changes in dimensions (calls dimensionsChangedCB)
	SoFieldSensor depthSensor;						///< used to track changes in dimensions (calls dimensionsChangedCB)
	SoFieldSensor widthSensor;						///< used to track changes in dimensions (calls dimensionsChangedCB)

	static void dimensionsChangedCB(void *, SoSensor*); ///< calls resizeWidgetCB
	virtual void resizeCB();						///< each sub class should behave correctly on dimension changes

private:


};

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#endif //_SO_LAYOUT_KIT_

