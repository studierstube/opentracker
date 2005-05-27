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

 /* ======================================================================= */
/** Header file for API definitions of SoPucPipLayout
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>, Valerie Maquil
  *
  * $Id: SoPucPipLayout.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoPucPipLayout_H
#define SoPucPipLayout_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif


#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoNodeSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "PucPipLayout.h"
#include "SoBuildStbStyleAction.h"
#include "SoBuildPucStyleAction.h"
#include "SoBuildPucExtStyleAction.h"
#include "SoBuildPanelAction.h"

#include "..\Puc\SoPucServer.h"

#include <string>

//----------------------------------------------------------------------------

/**
 * provides means to create a Stb Application that is controllable with puc appliances. Therefore you need to specify
 * the appliance's functions through the puc states, and some layout hints. The user interface is then automatically generated
 * and allows you to control your appliance.
 * For the user interface you may specify the size, the style that should be used to generate the layout, and properties relative
 * to the chosen style (units, columns).

  CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>geometry </td><td>SoSeparator</td><td>SoSeparator</td><td>NO</td></tr>
  <tr> <td>pucAppliances </td><td>SoPucServer</td><td>SoPucServer</td><td>YES</td></tr>
  </table>



File Format / defaults:

@verbatim
SoPucPipLayout {
	SoSFFloat width				1.0
	SoSFFloat height			1.0
	SoSFFloat depth				1.0
	SoSFInt32 units				5  #only for STB style
	SoSFInt32 columns			2  #only for PUCEXT style
    SoSFEnum style				PUC

}
@endverbatim


  @ingroup puc
 */

class PUC_PIP_LAYOUT_API SoPucPipLayout : public SoLayoutKit
{
    SO_KIT_HEADER(SoPucPipLayout);
	/// if puc states are defined in pucAppliances this node will be overwritten, otherwise you can define your own layout on this place
//	SO_KIT_CATALOG_ENTRY_HEADER(geometry);
	/// this node defines the SoPucServer containing the devices with all the puc states
	SO_KIT_CATALOG_ENTRY_HEADER(pucAppliances);

public:
//	SoSFFloat width;	///<specifies the width of the region which should be used for the user interface
//	SoSFFloat height;	///<specifies the height of the region which should be used for the user interface
//	SoSFFloat depth;	///<specifies the depth of the region which should be used for the user interface
	/**This field is only relevant for the STB style. Here you can specify how many units should be in one row.
	A push button or a toggle button corresponds to one unit, a slider or a list box corresponds to two of them. Default value: 2*/
	SoSFInt32 units;
	/**This field is only relevant for the PUCEXT style. It specifies on how many columns the widgets should be distributed.
	Default value: 5 */
	SoSFInt32 columns;

	/** is triggered each time the layout has been updated. Is normally connected to SoStyleSwitch::updateStyle to apply the style
	* to the new created widgets */
	SoSFTrigger layoutUpdated;

	enum Style
	{
		/**The puc style creates one row for each widget and places the label on his left (see SoBuildPucStyleAction) */
		PUC = 0,
		/**The studierstube style puts the widgets next to each other in the same row until the row is full, and creates then a new one
		(see SoBuildStbStyleAction). The field units allows you to specify how many widgets should be placed in one row.*/
		STB = 1,
		/**The puc extended style represents an extended version of the above PUC style (see SoBuildPucExtStyleAction). The field
		columns allows you to specify on how many columns the widgets should be layed out.*/
		PUCEXT = 2,
	} ;
	///defines which style should be used to build the layout
    SoSFEnum style;
	/** Inventor class initialization */
    static void initClass();
	///constructor
    SoPucPipLayout();

	///sets the connections of the sensors
	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

	virtual void doStyleAction(SoAction *action);

protected:
	///starts the SoBuildPanelAction
	void doLayout();
	///function that is called when the "pucAppliances"-part is set. Calls doLayout().
	static void nodeSensorCB(void *data, SoSensor *sen);
	///destructor
	virtual ~SoPucPipLayout();

	virtual void resizeCB();						///< each sub class should behave correctly on dimension changes

	///sensor detecting changes in the "pucAppliances"-part
	SoNodeSensor nodeSensor;
};

//----------------------------------------------------------------------------

#endif // SoPucPipLayout_H