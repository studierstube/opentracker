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
/** class definition for SoShapePlaneKit
  *
  * base class for scene objects the user can interact with
  *
  * @author  Thomas Psik
  *
  * $Id: SoShapePlaneKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_TOOLTIP_KIT_
#define _SO_TOOLTIP_KIT_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#include <Inventor/nodekits/SoShapeKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


/**
 * Represents a tooltip plane with text. This geometry will be displayed if a user stays within a widget for a period of time.
 * You can add a tooltip by specifying it in the widget. This is done by setting the SoBeaviorKit::tooltip field. The tooltip geometry
 * will be switched off when the user leaves the button.
 *
 * The SoShapePlaneKit uses a SoLabelKit to display the text on the plane.
 *
 * @see SoBehaviorKit, SoLabelKit
 *
 * @todo clean up. Plane should be a SoShapeKit (or SoAppearanceKit) 
 
File Format / defaults:

@verbatim

SoShapePlaneKit {
    SoSFString  text				""		# text on the tooltip plane
	SoSFBool    show				FALSE	# if true the tooltip is displayed 
	SoSFFloat   timeOut				1.2     # seconds the user must stay within the widgets hyseresis box to display the tooltip
}
  
@endverbatim

Examples

SoShapePlaneKit { text "This is a tooltip" show TRUE}			# shows a tooltip 

# shows red tooltip text on a black plane
SoShapePlaneKit { 
	text "This is a tooltip" 
	show TRUE
	material Material {diffuseColor 0 0 0 }
	label.material Material {diffuseColor 1 0 0 }
}
			
SoPushButton 
  {
	tooltip SoShapePlaneKit { text "This is a push button" }  # tooltip will be activated when user is within the pushbutton
  }



 * @ingroup widgets
 */
class STBAPI_API SoShapePlaneKit : public SoShapeKit {

	SO_KIT_HEADER(SoShapePlaneKit);
	
    /**
	 * root separator
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(base);
	SO_KIT_CATALOG_ENTRY_HEADER(nobbox);
  	SO_KIT_CATALOG_ENTRY_HEADER(onOffSwitch);
  	SO_KIT_CATALOG_ENTRY_HEADER(geometry);
	SO_KIT_CATALOG_ENTRY_HEADER(transform);
	SO_KIT_CATALOG_ENTRY_HEADER(planeGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(scale);
	SO_KIT_CATALOG_ENTRY_HEADER(material);
	SO_KIT_CATALOG_ENTRY_HEADER(plane);
	SO_KIT_CATALOG_ENTRY_HEADER(label);

public:

    SoSFString    text;
	
	SoSFBool      show;

	SoSFFloat     timeOut;

	/*
	 * constructor
	 */
	SoShapePlaneKit();

	/**
	 * destructor
	 */
	virtual ~SoShapePlaneKit();

	void updateVisibility();

	void startTimeIn();
	void stopTimeIn();

SoINTERNAL public:

	static void initClass();
protected:
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
	
	SoFieldSensor showSensor;
	SoAlarmSensor timeInSensor;

	static void showCB(void *data, SoSensor *sensor);
	static void timeOutCB(void *data, SoSensor *sensor);
};

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#endif //_SO_TOOLTIP_KIT_

