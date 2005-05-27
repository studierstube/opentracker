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
/** header file for SoStatusBox class.
  *
  * @author Valérie Maquil
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_STATUSBOX_
#define _SO_STATUSBOX_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFTrigger.h>

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTexture2.h>
#include <stbapi/widgets/SoLabelKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


/**
 * Represents a status box where a description of the currently highlighted widget is displayed. 
 * It mainly consists of a plane and a text which describes the widgets.
 * You may specify the appearance of the plane and the label when a widget is highlighted ("onPlane"
 * and "onLabel") and when no widget is highlighted ("offPlane" and "offLabel"). The text for "onLabel"
 * is taken from the field "statusBoxText" of the currently active widget.
 * The SoStatusBox works only in combination with the SoStatusBoxLayout which makes the connections
 * between the widgets and the notifications of the status box.
 
File Format / defaults:

@verbatim

SoStatusBox {
    SoMFString  text				""		# text that is currently displayed
	SoSFBool    show				FALSE	# if true a message is displayed 
}
  
@endverbatim

CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>onOffSwitch </td><td>SoSwitch</td><td>SoSwitch</td><td>NO</td></tr>
  <tr> <td>onGeometry </td><td>SoSeparator</td><td>SoSeparator</td><td>NO</td></tr>
  <tr> <td>offGeometry </td><td>SoSeparator</td><td>SoSeparator</td><td>NO</td></tr>
  <tr> <td>onPlane </td><td>SoShapeKit</td><td>SoShapeKit</td><td>NO</td></tr>
  <tr> <td>offPlane </td><td>SoShapeKit</td><td>SoShapeKit</td><td>NO</td></tr>
  <tr> <td>onLabel </td><td>SoLabelKit</td><td>SoLabelKit</td><td>NO</td></tr>
  <tr> <td>offLabel </td><td>SoLabelKit</td><td>SoLabelKit</td><td>NO</td></tr>	
  </table>


Example

 SoStatusBox { 
	width 0.08 depth 0.04 height 0.01

	onLabel SoLabelKit { 
		appearance SoAppearanceKit { 
			font SoFont { name "Trebuchet MS" size 0.9 } 
			material Material { diffuseColor 0 0 0 } }}

	onPlane DEF PLANE SoShapeKit { 
		coordinate3 SoCoordinate3 { point [0.5 0 0.5  ,0.5  0 -0.5, -0.5  0 -0.5, -0.5 0 0.5 ] }
		shape FaceSet {}
		appearance SoAppearanceKit {material Material {diffuseColor .7 .7 .8 transparency 0 }}}
	
	offPlane USE PLANE
	}


 * @ingroup widgets
 */


class STBAPI_API SoStatusBox : public SoLayoutKit {

	SO_KIT_HEADER(SoStatusBox);
	
    /**
	 * root separator
	 */
  	SO_KIT_CATALOG_ENTRY_HEADER(onOffSwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(onGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(offGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(onPlane);
	SO_KIT_CATALOG_ENTRY_HEADER(offPlane);
	SO_KIT_CATALOG_ENTRY_HEADER(onLabel);
	SO_KIT_CATALOG_ENTRY_HEADER(offLabel);
	SO_KIT_CATALOG_ENTRY_HEADER(onContents);
	SO_KIT_CATALOG_ENTRY_HEADER(offContents);


public:

    SoMFString    text;
	
	SoSFBool      show;

	SoSFBool      hideLabel;


	/*
	 * constructor
	 */
	SoStatusBox();

	/**
	 * destructor
	 */
	virtual ~SoStatusBox();

	void updateVisibility();

	void setLabel(SoMFString *sbText);


SoINTERNAL public:

	static void initClass();
protected:

	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways );

	SoFieldSensor showSensor;

	static void showCB(void *data, SoSensor *sensor);
	
	SoFieldSensor hideLabelSensor;

	static void hideLabelCB(void *data, SoSensor *sensor);
};

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#endif //_SO_STATUSBOX_
