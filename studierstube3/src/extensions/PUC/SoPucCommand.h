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
/** Header file for API definitions of SoPucCommand
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucCommand.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef SoPucCommand_H
#define SoPucCommand_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/sensors/SoFieldSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoPucElementBase.h"

#include <string>

class SoPucDevice;
/**
 Represents a PUC command in the OIV scene graph. A COMMAND is normally represented as a push button. Note that there is no value. Also setting the trigger (fire the trigger)
 in the scene graph will not be visible at the client side. This element allows communication from the client to the server only.

 CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>activeIf </td><td>SoPucActiveIfNode</td><td>SoPucActiveIfNode</td><td>YES</td></tr>
  </table>


File Format / defaults:

@verbatim
SoPucCommand {
	SoSFTrigger			trigger				 # field that can be connected in the scene graph, fires when client activates the command
 	SoPucActiveIfNode	activeIf		NULL # see SoPucStateBase::activeIf
	SoMFString			labels			[]   # see SoPucElementBase::labels
   }
}
@endverbatim

See:
	SoPucStateBase::activeIf, SoPucElementBase::labels, SoPucStateBase::valueLabels


  @ingroup puc
 */


class PUC_API SoPucCommand : public SoPucElementBase
{
    SO_KIT_HEADER(SoPucCommand);

public:

	SoSFTrigger trigger;
	SoSFTrigger triggerIn;

    static void initClass();
    SoPucCommand();
	virtual std::string getElementName() const;

	std::string getCommandName() const;
	void fireAtWill();

protected:
    virtual ~SoPucCommand();

	static void triggerInCB(void *data, SoSensor *sensor);
	SoFieldSensor triggerSensor;	///< sensor used by to check wether triggerIn has been fired
	bool fireTrigger;

	/** */
	friend class SoPucDevice;
};

//----------------------------------------------------------------------------

#endif // STBPUCVALUEGROUP_H