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
/** Header file for API definitions for the application dll of the Invisible Person
  * application.
  of ifnde  *
  * $Id: SoPucStateEnumerated.h 3823 2004-10-05 12:27:08Z tomp $
  * @file                                                                   */
 /* ======================================================================= */



#ifndef SoPucStateEnumerated_H
#define SoPucStateEnumerated_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFInt32.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoPucStateBase.h"

/**
 Represents a PUC enumerated state in the OIV scene graph. An ENUMERATED state is normally represented as a combo box. Note that the
 SoPucStateBase::valueLabels is mandatory for this state. <b>A big difference to C++ : Value starts with 1!! </b> So if you want to use
 SoPucStateEnumerated as input for a SoSwitch be sure to add an empty child at the begin, to compensate SoPucStateEnumerated starting at 1.

 CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>activeIf </td><td>SoPucActiveIfNode</td><td>SoPucActiveIfNode</td><td>YES</td></tr>
  </table>


File Format / defaults:

@verbatim
SoPucStateEnumerated {
	SoPucActiveIfNode	activeIf		NULL # conditions that enable / disable the state see SoPucElementBase::activeIf
	SoMFString			labels			[]   # labels of state see SoPucElementBase::labels
 	SoSFInt32			priority		0	 # priority of state see SoPucElementBase::priority

	SoMFString			valueLabels		[]	 # labels for values mandatory for this state !! see SoPucStateBase::valueLabels
    SoSFEnum			access			ANY  # access level for clients see SoPucStateBase::access

	SoSFInt32			value			0    # value of the state (0 is not valid, as value starts with 1)
 }

@endverbatim

See:
	SoPucStateBase::activeIf, SoPucElementBase::labels, SoPucStateBase::valueLabels


  @ingroup puc
 */

class PUC_API SoPucStateEnumerated : public SoPucStateBase
{
    SO_KIT_HEADER(SoPucStateEnumerated);

public:
	// special
	SoSFInt32	value;

    static void initClass();
    SoPucStateEnumerated();

	virtual std::string getType() const;
	virtual std::string getStateValue();
	virtual bool setStateValue(std::string value_);

	virtual bool isEqual(const SbString &value) const;
	virtual bool isGreaterThan(const SbString &value) const;
	virtual bool isLessThan(const SbString &value) const;

protected:
	virtual void valueSetCallback(SoSensor *sensor);

    virtual ~SoPucStateEnumerated();
};

//----------------------------------------------------------------------------

#endif // SoPucStateEnumerated_H