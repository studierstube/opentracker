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
/** Header file for API definitions of SoPucStateInt
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucStateInt.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoPucStateInt_H
#define SoPucStateInt_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFint32.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoPucStateBase.h"
/**
 Represents a PUC integer state in the OIV scene graph. A INT-state is normally represented as an edit box, 
 if SoPucStateInt::min, SoPucStateInt::max and SoPucStateInt::incr are specified
 a slidebar can be created (on devices that have this widget implemented).

 Please note that setting either SoPucStateInt::min or SoPucStateInt::max will
 result in checking the boundaries when setting the values in the scene graph (over field connections). For example setting 
 SoPucStateInt::min to 2 will result in a invalid definition as SoPucStateInt::max will be still 0 (min 2 , max 0 -> no values fit).
 To avoid unwanted features please allways set both fields. If both min and max have the same value, boundary checks are omitted.

  CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>activeIf </td><td>SoPucActiveIfNode</td><td>SoPucActiveIfNode</td><td>YES</td></tr>
  </table>


File Format / defaults:

@verbatim
SoPucStateInt {
   	SoPucActiveIfNode	activeIf		NULL # conditions that enable / disable the state see SoPucElementBase::activeIf
	SoMFString			labels			[]   # labels of state see SoPucElementBase::labels
 	SoSFInt32			priority		0	 # priority of state see SoPucElementBase::priority

	SoMFString			valueLabels		[]	 # labels for values see SoPucStateBase::valueLabels
    SoSFEnum			access			ANY  # access level for clients see SoPucStateBase::access 

	SoSFInt32			min				0    # minimum that the state can be set to
    SoSFInt32			max				0    # maximum that the state can be set to
    SoSFInt32			incr			1    # increase steps of value (used when generating a scrollbar)
    SoSFInt32			value			0    # value of the state
	}
@endverbatim

See:
	SoPucStateBase::activeIf, SoPucElementBase::labels, SoPucStateBase::valueLabels


  @ingroup puc
 */

class PUC_API SoPucStateInt : public SoPucStateBase
{
    SO_KIT_HEADER(SoPucStateInt);

public:
	// special
	SoSFInt32  min;
	SoSFInt32  max;
	SoSFInt32  incr;
	SoSFInt32  value;

    static void initClass();
    SoPucStateInt();

	virtual std::string getType() const;
	virtual std::string getStateValue();
	virtual bool setStateValue(std::string value_);

	virtual bool isEqual(const SbString &value) const;
	virtual bool isGreaterThan(const SbString &value) const;
	virtual bool isLessThan(const SbString &value) const;
protected:
    virtual ~SoPucStateInt();
	virtual void valueSetCallback(SoSensor *sensor);
};

//----------------------------------------------------------------------------

#endif // SoPucStateInt_H