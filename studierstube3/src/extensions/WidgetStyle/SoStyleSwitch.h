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
/** class definition of SoStyleSwitch
  *
  * @author  Thomas Psik
  * $Id: SoStyleSwitch.h 3596 2004-08-10 13:34:54Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_WIDGET_STYLE_SWITCH
#define _SO_WIDGET_STYLE_SWITCH

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/SoType.h>

#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTrigger.h>

#include <Inventor/sensors/SoFieldSensor.h>

#include "WidgetStyle.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The SoStyleSwitch class implements a typical 2D slider widget. It allows to set 
 * a range of values and select from that by moving the slider knob. All of
 * the widgets geometry can be parametrized with your own scenegraphs.
 *
 * The minValue and maxValue fields set the value range and the value field
 * will output the current value set. The width, height and depth fields
 * from the super class SoDraggerKit set the size of the slider. 
 * The minVector and maxVector define in what range the slider's knob can move.
 * These values are in the same coordinate system as the slider's geometry.
 *
 * The parts staticSeparator, staticGeometry and staticScale allow you to
 * specify how the static body part of the slider should look like. The
 * moving knob can be specified using the parts of the super class.
 *
 * File format :
@verbatim
SoStyleSwitch {

  }
@endverbatim
 *
 * @ingroup widgetstyle
 */

class WIDGETSTYLE_API SoStyleSwitch : public SoBaseKit
{
	SO_KIT_HEADER(SoStyleSwitch);

	SO_KIT_CATALOG_ENTRY_HEADER(stylesGroup);
	
	SO_KIT_CATALOG_ENTRY_HEADER(widgets);

	//the rest of the geometry is added here: the track etc.
public:

	static void initClass();

	SoStyleSwitch();
	SoStyleSwitch(SoStyleSwitch *in);
	~SoStyleSwitch();
	
	// add fields
	SoSFInt32 whichStyle;
	SoSFTrigger updateStyle;

protected:	
	void applyStyleToWidgets();

	SoFieldSensor whichStyleSensor;
   	static void whichStyleChangedCB(void *data, SoSensor *);

	SoFieldSensor updateStyleSensor;
   	static void updateCB(void *data, SoSensor *);

	SoFieldSensor widgetsSensor;
   	static void widgetsChangedCB(void *data, SoSensor *);
	
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

		/**
	 replaces the widget geometry with the elements set in SoWidgetStyle where it has not been overwritten by 
	 anyother method (eg specification in file)
	*/
//	void applyWidgetStyles();

};

#endif
