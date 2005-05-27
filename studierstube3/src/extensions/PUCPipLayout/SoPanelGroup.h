/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** Header file for API definitions of SoPanelGroup
  *
  * @author Valerie Maquil
  *
  * $Id: SoPanelGroup.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SoPanelGroup_H_
#define _SoPanelGroup_H_

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/interaction/SoLayoutKit.h>
#include <stbapi/misc/SoAbortGroup.h>
#include <stbapi/util/ivio.h>
#include <stbapi/widgets/SoLabelListBox.h>
#include "SoWidgetLayoutGroup.h"
#include "SoPanel.h"


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stdlib.h>
#include <string>
#include <iostream>

/**
 * implementation of a panel structure. A panel structure consists of an 
 * SoLabelListBox specified in 'titles' and a certain number of 
 * SoWidgetLayoutGroup's specfied in 'sheets'. The list box is placed at the 
 * top or at the right side of the pip and the remaining space is reserved 
 * for the different sheets. Each button of the list box is coupled with the 
 * panel sheet corresponding to the order in which they have been defined.
 * 
 * To definde a panel structure, only the SoPanelGroup is needed where the 
 * 'titles' and 'sheets' have to be specified. The SoPanel is an intermediary 
 * class needed to commute the real geometry of this structure. The sheets of 
 * the SoPanelGroup are then transferred to the SoPanel, which adds them as 
 * children of a SoSwitch node.
 * 
 * @ingroup puc
 * @see SoPanel
 *
   CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>hiddenGeometry </td><td>SoAbortGroup</td><td>SoAbortGroup</td><td>YES</td></tr>
  <tr> <td>elements </td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>
  <tr> <td>pipLayout</td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>	
  <tr> <td>titles </td><td>SoLabelListBox</td><td>SoLabelListBox</td><td>NO</td></tr>
  <tr> <td>sheets</td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>	
  </table>


  Extra Information for List Parts from Above Table

  <table>
	<tr> <td>Part Name</td><td>Container Type</td><td>Possible Types</td></tr>
	<tr> <td>elements</td><td>SoGroup</td><td>SoBehaviorKit</td></tr>
	<tr> <td>pipLayout</td><td>SoSeparator</td><td>SoSeparator</td></tr>
	<tr> <td>sheets</td><td>SoGroup</td><td>SoBehaviorKit</td></tr>

   </table>

  File Format / defaults:

@verbatim
SoPanelGroup {

	SoSFEnum    navigationPlacing		 LEFT	
 	SoSFInt32 panelIndex			     0
	SoSFInt32 panelIndexIn	             0

    # from SoBehaviorKit
  	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0
  }
  @endverbatim
 */


class PUC_PIP_LAYOUT_API SoPanelGroup : public SoWidgetLayoutGroup 
{
    SO_KIT_HEADER(SoPanelGroup);
	
	///contains the labels of the different panel sheets
    SO_KIT_CATALOG_ENTRY_HEADER(titles);
	///contains the different panel sheets a SoWidgetLayoutGroup
    SO_KIT_CATALOG_ENTRY_HEADER(sheets);


public:
	/** Inventor class initialization */
    static void initClass();

	enum NavigationPlacing
	{
		NONE = 0,		/// ignore navigation
		TOP = 1,		/// navigation will be placed above the group
		LEFT = 2,		/// navigation will be placed left of the group 
		RIGHT = 3,      /// navigation will be placed right of the group 
		BOTTOM = 4,     /// navigation will be placed below of the group 
	} ;
	
	///the constructor
    SoPanelGroup();
	
	///the destructor
    virtual ~SoPanelGroup();
	
	///defines the index of the current active panel (field for connections out)
	SoSFInt32 panelIndex;
	///defines the index of the current active panel (field for connections in/out)
	SoSFInt32 panelIndexIn;
	///specifies where the list box should be placed. Currently only TOP and LEFT are implemented.
    SoSFEnum navigationPlacing;
	///indicates the size of the list box in percent [0;1]
	SoSFFloat titlesToSheetsRatio;
	///indicates the number of buttons of the list box that should be visible
	SoSFInt32 numOfButtons;

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways); ///< sets up the connection of the field sensor
	
	virtual void doStyleAction(SoAction *action);

protected:

	///field sensor attached to the panel index
	SoFieldSensor panelIndexSensor;
	///method that is called when panelIndex changes. Forwards its value to its underlying SoPanel.
	static void panelIndexChangedCB( void * data, SoSensor * sensor );

	///field sensor attached to the sheets node
	SoNodeSensor sheetsSensor;
	///method that is called when sheets changes. 
	static void sheetsChangedCB( void * data, SoSensor * sensor );
	// does the actual layout. Is called by sheetsChangedCB()
	void updateLayout();

	///field sensor attached to panelIndexIn
	SoFieldSensor panelIndexInSensor;
	///method that is called when panelIndexIn changes. In this case, its value is assigned to panelIndex.
	static void panelIndexInChangedCB( void * data, SoSensor * sensor );

	///contains the underlying SoPanel
	SoPanel *layoutPanel;

	virtual void resizeCB();	///< each sub class should behave correctly on dimension changes



};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif