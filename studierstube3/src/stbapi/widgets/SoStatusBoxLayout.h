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

#ifndef _SO_STATUSBOXLAYOUT_
#define _SO_STATUSBOXLAYOUT_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/SbPList.h>

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


/**
 * Container class used to implement a GUI containing a SoStatusBox. All layout information
 * (widgets and SoStatusBox) needs to be defined in the part "layout". When the user touches
 * one of the widgets, the text found in the field statusBoxText of the corresponding widget
 * is showed on the plane of the SoStatusBox.
 *
 
  CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>base </td><td>SoSeparator</td><td>SoSeparator</td><td>NO</td></tr>
  <tr> <td>layout </td><td>SoSeparator</td><td>SoSeparator</td><td>NO</td></tr>
  <tr> <td>hiddenGeometry </td><td>SoAbortGroup</td><td>SoAbortGroup</td><td>NO</td></tr>
  <tr> <td>statusBox </td><td>SoStatusBox</td><td>SoStatusBox</td><td>NO</td></tr>
  <tr> <td>widgets </td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>	
  </table>


  Extra Information for List Parts from Above Table

  <table>
	<tr> <td>Part Name</td><td>Container Type</td><td>Possible Types</td></tr>
	<tr> <td>widgets</td><td>SoGroup</td><td>SoBehaviorKit</td></tr>
   </table>


 * @ingroup widgets
 */


class STBAPI_API SoStatusBoxLayout : public SoBaseKit {

	SO_KIT_HEADER(SoStatusBoxLayout);
	
    /**
	 * root separator
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(base);
	SO_KIT_CATALOG_ENTRY_HEADER(layout);
	SO_KIT_CATALOG_ENTRY_HEADER(hiddenGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(statusBox);
	SO_KIT_CATALOG_ENTRY_HEADER(widgets);


public:
	/*
	 * constructor
	 */
	SoStatusBoxLayout();

	/**
	 * destructor
	 */
	virtual ~SoStatusBoxLayout();


SoINTERNAL public:

	static void initClass();
protected:

	/** 
	 * sets up the connections of the sensors
	 */
	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways );

	/**
	 * array of field sensors attached to each widget
	 */
	//SoFieldSensor focusSensor[50];
	SbPList   focusSensors;
	/**
	 * node sensor attached to the layout part of the SoStatusBoxLayout
	 */
	SoNodeSensor layoutSensor;

	/**
	 * callback function called when an event occurs inside a widget
	 */
	static void focusCB(void *data, SoSensor *sensor);
	/**
	 * callback function called when layout information is added
	 */
	static void layoutCB(void *data, SoSensor *sensor);
	
	/**
	 * uses a SoSearchAction to look for widgets of type SoBehaviorKit
	 * and a SoStatusBox, to make the connections between them
	 */
	void findWidgets(SoNode *start);
};

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#endif //_SO_STATUSBOX_
