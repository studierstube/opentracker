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
/** Header file for API definitions of SoWidgetLayoutGroup
  *
  * @author Valerie Maquil
  *
  * $Id: SoWidgetLayoutGroup.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SoWidgetLayoutGroup_H_
#define _SoWidgetLayoutGroup_H_

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFTrigger.h>

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/misc/SoAbortGroup.h>
#include <stbapi/interaction/SoLayoutKit.h>
#include <stbapi/util/ivio.h>
#include "PucPipLayout.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stdlib.h>
#include <string>
#include <iostream>


/**
 * represents a hierarchical structure of widgets. A WidgetLayoutGroup can contain widgets and/or other WidgetLayoutGroups
 * as elements. When created, a WidgetLayoutGroup places all his elements in numOfRows rows and numOfCols columns and calculates
 * width, depth and translations for each of them. These informations are then stored in the part "pipLayout".
 * 
 * @ingroup puc
 *
   CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>hiddenGeometry </td><td>SoAbortGroup</td><td>SoAbortGroup</td><td>YES</td></tr>
  <tr> <td>elements </td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>
  <tr> <td>pipLayout</td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>	
  </table>


  Extra Information for List Parts from Above Table

  <table>
	<tr> <td>Part Name</td><td>Container Type</td><td>Possible Types</td></tr>
	<tr> <td>elements</td><td>SoGroup</td><td>SoLayoutKit</td></tr>
	<tr> <td>pipLayout</td><td>SoSeparator</td><td>SoSeparator</td></tr>
   </table>

  File Format / defaults:

@verbatim
SoWidgetLayoutGroup {
 
  	SoSFFloat	width					 1.0
	SoSFFloat	height					 1.0
	SoSFFloat	depth					 1.0

	SoSFInt32	numOfRows				 -1
	SoSFInt32	numOfCols				 -1
	SoSFFloat   spacingWidth			 0.2 #20 %
	SoSFFloat   spacingWidth			 0.2 #20 %
	SoMFInt32	sizeOfRows				 1	
	SoMFInt32	sizeOfColumns			 1    
												
  }
  @endverbatim
 */

class PUC_PIP_LAYOUT_API SoWidgetLayoutGroup : public SoLayoutKit 
{
    SO_KIT_HEADER(SoWidgetLayoutGroup);
    SO_KIT_CATALOG_ENTRY_HEADER(hiddenGeometry);
	
	///should contain all the widgets and groups which belong to this SoWidgetLayoutGroup
    SO_KIT_CATALOG_ENTRY_HEADER(elements);
	///here the final scenegraph is constructed; should be left empty. 
    SO_KIT_CATALOG_ENTRY_HEADER(pipLayout);


public:
	/** Inventor class initialization */
    static void initClass();
	
	///the constructor
    SoWidgetLayoutGroup();
	
	///the destructor
    virtual ~SoWidgetLayoutGroup();
	
	/** indicates how many rows should be used to generate the layout. When negative, the number of rows
	is calculated by means of the number of children and the number of columns. Default value: -1
	*/
	SoSFInt32 numOfRows;

  	/** indicates how many columns should be used to generate the layout. When negative, the number of columns
	is calculated by means of the number of children and the number of rows. Default value: -1
	*/
    SoSFInt32 numOfCols;
	/** horizontal space which should be left between elements of one WidgetLayoutGroup. Given as a normalized 
	* number representing a percentage of one average column. Default value: 0.20 (20 %)*/
	SoSFFloat spacingWidth; 
	/** vertical space which should be left between elements of one WidgetLayoutGroup. Given as a normalized 
	number representing a percentage of one average row. Default value: 0.20 (20 %)*/
    SoSFFloat spacingDepth; 

	/** multiple field which contains the sizes of the rows propotional to each other.
	* Example: sizeOfRows [1, 2, 1] implicates a distribution of the rows of 1/4, 2/4, 1/4
	*/
	SoMFInt32 sizeOfRows;

	/** multiple field which contains the sizes of the columns propotional to each other.
	* Example: sizeOfCols [1, 2, 1] implicates a distribution of the columns of 1/4, 2/4, 1/4
	*/
	SoMFInt32 sizeOfCols;

	/** is triggered each time the layout has been updated. Is normally connected to SoStyleSwitch::updateStyle to apply the style
	* to the new created widgets */
	SoSFTrigger layoutUpdated;

	void buildPipLayout(); ///< calculates the new parameters (position, size) for all the elements and builds the layout 

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways); ///< sets up the connection of the field sensor

	virtual void doStyleAction(SoAction *action);   ///< apply style to pipLayout(s)

protected:
	/// each sub class should behave correctly on dimension changes
	virtual void resizeCB();


};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif