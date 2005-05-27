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
/** Header file for API definitions of SoPucStateString
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucStateString.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef SoPucStateString_H
#define SoPucStateString_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFString.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoPucStateBase.h"

/**
 Represents a PUC string state in the OIV scene graph. A STRING-state is normally represented as an edit box. Please note when useing string states
 that every change to the state value will be distributed to all clients. This is important when useing the string value in the scene graph, as each change
 (adding, removing, replacing a character) will trigger a field change!

  CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>activeIf </td><td>SoPucActiveIfNode</td><td>SoPucActiveIfNode</td><td>YES</td></tr>
  </table>


File Format / defaults:

@verbatim
SoPucStateString {
   	SoPucActiveIfNode	activeIf		NULL # conditions that enable / disable the state see SoPucElementBase::activeIf
	SoMFString			labels			[]   # labels of state see SoPucElementBase::labels
 	SoSFInt32			priority		0	 # priority of state see SoPucElementBase::priority

	SoMFString			valueLabels		[]	 # labels for values see SoPucStateBase::valueLabels
    SoSFEnum			access			ANY  # access level for clients see SoPucStateBase::access 

	SoSFString			value			""   # value of the state
	}
}
@endverbatim

See:
	SoPucStateBase::activeIf, SoPucElementBase::labels, SoPucStateBase::valueLabels

  @ingroup puc
 */
class PUC_API SoPucStateString : public SoPucStateBase
{
    SO_KIT_HEADER(SoPucStateString);

public:
	// special
	SoSFString value;

    static void initClass();
    SoPucStateString();

	virtual std::string getType() const;
	virtual std::string getStateValue();
	virtual bool setStateValue(std::string value_);

	virtual bool isEqual(const SbString &value) const;
	virtual bool isGreaterThan(const SbString &value) const;
	virtual bool isLessThan(const SbString &value) const;

protected:
    virtual ~SoPucStateString();
};

//----------------------------------------------------------------------------

#endif // SoPucStateString_H