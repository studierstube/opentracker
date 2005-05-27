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
/** Header file for API definitions of SoPanel
  *
  * @author Valerie Maquil
  *
  * $Id: SoPanel.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SoPanel_H_
#define _SoPanel_H_

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/interaction/SoLayoutKit.h>
#include <stbapi/misc/SoAbortGroup.h>
#include <stbapi/util/ivio.h>
#include <stbapi/widgets/SoLabelListBox.h>
#include "SoWidgetLayoutGroup.h"


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stdlib.h>
#include <string>
#include <iostream>


/**
 * intermediary class of an panel structure representing the different sheets 
 * of the panel structure. This class distinguishes itself from the 
 * SoWidgetLayoutGroup by the way it places its elements. The elements of the 
 * SoPanel are created as children of a SoSwitch node where only one element 
 * at the time will be rendered. The index of the current activ element is 
 * given in the field panelIndex. 
 * All the elements are given the same size: the area of the pip sheet assigned to this SoPanel.
 * 
 * @ingroup puc
 * @see SoPanelGroup
 *
   CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>hiddenGeometry </td><td>SoAbortGroup</td><td>SoAbortGroup</td><td>YES</td></tr>
  <tr> <td>elements </td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>
  <tr> <td>pipLayout</td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>	
  <tr> <td>panelLayout </td><td>SoSwitch</td><td>SoSwitch</td><td>YES</td></tr>
  </table>


  Extra Information for List Parts from Above Table

  <table>
	<tr> <td>Part Name</td><td>Container Type</td><td>Possible Types</td></tr>
	<tr> <td>elements</td><td>SoGroup</td><td>SoBehaviorKit</td></tr>
	<tr> <td>pipLayout</td><td>SoSeparator</td><td>SoSeparator</td></tr>
   </table>

  File Format / defaults:

@verbatim
SoPanelGroup {

	SoSFInt32 panelIndex			     0
	SoSFInt32 panelIndexIn	             0
 
    # from SoBehaviorKit
  	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0
  }
  @endverbatim
 */


class PUC_PIP_LAYOUT_API SoPanel : public SoWidgetLayoutGroup 
{
    SO_KIT_HEADER(SoPanel);
	
	///contains the different sheets of the panel structure
	SO_KIT_CATALOG_ENTRY_HEADER(panelLayout);



public:
	/** Inventor class initialization */
    static void initClass();
	
	///the constructor
    SoPanel();
	
	///the destructor
    virtual ~SoPanel();
	
	///defines the index of the current active panel (field for connections out)
	SoSFInt32 panelIndex;
	///defines the index of the current active panel (field for connections in/out)
	SoSFInt32 panelIndexIn;

	/**calculates the new parameters for all the elements and builds the 
	 * layout: position and size are just copied from the SoPanel.
	 */
	void buildPipLayout(); 

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways); ///< sets up the connection of the field sensors

	///sets the panelIndex to the specified value
	void setPanelIndexIn(int index) { panelIndexIn.setValue(index); }

protected:

	///field sensor attached to the panel index
	SoFieldSensor panelIndexSensor;
	/**method that is called when panelIndex changes. It changes the 
	 * current active child of the panelLayout part.. */
	static void panelIndexChangedCB( void * data, SoSensor * sensor );

	///field sensor attached to panelIndexIn
	SoFieldSensor panelIndexInSensor;
	///method that is called when panelIndexIn changes. In this case, its value is assigned to panelIndex.
	static void panelIndexInChangedCB( void * data, SoSensor * sensor );

	///pointer to the panelLayout
	SoSwitch *switchLayout;	

	virtual void resizeCB(); ///< each sub class should behave correctly on dimension changes

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif