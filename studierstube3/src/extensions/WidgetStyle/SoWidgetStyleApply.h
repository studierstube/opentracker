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
/** class definition of SoWidgetStyleGroup
  *
  * @author  Thomas Psik
  * $Id: SoWidgetStyleGroup.h 3596 2004-08-10 13:34:54Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_SoWidgetStyleApply_H
#define _SO_SoWidgetStyleApply_H

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoNode.h>

#include <Inventor/nodes/SoSubNode.h>

#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFNode.h>

#include <Inventor/sensors/SoFieldSensor.h>

#include "WidgetStyle.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The SoWidgetStyleApply class implements some helper functions to apply a style to a specific widget.
 *

@verbatim

 @endverbatim
 *
 * @ingroup widgetstyle
 */

 /*
class SoButtonKit;
class SoToggleButton;
class SoPushButton;
class SoRadioButton;
class SoSimpleSlider;


class SoButtonStyle;
class SoToggleButtonStyle;
class SoSliderStyle;
class SoLabelStyle;
class SoIncrementalSliderStyle;
class SoToggleButtonBoxStyle;
*/
class SoBehaviorKit;
class SoLabelKit;
class SoTooltipKit;
class SoWidgetStyleGroup;

class WIDGETSTYLE_API SoWidgetStyleApply {
public:
	static void applyStyleToWidget(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index);
protected:
	// helper
	static SoNode *getStyleNodeForIndex(SoWidgetStyleGroup *style, int index, SoType &type);

	static void applyStyleToLabelKit(SoLabelKit *label, SoLabelKit *labelTempl);
	static void applyStyleToTooltipKit(SoTooltipKit *tooltip, SoTooltipKit *tooltipTempl);

	// special functions for all widget types
	static void applyStyleToBehaviorKit(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index);
	static void applyStyleToButtonKit(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index);
	static void applyStyleToSlider(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index);
	static void applyStyleToSimpleSlider(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index);
	static void applyStyleToIncrementalSlider(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index);
	static void applyStyleToLabelListBox(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index);

};

#endif //_SO_SoWidgetStyleApply_H
