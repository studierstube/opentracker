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

#ifndef _SO_WIDGET_STYLE
#define _SO_WIDGET_STYLE

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
 * The SoWidgetStyleGroup class implements a typical 2D slider widget. It allows to set 
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

  depends on: Widgets-lib

@verbatim

# the fist element of styles *must* be a derived from SoBehaviourKit, as the type of the widget is determined
# from this entry.


Styles:

SoStyleGroup {
    styleName "onOff"

	styles [ SoToggleButton { } ]
}


SoStyleGroup {
    styleName "exit"
	styles [  SoPushButton { } ]
}

# first define style for class itself in the first element (eg. SoTextListBox)
# then define SoStyleGroups with templates for the sub elements (eg. SoIncrementalSlider, SoToggleButton).
  
SoStyleGroup {
    styleName "fileList"
	
    styles
    [
	    SoTextListBox { }
	
	    SoStyleGroup { # for SoToggleButton(s) in SoTextListBox
			styles [
				SoToggleButton { } # 1st from 3
				SoToggleButton { } # 2nd from 3
				SoToggleButton { } # 3nd from 3
            ]
        }

        SoStyleGroup { # for SoIncrementalSlider in SoTextListBox

			styles [
			   SoIncrementalSlider { } 
		       SoStyleGroup { # up/down buttons in SoIncrementalSlider 
				   styles [
					 SoPushButton { } # up-button
					 SoPushButton { } # down-button
				   ]
			   }
			   SoStyleGroup {
			       styles [
					 SoSimpleSlider {} # slider for SoIncrementalSlider
				   ]
			   }
			]
         }
    ]
}

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
class WIDGETSTYLE_API SoWidgetStyleGroup : public SoNode
{
	SO_NODE_HEADER(SoWidgetStyleGroup);

	//the rest of the geometry is added here: the track etc.
public:

	static void initClass();

	/** default constructor 
	*/
	SoWidgetStyleGroup();

	/** copy constructor 
	*/
	SoWidgetStyleGroup(SoWidgetStyleGroup *in);
	~SoWidgetStyleGroup();
	
	// SoSFString applyToClass;
	// SoSFNode parentStyle;

	// fields
	SoSFString styleName;
	SoMFNode styles;

	void applyStyle (SoBehaviorKit *widget);

	SoWidgetStyleGroup *getDirectStyleForClass(SoBehaviorKit *widget);
	SoWidgetStyleGroup *getStyleForClass(SbName className);

	SoNode *getTemplateForIndex(int index, SoType &type);

	SoNode *getTemplateForClass(SoNode *);

	void setStyleForClass(SbName className);
	void addStyle(SoWidgetStyleGroup *style);

	void setStyleInGroup(SoWidgetStyleGroup *style);
	SoWidgetStyleGroup *getTemplateGroupForType(SoType &type);

protected:	
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

};



#endif
