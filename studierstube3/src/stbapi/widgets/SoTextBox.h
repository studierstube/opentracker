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
/** class definition of SoTextBox
  *
  * class to be used when the application developer wants to add text box 
  * as one type of widget. In STB it is only possible to display text, but not to change it. Possible intergration
  * with SoAlphabetKit should enable a SoEditBox, which would also allow changing of text .
  *
  * @author Thomas Psik
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SO_TEXTBOX_
#define _SO_TEXTBOX_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>

#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/interaction/SoBehaviorKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


/**
 * SoTextBox is the class to be used, when
 * the application developer wants to add
 * button-like behavior to one of the scene objects
 *
 * The textboxes's size can be determined with the width,
 * height and depth field. These fields expect the
 * supplied geometry to be in a unit cube centered
 * at (0,0,0), else the proportions do not match.

  CATALOG PARTS

  All Parts (from SoBehaviorKit)
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>label </td><td>SoLabelKit</td><td>SoLabelKit</td><td>YES</td></tr>
  <tr> <td>tooltip </td><td>SoTooltipKit</td><td>SoTooltipKit</td><td>YES</td></tr>
 </table>

File Format / defaults:

@verbatim

SoTextBox {

  	SoSFBool	highlighted				FALSE	# highlight state (pen is within the text) ? useful ??
	SoMFString  text					""		# text that is beeing displayed

	# from SoBehaviorKit

	SoSFFloat	hysteresisBoxScale		 1.0
	SoSFFloat   hysteresisBoxScaleHeigth 1.0
	SoSFBool	enable					TRUE	# if true the widget will be enabled else it is disabled (should not react on interaction)
	SoSFBool	applyWidgetStyle		TRUE    # if true a widget style will replace the geometry and parameters of this widget
												# else the widget will not be changed
  
	# from SoLayoutKit

  	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0
  }
   
@endverbatim

Examples

	 
 * @ingroup widgets
 */

class STBAPI_API SoTextBox : public SoBehaviorKit
{
	SO_BEHAVIOR_HEADER(SoTextBox);

	/// SoRotation  - use to rotate the text into the plane of the widgets
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(rotation);

	/// SoFont  - add general font here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(font);

	/// SoSeparator - add normal on geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(normalGeometry);

	/// SoSeparator - add highlighted on geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(highlightedGeometry);

	/// SoSeparator - add disabled on geometry here
	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(disabledGeometry);

public:

	/**
	 * 'highlighted' and describes the textbox's state
	 */
	SoSFBool highlighted;
	
	/**
	*	String text that has to be displayed
	*/
	SoMFString text;

	/**
	 * If the user enters the buttons volume, if becomes highlighted,
	 * Depending on its state before highlighting, it is either
	 * HIGHLIGHTED_ON or HIGHLIGHTED_OFF
	 */
	enum GeometryState
	{
		NORMAL = 0,
		HIGHLIGHTED,
		DISABLED
	};

	/**
	 * the class initialization
	 */
	static void initClass();

	/**
	 * the constructor
	 */
	SoTextBox();

	/**
	 * the destructor
	 */
	~SoTextBox();

	/**
	 * our handle event method
	 */
	void handle3DEvent(SoHandle3DEventAction* h3a);

	/**
	*  handle change in enable field
	**/
	virtual void adaptToEnable();

	/**
	 * Adapt the textbox's geometry to the state
	 * described by the 'highlighted' field
	 */
	void adaptGeometryToState();

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

protected:

  SoMFNode actionEnterGestures;
  SoMFNode actionExitGestures;

  SoFieldSensor hlSensor;
  static void stateChangedCB( void * userdata, SoSensor * sensor );

  virtual void resizeWidgetCB();
  /**
  * helper function that looks for a SoAsciiText in the Separator and connects it to the text field
  **/
  void connectAsciiTextToValue(SoSeparator *sep);
   
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SO_BUTTON_KIT_

